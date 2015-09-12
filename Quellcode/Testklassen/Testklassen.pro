#-------------------------------------------------
#
# Project created by QtCreator 2015-09-09T09:52:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Testklassen
TEMPLATE = app


SOURCES += \
    ../algorithms/Crs.cpp \
    ../model/Area.cpp \
    ../presentation/Qcustomplot.cpp \
    ../algorithms/Iterativesolver.cpp \
    ../algorithms/Jacobi.cpp \
    ../algorithms/Gaussseidel.cpp \
    ../algorithms/Intmethod.cpp \
    ../algorithms/Impeuler.cpp \
    ../algorithms/Cranknicolson.cpp \
    ../model/Model.cpp \
    ../presentation/Uiinit.cpp \
    ../presentation/Ui.cpp\
    ../presentation/Controller.cpp \
    Testerarea.cpp \
    Main.cpp \
    Testercrs.cpp \
    Testeriterativesolver.cpp \
    Testwindow.cpp

HEADERS  += \
    ../algorithms/Crs.h \
    ../model/Area.h \
    ../presentation/Qcustomplot.h \
    ../algorithms/Iterativesolver.h \
    ../algorithms/Jacobi.h \
    ../algorithms/Gaussseidel.h \
    ../algorithms/Intmethod.h \
    ../algorithms/Impeuler.h \
    ../algorithms/Cranknicolson.h \
    ../model/Model.h \
    ../presentation/Ui.h \
    ../presentation/Controller.h \
    Testerarea.h \
    Testercrs.h \
    Testeriterativesolver.h \
    Testwindow.h
