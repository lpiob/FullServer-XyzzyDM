#define MAX_HOUSES 280
#define DIALOG_DOMY		3137
#define DIALOG_DOMY_OUTSIDEPICKUP	(DIALOG_DOMY+0)
#define DIALOG_DOMY_CZYNSZ	(DIALOG_DOMY+1)
#define DIALOG_DOMY_CZYNSZ_CONFIRM	(DIALOG_DOMY+2)
#define DIALOG_DOMY_INFO			(DIALOG_DOMY+4)
#define DIALOG_DOMY_SPRZEDAZ		(DIALOG_DOMY+5)

enum e_domy{
	ed_id,
	ed_pickup,
	ed_cp,
	Text3D:ed_3dtext,
	ed_mapicon,
	ed_koszt,
	ed_owner,
	ed_ownernick[32],
	Float:ed_entrance[3],	// XYZ
	Float:ed_exit[4],		// XYZA
	Float:ed_home[4],		// XYZA
	ed_homei,				// interior
	ed_homevw,				// vw - domyslnie VW_DOMY+ID
	ed_audioURL[128],

	ed_daysLeft,
	Float:ed_paidTo[12],
	bool:ed_open,
	ed_vehiclesallowed,
	Float:ed_vehicleradius,
	bool:ehv_active,
	ehv_id,
	ehv_model,
	Float:ehv_spawnPos[4],
	ehv_color[2],
	ehv_plate[32],
	ehv_components[14],

	ed_restrict_gang
}

new FSDomy[MAX_HOUSES][e_domy];
new IloscDomow=0;

domy_Reload(){
	new dane[128];
//	format(dane,sizeof dane,"SELECT h.id,h.X,h.Y,h.Z,h.exitX,h.exitY,h.exitZ,h.exitA,IFNULL(h.ownerid,0) ownerid,h.koszt,IFNULL(p.nick,'nn') nick,IFNULL(h.paidTo,'-'),IFNULL(DATEDIFF(h.paidTo,NOW()),-1), homeX,homeY,homeZ,homeA,homeI,homeVW,vehicles_allowed,vehicle_radius,hv.model,hv.X,hv.Y,hv.Z,hv.A,hv.color1,hv.color2,hv.plate
// 	 FROM fs_houses h LEFT JOIN fs_players p ON p.id=h.ownerid LEFT JOIN fs_houses_vehicles hv ON hv.houseid=h.id AND h.ownerid>0 AND h.paidTo>DATE(NOW()) LIMIT %d",MAX_HOUSES);
//	format(dane,sizeof dane,"SELECT h.id,h.X,h.Y,h.Z,h.exitX,h.exitY,h.exitZ,h.exitA,IFNULL(h.ownerid,0) ownerid,h.koszt,IFNULL(p.nick,'nn') nick,IFNULL(h.paidTo,'-'),IFNULL(DATEDIFF(h.paidTo,NOW()),-1), homeX,homeY,homeZ,homeA,homeI,homeVW,vehicles_allowed,vehicle_radius,IFNULL(hv.model,0),IFNULL(hv.X,0),IFNULL(hv.Y,0),IFNULL(hv.Z,0),IFNULL(hv.A,0),IFNULL(hv.color1,0),IFNULL(hv.color2,0),IFNULL(hv.plate,"-") 
//	FROM fs_houses h LEFT JOIN fs_players p ON p.id=h.ownerid LEFT JOIN fs_houses_vehicles hv ON hv.houseid=h.id AND h.ownerid>0 AND h.paidTo>DATE(NOW()) LIMIT %d",MAX_HOUSES);
	format(dane,sizeof dane,"SELECT * from fs_view_housesandvehicles LIMIT %d",MAX_HOUSES);
//SELECT h.id,h.X,h.Y,h.Z,h.exitX,h.exitY,h.exitZ,h.exitA,IFNULL(h.ownerid,0) ownerid,h.koszt,IFNULL(p.nick,'nn') nick,IFNULL(h.paidTo,'-'),IFNULL(DATEDIFF(h.paidTo,NOW()),-1),homeX,homeY,homeZ,homeA,homeI,homeVW,vehicles_allowed,
// vehicle_radius FROM fs_houses h LEFT JOIN fs_players p ON p.id=h.ownerid LIMIT %d",MAX_HOUSES);
//	printf(dane);

//	mysql_query(dane,SQL_RI_DOMY_LISTA);
	mysql_query(dane);
	domy_OnMysqlQuery(SQL_RI_DOMY_LISTA,0);

	foreach(playerid)
	    if (pData[playerid][loggedIn] && pTemp[playerid][e_houseid]>=0 && FSDomy[pTemp[playerid][e_houseid]][ed_owner]==pData[playerid][accountID])
			domy_OnHouseOwnerLogin(playerid,false);
}

