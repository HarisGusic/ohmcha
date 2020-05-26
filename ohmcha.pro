QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/fileio/xml.cpp \
    src/model/component.cpp \
    src/model/graph.cpp \
    src/model/kirchhoff.cpp \
    src/program/program.cpp \
    src/test/test.cpp \
    src/ui/graphic_component.cpp \
    src/ui/mainwindow.cpp \
    src/ui/circuitview.cpp

HEADERS += \
    src/fileio/xml.h \
    src/model/component.h \
    src/model/graph.h \
    src/model/kirchhoff.h \
    src/program/program.h \
    src/test/test.h \
    src/ui/graphic_component.h \
    src/ui/mainwindow.h \
    src/ui/circuitview.h

FORMS += \
    src/ui/mainwindow.ui

include(qmake-specific.pri)
INCLUDEPATH += /usr/include/eigen3

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
