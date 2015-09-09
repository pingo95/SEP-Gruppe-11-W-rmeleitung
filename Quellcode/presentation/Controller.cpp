#include "Controller.h"

presentation::Controller::Controller(QObject * parent)
    : QObject(parent), model(NULL), startedNewHeatSource(false),
      startedNewThermalConductivity(false), ui(NULL),
      userInput(new QInputDialog), errorMessages(new QMessageBox)
{
    errorMessages->setWindowTitle("Fehlermeldung");
    errorMessages->setIcon(QMessageBox::Critical);
    errorMessages->setStandardButtons(QMessageBox::Ok);
}

presentation::Controller::~Controller()
{
    delete userInput;
    delete errorMessages;
}

// Diese Funktion setzt die Referenz (als Zeiger) auf das mit dem Controller
// verbundene Modell
void presentation::Controller::setModel(model::Model *model)
{
    this->model = model;
}

// Diese Funktion setzt die Referenz (als Zeiger) auf das mit dem Controller
// verbundene UI und die entsprechenden Signale & Slots werden in der
// korrespondierenden set-Funktion des UI gesetzt
void presentation::Controller::setUI(UI *ui)
{
    this->ui = ui;
}

// Diese Funktion überprüft, ob ein neues Wärmequellen-Gebiet begonnen wurde und
// zeichnet diese im UI. (Für den Fall, dass das Modell eine Update-Benachrichtigung
// an das UI schickt (z.B. aufgrund einer abgeschlossen Simulation), während der
// Benutzer gerade ein neues Gebiet erstellt)
void presentation::Controller::testPartialHeatSource()
{
    if(startedNewHeatSource)
        ui->drawPartialHeatSource(partialAreaX,partialAreaY);
}

// Diese Funktion überprüft, ob ein neues Wärmeleitkoeffizienten-Gebiet begonnen wurde
// und zeichnet diese im UI. (Für den Fall, dass das Modell eine Update-Benachrichtigung
// an das UI schickt (z.B. aufgrund einer abgeschlossen Simulation), während der
// Benutzer gerade ein neues Gebiet erstellt)
void presentation::Controller::testPartialThermalConductivity()
{
    if(startedNewThermalConductivity)
        ui->drawPartialThermalConductivity(partialAreaX,partialAreaY);
}

