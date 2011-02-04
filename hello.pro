# -------------------------------------------------
# Project created by QtCreator 2011-01-24T13:35:31
# -------------------------------------------------
TARGET = hello
TEMPLATE = app
SOURCES += main.cpp \
    hello.cpp \
    dialog.cpp \
    termupdatethread.cpp \
    qgpio.cpp \
    myspidev.cpp \
    trendchart.cpp
HEADERS += hello.h \
    dialog.h \
    termupdatethread.h \
    qgpio.h \
    myspidev.h \
    trendchart.h
FORMS += hello.ui \
    dialog.ui
LIBS += -lts
RESOURCES += hello.qrc
OTHER_FILES += one.svg
QMAKE_POST_LINK = arm-linux-gnueabi-strip \
    -s \
    $(TARGET) \
    && \
    cp \
    $(TARGET) \
    /home/ftp/arm
MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build
RCC_DIR = build
