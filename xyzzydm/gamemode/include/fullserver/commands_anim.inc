stock LoopingAnim(playerid,animlib[],animname[], Float:Speed, looping, lockx, locky, lockz, lp)
{
	if (pData[playerid][pAttraction]==A_DERBY || pData[playerid][pAttraction]==A_RACE || pData[playerid][pAttraction]==A_CHOWANY   || pData[playerid][pAttraction]==A_WG || pData[playerid][pAttraction]==A_STRZELNICA  )  {
		Msg(playerid, COLOR_ERROR, "Nie mozesz tego teraz zrobic...", false);
		return;
	}

    pTemp[playerid][performingAnim] = true;
	ApplyAnimation(playerid, animlib, animname, Speed, looping, lockx, locky, lockz, lp, 1);
}


stock OnePlayAnim(playerid,animlib[],animname[], Float:Speed, looping, lockx, locky, lockz, lp)
{
	if (pData[playerid][pAttraction]==A_DERBY || pData[playerid][pAttraction]==A_RACE || pData[playerid][pAttraction]==A_CHOWANY   || pData[playerid][pAttraction]==A_WG || pData[playerid][pAttraction]==A_STRZELNICA) {
		Msg(playerid, COLOR_ERROR, "Nie mozesz tego teraz zrobic...", false);
		return;
	}
    pTemp[playerid][performingAnim] = true;
	ApplyAnimation(playerid, animlib, animname, Speed, looping, lockx, locky, lockz, lp, 1);
}


stock StopLoopingAnim(playerid)
{
    pTemp[playerid][performingAnim] = false;
	ApplyAnimation(playerid, "CARRY", "crry_prtial", 4.0, 0, 0, 0, 0, 0);
	ClearAnimations(playerid);
}


CMD:anims(playerid){
	new string[800];
	strcat(string,"/Rece     /Rece2    /Rece3    /Rece4 \n");
	strcat(string,"/Rece5    /Rece6    /Bar2     /Bar3 \n");
	strcat(string,"/Szafka   /Zegarek  /Lez      /Hide\n");
	strcat(string,"/Rzygaj   /Grubas   /Grubas2  /Taichi\n");
	strcat(string,"/Siadaj   /Chat     /Ratunku  /Kopniak\n");
	strcat(string,"/Dance    /Fucku    /Cellin   /Cellout\n");
	strcat(string,"/Pij      /Smoke    /Fsmoke   /Krzeslo\n");
	strcat(string,"/Krzeslo2 /Calus    /Trup     /Trup2\n");
	strcat(string,"/Wankin   /Wankout  /Deal     /Boks\n");
	strcat(string,"/Lol      /Bomba    /Aresztuj /Opalaj\n");
	strcat(string,"/Opalaj2  /Opalaj3  /Turlaj   /Klaps\n");
	strcat(string,"/Kradnij  /Kaleka   /Swat     /Swat2\n");
	strcat(string,"/Swat3    /Piwo     /Drunk    /Rap\n");
	strcat(string,"/Lookout  /Napad    /Papieros /Cpun\n");
	strcat(string,"/Cpun2    /Cpun3    /Cpun4    /Cpun5\n");
	strcat(string,"/Skok2    /Skok3    /Jedz     /Jedz2\n");
	strcat(string,"/Jedz3    /Wino     /Taniec   /Taniec2\n");
	strcat(string,"/Taniec3  /Taniec4  /Taniec5  /Taniec6\n");
	strcat(string,"/Taniec7  /Rolki    /Sprunk   /Inbedleft\n");
	strcat(string,"/Inbedright /Poddajsie  /Aresztowany  /Aresztuj2");

	ShowPlayerDialog(playerid,DIALOG_HELP_CMD_ANIMATIONS,0,"CMD> Animacje",string,"OK","OK");
	
	return 1;
}