// Dieser Slot verwaltet Mausklicks auf die Fläche zum Erstellen neuer
// Gebiete für Wärmequellen
void presentation::Controller::heatSourcesClickSlot(QMouseEvent *event)
{
    // Umwandeln von Pixeln in Koordinaten:
    double x,y;
    ui->heatSourcePixelToCoords(event->x(),event->y(),x,y);
    // Falls ein neues Gebiet bereits angefangen wurde:
    if(startedNewHeatSource)
    {
        // Überprüfen, ob der Punkt nahe an dem ersten Punkt liegt
        // (Schließbedingung)
        double diffX = fabs(x - partialAreaX.first());
        double diffY = fabs(y - partialAreaY.first());
        QSize plotSize = ui->getHeatSourcePlotSize();
        // (x,y) € [0,1] x [0,1], Radius von 10 Pixeln:
        double epsilonX = 10./plotSize.width();
        double epsilonY = 10./plotSize.height();
        if((diffX <= epsilonX) && (diffY <= epsilonY))
        {
            // Den ersten Punkt als neuen (letzten) Punkt hinzufügen
            partialAreaX.append(partialAreaX.first());
            partialAreaY.append(partialAreaY.first());
            // Überprüfen, ob das neue Gebiet ein gültiges ist, d.h.
            // einfach wegzusammenhängend
            if(model::Area::validateArea(partialAreaX,partialAreaY))
            {
                ui->drawPartialHeatSource(partialAreaX,partialAreaY);
                // Wert für das neue Gebiet vom Benutzer abfragen
                QString title = "Eingabe Wert Waermequellen",
                       text = "Bitte geben Sie nun den Wert für die "
                              "neue Wärmequelle ein:";
                bool ok;
                double value = userInput->getDouble(ui,title,text,0,0,10000,2,&ok);
                // Gebiet zum Modell hinzufügen
                model->addHeatSource(new model::Area(partialAreaX,
                                                     partialAreaY, ok ? value : 0, "Wärmequelle"));

                // Temporäres Gebiet zurücksetzen
                partialAreaX.clear();
                partialAreaY.clear();
                startedNewHeatSource = false;
            }
            else
            {
                // Fehlermeldung ausgeben:
                errorMessages->setText("Das Gebiet, dass Sie gezeichnet haben ist "
                                       "ungültig. Bitte versuchen Sie es erneut.");
                errorMessages->setDetailedText("Damit ein Gebiet gültig ist, muss es"
                                               " geschlossen einfach wegzusammenhängend"
                                               " sein. D.h. es dürfen sich keine Kanten"
                                               " schneiden oder doppelt vorkommen und es"
                                               "muss aus mindestens drei Punkten bestehen.");
                errorMessages->exec();

                // Temporäres Gebiet zurücksetzen
                partialAreaX.clear();
                partialAreaY.clear();
                startedNewHeatSource = false;
            }
        }
        else
        {
            // Neuen Punkt hinzufügen
            partialAreaX.append(x);
            partialAreaY.append(y);
        }
    }
    else
    {
        // Neues Gebiet anfangen
        startedNewHeatSource = true;
        partialAreaX.append(x);
        partialAreaY.append(y);
    }
    // Temporäres Gebiet zeichnen (ggf. ersetzt ein Leeres das Vorherige)
    ui->drawPartialHeatSource(partialAreaX,partialAreaY);
}

// Dieser Slot updatet den Wert für ein Wärmequellengebiet, falls der neue
// gültig ist
void presentation::Controller::heatSourceValueChangedSlot(int pos, int column)
{
    // Testen ob auch wirklich Gebietswert geändert wurde, da das Signal bei
    // Änderungen in allen Felder des Tabellen Widgets ausgelöst wird
    if(column != UI::ColumnValue) return; // TODO: Wert überprüfen
    int id = ui->getThermalConductivityID(pos);
    double value = ui->getNewHeatSourceValue(pos);
    // Temperatur in Kelvin
    if(value > 0 && value != model->getHeatSource(id)->getValue())
        // Wert updaten
        model->updateHeatSourceValue(pos,value);
    else
    {
        // Fehlermeldung ausgeben:
        errorMessages->setText("Der Wert, den Sie eingegeben haben ist "
                               "ungültig. Bitte versuchen Sie es erneut.");
        errorMessages->setDetailedText("Das Programm nutzt für Temperaturen"
                                       " die Kelvin Skala, daher sind nur W"
                                       "erte größer null zulässig.");
        errorMessages->exec();
    }
}

// Dieser Slot updatet den Wert für den unteren Rand, falls der neue gültig ist
void presentation::Controller::newBottomBoundarySlot(double newBottomBoundary)
{
    // Temperatur in Kelvin
    if(newBottomBoundary > 0)
        // Wert updaten
        model->setBottomBoundary(newBottomBoundary);
    else
    {
        // Fehlermeldung ausgeben:
        errorMessages->setText("Der Wert, den Sie eingegeben haben ist "
                               "ungültig. Bitte versuchen Sie es erneut.");
        errorMessages->setDetailedText("Das Programm nutzt für Temperaturen"
                                       " die Kelvin Skala, daher sind nur W"
                                       "erte größer null zulässig.");
        errorMessages->exec();
    }
}

