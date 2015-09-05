#include "Area.h"

//Vorbedingung: xKoords und yKoords wurden vorher mit validateArea getestet
// Erstellt neues Gebiet:
model::Area::Area(QVector<double> const & xKoords,
                  QVector<double> const & yKoords, double value):
    xKoords(xKoords), yKoords(yKoords), value(value)
{

}

// Testet, ob die Punkte in den Vektoren ein gültiges Gebiet ergeben würden
bool model::Area::validateArea(QVector<double> const & xKoords,
                               QVector<double> const & yKoords)
{
    // 1. Bedingung: gleich viele x und y Werte:
    if(xKoords.size() != yKoords.size()) return false;
    // 2. Bedingung: geschlossenes Gebiet:
    if(!((xKoords.first()==xKoords.last()) && (yKoords.first()==yKoords.last()))) return false;
    // 3. Bedingung: keine sich schneidene Seiten
    // Die i-te Kante besteht aus den i-ten und (i+1)-ten Werten in xKoords und yKoords,
    // d.h. insgesamt xKoords-1 Kanten
    for(int i = 0; i < xKoords.size()-1; ++i)
        for(int j = 0; j < yKoords.size()-1; ++j)
        {
            // Überspringen der aktuellen Kante
            if(j==i) continue;
            // Nachbarkanten dürfen sich im gemeinsamen Punkt schneiden,
            // aber nicht überlagern
            // linker (vorheriger) Nachbar:
            if(j-i == -1)
            {
                if(onLine(xKoords[i-1],yKoords[i-1],xKoords[i],yKoords[i],
                          xKoords[i+1],yKoords[i+1])
                     || onLine(xKoords[i],yKoords[i],xKoords[i+1],yKoords[i+1],
                               xKoords[i-1],yKoords[i-1])) return false;
            }
            // rechte ( nachfolgender) Nachbar:
            if(j-i == 1)
            {
                if(onLine(xKoords[i],yKoords[i],xKoords[i+1],yKoords[i+1],
                          xKoords[i+2],yKoords[i+2])
                     || onLine(xKoords[i+1],yKoords[i+1],xKoords[i+2],yKoords[i+2],
                               xKoords[i],yKoords[i])) return false;
            }
            // Ansonsten normaler "Schnitt-Test":
            if(segIntersect(xKoords[i],yKoords[i],xKoords[i+1],yKoords[i+1],
                            xKoords[j],yKoords[j],xKoords[j+1],yKoords[j+1])) return false;
        }
    return true;
}

// Returned Rechteck, das parallel zur x und y Achse das Gebiet einschließt
void model::Area::getConvexRectangle(double &xMin, double &xMax,
                                     double &yMin, double &yMax) const
{
    xMin = xKoords[0];
    xMax = xKoords[0];
    yMin = yKoords[0];
    yMax = yKoords[0];

    for(int i = 1; i < xKoords.size()-1; ++i)
    {
        if(xKoords[i] < xMin) xMin = xKoords[i];
        else if(xKoords[i] > xMax) xMax = xKoords[i];

        if(yKoords[i] < yMin) yMin = yKoords[i];
        else if(yKoords[i] > yMax) yMax = yKoords[i];
    }
}

void model::Area::getPoints(QVector<double> & xKoords,
                            QVector<double> & yKoords) const
{
    xKoords = this->xKoords;
    yKoords = this->yKoords;
}

double model::Area::getValue() const
{
    return this->value;
}

