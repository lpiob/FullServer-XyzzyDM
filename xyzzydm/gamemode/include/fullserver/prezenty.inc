/* prezenty: walizki, paczki
 * Lukasz Biegaj, 2011
 * 
 */

// WALIZKA: 13% szans na bombe, respekt od 0 do 9, kasy 0-250k
// PREZENt: 14% szans na bombe, respekt od 2 do 12, kasy 0-150k
// PODKOWA: bez bomb, respekt 0-4, kasa 0-350k

#define MAX_GIFT_PICKUPS	3
#define GIFT_WALIZKA	0
#define	GIFT_PREZENT	1
#define GIFT_PODKOWA	2

enum e_Prezenty
{
    bool:epAktywne,
    epPickup,
    epDynamicArea,
    Float:epXYZ[3],    
    epInterior,
    epVirtualWorld,
    epOpis[256],
    epDid,
    epCash,
    epRespect
    
}

new
 gPrezentyPickup[MAX_GIFT_PICKUPS][e_Prezenty],
 gPrezentyAudio[MAX_PLAYERS];

CMD:walizka(playerid,params[]){
    if(!IsGM(playerid)) {
		if (!gPrezentyPickup[GIFT_WALIZKA][epAktywne])
			return Msg(playerid,COLOR_INFO,"Nie ma obecnie zadnej walizki do odnalezienia");	// obecnie nie ma walizki do znalezenia
		else
			return Msg(playerid,COLOR_INFO,gPrezentyPickup[GIFT_WALIZKA][epOpis]);
	}

	if (IsGM(playerid) && strcmp(params,"usun",false)==0) {
		if (Bonus_DestroyPickup(GIFT_WALIZKA))
			return Msg(playerid,COLOR_INFO,"Walizka usunieta");
		else
			return Msg(playerid,COLOR_INFO,"Nie ma obecnie zadnej walizki!");
	}
	if (IsGM(playerid) && strcmp(params,"tp",false)==0) {
		if (gPrezentyPickup[GIFT_WALIZKA][epAktywne])
			Teleport(T_PLAYER, playerid, gPrezentyPickup[GIFT_WALIZKA][epXYZ][0], 	gPrezentyPickup[GIFT_WALIZKA][epXYZ][1], gPrezentyPickup[GIFT_WALIZKA][epXYZ][2], FLOAT_NAN, gPrezentyPickup[GIFT_WALIZKA][epInterior], gPrezentyPickup[GIFT_WALIZKA][epVirtualWorld]);
		else
			Msg(playerid,COLOR_ERROR, "Nie ma obecnie walizki na mapie");
		return 1;
	}
	new podpowiedz[200];
	if(sscanf(params,"s[200]",podpowiedz)) {
		if (!gPrezentyPickup[GIFT_WALIZKA][epAktywne])
			Msg(playerid,COLOR_INFO,"Obecnie nie ma na mapie zadnej walizki.");
		else
			Msg(playerid,COLOR_INFO,gPrezentyPickup[GIFT_WALIZKA][epOpis]);

		return Msg(playerid,COLOR_ERROR,"/walizka <podpowiedz/tp/usun>");
	}

	new bool:zaevent;
	if (strfind(podpowiedz,"event",true)>=0)	zaevent=true;
	if (!zaevent && strfind(podpowiedz,"ztp",true)>=0)	zaevent=true;
	if (!zaevent && strfind(podpowiedz,"ksw",true)>=0)	zaevent=true;
	if (!zaevent && strfind(podpowiedz,"wwk",true)>=0)	zaevent=true;
	if (!zaevent && strfind(podpowiedz,"za ev",true)>=0)	zaevent=true;
	if (!zaevent && strfind(podpowiedz,"dla id ",true)>=0)	zaevent=true;

	if (nowyPrezent(playerid,GIFT_WALIZKA)) {
		format(gPrezentyPickup[GIFT_WALIZKA][epOpis],256,"~w~%s zgubil%s walizke.~n~Podpowiedz: ~r~%s", GetPlayerProperName(playerid),Kobieta(playerid)?("a"):(""),podpowiedz);
		if (zaevent)
			ShowAnnouncement2(gPrezentyPickup[GIFT_WALIZKA][epOpis]);
		else
			ShowAnnouncement(gPrezentyPickup[GIFT_WALIZKA][epOpis]);
		// GameTextForAll(gPrezentyPickup[GIFT_WALIZKA][epOpis],7000,5);
		format(gPrezentyPickup[GIFT_WALIZKA][epOpis],256,"%s zgubil%s walizke. Podpowiedz: %s", GetPlayerProperName(playerid),Kobieta(playerid)?("a"):(""),podpowiedz);
	}
    return 1;
}

