#!/bin/bash

rm release/*
rm build/debug/*
rm build/debug/.moc/*
rm build/debug/.obj/*
rm build/debug/.qrc/*
rm build/debug/.u/*
rm build/release/*
rm build/release/.moc/*
rm build/release/.obj/*
rm build/release/.qrc/*
rm build/release/.u/*
rm src/qt/locale/*.qm

rm Makefile
rm Makefile.Debug
rm Makefile.Release
rm xtrabytes_plugin_import.cpp
rm xtrabytes_resource.rc