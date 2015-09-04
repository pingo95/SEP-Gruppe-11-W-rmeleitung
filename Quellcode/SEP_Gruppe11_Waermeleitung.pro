#-------------------------------------------------
#
# Project created by QtCreator 2015-09-04T09:15:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = SEP_Gruppe11_Waermeleitung
TEMPLATE = app


SOURCES += algorithms/Crs.cpp \
    algorithms/Iterativesolver.cpp \
    algorithms/Jacobi.cpp \
    algorithms/Gaussseidel.cpp \
    algorithms/Intmethod.cpp \
    algorithms/Impeuler.cpp \
    algorithms/Cranknicolson.cpp \
    model/Area.cpp \
    presentation/Qcustomplot.cpp \
    model/Model.cpp \
    presentation/Ui.cpp \
    presentation/Controller.cpp \
    main.cpp



HEADERS  += \
    algorithms/Crs.h \
    algorithms/Iterativesolver.h \
    algorithms/Jacobi.h \
    algorithms/Gaussseidel.h \
    algorithms/Intmethod.h \
    algorithms/Impeuler.h \
    algorithms/Cranknicolson.h \
    model/Area.h \
    presentation/Qcustomplot.h \
    model/Model.h \
    presentation/Ui.h \
    presentation/Controller.h