domy_OnMysqlQuery(resultid,spareid){
	#pragma unused spareid
	if(resultid!=SQL_RI_DOMY_LISTA) return 0;
	new dane[500],Float:DX,Float:DY,Float:DZ,opis[127];
	mysql_store_result();
	new i=0;
	if (mysql_num_rows()>0)
	while(mysql_fetch_row(dane,"|")) {
		new vcomp[80];
		sscanf(dane,"p<|>dfffffffdds[32]s[11]dffffiiifD(0)F(0)F(0)F(0)F(0)D(0)D(0)S(-)[32]S(-)[128]S(-)[80]D(0)",FSDomy[i][ed_id],DX,DY,DZ, 
							FSDomy[i][ed_exit][0], FSDomy[i][ed_exit][1], FSDomy[i][ed_exit][2], FSDomy[i][ed_exit][3],
							FSDomy[i][ed_owner],FSDomy[i][ed_koszt],FSDomy[i][ed_ownernick], FSDomy[i][ed_paidTo], FSDomy[i][ed_daysLeft],
							FSDomy[i][ed_home][0], FSDomy[i][ed_home][1], FSDomy[i][ed_home][2], FSDomy[i][ed_home][3],
							FSDomy[i][ed_homei], FSDomy[i][ed_homevw], FSDomy[i][ed_vehiclesallowed], FSDomy[i][ed_vehicleradius],
							FSDomy[i][ehv_model], FSDomy[i][ehv_spawnPos][0],  FSDomy[i][ehv_spawnPos][1], FSDomy[i][ehv_spawnPos][2], FSDomy[i][ehv_spawnPos][3],
							FSDomy[i][ehv_color][0], FSDomy[i][ehv_color][1], FSDomy[i][ehv_plate],
							FSDomy[i][ed_audioURL], vcomp, FSDomy[i][ed_restrict_gang]
							);
		
		if (FSDomy[i][ed_homevw]<0) FSDomy[i][ed_homevw]=VW_UNIQUE_HOUSE+FSDomy[i][ed_id];
		sscanf(vcomp,"p<:>dddddddddddddd", FSDomy[i][ehv_components][0], FSDomy[i][ehv_components][1], FSDomy[i][ehv_components][2], FSDomy[i][ehv_components][3],
			FSDomy[i][ehv_components][4], FSDomy[i][ehv_components][5], FSDomy[i][ehv_components][6], FSDomy[i][ehv_components][7],
			FSDomy[i][ehv_components][8], FSDomy[i][ehv_components][9], FSDomy[i][ehv_components][10], FSDomy[i][ehv_components][11],
			FSDomy[i][ehv_components][12], FSDomy[i][ehv_components][13]);

		if (FSDomy[i][ehv_model]>0) 
			FSDomy[i][ehv_active]=true;
		else
			FSDomy[i][ehv_active]=false;

		if (FSDomy[i][ehv_active] && FSDomy[i][ehv_id]!=INVALID_VEHICLE_ID && GetVehicleModel(FSDomy[i][ehv_id])!=FSDomy[i][ehv_model])	{ // todo sprawdzanie modelu?
			tVehicles[FSDomy[i][ehv_id]][vo_private]=false;
			tVehicles[FSDomy[i][ehv_id]][vo_owningPlayerId]=INVALID_PLAYER_ID;
			FSDomy[i][ehv_id]=INVALID_VEHICLE_ID;
//			DestroyVehicle(FSDomy[i][ehv_id]);	// quick hack, ugly TODO
		}

		
		if (IsValidDynamicPickup(FSDomy[i][ed_pickup]))	DestroyDynamicPickup(FSDomy[i][ed_pickup]);
		if (IsValidDynamicCP(FSDomy[i][ed_cp])) DestroyDynamicCP(FSDomy[i][ed_cp]);
		if (IsValidDynamic3DTextLabel(FSDomy[i][ed_3dtext]))	DestroyDynamic3DTextLabel(FSDomy[i][ed_3dtext]);

		if (IsValidDynamicMapicon(FSDomy[i][ed_mapicon])) DestroyDynamicMapicon(FSDomy[i][ed_mapicon]);

		if (FSDomy[i][ed_daysLeft]<0) FSDomy[i][ed_owner]=0;
		
		if (FSDomy[i][ed_owner]>0 && FSDomy[i][ed_daysLeft]>=0) {
			format(opis,sizeof opis,"%s", FSDomy[i][ed_ownernick], FSDomy[i][ed_id]);
		} else {

			if (FSDomy[i][ed_restrict_gang]==NO_GANG)
				format(opis,sizeof opis,"NA SPRZEDAZ{AAFFAA}");
			else
				format(opis,sizeof opis,"NA SPRZEDAZ\ntylko {%06x}%s{AAFFAA}", gData[FSDomy[i][ed_restrict_gang]][gColor], gData[FSDomy[i][ed_restrict_gang]][tag]);

			if (FSDomy[i][ed_koszt]==0)
				format(opis,sizeof opis,"%s\nSMS",opis);
			else
				format(opis,sizeof opis,"%s\n%d na dobe", opis, FSDomy[i][ed_koszt]);
			format(opis,sizeof opis,"%s\n{F4FF29}www.fullserver.eu/domy/%d", opis, FSDomy[i][ed_id]);
		}

		if (FSDomy[i][ed_exit][0]==0 && FSDomy[i][ed_exit][1]==0) 
			format(opis, sizeof opis,"%s\nBEZ WYJSCIA", opis);

		FSDomy[i][ed_3dtext]=CreateDynamic3DTextLabel(opis,0xf0aaf0ff,DX,DY,DZ+0.5,25,INVALID_PLAYER_ID,INVALID_VEHICLE_ID,1,0,0);

		FSDomy[i][ed_pickup]=CreateDynamicPickup(
					((FSDomy[i][ed_owner]>0)?PICKUP_HOUSE_PURCHASED:PICKUP_HOUSE_NOTPURCHASED),1,	DX,DY,DZ,	0,0, -1, 50.0);
		FSDomy[i][ed_cp]=CreateDynamicCP(DX,DY,DZ,1.5,	0,0, -1, 5.0);

		FSDomy[i][ed_mapicon]=CreateDynamicMapIcon(DX,DY,DZ,32,1,0,0,-1,6000);

		FSDomy[i][ed_entrance][0]=DX;
		FSDomy[i][ed_entrance][1]=DY;
		FSDomy[i][ed_entrance][2]=DZ;

		i++;
		

	}
	mysql_free_result();
	
	IloscDomow=i;
	for (i=IloscDomow;i<MAX_HOUSES;i++) // jesli zmniejszyla sie ilosc domow, to tutaj usuwamy nadmiar
		if (FSDomy[i][ed_id]>0) {	
			if (IsValidDynamicPickup(FSDomy[i][ed_pickup]))	DestroyDynamicPickup(FSDomy[i][ed_pickup]);
			if (IsValidDynamic3DTextLabel(FSDomy[i][ed_3dtext]))	DestroyDynamic3DTextLabel(FSDomy[i][ed_3dtext]);
			FSDomy[i][ed_id]=0;
		}

	return IloscDomow++;
}

