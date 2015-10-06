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
    model/Area.cpp \
    presentation/Qcustomplot.cpp \
    model/Simulationsetup.cpp \
    model/Simulationworker.cpp \
    model/Model.cpp \
    presentation/Doublespinboxdelegate.cpp \
    presentation/Areawidget.cpp \
    presentation/Ibvswidget.cpp \
    presentation/Simulationwidget.cpp \
    presentation/Visualizationwidget.cpp \
    presentation/Optimizationwidget.cpp \
    presentation/Helpwidget.cpp \
    presentation/Ui.cpp \
    presentation/Controller.cpp \
    Main.cpp

HEADERS  += \
    algorithms/Crs.h \
    algorithms/Crs.hpp \
    algorithms/Solver.h \
    algorithms/Solver.hpp \
    algorithms/Iterativesolver.h \
    algorithms/Iterativesolver.hpp \
    algorithms/Jacobi.h \
    algorithms/Jacobi.hpp \
    algorithms/Gaussseidel.h \
    algorithms/Gaussseidel.hpp \
    algorithms/LU.h \
    algorithms/LU.hpp \
    algorithms/Intmethod.h \
    algorithms/Intmethod.hpp \
    algorithms/Impeuler.h \
    algorithms/Impeuler.hpp \
    algorithms/Cranknicolson.h \
    algorithms/Cranknicolson.hpp \
    presentation/Qcustomplot.h \
    algorithms/dco.hpp \
    model/Area.h \
    model/Simulationsetup.h \
    model/Simulationworker.h \
    model/Model.h \
    presentation/Doublespinboxdelegate.h \
    presentation/Areawidget.h \
    presentation/Ibvswidget.h \
    presentation/Simulationwidget.h \
    presentation/Visualizationwidget.h \
    presentation/Optimizationwidget.h \
    presentation/Helpwidget.h \
    presentation/Ui.h \
    presentation/Controller.h

RESOURCES += \
    presentation/uiicons.qrc

LIBS += -L"$$_PRO_FILE_PWD_/algorithms" -ldco
unix{
    runTarget.target = run
    runTarget.commands = export NAG_KUSARI_FILE=./algorithms/key.txt; $$TARGET;
    runTarget.depends = first
    QMAKE_EXTRA_TARGETS += runTarget
}