// Dieser Slot updatet den Anfangswert, falls der neue gültig ist
void presentation::Controller::newInitialValueSlot(double newInitialValue)
{
    // Temperatur in Kelvin
    if(newInitialValue > 0)
        // Wert updaten
        model->setInitialValue(newInitialValue);
    else
    {
        // Fehlermeldung ausgeben:
        errorMessages->setText("Der Wert, den Sie eingegeben haben ist "
                               "ungültig. Bitte versuchen Sie es erneut.");
        errorMessages->setDetailedText("Das Programm nutzt für Temperaturen"
                                       " die Kelvin Skala, daher sind nur W"
                                       "erte größer null zulässig.");
        errorMessages->exec();
    }
}

// Dieser Slot updatet den Wert für den linken Rand, falls der neue gültig ist
void presentation::Controller::newLeftBoundarySlot(double newLeftBoundary)
{
    // Temperatur in Kelvin
    if(newLeftBoundary > 0)
        // Wert updaten
        model->setLeftBoundary(newLeftBoundary);
    else
    {
        // Fehlermeldung ausgeben:
        errorMessages->setText("Der Wert, den Sie eingegeben haben ist "
                               "ungültig. Bitte versuchen Sie es erneut.");
        errorMessages->setDetailedText("Das Programm nutzt für Temperaturen"
                                       " die Kelvin Skala, daher sind nur W"
                                       "erte größer null zulässig.");
        errorMessages->exec();
    }
}

// Dieser Slot updatet den Wert für die Zeitdiskretisierung, falls der neue
// gültig ist
void presentation::Controller::newMSlot(int newM)
{
    if ((newM <= 800) && (newM > 0))
        // Wert updaten
        model->setM(newM);
    else
    {
        // Fehlermeldung ausgeben:
        errorMessages->setText("Der Wert, den Sie eingegeben haben ist "
                               "ungültig. Bitte versuchen Sie es erneut.");
        errorMessages->setDetailedText("Der zulässige Wertebereich für die"
                                       " Zeitdiskretisierungsgröße m ist (0,800].");
        errorMessages->exec();
    }
}

// Dieser Slot updatet den Wert für die Ortdiskretisierung, falls der neue
// gültig ist
void presentation::Controller::newNSlot(int newN)
{
    if ((newN <= 500) && (newN > 0))
        // Wert updaten
        model->setN(newN);
    else
    {
        // Fehlermeldung ausgeben:
        errorMessages->setText("Der Wert, den Sie eingegeben haben ist "
                               "ungültig. Bitte versuchen Sie es erneut.");
        errorMessages->setDetailedText("Der zulässige Wertebereich für die"
                                       " Ortsdiskretisierungsgröße n ist (0,500].");
        errorMessages->exec();
    }
}

// Dieser Slot updatet den Wert für den rechten Rand, falls der neue gültig ist
void presentation::Controller::newRightBoundarySlot(double newRightBoundary)
{
    // Temperatur in Kelvin
    if(newRightBoundary > 0)
        // Wert updaten
        model->setRightBoundary(newRightBoundary);
    else
    {
        // Fehlermeldung ausgeben:
        errorMessages->setText("Der Wert, den Sie eingegeben haben ist "
                               "ungültig. Bitte versuchen Sie es erneut.");
        errorMessages->setDetailedText("Das Programm nutzt für Temperaturen"
                                       " die Kelvin Skala, daher sind nur W"
                                       "erte größer null zulässig.");
        errorMessages->exec();
    }
}

// Dieser Slot updatet den Wert für den oberen Rand, falls der neue gültig ist
void presentation::Controller::newTopBoundarySlot(double newTopBoundary)
{
    // Temperatur in Kelvin
    if(newTopBoundary > 0)
        // Wert updaten
        model->setTopBoundary(newTopBoundary);
    else
    {
        // Fehlermeldung ausgeben:
        errorMessages->setText("Der Wert, den Sie eingegeben haben ist "
                               "ungültig. Bitte versuchen Sie es erneut.");
        errorMessages->setDetailedText("Das Programm nutzt für Temperaturen"
                                       " die Kelvin Skala, daher sind nur W"
                                       "erte größer null zulässig.");
        errorMessages->exec();
    }
}

