#include <a_samp>
#include <zcmd>
#include <sscanf2>
#include <mysql>

#define DIALOG_POJAZDY_OFFSET 1110
#define DIALOG_POJAZDY_TYP 		(DIALOG_POJAZDY_OFFSET+0)
#define DIALOG_POJAZDY_RECZNIE 	(DIALOG_POJAZDY_OFFSET+1)
#define DIALOG_POJAZDY_LISTA 	(DIALOG_POJAZDY_OFFSET+2)
#define DIALOG_POJAZDY_RODZAJE	(DIALOG_POJAZDY_OFFSET+3)


new rodzaje[][]={
	"Motory",
	"Lowridery",
	"Kabriolety",
	"Auta kombi",
	"Pojazdy off-road",
	"Auta sedan",
	"Auta sportowe",
	"Pojazdy przemyslowe",
	"Pojazdy specjalne",
	"Helikoptery",
	"Samoloty",
	"Lodzie",
	"Rowery"
}


new pojazdy[][]={
	{521,522,581,461,463,586,468,462,448},
	{536,575,534,567,535,566,576,412},
	{480,533,439,555},
	{418,404,479,458,561},
	{568,424,573,579,400,500,444,489,495},
	{445,401,518,527,542,507,562,585,419,526,466,492,474,546,517,410,551,516,467,426,536,547,405,580,560,550,549,540,491,529,421},
	{602,429,496,402,541,415},
	{499,422,482,498,609,524,578,455,403,414,582,443,514,600,413,515,440,543,459,531,408,552,478,456,554},
	{485,457,483,532,486,406,530,434,545,588,571,572,423,442,428,409,574,525,583},
	{548,417,487,497,563,469},
	{511,512,593,553,519,460,513},
	{472,473,493,595,484,430,453,452,446,454},
	{481,509,510}
}





new txt_rodzaje[1024];
new txt_pojazdy[][]={
	"FCR-900\r\nNRG-500\r\nBF-400\r\nPCJ-600\r\nFreeway\r\nWayfarer\r\nSanchez\r\nFaggio\r\nPizzaboy",
	"Blade\r\nBroadway\r\nRemington\r\nSavanna\r\nSlamvan\r\nTahoma\r\nTornado\r\nVoodoo",
	"Comet\r\nFeltzer\r\nStallion\r\nWindsor",
	"Moonbeam\r\nPerennial\r\nRegina\r\nSolair\r\nStratum",
	"Bandito\r\nBF Injection\r\nDune\r\nHuntley\r\nMesa\r\nMonster\r\nRancher\r\nSandking",
	"Admiral\r\nBravura\r\nBuccaneer\r\nCadrona\r\nClover\r\nElegant\r\nElegy\r\nEmperor\r\nEsperanto\r\nFortune\r\nGlendale\r\nGreenwood\r\nHermes\r\nIntruder\r\nMajestic\r\nManana\r\nMerit\r\nNebula\r\nOceanic\r\nPremier\r\nPrevion\r\nPrimo\r\nSentinel\r\nStafford\r\nSultan\r\nSunrise\r\nTampa\r\nVincent\r\nVirgo\r\nWillard\r\nWashington",
	"Alpha\r\nBanshee\r\nBlista compact\r\nBuffalo\r\nBullet\r\nCheetah",
	"Benson\r\nBobcat\r\nBurrito\r\nBoxville\r\nBoxburg\r\nCement truck\r\nDFT-30\r\nFlatbed\r\nLinerunner\r\nMule\r\nNewsvan\r\nPacker\r\nPetrol tanker\r\nPicador\r\nPony\r\nRoadtrain\r\nRumpo\r\nSadler\r\nTopfun\r\nTractor\r\nTrashmaster\r\nUtility Van\r\nWalton\r\nYankee\r\nYosemite",
	"Baggage\r\nCaddy\r\nCamper\r\nCombine Harvester\r\nDozer\r\nDumper\r\nForklift\r\nHotknife\r\nHustler\r\nHotdog\r\nKart\r\nMower\r\nMr Whoopee\r\nRomero\r\nSecuricar\r\nStretch\r\nSweeper\r\nTowtruck\r\nTug",
	"Cargobob\r\nLeviathan\r\nMaverick\r\nPolice Maverick\r\nRaindance\r\nSparrow",
	"Beagle\r\nCropduster\r\nDodo\r\nNevada\r\nShamal\r\nSkimmer\r\nStuntplane",
	"Coastguard\r\nDinghy\r\nJetmax\r\nLaunch\r\nMarquis\r\nPredator\r\nReefer\r\nSpeeder\r\nSquallo\r\nTropic",
	"BMX\r\nSkladak\r\nRower gorski"
};


public OnFilterScriptInit(){
	txt_rodzaje="Wprowadz nazwe...";
	for(new i = 0; i < sizeof rodzaje; i++) {
		format(txt_rodzaje,sizeof txt_rodzaje,"%s\r\n%s", txt_rodzaje, rodzaje[i]);
	}
	
	return 1;
}

CMD:cars(playerid,params[]){
	return cmd_pojazdy(playerid,params);
}

