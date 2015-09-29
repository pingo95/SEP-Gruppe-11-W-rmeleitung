#ifndef AREA_H
#define AREA_H
#include <QVector>
#include <QString>
#include <QMap>
#include <QTextStream>

/*!
 * \brief Enthält die Modell-Klasse des MVC und die für die
 * Datenhaltung und Simulation notwendigen Klassen
 */
namespace model {
    /*!
     * \brief Die Klasse Area implementiert die geometrischen Gebiete die als Wärmequellen-
     * oder Temperaturleitkoeffizienten-Gebiete verwendet werden.
     *
     * Ein Gebiet besteht dabei jeweils aus:
     *      - einem Typ, d.h. entweder
     *          - Wärmequelle oder
     *          - Temperaturleitkoeffizient
     *      - einer (pro Typ jeweils) eindeutigen ID
     *      - einem Gebietswert
     *      - mindestens vier Punkten mit jeweils x- und y-Koordinate.
     *
     * Die Punkte werden dabei so interpretiert, dass jeweils zwei aufeinander folgende
     * Punkte mit einer geraden Linie verbunden werden und diese Linien die Umrandung des
     * Gebietes bilden.
     *
     * Damit ein Gebiet gültig ist muss es einfach wegzusammenhängend sein, d.h. im einzelnen:
     *      - muss der erste Punkt auch der letzte Punkt des Gebietes sein
     *      - es dürfen sich keine zwei Kanten schneiden
     *      - es dürfen keine Punkte (außer dem ersten) oder Kanten doppelt vorkommen.
     *
     * Die Klasse stellt folgende Funktionalitäten zur Verfügung:
     *      - eine statische Funktion, um die Gültigkeit eines Gebietes zu überprüfen
     *      - eine statische Funktion, um die IDs aller Typen zurückzusetzten
     *      - Getter-Funktionen für die ID, Punkte und den Wert eines Gebietes
     *      - eine Setter-Funktion für den Wert des Gebietes
     *      - eine Funktion, die ein das Gebiet umfassendes Rechteck zurückgibt
     *      - eine Funktion, die überprüft, ob ein übergebener Punkt in dem Gebiet liegt.
     *
     * Die Algorithmen, die intern zum Überprüfen genutzt werden, ob zwei Kanten sich
     * schneiden oder ein Punkt auf einer Kante liegt, stammen aus der Vorlesung
     * 'Datenstrukturen und Algorithmen' der RWTH Aachen, siehe auch:
     *      - <http://moves.rwth-aachen.de/teaching/ss-15/dsal/>
     *
     * Der Algorithmus, zum Testen, ob ein Punkt in einem Gebiet liegt, ist ein
     * Punkt-im-Polygon-Test nach Jordan in einer Strahl-Methode-Variante, inspiriert
     * durch:
     *      - <https://de.wikipedia.org/wiki/Punkt-in-Polygon-Test_nach_Jordan>
     *      - <http://rw7.de/ralf/inffaq/polygon.html>
     *
     * \see SimulationSetup::AreaType
     */
    class Area
    {
    //Funktionen:
    public:
        /*!
         * \brief Konstruktor für die Klasse Area.
         * \param xKoords x-Werte der Punkte des neuen Gebietes
         * \param yKoords y-Werte der Punkte des neuen Gebietes
         * \param value Wert des neuen Gebietes
         * \param type Typ des neuen Gebietes
         *
         * Erstellt ein neues Gebiet definiert durch die übergebenen Punkte. Es findet
         * keine Überprüfung der Gültigkeit statt, dies sollte vorher durch Aufruf der
         * statischen Funktion validateArea geschehen. Das Gebiet bekommt automatisch
         * die nächste freie ID für den Typ des Gebietes zugewiesen.
         * \see validateArea
         */
        Area(QVector<double> const & xKoords,
             QVector<double> const & yKoords,
             double value, int const type);
        /*!
         * \brief Copy-Konstruktor der Klasse Area.
         * \param rhs zu kopierendes Gebiet
         *
         * Beim Kopieren wird dem neuen Gebiet keine neue freie ID zugewiesen, sondern
         * die des übergebenen Gebietes, da dieser Konstruktor zum Erstellen von temporären
         * Kopien gedacht ist, damit während einer Simulation die Gebiete weiter verändert
         * werden können.
         * \see SimulationWorker::startSimulationSlot
         */
        explicit Area(Area const &rhs);
//        ~Area();
        /*!
         * \brief resetIDs setzt die idCounter für alle Gebietstypen zurück.
         *
         * Implementiert für das Zurücksetzen der gesamten Simulationseinstellungen.
         * \see Model::resetSetup
         */
        static void resetIDs();
        /*!
         * \brief validateArea überprüft, ob die übergebenen Punkte ein gültiges Gebiet
         * ergeben würden.
         * \param xKoords x-Werte der Punkte
         * \param yKoords y-Werte der Punkte
         * \return Gültigkeit des möglichen Gebietes
         *
         * Sollte vor Aufruf des Konstruktors benutzt werden, um die Gültigkeit des neuen
         * Gebietes sicherzustellen.
         *
         * Damit ein Gebiet gültig ist muss es einfach wegzusammenhängend sein, d.h. im einzelnen:
         *      - muss der erste Punkt auch der letzte Punkt des Gebietes sein
         *      - es dürfen sich keine zwei Kanten schneiden
         *      - es dürfen keine Punkte (außer dem ersten) oder Kanten doppelt vorkommen.
         * \see Area::Area
         */
        static bool validateArea(QVector<double> const & xKoords,
                                 QVector<double> const & yKoords);