// Dieser Slot updatet den Wert für den Endzeitpunkt, falls der neue
// gültig ist
void presentation::Controller::newTSlot(double newT)
{
    if (newT > 0)
        // Wert updaten
        model->setT(newT);
    else
    {
        // Fehlermeldung ausgeben:
        errorMessages->setText("Der Wert, den Sie eingegeben haben ist "
                               "ungültig. Bitte versuchen Sie es erneut.");
        errorMessages->setDetailedText("Für den Endzeitpunkt muss ein Wert"
                                       " größer Null gewählt werden, die E"
                                       "inheit ist Sekunden.");
        errorMessages->exec();
    }
}

// Dieser Slot spielt, falls bereits eine Simulation durchgeführt wurde,
// deren Ergebnis als "Video" ab
void presentation::Controller::playVideoSlot()
{
    // Überprüfen, ob schon simuliert wurde
    if(model->getSimulated())
    {
        // Startbild Nr. aus dem UI
        int start = ui->getInitialFrame();
        // Schrittanzahl aus dem Modell
        int end = model->getResultM();
        // Video als Serie von einzel Bildern visualisieren
        for(int i = start; i <= end; ++i)
            ui->visualizeState(i);
    }
    else
    {
        // Fehlermeldung ausgeben:
        errorMessages->setText("Es muss zunächst eine Simulation"
                               " durchgeführt werden, bevor ein "
                               "Ergebnis visualiziert werden kann.");
        errorMessages->setDetailedText("");
        errorMessages->exec();
    }
}

// Dieser Slot updatet die gewählte Integrationsmethode, dies ist aber nur
// möglich falls gerade nicht simuliert wird
void presentation::Controller::selectIntMethodSlot(QString newIntMethod)
{
    if(model->getSimulating())
    {
        // Es wird gerade simuliert, UI muss den Wert zurücksetzen
        ui->updateNotification();

        // Fehlermeldung ausgeben:
        errorMessages->setText("Es wird gerade eine Simulation durchgeführt,"
                               " bitte versuchen Sie es nach Ende der Simulation erneut.");
        errorMessages->setDetailedText("");
        errorMessages->exec();

    }
    else
    {
        // Keine Simulation, Update des Wertes
        model->selectIntMethod(newIntMethod);
    }
}

// Dieser Slot updatet den gewählten iterativen Löser, dies ist aber nur
// möglich falls gerade nicht simuliert wird
void presentation::Controller::selectIterativeSolverSlot(QString newIterativeSolver)
{
    if(model->getSimulating())
    {
        // Es wird gerade simuliert, UI muss den Wert zurücksetzen
        ui->updateNotification();

        // Fehlermeldung ausgeben:
        errorMessages->setText("Es wird gerade eine Simulation durchgeführt,"
                               " bitte versuchen Sie es nach Ende der Simulation erneut.");
        errorMessages->setDetailedText("");
        errorMessages->exec();
    }
    else
    {
        // Keine Simulation, Update des Wertes
        model->selectIterativeSolver(newIterativeSolver);
    }
}

// Dieser Slot startet die Simulation, falls diese nicht schon läuft
void presentation::Controller::simulateSlot()
{
    if(model->getSimulating())
    {
        // Fehlermeldung ausgeben:
        errorMessages->setText("Es wird gerade eine Simulation durchgeführt,"
                               " bitte versuchen Sie es nach Ende der Simulation erneut.");
        errorMessages->setDetailedText("");
        errorMessages->exec();
    }
    else
        model->simulate();
}