domy_SpawnVehicle(playerid,Float:pX=-1.0, Float:pY=-1.0, Float:pZ=-1.0, Float:pA=-1.0){
	new hid=pTemp[playerid][e_houseid];
	if (hid<0) return 0;
//	printf("spawnveh hid %d id %d active %d model %d", hid, FSDomy[hid][ehv_id], FSDomy[hid][ehv_active], FSDomy[hid][ehv_model]);
	if (!(FSDomy[hid][ehv_model]>=400))
		return 0;

	

	// todo usuniecie starego?
//	printf("Stworzony pojazd %d dla gracza %d model %d", cvid, playerid, FSDomy[hid][ehv_model]);

	if (FSDomy[hid][ehv_id]!=INVALID_VEHICLE_ID && tVehicles[FSDomy[hid][ehv_id]][vo_private] && tVehicles[FSDomy[hid][ehv_id]][vo_houseid]==hid && GetVehicleModel(FSDomy[hid][ehv_id])==FSDomy[hid][ehv_model]) {
	} else {
		if (FSDomy[hid][ehv_id]!=INVALID_VEHICLE_ID && FSDomy[hid][ehv_id]>0) {
//			printf("Usuwanie starego pojazdu %d", FSDomy[hid][ehv_id]);
			// niszczymy stary pojazd i informacje o tym ze jest prywatny
			tVehicles[FSDomy[hid][ehv_id]][vo_private]=false;
			tVehicles[FSDomy[hid][ehv_id]][vo_owningPlayerId]=INVALID_PLAYER_ID;
			tVehicles[FSDomy[hid][ehv_id]][vo_houseid]=-1;
			tVehicles[FSDomy[hid][ehv_id]][vo_destroyIfEmpty]=true;
			tVehicles[FSDomy[hid][ehv_id]][vo_licensePlateSet]=false;
			tVehicles[FSDomy[hid][ehv_id]][vo_color][0]=-1;
			tVehicles[FSDomy[hid][ehv_id]][vo_color][1]=-1;
			tVehicles[FSDomy[hid][ehv_id]][vo_static]=false;
		}
		new cvid=CreateVehicle(FSDomy[hid][ehv_model], pX==-1 ? FSDomy[hid][ehv_spawnPos][0] : pX,  pY==-1 ? FSDomy[hid][ehv_spawnPos][1] : pY, pZ==-1 ? FSDomy[hid][ehv_spawnPos][2] : pZ, pA==-1 ? FSDomy[hid][ehv_spawnPos][3] : pA, 
		FSDomy[hid][ehv_color][0], FSDomy[hid][ehv_color][1],4*3600);
		FSDomy[hid][ehv_id]=cvid;
		for(new i=0;i<=13;i++)
			if (FSDomy[hid][ehv_components][i]>0)
				AddVehicleComponent(cvid, FSDomy[hid][ehv_components][i]);
	}

	tVehicles[FSDomy[hid][ehv_id]][vo_destroyIfEmpty]=false;
	tVehicles[FSDomy[hid][ehv_id]][vo_private]=true;
	tVehicles[FSDomy[hid][ehv_id]][vo_owningPlayerId]=playerid;
	tVehicles[FSDomy[hid][ehv_id]][vo_houseid]=hid;
	tVehicles[FSDomy[hid][ehv_id]][vo_static]=false;
	tVehicles[FSDomy[hid][ehv_id]][vo_color][0]=FSDomy[hid][ehv_color][0];
	tVehicles[FSDomy[hid][ehv_id]][vo_color][1]=FSDomy[hid][ehv_color][1];


	if (strlen(FSDomy[hid][ehv_plate])>1) {
		SetVehicleNumberPlate(FSDomy[hid][ehv_id],FSDomy[hid][ehv_plate]);
		tVehicles[FSDomy[hid][ehv_id]][vo_licensePlateSet]=true;
	}

	new engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(FSDomy[hid][ehv_id], engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(FSDomy[hid][ehv_id], engine, lights, alarm, 0, bonnet, boot, objective);

	SetVehicleParamsForPlayer(FSDomy[hid][ehv_id], playerid, 0, DOOR_OPENED);

	vehicleDoorState[FSDomy[hid][ehv_id]] = DOOR_CLOSED;
    vehicleDoorOwner[FSDomy[hid][ehv_id]] = playerid;


	return 1;
	
}

domy_OnHouseOwnerLogin(playerid,bool:notify=true){
//	printf("house owner login %d %d %d", playerid, pData[playerid][accountID], pTemp[playerid][e_houseid]);
	if (pData[playerid][loggedIn] && pTemp[playerid][e_houseid]>=0 && FSDomy[pTemp[playerid][e_houseid]][ed_owner]==pData[playerid][accountID]) {
		new hid=pTemp[playerid][e_houseid];
		if (!IsValidDynamicMapIcon(FSDomy[hid][ed_mapicon])) {
			FSDomy[hid][ed_mapicon]=CreateDynamicMapIcon(FSDomy[hid][ed_entrance][0],FSDomy[hid][ed_entrance][1],FSDomy[hid][ed_entrance][2],31,1,0,0,playerid,6000);
			Streamer_SetIntData(STREAMER_TYPE_MAP_ICON, FSDomy[hid][ed_mapicon], E_STREAMER_STYLE, 1);
			if (FSDomy[pTemp[playerid][e_houseid]][ehv_active] && FSDomy[pTemp[playerid][e_houseid]][ehv_model]>0)
				domy_SpawnVehicle(playerid);

		}

		if (notify) {
			new buffer[128];
			SendClientMessage(playerid,-1," ");
			format(buffer, sizeof buffer, "Czynsz Twojego domu jest oplacony do {b}%s{/b} ({b}%d{/b} %s)", FSDomy[hid][ed_paidTo], FSDomy[hid][ed_daysLeft], dli(FSDomy[hid][ed_daysLeft],"dzien","dni","dni") );
			Msg(playerid,COLOR_INFO,buffer);
		}

//			Msg(playerid,COLOR_INFO,"Twoj pojazd czeka pod Twoim domem");
//		SendClientMessage(playerid,-1," ");
	}
	return 1;
}

domy_OnHouseOwnerDisconnects(playerid){
	if (pData[playerid][loggedIn] && pTemp[playerid][e_houseid]>=0 && FSDomy[pTemp[playerid][e_houseid]][ed_owner]==pData[playerid][accountID]) {
		//  FSDomy[pTemp[playerid][e_houseid]][ehv_active]
		new hid=pTemp[playerid][e_houseid];
		new cvid=FSDomy[hid][ehv_id];
		// DestroyVehicle(cvid); // niech chwile zostanie, a co
		tVehicles[cvid][vo_private]=true;
		tVehicles[cvid][vo_owningPlayerId]=INVALID_PLAYER_ID;
		tVehicles[cvid][vo_houseid]=hid;
		DestroyDynamicMapIcon(FSDomy[hid][ed_mapicon]);
	}
}

//domy_OPPickUpDynamicPickup(playerid,pickupid){
domy_OPEnterDynamicCP(playerid,checkpointid){
	if (GetPlayerVirtualWorld(playerid)!=0 || GetPlayerInterior(playerid)!=0 || GetPlayerState(playerid)!=PLAYER_STATE_ONFOOT) return 0;
//	new bool:pdelayActive;

//	if (GetTickCount()-pTemp[playerid][pickupDelay] < MIN_PICKUP_DELAY)
//		pdelayActive=true;
		
	for (new i=0;i<IloscDomow;i++) {
		if(FSDomy[i][ed_id]>0 && FSDomy[i][ed_cp]==checkpointid) {
			if (IsPlayerInAnyVehicle(playerid)) return 1;

			//if (pdelayActive) return 1;
//			pTemp[playerid][pickupDelay]=GetTickCount();
			domy_PokazMenuDomu(playerid,i);

/*			if (FSDomy[i][ed_owner]>0 && FSDomy[i][ed_owner]==pData[playerid][accountID]) {	// wlasciciel
				domy_PokazMenuDomu(playerid, i);
				//domy_Enter(playerid, i);
				return 1;
			} else if (FSDomy[i][ed_owner]>0) { // TODO && zamkniety
				if (FSDomy[i][ed_open]) {
					domy_Enter(playerid,i);

				} else {
					domy_Zapukaj(playerid,i);
					return Msg(playerid,COLOR_ERROR,"Zamkniete! Pukasz do drzwi...");	

				}
			} else if (FSDomy[i][ed_owner]==0) {
				domy_PokazMenuDomu(playerid, i);
				return 1;
			}*/
			return 1;

		}
	}
	return 0;
}


domy_tpto(playerid,domid=-1,Float:angl=0.0){
	if (domid<0) {
		if (pTemp[playerid][e_houseid]<0) return;
//		domid=domy_findReverseID(pTemp[playerid][e_houseid]);
		domid=pTemp[playerid][e_houseid];
	}
	if (FSDomy[domid][ed_exit][0]!=0 && FSDomy[domid][ed_exit][1]!=0)
		Teleport(T_PLAYER, playerid, FSDomy[domid][ed_exit][0], FSDomy[domid][ed_exit][1], FSDomy[domid][ed_exit][2], FSDomy[domid][ed_exit][3]-angl,  0, 0, true);
	else	// do pickupu
		Teleport(T_PLAYER, playerid, FSDomy[domid][ed_entrance][0], FSDomy[domid][ed_entrance][1], FSDomy[domid][ed_entrance][2], FLOAT_NAN,  0, 0, true);
	return;
}

CMD:dodajdom(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
	new opis[64],koszt;
	if (sscanf(params,"ds[64]",koszt,opis))
		return Msg(playerid,COLOR_ERROR,"Uzyj: /dodajdom {b}<koszt> <opis>{b}             Koszt: 0-SMS, 1-wzwyz - ilosc respektu na dobe");
	new buffer[255],esc_opis[64],Float:PX,Float:PY,Float:PZ;
	GetPlayerPos(playerid,PX,PY,PZ);
	mysql_real_escape_string(opis,esc_opis);
	format(buffer,sizeof buffer,"INSERT INTO fs_houses SET descr='%s',X=%.2f,Y=%.2f,Z=%.2f,ownerid=0,koszt=%d",
				esc_opis, PX,PY,PZ, koszt);
	mysql_query(buffer);
	format(buffer,sizeof buffer,"Dodano dom o ID {b}%d{/b}. Wpisz {b}/domyreload{/b} aby przeladowac domy, {b}/usundom id{/b} aby go usunac.", mysql_insert_id());
	return Msg(playerid,COLOR_INFO,buffer);
}

CMD:usundom(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
	new domid;
	if (sscanf(params,"d",domid))
		return Msg(playerid,COLOR_ERROR,"Uzyj: /usundom {b}id{/b}");
	new buffer[255];
	format(buffer,sizeof buffer,"DELETE FROM fs_houses WHERE id=%d AND (ownerid=0 OR ownerid IS NULL)", domid);
	mysql_query(buffer);
	if (mysql_affected_rows()==0)
		Msg(playerid,COLOR_ERROR,"Domu nie usunieto, bledne ID lub posiada wlasciciela!");
	else 
		Msg(playerid,COLOR_INFO,"Dom zostal usuniety. Wpisz /domyreload aby je przeladowac.");
	return 1;
}

CMD:domtp(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
	new domid;
	if (sscanf(params,"d",domid))
		return Msg(playerid,COLOR_ERROR,"Uzyj: /domtp {b}id{/b} - aby przeniesc sie do danego domu");

	domid=domy_findReverseID(domid);

	if (domid>=0 && FSDomy[domid][ed_id]>0)
		domy_tpto(playerid,domid,-180);
	else
		Msg(playerid,COLOR_ERROR,"Podano bledny {b}id{/b} domu");
	return 1;
}

CMD:domwyjscie(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
	if (IsPlayerInAnyVehicle(playerid)) return Msg(playerid,COLOR_ERROR,"Musisz wysiasc z pojazdu!");
	new domid;
	if (sscanf(params,"d",domid))
		return Msg(playerid,COLOR_ERROR,"Uzyj: /domwyjscie {b}id{/b} - zapisuje biezace polozenie i kierunek, jako domyslny po wyjsciu z domu");
	
	domid=domy_findReverseID(domid);

	if (domid>=0 && FSDomy[domid][ed_id]>0) {
		GetPlayerPos(playerid,FSDomy[domid][ed_exit][0], FSDomy[domid][ed_exit][1], FSDomy[domid][ed_exit][2]);
		GetPlayerFacingAngle(playerid,FSDomy[domid][ed_exit][3]);
		new buffer[255];
		format(buffer,sizeof buffer,"UPDATE fs_houses SET exitX=%f,exitY=%f,exitZ=%f,exitA=%f WHERE id=%d", 
					FSDomy[domid][ed_exit][0], FSDomy[domid][ed_exit][1], FSDomy[domid][ed_exit][2],
					FSDomy[domid][ed_exit][3],
					FSDomy[domid][ed_id]);
		mysql_query(buffer);
		Msg(playerid,COLOR_INFO2,"Zapisano informacje o domu. Wpisz /domyreload aby je przeladowac.");
	} else
		Msg(playerid,COLOR_ERROR,"Podano bledny {b}id{/b} domu");
	return 1;
}

CMD:domwnetrze(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
	if (IsPlayerInAnyVehicle(playerid)) return Msg(playerid,COLOR_ERROR,"Musisz wysiasc z pojazdu!");
	new domid;
	if (sscanf(params,"d",domid))
		return Msg(playerid,COLOR_ERROR,"Uzyj: /domwnetrze {b}id{/b} - zapisuje biezace polozenie i kierunek, jako domyslne po wyjsciu do domu");
	
	domid=domy_findReverseID(domid);

	if (domid>=0 && FSDomy[domid][ed_id]>0) {
		GetPlayerPos(playerid,FSDomy[domid][ed_home][0], FSDomy[domid][ed_home][1], FSDomy[domid][ed_home][2]);
		GetPlayerFacingAngle(playerid,FSDomy[domid][ed_home][3]);
		FSDomy[domid][ed_homei]=GetPlayerInterior(playerid);
		new buffer[255];
		format(buffer,sizeof buffer,"UPDATE fs_houses SET homeX=%f,homeY=%f,homeZ=%f,homeA=%f,homeI=%d WHERE id=%d", 
					FSDomy[domid][ed_home][0], FSDomy[domid][ed_home][1], FSDomy[domid][ed_home][2],
					FSDomy[domid][ed_home][3], FSDomy[domid][ed_homei],
					FSDomy[domid][ed_id]);
		mysql_query(buffer);
		Msg(playerid,COLOR_INFO2,"Zapisano informacje o domu. Wpisz /domyreload aby je przeladowac.");
	} else
		Msg(playerid,COLOR_ERROR,"Podano bledny {b}id{/b} domu");
	return 1;
}




CMD:domkoszt(playerid,params[]){
	if (!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
	new domid,koszt;
	if (sscanf(params,"dd",domid,koszt))
		return Msg(playerid,COLOR_ERROR,"Uzyj: /domkoszt {b}id{/b} {b}koszt{/b} - Gdzie koszt to ilosc respektu na dobe, lub 0 - platnosc SMS");
	
	domid=domy_findReverseID(domid);

	if (domid>=0 && FSDomy[domid][ed_id]>0) {
		new buffer[255];
		format(buffer,sizeof buffer,"UPDATE fs_houses SET koszt=%d WHERE id=%d", koszt,FSDomy[domid][ed_id]);
		mysql_query(buffer);
		Msg(playerid,COLOR_INFO2,"Zapisano informacje o domu. Wpisz /domyreload aby je przeladowac.");
	} else
		Msg(playerid,COLOR_ERROR,"Podano bledny {b}id{/b} domu");
	return 1;
}

CMD:dom(playerid,params[]){
	if (!pData[playerid][loggedIn])
		return Msg(playerid,COLOR_ERROR,"Nie posiadasz wlasnego domu, jesli chcesz go miec, to zacznij od rejestracji ({b}/rejestracja{/b} lub {b}/rej{/b})");

	if(!TeleportAllowed(playerid)) return Msg(playerid, COLOR_ERROR, TXT(playerid, 219));

	new hid=domy_findHouseByOwnerID(pData[playerid][accountID]);

	if (hid==-1)
		return Msg(playerid,COLOR_ERROR,"Nie odnaleziono Twojego domu, wyglada na to, ze go nie posiadasz.");

	if (FSDomy[hid][ed_exit][0]!=0 && FSDomy[hid][ed_exit][1]!=0)
			Teleport(T_PLAYER, playerid, FSDomy[hid][ed_exit][0], FSDomy[hid][ed_exit][1], FSDomy[hid][ed_exit][2], FSDomy[hid][ed_exit][3],  0, 0);
	else	{ // do pickupu
			Msg(playerid,COLOR_INFO2,"UWAGA! Twoj dom nie ma ustalonego punktu wyjscia. Popros admina 3-ciego poziomu, aby to ustawil.");
			Teleport(T_PLAYER, playerid, FSDomy[hid][ed_entrance][0]+random(3)-1.5, FSDomy[hid][ed_entrance][1]+random(3)-1.5, FSDomy[hid][ed_entrance][2], FLOAT_NAN,  0, 0);
	}
	return 1;
}

domy_findReverseID(dbid){
	if (dbid==0) return -1;
	for(new i=0;i<sizeof FSDomy;i++)
		if (dbid==FSDomy[i][ed_id]) return i;
	return -1;
}

/*
 * szukamy domu danego gracza	// TODO zrobic w SQL podczas laczenia sie klienta
 */
domy_findHouseByOwnerID(oid){
	for(new i=0;i<sizeof FSDomy;i++)
		if(FSDomy[i][ed_owner]==oid) return i;
	return -1;
}

domy_PokazMenuDomu(playerid, houseid=-1){
	// W wejdz D otworz/zamknij Z zobacz K kup
//	printf("menu domu %d h %d", playerid, houseid);
	if (houseid>=0)
		pTemp[playerid][tmpLastHouse]=houseid;
	new buf[1024];
	if (FSDomy[houseid][ed_home][0]==0 && FSDomy[houseid][ed_home][1]==0)
		return Msg(playerid, COLOR_INFO,"Zamkniete - remont");
	if (
			( FSDomy[houseid][ed_owner]==0 || FSDomy[houseid][ed_daysLeft]<0  ) && 
			FSDomy[houseid][ed_restrict_gang]!=NO_GANG && FSDomy[houseid][ed_restrict_gang]!=pData[playerid][gang]
		)
		return Msg(playerid, COLOR_INFO, "Tylko dla czlonkow klanu!");
	if (pData[playerid][accountID]>0 && FSDomy[houseid][ed_owner]==pData[playerid][accountID]) {
		strcat(buf, "{000000}W {ffffff}Wejdz");

		if (FSDomy[houseid][ed_open])
			strcat(buf, "\n{000000}D {ff3030}Zamknij dom");
		else
			strcat(buf, "\n{000000}D {1CC718}Otworz dom");
		if (FSDomy[houseid][ed_vehiclesallowed]>0)
			strcat(buf, "\n{000000}P {09A7FF}Zapisz pojazd");

		format(buf, sizeof buf, "%s\n{000000}K {ffffff}Oplacony do: %s", buf, FSDomy[houseid][ed_paidTo]);


	} else if (FSDomy[houseid][ed_owner]>0 && FSDomy[houseid][ed_daysLeft]>=0) {	// zajety
		strcat(buf, "{000000}W {ffffff}Wejdz");
		strcat(buf, "\n{000000}S {3030ff}Zapukaj");
	} else if ((!pData[playerid][loggedIn] || FSDomy[houseid][ed_owner]!=pData[playerid][accountID]) && (	FSDomy[houseid][ed_owner]==0 || FSDomy[houseid][ed_daysLeft]<0 	))
		strcat(buf, "{000000}Z {ffffff}Zobacz dom");
	if (	(	FSDomy[houseid][ed_owner]==0 || FSDomy[houseid][ed_daysLeft]<0 	)
		&&	(domy_findHouseByOwnerID(pData[playerid][accountID])==-1))	// gracz nie ma domu
		strcat(buf, "\n{000000}K {ffffff}Kup dom");

	if (pData[playerid][accountID]>0 && FSDomy[houseid][ed_owner]==pData[playerid][accountID])
		strcat(buf,"\n{000000}E {A4171A}Sprzedaj dom");

	format(buf,sizeof buf,	"%s\n {ECFF19}http://www.fullserver.eu/domy/%d", buf,FSDomy[houseid][ed_id]);
	return ShowPlayerDialog(playerid, DIALOG_DOMY_OUTSIDEPICKUP, DIALOG_STYLE_LIST, "DOM", buf, "OK", "Wyjdz");
}

domy_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]){
	#pragma unused listitem
	new buf[255];

	switch(dialogid){
	 case DIALOG_DOMY_INFO: {
		return ShowPlayerDialog(playerid, DIALOG_DOMY_OUTSIDEPICKUP, DIALOG_STYLE_LIST, "Dom", buf, "OK", "Wyjdz");
	 }
	 case DIALOG_DOMY_OUTSIDEPICKUP: {
		if (!response) return 1;
		switch(inputtext[0]){
			case 'W':
				return domy_Enter(playerid, pTemp[playerid][tmpLastHouse]);
			case 'Z':
				return domy_Enter(playerid, pTemp[playerid][tmpLastHouse]);
			case 'K':				
				return domy_OplacCzynsz(playerid,pTemp[playerid][tmpLastHouse]);
			case 'E': {
				new buf2[1024],koszt;
				koszt=FSDomy[pTemp[playerid][tmpLastHouse]][ed_daysLeft]*FSDomy[pTemp[playerid][tmpLastHouse]][ed_koszt]*30+314;
				format(buf2, sizeof buf2, "Mozesz opuscic ten dom. Za pozostaly czas wynajmu zostanie Ci zwrocony\nekwiwalent w postaci gotowki: {90ff90}%d${ffffff}.\n\nJesli jestes pewien, ze chcesz tego dokonac, wpisz w ponizszym polu\nwyraz: {ffffff}TAK",  koszt);
				return ShowPlayerDialog(playerid, DIALOG_DOMY_SPRZEDAZ, DIALOG_STYLE_INPUT, "Wyprowadzka", buf2, "Potwierdz", "Anuluj");
			}
			case 'S': {
				Msg(playerid,COLOR_INFO,"Pukasz do drzwi");
				return domy_Zapukaj(playerid, pTemp[playerid][tmpLastHouse]);
			}
			case 'D': {
				new houseid=pTemp[playerid][tmpLastHouse];
				if (FSDomy[houseid][ed_open]) {
						FSDomy[houseid][ed_open]=false;
						Msg(playerid,COLOR_INFO,"Twoj dom zostal {b}zamkniety{/b}.");
				} else { 
						FSDomy[houseid][ed_open]=true;
						Msg(playerid,COLOR_INFO,"Twoj dom zostal {b}otwarty{/b}.");
				}
				return domy_PokazMenuDomu(playerid);
			}
			case 'P': {
				return domy_ZapiszPojazd(playerid, pTemp[playerid][tmpLastHouse]);
			}
			default:
				return 1;
		}
	 }
	 case DIALOG_DOMY_CZYNSZ: {

		if (!pData[playerid][loggedIn])
			return Msg(playerid, COLOR_ERROR, "Tylko zarejestrowani gracze moga kupowac domy. Wpisz {b}/rejestracja{/b} aby sie zarejestrowac!");

		if (FSDomy[pTemp[playerid][tmpLastHouse]][ed_koszt]<=0)
			return Msg(playerid, COLOR_ERROR, "Mozliwosc kupowania domow za sms jest obecnie wylaczona.");
	
		if (domy_findHouseByOwnerID(pData[playerid][accountID])>=0 && domy_findHouseByOwnerID(pData[playerid][accountID])!=pTemp[playerid][tmpLastHouse])
			return Msg(playerid,COLOR_ERROR,"Masz juz dom!");

		if (!response) 
			return domy_PokazMenuDomu(playerid);
		new dni;
		if (sscanf(inputtext,"d",dni))
			return domy_OplacCzynsz(playerid,-1,"Podano nieprawidlowa wartosc!");

		new lacznie=dni*FSDomy[pTemp[playerid][tmpLastHouse]][ed_koszt];
		pTemp[playerid][tmpDialogValue]=dni;

		if (dni<1 || lacznie<1)
			return domy_OplacCzynsz(playerid,-1,"Podano nieprawidlowa wartosc!");

		if (pData[playerid][respect]<lacznie) {
			new buf2[500];
			format(buf2,sizeof buf2,"Nie masz wystarczajacej ilosci respektu! Sprobuj za jakis czas, badz tez oplac dom SMSem na stronie {F4FF29}www.fullserver.eu/domy/%d", FSDomy[pTemp[playerid][tmpLastHouse]][ed_id]);
			return domy_OplacCzynsz(playerid,-1,buf);
		}
		
		format(buf, sizeof buf, "Ilosc respektu: %d\nIlosc dni: %d", lacznie, dni);
		ShowPlayerDialog(playerid, DIALOG_DOMY_CZYNSZ_CONFIRM, DIALOG_STYLE_MSGBOX, "Potwierdz wplate", buf, "OK", "Anuluj");
		
	 }
	 case DIALOG_DOMY_CZYNSZ_CONFIRM: {
		if (!response)
			return domy_PokazMenuDomu(playerid);
		if (!pData[playerid][loggedIn])
			return Msg(playerid, COLOR_ERROR, "Tylko zarejestrowani gracze moga kupowac domy. Wpisz {b}/rejestracja{/b} aby sie zarejestrowac!");

		if (pTemp[playerid][tmpDialogValue]<1)
			return domy_OplacCzynsz(playerid,-1,"Podano nieprawidlowa wartosc!");
		new lacznie=pTemp[playerid][tmpDialogValue]*FSDomy[pTemp[playerid][tmpLastHouse]][ed_koszt];
		if (pData[playerid][respect]<pTemp[playerid][tmpDialogValue])
			return domy_OplacCzynsz(playerid,-1,"Nie masz wystarczajacej ilosci respektu! Sprobuj ponownie, lub skorzystaj z mozliwosci oplacenia domu przez SMS.");
		new domid=FSDomy[pTemp[playerid][tmpLastHouse]][ed_id];
		if (domid<0)
			return Msg(playerid, COLOR_ERROR, "Wystapil blad. Nie odnaleziono domu. Operacjna anulowana.");

		if (FSDomy[pTemp[playerid][tmpLastHouse]][ed_daysLeft]<0 || FSDomy[pTemp[playerid][tmpLastHouse]][ed_owner]==0) 
			format(buf,sizeof buf,"UPDATE fs_houses set paidTo=NOW() + INTERVAL %d DAY, ownerid=%d WHERE id=%d AND (paidTo<=NOW() OR paidTO IS NULL OR ownerid IS NULL OR ownerid=0)", pTemp[playerid][tmpDialogValue], pData[playerid][accountID],domid);
		else 
			format(buf,sizeof buf,"UPDATE fs_houses set paidTo=paidTo + INTERVAL %d DAY WHERE ownerid=%d AND id=%d", pTemp[playerid][tmpDialogValue], pData[playerid][accountID],domid);
		if (mysql_query(buf) || mysql_affected_rows()<1)
			return Msg(playerid,COLOR_ERROR,"Nastapil blad podczas dokonywania operacji. Zostala ona anulowana.");
		Msg(playerid,COLOR_INFO,"Gratulacje! Oplaciles swoj dom!");
		pData[playerid][respect]-=lacznie;
		pTemp[playerid][e_houseid]=pTemp[playerid][tmpLastHouse];//aqq
		SetPlayerScore(playerid,pData[playerid][respect]);
		SavePlayerData(playerid);
		domy_Reload();
		return 1;
	 }
	 case DIALOG_DOMY_SPRZEDAZ: {
		if (!response)
			return domy_PokazMenuDomu(playerid);

		if (strcmp(inputtext,"tak",true)!=0) {
			format(buf, sizeof buf, "Nie potwierdzil%s checi sprzedazy domu.", Kobieta(playerid)?("as"):("es"));
			Msg(playerid,COLOR_INFO2,buf);
			return domy_PokazMenuDomu(playerid);
		}

		new domid=pTemp[playerid][tmpLastHouse];

		if (FSDomy[domid][ed_owner]==0 || FSDomy[domid][ed_owner]!=pData[playerid][accountID]) {
			Msg(playerid, COLOR_ERROR, "Wystapil blad! Ten dom nei nalezy do Ciebie!");
			return 1;
		}
		new domidDB=FSDomy[domid][ed_id];			
		new kwota=FSDomy[domid][ed_daysLeft]*FSDomy[domid][ed_koszt]*30+314;
		GivePlayerMoney(playerid, kwota);
		format(buf, sizeof buf, "Dostajesz {b}%d${/b} zwrotu za pozostaly czynsz.", kwota);
		Msg(playerid, COLOR_INFO, buf);
		format(buf,sizeof buf,"UPDATE fs_houses set paidTo='0000-00-00', ownerid=0 WHERE id=%d", domidDB);
		mysql_query(buf);
		domy_Reload();
		domy_tpto(playerid,domid);
	 }
	}
	return 1;
}