CMD:bar2(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "BAR", "Barserve_bottle", 4.000000, 0, 1, 1, 1, -1);
	Msg(playerid, COLOR_INFO2, "Podajesz zimne piwko");
	return 1;
	#pragma unused cmdtext
}

CMD:bar3(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "BAR", "Barserve_give", 4.000000, 0, 1, 1, 1, -1);
	Msg(playerid, COLOR_INFO2, "Podajesz zimne piwko");
	return 1;
	#pragma unused cmdtext
}

CMD:opalaj(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "BEACH", "ParkSit_W_loop", 4.000000, 0, 1, 1, 1, -1);
	Msg(playerid, COLOR_INFO2, "Opalasz sie");
	return 1;
	#pragma unused cmdtext
}

CMD:opalaj2(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "BEACH", "SitnWait_loop_W", 4.000000, 0, 1, 1, 1, -1);
	Msg(playerid, COLOR_INFO2, "Opalasz sie");
	return 1;
	#pragma unused cmdtext
}

CMD:rozmowaauto(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "CAR_CHAT", "car_talkm_loop", 4.000000, 0, 1, 1, 1, -1);
	Msg(playerid, COLOR_INFO2, "Rozmawiasz z kims w aucie");
	return 1;
	#pragma unused cmdtext
}

CMD:szafka(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "COP_AMBIENT", "Copbrowse_nod", 4.000000, 0, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Bierzesz cos z szafki");
	return 1;
	#pragma unused cmdtext
}

CMD:zegarek(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "COP_AMBIENT", "Coplook_nod", 4.000000, 0, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Patrzysz na zegarek");
	return 1;
	#pragma unused cmdtext
}

CMD:cpun2(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "CRACK", "crckdeth1", 4.000000, 0, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Lezysz jak cpun");
	return 1;
	#pragma unused cmdtext
}

CMD:cpun3(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "CRACK", "crckdeth3", 4.000000, 0, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Lezysz jak cpun");
	return 1;
	#pragma unused cmdtext
}

CMD:cpun4(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "CRACK", "crckdeth4", 4.000000, 0, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Lezysz jak cpun");
	return 1;
	#pragma unused cmdtext
}

CMD:cpun5(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "CRACK", "crckidle4", 4.000000, 0, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Lezysz jak cpun");
	return 1;
	#pragma unused cmdtext
}

CMD:rolki(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "SKATE", "skate_sprint", 4.0999, 1, 1, 1, 1, 1);
	Msg(playerid, COLOR_INFO2, "Jedziesz na rolkach");
	return 1;
	#pragma unused cmdtext
}

/*CMD:skok3(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "DAM_JUMP", "Jump_Roll", 4.000000, 0, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Probujesz wykonac skok");
	return 1;
	#pragma unused cmdtext
}*/

CMD:taniec(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "DANCING", "bd_clap", 4.000000, 1, 1, 1, 1, 1);
	Msg(playerid, COLOR_INFO2, "Tanczysz");
	return 1;
	#pragma unused cmdtext
}

CMD:taniec2(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "DANCING", "bd_clap1", 4.000000, 1, 1, 1, 1, 1);
	Msg(playerid, COLOR_INFO2, "Tanczysz");
	return 1;
	#pragma unused cmdtext
}

CMD:taniec3(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "DANCING", "DAN_Down_A", 4.000000, 1, 1, 1, 1, 1);
	Msg(playerid, COLOR_INFO2, "Tanczysz");
	return 1;
	#pragma unused cmdtext
}

CMD:taniec4(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "DANCING", "DAN_Left_A", 4.000000, 1, 1, 1, 1, 1);
	Msg(playerid, COLOR_INFO2, "Tanczysz");
	return 1;
	#pragma unused cmdtext
}

CMD:taniec5(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "DANCING", "DAN_Right_A", 4.000000, 1, 1, 1, 1, 1);
	Msg(playerid, COLOR_INFO2, "Tanczysz");
	return 1;
	#pragma unused cmdtext
}

