#ifndef AREA_H
#define AREA_H
#include <QVector>
#include <QString>
#include <QMap>

namespace model {

    class Area
    {
    //Funktionen:
    public:
        explicit Area(QVector<double> const & xKoords,
             QVector<double> const & yKoords, double value = 0., QString const type = "Undefiniert");
        ~Area();
        static int getCurrentMaxID(const QString type);
        static bool validateArea(QVector<double> const & xKoords,
                                 QVector<double> const & yKoords);

        void getConvexRectangle(double & xMin, double & xMax,
                                double & yMin, double & yMax) const;
        int getID() const;
        void getPoints(QVector<double> & xKoords,
                       QVector<double> & yKoords) const;
        double getValue() const;
        bool insidePoint(double const xKoord, double const yKoord) const;
        void setValue(double const value);

    private:
        explicit Area();
        static double det(double const pX, double const pY,
                          double const qX, double const qY);
        static double direction(double const pX, double const pY,
                                double const qX, double const qY,
                                double const rX, double const rY);
        static bool onLine(double const pX, double const pY,
                           double const qX, double const qY,
                           double const xX, double const xY);
        static bool onSegment(double const pX, double const pY,
                              double const qX, double const qY,
                              double const xX, double const xY);
        static bool segIntersect(double const pX, double const pY,
                                 double const qX, double const qY,
                                 double const rX, double const rY,
                                 double const sX, double const sY);

    //Attribute:
    private:
        explicit Area(Area const &);
        static QMap<QString,int> idCounters;

        int const id;
        QString const type;
        double value;
        QVector<double> const xKoords;
        QVector<double> const yKoords;
    };

}

#endif // AREA_H