domy_OplacCzynsz(playerid,hid=-1,komunikat[]=""){
	new buf[1024];
	if (hid==-1) hid=pTemp[playerid][tmpLastHouse];

	if (!pData[playerid][loggedIn])
		return Msg(playerid,COLOR_ERROR,"Tylko zarejestrowani gracze moga kupowac domy. Wpisz {b}/rejestracja{/b} aby sie zarejestrowac!");
	if (pData[playerid][respect]<FSDomy[hid][ed_koszt]) {
		
		Msg(playerid,COLOR_ERROR,"Niestety! Nie masz wystarczajaco duzo respektu, by oplacic chociaz dzien wynajmu!");
		format(buf, sizeof buf, "Uzbieraj wiecej respektu, badz oplac dom za pomoca smsa pod adresem {b}http://fullserver.eu/domy/%d{/b}", FSDomy[hid][ed_id]);
		return Msg(playerid, COLOR_ERROR, buf);

	}
	format(buf, sizeof buf, "Czynsz za dobe wynosi %d respektu.", FSDomy[hid][ed_koszt]);
	if (FSDomy[hid][ed_daysLeft]>=0 && FSDomy[hid][ed_owner]>0)
		format(buf,sizeof buf, "%s Obecnie dom jest oplacony do %s (dni: %d).", buf, FSDomy[hid][ed_paidTo], FSDomy[hid][ed_daysLeft]);
	strcat(buf, "\nPodaj, na ile dni chcesz oplacic dom:");
	if (strlen(komunikat)>2) { strcat(buf, "\n{ff0000}"); strcat(buf, komunikat); }
	return ShowPlayerDialog(playerid, DIALOG_DOMY_CZYNSZ, DIALOG_STYLE_INPUT, "Oplacanie domu", buf, "OK", "Anuluj");
}

