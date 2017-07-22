#!/bin/sh
CP="../lib/*:classes"
SP=src/

/bin/rm -f example2.jar
/bin/rm -rf classes
/bin/mkdir -p classes/

javac -encoding utf8 -sourcepath "${SP}" -classpath "${CP}" -d classes/ src/*.java || exit 1

echo "XtraBYtes example2 module compiled."

jar cf example2.jar -C classes . || exit 1
/bin/rm -rf classes

echo "The example2.jar generated successfully."
