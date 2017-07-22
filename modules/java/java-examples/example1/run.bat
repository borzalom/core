@ECHO OFF
IF EXIST java (
	start "XTRABYTES EXAMPLE1" java -cp example1.jar;lib\* XtraBYtes.Example1
) ELSE (
	ECHO Java software not found on your system. Please go to http://java.com/en/ to download a copy of Java.
)