domy_Enter(playerid,houseid){
	if (pData[playerid][adminLevel]>=LEVEL_ADMIN3 )

	pTemp[playerid][tmpLastHouse]=houseid;

	if (!FSDomy[houseid][ed_open] && FSDomy[houseid][ed_owner]>0 && FSDomy[houseid][ed_owner]!=pData[playerid][accountID])	// dom zamkniety i ma wlasciciela, nie jest nim gracz
		return ShowPlayerDialog(playerid, DIALOG_DOMY_INFO, DIALOG_STYLE_MSGBOX, "Dom", "Zamkniete!", "OK", "");
	
	domy_SaveReturnPath(playerid,houseid);
//	Teleport(T_PLAYER, playerid, -1136.40,1050.63, 250,  10, FSDomy[houseid][ed_homevw]);
	Teleport(T_PLAYER, playerid, FSDomy[houseid][ed_home][0],FSDomy[houseid][ed_home][1],FSDomy[houseid][ed_home][2], FSDomy[houseid][ed_home][3],  FSDomy[houseid][ed_homei], FSDomy[houseid][ed_homevw], true);
	if (FSDomy[houseid][ed_owner]==playerid)
		GameTextForPlayer(playerid, "Witaj w domu", 1000, 1);
	else if (FSDomy[houseid][ed_owner]>0) {
		new buf[64];
		format(buf, sizeof buf, "Dom %s", FSDomy[houseid][ed_ownernick]);
		GameTextForPlayer(playerid, buf, 1000, 1);
	} else {
		// todo sprawdzanie czy ktos jest w domu
		GameTextForPlayer(playerid, "Dom na sprzedaz", 1000, 1);
	}

//	printf("DOM %d URl %s", houseid, FSDomy[houseid][ed_audioURL]);
	if (strlen(FSDomy[houseid][ed_audioURL])>4 && Audio_IsClientConnected(playerid)) {
//		Audio_Stop(playerid,DomyAudioBG[playerid]);
		Audio_StopBGSound(playerid);		
//		DomyAudioBG[playerid]=Audio_PlayStreamed(playerid, FSDomy[houseid][ed_audioURL], false, true, true);
		Audio_PlayBGStream(playerid, FSDomy[houseid][ed_audioURL], false, true, true);
	}
		
		
	return 1;
}

