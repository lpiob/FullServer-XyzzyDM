#pragma tabsize	0

#define VEHICLE_DEFAULT_HP	3000

#define GAMETIMESYNCTOREAL

#define NIEMAPERZY (pData[playerid][accountID]!=43 && pData[playerid][accountID]!=55)

#define USE_REGEX

#define FLOAT_NAN     (Float:0xFFFFFFFF)

#define SALT_ALOGIN 				"thah9eZi"

// KEY_PRESSED(keys)
#define KEY_PRESSED(%0) \
	(((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))

// RELEASED(keys)
#define KEY_RELEASED(%0) \
	(((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))

#define MAX_SERVER_PLAYERS 			314
#undef MAX_PLAYERS
#define MAX_PLAYERS 				(MAX_SERVER_PLAYERS)
#define MAX_LANGUAGES				1
#define MAX_LANG_ROWS				550
#define MAX_LANG_TEXT				128
#define MAX_LOGIN_ATTEMPS			5
#define MAX_TEXTDRAWS				120
#define MAX_PLAYER_TEXTDRAWS		9
#define MAX_GANGS					25
#define MAX_ATTRACTIONS				8
#define MAX_MONEY_PICKUPS			32
#define MAX_HUD_ELEMENTS			9
#define MAX_CENSORED_WORDS			500
#define MAX_REPORTS					30

#define MIN_QUEUE_CHOWANY			3
#define MIN_QUEUE_SPS				2
#define MIN_QUEUE_DERBY				2
#define MIN_QUEUE_LABIRYNT			2
#define MIN_QUEUE_RACE				1
#define MIN_QUEUE_DRIFT				1
#define MIN_QUEUE_WG				2
#define MIN_QUEUE_STRZELNICA		3

#define MAX_QUEUE_CHOWANY			80
#define MAX_QUEUE_SPS				75
#define MAX_QUEUE_DERBY				50
#define MAX_QUEUE_LABIRYNT			15
#define MAX_QUEUE_RACE				80
#define MAX_QUEUE_DRIFT				3
#define MAX_QUEUE_WG				200
#define MAX_QUEUE_STRZELNICA		20

#define DB_PLAYERS					"players"
#define DB_GANGS					"gangs"
#define DB_BANS						"bans"
#define DB_IPBANS					"ipbans"
#define DB_STATS					"stats"
#define DB_PLAYERSINGANGS			"players_in_gangs"
#define DB_CONFIG					"config"
//#define DB_HOUSES					"houses"

#define LOG_SYSTEM					0
#define LOG_COMMANDS				1
#define LOG_CHAT					2
#define LOG_PM						3
#define LOG_OCHAT					4
#define LOG_ANN						5
#define LOG_PLAYERS					6
//new File:fh_logs[6];
//new logsFlushed[6];

#define MAX_LOG_STRING_LENGTH 	200

#define PATH_logBase			"XyzzyDM/log/"
#define PATH_logSystem 			"system"
#define PATH_logCmd				"commands"
#define PATH_logChat			"mainchat"
#define PATH_logPM				"pm"
#define PATH_logOtherChats		"otherchats"
#define PATH_logAnnounce		"announces"

#define A_NONE						-1
#define A_CHOWANY					0
#define A_SPS						1
#define A_DERBY						2
#define A_LABIRYNT					3
#define A_RACE						4
#define A_DRIFT						5
#define A_WG						6
#define A_STRZELNICA				7
#define A_ARENA						8
#define A_MINIGAME					9

#define MINIGAME_NONE				0
#define MINIGAME_FISHING			1

#define A_STATE_OFF					0
#define A_STATE_STARTING			1
#define A_STATE_ON					2

// #define VW_UNIQE_JAIL				1000
#define VW_UNIQUE_HOUSE				2000
#define VW_UNIQUE_ARENASOLO			3000
#define VW_UNIQUE_GANG				100
#define VW_DEFAULT					0
#define VW_STRZELNICA					1
#define VW_LABIRYNT					2
#define VW_WG						3
#define VW_CHOWANY					4
#define VW_DERBY					5
#define VW_RACE						6
#define VW_SPS						7
#define	VW_ARENA					8
#define VW_ARENA2					9
#define VW_JAIL						10
#define VW_MISC						15
#define VW_KSW						12
#define	VW_TROLLE					13
#define VW_DRIFT					14

#define VW_BOTS						19

#define CITY_LS 					0
#define CITY_SF 					1
#define CITY_LV 					2

#define LANG_POLISH					0
#define LANG_ENGLISH				1

#define LEAVE_REASON_TIMEOUT		0
#define LEAVE_REASON_LEAVING		1
#define LEAVE_REASON_KICKBAN		2

#define LEVEL_NORMAL				0
#define LEVEL_GM					1
#define LEVEL_ADMIN1				2
#define LEVEL_ADMIN2				3
#define LEVEL_ADMIN3				4
#define LEVEL_ADMINHIDDEN			5

#define ATTACH_TMPMISC	3
#define	ATTACH_FACTION	4


#define DIALOG_LANGUAGE				0
#define DIALOG_LOGIN				1
#define DIALOG_CONFIG_MAIN			2
#define DIALOG_CONFIG_PCOLORS		3
#define DIALOG_CONFIG_CCOLORS		4
#define DIALOG_CONFIG_PCOLORS_SET	5
#define DIALOG_CONFIG_CCOLORS_SET	6
#define DIALOG_NICK_CHANGE			7
#define DIALOG_BAN_CONFIRM			8
#define DIALOG_HELP_MAIN			9
#define DIALOG_HELP_RULES			10
#define DIALOG_HELP_RESPECT			11
#define DIALOG_HELP_STARS			12
#define DIALOG_HELP_VIP				13
#define DIALOG_HELP_AUTHOR			14
#define DIALOG_HELP_CMD				15
#define DIALOG_HELP_CMD_GENERAL		16
#define DIALOG_HELP_CMD_ATRACTIONS	17
#define DIALOG_HELP_CMD_ACCOUNT		18
#define DIALOG_HELP_CMD_HOUSES		19
#define DIALOG_HELP_CMD_RESPECT		20
#define DIALOG_HELP_CMD_TELEPORTS	21	
#define DIALOG_HELP_CMD_ANIMATIONS	22
#define DIALOG_HELP_CMD_VIP			23
#define DIALOG_HELP_CMD_ADMIN		24
#define DIALOG_BANK_MAIN			25
#define DIALOG_BANK_BALANCE			26
#define DIALOG_BANK_DEPOSIT_INPUT	27
#define DIALOG_BANK_DEPOSIT_INFO	28
#define DIALOG_BANK_WITHDRAW_INPUT	29
#define DIALOG_BANK_WITHDRAW_INFO	30
#define DIALOG_BANK_TRANSFER_STEP1	31
#define DIALOG_BANK_TRANSFER_STEP2	32
#define DIALOG_BANK_TRANSFER_STEP3	33
#define DIALOG_BANK_TRANSFER_INFO	34
#define DIALOG_KICK_CONFIRM			35
#define DIALOG_STATS_MAIN			36
#define DIALOG_STATS_PLAYER			37
#define DIALOG_STATS_SERVER			38
#define DIALOG_TOP_MAIN				39
#define DIALOG_TOP_RESPECT			40
#define DIALOG_TOP_FRAGS			41
#define DIALOG_TOP_TIMEPLAYED		42
#define DIALOG_UNBAN_CONFIRM		43
#define DIALOG_BANIP_CONFIRM		44
#define DIALOG_UNBANIP_CONFIRM		45
#define DIALOG_TOP_MONEY			46
#define DIALOG_TOP_SKILL			47
#define DIALOG_STATS_GANG_LIST		48
#define DIALOG_STATS_GANG_INFO		49
#define DIALOG_TOP_GANGS			50
#define DIALOG_CMDSEL				51
#define DIALOG_STYLWALKI			52
//#define DIALOG_CHANGECAR			51
//#define DIALOG_CHANGECOLOR			52
#define DIALOG_HUD					53
#define DIALOG_CONFIG_MAXPING		54
#define DIALOG_CONFIG_ASETTINGS		55
#define DIALOG_CONFIG_ASETTINGS_LIST	56
#define DIALOG_CONFIG_ASETTINGS_NAME	57
#define DIALOG_CONFIG_ASETTINGS_QUEUE	58
#define DIALOG_CONFIG_ASETTINGS_TIME	59
#define DIALOG_PASSWORD_CHANGE			60
#define DIALOG_CONFIG_CENSORSHIP		61
#define DIALOG_CONFIG_CENSORSHIP_LIST	62
#define DIALOG_CONFIG_CENSORSHIP_ADD	63
#define DIALOG_CONFIG_CENSORSHIP_DELETE	64
#define DIALOG_REPORTS				65
#define DIALOG_REPORTS_MANAGE		66


#define DIALOG_PLAYER_INFO			68
#define DIALOG_WEAPON_QUICKBUY		69
#define	DIALOG_ARENASOLO_SELECT		70
#define DIALOG_WEAPON_SELECT		71

#define DIALOG_GANG					200
#define DIALOG_GANG_MAINMENU		(DIALOG_GANG+0)
#define DIALOG_GANG_CONFIRMLEAVING	(DIALOG_GANG+1)





#define TD_DATETIME					33
#define TD_VOTING					34
#define TD_ANNOUNCE					35
#define TD_STARS_START				36
#define TD_STARS_END				66
#define TD_ATTRACTIONBOX			67




// 68-74	to byl gradient od attractionbox, juz nie uzywany

#define	TD_ARACEBOX					68
#define TD_ANN2						69
#define TD_AWGINFO					70	
#define TD_AWGINFOHP				71
#define TD_TIME						72
// 73,74,75 - fullserver, wersja, tekst reklamowy - widoczny w wybieralce skinow

// #define TD_VEHICLEBOX				75
#define TD_ADRIFTBOX				82
#define TD_INFOBOX					83
#define TD_ATTRACTION_TITLES		84
#define TD_ATTRACTION_STATES		85
#define TD_WELCOMEBOX				86
#define TD_WELCOMEBOX2				94
#define TD_WELCOMETITLE				95
#define TD_WELCOMETEXT				96

#define TD_STAT						97

/*#define TD_STAT_ID					97
#define TD_STAT_NICK				98
#define TD_STAT_PING				99
#define TD_STAT_ONLINE_PL			100
#define TD_STAT_ONLINE_EN			101
#define TD_STAT_PLAYERS_PL			102
#define TD_STAT_PLAYERS_EN			103
#define TD_STAT_PLAYERS_DATA		104
#define TD_STAT_VIPS_PL				105
#define TD_STAT_VIPS_EN				106
#define TD_STAT_VIPS_DATA			107
#define TD_STAT_ADMINS_PL			108
#define TD_STAT_ADMINS_EN			109
#define TD_STAT_ADMINS_DATA			110
#define TD_STAT_RESPECT_PL			111
#define TD_STAT_RESPECT_EN			112
#define TD_STAT_SKILL				113*/
#define TD_FULLSERVER				114

// #define TD_FULLSERVERFLASH			116
#define TD_AREAFULLDM				117
#define TD_AREANODM					118
#define TD_WARNING					119

#define PTD_VEHICLEINFO				0
#define PTD_NICK					1
#define PTD_STAT					2
#define PTD_SCORECHANGE				3
#define PTD_ACHIEVEMENT				4
#define PTD_ARACEBOX				5
#define PTD_FPS						6
#define PTD_INFOBOX					7
#define PTD_ADRIFTBOX				8

#define TDE_WIDE		 			0
#define TDE_CITYSELECTPLAYERSLS		1
#define TDE_CITYSELECTPLAYERSSF		2
#define TDE_CITYSELECTPLAYERSLV		3
#define TDE_CITYSELECTLS			4
#define TDE_CITYSELECTSF			5
#define TDE_CITYSELECTLV			6
#define TDE_HINT_CITYSELECT			7
#define TDE_VOTING					8
#define TDE_DATETIME				9
#define TDE_ATTRACTIONBOX			10
#define TDE_VEHICLEBOX				11
#define TDE_INFOBOX					12
#define TDE_WELCOMEBOX				13
#define TDE_STATS					14
#define TDE_FULLSERVERLOGO			15
#define TDE_FPS						16
#define TDE_WYBIERALKA				17

#define COLOR_3DTEXT_INFORMATION	0xF5CB0060
#define COLOR_3DTEXT_HITMAN			0xB51010FF
#define COLOR_3DTEXT_FREEHOUSE		0x72CB00FF
#define COLOR_3DTEXT_PURCHASEDHOUSE	0x0097FFFF

#define HUD_STATUSBAR				0
#define HUD_ATTRACTIONBOX			1
#define HUD_VEHICLEBOX				2
#define HUD_ATTRACTIONNEWS			3
#define HUD_SERVERLOGO				4
#define HUD_VOTING					5
#define HUD_DATE					6
#define HUD_STARS					7
#define HUD_FPS						8


#define UPDATE_TIMER_TIME			1000
#define	AUTO_KEY_REPEAT_TIME		600
#define RESPECT_BONUS_INTERVAL		3600
#define PLAYER_DATA_UPDATE_INTERVAL	60000 * 3
#define PLAYER_COLOR_ALPHA			150
#define NO_GANG						0

#define GANG_RANK_NONE	0
#define GANG_RANK_SUSPENDED	1
#define	GANG_RANK_MEMBER	2
#define	GANG_RANK_LEADER	3
#define GANG_RANK_OWNER		4

#define ARENA_MAX		13	// maksymalna ilosc aren
#define ARENA_NONE		0
#define ARENA_SOLO		1
#define ARENA_PUEBLO	2
#define ARENA_RPG		3
#define ARENA_MINIGUN	4
#define ARENA_PRODM		5
#define	ARENA_ONEDE		6
#define ARENA_ONEDE2	7
#define ARENA_AZTEC		8
#define ARENA_ONESHOOT	9
#define	ARENA_SNIPER	10
#define ARENA_PIER		11
#define	ARENA_SO	12


#define ON_START_MONEY				2000

#define A_WG_TEAM_GREEN				0
#define A_WG_TEAM_BLUE				1

#define A_CHOWANY_HIDDING			0
#define A_CHOWANY_SEARCHING			1

#define PICKUP_HOUSE_NOTPURCHASED	1273
#define PICKUP_HOUSE_PURCHASED		1272

#define ANN_MODE_OFF				0
#define ANN_MODE_FADE_IN			1
#define ANN_MODE_FADE_OUT			2
#define ANN_MODE_ON					3

#define DOOR_OPENED					0
#define DOOR_CLOSED					1

#define BUTTON_NEXT					1
#define BUTTON_BACK					0
#define BUTTON_OK					1
#define BUTTON_QUIT					0
#define BUTTON_YES					1
#define BUTTON_NO					0

#define COLOR_INFO					0
#define COLOR_INFO2					1
#define COLOR_ERROR					2
#define COLOR_JOININFO				3
#define COLOR_LEAVEINFO				4
#define COLOR_ADMIN					5
#define COLOR_ADMIN3				6
#define COLOR_PRIVATEMSG			7
#define COLOR_IC					8
#define COLOR_ME					9
#define COLOR_GM					10
#define COLOR_INFO3					11
#define COLOR_VIPSAY				12
#define COLOR_VIP					13

#define T_PLAYER					0
#define T_BOTH						1
#define T_BOTH_PV					2	// teleport both and preserve velocity

#define f_c_PCOLORS					0
#define f_c_CCOLORS					1
#define f_c_MAXPING					2
#define f_c_ASETTINGS				3
#define f_c_CENSORSHIP				4
#define f_c_CHATCOLORS				5

#define f_c_a_NAME					0
#define f_c_a_QUEUE					1
#define f_c_a_TIME					2

#define f_c_c_ENABLEDISABLE			0
#define f_c_c_LIST					1
#define f_c_c_ADDWORD				2
#define f_c_c_DELETEWORD			3

#define f_h_RULES					0
#define f_h_CMD						1
#define f_h_RESPECT					2
#define f_h_STARS					3
#define f_h_VIP						4
#define f_h_AUTHOR					5

#define f_h_CMD_GENERAL				0
#define f_h_CMD_ATRACTIONS			1
#define f_h_CMD_ACCOUNT				2
#define f_h_CMD_HOUSES				3
#define f_h_CMD_RESPECT				4
#define f_h_CMD_TELEPORTS			5
#define f_h_CMD_ANIMATIONS			6
#define f_h_CMD_VIP					7
#define f_h_CMD_ADMIN				8

#define f_b_BALANCE					0
#define f_b_DEPOSIT					1
#define f_b_WITHDRAW				2
#define f_b_TRANSFER				3

#define f_s_PLAYERSTATS				0
#define f_s_SERVERSTATS				1
#define f_s_GANGSTATS				2

#define f_t_RESPECT					0
#define f_t_SKILL					1
#define f_t_FRAGS					2
#define f_t_MONEY					3
#define f_t_PLAYEDTIME				4
#define f_t_GANGS					5

#define ERROR_NOT_FOUND     		0xFFFE
#define ERROR_FOUND_MULTIPLE		0xFFFF

#define foreach(%1) for(new %1 = 0; %1 <= gmTemp[highestPlayerID]; %1++) if(IsPlayerConnected(%1))
#define HidePlayerDialog(%1) ShowPlayerDialog(%1, -1, 0, " ", " ", " ", " ")
#define dli(%1,%2,%3,%4) ((%1==1)?(%2):(((%1% 10>1)&&(%1% 10<5)&&!((%1% 100>=10)&&(%1% 100<=21)))?(%3):(%4)))
// #define dli(%1,%2,%3) ( (%1==1) ? %2 : %3  )

#define SCM(%1,%2,%3) SendClientMessage(%1, %2, %3)

#define MUST_BE_SPAWNED(%1) if(!(IsPlayerSpawned(%1) || pData[%1][spectating] != INVALID_PLAYER_ID)) return Msg(%1, COLOR_ERROR, TXT(%1, 24))

#define VF_STREET		1	// jezdzi
#define VF_AIRBORNE		2
#define VF_NATATORIAL	4	// plywajace
#define VF_MILITARY		8
#define	VF_RAILROAD		16	// kolejowe
#define VF_RC			32	// zdalnie sterowane
#define VF_ROAD			64	// drogowe
#define VF_AIRPLANE		128	// samoloty
#define VF_HELICOPTER	256
#define VF_BIKES		512	// motory i rowery
#define VF_TRAILER		1024
#define	VF_TOWABLE		2048	// mozliwe do holowania

#define SQL_RI_BACKGROUND	-1	// uzywane przy mysql_query, dla wykonywania zapytan w tle, bez koneicznosci przetwarzania odpowiedz
#define SQL_RI_DOMY_LISTA	2
#define SQL_RI_VEHNAMES		3	// nazwy pojazdow

#define ART_ADD	0
#define ART_UPDATE 1

new gstr[127];

enum e_achievements {
	e_walizki,		// oprogramowane
	e_kills,		// oprogramowane
	e_longplay,		// oprogramowane
	e_racefcnt,		// oprogramowane
	e_racedist,		// oprogramowane
	e_derbyfan,		// oprogramowane
	e_derbywin,		// oprogramowanie
	e_fotocash,		// oprogramowane - kasa wydana na fotoradary
	e_driftpts		// oprogramowane - punkty driftu
}


enum e_gmData
{
	DB_hostname[50],
	DB_username[50],
	DB_password[50],
	DB_database[50],
	DB_prefix[32],
	
	DB_players[32],
	DB_gangs[32],
	DB_bans[32],
	DB_ipbans[32],
	DB_stats[32],
	DB_playersingangs[32],
	DB_config[32],
//	DB_houses[32],
	
	color_normalUser,
	color_fsUser,
	color_vipUser,
	color_gmUser,
	color_lvl1User,
	color_lvl2User,
	color_adminUser,
	color_chatInfo,
	color_chatInfo2,
	color_chatInfo3,
	color_chatError,
	color_chatGM,
	color_chatAdmin,
	color_chatAdmin3,
	color_chatPM,
	color_chatIC,
	color_chatME,
	color_joinInfo,
	color_leaveInfo,
	color_vipSay,
	color_chatVip,
	color_chatInfo_HL,
	color_chatInfo2_HL,
	color_chatInfo3_HL,
	color_chatError_HL,
	color_chatGM_HL,
	color_chatAdmin_HL,
	color_chatAdmin3_HL,
	color_chatPM_HL,
	color_chatIC_HL,
	color_chatME_HL,
	color_joinInfo_HL,
	color_leaveInfo_HL,
	color_vipSay_HL,
	color_chatVip_HL,
	
	artefactOwner,
	
	maxPing,
	bool:censorship,
	maxCensoredWords,
	bool:chatColors,

	serverIP[16],
	serverPORT
}

enum e_gmNpcTypes
{
	gmnt_fullserver,
	gmnt_fullserver_veh
}

enum e_gmTemp
{

	interval_hourBonus,



	interval_starsUpdate,
	interval_jailCheck,
	interval_aTextDrawUpdate,
	interval_aStrzelnicaUpdate,
	interval_artefact,
//	interval_houseCheck,
	interval_statsUpdate,

	interval_aLabiryntUpdate,
	interval_aWGUpdate,
	interval_aDerbyUpdate,
	interval_aRaceUpdate,
	interval_aDriftUpdate,
	interval_aSPSUpdate,
	interval_aChowanyUpdate,
	interval_vehicleRespawn,
	interval_AFKCheck,
	interval_voteCheck,
	interval_gangs,
	
	timer_MoneyPickups_step,
	timer_SavePlayersData_step,	// automatyczna regulacja czestotliwosci zapisywania danych graczy w zaleznosci od ich ilosci
	timer_SavePlayersData_lastidx,
	timer_PingUpdate_lastidx,
	bool:timer_TimeSync_step,
	timer_EPS_step,
	timer_PRA_step,
	timer_Artefakt_step,
	lastWeatherChangeTime,

	gametime_offset,
	lastHour,
	lastDay,
	ann_effect_alpha,
	ann_effect_mode,
	ann2_step,
	bool:ann_switchToNew,
	ann_switchToNewText[128],
	timerAnnounce,
	timerAnnounceEnd,
	timerAnnounce2,
	aStrzelnicaPlayers[MAX_QUEUE_STRZELNICA],
	aStrzelnicaMaxPlayers,
	aStrzelnicaCount,
	aStrzelnicaStartTick,
	timerStrzelnicaCountdown,
	aStrzelnicaShooter,
	moneyPickupIdx,
	fsaIndex,
	fsclIndex,
	fsaTimer,
	fsaCurrentColor,
	aLabiryntPlayers[MAX_QUEUE_LABIRYNT],
	aLabiryntMaxPlayers,
	aLabiryntCount,
	timerLabiryntCountdown,
	aLabiryntStartTick,

	aSPSPlayers[MAX_QUEUE_SPS],
	aSPSMaxPlayers,
	aSPSCount,
	timerSPSCountdown,
	aSPSStartTick,




	aWGPlayers[MAX_QUEUE_WG],
	aWGMaxPlayers,
	aWGCount,
	timerWGCountdown,
	timerWGWeapSel,
	aWGStartTick,
	aWGArena,
	aWGDescr[64],
	aWGInterior,
	Float:aWGWorldBounds[4],


	aWGForceArena,
	Bar:aWGPBar,



	aRacePlayers[MAX_QUEUE_RACE],
	aRacePlayersR[MAX_PLAYERS],
	aRaceDescr[256],
	Float:aRaceDistance,
	aRaceMaxPlayers,
	aRaceCount,
	aRaceArena,
	aRaceInterior,
	aRaceVehicle,
	aRaceCountdown,
	aRaceStartTick,
	aRaceNumCP,
	timerRaceCountdown,
	aRaceForceCircuit,

	aDriftPlayers[MAX_QUEUE_DRIFT],
	aDriftPlayersR[MAX_PLAYERS],
	aDriftDescr[256],
	Float:aDriftDistance,
	aDriftMaxPlayers,
	aDriftCount,
	aDriftArena,
	aDriftInterior,
	aDriftVehicle,
	aDriveCountdown,
	aDriftStartTick,
	aDriftNumCP,
	aDriftParTime,
	timerDriftCountdown,
	aDriftForceCircuit,

	aDerbyPlayers[MAX_QUEUE_DERBY],
	aDerbyMaxPlayers,
	aDerbyCount,
	aDerbyArena,
	aDerbyArenaDescr[25],
	aDerbyInterior,
	aDerbyVehicle,
	timerDerbyCountdown,
	aDerbyStartTick,
	aDerbyForceArena,

	Float:aWGCurrentArea[6],
	aChowanyPlayers[MAX_QUEUE_CHOWANY],
	aChowanyMaxPlayers,
	aChowanyCount,
	aChowanyArena,
	aChowanyForceArena,
	aChowanyDescr[70],
	aChowanyInterior,
	timerChowanyCountdown,
	aChowanyStartTick,
	Float:aChowanyCurrentArea[6],
	
	// glosowanie
	voteQuestion[256],
	voteTimeleft,
	voteAuthor,
	voteCount[2],	// tak, nie

	pPlayerCount,
	pAdminCount,
	pAbsAdminCount,	// wraz z ukrytym
	pVipCount,
	pGMCount,
	highestPlayerID,

	AdminPasswordHash[33],

	bool:updateTimerRunning,
	performBenchmark,

	chatLastMSGSender,
	chatLastText[127],
	chatMSGCount,
	currentWeather,

	snoopPM,	// 0 - nie, 1 - lajt, 2 - all
	showJoins,	// 0 - nie, 1 - dla adminow, 2 - all
	bool:protAll,
	bool:snoopCMD,
	bool:chatDisabled,



	odliczLast,
	playallLast,

	artefactDropped


	
}

enum e_pData
{
	selectedSpawn,
	bool: citySelecting,
	language,
	bool:loggedIn,
	bool:vipEnabled,
	vipDaysLeft,
	vipToDate[12],	// chyba 10
	allowedLevel,	// na jaki poziom moze sie zalogowac
	adminLevel,		// na jaki poziom sie zalogowal
	session,
//	joinCount,
	gang,
	gangRank,
	respect,
	pmoney,

	bool:logonDialog,
	accountID,
	spectating,
	bool:destroyMyVehicle,
	lastVehicle,
	lastUsedSkin,
	bool:classSelecting,
	kills,
	teamkills,
	deaths,
	suicides,
	averagePing,
	skill,
	jail,
	mute,
	pAttraction,
	bool:aChowany,
	bool:aSPS,
	bool:aDerby,
	bool:aLabirynt,
	bool:aRace,
	bool:aDrift,
	bool:aWG,
	bool:aStrzelnica,
	hitman,
//	bool:previewingHouse,
	bool:statsShowed,
	bool:hudSetting[MAX_HUD_ELEMENTS],
	currentColor,
	bool:reported
}

#define ACCEPTS_NONE		0
#define ACCEPTS_REGISTERED	1
#define ACCEPTS_GANG		2
#define ACCEPTS_VIPS		4
#define ACCEPTS_GMS			8
#define ACCEPTS_ADMINS		16
#define ACCEPTS_ALL			(ACCEPTS_ADMINS+ACCEPTS_GMS+ACCEPTS_VIPS+ACCEPTS_GANG+ACCEPTS_REGISTERED)


enum e_acceptation {
	eac_pm,
	eac_solo
}

enum e_pTemp
{
	bool:isRcon,
	bool: holdingKey,
	loginAttemps,
	timerFlashScreen,
//	joinCount,
	bool:firstCitySelect,
	lastDialog,
	lastDialogItem,
	newNick[24],
	tmpTargetPlayerID,
	tmpBanTime,
	tmpReason[128],
	tmpPeriod,
	bool:firstSelectingScreen,
	timerFreezePlayer,
	timerPutInVehicleAfterTeleport,
	timerStopPlayerMusic,
	shipIdle,
//	PlayerText3D:p3D_ship,
	bankTransferAID,
	bankTransferAName[24],
	bankTransferAmount,
	lastMsgTick,
	spamCount,
	tmpPlayerName[24],
	pingSum,
	pingChecks,
	pingWarningCount,
	tmpBanAccountName[24],
	tmpBanIP[24],
	lastSessionSaveTick,
//	lastRespect,
	lastWeaponUsed,
	lastWeaponHolded,
	lastPlayerKilled,
	killStreak,
	Float:specPosition[4],
	specInterior,
	specVirtualWorld,
	bool:specPosReturn,
	statsGangListID[MAX_GANGS],
//	Text3D:hitman3DTextLabel,
	Text3D:FactionName,
	artefactStartTime,
//	currentHouse,
//	tmpHouse,
	tmpConfigAttraction,
	newPassword[32],
	reportedTick[MAX_SERVER_PLAYERS],
	reportSync[MAX_REPORTS],
	reportTmpPlayerId,

	aWGTeam,
	bool:aWGDead,

	aChowanySide,
	bool:aCHDead,

	staleTime,
	lcTicks,	// local chat ticks
	bool:AFK,
	Text3D:p3d_status,
	bool:performingAnim,
	bool:hasVoted,
	bool:tdanimAchievement,
	tdanimScore,	// zawiera wartosc aktualnie pokazywana (ilosc otrzymanego respektu)
	warningReceived,
	bool:firstClassSelection,
	onArena,
	pickupDelay,
	teleCPDelay,

	bonusHours,
	miniGame,
	accepts[e_acceptation],

	tmpLastHouse,
	tmpDialogValue,
	e_houseid,
	drunkLevel,drunkLevelL,

	playerColorAlpha,
	tmpAlpha,

	dialogCB[32],

	bool:stopBGSound,
//	bool:dirtyHack2Reconnect,
	TPInv,
	bool:disableWeaponCheck,

	bool:wStrefieNODM,
	bool:wStrefieFULLDM,

	curPos,
	lastPos,	// uzywane do wykrywania nieruchomych graczy

	bool:dead,

	bool:cenzura,
	bool:protkill,
	bool:protping,
	bool:lockedPos,
	bool:weaponsAllowed,
	bool:troll,

	audio_vehicle,

	vehicleSpecialLastUsed,

	weaponSkill_silenced,
	weaponSkill_pistol,
	weaponSkill_sawnoff,
	weaponSkill_uzi,

	bannedPlayersCnt,	// banned players respect sum - suma respektu zbanowanych graczy - zabezpieczenie przeciwko zbanowaniu wszystkich na serwerze
	bannedPlayersRS,
	faction,
	arenaScore[ARENA_MAX],
	bool:fakeAFK,
	ept_dialogid
	

}

enum e_gData
{
	respect,
	name[32],
	tag[5],
	gColor,
	Float:gBaseSpawn[4],
	Float:gSpawnPoint[4]
}


enum e_gPickup
{
	ship,
	artefact
}

enum e_Vectors
{
	Float:X,
	Float:Y,
	Float:Z,
	Float:A
}

enum e_tpLOC
{
	Float:tpX,Float:tpY,Float:tpZ,Float:tpAngle,
	tpWorld,
	tpInterior,
	bool:tpStreamed
}

enum e_tpRET
{
	Float: tpX,Float:tpY,Float:tpZ,Float:tpA,
	tpDestInt,tpInterior,tpWorld,bool:tpStreamed
}

enum e_aData
{
	aName[32],
	aStartPlayers,
	aState,
	aStartTick,
	aStartingTime
}

enum e_gMoneyPickups
{
	gmpPickupID,
	gmpMoney,
	gmpTime,
	Text3D:gmp3DText
}

enum e_gReports
{
	bool:rOccupiedIndex,
	rPlayerId,
	rPlayerName[24],
	rReportingPlayerName[24],
	rReason[51]
}

enum e_vehOptions {
	bool:vo_static,
	bool:vo_destroyIfEmpty,
	bool:vo_licensePlateSet,
	bool:vo_licensePlate[30],
	bool:vo_private,
	vo_houseid,
	vo_owningPlayerId,
	bool:vo_used,
	bool:vo_hasTurbo,		// nitro wspomagane turbem
	bool:vo_occupied,
	vo_color[2],
	vo_paintjob,
	vo_driver,
	bool:vo_drift		// auto drifterskie
}

new
 gmData[e_gmData],
 gLang[MAX_LANGUAGES][MAX_LANG_ROWS][MAX_LANG_TEXT],
 pData[MAX_SERVER_PLAYERS][e_pData],
 gData[MAX_GANGS + 1][e_gData],
 aData[MAX_ATTRACTIONS][e_aData],
// hData[MAX_HOUSES][e_hData],
 gmTemp[e_gmTemp],
// gTemp[e_gTemp],
 pTemp[MAX_SERVER_PLAYERS][e_pTemp],
 gPickup[e_gPickup],
 gMoneyPickup[MAX_MONEY_PICKUPS][e_gMoneyPickups],
 gCensoredWord[MAX_CENSORED_WORDS][32],
 gReports[MAX_REPORTS][e_gReports],
 gmNPC[e_gmNpcTypes]={ INVALID_PLAYER_ID },
 
 Text:gTextDraw[MAX_TEXTDRAWS],
 Text:pTextDraw[MAX_PLAYER_TEXTDRAWS][MAX_SERVER_PLAYERS],
 vehicleDoorState[MAX_VEHICLES],
 vehicleDoorOwner[MAX_VEHICLES],
 bool:vehicleHasNitro[MAX_VEHICLES]={false,...},
 MySQL:hMySQL,

 bool:tVehicleUsed[MAX_VEHICLES]={false,...},
 bool:tVehicleSpawned[MAX_VEHICLES]={false,...},

 bool:tVehicles[MAX_VEHICLES][e_vehOptions],

 oKratki[MAX_PLAYERS]={INVALID_OBJECT_ID,...};

new PL_fNames_config[][] = {
 "Kolory graczy",
 "Kolory informacji na czacie",
 "Maksymalny dopuszczalny ping",
 "Ustawienia atrakcji",
 "Cenzura s³ów",
 "Kolorowanie czatu"
};

new EN_fNames_config[][] = {
 "Player colours",
 "Chat notification colours",
 "Maximum ping value",
 "Attraction settings",
 "Word censorship",
 "Chat colour embedding"
};

new PL_fNames_config_pColors[][] = {
 "Domyœlny",
 "Gracz z tagiem [FS]",
 "VIP",
 "Game Moderator",
 "Admin - poziom 1",
 "Admin - poziom 2",
 "Admin - poziom 3 / RCON"
};

new EN_fNames_config_pColors[][] = {
 "Default",
 "Player with [FS]",
 "VIP",
 "Game Moderator",
 "Admin - level 1",
 "Admin - level 2",
 "Admin - level 3 / RCON"
};

enum e_WeaponData {
	ewd_id,
	ewd_PLname[21],
	ewd_baseCost,
	ewd_baseAmmo,
	ewd_slot,
	bool:ewd_quickBuy,
	bool:ewd_damagingWeapon
}


new quickbuyWeapons[][e_WeaponData]={
	{0, "Piesci", 0, 0, 0, false, true},
	{1, "Kastet", 100, 1, 0, true, true},
	{2, "Kij golfowy", 200, 1, 1, true, true},
	{3, "Palka policyjna", 500, 1, 1, true, true},
	{4,	"Noz", 250, 1, 1, true, true},
	{5, "Baseball", 400, 1, 1, true, true},
	{8, "Katana", 2500, 1, 1, true, true},
	{14,"Kwiaty", 300,1, 10, true, true},
	{22,"Pistolet 9mm", 4000, 500, 2, true, true},
	{23,"Pistolet z tlumikiem", 4000, 500, 2, true, true},
	{24,"Desert Eagle", 5000, 1000, 2, true, true},
	{25,"Shotgun", 5000, 500, 3, true, true},
	{26,"Sawn-off Shotgun", 8000, 500, 3, true, true},
	{27,"Combat Shotgun", 25000, 500, 3, true, true},
	{28,"Micro SMG",10000, 1000, 4, true, true},
	{29,"MP5",10000, 1000, 4, true, true},
	{30,"AK-47",18000,1000, 5, true, true},
	{31,"M4",20000,1000, 5, true, true},
	{32,"Tec9",10000,1000, 4, true, true},
	{33,"Strzelba",5000,250,6, true, true},
	{34,"Snajperka",20000,400,6, true, true},
	{42,"Gasnica", 500, 250,9, true, true},
	{43,"Aparat", 500, 100, 9, true, false},
	{46,"Spadochron", 250, 1, 11, true, false}
};

new weaponSlots[]={0,0,1,1,1,1,1,1,1,1,10,10,10,10,10,10,8,8,8,-1,-1,-1,2,2,2,3,3,3,4,4,5,5,4,6,6,7,7,7,7,8,12,9,9,9};
new weaponName[][22]={
	"Piêœci",	// 0
	"Kastet",
	"Kij golfowy",
	"Pa³ka policyjna",
	"Nó¿",		// 4
	"Kij baseballowy",
	"£opata",
	"Kij bilardowy",
	"Katana",
	"Pi³a mechaniczna",
	"Fioletowe dildo",
	"Bia³e dildo",
	"D³ugie bia³e dildo",
	"Bia³e dildo",
	"Kwiaty",
	"Laska",	// 15
	"Granaty",
	"Gas ³zawi¹cy",
	"Koktail molotova",
	"none",
	"none",
	"none",
	"Pistolet 9mm",
	"Pistolet z t³umikiem",
	"Desert Eagle",
	"Shotgun",
	"Sawn-off Shotgun",
	"Combat Shotgun",
	"Micro Uzi",
	"MP5",
	"AK-47",
	"M4",
	"Tec9",
	"Strzelba",
	"Sniper Rifle",
	"RPG",
	"Wyrzutnia rakiet",
	"Miotacz ognia",
	"Minigun",
	"£adunki wybuchowe",
	"Detonator",
	"Spray",
	"Gaœnica",
	"Aparat",
	"Gogle noktowizyjne",
	"Gogle termiczne",
	"Spadochron"
};






/*	
new PL_fNames_weapons_quickbuy[][]={
	"",					//Brak (piesci)",
	"Kastet",
	"Kij golfowy",
	"Palka policyjna",
	"Noz",		//4
	"Kij baseballowy",
	"",					// Lopata",				6
	"",					// Kij bilardowy",		7
	"Katana",									8
	"",					// Pila mechaniczna", 	9
	"",					// Fioletowe dildo",	10
	"",					// Biale dildo",		11
	"",					// Dlugie biale dildo",	12
	"",					// Biale dildo",		13
	"Kwiaty",									14
	"",					// Laska",				15
	"",					// Granaty",			16
	"",					// Gas lzawiacy",		17
	"",					//Koktail molotova",	18
	"",					// none",				19
	"",					// none",				20
	"",					// none",				21
	"Pistolet 9mm",								22
	"Pistolet z tlumikiem",
	"Desert Eagle",
	"Shotgun",
	"Sawn-off Shotgun",
	"Combat Shotgun",
	"Micro Uzi",
	"MP5",
	"AK-47",
	"M4",
	"Tec9",
	"Strzelba",
	"Sniper Rifle",
	"",					// RPG",
	"",					// Wyrzutnia rakiet",
	"",					// Miotacz ognia",
	"",					// Minigun",
	"",					// Ladunki wybuchowe",
	"",					// Detonator",
	"",					// Spray",
	"Gasnica",
	"Aparat",
	"",					// Gogle noktowizyjne",
	"",					// Gogle termiczne",
	"Spadochron"
}

new baseWeaponPrices[]={
	0,
	100,
	100,
	500,
	250,
	500,
	0,
	0,
	2500,
	0,
	0,
	0,
	0,
	0,
	300,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	4000,
	6000,// "Pistolet z tlumikiem",
	5000,// "Desert Eagle",			// Shotgun - $5000\nSawn-off - $8000\nCombat Shotgun - $25000\nMicro SMG - $10000\nMP5 - $12000\nAK-47 - $13000\nM4 - $15000\n
									//Tec9 - $10000\nCountry Rifle - $5000\nSniper Rifle - $20000\nSpray - $100\nGasnica - $500\nSpadochron - $100");
	5000,// "Shotgun",
	8000,//"Sawn-off Shotgun",
	25000,//"Combat Shotgun",
	10000,//"Micro Uzi",
	12000,//"MP5",
	20000,//"AK-47",
	18000,//"M4",
	10000,//"Tec9",
	5000,//"Strzelba",
	20000,//"Sniper Rifle",
	0,//"",					// RPG",
	0,//"",					// Wyrzutnia rakiet",
	0,//"",					// Miotacz ognia",
	0,//"",					// Minigun",
	0,//"",					// Ladunki wybuchowe",
	0,//"",					// Detonator",
	0,//"",					// Spray",
	500,//"Gasnica",
	500,//"Aparat",
	0,//"",					// Gogle noktowizyjne",
	0,//"",					// Gogle termiczne",
	250//"Spadochron"
}
*/


new PL_fNames_config_cColors[][] = {
 "Normalna osobista informacja",
 "Wa¿na osobista informacja",
 "Globalna informacja",
 "B³¹d",
 "Do³¹czenie do serwera",
 "Opuszczenie serwera",
 "Czat moderatora",
 "Czat admina",
 "Czat admina (poziom 3)",
 "Prywatna wiadomoœæ",
 "Czat lokalny",
 "/me",
 "(wyró¿nienie) Normalna osobista informacja",
 "(wyró¿nienie) Wa¿na osobista informacja",
 "(wyró¿nienie) Globalna informacja",
 "(wyró¿nienie) B³¹d",
 "(wyró¿nienie) Do³¹czenie do serwera",
 "(wyró¿nienie) Opuszczenie serwera",
 "(wyró¿nienie) Czat moderatora",
 "(wyró¿nienie) Czat admina",
 "(wyró¿nienie) Czat admina (poziom 3)",
 "(wyró¿nienie) Prywatna wiadomoœæ",
 "(wyró¿nienie) Czat lokalny",
 "(wyró¿nienie) /me"
};

new EN_fNames_config_cColors[][] = {
 "Personal normal information",
 "Personal important information",
 "Global information",
 "Error",
 "Player joins the server",
 "Player leaves the server",
 "Game moderator chat",
 "Admin chat",
 "Master admin chat (level 3)",
 "Private message",
 "Local chat",
 "/me",
 "(highlight) Personal normal information",
 "(highlight) Personal important information",
 "(highlight) Global information",
 "(highlight) Error",
 "(highlight) Player joins the server",
 "(highlight) Player leaves the server",
 "(highlight) Game moderator chat",
 "(highlight) Admin chat",
 "(highlight) Master admin chat (level 3)",
 "(highlight) Private message",
 "(highlight) Local chat",
 "(highlight) /me"
};

new PL_fNames_config_attraction[][] = {
 "Nazwa",
 "Wielkoœæ kolejki",
 "Czas startowania"
};

new EN_fNames_config_attraction[][] = {
 "Name",
 "Queue size",
 "Starting time"
};

new PL_fNames_help[][] = {
 "Regulamin serwera",
 "Komendy",
 "Respekt",
 "Gwiazdki",
 "Konta VIP",
 "O autorach"
};

new EN_fNames_help[][] = {
 "Server rules",
 "Commands",
 "Respect",
 "Stars",
 "VIP accounts",
 "About the author"
};

new PL_fNames_help_cmd[][] = {
 "Podstawowe",
 "Atrakcje",
 "Zarz¹dzanie kontem",
 "Domy",
 "Respekt",
 "Teleporty",
 "Animacje",
 "VIP"
// "Admin"
};

new EN_fNames_help_cmd[][] = {
 "General",
 "Attractions",
 "Account management",
 "Houses",
 "Respect",
 "Teleports",
 "Animations",
 "VIP"
// "Admin"
};

new PL_fNames_bank[][] = {
 "Stan konta",
 "Wp³ata",
 "Wyp³ata",
 "Przelew"
};

new EN_fNames_bank[][] = {
 "Balance",
 "Deposit",
 "Withdraw",
 "Transfer"
};

new PL_fNames_stats[][] = {
 "Twoje statystyki",
 "Statystyki serwera",
 "Statystyki gangów"
};

new EN_fNames_stats[][] = {
 "Your statistics",
 "Server statistics",
 "Gang statistics"
};

new PL_fNames_top[][] = {
 "Respekt",
 "Skill",
 "Fragi",
 "Pieni¹dze",
 "Czas gry",
 "Gangi"
};

new EN_fNames_top[][] = {
 "Respect",
 "Skill",
 "Kills",
 "Money",
 "Time played",
 "Gangs"
};

new PL_fNames_hud[][] = {
 "Pasek stanu",
 "Tabela atrakcji",
 "Status pojazdu",
 "Informacje do atrakcji",
 "Logo serwera",
 "Strefa DM",
 "Data, godzina, ilosc graczy",
 "Gwiazdki",
 "Licznik FPS"
};

new EN_fNames_hud[][] = {
 "Status bar",
 "Attraction box",
 "Vehicle status box",
 "Current attraction news",
 "Server logo",
 "DM area",
 "Time, date, number of players",
 "Stars",
 "FPS counter"
};

new PL_fNames_censorship_off[][] = {
 "W³¹cz cenzurê",
 "Lista s³ów",
 "Dodaj nowe s³owo",
 "Usuñ s³owo"
};

new EN_fNames_censorship_off[][] = {
 "Enable censorship",
 "Word list",
 "Add new word",
 "Delete word"
};

new PL_fNames_censorship_on[][] = {
 "Wy³¹cz cenzurê",
 "Lista s³ów",
 "Dodaj nowe s³owo",
 "Usuñ s³owo"
};

new EN_fNames_censorship_on[][] = {
 "Disable censorship",
 "Word list",
 "Add new word",
 "Delete word"
};

new PL_szHelpRules[][] = {
	"#1. Zakaz uzywania cheatow, spamerow, trainerow itd.",
	"#2. Zakaz wyzywania innych oraz przeklinania na czacie",
	"#3. Zakaz reklamowania innych serwerow",
	"#4. Zakaz zabijania w strefach bez DM",
	"#5. Zakaz przeszkadzania w zabawach zorganizowanych",
	"#6. Zakaz CK, DB, HK za wyjatkiem ''Strefy smierci''",
	" ",
	"Pelny regulamin serwera dostepny jest na forum",
	"{ff3030}http://www.fullserver.eu/"
};

new EN_szHelpRules[][] = {
 "rules"
};

new PL_szHelpRespect[][] = {
// "Pomoc w przygotowaniu..."
 "Respekt zdobywasz poprzez:",
 " - gre - automatycznie co ok. minute dostajesz kilka punktow, oraz premie o kazdej pelnej godzinie gry",
 " - udzial w atrakcjach - punkty respektu sa nagroda w kazdej z atrakcji na zapisy",
 " - zbieranie bonusow: walizek, prezentow, podkow,",
 " - udzial w eventach organizowanych przez moderatorow/adminow,",
 " ",
 "Dzieki wyzszemu respektowi:",
 " - masz lepsze uzbrojonie na spawnie,",
 " - uzyskujesz dostep do szerszego arsenalu broni (granaty, koktajle molotowa!),",
 " - zdobywasz uznanie wsrod innych graczy,",
 " ",
 "Mozesz kupic kilka udogodnien, w zamian za swoj respekt. Wpisz /rcmd aby zakupic: niewidzialnosc, armor,",
 "wiekszy skill broni + dwie bronie na spawnie (dualso/uzi/pistolety). Respekt mozesz rowniez spozytkowac",
 "na oplacenie wynajetego domu."

};

new EN_szHelpRespect[][] = {
// "respect"
 "Respekt zdobywasz poprzez:",
 " - gre - automatycznie co ok. minute dostajesz kilka punktow, oraz premie o kazdej pelnej godzinie gry",
 " - udzial w atrakcjach - punkty respektu sa nagroda w kazdej z atrakcji na zapisy",
 " - zbieranie bonusow: walizek, prezentow, podkow,",
 " - udzial w eventach organizowanych przez moderatorow/adminow,",
 " ",
 "Dzieki wyzszemu respektowi:",
 " - masz lepsze uzbrojonie na spawnie,",
 " - uzyskujesz dostep do szerszego arsenalu broni (granaty, koktajle molotowa!),",
 " - zdobywasz uznanie wsrod innych graczy,",
 " ",
 "Mozesz kupic kilka udogodnien, w zamian za swoj respekt. Wpisz /rcmd aby zakupic: niewidzialnosc, armor,",
 "wiekszy skill broni + dwie bronie na spawnie (dualso/uzi/pistolety). Respekt mozesz rowniez spozytkowac",
 "na oplacenie wynajetego domu."

};

new PL_szHelpStars[][] = {
 "{FF6501}Pomaranczowe {ffffff} - 1-29 respektu",
 "{E3C759}Zolte {ffffff} - 30-209 respektu",
 "{30FF30}Zielone {ffffff} - 210-809 respektu",
 "{3030FF}Niebieskie {ffffff} - 810-6809 respektu",
 "{FF3030}Czerwone {ffffff} - 6810+ respektu",
 " ",
 "Bronie na spawnie: ",
 " 0+\tkastet/kwiaty, deagle, mp5",
 " ",
 " 1000+\tshotgun",
 " 1500+\t1/4 kamizelki",
 " 2000+\tm4",
 " 3500+\tdodatkowa amunicja, 1/2 kamizelki",
 " 5000+\t3/4 kamizelki",
 " 7000+\tsnajperka",
 " 10000+\tnoz",
 " 15000+\tcala kamizelka",
 " 40000+\tkoktajle molotowa",
 " 75000+\tgranaty"
};

new EN_szHelpStars[][] = {
 "stars"
};

new PL_szHelpVIP[][] = {
	"{FFEF00}Mozliwosci konta VIP:",
	"{a9c4e4} ",
	"* Spawnowanie za darmo wszystkich pojazdow",
	"* Spawnowanie dowolnego pojazdu podajac jego nazwe/ID",
 	"* Dodawanie sobie jedna komenda zestawu broni za darmo",
    "* Dodawanie sobie dowolnej broni wraz z amunicja",
    "* Pisanie na czacie jako VIP",
    "* Pisanie na prywatnym czacie Vipow i Adminow",
    "* Naprawianie pojazdu dowolnemu graczowi za darmo",
    "* Posiadanie wyrozniajacego sie koloru Zoltego",
    "* Uzdrawianie dowolnego gracza za darmo",
    "* Dodawanie sobie/komus kamizelki kuloodpornej za darmo",
	"* Teleportowanie graczy do siebie i od siebie",
	" ",
	"Zostan VIPem - doladuj swoje konto"
};

new EN_szHelpVIP[][] = {
 "VIP accounts"
};

new PL_szHelpAuthor[][] = {
 "{0092F5}XyzzyDM {F5C172} wersja publiczna",
 GMVERSION, GMCOMPILED,
 " ",
 "{A9C4E4}Kod: {C1D0E5}Arnold \"SeriouS\" Fornalczyk {A9C4E4}oraz {C1D0E5}Lukasz \"Wielebny\" Biegaj",
 " ",
 "{A9C4E4}© Copyright 2010-2012 - FullServer™ - www.FullServer.eu"
};

new EN_szHelpAuthor[][] = {
 "{0092F5}FullServer™ {F5C172}Deathmatch",
 " ",
 "{A9C4E4}Scripting: {C1D0E5}Arnold \"SeriouS\" Fornalczyk {A9C4E4}and {C1D0E5}XJL",
 "{A9C4E4}Thanks to the creators of buildings, arenas and other elements of the game",
 "{C1D0E5}CTX123, Bas_, DavO, LU, NeutroN, Kicia, Sayu,",
 "Saracen, Sebox, Surv, Times, Theo, WUBE and others",
 " ",
 "{A9C4E4}Credits for: {C1D0E5}DracoBlue, Incognito (Streamer Plugin), Sacky",
 "StrickenKid (MySQL Plugin), Zeex (ZCMD System)", // Totto8492 (GeoIP Plugin), 
 " ",
 "{A9C4E4}© Copyright 2010-2011 - FullServer™ - www.FullServer.eu"
};

new PL_szHelpCmdGeneral[][] = {
 "Dokladny opis komend jest w przygotowaniu i bedzie dostepny w ciagu kilku dni",
 " ",
 "{60ff90}/100hp /bronie /rsp /spadochron /kill /lp /sp /telpos /savepos /staty",
 "{30ff60}/p /pojazdy /raport /pm /l /gang /top /hitman /fps",
 "{6030ff}/lock /unlock /maska /bagaznik /napraw /flip /tune /tunemenu /kolorauto",
 "{ff6030}/credits /pomoc /admins /vips"
};

new EN_szHelpCmdGeneral[][] = {
 "gener"
};

new PL_szHelpCmdAtractions[][] = {
 "{ffffff}Na zapisy:{A5C3E7}",
 "   /chowany, /derby, /labirynt, /race, /wg, /strzelnica",
 "{ffffff}/areny:{A5C3E7}",
 "   /pro, /rpg, /minigun, /arenasolo, /solo",
 "   /onede, /onede2, /oneshoot",
 "{ffffff}Minigry:{A5C3E7}",
 "   /tir, /warsztat, /policja, /dilerzy, /drag, /sky",
 "{ffffff}stunt+fun:{A5C3E7}",
 "   /zjazd, /zjazd2, /zjazd3, /stunt, /stunt2, /stuntpipe",
 "   /stuntpark, /uboot, /kosciol, /sky, /parkour, /andromeda",
 "   /vip",
 "{ffffff}Full DM:{A5C3E7}",
 "   /afganistan, /wietnam, /lv"
};

new EN_szHelpCmdAtractions[][] = {
 "attraction commands"
};

new PL_szHelpCmdAccount[][] = {
 "/rejestracja",
 "/nowynick - pozwala przeniesc statystyki na nowy nick",
 "/zmienhaslo",
 "/pmon, /pmoff",
 "/soloacc"
};

new EN_szHelpCmdAccount[][] = {
 "account management commands"
};

new PL_szHelpCmdHouses[][] = {
 "Aby zarzadzac domem, wejdz w pickup przy wejsciu",
 "i wybierz pozadana opcje z MENU. Nie ma potrzeby",
 "pamietania kilkudziesieciu komend, wystarcza tylko dwie:",
 " ",
 "/dom - przenosi pod dom",
 "/mp - przenosi do zapisanego pojazdu",
 " ",
 "Mozesz przegladnac liste wszystkich domow, zobaczyc je na mapie,",
 "a takze dokonac zakupu/przedluzenia waznosci domu (bez koniecznosci",
 "wydawania respektu) na naszej stronie:",
 " ",
 "{ff3030}www.fullserver.eu/domy/{ffffff}"
};

new EN_szHelpCmdHouses[][] = {
 "commands for houses"
};

/*new PL_szHelpCmdRespect[][] = {

};

new EN_szHelpCmdRespect[][] = {
 "commands for respect"
};*/

new PL_szHelpCmdTeleports[][] = {
 "/wyspa /wyspas /osiedle(1-5)  /g(1-4)  /wojsko  /wojsko2  /tama  /miasteczko  /impra  /tunels",
 "/tunesf  /tunelv  /molo  /zadupie  /lslot  /sflot  /lvlot  /kosciol  /chiliad  /lot  /stlot",
 "/zakochani  /ammo  /plazasf  /wiezowiec  /skatepark  /plaza  /pgr  /dd  /kart  /db",
 "/kss  /vc  /statek  /baza3  /stunt /stuntpark /stuntpipe /4smoki  /zjazd  /zjazd2  /tereno ",
 "/gs /ls  /sf /sf2 /lv  /drift  /rcshop  /cpn  /cjgarage  /calligula  /andromeda  /wooziebed",
 "/jaysdin  /woc  /tsdin  /wh  /wh2  /resta  /brothel  /brothel2  /libertycity ",
 "/uboot /poszukiwacze"
};

new EN_szHelpCmdTeleports[][] = {
 "/wyspa  /osiedle(1-5)  /g(1-4)  /wojsko  /wojsko2  /tama  /miasteczko  /impra  /tunels",
 "/tunesf  /tunelv  /molo  /zadupie  /lslot  /sflot  /lvlot  /chilliad  /lot  /lot2",
 "/zakochani  /ammo  /plazasf  /wiezowiec  /skatepark  /plaza  /pgr  /dd  /kart  /db",
 "/kss  /vc  /statek  /baza3  /stuntcity  /4smoki  /zjazd  /zjazd2  /tereno  /forteca",
 "/ls  /sf  /lv  /drift1-7  /rcshop  /cpn  /cjgarage  /calligula  /andromeda  /wooziebed",
 "/jaysdin  /woc  /tsdin  /wh  /wh2  /resta  /brothel  /brothel2  /libertycity /church",
 "/uboot"
};

new PL_szHelpCmdAnimations[][] = {
 "komendy animacji"
};

new EN_szHelpCmdAnimations[][] = {
 "animation commands"
};

new PL_szHelpCmdVIP[][] = {
 "{FFEF00}Komendy VIP-ów",
 "{A9C4E4} ",
 "/varmor [id/nick] - dodawanie armora sobie/komus",
 "/vheal [id/nick] - uleczanie siebie/kogos",
 "/vtt [id/nick] - przenoszenie do gracza (za zgoda)",
 "/vth [id/nick] - przenoszenie gracza do siebie (za zgoda)",
 "/vsay <tekst> - pisanie na czacie jako VIP",
 "/v - prywatny czat VIPow, GMow i Adminow",
 "/vbron - sprawnowanie broni",
 "/vzestaw - zestaw broni VIPA",
 "/vpozostalo - sprawdzanie waznosci konta VIP",
 "/vnapraw [id/nick] - naprawia pojazd swoj/gracza",
 " ",
 "Doladowania kont VIP pod adresem",
 "{FFEF00}http://www.fullserver.eu/doladowania/"
};

new EN_szHelpCmdVIP[][] = {
 "VIP commands"
};

new PL_szHelpCmdAdmin[][] = {
 "komendy admina"
};

new EN_szHelpCmdAdmin[][] = {
 "admin commands"
};

new PL_szStatsPlayer[][] = {
 "{A9C4E4}Data rejestracji: {FFFFFF}%s",
 "{A9C4E4}Respekt: {FFFFFF}%i",
 "{A9C4E4}Fragów (swoich cz³onków gangu): {FFFFFF}%i (%i)",
 "{A9C4E4}Œmierci (samobójstw): {FFFFFF}%i (%i)",
 "{A9C4E4}Liczba kicków: {FFFFFF}%s",
 "{A9C4E4}Liczba banów: {FFFFFF}%s",
 "{A9C4E4}Œredni ping: {FFFFFF}%i",
 "{A9C4E4}£¹czny czas gry: {FFFFFF}%i %s"
};

new EN_szStatsPlayer[][] = {
 "{A9C4E4}Registered date: {FFFFFF}%s",
 "{A9C4E4}Respect: {FFFFFF}%i",
 "{A9C4E4}Kills (team-kills): {FFFFFF}%i (%i)",
 "{A9C4E4}Deaths (suicides): {FFFFFF}%i (%i)",
 "{A9C4E4}Kick count: {FFFFFF}%i",
 "{A9C4E4}Ban count: {FFFFFF}%i",
 "{A9C4E4}Average ping: {FFFFFF}%i",
 "{A9C4E4}Time played: {FFFFFF}%i %s"
};

new PL_szStatsServer[][] = {
 "{A9C4E4}Najwiêcej graczy: {FFFFFF}%s (%s)",
 "{A9C4E4}Zarejestrowanych graczy: {FFFFFF}%i",
 "{A9C4E4}Wejœæ na serwer: {FFFFFF}%s",
 "{A9C4E4}Fragów: {FFFFFF}%s",
 "{A9C4E4}Œmierci: {FFFFFF}%s",
 "{A9C4E4}Kicków: {FFFFFF}%s",
 "{A9C4E4}Banów: {FFFFFF}%s"
};

new EN_szStatsServer[][] = {
 "{A9C4E4}Most players online: {FFFFFF}%s (%s)",
 "{A9C4E4}Registered players: {FFFFFF}%i",
 "{A9C4E4}Server joins: {FFFFFF}%s",
 "{A9C4E4}Kills: {FFFFFF}%s",
 "{A9C4E4}Deaths: {FFFFFF}%s",
 "{A9C4E4}Kicks: {FFFFFF}%s",
 "{A9C4E4}Bans: {FFFFFF}%s"
};

new PL_szTopRespect[][] = {
 "respekt"
};

new EN_szTopRespect[][] = {
 "respect"
};

new PL_szTopFrags[][] = {
 "fragi"
};

new EN_szTopFrags[][] = {
 "frags"
};

new PL_szTopPlayedTime[][] = {
 "czas gry"
};

new EN_szTopPlayedTime[][] = {
 "time played"
};

enum e_pogody {
	ep_id,
	ep_nazwa[16],
	ep_czastrwania
}


new DATA_weathers[] = {0, 1, 2, 3, 4, 5, 7, 9, 12, 17, 20};
	// mgliste: 10

new vehicleName[][33] = {
 "Landstalker",
 "Bravura",
 "Buffalo",
 "Linerunner",
 "Perenial",
 "Sentinel",
 "Dumper",
 "Firetruck",
 "Trashmaster",
 "Stretch",
 "Manana",
 "Infernus",
 "Voodoo",
 "Pony",
 "Mule",
 "Cheetah",
 "Ambulance",
 "Leviathan",
 "Moonbeam",
 "Esperanto",
 "Taxi",
 "Washington",
 "Bobcat",
 "Mr Whoopee",
 "BF Injection",
 "Hunter",
 "Premier",
 "Enforcer",
 "Securicar",
 "Banshee",
 "Predator",
 "Bus",
 "Rhino",
 "Barracks",
 "Hotknife",
 "Trailer",
 "Previon",
 "Coach",
 "Cabbie",
 "Stallion",
 "Rumpo",
 "RC Bandit",
 "Romero",
 "Packer",
 "Monster",
 "Admiral",
 "Squalo",
 "Seasparrow",
 "Pizzaboy",
 "Tram",
 "Trailer",
 "Turismo",
 "Speeder",
 "Reefer",
 "Tropic",
 "Flatbed",
 "Yankee",
 "Caddy",
 "Solair",
 "Berkley's RC Van",
 "Skimmer",
 "PCJ-600",
 "Faggio",
 "Freeway",
 "RC Baron",
 "RC Raider",
 "Glendale",
 "Oceanic",
 "Sanchez",
 "Sparrow",
 "Patriot",
 "Quad",
 "Coastguard",
 "Dinghy",
 "Hermes",
 "Sabre",
 "Rustler",
 "ZR-350",
 "Walton",
 "Regina",
 "Comet",
 "BMX",
 "Burrito",
 "Camper",
 "Marquis",
 "Baggage",
 "Dozer",
 "Maverick",
 "News Chopper",
 "Rancher",
 "FBI Rancher",
 "Virgo",
 "Greenwood",
 "Jetmax",
 "Hotring",
 "Sandking",
 "Blista Compact",
 "Police Maverick",
 "Boxville",
 "Benson",
 "Mesa",
 "RC Goblin",
 "Hotring Racer",
 "Hotring Racer",
 "Bloodring Banger",
 "Rancher",
 "Super GT",
 "Elegant",
 "Journey",
 "Bike",
 "Mountain Bike",
 "Beagle",
 "Cropdust",
 "Stunt",
 "Tanker",
 "RoadTrain",
 "Nebula",
 "Majestic",
 "Buccaneer",
 "Shamal",
 "Hydra",
 "FCR-900",
 "NRG-500",
 "HPV1000",
 "Cement Truck",
 "Tow Truck",
 "Fortune",
 "Cadrona",
 "FBI Truck",
 "Willard",
 "Forklift",
 "Tractor",
 "Combine",
 "Feltzer",
 "Remington",
 "Slamvan",
 "Blade",
 "Freight",
 "Streak",
 "Vortex",
 "Vincent",
 "Bullet",
 "Clover",
 "Sadler",
 "Firetruck",
 "Hustler",
 "Intruder",
 "Primo",
 "Cargobob",
 "Tampa",
 "Sunrise",
 "Merit",
 "Utility",
 "Nevada",
 "Yosemite",
 "Windsor",
 "Monster",
 "Monster",
 "Uranus",
 "Jester",
 "Sultan",
 "Stratum",
 "Elegy",
 "Raindance",
 "RC Tiger",
 "Flash",
 "Tahoma",
 "Savanna",
 "Bandito",
 "Freight",
 "Trailer",
 "Kart",
 "Mower",
 "Duneride",
 "Sweeper",
 "Broadway",
 "Tornado",
 "AT-400",
 "DFT-30",
 "Huntley",
 "Stafford",
 "BF-400",
 "Newsvan",
 "Tug",
 "Trailer",
 "Emperor",
 "Wayfarer",
 "Euros",
 "Hotdog",
 "Club",
 "Freight box",
 "Trailer",
 "Andromada",
 "Dodo",
 "RC Cam",
 "Launch",
 "Police Car (LSPD)",
 "Police Car (SFPD)",
 "Police Car (LVPD)",
 "Police Ranger",
 "Picador",
 "S.W.A.T. Van",
 "Alpha",
 "Phoenix",
 "Glendale",
 "Sadler",
 "Luggage Trailer",
 "Luggage Trailer",
 "Stair Trailer",
 "Boxville",
 "Farm Plow",
 "Utility Trailer"
};

new defaultColorsFields[][] = {
// Player colors
 "color_normalUser",
 "color_fsUser",
 "color_vipUser",
 "color_gmUser",
 "color_lvl1User",
 "color_lvl2User",
 "color_adminUser",
// Chat colors
 "color_chatInfo",
 "color_chatInfo2",
 "color_chatInfo3",
 "color_chatError",
 "color_chatGM",
 "color_chatAdmin",
 "color_chatAdmin3",
 "color_chatPM",
 "color_chatIC",
 "color_chatME",
 "color_joinInfo",
 "color_leaveInfo",
 "color_vipSay",
 "color_chatVip",
// Chat colors (highlight)
 "color_chatInfo_HL",
 "color_chatInfo2_HL",
 "color_chatInfo3_HL",
 "color_chatError_HL",
 "color_chatGM_HL",
 "color_chatAdmin_HL",
 "color_chatAdmin3_HL",
 "color_chatPM_HL",
 "color_chatIC_HL",
 "color_chatME_HL",
 "color_joinInfo_HL",
 "color_leaveInfo_HL",
 "color_vipSay_HL",
 "color_chatVip_HL",
// Attractions
 "a_chowany",
 "a_sps",
 "a_derby",
 "a_labirynt",
 "a_race",
 "a_drift",
 "a_wg",
 "a_strzelnica",
 "censorship", // Censorship
 "max_ping" // Max ping
 
};

new defaultColorsData[][] = {

// Player colors
 "909090",
 "00A1FF",
 "FACB00",
 "58A100",
 "900000",
 "A50000",
 "C00000",

// Chat colors
 "FFE5A1",
 "E53E00",
 "7DBB00",
 "B01010",
 "FFFF00", //"4EA100",
 "FF0000", //"B00000",
 "FF3366", //"C80000",
 "FFD0AC",
 "FFA115",
 "72C100",
 "A0A0A0",
 "808080",
 "0097C1",
 "008DE5",

// Chat colors (highlight)
 "EAB171",
 "AC3E00",
 "589700",
 "EA2A2A",
 "247D00",
 "830000",
 "9C0000",
 "CFA07C",
 "DF8110",
 "429100",
 "707070",
 "505050",
 "0077A1",
 "006DCB",
 
// Attractions
 "/Chowany 5 20",
 "/SPS 1 20",
 "/Derby 5 20",
 "/Labirynt 5 20",
 "/Race 5 20",
 "/Drifting 1 10",
 "/WG 5 20",
 "/Strzelnica 5 20",

 "1", // Censorship
 "400" // Max ping
 
};

enum e_vehcolors {
	evc_id,
	evc_value,
	evc_name[24],
	bool:evc_quickSelect
}

new vehicleColors[][e_vehcolors] = {
 {0, 0x000000, "Czarny", true},
 {1, 0xFFFFFF, "Bialy", true},
 {2, 0x2975a6, "Jelly Bean", true},
 {3, 0x840510, "Red Devil", true},
 {4, 0x253739, "Outer Space", true},
 {5, 0x87446f, "Cannon Pink", false},
 {6, 0xd68f11, "Golden Bell", true},
 {7, 0x4c75b7, "Steel Blue", false},
 {8, 0xBDBDC5, "French Gray", true},
 {13, 0x58595B, "Abbey", false},
 {14, 0xD5DAD4, "Tasman", true},
 {15, 0x9CA1A4, "Gray Chateau", false},
 {16, 0x335F3E, "Killarney", true},
 {17, 0x7C0A2B, "Monarch", false},
 {20, 0x3B4E79, "East Bay", true},
 {54, 0x282f4c, "Pickled Bluewood", false},
 {79, 0x0D316D, "Madison", true},
 {92, 0x6D6D6F, "Mid Gray", false},
 {93, 0x0f6a89, "Surfie Green", true},
 {112, 0x596D86, "Blue Bayoux", false},
 {113, 0x483433, "Woody Brown", true},
 {114, 0x456250, "Mineral Green", false},
 {118, 0xa3adc6, "Cadet Blue", true},
 {126, 0xed6aae, "Brilliant Rose", false},
	// kolory 0.3c
 {130, 0xa96e44, "Salmon", true},
 {131, 0x2B946D, "Eucalyptus", false},
 {136, 0xA0296B, "Fluorescent Pink", true},
 {144, 0x0f8450, "Salem", false},
 {149, 0x500F5A, "Purple", true},
 {151, 0xAD0917, "Fluoresecent Red", false},
 {158, 0x180835, "Violet", true},
 {243, 0x080c01, "Dark Green", true},
 {252, 0x0a2b01, "Brilliant Green", false}
};

new fsLogoColors[] = {
 0x0080FFFF,
 0xC62F2FFF,
 0x1A8D1AFF,
 0xF5AC00FF,
 0xB16DEAFF
};

new Float:aLabiryntSpawns[][e_Vectors] = {
 {1544.6401, -1519.7593, 23.5469, 90.7880},
 {1547.4495, -1526.7908, 23.5469, 180.0263},
 {1547.4312, -1541.5918, 23.5469, 180.2142},
 {1547.3451, -1562.1705, 23.5469, 179.8382},
 {1535.3186, -1565.0165, 23.5469, 92.1041},
 {1521.3843, -1550.7931, 23.5469, 269.1157},
 {1513.0377, -1550.9181, 23.5469, 90.7253},
 {1477.6400, -1546.8966, 23.5469, 89.7854},
 {1469.8767, -1529.3004, 23.5469, 358.6046}, 
 {1474.4319, -1516.7545, 23.5469, 91.5168},
 {1496.5497, -1516.6305, 23.5469, 92.5426}
};

new Float:aLabiryntCheckpoints[][e_Vectors] = {
 {1527.5209, -1533.5161, 23.5469},
 {1492.1079, -1530.3351, 23.5469},
 {1514.0939, -1534.1798, 23.5469},
 {1477.6660, -1536.8463, 23.5469}
};

new telpos[e_tpLOC];
new evtp[e_tpLOC];
new privpos[MAX_PLAYERS][e_tpLOC];
new privpos2[MAX_PLAYERS][e_tpLOC];
new retpos[MAX_PLAYERS][e_tpRET];

/*new wgTeamColors[] = {
 0x399C00A0, // green
 0x006DDBA0  // blue
};*/

/*new wgRandomWeapons[][] = {
 {24, 29, 34},
 {25, 31, 34}
}*/


enum e_arenasolo {
	eas_nazwa[32],
	eas_code,
	eas_interior,
	Float:eas_ar,
	Float:eas_spawn1[4],
	Float:eas_spawn2[4]
};

new DATA_arenysolo[][e_arenasolo]={
	{ "Boxing ring",	10,	9, 0.0, { -1111.19, -1000.82,2132.82, -135.0 }, { -1106.98, -1004.98,2132.82 } },
	{ "Highscrapper", 15, 0,	99.0,	{	-220.0, -1345.0, 195.88, 280.0}, { -84.14,-1350.01,195.75, 90.0} },
	{ "Mordownia",	20, 9, 99.0, { -1025.00,-963.34, 129.26,  175.0 }, { -1025.00,-1023.34, 129.26,  356.0 } },
	{ "Rotunda", 30, 9, 99.0, { -1100.1,-1014.64, 129.52, 0.1 }, { -1100.1,-971.68, 129.52, 180.1 } },
	{ "Sky", 40, 7, 99.0, { -1100.1,-1014.64, 129.52, 0.1 }, { -1100.1,-971.68, 129.52, 180.1 } },
	{ "WH",	50, 1, 99.0, { 1413.93, 3.22, 1000.92, 135.0 }, { 1372.25, -35.94, 1000.92, 315.0 } }

};
