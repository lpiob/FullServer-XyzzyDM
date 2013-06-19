#!/bin/sh

date
SVER=`svnversion ../..`
# Jezeli nie uzywasz repozytorium SVN, zastap powyzsza linie inna komenda ktora zwroci numer wersji
# lub zamien na ponizsza linię
#SVER="1000"

echo \#define GMVERSION \"3.3r$SVER\" > ../include/fullserver/version.inc
KIEDY=`date +%x\ %T`
GDZIE=`hostname`
GMHOST="127.0.0.1"
GMPORT="7777"

echo \#define GMCOMPILED \"skompilowana $KIEDY przez $USER@$GDZIE\" >> ../include/fullserver/version.inc
echo \#define GMHOST \"$GMHOST\" >> ../include/fullserver/version.inc
echo \#define GMPORT $GMPORT >> ../include/fullserver/version.inc

../pawno/pawncc.exe -v2 -i..\\include fs.pwn  -\;\+ -\\ -\(\+
#WINEPREFIX=wine pawno/pawncc -i/pawno/include_0.3c -i..\\include fs.pwn  -\;\+ -\\ -\(\+ && ncftpput -u login -p haslo -C host fs.amx gamemodes/fs.amx
