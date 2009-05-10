TEMPLATE = app
TARGET = QtDownloadManager
QT += core \
    gui \
    network
HEADERS += preferencesdialog.h \
    editdownloaddialog.h \
    aboutdialog.h \
    Downloads.h \
    adddownload.h \
    mainwindow.h
SOURCES += preferencesdialog.cpp \
    editdownloaddialog.cpp \
    aboutdialog.cpp \
    Downloads.cpp \
    adddownload.cpp \
    main.cpp \
    mainwindow.cpp
FORMS += preferencesdialog.ui \
    editdownloaddialog.ui \
    aboutdialog.ui \
    adddownload.ui \
    mainwindow.ui
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