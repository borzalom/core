@ECHO OFF
IF EXIST java (
	start "XTRABYTES EXAMPLE2" java -cp example2.jar;lib\* XtraBYtes.Example2
) ELSE (
	ECHO Java software not found on your system. Please go to http://java.com/en/ to download a copy of Java.
)

