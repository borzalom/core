#!/bin/bash
export PATH=/usr/lib/mxe/usr/bin:$PATH

MXE_INCLUDE_PATH=/usr/lib/mxe/usr/i686-w64-mingw32.static/include
MXE_LIB_PATH=/usr/lib/mxe/usr/i686-w64-mingw32.static/lib

i686-w64-mingw32.static-qmake-qt5 \
    BOOST_LIB_SUFFIX=-mt \
    BOOST_THREAD_LIB_SUFFIX=_win32-mt \
    BOOST_INCLUDE_PATH=$MXE_INCLUDE_PATH/boost \
    BOOST_LIB_PATH=$MXE_LIB_PATH \
    QMAKE_LRELEASE=/usr/lib/mxe/usr/i686-w64-mingw32.static/qt5/bin/lrelease xtrabytes.pro

make -f Makefile.Release