// Dieser Slot verhindert, dass während des Erstellens eines neuen Gebietes,
// der Tab gewechselt werden kann, d.h. ohne das neue Gebiet fertigzustellen
void presentation::Controller::tabChangedSlot(int newTab)
{
    // Falls gerade ein neues Wärmequellen-Gebiet erstellt wird
    // und der entsprechende Tab nicht mehr geöffnet ist, in diesen
    // zurücksetzen
    if(startedNewHeatSource && !(newTab == UI::TabConfiguration
                                 || newTab == UI::TabHeatSources))
    {
        ui->revertTabChange(UI::TabHeatSources);
        // Fehlermeldung ausgeben:
        errorMessages->setText("Das von Ihnen angefangen Gebiet muss entweder "
                               "abgeschlossen oder abgebrochen werden, bevor Sie"
                               " den Tab wechseln können.");
        errorMessages->setDetailedText("");
        errorMessages->exec();
        return;
    }

    // Analog für den Fall, dass ein Wärmeleitkoeffizienten-Gebiet begonnen wurde
    if(startedNewThermalConductivity && !(newTab == UI::TabConfiguration
                                         || newTab == UI::TabThermalConductivity))
    {
        ui->revertTabChange(UI::TabThermalConductivity);
        // Fehlermeldung ausgeben:
        errorMessages->setText("Das von Ihnen angefangen Gebiet muss entweder "
                               "abgeschlossen oder abgebrochen werden, bevor Sie"
                               " den Tab wechseln können.");
        errorMessages->setDetailedText("");
        errorMessages->exec();
        return;
    }

    // Aktiven Tab im UI ändern und UI updaten
    ui->setActiveTab(newTab);
}

// Dieser Slot verwaltet Mausklicks auf die Fläche zum Erstellen neuer
// Gebiete für Wärmeleitkoeffizienten
void presentation::Controller::thermalConductivitiesClickSlot(QMouseEvent *event)
{
    // Umwandeln von Pixeln in Koordinaten:
    double x,y;
    ui->thermalConductivityPixelToCoords(event->x(),event->y(),x,y);
    // Falls ein neues Gebiet bereits angefangen wurde:
    if(startedNewThermalConductivity)
    {
        // Überprüfen, ob der Punkt nahe an dem ersten Punkt liegt
        // (Schließbedingung)
        double diffX = fabs(x - partialAreaX.first());
        double diffY = fabs(y - partialAreaY.first());
        QSize plotSize = ui->getThermalConductivityPlotSize();
        // (x,y) € [0,1] x [0,1], Radius von 10 Pixeln:
        double epsilonX = 10./plotSize.width();
        double epsilonY = 10./plotSize.height();
        if((diffX <= epsilonX) && (diffY <= epsilonY))
        {
            // Den ersten Punkt als neuen (letzten) Punkt hinzufügen
            partialAreaX.append(partialAreaX.first());
            partialAreaY.append(partialAreaY.first());
            // Überprüfen, ob das neue Gebiet ein gültiges ist, d.h.
            // einfach wegzusammenhängend
            if(model::Area::validateArea(partialAreaX,partialAreaY))
            {
                ui->drawPartialThermalConductivity(partialAreaX,partialAreaY);
                // Den Wert für das neue Gebiet vom Benuter abfragen
                QString title = "Eingabe Wert Waermeleitkoeffizient",
                       text = "Bitte geben Sie nun den Wert für das "
                              "neue Wärmeleitkoeffizienten-Gebiet ein:";
                bool ok;
                double value = userInput->getDouble(ui,title,text,0,0,10000,2,&ok);

                // Gebiet zum Modell hinzufügen
                model->addThermalConductivity(new model::Area(partialAreaX,
                                                              partialAreaY,ok ? value : 0, "Wärmeleitkoeffizient"));

                // Temporäres Gebiet zurücksetzen
                partialAreaX.clear();
                partialAreaY.clear();
                startedNewThermalConductivity = false;
            }
            else
            {
                // Fehlermeldung ausgeben:
                errorMessages->setText("Das Gebiet, dass Sie gezeichnet haben ist "
                                       "ungültig. Bitte versuchen Sie es erneut.");
                errorMessages->setDetailedText("Damit ein Gebiet gültig ist, muss es"
                                               " geschlossen einfach wegzusammenhängend"
                                               " sein. D.h. es dürfen sich keine Kanten"
                                               " schneiden oder doppelt vorkommen und es"
                                               "muss aus mindestens drei Punkten bestehen.");
                errorMessages->exec();

                // Temporäres Gebiet zurücksetzen
                partialAreaX.clear();
                partialAreaY.clear();
                startedNewThermalConductivity = false;
            }
        }
        else
        {
            // Neuen Punkt hinzufügen
            partialAreaX.append(x);
            partialAreaY.append(y);
        }
    }
    else
    {
        // Neues Gebiet anfangen
        startedNewThermalConductivity = true;
        partialAreaX.append(x);
        partialAreaY.append(y);
    }
    // Temporäres Gebiet zeichnen (ggf. ersetzt ein Leeres das Vorherige)
    ui->drawPartialThermalConductivity(partialAreaX,partialAreaY);
}

