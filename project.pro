QT += core widgets

CONFIG += c++17

TARGET = gtfs_route_planner
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    network.cpp \
    scheduled_trip.cpp \
    csv.cpp

HEADERS += \
    mainwindow.h \
    network.h \
    scheduled_trip.h \
    types.h \
    csv.h