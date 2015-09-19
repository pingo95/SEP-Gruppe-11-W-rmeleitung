#include "Controller.h"

presentation::Controller::Controller(QObject * parent)
    : QObject(parent), loopBack(false), model(NULL), ui(NULL),
      userInput(new QInputDialog), errorMessages(new QMessageBox)
{
    started[0] = false;
    started[1] = false;
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
//    errorMessages->setParent(ui);
//    userInput->setParent(ui);
}

// Diese Funktion überprüft, ob ein neues Wärmequellen-Gebiet begonnen wurde und
// zeichnet diese im UI. (Für den Fall, dass das Modell eine Update-Benachrichtigung
// an das UI schickt (z.B. aufgrund einer abgeschlossen Simulation), während der
// Benutzer gerade ein neues Gebiet erstellt)
void presentation::Controller::testPartialArea(model::SimulationSetup::AreaType type)
{
    if(started[type])
        ui->drawPartialArea(partialAreaX,partialAreaY,type);
}

//// Diese Funktion überprüft, ob ein neues Wärmeleitkoeffizienten-Gebiet begonnen wurde
//// und zeichnet diese im UI. (Für den Fall, dass das Modell eine Update-Benachrichtigung
//// an das UI schickt (z.B. aufgrund einer abgeschlossen Simulation), während der
//// Benutzer gerade ein neues Gebiet erstellt)
//void presentation::Controller::testPartialThermalDiffusivity()
//{
//    if(started[model::SimulationSetup::AreaThermalDiffusivity])
//        ui->drawPartialThermalDiffusivity(partialAreaX,partialAreaY);
//}

// Dieser Slot verwaltet Mausklicks auf die Fläche zum Erstellen neuer
// Gebiete für Wärmequellen
void presentation::Controller::areaClickSlot(double xKoord, double yKoord, QSize plateSize,
                                             double valueShift, model::SimulationSetup::AreaType type)
{
    // QCustomPlots sind etwas neben dem Rand klickbar, dies wird hier abgefangen
    if(xKoord < 0)
        xKoord = 0;
    else
        if(xKoord > 1)
            xKoord = 1;
    if(yKoord < 0)
        yKoord = 0;
    else
        if(yKoord > 1)
            yKoord = 1;

    // Falls ein neues Gebiet bereits angefangen wurde:
    if(started[type])
    {
        // Überprüfen, ob der Punkt nahe an dem ersten Punkt liegt
        // (Schließbedingung)
        double diffX = fabs(xKoord - partialAreaX.first());
        double diffY = fabs(yKoord - partialAreaY.first());
        // (x,y) € [0,1] x [0,1], Radius von 10 Pixeln:
        double epsilonX = 10./plateSize.width();
        double epsilonY = 10./plateSize.height();
        if((diffX <= epsilonX) && (diffY <= epsilonY))
        {
            // Den ersten Punkt als neuen (letzten) Punkt hinzufügen
            partialAreaX.append(partialAreaX.first());
            partialAreaY.append(partialAreaY.first());
            // Überprüfen, ob das neue Gebiet ein gültiges ist, d.h.
            // einfach wegzusammenhängend
            if(model::Area::validateArea(partialAreaX,partialAreaY))
            {
                ui->drawPartialArea(partialAreaX,partialAreaY,type);
                // Wert für das neue Gebiet vom Benutzer abfragen
                QString title = "Eingabe Gebietswert",
                       text = "Bitte geben Sie nun den Wert für das "
                              "neue Gebiet ein:";
                bool ok;

                double value = userInput->getDouble(ui,title,text,model::SimulationSetup::AreaMinValue[type]/valueShift,
                                                    model::SimulationSetup::AreaMinValue[type]/valueShift,
                                                    model::SimulationSetup::AreaMaxValue[type]/valueShift,
                                                    0,&ok);

                // Gebiet zum Modell hinzufügen
                started[type] = false;
                loopBack = true;
                model->addNewArea(partialAreaX, partialAreaY, ok ? value * valueShift
                                        : model::SimulationSetup::AreaMinValue[type],type);
                // Temporäres Gebiet zurücksetzen
                partialAreaX.clear();
                partialAreaY.clear();
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
                started[type] = false;
            }
        }
        else
        {
            // Neuen Punkt hinzufügen
            partialAreaX.append(xKoord);
            partialAreaY.append(yKoord);
        }
    }
    else
    {
        // Neues Gebiet anfangen
        started[type] = true;
        partialAreaX.append(xKoord);
        partialAreaY.append(yKoord);
    }
    // Temporäres Gebiet zeichnen (ggf. ersetzt ein Leeres das Vorherige)
    ui->drawPartialArea(partialAreaX,partialAreaY,type);
}

// Dieser Slot updatet den Wert für ein Wärmequellengebiet, falls der neue
// gültig ist
void presentation::Controller::areaValueChangedSlot(int pos, double newValue, bool ok,
                                                    model::SimulationSetup::AreaType type)
{
    if(loopBack)
    {
        loopBack = false;
        return;
    }
    // Temperatur in Kelvin
    if(newValue >= model::SimulationSetup::AreaMinValue[type] &&
            newValue <= model::SimulationSetup::AreaMaxValue[type] && ok)
        if(pos == 0)
            model->setAreaBackground(newValue,type);
        else
            // Wert updaten
            model->updateAreaValue(pos-1,newValue,type);
    else
    {
        ui->updateNotification();
        // Fehlermeldung ausgeben:
        errorMessages->setText("Der Wert, den Sie eingegeben haben ist "
                               "ungültig. Bitte versuchen Sie es erneut.");
        errorMessages->setDetailedText("Im Hilfe-Tab finden Sie die gültigen Werte"
                                       "bereiche für die beiden Gebietsarten.");
        errorMessages->exec();
    }
}




