#define AUDIOID_KICK	2
#define AUDIOID_BAN		3
#define AUDIOID_WARN_MALE	5
#define AUDIOID_WARN_FEMALE	6

#define AUDIOID_INTRO	20
#define AUDIOID_ACHIEVEMENT	30

#define AUDIOID_FXSUBMARINE	42
#define AUDIOID_FXKOSCIOL 43
#define AUDIOID_FXTUNEL1 44
#define AUDIOID_FXTUNEL2 45
#define AUDIOID_FXAFGANISTAN	46
#define AUDIOID_FXWIETNAM		47
#define AUDIOID_FXHACJENDA		48
#define AUDIOID_FXYAKUZA		49

#define AUDIOID_FXFOTORADAR		50
#define AUDIOID_FXDOMPUKANIE	51

#define AUDIOID_WALIZKA	60
#define AUDIOID_PREZENT	61
#define AUDIOID_PODKOWA 62

#define AUDIOID_BEEP1 70
#define AUDIOID_BEEP2 71
#define AUDIOID_BEEP3 72
#define AUDIOID_BEEP4 73

static audio_radiaAirtraffic[][]={
	"http://yp.shoutcast.com/sbin/tunein-station.pls?id=1115871",
	"http://mtl2.liveatc.net/epwa.m3u"
};

static audio_radiaPolice[][]={
	"http://yp.shoutcast.com/sbin/tunein-station.pls?id=594724"
};

static audio_radiaCB[][]={
	"http://freeshoutcast.pl:10410/"
//	"http://yp.shoutcast.com/sbin/tunein-station.pls?id=1934365"
};


new Bar:audio_pbar[MAX_PLAYERS];
new backgroundSounds[MAX_PLAYERS];


Audio_PlayBGSound(playerid,audioid,bool:pause=false,bool:loop=true,bool:downmix=true){
	if (!Audio_IsClientConnected(playerid)) return 0;
	if (pData[playerid][spectating] != INVALID_PLAYER_ID) return 0;

	if (backgroundSounds[playerid]) Audio_Stop(playerid,backgroundSounds[playerid]);
	backgroundSounds[playerid]=Audio_Play(playerid,audioid,pause,loop,downmix);
	return backgroundSounds[playerid];
}

Audio_PlayBGStream(playerid,streamurl[],bool:pause=false,bool:loop=true,bool:downmix=true){
	if (!Audio_IsClientConnected(playerid)) return 0;
	if (pData[playerid][spectating] != INVALID_PLAYER_ID) return 0;

	if (backgroundSounds[playerid]) Audio_Stop(playerid,backgroundSounds[playerid]);
	backgroundSounds[playerid]=Audio_PlayStreamed(playerid,streamurl,pause,loop,downmix);
	return backgroundSounds[playerid];
}


Audio_StopBGSound(playerid){
	if (!Audio_IsClientConnected(playerid)) return 0;
	if (backgroundSounds[playerid]) Audio_Stop(playerid,backgroundSounds[playerid]);
	return 1;
}

public Audio_OnClientConnect(playerid) {
    Audio_TransferPack(playerid);
	backgroundSounds[playerid]=0;
}

public Audio_OnSetPack(audiopack[]) {
    foreach(i)
        Audio_TransferPack(i);
    return 1;
}

forward Audio_RetransferPack(playerid);
public Audio_RetransferPack(playerid){
	Audio_TransferPack(playerid);
	return 1;
}

public Audio_OnClientDisconnect(playerid){
	backgroundSounds[playerid]=0;
	if (!IsPlayerConnected(playerid)) return 0;
	return Msg(playerid,COLOR_ERROR, "Polaczenie z pluginem audio {b}utracone{/b}.");
}

public Audio_OnTransferFile(playerid,file[],current,total,result) {
    if (current==1) {
		Msg(playerid, COLOR_INFO, "Rozpoczeto synchronizacje plikow audio", false, true);
		audio_pbar[playerid]=CreateProgressBar(38.00, 125.00, 156.50, 3.19, 16711935, total);
		SetProgressBarValue(audio_pbar[playerid],current);
		ShowProgressBarForPlayer(playerid,audio_pbar[playerid]);
	} else {
		SetProgressBarValue(audio_pbar[playerid],current);
		UpdateProgressBar(audio_pbar[playerid],playerid);
	}
    if (total==current) {
		Msg(playerid, COLOR_INFO2, "Synchronizacja plikow audio zakonczona", true, true);
//		DisablePlayerSounds(playerid);	// standradowe dzwieki gta, byc moze leci jeszcze melodyjka ze startu
//		Audio_Play(playerid,AUDIOID_INTRO, false, false, true);
		HideProgressBarForPlayer(playerid,audio_pbar[playerid]);
		DestroyProgressBar(audio_pbar[playerid]);
	}
    return 1;
}

//CMD:stopaudio(playerid)
//	return	Audio_TransferPack(playerid);

Audio_OnPlayerStateChange(playerid,newstate,oldstate){
	if (!Audio_IsClientConnected(playerid)) return;
	if (oldstate==2 && pTemp[playerid][audio_vehicle]>-1) {
		Audio_Stop(playerid, pTemp[playerid][audio_vehicle]);
		pTemp[playerid][audio_vehicle]=-1;
	} else if (newstate==2) {
		new vm=GetVehicleModel(GetPlayerVehicleID(playerid));
		switch(vm){
			case 520,519,553,577,592,593,460,476,511,512,513: {
				new r=random(sizeof audio_radiaAirtraffic);
				pTemp[playerid][audio_vehicle]=Audio_PlayStreamed(playerid, audio_radiaAirtraffic[r], false, true, true);
			}
			case 596,597,598,523,497: {
				new r=random(sizeof audio_radiaPolice);
				pTemp[playerid][audio_vehicle]=Audio_PlayStreamed(playerid, audio_radiaPolice[r], false, true, true);
			}
			case 403,514,515: {
				new r=random(sizeof audio_radiaCB);
				pTemp[playerid][audio_vehicle]=Audio_PlayStreamed(playerid, audio_radiaCB[r], false, true, true);
			}
		}
	
	}
	return;

}