#!/bin/sh
CP="../lib/*:classes"
SP=src/

/bin/rm -f example1.jar
/bin/rm -rf classes
/bin/mkdir -p classes/

javac -encoding utf8 -sourcepath "${SP}" -classpath "${CP}" -d classes/ src/*.java || exit 1

echo "XtraBYtes example1 module compiled."

jar cf example1.jar -C classes . || exit 1
/bin/rm -rf classes

echo "The xtrabytes-example1.jar generated successfully."
