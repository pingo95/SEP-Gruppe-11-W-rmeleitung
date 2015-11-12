#ifndef TESTERDCO_H
#define TESTERDCO_H
#include <QTextEdit>

namespace model {

    class TesterDCO
    {
    public:
        TesterDCO();
        ~TesterDCO();

        QWidget * constantLenghtTest();
        QWidget * GoldsteinArmijoTest();
    private:
        QTextEdit * output;
    };

}


#endif // TESTERDCO_H
