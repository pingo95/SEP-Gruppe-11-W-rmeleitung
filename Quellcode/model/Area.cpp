#include "Area.h"

model::Area::Area(QVector<double> xKoords, QVector<double> yKoords, double value):
    xKoords(xKoords), yKoords(yKoords), value(value)
{

}

bool model::Area::validateArea(QVector<double> const & xKoords,
                                      QVector<double> const & yKoords)
{

}

void model::Area::getConvexRectangle(double &xMin, double &xMax,
                                     double &yMin, double &yMax) const
{

}

void model::Area::getPoints(QVector<double> const & xKoords,
                            QVector<double> const & yKoords) const
{
//    xKoords = this->xKoords;
//    yKoords = this->yKoords;
}

double model::Area::getValue() const
{
    return this->value;
}

bool model::Area::insidePoint(double const xKoord, double const yKoord) const
{

}

void model::Area::setValue(double const value)
{
    this->value = value;
}

double model::Area::det(double const pX, double const pY,
                        double const qX, double const qY) const
{

}

double model::Area::direction(double const pX, double const pY,
                              double const qX, double const qY,
                              double const xX, double const xY) const
{

}

bool model::Area::onSegment(double const pX, double const pY,
                            double const qX, double const qY,
                            double const xX, double const xY) const
{

}

bool model::Area::segIntersect(double const pX, double const pY,
                               double const qX, double const qY,
                               double const rX, double const rY,
                               double const sX, double const sY) const
{

}