CMD:prezent(playerid,params[]){	
    if(!IsGM(playerid)) {
		if (!gPrezentyPickup[GIFT_PREZENT][epAktywne])
			return Msg(playerid,COLOR_INFO,"Nie ma obecnie zadnego prezentu na mapie.");	// obecnie nie ma prezentu do znalezenia
		else
			return Msg(playerid,COLOR_INFO,gPrezentyPickup[GIFT_PREZENT][epOpis]);
	}
	if (IsGM(playerid) && strcmp(params,"usun",false)==0) {
		if (Bonus_DestroyPickup(GIFT_PREZENT))
			return Msg(playerid,COLOR_INFO,"Prezent usuniety");
		else
			return Msg(playerid,COLOR_INFO,"Nie ma obecnie zadnego prezentu!");
	}
	if (IsGM(playerid) && strcmp(params,"tp",false)==0) {
		if (gPrezentyPickup[GIFT_PREZENT][epAktywne])
			Teleport(T_PLAYER, playerid, gPrezentyPickup[GIFT_PREZENT][epXYZ][0], 	gPrezentyPickup[GIFT_PREZENT][epXYZ][1], gPrezentyPickup[GIFT_PREZENT][epXYZ][2], FLOAT_NAN, gPrezentyPickup[GIFT_PREZENT][epInterior], gPrezentyPickup[GIFT_PREZENT][epVirtualWorld]);
		else
			Msg(playerid,COLOR_ERROR, "Nie ma obecnie prezentu na mapie");
		return 1;
	}

	new podpowiedz[200];
	if(sscanf(params,"s[200]",podpowiedz)) {
		if (!gPrezentyPickup[GIFT_PREZENT][epAktywne])
			Msg(playerid,COLOR_INFO,"Obecnie nie ma na mapie zadnego prezentu.");
		else
			Msg(playerid,COLOR_INFO,gPrezentyPickup[GIFT_PREZENT][epOpis]);

		return Msg(playerid,COLOR_ERROR,"/prezent <podpowiedz/tp/usun>");
	}

	new bool:zaevent;
	if (strfind(podpowiedz,"event",true)>=0)	zaevent=true;
	if (!zaevent && strfind(podpowiedz,"ztp",true)>=0)	zaevent=true;
	if (!zaevent && strfind(podpowiedz,"ksw",true)>=0)	zaevent=true;
	if (!zaevent && strfind(podpowiedz,"wwk",true)>=0)	zaevent=true;
	if (!zaevent && strfind(podpowiedz,"za ev",true)>=0)	zaevent=true;
	if (!zaevent && strfind(podpowiedz,"dla id ",true)>=0)	zaevent=true;

	if (nowyPrezent(playerid,GIFT_PREZENT)) {
		format(gPrezentyPickup[GIFT_PREZENT][epOpis],256,"~w~%s zgubil%s prezent.~n~Podpowiedz: ~b~%s", GetPlayerProperName(playerid),Kobieta(playerid)?("a"):(""),podpowiedz);
		if (zaevent)
			ShowAnnouncement2(gPrezentyPickup[GIFT_PREZENT][epOpis]);
		else
			ShowAnnouncement(gPrezentyPickup[GIFT_PREZENT][epOpis]);
		//GameTextForAll(gPrezentyPickup[GIFT_PREZENT][epOpis],7000,5);
		format(gPrezentyPickup[GIFT_PREZENT][epOpis],256,"%s zgubil%s prezent.~n~Podpowiedz: %s",GetPlayerProperName(playerid),Kobieta(playerid)?("a"):(""),podpowiedz);
	}
	return 1;
}

