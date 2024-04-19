QT       += core gui opengl openglwidgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include("QtGifImage/src/gifimage/qtgifimage.pri")

SOURCES += \
    ../Model/graph.cc \
    ../Model/matrix.cpp \
    ../Model/model.cpp \
    main.cpp \
    qcustomplot.cpp \
    view.cpp \
    widget.cc

HEADERS += \
    ../Controller/controller.h \
    ../Model/graph.h \
    ../Model/matrix.h \
    ../Model/model.h \
    qcustomplot.h \
    view.h \
    widget.h

FORMS += \
    view.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SUBDIRS += \

DISTFILES += \
    emnist-letters-mapping.txt \
    emnist-letters-test.csv \
    emnist-letters-train.csv