        /*!
         * \brief Getter-Funktion für die Id des Gebietes.
         * \return die Gebiet-Id
         */
        int getID() const;
        /*!
         * \brief Getter-Funktion für die Punkte des Gebietes (zum Visualisieren).
         * \param xKoords x-Werte der Punkte
         * \param yKoords y-Werte der Punkte
         *
         * Die Punkte werden dabei in den per Referenz übergebenen Vektoren gespeichert.
         */
        void getPoints(QVector<double> & xKoords,
                       QVector<double> & yKoords) const;
        /*!
         * \brief getTransitiveRectangle gibt ein Rechteck zurück, das das Gebiet umschließt.
         * \param xMin minimaler x-Wert des Rechtecks
         * \param xMax maximaler x-Wert des Rechtecks
         * \param yMin minimaler y-Wert des Rechtecks
         * \param yMax maximaler y-Wert des Rechtecks
         *
         * Diese Funktion dient dazu bei der Umrechnung der Gebiete auf die Gitterpunkte
         * während einer Simulation die zu überprüfenden Punkte einzugrenzen, bevor diese
         * einzelnd mit insidePoint überrpüft werden.
         *
         * Die Punkte werden dabei in den per Referenz übergebenen Variablen gespeichert.
         * \see insidePoint, SimulationWorker::startSimulationSlot
         */
        void getTransitiveRectangle(double & xMin, double & xMax,
                                    double & yMin, double & yMax) const;
        /*!
         * \brief Getter-Funktion für den Gebietswert
         * \return der Gebietswert
         * \see setValue
         */
        double getValue() const;
        /*!
         * \brief insidePoint überprüft, ob der übergebene Punkt in dem Gebiet liegt
         * \param xKoord x-Wert des zu überprüfenden Punktes
         * \param yKoord y-Wert des zu überprüfenden Punktes
         * \return True falls der Punkt im Gebiet liegt, sonst false
         *
         * Der Test wird mittels des Punkt-im-Polygon-Test nach Jordan in einer
         * Strahl-Methode-Variante durchgeführt.
         *
         * Der Test wird benötigt, um die Gebiete während einer Simulation auf die
         * Gitterpunkte umzurechnen, zuerst sollten die möglichen Punkte durch
         * getTransitiveRectangle eingegrenzt werden.
         *
         * \see getTransitiveRectangle, SimulationWorker::startSimulationSlot
         */
        bool insidePoint(double const xKoord, double const yKoord) const;
        /*!
         * \brief Setter-Funktion für den Gebietswert
         * \param value der neue Gebietswert
         * \see getValue
         */
        void setValue(double const value);

    private:
        explicit Area();
        static double det(double const pX, double const pY,
                          double const qX, double const qY);
        // Richtung des Knicks zwischen den Stracken pq und qr:
        static double direction(double const pX, double const pY,
                                double const qX, double const qY,
                                double const rX, double const rY);
        // Testet ob x auf der Stracke pq liegt:
        static bool onLine(double const pX, double const pY,
                           double const qX, double const qY,
                           double const xX, double const xY);
        // Vorbedingung: x liegt auf (der Verlängerung von) der Strecke pq
        // Testet, ob x auch zwischen p und q liegt:
        static bool onSegment(double const pX, double const pY,
                              double const qX, double const qY,
                              double const xX, double const xY);
        // Testet, ob die Strecken pq und rs sich schneiden:
        static bool segIntersect(double const pX, double const pY,
                                 double const qX, double const qY,
                                 double const rX, double const rY,
                                 double const sX, double const sY);

    //Attribute:
    private:
        static QMap<int,int> idCounters;

        int const id;
        int const type;
        double value;
        QVector<double> const xKoords;
        QVector<double> const yKoords;

        /*!
         *  \brief Zugriffsrechte für TestKlasse
         */
        friend class TesterArea;
        friend QTextStream & operator<<(QTextStream & out, Area & area);
    };

    /*!
     * \brief Überladen des operator << für QTextStream und Area.
     * \param out QTextStream, in den die Einstellungen geschrieben wird
     * \param area auszugebenes Gebiet
     * \return QTextStream, in den die Einstellungen geschrieben wird
     *
     * Dieser Operator wurde für das Speichern von Simulationseinstellungen überladen.
     */
    QTextStream & operator<<(QTextStream & out, Area & area);
}
#endif // AREA_H
