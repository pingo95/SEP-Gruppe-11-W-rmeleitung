#ifndef AREA_H
#define AREA_H
#include <QVector>

namespace model {

    class Area
    {
        QVector<double> const xKoords;
        QVector<double> const yKoords;
        double value;

    public:
        Area(QVector<double> xKoords, QVector<double> yKoords, double value);
        static bool validateArea(QVector<double> const & xKoords,
                                 QVector<double> const & yKoords);

        void getConvexRectangle(double & xMin,double & xMax,
                                double & yMin, double & yMax) const;
        void getPoints(QVector<double> const & xKoords,
                       QVector<double> const & yKoords) const;
        double getValue() const;
        bool insidePoint(double const xKoord, double const yKoord) const;
        void setValue(double const value);

    private:
        Area();

        double det(double const pX, double const pY,
                   double const qX, double const qY) const;
        double direction(double const pX, double const pY,
                         double const qX, double const qY,
                         double const xX, double const xY) const;
        bool onSegment(double const pX, double const pY,
                       double const qX, double const qY,
                       double const xX, double const xY) const;
        bool segIntersect(double const pX, double const pY,
                          double const qX, double const qY,
                          double const rX, double const rY,
                          double const sX, double const sY) const;

    };

}

#endif // AREA_H
