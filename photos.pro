#
# Specifies the name of the target file for the application.
#
TARGET = Photos

#
# Specifies to Qt to generated the Makefile for
# building an application, as opposed to something like
# library.
#
TEMPLATE = app

#
# Separates the build file directories for
# the generated Qt files.
#
#release: DESTDIR = build/release
#debug:   DESTDIR = build/debug
DESTDIR = src/build
OBJECTS_DIR = $$DESTDIR/obj
MOC_DIR = $$DESTDIR/moc
RCC_DIR = $$DESTDIR/qrc
UI_DIR = $$DESTDIR/ui

#
# Specifies the Qt modules we want to use.
#
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#
# Qt configuration options
#
CONFIG += c++11

#
# Disables all the APIs deprecated before Qt 6.0.0 to ensure
# the code fails to compile if using depreciated APIs.
#
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

#
# Application source cpp files
#
SOURCES += \
    src/main.cpp \
    src/pages/MainWindow.cpp

#
# Application header files
#
HEADERS += \
    src/pages/MainWindow.hpp

#
# Applicatin UI files
#
FORMS += \
    src/ui/MainWindow.ui

#
# Application translation files
#
TRANSLATIONS += \
    src/translations/photos_en_US.ts

#
# Default rules for deployment.
#
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
