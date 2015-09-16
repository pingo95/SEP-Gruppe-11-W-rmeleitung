#-------------------------------------------------
#
# Project created by QtCreator 2015-09-04T09:15:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SEP_Gruppe11_Waermeleitung
TEMPLATE = app


SOURCES += \
    algorithms/Crs.cpp \
    model/Area.cpp \
    presentation/Qcustomplot.cpp \
    algorithms/Iterativesolver.cpp \
    algorithms/Jacobi.cpp \
    algorithms/Gaussseidel.cpp \
    algorithms/Intmethod.cpp \
    algorithms/Impeuler.cpp \
    algorithms/Cranknicolson.cpp \
    model/Simulationworker.cpp \
    model/Model.cpp \
    presentation/Optimizationwidget.cpp \
    presentation/Uiinit.cpp \
    presentation/Ui.cpp \
    presentation/Controller.cpp \
    Main.cpp



HEADERS  += \
    algorithms/Crs.h \
    model/Area.h \
    presentation/Qcustomplot.h \
    algorithms/Iterativesolver.h \
    algorithms/Jacobi.h \
    algorithms/Gaussseidel.h \
    algorithms/Intmethod.h \
    algorithms/Impeuler.h \
    algorithms/Cranknicolson.h \
    model/Simulationworker.h \
    model/Model.h \
    presentation/Optimizationwidget.h \
    presentation/Ui.h \
    presentation/Controller.h

RESOURCES += \
    presentation/uiicons.qrc
