## XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
## Copyright (c) 2017, Zoltan Szabo. All rights reserved.
## Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
## Licensed under GNU General Public License Version 3 or later (the "GPL")

TEMPLATE = app
TARGET = xtrabytes
INCLUDEPATH += src src/gui

HEADERS  += src/version.h

DEFINES +=  "VERSION_MAJOR=$$XTRABYTES_VERSION_MAJOR"\
            "VERSION_MINOR=$$XTRABYTES_VERSION_MINOR"\
            "VERSION_REVISION=$$XTRABYTES_VERSION_REVISION"\
            "VERSION_BUILD=$$XTRABYTES_VERSION_BUILD"

#Target version
VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_REVISION}.$${VERSION_BUILD}

QT += core gui widgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
lessThan(QT_MAJOR_VERSION, 5): CONFIG += static

SOURCES +=   src/xtrabytes.cpp \
	     src/gui/xtrabytesgui.cpp \
	     src/gui/aboutdialog.cpp \
	     src/gui/overviewpage.cpp

HEADERS +=   src/gui/xtrabytesgui.h \
	     src/gui/aboutdialog.h \
	     src/gui/overviewpage.h

FORMS +=     src/gui/forms/aboutdialog.ui \
	     src/gui/forms/overviewpage.ui

RESOURCES += src/gui/xtrabytes.qrc


# Dependency library locations can be customized with:
#    BOOST_INCLUDE_PATH, BOOST_LIB_PATH


CONFIG(debug, debug|release) {
    DESTDIR = build/debug
}
CONFIG(release, debug|release) {
    DESTDIR = build/release
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.u


# for lrelease/lupdate
# also add new translations to src/qt/xtrabytes.qrc under translations/
TRANSLATIONS = $$files(src/qt/locale/xtrabytes_*.ts)

isEmpty(QMAKE_LRELEASE) {
    win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}

LIBS += -lboost_system

system($$QMAKE_LRELEASE -silent $$_PRO_FILE_)