CMD:pojazdy(playerid,params[]){
	ShowPlayerDialog(playerid,DIALOG_POJAZDY_TYP,DIALOG_STYLE_LIST,"Wybierz rodzaj pojazdu:",txt_rodzaje,"OK","Anuluj");
	return 1;
}

MenuPojazdow(playerid,listitem){
	ShowPlayerDialog(playerid,DIALOG_POJAZDY_RODZAJE+listitem-1, DIALOG_STYLE_LIST, "Wybierz rodzaj pojazdu:", txt_pojazdy[listitem-1], "OK", "Anuluj");
	return 1;
}

ZespawnujPojazd(playerid,rodzaj,listitem){
	new r=CallRemoteFunction("spawnVehicleForPlayer","ddd",playerid,pojazdy[rodzaj][listitem],1);
	if (r==0 || r==INVALID_VEHICLE_ID)
		return SendClientMessage(playerid,-1,"Nie mozesz teraz tego zrobic!");
	return 1;
}

CMD:p(playerid,params[]){
	new txt[16],buffer[255];
	if (sscanf(params,"s[16]",txt))
		return ShowPlayerDialog(playerid,DIALOG_POJAZDY_RECZNIE, DIALOG_STYLE_INPUT, "Wyszukiwanie pojazdu", "Wpisz nazwe pojazdu lub jej fragment:", "OK", "Anuluj");

	new vname[16];
	mysql_real_escape_string(txt,vname);
	if (strlen(vname)>3) 
		format(buffer,sizeof buffer,"SELECT vid,name FROM fs_vehicles WHERE minPoziom='gracz' AND (name LIKE '%%%s%%' OR name SOUNDS LIKE '%s' OR altnames LIKE '%%%s%%' OR altnames SOUNDS LIKE '%s') LIMIT 10", vname,vname,vname,vname);
	else
		format(buffer,sizeof buffer,"SELECT vid,name FROM fs_vehicles WHERE minPoziom='gracz' AND (name LIKE '%%%s%%' OR altnames LIKE '%%%s%%') LIMIT 10", vname, vname);

	mysql_query(buffer);

	new vid,vehname[33];
	mysql_store_result();
	if (mysql_num_rows()==0)
		SendClientMessage(playerid,0xffffff,"Nie znaleziono zadnego pojazdu  o pasujacej nazwie!");
	else if (mysql_num_rows()==1) {
		mysql_fetch_row(buffer," ");
        sscanf(buffer,"ds[32]",vid,vehname);
		if (vid>0) {
			new r=CallRemoteFunction("spawnVehicleForPlayer","ddd",playerid,vid,1);
			if (r==0 || r==INVALID_VEHICLE_ID)
				return SendClientMessage(playerid,-1,"Nie mozesz teraz tego zrobic!");
			format(buffer,sizeof buffer,"Utworzono pojazd %s", vehname);	
			SendClientMessage(playerid,0xffffff,buffer);
		}
	}
	else if (mysql_num_rows()>1) {
		new txt_lista[1023];
	    for (new i=0;i<mysql_num_rows();i++){
			mysql_fetch_row(buffer," ");
    	    sscanf(buffer,"ds[32]",vid,vehname);
			if (i==0)
				format(txt_lista,sizeof txt_lista,"{000000}%d{ffffff} %s",vid,vehname);
			else
				format(txt_lista,sizeof txt_lista,"%s\n{000000}%d{ffffff} %s",txt_lista,vid,vehname);


		}
		ShowPlayerDialog(playerid,DIALOG_POJAZDY_LISTA,DIALOG_STYLE_LIST,"Znalezione pojazdy:",txt_lista,"OK","Anuluj");

	}
	mysql_free_result();
	return 1;
}
	
public OnDialogResponse(playerid,dialogid,response,listitem,inputtext[]){
	switch(dialogid){
		case DIALOG_POJAZDY_LISTA: {
			if (!response) return 1;
			printf("%d %d %d %d %s",playerid,dialogid,response,listitem,inputtext);
			new vid,vname[33];
			sscanf(inputtext,"ds[32]",vid,vname);
			if (vid>=400) {
				new r=CallRemoteFunction("spawnVehicleForPlayer","ddd",playerid,vid,1)
				if (r==0 || r==INVALID_VEHICLE_ID)
					return SendClientMessage(playerid,-1,"Nie mozesz teraz tego zrobic!");
			}
			return 1;
		}
		case DIALOG_POJAZDY_TYP: {
			if (!response) return 1;
			switch(listitem){
				case 0:	
					return cmd_p(playerid,"");
				default:
					return MenuPojazdow(playerid,listitem);				
			}
		}
		case DIALOG_POJAZDY_RECZNIE: {
			if(!response) return 1;
			return cmd_p(playerid,inputtext);
		}
	}
	if (dialogid>=DIALOG_POJAZDY_RODZAJE && dialogid<=DIALOG_POJAZDY_RODZAJE+sizeof(pojazdy)) {
		if (!response) return cmd_pojazdy(playerid,"");
		return ZespawnujPojazd(playerid,dialogid-DIALOG_POJAZDY_RODZAJE,listitem);
	}
	return 0;
}

