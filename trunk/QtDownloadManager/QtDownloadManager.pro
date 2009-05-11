TEMPLATE = app
TARGET = QtDownloadManager
QT += core \
    gui \
    network
HEADERS += src/Downloads.h \
    src/aboutdialog.h \
    src/adddownload.h \
    src/editdownloaddialog.h \
    src/mainwindow.h \
    src/preferencesdialog.h
SOURCES += src/Downloads.cpp \
    src/aboutdialog.cpp \
    src/adddownload.cpp \
    src/editdownloaddialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/preferencesdialog.cpp
FORMS += src/aboutdialog.ui \
    src/adddownload.ui \
    src/editdownloaddialog.ui \
    src/mainwindow.ui \
    src/preferencesdialog.ui
RESOURCES += images.qrc

target.path += /opt/QDM
INSTALLS += target
scripts.path = $$target.path/scripts
scripts.files = scripts/*
INSTALLS += scripts
configs.path = $$target.path/configs
configs.files = configs/*
INSTALLS += configs
icons.path = $$target.path
icons.files = images/icon.png
INSTALLS += icons

OBJECTS_DIR = obj
MOC_DIR = obj
RCC_DIR = resources
UI_DIR = ui