domy_SaveReturnPath(playerid,hid) {
	pTemp[playerid][tmpLastHouse]=hid;

	if (FSDomy[hid][ed_exit][0]==0 && FSDomy[hid][ed_exit][1]==0) {
		pTemp[playerid][pickupDelay]=GetTickCount();
		return SaveReturnPath(playerid, FSDomy[hid][ed_homei]);
	}

	retpos[playerid][tpDestInt]=FSDomy[hid][ed_homei];
	retpos[playerid][tpWorld]=GetPlayerVirtualWorld(playerid);
	retpos[playerid][tpInterior]=GetPlayerInterior(playerid);
	retpos[playerid][tpStreamed]=(Streamer_CountVisibleItems(playerid, STREAMER_TYPE_OBJECT)>0)?true:false;

	retpos[playerid][tpX]=FSDomy[hid][ed_exit][0];
	retpos[playerid][tpY]=FSDomy[hid][ed_exit][1];
	retpos[playerid][tpZ]=FSDomy[hid][ed_exit][2];
	retpos[playerid][tpA]=FSDomy[hid][ed_exit][3];
	
	return 1;
}


domy_Zapukaj(playerid,hid){
	#pragma unused playerid
	if (FSDomy[hid][ed_homevw]==0) return 1;
	foreach(p) 
		if (GetPlayerVirtualWorld(p)==FSDomy[hid][ed_homevw]) {
			Msg(p,COLOR_INFO2,"Ktos puka do drzwi");
			if (Audio_IsClientConnected(p))
				Audio_Play(playerid,AUDIOID_FXDOMPUKANIE,false,false,true);
		}
	return 1;
}