// Dieser Slot updatet den Wert für ein Wärmeleitkoeffizienten-Gebiet,
// falls der neue gültig ist
void presentation::Controller::thermalConductivityValueChangedSlot(int pos, int column)
{
    // Testen ob auch wirklich Gebietswert geändert wurde, da das Signal bei
    // Änderungen in allen Felder des Tabellen Widgets ausgelöst wird
    if(column != UI::ColumnValue) return; // TODO: Wert überprüfen
    int id = ui->getHeatSourceID(pos);
    double value = ui->getNewThermalConductivityValue(pos);
    // Temperatur in Kelvin
    if(value > 0 && value != model->getThermalConductivity(id)->getValue())
        model->updateThermalConductivityValue(pos,value);
    else
    {
        // Fehlermeldung ausgeben:
        errorMessages->setText("Der Wert, den Sie eingegeben haben ist "
                               "ungültig. Bitte versuchen Sie es erneut.");
        errorMessages->setDetailedText("Das Programm nutzt für Temperaturen"
                                       " die Kelvin Skala, daher sind nur W"
                                       "erte größer null zulässig.");
        errorMessages->exec();
    }
}

// Dieser Slot löscht das zuletzt erstellte Wärmequellen-Gebiet, falls
// bereits mindestens eins erstellt wurde
void presentation::Controller::undoHeatSourceSlot()
{
    if(model->getHeatSourcesCount() > 0)
        model->removeLastHeatSource();
    else
    {
        // Fehlermeldung ausgeben:
        errorMessages->setText("Es wurde noch kein Gebiet hinzugefügt.");
        errorMessages->setDetailedText("");
        errorMessages->exec();
    }
}

// Dieser Slot löscht das zuletzt erstellte Wärmequellen-Gebiet, falls
// bereits mindestens eins erstellt wurde
void presentation::Controller::undoThermalConductivitySlot()
{
    if(model->getThermalConductivitiesCount() > 0)
        model->removeLastThermalConductivity();
    else
    {
        // Fehlermeldung ausgeben:
        errorMessages->setText("Es wurde noch kein Gebiet hinzugefügt.");
        errorMessages->setDetailedText("");
        errorMessages->exec();
    }
}

// Dieser Slot visualiziert einen einzelnen Zeitpunkt der letzen Simulation,
// vorrausgesetzt es wurde bereits eine durchgeführt
void presentation::Controller::visualizeStateSlot(int frame)
{
    if(model->getSimulated())
        ui->visualizeState(frame);
    else
    {
        // Fehlermeldung ausgeben:
        errorMessages->setText("Es muss zunächst eine Simulation"
                               " durchgeführt werden, bevor ein "
                               "Ergebnis visualiziert werden kann.");
        errorMessages->setDetailedText("");
        errorMessages->exec();
    }
}