CMD:podkowa(playerid,params[]){
    if(!IsGM(playerid)) {
		if (!gPrezentyPickup[GIFT_PODKOWA][epAktywne])
			return Msg(playerid,COLOR_INFO,TXT(playerid,462));	// obecnie nie ma prezentu do znalezenia
		else
			return Msg(playerid,COLOR_INFO,gPrezentyPickup[GIFT_PODKOWA][epOpis]);
	}

	if (IsGM(playerid) && strcmp(params,"usun",false)==0) {
		if (Bonus_DestroyPickup(GIFT_PODKOWA))
			return Msg(playerid,COLOR_INFO,"Podkowa usunieta");
		else
			return Msg(playerid,COLOR_INFO,"Nie ma obecnie zadnej podkowy!");
	}
	if (IsGM(playerid) && strcmp(params,"tp",false)==0) {
		if (gPrezentyPickup[GIFT_PREZENT][epAktywne])
			Teleport(T_PLAYER, playerid, gPrezentyPickup[GIFT_PODKOWA][epXYZ][0], 	gPrezentyPickup[GIFT_PODKOWA][epXYZ][1], gPrezentyPickup[GIFT_PODKOWA][epXYZ][2], FLOAT_NAN, gPrezentyPickup[GIFT_PODKOWA][epInterior], gPrezentyPickup[GIFT_PODKOWA][epVirtualWorld]);
		else
			Msg(playerid,COLOR_ERROR, "Nie ma obecnie podkowy na mapie");
		return 1;
	}		

	new podpowiedz[200];
	if(sscanf(params,"s[200]",podpowiedz)) {
		if (!gPrezentyPickup[GIFT_PODKOWA][epAktywne])
			Msg(playerid,COLOR_INFO,"Obecnie nie ma na mapie zadnej podkowy.");
		else
			Msg(playerid,COLOR_INFO,gPrezentyPickup[GIFT_PODKOWA][epOpis]);

		return Msg(playerid,COLOR_ERROR,"/podkowa {b}<podpowiedz/tp/usun>{/b}");
	}

	new bool:zaevent;
	if (strfind(podpowiedz,"event",true)>=0)	zaevent=true;
	if (!zaevent && strfind(podpowiedz,"ztp",true)>=0)	zaevent=true;
	if (!zaevent && strfind(podpowiedz,"ksw",true)>=0)	zaevent=true;
	if (!zaevent && strfind(podpowiedz,"wwk",true)>=0)	zaevent=true;
	if (!zaevent && strfind(podpowiedz,"za ev",true)>=0)	zaevent=true;
	if (!zaevent && strfind(podpowiedz,"dla id ",true)>=0)	zaevent=true;

	if (nowyPrezent(playerid,GIFT_PODKOWA)) {
		format(gPrezentyPickup[GIFT_PODKOWA][epOpis],256,"~w~%s ukryl%s gdzies podkowe.~n~Podpowiedz: ~b~%s",GetPlayerProperName(playerid),Kobieta(playerid)?("a"):(""),podpowiedz);
		if (zaevent)
			ShowAnnouncement2(gPrezentyPickup[GIFT_PODKOWA][epOpis]);
		else
			ShowAnnouncement(gPrezentyPickup[GIFT_PODKOWA][epOpis]);
		//GameTextForAll(gPrezentyPickup[GIFT_PODKOWA][epOpis],7000,5);
		format(gPrezentyPickup[GIFT_PODKOWA][epOpis],256,"%s ukryl%s gdzies podkowe.~n~Podpowiedz: %s",GetPlayerProperName(playerid),Kobieta(playerid)?("a"):(""),podpowiedz);
	}
	return 1;

}

