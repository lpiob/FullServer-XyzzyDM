#define DIALOG_LOGOWANIE			1
#define DIALOG_LOGOWANIE_WOFF	2


stock AC_ShowPlayerDialog(playerid, dialogid, style, caption[], info[], button1[], button2[]){
	pTemp[playerid][ept_dialogid]=dialogid;
	return ShowPlayerDialog(playerid, dialogid, style, caption, info, button1, button2);
}

#if defined _ALS_ShowPlayerDialog
    #undef ShowPlayerDialog
#else
    #define _ALS_ShowPlayerDialog
#endif
#define ShowPlayerDialog AC_ShowPlayerDialog

