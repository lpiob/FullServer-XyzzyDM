CreateTextDraws()
{
	new
	 Float:tmp = 0.0;
	 
	gTextDraw[26] = TextDrawCreate(0.0, 0.0, "~n~~n~~n~~n~~n~~n~~n~");
 	TextDrawLetterSize(gTextDraw[26], 0.0, 10.0);
 	TextDrawUseBox(gTextDraw[26], 1);
 	TextDrawBoxColor(gTextDraw[26], 0xFFFFFFBB);
	 
	for(new i = 0; i < 10; i++)
	{
		gTextDraw[i] = DrawRectangle(0.0, 640.0, 0.0, 60.0 + tmp, 0x00000015);
		tmp += 1.0;
	}
	
	tmp = 0.0;
	
	for(new i = 10; i < 20; i++)
	{
		gTextDraw[i] = DrawRectangle(0.0, 640.0, 388.0 - tmp, 480.0, 0x00000015);
		tmp += 1.0;
	}
	
	gTextDraw[20] = TextDrawCreate(320.0, 14.5, "Graczy:~n~~n~0 / 0");
	TextDrawLetterSize(gTextDraw[20], 0.35, 1.4);
	TextDrawColor(gTextDraw[20], 0x000000FF);
	TextDrawFont(gTextDraw[20], 0);
	TextDrawSetShadow(gTextDraw[20], 0);
	TextDrawSetOutline(gTextDraw[20], 1);
	TextDrawBackgroundColor(gTextDraw[20], 0xFFDB2AFF);
	TextDrawAlignment(gTextDraw[20], 2);
	
	gTextDraw[21] = TextDrawCreate(320.0, 14.5, "Graczy:~n~~n~0 / 0");
	TextDrawLetterSize(gTextDraw[21], 0.35, 1.4);
	TextDrawColor(gTextDraw[21], 0x000000FF);
	TextDrawFont(gTextDraw[21], 0);
	TextDrawSetShadow(gTextDraw[21], 0);
	TextDrawSetOutline(gTextDraw[21], 1);
	TextDrawBackgroundColor(gTextDraw[21], 0xFFFFFFFF);
	TextDrawAlignment(gTextDraw[21], 2);
	
	gTextDraw[22] = TextDrawCreate(320.0, 14.5, "Graczy:~n~~n~0 / 0");
	TextDrawLetterSize(gTextDraw[22], 0.35, 1.4);
	TextDrawColor(gTextDraw[22], 0x000000FF);
	TextDrawFont(gTextDraw[22], 0);
	TextDrawSetShadow(gTextDraw[22], 0);
	TextDrawSetOutline(gTextDraw[22], 1);
	TextDrawBackgroundColor(gTextDraw[22], 0x7DC100FF);
	TextDrawAlignment(gTextDraw[22], 2);
	
	gTextDraw[23] = TextDrawCreate(320.0, 401.0, "~<~ Dom ~>~__");
	TextDrawLetterSize(gTextDraw[23], 0.85, 2.6);
	TextDrawColor(gTextDraw[23], 0x000000FF);
	TextDrawFont(gTextDraw[23], 0);
	TextDrawSetShadow(gTextDraw[23], 0);
	TextDrawSetOutline(gTextDraw[23], 1);
	TextDrawBackgroundColor(gTextDraw[23], 0xFFDB2AFF);
	TextDrawAlignment(gTextDraw[23], 2);
	
	gTextDraw[24] = TextDrawCreate(320.0, 401.0, "~<~ Baza gangu ~>~__");
	TextDrawLetterSize(gTextDraw[24], 0.85, 2.6);
	TextDrawColor(gTextDraw[24], 0x000000FF);
	TextDrawFont(gTextDraw[24], 0);
	TextDrawSetShadow(gTextDraw[24], 0);
	TextDrawSetOutline(gTextDraw[24], 1);
	TextDrawBackgroundColor(gTextDraw[24], 0xFFFFFFFF);
	TextDrawAlignment(gTextDraw[24], 2);
	
	gTextDraw[25] = TextDrawCreate(320.0, 401.0, "~<~ Full Andreas ~>~__");
	TextDrawLetterSize(gTextDraw[25], 0.85, 2.6);
	TextDrawColor(gTextDraw[25], 0x000000FF);
	TextDrawFont(gTextDraw[25], 0);
	TextDrawSetShadow(gTextDraw[25], 0);
	TextDrawSetOutline(gTextDraw[25], 1);
	TextDrawBackgroundColor(gTextDraw[25], 0x7DC100FF);
	TextDrawAlignment(gTextDraw[25], 2);
	
	gTextDraw[27] = DrawRectangle(220.0, 420.0, 220.0, 270.0, 0x00000060);
	gTextDraw[28] = DrawRectangle(221.0, 419.0, 221.5, 230.0, 0x00000070);
	
	gTextDraw[29] = TextDrawCreate(230.0, 239.0, "Uzyj strzalek ~<~ oraz ~>~ aby zmienic miasto.~n~~n~Nacisnij ENTER aby zaakceptowac.");
	TextDrawLetterSize(gTextDraw[29], 0.25, 0.9);
	TextDrawColor(gTextDraw[29], 0xFFFFB6FF);
	TextDrawFont(gTextDraw[29], 1);
	TextDrawSetShadow(gTextDraw[29], 1);
	TextDrawBackgroundColor(gTextDraw[29], 0x00000050);
	TextDrawAlignment(gTextDraw[29], 1);
	
	gTextDraw[30] = TextDrawCreate(226.0, 239.0, "Use arrows ~<~ and ~>~ to switch between cities.~n~~n~Press ENTER to accept.");
	TextDrawLetterSize(gTextDraw[30], 0.25, 0.9);
	TextDrawColor(gTextDraw[30], 0xFFFFB6FF);
	TextDrawFont(gTextDraw[30], 1);
	TextDrawSetShadow(gTextDraw[30], 1);
	TextDrawBackgroundColor(gTextDraw[30], 0x00000050);
	TextDrawAlignment(gTextDraw[30], 1);
	
	gTextDraw[31] = TextDrawCreate(224.0, 221.0, "Podpowiedz");
	TextDrawLetterSize(gTextDraw[31], 0.22, 0.9);
	TextDrawColor(gTextDraw[31], 0xDDDDDDFF);
	TextDrawFont(gTextDraw[31], 2);
	TextDrawSetShadow(gTextDraw[31], 0);
	TextDrawAlignment(gTextDraw[31], 1);
	
	gTextDraw[32] = TextDrawCreate(224.0, 221.0, "Hint");
	TextDrawLetterSize(gTextDraw[32], 0.22, 0.9);
	TextDrawColor(gTextDraw[32], 0xDDDDDDFF);
	TextDrawFont(gTextDraw[32], 2);
	TextDrawSetShadow(gTextDraw[32], 0);
	TextDrawAlignment(gTextDraw[32], 1);
	// 73,74,75 - fullserver, wersja, tekst reklamowy - widoczny w wybieralce skinow

	gTextDraw[73] = TextDrawCreate(186.000000, 178.000000, "XyzzyDM");
	TextDrawAlignment(gTextDraw[73], 3);
	TextDrawBackgroundColor(gTextDraw[73], 799);
	TextDrawFont(gTextDraw[73], 0);
	TextDrawLetterSize(gTextDraw[73], 1.149999, 3.099998);
	TextDrawColor(gTextDraw[73], -1);
	TextDrawSetOutline(gTextDraw[73], 1);
	TextDrawSetProportional(gTextDraw[73], 1);

	gTextDraw[74] = TextDrawCreate(187.000000, 208.000000, "v" #GMVERSION);
	TextDrawAlignment(gTextDraw[74], 3);
	TextDrawBackgroundColor(gTextDraw[74], 255);
	TextDrawFont(gTextDraw[74], 1);
	TextDrawLetterSize(gTextDraw[74], 0.500000, 1.000000);
	TextDrawColor(gTextDraw[74], 808464432);
	TextDrawSetOutline(gTextDraw[74], 0);
	TextDrawSetProportional(gTextDraw[74], 1);
	TextDrawSetShadow(gTextDraw[74], 0);

	gTextDraw[75] = TextDrawCreate(114.000000, 234.000000, "SA:MP");
	TextDrawAlignment(gTextDraw[75], 2);
	TextDrawBackgroundColor(gTextDraw[75], 255);
	TextDrawFont(gTextDraw[75], 1);
	TextDrawLetterSize(gTextDraw[75], 0.370000, 1.399999);
	TextDrawColor(gTextDraw[75], -1);
	TextDrawSetOutline(gTextDraw[75], 0);
	TextDrawSetProportional(gTextDraw[75], 1);
	TextDrawSetShadow(gTextDraw[75], 1);
	TextDrawUseBox(gTextDraw[75], 1);
	TextDrawBoxColor(gTextDraw[75], 0);
	TextDrawTextSize(gTextDraw[75], 85.000000, 146.000000);

	
#if defined GAMETIMESYNCTOREAL
	gTextDraw[TD_TIME]=TextDrawCreate(579.000000, 22.000000, "08:17");
	TextDrawAlignment(gTextDraw[TD_TIME], 2);
	TextDrawBackgroundColor(gTextDraw[TD_TIME], 255);
	TextDrawFont(gTextDraw[TD_TIME], 3);
	TextDrawLetterSize(gTextDraw[TD_TIME], 0.619999, 2.100000);
	TextDrawColor(gTextDraw[TD_TIME], -1);
	TextDrawSetOutline(gTextDraw[TD_TIME], 1);
	TextDrawSetProportional(gTextDraw[TD_TIME], 0);
#endif

//	gTextDraw[TD_DATETIME] = TextDrawCreate(635.0, 436.0, "_");//50 graczy (~y~5/~b~~h~2~w~/~r~4~w~) ~b~03 luty");	// aqq
	gTextDraw[TD_DATETIME] = TextDrawCreate(635.0, 431.0, "_");//50 graczy (~y~5/~b~~h~2~w~/~r~4~w~) ~b~03 luty");	// aqq
	TextDrawAlignment(gTextDraw[TD_DATETIME],3);
//	TextDrawLetterSize(gTextDraw[TD_DATETIME], 0.24, 1.1);	bylo
	TextDrawLetterSize(gTextDraw[TD_DATETIME], 0.22, 1.3);
	TextDrawColor(gTextDraw[TD_DATETIME], -1);
	TextDrawFont(gTextDraw[TD_DATETIME], 1);
	TextDrawSetOutline(gTextDraw[TD_DATETIME], 1);
	TextDrawSetProportional(gTextDraw[TD_DATETIME], 1);
	TextDrawBackgroundColor(gTextDraw[TD_DATETIME], 96);

	gTextDraw[TD_VOTING] = TextDrawCreate(300.000000, 298.000000, " ");//Lorem ipsum dolor sit amest non habius pas erpa. Lorem ipsum~n~/TAK   |   /NIE");
	TextDrawAlignment(gTextDraw[TD_VOTING], 3);
	TextDrawBackgroundColor(gTextDraw[TD_VOTING], 255); //52364063);
	TextDrawFont(gTextDraw[TD_VOTING], 1);
	TextDrawLetterSize(gTextDraw[TD_VOTING], 0.309998, 1.100000);// 0.399998, 1.299998);
	TextDrawColor(gTextDraw[TD_VOTING], -1);
	TextDrawSetOutline(gTextDraw[TD_VOTING], 1);
	TextDrawSetProportional(gTextDraw[TD_VOTING], 1);
	TextDrawUseBox(gTextDraw[TD_VOTING], 1);
	TextDrawBoxColor(gTextDraw[TD_VOTING], 64);//-855664316);//1408581440);
	TextDrawTextSize(gTextDraw[TD_VOTING], 0.0,0.0);//90.000000, 0.000000);
	
	gTextDraw[TD_ANNOUNCE] = TextDrawCreate(320.0, 140.0, " ");	// bylo 160 - przy dwoch liniach zaslanialo celownik i wkurwialo na WG
	TextDrawLetterSize(gTextDraw[TD_ANNOUNCE], 0.45, 1.6);
	TextDrawColor(gTextDraw[TD_ANNOUNCE], 0xFFFFD0FF);
	TextDrawFont(gTextDraw[TD_ANNOUNCE], 1);
	TextDrawSetShadow(gTextDraw[TD_ANNOUNCE], 0);
	TextDrawSetOutline(gTextDraw[TD_ANNOUNCE], 1);
	TextDrawBackgroundColor(gTextDraw[TD_ANNOUNCE], 0x00000070);
	TextDrawAlignment(gTextDraw[TD_ANNOUNCE], 2);
	TextDrawTextSize(gTextDraw[TD_ANNOUNCE], 400.0, 400.0);

	gTextDraw[TD_ANN2] = TextDrawCreate(488.000000, 270.000000, "_"); // lorem ipsum dolor sit amest lorem ipsum");
	TextDrawAlignment(gTextDraw[TD_ANN2], 2);
	TextDrawBackgroundColor(gTextDraw[TD_ANN2], 144);
	TextDrawFont(gTextDraw[TD_ANN2], 1);
	TextDrawLetterSize(gTextDraw[TD_ANN2], 0.300000, 1.300000);
	TextDrawColor(gTextDraw[TD_ANN2], -1);
	TextDrawSetOutline(gTextDraw[TD_ANN2], 0);
	TextDrawSetProportional(gTextDraw[TD_ANN2], 1);
	TextDrawSetShadow(gTextDraw[TD_ANN2], 1);
	TextDrawUseBox(gTextDraw[TD_ANN2], 1);
	TextDrawBoxColor(gTextDraw[TD_ANN2], 0);
	TextDrawTextSize(gTextDraw[TD_ANN2], 0.000000, 298.000000);

	for(new i = TD_STARS_START; i < TD_STARS_END; i++)
	{
		switch(i - TD_STARS_START + 1)
		{
			case 1, 7,  13, 19, 25:	gTextDraw[i] = TextDrawCreate(500.0, 103.0, "]");
			case 2, 8,  14, 20, 26:	gTextDraw[i] = TextDrawCreate(500.0, 103.0, "]]");
			case 3, 9,  15, 21, 27:	gTextDraw[i] = TextDrawCreate(500.0, 103.0, "]]]");
			case 4, 10, 16, 22, 28:	gTextDraw[i] = TextDrawCreate(500.0, 103.0, "]]]]");
			case 5, 11, 17, 23, 29:	gTextDraw[i] = TextDrawCreate(500.0, 103.0, "]]]]]");
			case 6, 12, 18, 24, 30:	gTextDraw[i] = TextDrawCreate(500.0, 103.0, "]]]]]]");
		}
		
		switch(i - TD_STARS_START + 1)
		{
			case 1..6: 		TextDrawColor(gTextDraw[i], 0xFA7800FF); // pomarañczowe
			case 7..12: 	TextDrawColor(gTextDraw[i], 0xFFEF00FF); // ¿ó³te
			case 13..18: 	TextDrawColor(gTextDraw[i], 0x00C100FF); // zielone
			case 19..24: 	TextDrawColor(gTextDraw[i], 0x0060FAFF); // niebieskie
			case 25..30: 	TextDrawColor(gTextDraw[i], 0xB50000FF); // czerwone
		}
		
		TextDrawLetterSize(gTextDraw[i], 0.535, 2.3);
		TextDrawFont(gTextDraw[i], 2);
		TextDrawSetShadow(gTextDraw[i], 0);
		TextDrawSetOutline(gTextDraw[i], 1);
		TextDrawBackgroundColor(gTextDraw[i], 0x00000099);
		TextDrawAlignment(gTextDraw[i], 1);
	}
	
	gTextDraw[TD_ATTRACTIONBOX] = DrawRectangle(170.0, 300.0, 345.0, 420.0, 0x00000030);	// bylo 415

	gTextDraw[TD_ARACEBOX] = TextDrawCreate(428.000000, 304.000000, " ");//~n~~w~Tierra Robada    ~y~25km~n~~n~~r~1. [FS]Ewelina_~n~~h~2. XJL~n~~h~3. WUBE~n~~h~4. WidzO~n~~h~5. _RSAH_DavO_");
	TextDrawBackgroundColor(gTextDraw[TD_ARACEBOX], 255);
	TextDrawFont(gTextDraw[TD_ARACEBOX], 1);
	TextDrawLetterSize(gTextDraw[TD_ARACEBOX], 0.270000, 1.000000);
	TextDrawColor(gTextDraw[TD_ARACEBOX], 65535);
	TextDrawSetOutline(gTextDraw[TD_ARACEBOX], 0);
	TextDrawSetProportional(gTextDraw[TD_ARACEBOX], 1);
	TextDrawSetShadow(gTextDraw[TD_ARACEBOX], 1);
	TextDrawUseBox(gTextDraw[TD_ARACEBOX], 1);
	TextDrawBoxColor(gTextDraw[TD_ARACEBOX], 96);
	TextDrawTextSize(gTextDraw[TD_ARACEBOX], 640.000000, 0.000000);


	gTextDraw[TD_ADRIFTBOX] = TextDrawCreate(428.000000, 304.000000, "_");	// textdraw atrakcji na drifcie
	TextDrawBackgroundColor(gTextDraw[TD_ADRIFTBOX], 255);
	TextDrawFont(gTextDraw[TD_ADRIFTBOX], 1);
	TextDrawLetterSize(gTextDraw[TD_ADRIFTBOX], 0.270000, 1.000000);
	TextDrawColor(gTextDraw[TD_ADRIFTBOX], 65535);
	TextDrawSetOutline(gTextDraw[TD_ADRIFTBOX], 0);
	TextDrawSetProportional(gTextDraw[TD_ADRIFTBOX], 1);
	TextDrawSetShadow(gTextDraw[TD_ADRIFTBOX], 0);
	TextDrawUseBox(gTextDraw[TD_ADRIFTBOX], 1);
	TextDrawBoxColor(gTextDraw[TD_ADRIFTBOX], 96);
	TextDrawTextSize(gTextDraw[TD_ADRIFTBOX], 640.000000, 0.000000);


	gTextDraw[TD_AWGINFO] = TextDrawCreate(535.000000, 330.000000, "~g~-   ~w~vs   ~b~-");
	TextDrawAlignment(gTextDraw[TD_AWGINFO], 2);
	TextDrawBackgroundColor(gTextDraw[TD_AWGINFO], -791621584);
	TextDrawFont(gTextDraw[TD_AWGINFO], 1);
	TextDrawLetterSize(gTextDraw[TD_AWGINFO], 0.759998, 2.500000);
	TextDrawColor(gTextDraw[TD_AWGINFO], -1);
	TextDrawSetOutline(gTextDraw[TD_AWGINFO], 1);
	TextDrawSetProportional(gTextDraw[TD_AWGINFO], 0);

	gTextDraw[TD_AWGINFOHP] = TextDrawCreate(535.000000, 361.000000, "~g~-HP               ~r~-HP");
	TextDrawAlignment(gTextDraw[TD_AWGINFOHP], 2);
	TextDrawBackgroundColor(gTextDraw[TD_AWGINFOHP], 255);
	TextDrawFont(gTextDraw[TD_AWGINFOHP], 1);
	TextDrawLetterSize(gTextDraw[TD_AWGINFOHP], 0.340000, 1.399999);
	TextDrawColor(gTextDraw[TD_AWGINFOHP], -1);
	TextDrawSetOutline(gTextDraw[TD_AWGINFOHP], 0);
	TextDrawSetProportional(gTextDraw[TD_AWGINFOHP], 1);
	TextDrawSetShadow(gTextDraw[TD_AWGINFOHP], 1);


/*	gTextDraw[TD_AWGINFO] = TextDrawCreate(535.000000, 340.000000, "~g~-   ~w~vs   ~b~-");
	TextDrawAlignment(gTextDraw[TD_AWGINFO], 2);
	TextDrawBackgroundColor(gTextDraw[TD_AWGINFO], -791621584);
	TextDrawFont(gTextDraw[TD_AWGINFO], 1);
	TextDrawLetterSize(gTextDraw[TD_AWGINFO], 0.770000, 2.600000);
	TextDrawColor(gTextDraw[TD_AWGINFO], -1);
	TextDrawSetOutline(gTextDraw[TD_AWGINFO], 1);
	TextDrawSetProportional(gTextDraw[TD_AWGINFO], 0);
	TextDrawShowForAll(gTextDraw[TD_AWGINFO]);*/


	tmp = 0.0;
	
	for(new i = TD_INFOBOX; i < TD_INFOBOX + 8; i++)
	{
		gTextDraw[i] = DrawRectangle(470.0 + tmp, 600.0 - tmp, 345.0 + tmp, 415.0 - tmp, 0x0000000A);
		tmp += 0.5;
	}
	
	gTextDraw[TD_ATTRACTION_TITLES] = TextDrawCreate(174.0, 347.0, " ");
	TextDrawLetterSize(gTextDraw[TD_ATTRACTION_TITLES], 0.20, 0.79);
	TextDrawColor(gTextDraw[TD_ATTRACTION_TITLES], 0x00FF20FF);
	TextDrawFont(gTextDraw[TD_ATTRACTION_TITLES], 2);
	TextDrawSetShadow(gTextDraw[TD_ATTRACTION_TITLES], 1);
	TextDrawSetOutline(gTextDraw[TD_ATTRACTION_TITLES], 0);
	TextDrawBackgroundColor(gTextDraw[TD_ATTRACTION_TITLES], 0x00000080);
	TextDrawAlignment(gTextDraw[TD_ATTRACTION_TITLES], 1);
	
	gTextDraw[TD_ATTRACTION_STATES] = TextDrawCreate(296.0, 347.0, " ");
	TextDrawLetterSize(gTextDraw[TD_ATTRACTION_STATES], 0.20, 0.79);
	TextDrawColor(gTextDraw[TD_ATTRACTION_STATES], 0x00FF20FF);
	TextDrawFont(gTextDraw[TD_ATTRACTION_STATES], 2);
	TextDrawSetShadow(gTextDraw[TD_ATTRACTION_STATES], 1);
	TextDrawSetOutline(gTextDraw[TD_ATTRACTION_STATES], 0);
	TextDrawBackgroundColor(gTextDraw[TD_ATTRACTION_STATES], 0x00000080);
	TextDrawAlignment(gTextDraw[TD_ATTRACTION_STATES], 3);
	
	tmp = 0.0;
	
	for(new i = TD_WELCOMEBOX; i < TD_WELCOMEBOX + 8; i++)
	{
//		gTextDraw[i] = DrawRectangle(20.0 + tmp, 180.0 - tmp, 150.0 + tmp, 300.0 - tmp, 0x0000000C);
		gTextDraw[i] = DrawRectangle(170.0 + tmp, 470.0 - tmp, 150.0 + tmp, 300.0 - tmp, 0x0000000C);
		tmp += 0.5;
	}
	
	gTextDraw[TD_WELCOMEBOX2] = DrawRectangle(175.0, 465.0, 155.0, 165.0, 0x00000036);
	
	gTextDraw[TD_WELCOMETITLE] = TextDrawCreate(320.0, 153.5, "Witamy");
	TextDrawLetterSize(gTextDraw[TD_WELCOMETITLE], 0.38, 1.25);
	TextDrawColor(gTextDraw[TD_WELCOMETITLE], 0x009CD0FF);
	TextDrawFont(gTextDraw[TD_WELCOMETITLE], 1);
	TextDrawSetShadow(gTextDraw[TD_WELCOMETITLE], 1);
	TextDrawSetOutline(gTextDraw[TD_WELCOMETITLE], 0);
	TextDrawBackgroundColor(gTextDraw[TD_WELCOMETITLE], 0x00000060);
	TextDrawAlignment(gTextDraw[TD_WELCOMETITLE], 2);
	
	gTextDraw[TD_WELCOMETEXT] = TextDrawCreate(176.0, 188.0, " ");
	TextDrawLetterSize(gTextDraw[TD_WELCOMETEXT], 0.32, 1.0);
	TextDrawColor(gTextDraw[TD_WELCOMETEXT], 0xFFFFFFFF);
	TextDrawFont(gTextDraw[TD_WELCOMETEXT], 1);
	TextDrawSetShadow(gTextDraw[TD_WELCOMETEXT], 1);
	TextDrawSetOutline(gTextDraw[TD_WELCOMETEXT], 0);
	TextDrawBackgroundColor(gTextDraw[TD_WELCOMETEXT], 0x00000060);
	TextDrawAlignment(gTextDraw[TD_WELCOMETEXT], 1);
	TextDrawTextSize(gTextDraw[TD_WELCOMETEXT], 464.0, 294.0);
	

	gmTemp[fsaCurrentColor]=fsLogoColors[0];
	gTextDraw[TD_FULLSERVER] = TextDrawCreate(59.0, 420.0, "XyzzyDM");
	TextDrawLetterSize(gTextDraw[TD_FULLSERVER], 0.69999, 1.80);
	TextDrawColor(gTextDraw[TD_FULLSERVER], gmTemp[fsaCurrentColor]);
	TextDrawFont(gTextDraw[TD_FULLSERVER], 0);
	TextDrawSetShadow(gTextDraw[TD_FULLSERVER], 1);
	TextDrawSetOutline(gTextDraw[TD_FULLSERVER], 0);
	TextDrawBackgroundColor(gTextDraw[TD_FULLSERVER], 0x000000FF);
	TextDrawAlignment(gTextDraw[TD_FULLSERVER], 2);
	
	
//	gTextDraw[TD_STAT] = TextDrawCreate(126.0, 429.0, " Nick (ID):                   Czas gry: Ping:     Respekt:  Skill:");
//	gTextDraw[TD_STAT] = TextDrawCreate(126.0, 429.0, "                              Czas gry: Ping:     Respekt:  Skill:");
	gTextDraw[TD_STAT] = TextDrawCreate(275.0, 429.0, "Czas gry: Ping:     Respekt:  Skill:");
	TextDrawLetterSize(gTextDraw[TD_STAT], 0.24, 0.8);
	TextDrawColor(gTextDraw[TD_STAT], -112);
	TextDrawFont(gTextDraw[TD_STAT], 1);
	TextDrawSetOutline(gTextDraw[TD_STAT], 1);
	TextDrawSetProportional(gTextDraw[TD_STAT], 0);
	TextDrawBackgroundColor(gTextDraw[TD_STAT], 96);
	

	//	TD_AREAFULLDM, TD_AREANODM
	gTextDraw[TD_AREAFULLDM]=TextDrawCreate(554.000000, 5.000000, "STREFA ~r~SMIERCI");
	TextDrawAlignment(gTextDraw[TD_AREAFULLDM], 2);
	TextDrawBackgroundColor(gTextDraw[TD_AREAFULLDM], 255);
	TextDrawFont(gTextDraw[TD_AREAFULLDM], 3);
	TextDrawLetterSize(gTextDraw[TD_AREAFULLDM], 0.399998, 1.299998);
	TextDrawColor(gTextDraw[TD_AREAFULLDM], 0xffffffff);
	TextDrawSetOutline(gTextDraw[TD_AREAFULLDM], 1);
	TextDrawSetProportional(gTextDraw[TD_AREAFULLDM], 1);

	gTextDraw[TD_AREANODM]=TextDrawCreate(554.000000, 5.000000, "STREFA ~b~~h~BEZ DM");
	TextDrawAlignment(gTextDraw[TD_AREANODM], 2);
	TextDrawBackgroundColor(gTextDraw[TD_AREANODM], 255);
	TextDrawFont(gTextDraw[TD_AREANODM], 3);
	TextDrawLetterSize(gTextDraw[TD_AREANODM], 0.399998, 1.299998);
	TextDrawColor(gTextDraw[TD_AREANODM], 0xffffffff);
	TextDrawSetOutline(gTextDraw[TD_AREANODM], 1);
	TextDrawSetProportional(gTextDraw[TD_AREANODM], 1);

	gTextDraw[TD_WARNING] = TextDrawCreate(320.000000, 171.000000, "~n~Otrzymano ostrzezenie od administratora!~n~~n~You have received a warning!~n~~n~");
	TextDrawAlignment(gTextDraw[TD_WARNING], 2);
	TextDrawBackgroundColor(gTextDraw[TD_WARNING], 255);
	TextDrawFont(gTextDraw[TD_WARNING], 1);
	TextDrawLetterSize(gTextDraw[TD_WARNING], 0.559999, 2.299999);
	TextDrawColor(gTextDraw[TD_WARNING], -1);
	TextDrawSetOutline(gTextDraw[TD_WARNING], 0);
	TextDrawSetProportional(gTextDraw[TD_WARNING], 1);
	TextDrawSetShadow(gTextDraw[TD_WARNING], 2);
	TextDrawUseBox(gTextDraw[TD_WARNING], 1);
	TextDrawBoxColor(gTextDraw[TD_WARNING], -805306272);
	TextDrawTextSize(gTextDraw[TD_WARNING], 50.000000, 410.000000);
	

	
	//  --- PER-PLAYER TEXTDRAWS ---
	
	for(new i = 0; i < MAX_SERVER_PLAYERS; i++)
	{
		pTextDraw[PTD_NICK][i] = TextDrawCreate(195.000000, 431.000000, "_");
		TextDrawAlignment(pTextDraw[PTD_NICK][i], 2);
		TextDrawBackgroundColor(pTextDraw[PTD_NICK][i], 96);
		TextDrawFont(pTextDraw[PTD_NICK][i], 1);
		TextDrawLetterSize(pTextDraw[PTD_NICK][i], 0.260000, 1.300000);
		TextDrawColor(pTextDraw[PTD_NICK][i], -1);
		TextDrawSetOutline(pTextDraw[PTD_NICK][i], 1);
		TextDrawSetProportional(pTextDraw[PTD_NICK][i], 1);
	
		pTextDraw[PTD_STAT][i] = TextDrawCreate(276.0, 436.0, "_ ");
		TextDrawLetterSize(pTextDraw[PTD_STAT][i], 0.24, 1.1);
		TextDrawBackgroundColor(pTextDraw[PTD_STAT][i], 96);
		TextDrawColor(pTextDraw[PTD_STAT][i], -1);
		TextDrawFont(pTextDraw[PTD_STAT][i], 1);
		TextDrawSetProportional(pTextDraw[PTD_STAT][i], 0);
		TextDrawUseBox(pTextDraw[PTD_STAT][i], 0);
		TextDrawSetOutline(pTextDraw[PTD_STAT][i], 1);
//		TextDrawBoxColor(pTextDraw[PTD_STAT][i], 48);
		TextDrawTextSize(pTextDraw[PTD_STAT][i],640.0,0.0);

		pTextDraw[PTD_VEHICLEINFO][i] = TextDrawCreate(532.000000, 386.000000, " "); //~r~~h~Helikopter policyjny~n~~b~~h~~h~~h~200 ~w~km/h    ~y~]]]~l~]]");
		TextDrawAlignment(pTextDraw[PTD_VEHICLEINFO][i], 2);
		TextDrawBackgroundColor(pTextDraw[PTD_VEHICLEINFO][i], 255);
		TextDrawFont(pTextDraw[PTD_VEHICLEINFO][i], 2);
		TextDrawLetterSize(pTextDraw[PTD_VEHICLEINFO][i], 0.360000, 1.600000);
		TextDrawColor(pTextDraw[PTD_VEHICLEINFO][i], -1);
		TextDrawSetOutline(pTextDraw[PTD_VEHICLEINFO][i], 0);
		TextDrawSetProportional(pTextDraw[PTD_VEHICLEINFO][i], 1);
		TextDrawSetShadow(pTextDraw[PTD_VEHICLEINFO][i], 1);
		TextDrawUseBox(pTextDraw[PTD_VEHICLEINFO][i], 1);
		TextDrawBoxColor(pTextDraw[PTD_VEHICLEINFO][i], 3145776);
		TextDrawTextSize(pTextDraw[PTD_VEHICLEINFO][i], 394.000000, 209.000000);
		
		pTextDraw[PTD_INFOBOX][i] = TextDrawCreate(534.5, 352.0, " ");
		TextDrawLetterSize(pTextDraw[PTD_INFOBOX][i], 0.30, 1.20);
		TextDrawColor(pTextDraw[PTD_INFOBOX][i], 0xFFFFA5FF);
		TextDrawFont(pTextDraw[PTD_INFOBOX][i], 1);
		TextDrawSetShadow(pTextDraw[PTD_INFOBOX][i], 1);
		TextDrawSetOutline(pTextDraw[PTD_INFOBOX][i], 0);
		TextDrawBackgroundColor(pTextDraw[PTD_INFOBOX][i], 0x00000080);
		TextDrawAlignment(pTextDraw[PTD_INFOBOX][i], 2);

		pTextDraw[PTD_FPS][i] = TextDrawCreate(620.000000, 411.000000, "--");
		TextDrawAlignment(pTextDraw[PTD_FPS][i], 2);
		TextDrawBackgroundColor(pTextDraw[PTD_FPS][i], 144);
		TextDrawFont(pTextDraw[PTD_FPS][i], 3);
		TextDrawLetterSize(pTextDraw[PTD_FPS][i], 0.649999, 2.499999);
		TextDrawColor(pTextDraw[PTD_FPS][i], -65281);
		TextDrawSetOutline(pTextDraw[PTD_FPS][i], 1);
		TextDrawSetProportional(pTextDraw[PTD_FPS][i], 1);


	}

	// attraction textdraws

	for(new i = 0; i < MAX_QUEUE_RACE; i++){
		pTextDraw[PTD_ARACEBOX][i] = TextDrawCreate(536.000000, 288.000000, "~b~3/10    ~h~5/18");
		TextDrawAlignment(pTextDraw[PTD_ARACEBOX][i], 2);
		TextDrawBackgroundColor(pTextDraw[PTD_ARACEBOX][i], 48);
		TextDrawFont(pTextDraw[PTD_ARACEBOX][i], 3);
		TextDrawLetterSize(pTextDraw[PTD_ARACEBOX][i], 0.689999, 2.400000);
		TextDrawColor(pTextDraw[PTD_ARACEBOX][i], -1);
		TextDrawSetOutline(pTextDraw[PTD_ARACEBOX][i], 1);
		TextDrawSetProportional(pTextDraw[PTD_ARACEBOX][i], 0);
	}

	for(new i = 0; i < MAX_QUEUE_DRIFT; i++){
		pTextDraw[PTD_ADRIFTBOX][i] = TextDrawCreate(536.000000, 288.000000, "~b~3/10    ~h~5/18");
		TextDrawAlignment(pTextDraw[PTD_ADRIFTBOX][i], 2);
		TextDrawBackgroundColor(pTextDraw[PTD_ADRIFTBOX][i], 48);
		TextDrawFont(pTextDraw[PTD_ADRIFTBOX][i], 3);
		TextDrawLetterSize(pTextDraw[PTD_ADRIFTBOX][i], 0.689999, 2.400000);
		TextDrawColor(pTextDraw[PTD_ADRIFTBOX][i], -1);
		TextDrawSetOutline(pTextDraw[PTD_ADRIFTBOX][i], 1);
		TextDrawSetProportional(pTextDraw[PTD_ADRIFTBOX][i], 0);
	}




}