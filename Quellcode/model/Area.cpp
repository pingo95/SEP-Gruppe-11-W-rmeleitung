#include "Area.h"
QMap<int, int> model::Area::idCounters;

// Vorbedingung: xKoords und yKoords wurden vorher mit validateArea getestet
// Erstellt neues Gebiet:
model::Area::Area(QVector<double> const & xKoords,
                  QVector<double> const & yKoords, double value, int const type):
    id(idCounters.value(type,1)), type(type), value(value),
    xKoords(xKoords), yKoords(yKoords)
{
    if(idCounters.contains(type))
        ++idCounters[type];
    else
        idCounters.insert(type,2);
}

model::Area::Area(const Area & rhs) : id(++idCounters[rhs.type]), type(rhs.type),
    value(rhs.value), xKoords(rhs.xKoords), yKoords(rhs.yKoords)
{

}

model::Area::~Area(){
//    --idCounters[type];
}

// Testet, ob die Punkte in den Vektoren ein gültiges Gebiet ergeben würden
bool model::Area::validateArea(QVector<double> const & xKoords,
                               QVector<double> const & yKoords)
{
    // 1. Bedingung: gleich viele x und y Werte:
    if(xKoords.size() != yKoords.size()) return false;
    // 2. Bedingung: geschlossenes Gebiet:
    if(!((xKoords.first()==xKoords.last()) && (yKoords.first()==yKoords.last()))) return false;
    // 3. Bedinungen: mindestens 4 Punkte (inkl. doppeltem Erstem)
    if(xKoords.size() < 4) return false;
    // 4. Bedingung: keine sich schneidene Seiten
    // Die i-te Kante besteht aus den i-ten und (i+1)-ten Werten in xKoords und yKoords,
    // d.h. insgesamt xKoords.size()-1 Kanten
    int n = xKoords.size()-1;
    for(int i = 0; i < n; ++i)
        for(int j = 0; j < n; ++j)
        {
            // Überspringen der aktuellen Kante
            if(j==i) continue;
            // Nachbarkanten dürfen sich im gemeinsamen Punkt schneiden,
            // aber nicht überlagern
            // linker (vorheriger) Nachbar:
            if(j == (i + n-1) % n)
            {
                if(onLine(xKoords[j],yKoords[j],xKoords[i],yKoords[i],
                          xKoords[i+1],yKoords[i+1])
                     || onLine(xKoords[i],yKoords[i],xKoords[i+1],yKoords[i+1],
                               xKoords[j],yKoords[j])) return false;
                continue;
            }
            // rechte ( nachfolgender) Nachbar:
            if(j == ((i + 1) % n))
            {
                if(onLine(xKoords[i],yKoords[i],xKoords[i+1],yKoords[i+1],
                          xKoords[j+1],yKoords[j+1])
                     || onLine(xKoords[i+1],yKoords[i+1],xKoords[j+1],yKoords[j+1],
                               xKoords[i],yKoords[i])) return false;
                continue;
            }
            // Ansonsten normaler "Schnitt-Test":
            if(segIntersect(xKoords[i],yKoords[i],xKoords[i+1],yKoords[i+1],
                            xKoords[j],yKoords[j],xKoords[j+1],yKoords[j+1])) return false;
        }
    return true;
}

// Returned Rechteck, das parallel zur x und y Achse das Gebiet einschließt
void model::Area::getTransitiveRectangle(double &xMin, double &xMax,
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

int model::Area::getID() const
{
    return id;
}

double model::Area::getValue() const
{
    return this->value;
}

// Testet, ob der Punkt innerhalb des Gebietes liegt:
bool model::Area::insidePoint(double const xKoord, double const yKoord) const
{
    // 1. Testet, ob der Punkt auf einer Kante des Gebietes liegt:
    for(int i = 0; i < xKoords.size()-1; ++i){
        if(onLine(xKoords[i],yKoords[i],xKoords[i+1],yKoords[i+1],xKoord,yKoord))
            return true;
    }

    // Strahl-Methode(Jordan)
    // Erste Auswahl des Strahls mit Richtung (c,s) ausgehend von (xKoord,yKoord)
    // Annahme: Gebiete sind immer Teilmengen von [0,1] x [0,1] und xKoord,yKoord € [0,1]
    double dX = 2, dY = 1.5;
    // Der Strahl darf keine Ecke des Gebietes treffen
    bool hit;
    do
    {
        hit = false;
        // Daher überprüfen ob eine Ecke getroffen:
        for(int i = 0; i < xKoords.size()-1; ++i)
            if(dX*(yKoords[i]-yKoord) == dY*(xKoords[i]-xKoord))
            {
                hit = true;
                break;
            }
        if(hit)
        {
            // Falls ja, neuen zufälligen Strahl
            dX = (double) qrand() / RAND_MAX + 1; //Zufallszahl zwischen 1 und 2 damit der Strahl den Rand erreicht
            dY = (double) qrand() / RAND_MAX * 2 - 1;
        }
    }
    while(hit);

    // Wenn gültiger Strahl gefunden, zählen wie oft der Strahl die Kanten schneidet
    int count = 0;
    for(int i = 0; i < xKoords.size()-1; ++i)
        if(segIntersect(xKoords[i],yKoords[i],xKoords[i+1],yKoords[i+1],xKoord,yKoord,xKoord+dX,yKoord+dY))
            ++count;
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
    double d = det(qX-pX,qY-pY,rX-qX,rY-qY);
    d = qAbs(d) < 1e-10 ? 0 : d;
    return d;
}

//Testet ob x auf pq liegt:
bool model::Area::onLine(double const pX, double const pY,
                         double const qX, double const qY,
                         double const xX, double const xY)
{
    double d = direction(pX,pY,qX,qY,xX,xY);
    bool o = onSegment(pX,pY,qX,qY,xX,xY);
    return d == 0 && o;
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

