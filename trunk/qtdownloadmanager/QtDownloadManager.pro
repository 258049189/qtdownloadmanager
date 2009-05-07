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