CMD:taniec6(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "DANCING", "DAN_Up_A", 4.000000, 1, 1, 1, 1, 1);
	Msg(playerid, COLOR_INFO2, "Tanczysz");
	return 1;
	#pragma unused cmdtext
}

CMD:taniec7(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "DANCING", "dnce_M_a", 4.000000, 1, 1, 1, 1, 1);
	Msg(playerid, COLOR_INFO2, "Tanczysz");
	return 1;
	#pragma unused cmdtext
}

CMD:grubas(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "FAT", "FatWalk", 4.000000, 1, 1, 1, 1, 1);
	Msg(playerid, COLOR_INFO2, "Idziesz jak grubas");
	return 1;
	#pragma unused cmdtext
}

CMD:grubas2(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "FAT", "FatRun", 4.000000, 1, 1, 1, 1, 1);
	Msg(playerid, COLOR_INFO2, "Biegniesz jak grubas");
	return 1;
	#pragma unused cmdtext
}

CMD:jedz2(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "FOOD", "FF_Die_Bkw", 4.000000, 0, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Jesz cos");
	return 1;
	#pragma unused cmdtext
}

CMD:jedz3(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "FOOD", "FF_Sit_Eat1", 4.000000, 0, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Jesz cos");
	return 1;
	#pragma unused cmdtext
}

CMD:rece4(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "GANGS", "hndshkfa", 4.000000, 0, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Wymachyjesz Rekami");
	return 1;
	#pragma unused cmdtext
}

CMD:rece5(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "GHANDS", "gsign1", 4.000000, 0, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Wymachyjesz Rekami");
	return 1;
	#pragma unused cmdtext
}

CMD:rece6(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "GHANDS", "gsign1LH", 4.000000, 0, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Wymachyjesz Rekami");
	return 1;
	#pragma unused cmdtext
}

CMD:krzeslo2(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "JST_BUISNESS", "girl_02", 4.000000, 1, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Siadasz na krzesle jak kobieta");
	return 1;
	#pragma unused cmdtext
}

CMD:turlaj(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "MD_CHASE", "MD_BIKE_Lnd_Roll_F", 4.000000, 1, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Turlasz sie xD");
	return 1;
	#pragma unused cmdtext
}

CMD:boks(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "GYMNASIUM", "GYMshadowbox", 4.000000, 1, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Trenujesz boks");
	return 1;
	#pragma unused cmdtext
}

CMD:kopniak(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "POLICE", "Door_Kick", 4.000000, 0, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Kopiesz :P");
	return 1;
	#pragma unused cmdtext
}

CMD:opalaj3(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "SUNBATHE", "ParkSit_W_idleA", 4.000000, 0, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Opalasz sie");
	return 1;
	#pragma unused cmdtext
}

CMD:swat(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "SWAT", "swt_breach_01", 4.000000, 0, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Biegasz jak SWAT");
	return 1;
	#pragma unused cmdtext
}

CMD:swat2(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "SWAT", "swt_breach_02", 4.000000, 0, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Biegasz jak SWAT");
	return 1;
	#pragma unused cmdtext
}

CMD:swat3(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "SWAT", "swt_breach_03", 4.000000, 0, 1, 1, 1, 0);
	Msg(playerid, COLOR_INFO2, "Biegasz jak SWAT");
	return 1;
	#pragma unused cmdtext
}

/*CMD:skok2(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "DAM_JUMP","DAM_Launch",4.1,0,1,1,1,1); // Umierasz
	Msg(playerid, COLOR_INFO2, "Robisz efektowny skok");
	return 1;
	#pragma unused cmdtext
}*/

CMD:trup2(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "SWAT","gnstwall_injurd",4.0,1,0,0,0,0); // Umierasz
	Msg(playerid, COLOR_INFO2, "Udajesz trupa");
	return 1;
	#pragma unused cmdtext
}