domy_ZapiszPojazd(playerid, houseid){
//	printf("lin %d aid %d ownerid %d lveh %d hid %d found %d", 
//		pData[playerid][loggedIn], pData[playerid][accountID], FSDomy[houseid][ed_owner], pData[playerid][lastVehicle],  houseid, domy_findHouseByOwnerID(pData[playerid][accountID]))
	if (!pData[playerid][loggedIn] || pData[playerid][accountID]!=FSDomy[houseid][ed_owner] || 
		pData[playerid][lastVehicle]==INVALID_VEHICLE_ID || houseid!=domy_findHouseByOwnerID(pData[playerid][accountID])
		) 
		return Msg(playerid,COLOR_ERROR,"Aby zapisac pojazd przy domu, zaparkuj go w poblizu wejscia i uaktywnij ta opcje");

	if (FSDomy[houseid][ed_vehiclesallowed]<0)
		return Msg(playerid,COLOR_ERROR,"Przy Twoim domie nie da sie zapisac pojazdu");

	new Float:vP[4];
	new vid=pData[playerid][lastVehicle];
//	GetVehicleVelocity(vid, vP[0], vP[1], vP[2]);
//	new Float:vdist=floatabs(vP[0])+floatabs(vP[1]),floatabs(vP[2]);
//	if (vdist>0.1)
//		return Msg(playerid,COLOR_ERROR,"Wyglada na to, ze nie zaparkowales swojego pojazdu prawidlowo.");
	
	GetVehiclePos(vid,vP[0], vP[1], vP[2]);
	GetVehicleZAngle(vid,vP[3]);
	new Float:vdist=GetDistanceBetweenPoints3D(vP[0], vP[1], vP[2], FSDomy[houseid][ed_exit][0], FSDomy[houseid][ed_exit][1], FSDomy[houseid][ed_exit][2]);
	if (vdist>FSDomy[houseid][ed_vehicleradius] && vdist<(FSDomy[houseid][ed_vehicleradius]*4))
		return Msg(playerid,COLOR_ERROR,"Pojazd stoi za daleko.");
	else if (vdist>FSDomy[houseid][ed_vehicleradius])
		return Msg(playerid,COLOR_ERROR,"Zaparkuj pojazd pod domem.");
	new buf[450];
	format(buf, sizeof buf, "DELETE FROM fs_houses_vehicles WHERE houseid=%d", FSDomy[houseid][ed_id]);
	mysql_query(buf);

	new vmodel=GetVehicleModel(vid);
	new vcomp[80];
	for(new i=0;i<=13;i++) {
		FSDomy[houseid][ehv_components][i]=0;
		FSDomy[houseid][ehv_components][i]=GetVehicleComponentInSlot(vid,i);
		format(vcomp,sizeof vcomp,"%s%s%d", vcomp, (strlen(vcomp)>0 ? (":") : ("")), FSDomy[houseid][ehv_components][i]);
	}


	format(buf, sizeof buf, "INSERT INTO fs_houses_vehicles SET houseid=%d,model=%d,X=%.2f,Y=%.2f,Z=%.2f,A=%.2f,color1=%d,color2=%d,components='%s',plate='%s'",
				FSDomy[houseid][ed_id], vmodel, vP[0], vP[1], vP[2], vP[3], tVehicles[vid][vo_color][0], tVehicles[vid][vo_color][1], vcomp, GetPlayerProperName(playerid));
	mysql_query(buf);
	Msg(playerid,COLOR_INFO,"Pojazd zostal zapisany. Uzyj {b}/mp{/b} aby go przywolac!");

	FSDomy[houseid][ehv_active]=true;
	FSDomy[houseid][ehv_model]=vmodel;
	FSDomy[houseid][ehv_spawnPos][0]=vP[0];
	FSDomy[houseid][ehv_spawnPos][1]=vP[1];
	FSDomy[houseid][ehv_spawnPos][2]=vP[2];
	FSDomy[houseid][ehv_spawnPos][3]=vP[3];
	FSDomy[houseid][ehv_color][0]=tVehicles[vid][vo_color][0];
	FSDomy[houseid][ehv_color][1]=tVehicles[vid][vo_color][1];

    if (!tVehicles[vid][vo_static] && !tVehicles[vid][vo_private])
        DestroyVehicle(vid);
    else
        SetVehicleToRespawn(vid);
	
	domy_SpawnVehicle(playerid);

    //  FSDomy[i][ehv_plate]


	return 1;
}

