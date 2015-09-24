#ifndef SOLVER_H
#define SOLVER_H

#include <QVector>

#include <math.h>
#include <cassert>
#include "Crs.h"

namespace algorithms {

    /*!
     * \brief Die abstrakte Basisklasse Solver stellt den Rumpf für Löser linearer Gleichungssysteme bereit
     *
     * lediglich die solve-Methode ist zu implementieren, diese soll die Lösung des LGS Ax=b berechnen/approximieren
     */
    template <class T>
    class Solver {
    //Funktionen:
    public:

        /*!
         * \brief Standardkonstruktor
         */
        Solver();

        /*!
         * \brief virtueller Destruktor
         */
        virtual ~Solver();

        virtual void decompose(CRS<T> const & matrix);
        virtual double getEps() const;
        virtual int getItCount() const;
        virtual int getMaxIt() const;
        virtual void setEps(double const eps);
        virtual void setMaxIt(int const maxIt);
        virtual void solve(QVector<T> & result, CRS<T> const & matrix, QVector<T> const & rhs)=0;

    //Attribute:
    protected:
        double eps;
        int itCount;
        int maxIt;
    };

    /*!
     *  \brief norm2 berechnet die 2Norm des übergebenen Vektors
     *  \param vec
     */
    template <class T>
    double norm2(QVector<T> const & vec);

    /*!
     *  \brief normInf berechnet die Maximumsnorm des übergebenen Vektors
     *  \param vec
     */
    template <class T>
    double normInf(QVector<T> const & vec);

}

#endif // SOLVER_H