CMD:ratunku(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid,"BD_FIRE","BD_Panic_03",4.0,1,0,0,0,0); // Ratunku
	Msg(playerid, COLOR_INFO2, "Machasz o pomoc!");
	return 1;
	#pragma unused cmdtext
}

CMD:trup(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "WUZI", "CS_Dead_Guy", 4.1,0,1,1,1,1); // Umierasz
	Msg(playerid, COLOR_INFO2, "Udajesz trupa");
	return 1;
	#pragma unused cmdtext
}

CMD:calus(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "KISSING", "Playa_Kiss_02", 4.0,0,0,0,0,0); // CAlOWANIE
	Msg(playerid, COLOR_INFO2, "Calujesz kogos");
	return 1;
	#pragma unused cmdtext
}

CMD:sikaj(playerid,cmdtext[]){//Na Dole Mapy!
	SetPlayerSpecialAction(playerid,68);
	Msg(playerid, COLOR_INFO2, "Odlewasz sie");
	return 1;
	#pragma unused cmdtext
}

CMD:krzeslo(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid,"INT_OFFICE","OFF_Sit_Idle_Loop",4.0,1,0,0,0,0); // Krzeslo
	Msg(playerid, COLOR_INFO2, "Siadasz na krzesle");
	return 1;
	#pragma unused cmdtext
}

CMD:rece(playerid,cmdtext[]){//Na Dole Mapy!
	SetPlayerSpecialAction(playerid,SPECIAL_ACTION_HANDSUP);
	Msg(playerid, COLOR_INFO2, "Poddajesz sie");
	return 1;
	#pragma unused cmdtext
}

CMD:wankin(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "PAULNMAC", "wank_loop", 4.0,1,0,0,0,0); // Walenie konia
	Msg(playerid, COLOR_INFO2, "Walisz konia");
	return 1;
	#pragma unused cmdtext
}

CMD:wankout(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "PAULNMAC", "wank_out", 4.0, 0, 0, 0, 0, 0); // Wytrysk
	Msg(playerid, COLOR_INFO2, "Robisz wytrysk xD");
	return 1;
	#pragma unused cmdtext
}

CMD:cellin(playerid,cmdtext[]){//Na Dole Mapy!
	SetPlayerSpecialAction(playerid,SPECIAL_ACTION_USECELLPHONE);
	Msg(playerid, COLOR_INFO2, "Dzwonisz do mamy");
	return 1;
	#pragma unused cmdtext
}

CMD:piwo(playerid,cmdtext[]){//Na Dole Mapy!
	SetPlayerDrunkLevel(playerid,10000);
	SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DRINK_BEER);
	SetPlayerFightingStyle(playerid, FIGHT_STYLE_ELBOW);
	Msg(playerid, COLOR_INFO2, "Pijesz Piwo!");
	
	return 1;
	#pragma unused cmdtext
}

CMD:wino(playerid,cmdtext[]){//Na Dole Mapy!
	SetPlayerDrunkLevel(playerid,1000000);
	SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DRINK_WINE);
	SetPlayerFightingStyle(playerid, FIGHT_STYLE_GRABKICK);
	Msg(playerid, COLOR_INFO2, "Pijesz Wino!");
	
	return 1;
	#pragma unused cmdtext
}

CMD:papieros(playerid,cmdtext[]){//Na Dole Mapy!
	SetPlayerSpecialAction(playerid,SPECIAL_ACTION_SMOKE_CIGGY);
	Msg(playerid, COLOR_INFO2, "Palisz Papierosa!");
	return 1;
	#pragma unused cmdtext
}

CMD:sprunk(playerid,cmdtext[]){//Na Dole Mapy!
	SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DRINK_SPRUNK);
	SetPlayerDrunkLevel(playerid, 0);
	Msg(playerid, COLOR_INFO2, "Pijesz Sprunka!");
	return 1;
	#pragma unused cmdtext
}