// Dieser Slot updatet den Wert für den Rand side, falls der neue gültig ist
void presentation::Controller::newIBVValueSlot(double newValue, model::SimulationSetup::IBV side)
{
    // Temperatur in Kelvin
    if(newValue >= model::SimulationSetup::MinTemperature && newValue <= model::SimulationSetup::MaxTemperature)
        // Wert updaten
        model->setIBV(newValue,side);
    else
    {
        ui->updateNotification();
        // Fehlermeldung ausgeben:
        errorMessages->setText("Der Wert, den Sie eingegeben haben ist "
                               "ungültig. Bitte versuchen Sie es erneut.");
        errorMessages->setDetailedText("Das Programm nutzt für Temperaturen"
                                       " die Kelvin Skala, daher sind nur W"
                                       "erte größer gleich null zulässig.");
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
        ui->updateNotification();
        // Fehlermeldung ausgeben:
        errorMessages->setText("Der Wert, den Sie eingegeben haben ist "
                               "ungültig. Bitte versuchen Sie es erneut.");
        errorMessages->setDetailedText("Der zulässige Wertebereich für die"
                                       " Zeitdiskretisierungsgröße m ist (1,800].");
        errorMessages->exec();
    }
}

void presentation::Controller::newMaxErrorSlot(double newMaxError)
{
    if ((newMaxError >= 1e-10) && (newMaxError <= 1e-5))
        // Wert updaten
        model->setSolverMaxError(newMaxError);
    else
    {
        ui->updateNotification();
        // Fehlermeldung ausgeben:
        errorMessages->setText("Der Wert, den Sie eingegeben haben ist "
                               "ungültig. Bitte versuchen Sie es erneut.");
        errorMessages->setDetailedText("Der zulässige Wertebereich für den"
                                       " maximalen Fehler des Lösers ist"
                                       " [1e-10,1e-5].");
        errorMessages->exec();
    }
}

void presentation::Controller::newMaxItSlot(int newMaxIt)
{
    if ((newMaxIt >= 1) && (newMaxIt <= 1000))
        // Wert updaten
        model->setSolverMaxIt(newMaxIt);
    else
    {
        ui->updateNotification();
        // Fehlermeldung ausgeben:
        errorMessages->setText("Der Wert, den Sie eingegeben haben ist "
                               "ungültig. Bitte versuchen Sie es erneut.");
        errorMessages->setDetailedText("Der zulässige Wertebereich für die"
                                       " maximale Iterationanzahl des Lösers ist"
                                       " [1,1000].");
        errorMessages->exec();
    }
}

// Dieser Slot updatet den Wert für die Ortdiskretisierung, falls der neue
// gültig ist
void presentation::Controller::newNSlot(int newN)
{
    if ((newN <= 500) && (newN > 2))
        // Wert updaten
        model->setN(newN);
    else
    {
        ui->updateNotification();
        // Fehlermeldung ausgeben:
        errorMessages->setText("Der Wert, den Sie eingegeben haben ist "
                               "ungültig. Bitte versuchen Sie es erneut.");
        errorMessages->setDetailedText("Der zulässige Wertebereich für die"
                                       " Ortsdiskretisierungsgröße n ist (2,500].");
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
        ui->updateNotification();
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
        if(model->getSimulating())
        {
            // Fehlermeldung ausgeben:
            errorMessages->setText("Es wird zurzeit simuliert, "
                                   "Ergebnisse können erst nach Ende "
                                   "der Simulation angezeigt werden.");
            errorMessages->setDetailedText("");
            errorMessages->exec();
        }
        else
        {
            // Startbild Nr. aus dem UI
            int start = ui->getInitialFrame();
            // Schrittanzahl aus dem Modell
            int end = model->getResultM();
            // Video als Serie von einzel Bildern visualisieren
            for(int i = start; i < end+1; ++i)
                ui->visualizeState(i);
        }
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
//    for(int i=0;i<700;++i)
//        ui->visualizeState(i);
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
void presentation::Controller::selectSolverSlot(QString newSolver)
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
        model->selectSolver(newSolver);
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
    if(started[model::SimulationSetup::AreaHeatSource] &&
            !(newTab == UI::TabConfiguration || newTab == UI::TabHeatSources))
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
    if(started[model::SimulationSetup::AreaThermalDiffusivity] &&
            !(newTab == UI::TabConfiguration || newTab == UI::TabThermalDiffusivities))
    {
        ui->revertTabChange(UI::TabThermalDiffusivities);
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

// Dieser Slot löscht das zuletzt erstellte Gebiet vom Typ type, falls
// bereits mindestens eins erstellt wurde
void presentation::Controller::undoAreaSlot(model::SimulationSetup::AreaType type)
{
    if(model->getSimulationSetup()->getAreaCount(type) > 0)
        model->removeLastArea(type);
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
