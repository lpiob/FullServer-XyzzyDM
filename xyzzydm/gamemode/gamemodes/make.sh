#!/bin/sh

date
#SVER=`svnversion ../..`
echo \#define GMVERSION \"3.3-NetShoot\" > ../include/fullserver/version.inc
KIEDY=`date +%x\ %T`
GDZIE=`hostname`
GMHOST="91.204.162.80"
#GMPORT="8888"

echo \#define GMCOMPILED \"skompilowana $KIEDY przez $USER@$GDZIE\" >> ../include/fullserver/version.inc
echo \#define GMHOST \"$GMHOST\" >> ../include/fullserver/version.inc
echo \#define GMPORT $GMPORT >> ../include/fullserver/version.inc

WINEPREFIX=/vol/n/.wg/ wine /pawno/pawncc -i/pawno/include_0.3d -i..\\include xyzzydm.pwn  -\;\+ -\\ -\(\+