CMD:cellout(playerid,cmdtext[]){//Na Dole Mapy!
	SetPlayerSpecialAction(playerid,SPECIAL_ACTION_STOPUSECELLPHONE);
	Msg(playerid, COLOR_INFO2, "Odkladasz telefon ");
	return 1;
	#pragma unused cmdtext
}

CMD:drunk(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid,"PED","WALK_DRUNK",4.0,1,1,1,1,1);
	Msg(playerid, COLOR_INFO2, "Chodzisz jak pijany(a)");
	return 1;
	#pragma unused cmdtext
}

CMD:bomb(playerid,cmdtext[]){//Na Dole Mapy!
	ClearAnimations(playerid);
	OnePlayAnim(playerid, "BOMBER", "BOM_Plant", 4.0, 0, 0, 0, 0, 0); // Podkladasz bombe.
	Msg(playerid, COLOR_INFO2, "Podkladasz bombe");
	return 1;
	#pragma unused cmdtext
}

CMD:aresztuj(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid,"ped", "ARRESTgun", 4.0, 0, 1, 1, 1, -1); // aresztujesz kogos.
	Msg(playerid, COLOR_INFO2, "Aresztujesz kogos");
	return 1;
	#pragma unused cmdtext
}

CMD:rap(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "RAPPING", "RAP_B_Loop", 4.0,1,0,0,0,0); // Rapujesz
	Msg(playerid, COLOR_INFO2, "Rapujesz ");
	return 1;
	#pragma unused cmdtext
}

CMD:lookout(playerid,cmdtext[]){//Na Dole Mapy!
	OnePlayAnim(playerid, "SHOP", "ROB_Shifty", 4.0, 0, 0, 0, 0, 0); // Patrzysz czy nie ma policji.
	Msg(playerid, COLOR_INFO2, "Rozgladasz sie");
	return 1;
	#pragma unused cmdtext
}

CMD:napad(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "SHOP", "ROB_Loop_Threat", 4.0, 1, 0, 0, 0, 0); // Napad
	Msg(playerid, COLOR_INFO2, "Robisz napad");
	return 1;
	#pragma unused cmdtext
}

CMD:rece2(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "COP_AMBIENT", "Coplook_loop", 4.0, 0, 1, 1, 1, -1); // Rece
	Msg(playerid, COLOR_INFO2, "Zakladasz rece");
	return 1;
	#pragma unused cmdtext
}

CMD:lez(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid,"SUNBATHE","batherdown",4.1,0,1,1,1,1); // Lez
	Msg(playerid, COLOR_INFO2, "Lezysz ");
	return 1;
	#pragma unused cmdtext
}

CMD:hide(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "ped", "cower", 3.0, 1, 0, 0, 0, 0); // Oslaniasz sie.
	Msg(playerid, COLOR_INFO2, "Oslaniasz sie. ");
	return 1;
	#pragma unused cmdtext
}

CMD:rzygaj(playerid,cmdtext[]){//Na Dole Mapy!
	OnePlayAnim(playerid, "FOOD", "EAT_Vomit_P", 3.0, 0, 0, 0, 0, 0); // Rzygasz
	Msg(playerid, COLOR_INFO2, "Blee! ");
	return 1;
	#pragma unused cmdtext
}

CMD:jedz(playerid,cmdtext[]){//Na Dole Mapy!
	OnePlayAnim(playerid, "FOOD", "EAT_Burger", 3.0, 0, 0, 0, 0, 0); // jesz buregera
	Msg(playerid, COLOR_INFO2, "Jesz cos ");
	return 1;
	#pragma unused cmdtext
}

CMD:deal(playerid,cmdtext[]){//Na Dole Mapy!
	OnePlayAnim(playerid, "DEALER", "DEALER_DEAL", 4.0, 0, 0, 0, 0, 0); // Dilujesz
	Msg(playerid, COLOR_INFO2, "Dealujesz z kims ");
	return 1;
	#pragma unused cmdtext
}

