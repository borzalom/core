## XtraBYtes - The Proof of Signature Blocktech Superchain - http://xtrabytes.global
## Copyright (c) 2017, Zoltan Szabo. All rights reserved.
## Copyright (c) 2017, XtraBYtes Founders and Developers ( see AUTHORS )
## Licensed under GNU General Public License Version 3 or later (the "GPL")

TEMPLATE = app
TARGET = xtrabytes
INCLUDEPATH += src src/gui

CONFIG += console

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
	     src/gui/overviewpage.cpp \
	     src/gui/models/m-xbridge.cpp \
	     src/util.cpp \
	     src/vitals.cpp \
	     src/redfat.cpp \
	     src/crypto.cpp \
	     src/error.cpp \
	     src/gui.cpp \
	     src/threads.cpp \
	     src/genesis.cpp \
	     src/superchain.cpp \
	     src/block.cpp

HEADERS +=   src/gui/xtrabytesgui.h \
	     src/gui/aboutdialog.h \
	     src/gui/models/m-xbridge.h \
	     src/gui/overviewpage.h

FORMS +=     src/gui/forms/aboutdialog.ui \
	     src/gui/forms/overviewpage.ui

RESOURCES += src/gui/xtrabytes.qrc


# Dependency library locations can be customized with:
#    BOOST_INCLUDE_PATH, BOOST_LIB_PATH


INCLUDEPATH += src/leveldb/include src/leveldb/helpers
LIBS += $$PWD/src/leveldb/libleveldb.a $$PWD/src/leveldb/libmemenv.a

!win32 {
    # we use QMAKE_CXXFLAGS_RELEASE even without RELEASE=1 because we use RELEASE to indicate linking preferences not -O preferences
    genleveldb.commands = cd $$PWD/src/leveldb && CC=$$QMAKE_CC CXX=$$QMAKE_CXX $(MAKE) OPT=\"$$QMAKE_CXXFLAGS $$QMAKE_CXXFLAGS_RELEASE\" libleveldb.a libmemenv.a
} else {
    # make an educated guess about what the ranlib command is called
    isEmpty(QMAKE_RANLIB) {
        QMAKE_RANLIB = $$replace(QMAKE_STRIP, strip, ranlib)
    }
    LIBS += -lshlwapi
    genleveldb.commands = cd $$PWD/src/leveldb && CC=$$QMAKE_CC CXX=$$QMAKE_CXX TARGET_OS=OS_WINDOWS_CROSSCOMPILE $(MAKE) OPT=\"$$QMAKE_CXXFLAGS $$QMAKE_CXXFLAGS_RELEASE\" libleveldb.a libmemenv.a && $$QMAKE_RANLIB $$PWD/src/leveldb/libleveldb.a && $$QMAKE_RANLIB $$PWD/src/leveldb/libmemenv.a
}
genleveldb.target = $$PWD/src/leveldb/libleveldb.a
genleveldb.depends = FORCE
PRE_TARGETDEPS += $$PWD/src/leveldb/libleveldb.a
QMAKE_EXTRA_TARGETS += genleveldb
# Gross ugly hack that depends on qmake internals, unfortunately there is no other way to do it.
QMAKE_CLEAN += $$PWD/src/leveldb/libleveldb.a; cd $$PWD/src/leveldb ; $(MAKE) clean


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
TRANSLATIONS = $$files(src/gui/locale/xtrabytes_*.ts)

isEmpty(QMAKE_LRELEASE) {
    win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}

INCLUDEPATH += $$OPENSSL_INCLUDE_PATH
LIBS += $$join(OPENSSL_LIB_PATH,,-L,)
LIBS += -lssl -lcrypto


LIBS += -lboost_system$$BOOST_LIB_SUFFIX -lboost_thread$$BOOST_THREAD_LIB_SUFFIX

system($$QMAKE_LRELEASE -silent $$_PRO_FILE_)
