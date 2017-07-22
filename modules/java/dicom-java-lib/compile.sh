#!/bin/sh
CP="../java-examples/lib/*:classes"
SP=src/

/bin/rm -f ../xtrabytes-dicom.jar
/bin/rm -rf classes
/bin/mkdir -p classes/

javac -encoding utf8 -sourcepath "${SP}" -classpath "${CP}" -d classes/ src/*.java || exit 1

echo "XtraBYtes DICOM java library compiled successfully"

jar cf ../java-examples/lib/xtrabytes-dicom.jar -C classes . || exit 1
/bin/rm -rf classes

echo "XtraBYtes DICOM java library generated successfully"
