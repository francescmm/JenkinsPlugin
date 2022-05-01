CONFIG += qt warn_on c++17 c++1z

DEFINES += QT_DEPRECATED_WARNINGS

TARGET = IntegrationExample

QT += network widgets

include(src/Jenkins.pri)
include(src/AuxiliarCustomWidgets/AuxiliarCustomWidgets.pri)
include(src/QLogger/QLogger.pri)
include(src/QPinnableTabWidget/QPinnableTabWidget.pri)

SOURCES += $$PWD/main.cpp
