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
//	"Pojazdy specjalne",
	"Helikoptery",
	"Samoloty",
	"Lodzie",
	"Rowery"
};


new pojazdy[][]={
	{522,581,463,468,462},
	{536,534,535,412},
	{533,439,555},
	{404,479,458,561},
	{568,424,400,500,489,495}, //5
	{445,562,585,419,492,474,546,517,426,536,547,405,580,560},
	{602,429,496,402,541,415},
	{499,422,482,498,609,524,578,455,403},//,414,582,443,514,600,413,515,440,543,459,531,408,552,478,456,554},
//	{485,457,483,532,486,406,530,434,545,588,571,572,423,442,428,409,574,525,583},
	{487,563,469},
	{511,512,593,513},
	{473,493,484,454},
	{481,510}
};





new txt_rodzaje[1024];
new txt_pojazdy[][]={
	"NRG-500\r\nBF-400\r\nFreeway\r\nSanchez\r\nFaggio",
	"Blade\r\nRemington\r\nSlamvan\r\nVoodoo",
	"Feltzer\r\nStallion\r\nWindsor",
	"Perennial\r\nRegina\r\nSolair\r\nStratum",
	"Bandito\r\nBF Injection\r\nMesa\r\nRancher\r\nSandking", //5 
	"Admiral\r\nElegy\r\nEmperor\r\nEsperanto\r\nGreenwood\r\nHermes\r\nIntruder\r\nMajestic\r\nPremier\r\nPrevion\r\nPrimo\r\nSentinel\r\nStafford\r\nSultan", //6
	"Alpha\r\nBanshee\r\nBlista compact\r\nBuffalo\r\nBullet\r\nCheetah", //7
	"Benson\r\nBobcat\r\nBurrito\r\nBoxville\r\nBoxburg\r\nCement truck\r\nDFT-30\r\nFlatbed\r\nLinerunner",//\r\nMule\r\nNewsvan\r\nPacker\r\nPetrol tanker\r\nPicador\r\nPony\r\nRoadtrain\r\nRumpo\r\nSadler\r\nTopfun\r\nTractor\r\nTrashmaster\r\nUtility Van\r\nWalton\r\nYankee\r\nYosemite",
//	"Baggage\r\nCaddy\r\nCamper\r\nCombine Harvester\r\nDozer\r\nDumper\r\nForklift\r\nHotknife\r\nHustler\r\nHotdog\r\nKart\r\nMower\r\nMr Whoopee\r\nRomero\r\nSecuricar\r\nStretch\r\nSweeper\r\nTowtruck\r\nTug", 
	"Maverick\r\nRaindance\r\nSparrow",
	"Beagle\r\nCropduster\r\nDodo\r\nStuntplane",
	"Dinghy\r\nJetmax\r\nMarquis\r\nTropic",
	"BMX\r\nRower gorski"
};


public OnFilterScriptInit(){
	txt_rodzaje="Wprowadz nazwe (tylko {FFFF00}VIP{ffffff})";
	for(new i = 0; i < sizeof rodzaje; i++) {
		format(txt_rodzaje,sizeof txt_rodzaje,"%s\r\n%s", txt_rodzaje, rodzaje[i]);
	}
	
	return 1;
}

CMD:cars(playerid,params[])
	return cmd_pojazdy(playerid,params);

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
public OnDialogResponse(playerid,dialogid,response,listitem,inputtext[]){
	switch(dialogid){
		case DIALOG_POJAZDY_LISTA: {
			if (!response) return 1;
			printf("%d %d %d %d %s",playerid,dialogid,response,listitem,inputtext);
			new vid,vname[33];
			sscanf(inputtext,"ds[32]",vid,vname);
			if (vid>=400) {
				new r=CallRemoteFunction("spawnVehicleForPlayer","ddd",playerid,vid,1);
				if (r==0 || r==INVALID_VEHICLE_ID)
					return SendClientMessage(playerid,-1,"Nie mozesz teraz tego zrobic!");
			}
			return 1;
		}
		case DIALOG_POJAZDY_TYP: {
			if (!response) return 1;
			switch(listitem){
				case 0:
					return CallRemoteFunction("cmd_p","d",playerid);
				default:
					return MenuPojazdow(playerid,listitem);	
			}
		}
	}
	if (dialogid>=DIALOG_POJAZDY_RODZAJE && dialogid<=DIALOG_POJAZDY_RODZAJE+sizeof(pojazdy)) {
		if (!response) return cmd_pojazdy(playerid,"");
		return ZespawnujPojazd(playerid,dialogid-DIALOG_POJAZDY_RODZAJE,listitem);
	}
	return 0;
}