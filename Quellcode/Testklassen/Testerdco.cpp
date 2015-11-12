#include "Testerdco.h"
#include "../model/Simulationworker.h"
#include "../algorithms/Crs.hpp"
#include "../algorithms/Solver.hpp"
#include "../algorithms/Iterativesolver.hpp"
#include "../algorithms/Jacobi.hpp"
#include "../algorithms/Gaussseidel.hpp"
#include "../algorithms/LU.hpp"
#include "../algorithms/Intmethod.hpp"
#include "../algorithms/Impeuler.hpp"
#include "../algorithms/Cranknicolson.hpp"
#include <vector>
#include <iostream>

QVector<AD_TYPE> x0_ref;
QVector<AD_TYPE> x0;
double ** observations;
long obsSize, optimizationN;
long const maxError = 1e-10, maxIt = 1;

QVector<AD_TYPE>* F(QVector<AD_TYPE> & x, QVector<AD_TYPE> * res)
{
    for(int i = 0; i < x.size(); ++i)
        (*res)[i] = x[i] * x[i];
    return res;
}

model::TesterDCO::TesterDCO()
{
    obsSize = 3;
    optimizationN = 9;
    x0_ref.resize(9);
    x0.resize(9);
    for(int i = 1; i < 10; ++i)
    {
        x0_ref[i-1] = i;
        x0[i-1] = i+.2;
    }
    observations = new double*[3];
    for(int i = 0; i < 3; ++i)
    {
        observations[i] = new double[3];
        for(int j = 0; j < 3; ++j)
            observations[i][j] = (i*3+j+1)*(i*3+j+1);
    }
}

model::TesterDCO::~TesterDCO()
{
    for(int i = 0; i < 3; ++i)
        delete [] observations[i];
    delete [] observations;
}

QWidget * model::TesterDCO::constantLenghtTest()
{
    output = new QTextEdit();
    output->setReadOnly(true);
    output->setMinimumSize(500,500);
    QString logText = "Optimierung mittels eines Steepest Descent Algorithms mit konstanter "
                      "Schrittweite:\n"
                      "Testfunktion: Die Quadratfunktion\n"
                      "Zielwerte der Parameter: {1,2,3,4,5,6,7,8,9}\n"
                      "Startwerte der Parameter: {1.2,2.2,3.2,4.2,5.2,6.2,7.2,8.2,9.2}\n"
                      "Zielwerte der Funktionsergebnisse: {1,4,9,16,25,36,49,64,81}\n\n";

    std::vector<AD_TYPE> optimizedCsAD(x0.toStdVector());
    int count = 0;
    AD_MODE::global_tape = AD_MODE::tape_t::create();
    AD_TYPE norm;
    QVector<AD_TYPE> * result = new QVector<AD_TYPE>;
    result->resize(9);
    do
    {
        AD_MODE::global_tape->register_variable(optimizedCsAD.data(),optimizationN);

        QVector<AD_TYPE> tmpCs (QVector<AD_TYPE>::fromStdVector(optimizedCsAD));
        result = F(tmpCs,result);
        AD_TYPE J = 0;
        for(int i = 0; i < obsSize; ++i) {
            for(int j = 0; j < obsSize; ++j) {
                J += ((*result)[i*obsSize + j] - observations[i][j])
                        *((*result)[i*obsSize + j] - observations[i][j]);
            }
        }
        J *= 1./((obsSize-1)*(obsSize-1));

        //std::cout << "J" << count+1 << ": " << J << std::endl<< std::endl;

        dco::derivative(J) = 1;
        AD_MODE::global_tape->interpret_adjoint();

        QVector<AD_TYPE> grad(QVector<AD_TYPE>::fromStdVector(optimizedCsAD));
        for(int i = 0; i < optimizationN; ++i)
            grad[i] = dco::derivative(optimizedCsAD[i]);

        AD_TYPE s = 1e-4;
        for(int i = 0; i < obsSize; ++i)
            for(int j = 0; j < obsSize; ++j)
                optimizedCsAD[i*obsSize+j]  -= s * grad[i*obsSize+j];

        norm = algorithms::norm2(grad);

        AD_MODE::global_tape->reset();

        logText += QString::number(count+1) + ". Schritt abgeschlossen:\nResiduum J: "
                + QString::number(dco::value(J)) + "\nNorm des Gradienten: "
                + QString::number(dco::value(norm)) + "\nAktuelle Parameter: {";
        for(int i = 0; i < 9; ++i)
            logText += QString::number(dco::value(optimizedCsAD[i])) + ",";
        logText += "}\n\n";
        ++count;
    }
    while(count < maxIt && norm-maxError > 0);
    AD_MODE::tape_t::remove(AD_MODE::global_tape);
    delete result;
    output->setText(logText);
    return output;
}

