#include <QApplication>
#include "./model/Model.h"
#include "./presentation/Ui.h"
#include "./presentation/Controller.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    //Deutsche Übersetzung für Standard Knöpfe
    QTranslator translator;
    translator.load("qt_de_DE", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    application.installTranslator(&translator);

     qRegisterMetaType<model::SimulationSetup >();

    //Anlegen der MVC Objekte
    model::Model * model = new model::Model();
    presentation::UI * ui = new presentation::UI();
    presentation::Controller * controller = new presentation::Controller();

    //Verbinden der MVC Objekte:
    model->setUI(ui);
    ui->setController(controller);
    ui->setModel(model);
    controller->setModel(model);
    controller->setUI(ui);

    //Anzeigen des UI:
    ui->show();

    //Ausführen der App:
    int ret =  application.exec();

    //Löschen der MVC Objekte:
    delete model;
    delete ui;
    delete controller;

    return ret;
}