CMD:cpun(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid, "CRACK", "crckdeth2", 4.0, 1, 0, 0, 0, 0); // Cpun
	Msg(playerid, COLOR_INFO2, "Lezysz jak cpun");
	return 1;
	#pragma unused cmdtext
}

CMD:smoke(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid,"SMOKING","M_smklean_loop",4.0, 1, 0, 0, 0, 0); // Palisz
	Msg(playerid, COLOR_INFO2, "Palisz papierosa");
	return 1;
	#pragma unused cmdtext
}

CMD:fsmoke(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid,"SMOKING","F_smklean_loop",4.0, 1, 0, 0, 0, 0); // Palisz
	Msg(playerid, COLOR_INFO2, "Palisz papierosa jak kobieta ");
	return 1;
	#pragma unused cmdtext
}

CMD:smoke2(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid,"SMOKING","M_smkstnd_loop",4.0, 1, 0, 0, 0, 0); // Palisz
	Msg(playerid, COLOR_INFO2, "Palisz papierosa");
	return 1;
	#pragma unused cmdtext
}

CMD:rece3(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid,"DAM_JUMP","DAM_Dive_Loop",4.0, 1, 0, 0, 0, 0); // Palisz
	Msg(playerid, COLOR_INFO2, "Stoisz na rekach");
	return 1;
	#pragma unused cmdtext
}

CMD:siadaj(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid,"BEACH", "ParkSit_M_loop", 4.0, 1, 0, 0, 0, 0); // Siadasz
	Msg(playerid, COLOR_INFO2, "Siadasz");
	return 1;
	#pragma unused cmdtext
}

CMD:chat(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid,"PED","IDLE_CHAT", 4.0,1,0,0,0,0);
	Msg(playerid, COLOR_INFO2, "Rozmawiasz z kims");
	return 1;
	#pragma unused cmdtext
}

CMD:fucku(playerid,cmdtext[]){//Na Dole Mapy!
	OnePlayAnim(playerid,"PED","fucku",4.0,0,0,0,0,0);
	Msg(playerid, COLOR_INFO2, "Pokazujesz SPIERDALAJ ");
	return 1;
	#pragma unused cmdtext
}

CMD:taichi(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid,"PARK","Tai_Chi_Loop",4.0,1,0,0,0,0);
	Msg(playerid, COLOR_INFO2, "Umiesz takewondo ");
	return 1;
	#pragma unused cmdtext
}

CMD:pij(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid,"BAR","dnk_stndF_loop",4.0,1,0,0,0,0);
	Msg(playerid, COLOR_INFO2, "Pijesz cos ");
	return 1;
	#pragma unused cmdtext
}

CMD:inbedright(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid,"INT_HOUSE","BED_In_R",4.1,0,1,1,1,1);
	Msg(playerid, COLOR_INFO2, "Idzisz spac");
	return 1;
	#pragma unused cmdtext
}

CMD:inbedleft(playerid,cmdtext[]){//Na Dole Mapy!
	LoopingAnim(playerid,"INT_HOUSE","BED_In_L",4.1,0,1,1,1,1);
	Msg(playerid, COLOR_INFO2, "Idziesz spac");
	return 1;
	#pragma unused cmdtext
}

CMD:dance(playerid,cmdtext[]){//Na Dole Mapy!
	new dance;
	if(sscanf(cmdtext,"d",dance)){
	    Msg(playerid,COLOR_ERROR,"U¿yj: /dance [1-4]");
		return 1;
	}
	
	if(dance < 1 || dance > 4) {
		Msg(playerid,COLOR_ERROR,"U¿yj: /dance [1-4]");
		return 1;
	}
	
	if(dance == 1) {
		SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE1);
	} else if(dance == 2) {
		SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE2);
	} else if(dance == 3) {
		SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE3);
	} else if(dance == 4) {
		SetPlayerSpecialAction(playerid,SPECIAL_ACTION_DANCE4);
	}
	
	return 1;
}