QWidget * model::TesterDCO::GoldsteinArmijoTest()
{
    output = new QTextEdit();
    output->setReadOnly(true);
    output->setMinimumSize(500,500);
    QString logText = "Optimierung mittels eines Steepest Descent Algorithms mit einer "
                      "Schrittweite nach der Goldstein-Armijo-Regel:\n"
                      "Testfunktion: Die Quadratfunktion\n"
                      "Zielwerte der Parameter: {1,2,3,4,5,6,7,8,9}\n"
                      "Startwerte der Parameter: {1.2,2.2,3.2,4.2,5.2,6.2,7.2,8.2,9.2}\n"
                      "Zielwerte der Funktionsergebnisse: {1,4,9,16,25,36,49,64,81}\n\n";

    std::vector<AD_TYPE> optimizedCsAD(x0.toStdVector());
    int count = 0;
    AD_MODE::global_tape = AD_MODE::tape_t::create();
    AD_TYPE norm;
    QVector<AD_TYPE> * result = new QVector<AD_TYPE>;
    result->resize(9);
    AD_TYPE sigma = 0.25, t = 1, tU = 0, tO, gradPhi = 0;
    do
    {
        AD_MODE::global_tape->register_variable(optimizedCsAD.data(),optimizationN);

        QVector<AD_TYPE> tmpCs (QVector<AD_TYPE>::fromStdVector(optimizedCsAD));
        result = F(tmpCs,result);
        AD_TYPE J = 0;
        for(int i = 0; i < obsSize; ++i) {
            for(int j = 0; j < obsSize; ++j) {
                J += ((*result)[i*obsSize + j] - observations[i][j])
                        *((*result)[i*obsSize + j] - observations[i][j]);
            }
        }
        J *= 1./((obsSize-1)*(obsSize-1));

        //std::cout << "J" << count+1 << ": " << J << std::endl<< std::endl;

        dco::derivative(J) = 1;
        AD_MODE::global_tape->interpret_adjoint();

        QVector<AD_TYPE> grad(QVector<AD_TYPE>::fromStdVector(optimizedCsAD));
        gradPhi = 0;
        for(int i = 0; i < optimizationN; ++i)
        {
            grad[i] = dco::derivative(optimizedCsAD[i]);
            gradPhi -= grad[i]*grad[i];
        }
        gradPhi *= 1./((obsSize-1)*(obsSize-1));
        std::cout << "gradPhi: " << gradPhi << std::endl;
        tU = 0;
        tO = t;
        std::cout << "Init t Bounds: tU: " << tU << " tO:" << tO << std::endl;
        AD_TYPE jT, jU, jO;
        bool flag = true;
        do
        {
            jU = J + (1-sigma) * tO * gradPhi;

            AD_MODE::global_tape->reset();
            for(int i = 0; i < optimizationN; ++i)
                tmpCs[i] = optimizedCsAD[i] + tO * grad[i];
            result = F(tmpCs,result);

            jT = 0;
            for(int i = 0; i < obsSize; ++i) {
                for(int j = 0; j < obsSize; ++j) {
                    jT += ((*result)[i*obsSize + j] - observations[i][j])
                            *((*result)[i*obsSize + j] - observations[i][j]);
                }
            }
            jT *= 1./((obsSize-1)*(obsSize-1));

            if(jT < jU)
            {
                tU = tO;
                tO *= 2;
            }
            else
                flag = false;
            std::cout << "jU: " << jU << " jT: " << jT << " tU: " << tU << " tO: " << tO << std::endl;
        }
        while(flag);

        jO = J + sigma * tO * gradPhi;
        if(jT > jO)
            flag = true;
        else
            t = tO;
        std::cout << "jU: " << jU << " jT: " << jT << " jO: " << jO << " tU: " << tU << " tO: " << tO << " t: " << t << std::endl;

        for(int i = 0; i < 20; ++i)
        {
            t = 0.5*(tU + tO);
            jU = J + (1-sigma) * t * gradPhi;
            jO = J + sigma * t * gradPhi;

            AD_MODE::global_tape->reset();
            for(int i = 0; i < optimizationN; ++i)
                tmpCs[i] = optimizedCsAD[i] - t * grad[i];
            result = F(tmpCs,result);

            jT = 0;
            for(int i = 0; i < obsSize; ++i) {
                for(int j = 0; j < obsSize; ++j) {
                    jT += ((*result)[i*obsSize + j] - observations[i][j])
                            *((*result)[i*obsSize + j] - observations[i][j]);
                }
            }
            jT *= 1./((obsSize-1)*(obsSize-1));

            if(jT < jU)
                tU = t;
            if(jT > jO)
                tO = t;
            std::cout << "jU: " << jU << " jT: " << jT << " jO: " << jO << " tU: " << tU << " tO: " << tO << " t: " << t << std::endl;
            if(jU <= jT && jT <= jO)
                flag = false;
        }

        std::cout << "jT" << count+1 << ": " << jT << std::endl << std::endl;
        for(int i = 0; i < optimizationN; ++i)
            optimizedCsAD[i]  -= t * grad[i];

        norm = algorithms::norm2(grad);

        logText += QString::number(count+1) + ". Schritt abgeschlossen:\nResiduum J: "
                + QString::number(dco::value(J)) + "\nNorm des Gradienten: "
                + QString::number(dco::value(norm)) + "\nAktuelle Parameter: {";
        for(int i = 0; i < 9; ++i)
            logText += QString::number(dco::value(optimizedCsAD[i])) + ",";
        logText += "}\n\n";
        ++count;
    }
    while(count < maxIt && norm-maxError > 0);
    AD_MODE::tape_t::remove(AD_MODE::global_tape);
    delete result;
    output->setText(logText);
    return output;
}