nowyPrezent(playerid,r){
    if (gPrezentyPickup[r][epAktywne]) {
		Msg(playerid,COLOR_ERROR,"Na mapie znajduje sie juz walizka/prezent.");

		return 0;
	}

    GetPlayerPos(playerid,gPrezentyPickup[r][epXYZ][0],gPrezentyPickup[r][epXYZ][1],gPrezentyPickup[r][epXYZ][2]);
    gPrezentyPickup[r][epInterior]=GetPlayerInterior(playerid);
    gPrezentyPickup[r][epVirtualWorld]=GetPlayerVirtualWorld(playerid);

	for(new i=0;i<MAX_GIFT_PICKUPS;i++) {
		if (r==i || gPrezentyPickup[i][epAktywne]==false ||
			gPrezentyPickup[i][epInterior]!=gPrezentyPickup[r][epInterior] || gPrezentyPickup[i][epVirtualWorld]!=gPrezentyPickup[r][epVirtualWorld]) continue;
		// aktywny bonus	

		if (GetDistanceBetweenPoints3D(
				gPrezentyPickup[r][epXYZ][0], gPrezentyPickup[r][epXYZ][1], gPrezentyPickup[r][epXYZ][2],
				gPrezentyPickup[i][epXYZ][i], gPrezentyPickup[i][epXYZ][1], gPrezentyPickup[i][epXYZ][2]) <= 80.0) {
			Msg(playerid,COLOR_ERROR,"W poblizu znajduje sie inny bonus, nie mozesz tu postawic drugiego");
			return 0;
			}
	}


	switch(r){
		case GIFT_WALIZKA:{
			gPrezentyPickup[r][epPickup]=CreateDynamicPickup(1210,1,gPrezentyPickup[r][epXYZ][0],gPrezentyPickup[r][epXYZ][1],gPrezentyPickup[r][epXYZ][2],gPrezentyPickup[r][epVirtualWorld],gPrezentyPickup[r][epInterior],-1,200);
		    gPrezentyPickup[r][epRespect]=random(13)-3;		// dlaczego losujemy przy tworzeniu walizki?
			gPrezentyPickup[r][epCash]=random(250000);		// zeby umozliwic wlascicielowi artefaktu widzenie zawartosci
			// gdy respect==-1 => bomba
		}
		case GIFT_PREZENT:{
			gPrezentyPickup[r][epPickup]=CreateDynamicPickup(19054+random(5),1,gPrezentyPickup[r][epXYZ][0],gPrezentyPickup[r][epXYZ][1],gPrezentyPickup[r][epXYZ][2],gPrezentyPickup[r][epVirtualWorld],gPrezentyPickup[r][epInterior],-1,200);
			gPrezentyPickup[r][epRespect]=random(7);
			if (gPrezentyPickup[r][epRespect]==0) gPrezentyPickup[r][epRespect]=-1; 
			else gPrezentyPickup[r][epRespect]*=2;
			gPrezentyPickup[r][epCash]=random(150000);
		}
		case GIFT_PODKOWA:{
			gPrezentyPickup[r][epPickup]=CreateDynamicPickup(954,1,gPrezentyPickup[r][epXYZ][0],gPrezentyPickup[r][epXYZ][1],gPrezentyPickup[r][epXYZ][2],gPrezentyPickup[r][epVirtualWorld],gPrezentyPickup[r][epInterior],-1,200);
			gPrezentyPickup[r][epRespect]=random(5);
			gPrezentyPickup[r][epCash]=random(350000);
		}
	}

    gPrezentyPickup[r][epAktywne]=true;
    gPrezentyPickup[r][epDynamicArea]=CreateDynamicSphere(gPrezentyPickup[r][epXYZ][0],gPrezentyPickup[r][epXYZ][1],gPrezentyPickup[r][epXYZ][2],75,gPrezentyPickup[r][epVirtualWorld],gPrezentyPickup[r][epInterior],-1);

    gPrezentyPickup[r][epDid]=playerid;


	return 1;
}

Bonus_DestroyPickup(pickuptype){
	if (!gPrezentyPickup[pickuptype][epAktywne]) return false;
	DestroyDynamicPickup(gPrezentyPickup[pickuptype][epPickup]);
	gPrezentyPickup[pickuptype][epAktywne]=false;

	foreach(userid)
		if (IsPlayerInDynamicArea(userid,gPrezentyPickup[pickuptype][epDynamicArea]) && Audio_IsClientConnected(userid))
			Audio_Stop(userid, gPrezentyAudio[userid]);

	DestroyDynamicArea(gPrezentyPickup[pickuptype][epDynamicArea]);
	return 1;
}