// Testet, ob der Punkt innerhalb des Gebietes liegt:
bool model::Area::insidePoint(double const xKoord, double const yKoord) const
{
    // 1. Testet, ob der Punkt auf einer Kante des Gebietes liegt:
    for(int i = 0; i < xKoords.size(); ++i){
        if(onLine(xKoords[i],yKoords[i],xKoords[i+1],yKoords[i+1],
                  xKoord,yKoord)) return true;
    }
    // 2. Strahl-Methode (Jordan) in einer Ignorier-Variante:
    bool ignore = false;
    int count = 0;
    // Annahme: Gebiete sind immer Teilmengen von [0,1] x [0,1]
    // Der Strahl geht vom Punkt aus parallel zur x-Achse bis zur rechten Begrenzung (x=1),
    // d.h. entspricht der Strecke xr
    // Die Gerade geht von der linken Begrenzung (x=0) parallel zur x-Achse durch den Punkt
    // bis zur rechten Begrenzung, d.h. entspricht der Strecke lr
    double lX = 0, lY = yKoord,
           rX = 1, rY = yKoord;
    for(int i = 1; i < xKoords.size(); ++i)
    {
        if(ignore)
        {
            if(onLine(lX,lY,rX,rY,xKoords[i],yKoords[i])) ignore = true;
            else
            {
                if(segIntersect(xKoords[i-1],yKoords[i-1],xKoords[i],yKoords[i],
                                lX,lY,rX,rY)) ++count;
                ignore = false;
            }
        }
        else
        {
            if(onLine(xKoord,yKoord,rX,rY,xKoords[i],yKoords[i])) ignore = true;
            else
            {
                if(segIntersect(xKoords[i-1],yKoords[i-1],xKoords[i],yKoords[i],
                                xKoord,yKoord,rX,rY)) ++count;
                ignore = false;
            }
        }
    }
    return count % 2 == 1;
}

void model::Area::setValue(double const value)
{
    this->value = value;
}

double model::Area::det(double const pX, double const pY,
                        double const qX, double const qY)
{
    return pX*qY - pY*qX;
}

// Richtung des Knicks zwischen pq und qr:
double model::Area::direction(double const pX, double const pY,
                              double const qX, double const qY,
                              double const rX, double const rY)
{
    return det(qX-pX,qY-pY,rX-qX,rY-qY);
}

//Testet ob x auf pq liegt:
bool model::Area::onLine(double const pX, double const pY,
                         double const qX, double const qY,
                         double const xX, double const xY)
{
    return direction(pX,pY,qX,qY,xX,xY) == 0 && onSegment(pX,pY,qX,qY,xX,xY);
}

// Vorbedingung: x liegt auf (der Verlängerung von) pq
// Testet, ob x auch zwischen p und q liegt:
bool model::Area::onSegment(double const pX, double const pY,
                            double const qX, double const qY,
                            double const xX, double const xY)
{
    double topRightX = pX > qX ? pX : qX;
    double topRightY = pY > qY ? pY : qY;
    double botLeftX  = pX < qX ? pX : qX;
    double botLeftY  = pY < qY ? pY : qY;
    return (xX <= topRightX) && (xY <= topRightY) &&
           (xX >= botLeftX)  && (xY >= botLeftY);
}

//Testet, ob pq und rs sich schneiden:
bool model::Area::segIntersect(double const pX, double const pY,
                               double const qX, double const qY,
                               double const rX, double const rY,
                               double const sX, double const sY)
{
    double d1 = direction(pX,pY,qX,qY,rX,rY),
           d2 = direction(pX,pY,qX,qY,sX,sY);
    // Testet, ob r bzw. s auf pq liegt:
    if(d1 == 0 && onSegment(pX,pY,qX,qY,rX,rY)) return true;
    if(d2 == 0 && onSegment(pX,pY,qX,qY,sX,sY)) return true;
    // Testet, ob r und s auf der selben Seite von pq liegen:
    if((d1 > 0 && d2 > 0) || (d1 < 0 && d2 < 0)) return false;

    double d3 = direction(rX,rY,sX,sY,pX,pY),
           d4 = direction(rX,rY,sX,sY,qX,qY);
    // Testet, ob p bzw. q auf rs liegt:
    if(d3 == 0 && onSegment(rX,rY,sX,sY,pX,pY)) return true;
    if(d4 == 0 && onSegment(rX,rY,sX,sY,qX,qY)) return true;
    // Testet, ob p und q auf der selben Seite von rs liegen:
    if((d3 > 0 && d4 > 0) || (d3 < 0 && d4 < 0)) return false;
    // Sonst:
    return true;
}