CMD:mp(playerid){
	MUST_BE_SPAWNED(playerid);
	if (!TeleportAllowed(playerid) || pData[playerid][pAttraction]!=A_NONE)
		return Msg(playerid,COLOR_ERROR,"Nie mozesz teraz tego zrobic.");
	
	if (!pData[playerid][loggedIn])	
		return Msg(playerid,COLOR_ERROR,"Zapisywanie i przywolywanie pojazdu jest mozliwe tylko dla zarejestrowanych graczy.");
	new hid=domy_findHouseByOwnerID(pData[playerid][accountID]);
	if (hid<0)
		return Msg(playerid,COLOR_ERROR,"Nie posiadasz domu!");
	if (!FSDomy[hid][ehv_active])
		return Msg(playerid,COLOR_ERROR,"Nie posiadasz zapisanego pojazdu!");

	if (FSDomy[hid][ed_vehiclesallowed]<1)
		return Msg(playerid,COLOR_ERROR,"Przy Twoim domie nie da sie zapisac pojazdu.");

	if (IsPlayerInAnyVehicle(playerid))
		return Msg(playerid,COLOR_ERROR,"Najpierw wysiadz z pojazdu");

	domy_SpawnVehicle(playerid);

/*	if (FSDomy[hid][ehv_id]==INVALID_VEHICLE_ID || tVehicles[FSDomy[hid][ehv_id]][vo_owningPlayerId]!=playerid) {
	//	DestroyVehicle(FSDomy[hid][ehv_id]);
		Msg(playerid,COLOR_INFO,"Nie mozna odnalezc Twojego pojazdu... sprobuj za chwile.");
		domy_SpawnVehicle(playerid);
		return 1;
	}*/

	new Float:pP[4];
	GetVehiclePos(FSDomy[hid][ehv_id],pP[0], pP[1], pP[2]);
	SetPlayerInterior(playerid,0);
	SetPlayerPos(playerid, pP[0], pP[1], pP[2]+1.5);

	PutPlayerInVehicle(playerid,FSDomy[hid][ehv_id], 0);
	
	return 1;
}

CMD:domyreload(playerid){
	if(!IsAdmin(playerid,LEVEL_ADMIN3) && NIEMAPERZY) return 0;
	Msg(playerid,COLOR_INFO2,"Przeladowywanie domow");
	domy_Reload();
	return 1;
}