prezenty_OPEnterDynamicArea(playerid,areaid){
    // prezenty (walizka,paczka,...)
    for (new i=0;i<MAX_GIFT_PICKUPS;i++)	
	if (gPrezentyPickup[i][epAktywne] && gPrezentyPickup[i][epDynamicArea]==areaid) {	// gracz znajduje sie w zasiegu prezentu
	    if (!Audio_IsClientConnected(playerid)) return 1;
		if (gPrezentyAudio[playerid]) Audio_Stop(playerid,gPrezentyAudio[playerid]);
	    gPrezentyAudio[playerid]=Audio_Play(playerid,AUDIOID_WALIZKA+i,false,true,true);
	    Audio_Set3DPosition(playerid, gPrezentyAudio[playerid], gPrezentyPickup[i][epXYZ][0], gPrezentyPickup[i][epXYZ][1], gPrezentyPickup[i][epXYZ][2], 50);
	    return 1;
	}
    return 0;
}

prezenty_OPLeaveDynamicArea(playerid,areaid){
    for (new i=0;i<MAX_GIFT_PICKUPS;i++)
	if (gPrezentyPickup[i][epDynamicArea]==areaid) {	// gracz znajduje sie w zasiegu prezentu
	    if (!Audio_IsClientConnected(playerid)) return 1;
	    Audio_Stop(playerid, gPrezentyAudio[playerid]);
	    return 1;
	}
    return 0;
}

prezenty_OPPickUpDynamicPickup(playerid,pickupid){
    for (new i=0;i<MAX_GIFT_PICKUPS;i++)
    if (gPrezentyPickup[i][epAktywne] && pickupid==gPrezentyPickup[i][epPickup]) {
		if (IsGM(playerid)) return 1;
//		DestroyDynamicPickup(gPrezentyPickup[i][epPickup]);

		new str[127];

		if (gPrezentyPickup[i][epRespect]<0) {	// bomba
			format(str,sizeof(str),"{ffffff}%s {cccccc}odnalazl/-a bonus, niestety miala wybuchowa zawartosc!",GetPlayerProperName(playerid));
			CreateExplosion(gPrezentyPickup[i][epXYZ][0]+random(1)-0.5, gPrezentyPickup[i][epXYZ][1]+random(1)-0.5, gPrezentyPickup[i][epXYZ][2]+random(1)-0.5, 2, 50);
			CreateExplosion(gPrezentyPickup[i][epXYZ][0]+random(1)-0.5, gPrezentyPickup[i][epXYZ][1]+random(1)-0.5, gPrezentyPickup[i][epXYZ][2]+random(1)-0.5, 2, 50);
			CreateExplosion(gPrezentyPickup[i][epXYZ][0]+random(1)-0.5, gPrezentyPickup[i][epXYZ][1]+random(1)-0.5, gPrezentyPickup[i][epXYZ][2]+random(1)-0.5, 2, 50);
		} else {
			if (SkinKobiecy(GetPlayerSkin(playerid)))
				format(str,sizeof(str),"{ffffff}%s {cccccc}odnalazla bonus i otrzymala {ffffff}%d {cccccc}gotowki i {ffffff}%d {cccccc}respektu",GetPlayerProperName(playerid), gPrezentyPickup[i][epCash], gPrezentyPickup[i][epRespect]);
			else
				format(str,sizeof(str),"{ffffff}%s {cccccc}odnalazl bonus i otrzymal {ffffff}%d {cccccc}gotowki i {ffffff}%d {cccccc}respektu",GetPlayerProperName(playerid), gPrezentyPickup[i][epCash], gPrezentyPickup[i][epRespect]);

	//		pData[playerid][respect] += gPrezentyPickup[i][epRespect];
	//		SetPlayerScore(playerid, pData[playerid][respect]);
			GivePlayerScore(playerid, gPrezentyPickup[i][epRespect],"walizka/podkowa/prezent");
		}

		SendClientMessageToAll(0xccccccff,str);

		GivePlayerMoney(playerid, gPrezentyPickup[i][epCash]);

		avt_record(playerid,e_walizki,1,ART_ADD);

//		gPrezentyPickup[i][epAktywne]=false;

//		foreach(userid)
//			if (IsPlayerInDynamicArea(userid,gPrezentyPickup[i][epDynamicArea]) && Audio_IsClientConnected(userid))
//				Audio_Stop(userid, gPrezentyAudio[userid]);
//		DestroyDynamicArea(gPrezentyPickup[i][epDynamicArea]);
		Bonus_DestroyPickup(i);

		
		return 1;
    }
    return 0;
}