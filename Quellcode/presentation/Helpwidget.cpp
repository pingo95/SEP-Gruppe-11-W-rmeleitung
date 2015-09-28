#include "Helpwidget.h"

presentation::HelpWidget::HelpWidget(QWidget *parent) : QWidget(parent)
{
        //Labels
    topLabel = new QLabel("Dies ist der Hilfe-Tab. Hier finden Sie "
                                  "ausführliche Beschreibungen und Erklärungen"
                                  " zu den Funktionen der anderen Tabs.",this);
    topLabel->setWordWrap(true);
    //topLabel->setStyleSheet("font: 12pt;");

    configurationHelp = new QLabel("Im Konfigurations-Tab befinden sich drei weitere Tabs, "
                                   "in denen Sie die Einstellungen zur Simulation festlegen können.\n "
                                   "In den ersten beiden Tabs können Sie Gebiete der "
                                   "Temperaturleitkoeffizienten/Wärmequellen markieren und deren Werte eingeben. ",this);
    configurationHelp->setWordWrap(true);
    thermalDiffusivitiesHelp = new QLabel("Der Temperaturleitkoeffizienten-Tab und der Wärmequellen-Tab bietet Ihnen folgende Funktionalitäten:\n"
                                          "1. Ein Gebiet hinzufügen: Klicken Sie entweder mit der linken Maustaste an die gewünschte Stelle auf "
                                          "der Platte oder geben Sie die Koordinaten über die Felder 'x-Koordinate' und 'y-Koordinate' ein und "
                                          "bestätigen Sie mit 'Punkt hinzufügen'. Um die Eingabe eines Gebietes abzuschließen müssen Sie auf den "
                                          "Startpunkt des Gebietes klicken. \nEin Gebiet ist nur dann korrekt, wenn es einfach wegzusammenhängend ist, "
                                          "d.h. Kanten dürfen sich nicht schneiden und es muss abgeschlossen sein. Wurde ein Gebiet korrekt "
                                          "eingegeben können Sie den Temperaturleitkoeffizienten (in 1e-06 m^2/s) bzw den Wert der Wärmequelle zu "
                                          "diesem Gebiet eingeben.\n"
                                          "2. Einen Punkt rückgängig machen: Um während der Eingabe eines neuen Gebietes einen Punkt rückgängig "
                                          "zu machen, klicken Sie auf den Knopf 'Rückgängig'.\n"
                                          "3. Ein Gebiet abbrechen: Um das aktuell begonnene Gebiet zu löschen, klcken Sie auf den Knopf 'Gebiet "
                                          "abbrechen'.\n"
                                          "4. Alle Gebiete löschen: Um alle Gebiete zu löschen, klicken Sie auf den Knopf 'Alle Gebiete löschen'.\n"
                                          "5. Ein Gebiet löschen: Um ein beliebiges Gebiet zu löschen, wählen Sie nun auf der Platte das Gebiet aus, "
                                          "welches Sie löschen wollen und klicken Sie anschließend auf den Knopf 'Gebiet löschen'.\n"
                                          "6. Reihenfolge der Gebiet tauschen:"
                                          "Sie können die Reihenfolge der eingegebenen Gebiete in der Tabelle ändern. Hierfür muss mehr als ein Gebiet "
                                          "auf der Platte eingegeben worden sein. Klicken Sie entweder mit der linken Maustaste auf das gewünschte "
                                          "Gebiet auf der Platte oder klicken Sie auf die Zeile des entsprechenden Gebietes. Anschließend können Sie "
                                          "die Reihenfolge mit den Pfeiltasten neben der Tabelle ändern. Mit einem Klick auf den einfachen Pfeil wird "
                                          "das ausgewählte Gebiet entweder um eine Position nach oben oder nach unten in der Tabelle verschoben. "
                                          "Mit einem Klick auf den doppelten Pfeil wird das ausgewählte Gebiet entweder an den Anfang oder ans Ende "
                                          "der Tablle verschoben. Das Gebiet welches in der Tablle an letzter Position steht, wird auf der Platte "
                                          "als oberstes Gebiet angezeigt.\n"
                                          "7. Wert eines Gebietes ändern: Um den Temperaturleitkoeffizienten bzw den Wert der Wärmequelle eines "
                                          "Gebietes zu ändern, klicken Sie mit einem Doppelklick auf den Wert in der Tabelle in der Spalte 'Werte'. "
                                          "Anschließend wird das ausgewählte Gebiet auf der Platte markiert und Sie können den neuen Wert "
                                          "eingeben. ",this);
    thermalDiffusivitiesHelp->setWordWrap(true);

    iBVsHelp = new QLabel("Der dritte Tab ist zur Einstellung der Anfangs- und Randwerte.\n"
                          "Der IBV-Tab bietet Ihnen folgende Funktionalitäten:\n"
                          "Um die Anfangswerte und Randwerte zu ändern, ändern Sie den gewünschten Wert in den Feldern "
                          "'Anfangswert eingeben', 'unteren Randwert eingeben', 'linken Randwert eingeben', 'rechten Randwert eingeben'"
                          " oder 'oberen Randwert eingeben'.",this);
    iBVsHelp->setWordWrap(true);

    simulationHelp = new QLabel("Der Simulations-Tab bietet Ihnen folgende Funktionalitäten:\n"
                                "1. Integrationsmethode ändern: Sie können die Integrationsmethode ändern, indem Sie auf die Combo-Box "
                                "klicken und dort die gewünschte Integrationsmethode auswählen.\n"
                                "2. Zeitschritte ändern: Um die Anzahl der Zeitschritte zu ändern, ändern Sie den Wert im Feld "
                                "'Zeitschritte M eingeben'.\n"
                                "3. Endzeitpunkt ändern: Um den Endzeitpunkt der Simulation zu ändern, ändern Sie den Wert im Feld "
                                "'Endzeitpunkt T eingeben'\n"
                                "4. Löser ändern: Sie können den Löser ändern, indem Sie auf die Combo-Box klicken und dort den "
                                "gewünschtn Löser auswählen.\n"
                                "5. Relative Genauigkeit ändern: Um den wert für die relative Genauigkeit zu ändern, ändern Sie "
                                "den Wert im Feld 'Relative Genauigkeit eingeben'.\n"
                                "6. Maximale Iterationszahl ändern: Um die Maximale Iterationszahl zu ändern, ändern Sie den Wert im "
                                "Feld 'Maximale Iterationszahl eingeben'.\n"
                                "7. Stützstellenanzahl ändern: Um die Anzahl der Stützstellen zu ändern, ändern Sie den Wert im Feld "
                                "'Stützstellen N eingeben'.\n"
                                "8. Simulation starten: Um eine Simulation zu starten, klicken Sie auf den Knopf 'Simulieren'. "
                                "Anschließend werden Ihnen Informationen zur Simulation ausgegeben und anhand eines Fortschrittsbalken "
                                "der Fortschritt der Simulation angezeigt.",this);
    simulationHelp->setWordWrap(true);

    visualizationHelp = new QLabel("Der Visualisierungs-Tab bietet Ihnen folgende Funktionalitäten:\n"
                                   "Nachdem eine Simulation erfolgreich angeschlossen worden ist, können Sie sich die "
                                   "Visualisierung der Temperaturverteilung als Video oder als Einzelbild anschauen.\n"
                                   "1. Video starten: Um das Video zu starten, drücken Sie auf den Knopf 'Play'.\n"
                                   "2. Bild anzeigen: Um ein Einzelbild anzeigen zu lassen, bewegen Sie den "
                                   "Schieberegler unter der Platte an die gwünschte Position.\n"
                                   "3. Video versetzt anfangen lassen: Bewegen Sie den Schieberegler an die Positon "
                                   "von der Sie das Video starten möchten und klicken Sie anschließend auf "
                                   "den Knopf 'Play'.",this);
    visualizationHelp->setWordWrap(true);

    optimizationKonfigurationHelp = new QLabel("Der Optimierungs-Tab besteht aus zwei weiteren Tabs:\n"
                                  "Der Konfigurations-Tab bietet Ihnen folgende Funktionalitäten:\n"
                                  "1. Laden von Messungen/Beobachtungen: Indem Sie auf den Knopf 'Laden' klicken, "
                                  "können Sie Messungen/Beobachtungen einlesen lassen.\n"
                                  "2. Vorhandene Wärmequellen nutzen: Indem Sie die Checkbox 'Nutze bereits "
                                  "vorhandene Wärmequellen zur Simulation' aktivieren, werden die eingegebenen "
                                  "Wärmequellen aus dem Tab Wärmequellen für die Simulation übernommen.\n"
                                  "3. Anfangs Temperaturleitkoeffizienten ändern: Um die Anfangs Temperaturleitkoeffizienten "
                                  "zu ändern, aktivieren Sie die Checkbox 'Überschreibe bereits vorhandene "
                                  "Temperaturleitkoeffizienten zur Simulation' und ändern Sie anschließend den Wert im "
                                  "entsprechenden Feld.\n"
                                  "4. Simuationseinstellungen ändern: Um die Simuationseinstellungen zu ändern wechslen "
                                  "Sie in den Simulations-Tab und ändern Sie dort die gewünschten Werte.\n"
                                  "5. Optimierung starten: Indem Sie auf den Knopf 'Optimierung starten' klicken, wird "
                                  "die Optimierung gestartet und anhand eines Fortschrittsbalken wird der Fortschritt "
                                  "angezeigt.",this);
    optimizationKonfigurationHelp->setWordWrap(true);
    optimizationResultsHelp = new QLabel("Der Ergebnis-Tab zeigt die gefitteten Temperaturleitkoeffizienten in Form einer Tabelle an.",this);
    optimizationResultsHelp->setWordWrap(true);

        //Platzhalter
    spacerItem = new QSpacerItem(0,0,QSizePolicy::Ignored,QSizePolicy::MinimumExpanding);

        //Layout
    layout = new QGridLayout(this);

    layout->addWidget(topLabel,0,0);
    layout->addWidget(configurationHelp,1,0);
    layout->addWidget(thermalDiffusivitiesHelp,2,0);
    layout->addWidget(iBVsHelp,3,0);
    layout->addWidget(simulationHelp,4,0);
    layout->addWidget(visualizationHelp,5,0);
    layout->addWidget(optimizationKonfigurationHelp,6,0);
    layout->addWidget(optimizationResultsHelp,7,0);
    layout->addItem(spacerItem,8,0);
}
