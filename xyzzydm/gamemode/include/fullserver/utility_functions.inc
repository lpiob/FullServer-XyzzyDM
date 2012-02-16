

forward TeleportUnfreeze(playerid);
forward TeleportVerify(playerid,Float:vectorZ);

vehicles_GetSpawnOffset(vid,&Float:vx,&Float:vy,&Float:vz) {
	vehicles_GetSpawnOffsetByModel(GetVehicleModel(vid),vx,vy,vz);
	return;
}
vehicles_GetSpawnOffsetByModel(model,&Float:vx,&Float:vy,&Float:vz) {
	switch (model) {
		case 432:		// rhino
			vz+=2;
		case 525:			// towtruck
			vz+=1;
		case 403,514,515:			// linerunner, petrol, roadtrain
			vz+=1;
		case 506:				// dumper
			vz+=0.5;
		case 443:			// laweta
			vz+=0.5;
		case 532:			// kombajn
			vz+=1.5;
		case 531:
			vz+=0.5;
		case 417,425,553,577,592,548:			// nevada i inne samoloty
			vz+=5;
	}
	return;
}


// quake's fast inverse sqrt
Float:InvSqrt(Float:value)
{
    new i, Float:x, Float:y;
    x = value * 0.5;
    y = value;
    i = _:y;
    i = 0x5f3759df - (i >> 1);
    y = Float:i;
    y = y * (1.5 - (x * y * y));
    //y = y * (1.5 - (x * y * y));
    return y;
}

#define sqrt(%0) (1.0 / InvSqrt((%0)))

bool:PlaySound(playerid, soundid, bool:pVectorInc = false)
{
	if(!IsPlayerConnected(playerid)) return false;
	
	new
	 pVector[e_Vectors];
	 
	if(pVectorInc)
	{
		GetPlayerPosition(playerid, pVector[X], pVector[Y], pVector[Z]);
	}
	
	PlayerPlaySound(playerid, soundid, pVector[X], pVector[Y], pVector[Z]);
	
	return true;
}

Msg(playerid, colorid, text[], bool:playSound = true, bool:displayPrefix = true)
{
	new
	 buffer[256],
	 curColor,
	 tmpColor[16],
	 tmpColorCode;
	
	switch(colorid)
	{
		case COLOR_INFO:
		{
			curColor = gmData[color_chatInfo];
			tmpColorCode = gmData[color_chatInfo_HL];
		}
		case COLOR_INFO2: 
		{
			curColor = gmData[color_chatInfo2];
			tmpColorCode = gmData[color_chatInfo2_HL];
		}
		case COLOR_INFO3: 
		{
			curColor = gmData[color_chatInfo3];
			tmpColorCode = gmData[color_chatInfo3_HL];
		}
		case COLOR_ERROR: 
		{
			curColor = gmData[color_chatError];
			tmpColorCode = gmData[color_chatError_HL];
		}
		case COLOR_JOININFO: 
		{
			curColor = gmData[color_joinInfo];
			tmpColorCode = gmData[color_joinInfo_HL];
		}
		case COLOR_LEAVEINFO: 
		{
			curColor = gmData[color_leaveInfo];
			tmpColorCode = gmData[color_leaveInfo_HL];
		}
		case COLOR_ADMIN: 
		{
			curColor = gmData[color_chatAdmin];
			tmpColorCode = gmData[color_chatAdmin_HL];
		}
		case COLOR_ADMIN3: 
		{
			curColor = gmData[color_chatAdmin3];
			tmpColorCode = gmData[color_chatAdmin3_HL];
		}
		case COLOR_PRIVATEMSG: 
		{
			curColor = gmData[color_chatPM];
			tmpColorCode = gmData[color_chatPM_HL];
		}
		case COLOR_IC: 
		{
			curColor = gmData[color_chatIC];
			tmpColorCode = gmData[color_chatIC_HL];
		}
		case COLOR_ME: 
		{
			curColor = gmData[color_chatME];
			tmpColorCode = gmData[color_chatME_HL];
		}
		case COLOR_GM: 
		{
			curColor = gmData[color_chatGM];
			tmpColorCode = gmData[color_chatGM_HL];
		}
		case COLOR_VIPSAY: 
		{
			curColor = gmData[color_vipSay];
			tmpColorCode = gmData[color_vipSay_HL];
		}
		case COLOR_VIP: 
		{
			curColor = gmData[color_chatVip];
			tmpColorCode = gmData[color_chatVip_HL];
		}
		
		default: curColor = colorid;
	}
	
	copy(text, buffer);
	
	format(tmpColor, sizeof tmpColor, "{%06x}", tmpColorCode);
//	regex_replace(text, "\\{b\\}", tmpColor, buffer, sizeof buffer);
	buffer = StringReplace("{b}", tmpColor, buffer);
	
	format(tmpColor, sizeof tmpColor, "{%06x}", curColor);
//	regex_replace(buffer, "\\{/b\\}", tmpColor, buffer2, sizeof buffer2);
	buffer = StringReplace("{/b}", tmpColor, buffer);
	
	if(displayPrefix) 
		strins(buffer, "››› ", 0);
	
	SendClientMessage(playerid, curColor * 256, buffer);
	
	if(playSound)
	{
		if(colorid == COLOR_ERROR)
		{
			PlaySound(playerid, 1085);
		}
		else
		{
			PlaySound(playerid, 1150);
		}
	}
	
	return 1;
}

/*COLOR(colorid)
{
	switch(colorid)
	{
		case COLOR_INFO: return gmData[color_chatInfo] * 256;
		case COLOR_INFO2: return gmData[color_chatInfo2] * 256;
		case COLOR_INFO3: return gmData[color_chatInfo3] * 256;
		case COLOR_ERROR: return gmData[color_chatError] * 256;
		case COLOR_JOININFO: return gmData[color_joinInfo] * 256;
		case COLOR_LEAVEINFO: return gmData[color_leaveInfo] * 256;
		case COLOR_ADMIN: return gmData[color_chatAdmin] * 256;
		case COLOR_ADMIN3: return gmData[color_chatAdmin3] * 256;
		case COLOR_PRIVATEMSG: return gmData[color_chatPM] * 256;
		case COLOR_IC: return gmData[color_chatIC] * 256;
		case COLOR_ME: return gmData[color_chatME] * 256;
		case COLOR_GM: return gmData[color_chatGM] * 256;
	}
	
	return 0xFFFFFFFF;
}*/

bool:IsNumeric(const string[])
{
	new
	 sign;
	 
	if(string[0] > '9' || string[0] < '0' || string[strlen(string) - 1] > '9' || string[strlen(string) - 1] < '0') return false;
	
	for(new i = 1; i < strlen(string); i++)
	{
		if(string[i] >= '0' && string[i] <= '9') sign = 0;
		else return false;
		if(sign > 1) return false;
	}
	
	return true;
}

FindString(string1[], string2[], &start = 0, &end = 0)
{
	if(!strlen(string1) || !strlen(string2)) return false;
	
	new
	 result;
	
	for(new i = 0; i < strlen(string1) - strlen(string2) + 1; i++)
	{
	    result = 0;
		
	    for(new g = 0; g < strlen(string2); g++)
	    {
	        if(toupper(string1[i + g]) == toupper(string2[g])) result++;
	    }
		
	    if(result == strlen(string2))
		{
			start = i;
			end = i + strlen(string2);
			
			return true;
		}
	}
	
	return false;
}

HexToInt(string[])
{
    if(string[0] == 0) return 0;

    new
     cur = 1,
     res = 0;
	 
    for(new i = strlen(string); i > 0; i--)
    {
        if(string[i - 1] < 58)
        {
            res = res + cur * (string[i - 1] - 48);
        }
        else
        {
            res = res + cur * (string[i - 1] - 65 + 10);
        }
		cur = cur * 16;
    }
	
    return res;
}

IntToHex(int)
{
	new
	 buffer[8],
	 i = 0,
	 result[8],
	 g = 0;
	
	while(int > 0)
	{
		new tmp = int % 16;
		
		if(tmp < 10) 	buffer[i] = tmp + 48;
		else 			buffer[i] = tmp + 55;
		
		i++;
		
		int = int / 16;
	}
	
	for(i = strlen(buffer) - 1; i >= 0; i--)
	{
		result[g++] = buffer[i];
	}
	
	while(strlen(result) < 6)
	{
		strins(result, "0", 0);
	}
	
	return result;
}

bool:IsFS(playerid)						// nieoptymalne! kod do tego odwoluje sie setki razy na sekunde! powinno byc w tablicy. TODO
{
	if (pData[playerid][accountID]==0) return false;
	new
	 szName[24];
	 
	GetPlayerName(playerid, szName, sizeof szName);
	
	if(szName[0] == '[' && szName[1] == 'F' && szName[2] == 'S' && szName[3] == ']')
	{
		return true;
	}
	
	return false;
}

GetPlayerProperName(playerid)			// nieoptymalne! kod do tego odwoluje sie setki razy na sekunde! powinno byc w tablicy. TODO
{
	new
	 buffer[24],
	 i = 0,
	 pTag[5];
	 
	GetPlayerName(playerid, buffer, sizeof buffer);
	
	if(IsFS(playerid)) strdel(buffer, 0, 4);
	
	copy(GetPlayerTag(playerid), pTag);
	
	while(strlen(gData[i][name]))
	{
//		printf("PTAG %s GANG %d -%s-", GetPlayerTag(i), i, gData[i][tag]);
		if(strcmp(pTag, gData[i][tag], true) == 0)
		{
			strdel(buffer, 0, strlen(pTag) + 2);
		}
	
		i++;
	}
	
	return buffer;
}

TXT(playerid, textId)
{
	return gLang[pData[playerid][language]][textId];
}

bool:IsPasswordCorrect(szPassword[])
{
	new
	 length = strlen(szPassword);
	 
	if(length < 5 || length > 30) return false;
	
	for(new i = 0; i < length; i++)
	{
		switch(szPassword[i])
		{
			case 'a'..'z': continue;
			case 'A'..'Z': continue;
			case '0'..'9': continue;
			case '!','@','#','$','%','^','&','*','(',')','_','+','"','=','-','[',']',';',
				':','\'','<','>',',','.','/','?','`','~': continue;
			default: return false;
		}
	}
	
	return true;
}

bool:IsNickCorrect(szNick[])
{
	new
	 length = strlen(szNick);
	 
	if(length < 3 || length > 20) return false;
	
	for(new i = 0; i < length; i++)
	{
		switch(szNick[i])
		{
			case 'a'..'z': continue;
			case 'A'..'Z': continue;
			case '0'..'9': continue;
			case '.', '_', '[', ']', '(', ')', '=', '$', '@': continue;
			default: return false;
		}
	}
	
	return true;
}

Text:DrawRectangle(Float:startX, Float:endX, Float:startY, Float:endY, color)
{
	endY -= startY;
	new Text:TD = TextDrawCreate(startX, startY, "~n~");
 	TextDrawLetterSize(TD, 0.0, (endY == 0.0) ? 0.0 : (endY / 9.1));
	TextDrawTextSize(TD, endX, endX);
 	TextDrawUseBox(TD, 1);
 	TextDrawBoxColor(TD, color);
	
	return TD;
}

/*Float:GetDistanceBetweenPoints2D(Float:vX1, Float:vY1, Float:vX2, Float:vY2)
{
	return floatsqroot(floatpower(floatabs(floatsub(vX1, vX2)), 2) + floatpower(floatabs(floatsub(vY1, vY2)), 2));
}*/

Float:GetDistanceBetweenPoints3D(Float:vX1, Float:vY1, Float:vZ1, Float:vX2, Float:vY2, Float:vZ2)
{
	return floatsqroot(floatpower(floatabs(floatsub(vX1, vX2)), 2) + floatpower(floatabs(floatsub(vY1, vY2)), 2) + floatpower(floatabs(floatsub(vZ1, vZ2)), 2));
}

ShowElement(playerid, tdElement, bool:show)
{
	switch(tdElement)
	{
		case TDE_WIDE:
		{
			if(show)
			{
				for(new i = 0; i < 10; i++) TextDrawShowForPlayer(playerid, gTextDraw[i]);
				for(new i = 10; i < 20; i++) TextDrawShowForPlayer(playerid, gTextDraw[i]);
			}
			else
			{
				for(new i = 0; i < 10; i++) TextDrawHideForPlayer(playerid, gTextDraw[i]);
				for(new i = 10; i < 20; i++) TextDrawHideForPlayer(playerid, gTextDraw[i]);
			}
		}
		
		case TDE_WYBIERALKA:
		{
			if (show){
				TextDrawShowForPlayer(playerid, gTextDraw[73]);
				TextDrawShowForPlayer(playerid, gTextDraw[74]);
				TextDrawShowForPlayer(playerid, gTextDraw[75]);
			} else {
				TextDrawHideForPlayer(playerid, gTextDraw[73]);
				TextDrawHideForPlayer(playerid, gTextDraw[74]);
				TextDrawHideForPlayer(playerid, gTextDraw[75]);
			}
		}
		case TDE_CITYSELECTPLAYERSLS:
		{
			if(show)
			{
				TextDrawShowForPlayer(playerid, gTextDraw[20]);
			}
			else
			{
				TextDrawHideForPlayer(playerid, gTextDraw[20]);
			}
		}
		
		case TDE_CITYSELECTPLAYERSSF:
		{
			if(show)
			{
				TextDrawShowForPlayer(playerid, gTextDraw[21]);
			}
			else
			{
				TextDrawHideForPlayer(playerid, gTextDraw[21]);
			}
		}
		
		case TDE_CITYSELECTPLAYERSLV:
		{
			if(show)
			{
				TextDrawShowForPlayer(playerid, gTextDraw[22]);
			}
			else
			{
				TextDrawHideForPlayer(playerid, gTextDraw[22]);
			}
		}
		
		case TDE_CITYSELECTLS:
		{
			if(show)
			{
				TextDrawShowForPlayer(playerid, gTextDraw[23]);
			}
			else
			{
				TextDrawHideForPlayer(playerid, gTextDraw[23]);
			}
		}
		
		case TDE_CITYSELECTSF:
		{
			if(show)
			{
				TextDrawShowForPlayer(playerid, gTextDraw[24]);
			}
			else
			{
				TextDrawHideForPlayer(playerid, gTextDraw[24]);
			}
		}
		
		case TDE_CITYSELECTLV:
		{
			if(show)
			{
				TextDrawShowForPlayer(playerid, gTextDraw[25]);
			}
			else
			{
				TextDrawHideForPlayer(playerid, gTextDraw[25]);
			}
		}
		
		case TDE_HINT_CITYSELECT:
		{
			if(show)
			{
				TextDrawShowForPlayer(playerid, gTextDraw[27]);
				TextDrawShowForPlayer(playerid, gTextDraw[28]);
				
				switch(pData[playerid][language])
				{
					case LANG_POLISH:
					{
						TextDrawShowForPlayer(playerid, gTextDraw[29]);
						TextDrawShowForPlayer(playerid, gTextDraw[31]);
					}
					case LANG_ENGLISH:
					{
						TextDrawShowForPlayer(playerid, gTextDraw[30]);
						TextDrawShowForPlayer(playerid, gTextDraw[32]);
					}
				}
			}
			else
			{
				TextDrawHideForPlayer(playerid, gTextDraw[27]);
				TextDrawHideForPlayer(playerid, gTextDraw[28]);
				
				switch(pData[playerid][language])
				{
					case LANG_POLISH:
					{
						TextDrawHideForPlayer(playerid, gTextDraw[29]);
						TextDrawHideForPlayer(playerid, gTextDraw[31]);
					}
					case LANG_ENGLISH:
					{
						TextDrawHideForPlayer(playerid, gTextDraw[30]);
						TextDrawHideForPlayer(playerid, gTextDraw[32]);
					}
				}
			}
		}
		case TDE_VOTING:
		{
			if(show) TextDrawShowForPlayer(playerid, gTextDraw[TD_VOTING]);
			else TextDrawHideForPlayer(playerid, gTextDraw[TD_VOTING]);
		}
		
		
		case TDE_DATETIME:
		{
			if(show) {
				TextDrawShowForPlayer(playerid, gTextDraw[TD_DATETIME]);
#if defined GAMETIMESYNCTOREAL
				TextDrawShowForPlayer(playerid, gTextDraw[TD_TIME]);
#endif
			}
			else {
				TextDrawHideForPlayer(playerid, gTextDraw[TD_DATETIME]);
#if defined GAMETIMESYNCTOREAL
				TextDrawHideForPlayer(playerid, gTextDraw[TD_TIME]);
#endif
			}
		}
		case TDE_FPS:
		{
			if(show) TextDrawShowForPlayer(playerid, pTextDraw[PTD_FPS][playerid]);
			else TextDrawHideForPlayer(playerid, pTextDraw[PTD_FPS][playerid]);
		}


		
		case TDE_ATTRACTIONBOX:
		{
			if(show)
			{
//				for(new i = TD_ATTRACTIONBOX; i < TD_ATTRACTIONBOX + 8; i++) TextDrawShowForPlayer(playerid, gTextDraw[i]);
				TextDrawShowForPlayer(playerid, gTextDraw[TD_ATTRACTIONBOX]);
				TextDrawShowForPlayer(playerid, gTextDraw[TD_ATTRACTION_TITLES]);
				TextDrawShowForPlayer(playerid, gTextDraw[TD_ATTRACTION_STATES]);
			}
			else
			{
//				for(new i = TD_ATTRACTIONBOX; i < TD_ATTRACTIONBOX + 8; i++) TextDrawHideForPlayer(playerid, gTextDraw[i]);
				TextDrawHideForPlayer(playerid,gTextDraw[TD_ATTRACTIONBOX]);
				TextDrawHideForPlayer(playerid, gTextDraw[TD_ATTRACTION_TITLES]);
				TextDrawHideForPlayer(playerid, gTextDraw[TD_ATTRACTION_STATES]);
			}
		}
		
		case TDE_VEHICLEBOX:
			if(show)
				TextDrawShowForPlayer(playerid, pTextDraw[PTD_VEHICLEINFO][playerid]);
			else
				TextDrawHideForPlayer(playerid, pTextDraw[PTD_VEHICLEINFO][playerid]);
		case TDE_INFOBOX:
		{
			if(show)
			{
//				for(new i = TD_INFOBOX; i < TD_INFOBOX + 8; i++) TextDrawShowForPlayer(playerid, gTextDraw[i]);
				TextDrawShowForPlayer(playerid, pTextDraw[PTD_INFOBOX][playerid]);
			}
			else
			{
//				for(new i = TD_INFOBOX; i < TD_INFOBOX + 8; i++) TextDrawHideForPlayer(playerid, gTextDraw[i]);
				TextDrawHideForPlayer(playerid, pTextDraw[PTD_INFOBOX][playerid]);
			}
		}
		
		case TDE_WELCOMEBOX:
		{
			if(show)
			{
				for(new i = TD_WELCOMEBOX; i < TD_WELCOMEBOX + 8; i++) TextDrawShowForPlayer(playerid, gTextDraw[i]);
				TextDrawShowForPlayer(playerid, gTextDraw[TD_WELCOMEBOX2]);
				TextDrawShowForPlayer(playerid, gTextDraw[TD_WELCOMETITLE]);
				TextDrawShowForPlayer(playerid, gTextDraw[TD_WELCOMETEXT]);
			}
			else
			{
				for(new i = TD_WELCOMEBOX; i < TD_WELCOMEBOX + 8; i++) TextDrawHideForPlayer(playerid, gTextDraw[i]);
				TextDrawHideForPlayer(playerid, gTextDraw[TD_WELCOMEBOX2]);
				TextDrawHideForPlayer(playerid, gTextDraw[TD_WELCOMETITLE]);
				TextDrawHideForPlayer(playerid, gTextDraw[TD_WELCOMETEXT]);
			}
		}
		
		case TDE_STATS:
		{
			if(show)
			{
				
				TextDrawShowForPlayer(playerid, gTextDraw[TD_STAT]);
				TextDrawShowForPlayer(playerid, pTextDraw[PTD_STAT][playerid]);
				TextDrawShowForPlayer(playerid, pTextDraw[PTD_NICK][playerid]);
				
				pData[playerid][statsShowed] = true;
			}
			else
			{
				
				TextDrawHideForPlayer(playerid, gTextDraw[TD_STAT]);
				TextDrawHideForPlayer(playerid, pTextDraw[PTD_STAT][playerid]);
				TextDrawHideForPlayer(playerid, pTextDraw[PTD_NICK][playerid]);
				
				pData[playerid][statsShowed] = false;
			}
		}
		
		case TDE_FULLSERVERLOGO:
		{
			if(show)
			{
				TextDrawShowForPlayer(playerid, gTextDraw[TD_FULLSERVER]);
				TextDrawShowForPlayer(playerid, gTextDraw[TD_FULLSERVERWEBSITE]);
			}
			else
			{
				TextDrawHideForPlayer(playerid, gTextDraw[TD_FULLSERVER]);
				TextDrawHideForPlayer(playerid, gTextDraw[TD_FULLSERVERWEBSITE]);
			}
		}
	}
}

stock bool:IsPlayerInArea(playerid, Float:maxX, Float:maxY, Float:minX, Float:minY)
// nie uzywane w kodzie
{
	new
	 pVector[e_Vectors];
	
	GetPlayerPosition(playerid, pVector[X], pVector[Y], pVector[Z]);
	
	if(	pVector[X] > minX && 
		pVector[X] < maxX && 
		pVector[Y] > minY && 
		pVector[Y] < maxY) return true;
	
	return false;
}

GetPlayerCount()
{
	new
	 result = 0;
	 
	foreach(i)
	{
		result++;
	}
	
	return result;
}


bool:IsPlayerSpawned(playerid)
{
	new pstate=GetPlayerState(playerid);
	if(pstate != 1 && pstate != 2 && pstate != 3)	return false;
	if(pData[playerid][citySelecting]) return false;
	
	return true;
}

bool:StringToBool(string[])
{
	if(string[0] == '0') return false;
	else return true;
}

BoolToInt(bool:boolean)
{
	if(boolean) return 1;
	else return 0;
}

StringToInt(string[])
{
	return strval(string);
}


IntToString(int)
{
	new
	 result[16];

	valstr(result, int);
	
	return result;
}

stock copy(string1[], string2[])
{
    
	for(new i = 0; i < strlen(string1); i++)
	{
	    string2[i] = string1[i];
	}
	
	string2[strlen(string1)] = EOS;
}

bool:IsHex(szHex[])
{
	new
	 length = strlen(szHex);
	 
	if(length < 1 || length > 6) return false;
	
	for(new i = 0; i < length; i++)
	{
		switch(szHex[i])
		{
			case 'a'..'f': continue;
			case 'A'..'F': continue;
			case '0'..'9': continue;
			default: return false;
		}
	}
	
	return true;
}

FindPlayer(playerName[])
{
    new result,
	    foundCount = 0;
		
	if(IsNumeric(playerName))
	{
		result = strval(playerName);
		if(!IsPlayerConnected(result)) return ERROR_NOT_FOUND;
	}
	else
	{
	    foreach(i)
	    {
			if(FindString(GetPlayerNick(i), playerName))	// bylo GetPlayerProperName
	        {
				foundCount++;
				result = i;
	        }
	    }
		if(foundCount < 1) return ERROR_NOT_FOUND;
		else if(foundCount > 1) return ERROR_FOUND_MULTIPLE;
	}
	
	return result;
}

GetPeriodName(playerid, period, number)
{
	switch(number)
	{
		case 1:
		{
			switch(period)
			{
				case 'c': return gLang[pData[playerid][language]][72];
				case 'm': return gLang[pData[playerid][language]][33];
				case 'g': return gLang[pData[playerid][language]][34];
				case 'h': return gLang[pData[playerid][language]][34];
				case 'd': return gLang[pData[playerid][language]][35];
				case 't': return gLang[pData[playerid][language]][36];
				case 's': return gLang[pData[playerid][language]][37];
			}
		}
		case 2..4:
		{
			switch(period)
			{
				case 'c': return gLang[pData[playerid][language]][73];
				case 'm': return gLang[pData[playerid][language]][38];
				case 'g': return gLang[pData[playerid][language]][39];
				case 'h': return gLang[pData[playerid][language]][39];
				case 'd': return gLang[pData[playerid][language]][40];
				case 't': return gLang[pData[playerid][language]][41];
				case 's': return gLang[pData[playerid][language]][42];
			}
		}
		default:
		{
			switch(period)
			{
				case 'c': return gLang[pData[playerid][language]][74];
				case 'm': return gLang[pData[playerid][language]][43];
				case 'g': return gLang[pData[playerid][language]][44];
				case 'h': return gLang[pData[playerid][language]][44];
				case 'd': return gLang[pData[playerid][language]][45];
				case 't': return gLang[pData[playerid][language]][46];
				case 's': return gLang[pData[playerid][language]][47];
			}
		}
	}

	return gLang[pData[playerid][language]][43];
}

GetPeriodName2(playerid, period, number)
{
	switch(number)
	{
		case 1:
		{
			switch(period)
			{
				case 'c': return gLang[pData[playerid][language]][161];
				case 'm': return gLang[pData[playerid][language]][162];
				case 'g': return gLang[pData[playerid][language]][163];
				case 'h': return gLang[pData[playerid][language]][163];
				case 'd': return gLang[pData[playerid][language]][35];
				case 't': return gLang[pData[playerid][language]][36];
				case 's': return gLang[pData[playerid][language]][37];
			}
		}
		case 2..4:
		{
			switch(period)
			{
				case 'c': return gLang[pData[playerid][language]][73];
				case 'm': return gLang[pData[playerid][language]][38];
				case 'g','h': return gLang[pData[playerid][language]][39];

				case 'd': return gLang[pData[playerid][language]][40];
				case 't': return gLang[pData[playerid][language]][41];
				case 's': return gLang[pData[playerid][language]][42];
			}
		}
		default:
		{
			switch(period)
			{
				case 'c': return gLang[pData[playerid][language]][74];
				case 'm': return gLang[pData[playerid][language]][43];
				case 'g','h': return gLang[pData[playerid][language]][44];
				case 'd': return gLang[pData[playerid][language]][45];
				case 't': return gLang[pData[playerid][language]][46];
				case 's': return gLang[pData[playerid][language]][47];
			}
		}
	}

	return gLang[pData[playerid][language]][43];
}

GetMonthName(playerid, month)
{
	return gLang[pData[playerid][language]][164 + month];
}

PassHash(const playerName[], const password[])
{
	new
	 hashed[80];
	
	format(hashed, sizeof hashed, "%schoh6AeT%s", playerName, password);
	
	return hashed;
}

GetMySQLNameOfPeriod(period)
{
	new
	 result[8];

	switch(period)
	{
		case 'm': copy("MINUTE", result);
		case 'g','h': copy("HOUR", result);
		case 'd': copy("DAY", result);
		case 't': copy("WEEK", result);
		case 's': copy("MONTH", result);
	}
	
	return result;
}

GetPlayerIP(playerid)
{
	new
	 szIP[16];

	GetPlayerIp(playerid, szIP, sizeof szIP);
	
	return szIP;
}

GetOptimalTimeUnit(&seconds, &unit)
{
	if(seconds < 60) // seconds
	{
		unit = 'c';
	}
	else if(seconds >= 60 && seconds < 3600) // minutes
	{
		unit = 'm';
		seconds /= 60;
	}
	else if(seconds >= 3600 && seconds < 864000) // hours
	{
		unit = 'g';
		seconds /= 3600;
	}
	else // days
	{
		unit = 'd';
//		seconds /= 5184000;
		seconds /= 86400;
	}
}

bool:IsAdmin(playerid, level = 2)
{
//	if(!IsPlayerAdmin(playerid) && pData[playerid][adminLevel] < level) return false;
	if(pData[playerid][adminLevel] < level) return false;
	return true;
}

bool:IsGM(playerid)
	return ((pData[playerid][adminLevel]<LEVEL_GM) ? false : true);

Teleport(type, playerid, Float:vectorX, Float:vectorY, Float:vectorZ, Float:angle = 0.0, interior = 0xFFF, virtualWorld = 0xFFF, bool:prestream=false)
{
	if(interior != 0xFFF) SetPlayerInterior(playerid, interior);
	if(virtualWorld != 0xFFF) SetPlayerVirtualWorld(playerid, virtualWorld);

	if (prestream)
		Streamer_UpdateEx(playerid,vectorX,vectorY,vectorZ);

	pTemp[playerid][specPosReturn]=false;

	if(type == T_PLAYER || !IsPlayerInAnyVehicle(playerid) ||  GetPlayerState(playerid)==PLAYER_STATE_PASSENGER)
	{
		if(GetPlayerAnimationIndex(playerid)==1134) RemovePlayerWeapon(playerid,46);
		SetPlayerPos(playerid, vectorX, vectorY, vectorZ);
		if(interior != 0xFFF) SetPlayerInterior(playerid, interior);
		if(virtualWorld != 0xFFF) SetPlayerVirtualWorld(playerid, virtualWorld);
		if (prestream) {
			FreezePlayer(playerid,750);
			SetPlayerPos(playerid, vectorX, vectorY, vectorZ);
			Streamer_Update(playerid);
		}
		if (angle==angle)
			SetPlayerFacingAngle(playerid, angle);
		
	} else if(type == T_BOTH) {

		new
		 vehicleid = GetPlayerVehicleID(playerid); 

		vehicles_GetSpawnOffset(vehicleid, vectorX, vectorY, vectorZ);
		if (GetPlayerInterior(playerid)>0 && GetPlayerInterior(playerid)!=interior) {
			SetPlayerInterior(playerid,interior);
			LinkVehicleToInterior(vehicleid,interior);
		}
		SetVehiclePos(vehicleid, vectorX, vectorY, vectorZ);
		if (angle==angle)
			SetVehicleZAngle(vehicleid, angle);
	} else if(type == T_BOTH_PV) {
		new
		 vehicleid = GetPlayerVehicleID(playerid),
		 Float:V[3];
		GetVehicleVelocity(vehicleid,V[0],V[1],V[2]);

		if (GetPlayerInterior(playerid)>0 && GetPlayerInterior(playerid)!=interior) {
			SetPlayerInterior(playerid,interior);
			LinkVehicleToInterior(vehicleid,interior);
		}

		SetVehiclePos(vehicleid, vectorX, vectorY, vectorZ);
		if (angle==angle)
			SetVehicleZAngle(vehicleid, angle);
		SetVehicleVelocity(vehicleid, V[0], V[1], V[2]);

	}
	if (prestream)
		SetTimerEx("TeleportVerify",1000,false,"df",playerid,vectorZ);

	pTemp[playerid][lastPos]=-1;
	pTemp[playerid][curPos]=1;

	SetCameraBehindPlayer(playerid);
}

public TeleportVerify(playerid,Float:vectorZ){
	new Float:PP[3];

	if (IsPlayerInAnyVehicle(playerid)) {
		GetVehiclePos(GetPlayerVehicleID(playerid),PP[0],PP[1],PP[2]);
		if (vectorZ-PP[2]>2)
			SetVehiclePos(GetPlayerVehicleID(playerid),PP[0],PP[1],vectorZ);
	} else {
		GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
		if (vectorZ-PP[2]>1)
			SetPlayerPos(playerid,PP[0],PP[1],vectorZ);
	}
	
	return 1; 
}


bool:IsValidSkin(skinid)
{
	if(skinid < 0 || skinid > 299) return false;
	if(skinid > 2 && skinid <= 6) return false;
	if(skinid == 8 || skinid == 42 || skinid == 65 || skinid == 74 || skinid == 86 || skinid == 119 || skinid == 149 || skinid == 208 || skinid == 239 || skinid==289) return false;
	if(skinid >= 265 && skinid<269) return false;
	if(skinid > 269 && skinid <= 273) return false;
	
	return true;
}

bool:IsPlayerInCube(playerid, Float:maxX, Float:maxY, Float:maxZ, Float:minX, Float:minY, Float:minZ)
{
	new
	 Float:pVector[e_Vectors];
	
	GetPlayerPosition(playerid, pVector[X], pVector[Y], pVector[Z]);
	   
	if(pVector[X] > minX && pVector[Y] > minY && pVector[Z] > minZ && pVector[X] < maxX && pVector[Y] < maxY && pVector[Z] < maxZ)
	{
		return true;
	}
   
	return false;
}

GetPlayerPosition(playerid, &Float:vX, &Float:vY, &Float:vZ, &Float:vA = 1.1111)
{
	GetPlayerPos(playerid, vX, vY, vZ);
	if(vA != 1.1111) GetPlayerFacingAngle(playerid, vA);	// co to za magic number, TODO zamienic na FLOAT_NAN. Idealny material na bohr-buga
}

/*SetPlayerPosition(playerid, Float:vX, Float:vY, Float:vZ, Float:vA)
{
	SetPlayerPos(playerid, vX, vY, vZ);
	SetPlayerFacingAngle(playerid, vA);
}*/

bool:IsCloseToATM(playerid)
{
	new
	 Float:pVector[e_Vectors],
	 Float:pDistance;
	
	GetPlayerPosition(playerid, pVector[X], pVector[Y], pVector[Z]);
	
	for(new i = 0; i < sizeof DATA_ATM; i++)
	{
		pDistance = GetDistanceBetweenPoints3D(pVector[X], pVector[Y], pVector[Z], DATA_ATM[i][X], DATA_ATM[i][Y], DATA_ATM[i][Z]);
		
		if(pDistance < 1.3) return true;
	}
	
	return false;
}

DownCase(letter)
{
	if(letter >= 'A' && letter <= 'Z')
	{
		return letter + 32;
	}
	
	switch(letter)
	{
		case 'Ê': return 'ê';
		case 'Ó': return 'ó';
		case 'Œ': return 'œ';
		case '£': return '³';
		case '¯': return '¿';
		case '': return 'Ÿ';
		case 'Æ': return 'æ';
		case 'Ñ': return 'ñ';
	}
	
	return letter;
}

SetAlpha(color, alpha)
{
	return (((color >> 24) & 0xFF) << 24 | ((color >> 16) & 0xFF) << 16 | ((color >> 8) & 0xFF) << 8 | floatround((float(color & 0xFF) / 255) * alpha));
}

GetPlayerCurrentSession(playerid)
{
	return (GetTickCount() - pData[playerid][session]) / 1000;
}

bool:IsCorrectIP(szIP[])
{
	if(!strlen(szIP) || strlen(szIP) > 15) return false;
	
	new
	 _numbers,
	 _dots,
	 _stars;
	
	for(new i = 0; i < strlen(szIP); i++)
	{
		if((szIP[i] < '0' && szIP[i] > '9') && szIP[i] != '.' && szIP[i] != '*') return false;
		
		switch(szIP[i])
		{
			case '0'..'9': _numbers++;
			case '.': _dots++;
			case '*': _stars++;
			default: return false;
		}
	}
	
	if(_numbers > 13) return false;
	if(_dots != 3) return false;
	if(_stars > 4) return false;
	if(_numbers + _stars < 4) return false;
	
	return true;
}

RefreshPlayerStars(playerid)
{
	new
	 pLevel;
	
	if(pData[playerid][respect] <= 4)											pLevel = 1;
	if(pData[playerid][respect] >= 5 && pData[playerid][respect] <= 9)			pLevel = 2;
	if(pData[playerid][respect] >= 10 && pData[playerid][respect] <= 14)		pLevel = 3;
	if(pData[playerid][respect] >= 15 && pData[playerid][respect] <= 19)		pLevel = 4;
	if(pData[playerid][respect] >= 20 && pData[playerid][respect] <= 24)		pLevel = 5;
	if(pData[playerid][respect] >= 25 && pData[playerid][respect] <= 29)		pLevel = 6;
	if(pData[playerid][respect] >= 30 && pData[playerid][respect] <= 59)		pLevel = 7;
	if(pData[playerid][respect] >= 60 && pData[playerid][respect] <= 89)		pLevel = 8;
	if(pData[playerid][respect] >= 90 && pData[playerid][respect] <= 119)		pLevel = 9;
	if(pData[playerid][respect] >= 120 && pData[playerid][respect] <= 149)		pLevel = 10;
	if(pData[playerid][respect] >= 150 && pData[playerid][respect] <= 179)		pLevel = 11;
	if(pData[playerid][respect] >= 180 && pData[playerid][respect] <= 209)		pLevel = 12;
	if(pData[playerid][respect] >= 210 && pData[playerid][respect] <= 309)		pLevel = 13;
	if(pData[playerid][respect] >= 310 && pData[playerid][respect] <= 409)		pLevel = 14;
	if(pData[playerid][respect] >= 410 && pData[playerid][respect] <= 509)		pLevel = 15;
	if(pData[playerid][respect] >= 510 && pData[playerid][respect] <= 609)		pLevel = 16;
	if(pData[playerid][respect] >= 610 && pData[playerid][respect] <= 709)		pLevel = 17;
	if(pData[playerid][respect] >= 710 && pData[playerid][respect] <= 809)		pLevel = 18;		
	if(pData[playerid][respect] >= 810 && pData[playerid][respect] <= 1809)		pLevel = 19;
	if(pData[playerid][respect] >= 1810 && pData[playerid][respect] <= 2809)	pLevel = 20;
	if(pData[playerid][respect] >= 2810 && pData[playerid][respect] <= 3809)	pLevel = 21;
	if(pData[playerid][respect] >= 3810 && pData[playerid][respect] <= 4809)	pLevel = 22;
	if(pData[playerid][respect] >= 4810 && pData[playerid][respect] <= 5809)	pLevel = 23;
	if(pData[playerid][respect] >= 5810 && pData[playerid][respect] <= 6809)	pLevel = 24;
	if(pData[playerid][respect] >= 6810 && pData[playerid][respect] <= 16809)	pLevel = 25;
	if(pData[playerid][respect] >= 16810 && pData[playerid][respect] <= 26809)	pLevel = 26;
	if(pData[playerid][respect] >= 26810 && pData[playerid][respect] <= 36809)	pLevel = 27;
	if(pData[playerid][respect] >= 36810 && pData[playerid][respect] <= 46809)	pLevel = 28;
	if(pData[playerid][respect] >= 46810 && pData[playerid][respect] <= 56809)	pLevel = 29;
	if(pData[playerid][respect] >= 56810)										pLevel = 30;
	
	for(new i = TD_STARS_START; i <= TD_STARS_END; i++)
	{
		TextDrawHideForPlayer(playerid, gTextDraw[i]);
	}
	
	switch(pLevel)
	{
		case 1..6:
		{
			for(new i = 1; i <= pLevel; i++) TextDrawShowForPlayer(playerid, gTextDraw[TD_STARS_START + pLevel - 1]);
		}
		
		case 7..12:
		{
			for(new i = 7; i <= pLevel; i++) TextDrawShowForPlayer(playerid, gTextDraw[TD_STARS_START + pLevel - 1]);
		}
		
		case 13..18:
		{
			for(new i = 13; i <= pLevel; i++) TextDrawShowForPlayer(playerid, gTextDraw[TD_STARS_START + pLevel - 1]);
		}
		
		case 19..24:
		{
			for(new i = 19; i <= pLevel; i++) TextDrawShowForPlayer(playerid, gTextDraw[TD_STARS_START + pLevel - 1]);
		}
		
		case 25..30:
		{
			for(new i = 25; i <= pLevel; i++) TextDrawShowForPlayer(playerid, gTextDraw[TD_STARS_START + pLevel - 1]);
		}
	}
}

GetVehicleSpeed(vehicleid)
{
	// 1000 jednostek w grze, przejechanych z predkoscia 100km/h zajmuje 28minut
	// wychodzi 46.2km
	// odl [j] / 21.645 = odl [km/h]
    new
	 vVector[e_Vectors];

	GetVehicleVelocity(vehicleid, vVector[X], vVector[Y], vVector[Z]);

	return floatround(floatmul(floatmul(vVector[X], vVector[X]) + floatmul(vVector[Y], vVector[Y]) + floatmul(vVector[Z], vVector[Z]), 190));
// bylo:	
//    result = floatsqroot(floatpower(floatabs(vVector[X]), 2.0) + floatpower(floatabs(vVector[Y]), 2.0) + floatpower(floatabs(vVector[Z]), 2.0));
//	return floatround(result * 100 * 1.61);
}


Float:GetPlayerSpeed(playerid)
{
    new
     Float:vVector[3];
    GetPlayerVelocity(playerid, vVector[0], vVector[1], vVector[2]);
    return floatabs(vVector[0])+floatabs(vVector[1])+floatabs(vVector[2]);
}


stock GetPlayerSpeedRescaled(playerid)
{
	// 1000 jednostek w grze, przejechanych z predkoscia 100km/h zajmuje 28minut
	// wychodzi 46.2km
	// odl [j] / 21.645 = odl [km/h]
    new
	 vVector[e_Vectors];

	GetPlayerVelocity(plyaerid, vVector[X], vVector[Y], vVector[Z]);

	return floatround(floatmul(floatmul(vVector[X], vVector[X]) + floatmul(vVector[Y], vVector[Y]) + floatmul(vVector[Z], vVector[Z]), 190));
}

bool:CheckTildes(szText[])
{
	new
	 tCount;
	
	for(new i = 0; i < strlen(szText); i++)
	{
		if(szText[i] == '~') tCount++;
	}
	
	if(tCount & 1) return false;
	else return true;
}

GetPlayerTag(playerid)
{
	new
	 szName[24],
	 idxStart = -1,
	 idxEnd = -1;
	
	GetPlayerName(playerid, szName, sizeof szName);
	
	for(new i = 0; i < strlen(szName); i++)
	{
		if(szName[i] == '[') idxStart = i;
		if(szName[i] == ']' && idxStart>-1 && (i-idxStart>1) && (!( (i-idxStart==3) && szName[i+1]=='F' && szName[i+2]=='S' )))
		{
			idxEnd = i;
			break;
		}
	}
	
	if(idxStart == -1 || idxEnd == -1 || idxStart >= idxEnd - 1)
	{
		format(szName, sizeof szName, "");
		return szName;
	}
	
	strdel(szName, 0, idxStart + 1);
	strdel(szName, idxEnd - 1, strlen(szName));
	
	return szName;
}

GetPlayerNick(playerid)
{
	new
	 szPlayerName[24];
	
	GetPlayerName(playerid, szPlayerName, sizeof szPlayerName);
	
	return szPlayerName;
}


StringReplace(trg[], newstr[], src[])
{
    new
	 f = 0,
     s1[256],
     tmp[256];
	 
    format(s1, sizeof s1, "%s", src);
	
    f = strfind(s1, trg);
	
    tmp[0] = 0;
	
    while (f >= 0)
	{
        strcat(tmp, ret_memcpy(s1, 0, f));
        strcat(tmp, newstr);
        format(s1, sizeof s1, "%s", ret_memcpy(s1, f + strlen(trg), strlen(s1) - f));
        f = strfind(s1, trg);
    }
	
    strcat(tmp, s1);
	
    return tmp;
}

ret_memcpy(source[], index = 0, numbytes)
{
	new
	 tmp[256];
	
	tmp[0] = 0;
	
	if(index >= strlen(source)) return tmp;
	if(numbytes + index >= strlen(source)) numbytes = strlen(source) - index;
	if(numbytes <= 0) return tmp;
	
	for(new i = index;i < numbytes + index; i++)
	{
		tmp[i - index] = source[i];
		if(source[i] == 0) return tmp;
	}
	
	tmp[numbytes] = 0;
	
	return tmp;
}
/*
GetPlayerCurrentHouseEntry(playerid)
{
	new
	 Float:pVector[e_Vectors],
	 Float:pDistance;
	
	GetPlayerPosition(playerid, pVector[X], pVector[Y], pVector[Z]);
	
	for(new i = 0; i < sizeof hData; i++)
	{
		pDistance = GetDistanceBetweenPoints3D(pVector[X], pVector[Y], pVector[Z], hData[i][hEntry][X], hData[i][hEntry][Y], hData[i][hEntry][Z]);
		
		if(pDistance < 1.0) return i;
	}
	
	return -1;
}
*/
bool:SpaceCheck(string[])
{
	for(new i = 0; i < strlen(string); i++)
	{
		if(string[i] == ' ') return true;
	}
	
	return false;
}

bool:IsCorrectWordForCensorship(szWord[])
{
	new
	 length = strlen(szWord);

	if(length < 3 || length > 30) return false;
	
	for(new i = 0; i < length; i++)
	{
		switch(szWord[i])
		{
			case '0'..'9': continue;
			case 'a'..'z': continue;
			case 'A'..'Z': continue;
			default: return false;
		}
	}
	
	return true;
}

bool:TeleportAllowed(playerid)
{
	if(gmData[artefactOwner] == playerid){ 
		switch(random(3)){
			case 0:
				Msg(playerid,COLOR_INFO2,"Czujesz dziwne mrowienie na plecach...", false);
			case 1:
				Msg(playerid,COLOR_INFO2,"Artefakt przez chwile staje sie bardzo ciezki...", false);
			case 2:
				Msg(playerid,COLOR_INFO2,"Artefakt na sekunde robi sie bardzo goracy...", false);
		}
		return false;
	}
	if(pData[playerid][jail] >= 0) return false;
	if(pTemp[playerid][troll]) return false;
	if (pData[playerid][pAttraction]==A_ARENA) {
			Msg(playerid,COLOR_INFO,"Jestes na arenie. Aby moc korzystac z komend opusc ja za pomoca komendy {b}/arenaexit{/b}.");
			GameTextForPlayer(playerid,"_~n~_~n~/arenaexit", 1000,5);
			return false;
	}
	if(pData[playerid][pAttraction] == A_WG && pTemp[gmTemp[aWGPlayers][playerid]][aWGDead]) { }
	else if(pData[playerid][pAttraction] != A_NONE) return false;

	if(!IsPlayerSpawned(playerid) || pData[playerid][spectating] != INVALID_PLAYER_ID) return false;
	
	return true;
}

bool:AttractionAllowed(playerid)
{
	if(pData[playerid][pAttraction] != A_NONE) return false;
	if(pData[playerid][spectating] != INVALID_PLAYER_ID) return false;
	if(!IsPlayerSpawned(playerid)) return false;
	if(pData[playerid][jail] > 0) return false;
	if(pTemp[playerid][staleTime]>5) return false;
//	if(pData[playerid][previewingHouse]) return false;
		
	return true;
}

bool:SpecAllowed(playerid)
{
//	if(pData[playerid][previewingHouse]) return false;
	if(pData[playerid][jail] >= 0) return false;
	if(pData[playerid][pAttraction] != A_NONE) return false;
//	if(!IsPlayerSpawned(playerid)) return false;
		
	return true;
}

GetAttractionPlayerCount(attractionId)
{
	new
	 result = 0;

	foreach(playerid)
	{
		switch(attractionId)
		{
			case A_CHOWANY: if(pData[playerid][aChowany]) result++;
			case A_SPS: if(pData[playerid][aSPS]) result++;
			case A_DERBY: if(pData[playerid][aDerby]) result++;
			case A_LABIRYNT: if(pData[playerid][aLabirynt]) result++;
			case A_RACE: if(pData[playerid][aRace]) result++;
			case A_DRIFT: if(pData[playerid][aDrift]) result++;
			case A_WG: if(pData[playerid][aWG]) result++;
			case A_STRZELNICA: if(pData[playerid][aStrzelnica]) result++;
		}
	}
	
	return result;
}

GetAttractionMinimumQueueValue(attractionId)
{
	switch(attractionId)
	{
		case A_CHOWANY: return MIN_QUEUE_CHOWANY;
		case A_SPS: return MIN_QUEUE_SPS;
		case A_DERBY: return MIN_QUEUE_DERBY;
		case A_LABIRYNT: return MIN_QUEUE_LABIRYNT;
		case A_RACE: return MIN_QUEUE_RACE;
		case A_DRIFT: return MIN_QUEUE_DRIFT;
		case A_WG: return MIN_QUEUE_WG;
		case A_STRZELNICA: return MIN_QUEUE_STRZELNICA;
	}
	
	return 0;
}

GetAttractionMaximumQueueValue(attractionId)
{
	switch(attractionId)
	{
		case A_CHOWANY: return MAX_QUEUE_CHOWANY;
		case A_SPS: return MAX_QUEUE_SPS;
		case A_DERBY: return MAX_QUEUE_DERBY;
		case A_LABIRYNT: return MAX_QUEUE_LABIRYNT;
		case A_RACE: return MAX_QUEUE_RACE;
		case A_DRIFT: return MAX_QUEUE_DRIFT;
		case A_WG: return MAX_QUEUE_WG;
		case A_STRZELNICA: return MAX_QUEUE_STRZELNICA;
	}
	
	return 0;
}

bool:IsPlayerNearWater(playerid){
	new vehid=GetPlayerVehicleID(playerid);
	if (vehid>0 && vehid!=INVALID_VEHICLE_ID)
		switch(GetVehicleModel(vehid)) {
			case 472,473,493,595,484,430,453,452,446,454,539,447,460:
				return true;
		}
	// nie jest w pojezdzie


	new index = GetPlayerAnimationIndex(playerid);
	if ((index >=  1538 && index <= 1541) || index == 1544)	return true;

	new Float:ElevDelta,Float:P[3];
	
	GetPlayerPos(playerid,P[0],P[1],P[2]);
	if (P[2]<-2 || P[2]>41) return false;	// kolo tamy

	MapAndreas_FindZ_For2DCoord(P[0],P[1],ElevDelta);
	if (P[2]-ElevDelta>9)	return false;
	if (P[2]<(ElevDelta-2) && (P[2]<-2 || P[2]>2))	// gracz jest pod najwyzszym punktem i nie na poziomie wody
		return false;
	if (P[2]>=-2 && P[2]<=2) return true;
	// todo czy obok jest punkt o wysokosci rowno 0.0000
	
	return false;
}

bool:IsNatatorialVehicleModel(vehiclemodel){
	switch(vehiclemodel){
		case 472,473,493,595,484,430,453,452,446,454,539,447,460:
                return true;
	}
	return false;
}

bool:IsBikeVehicle(vehiclemodel){
	switch(vehiclemodel){
//		case 448,461,462,463,468,471,481,509,510,521,522,581,586:	// motory i rowery
		case 481,509,510:	// same rowery
			return true;
	}
	return false;
}

spawnVehicleNearPlayer(playerid,vehicleid){
	if (GetPlayerInterior(playerid)!=0 || GetPlayerVirtualWorld(playerid)!=0)
		return INVALID_VEHICLE_ID;
	new Float:P[4],vehid;
	GetPlayerPos(playerid,P[0],P[1],P[2]);
	GetPlayerFacingAngle(playerid,P[3]);
	vehicles_GetSpawnOffsetByModel(vehicleid,P[0],P[1],P[2]);
	vehid=CreateVehicle(vehicleid, P[0],P[1],P[2],P[3],-1,-1,600);
	tVehicleSpawned[vehid]=true;

	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehid, 1, 1, 0, 0, bonnet, boot, objective);
	SetVehicleParamsForPlayer(vehid, playerid, 0, DOOR_OPENED);
	vehicleDoorState[vehid] = DOOR_OPENED;
	vehicleDoorOwner[vehid] = playerid;
	
	vehicleHasNitro[vehid]=false;
	tVehicles[vehid][vo_hasTurbo]=false;
	SetVehicleHealth(vehid, VEHICLE_DEFAULT_HP);
	return vehid;
}

public spawnVehicleForPlayer(playerid,vehicleid,bool:replace){
	if (GetPlayerVirtualWorld(playerid)!=0)
		return INVALID_VEHICLE_ID;
	
	if (GetPlayerInterior(playerid)!=0) {
		if ((IsGM(playerid) || pData[playerid][vipEnabled]) && IsBikeVehicle(vehicleid))
			{ }
		else
			return INVALID_VEHICLE_ID;
	}
	
    new Float:P[4],vehid;
    GetPlayerPos(playerid,P[0],P[1],P[2]);
	if (IsPlayerInAnyVehicle(playerid))
		GetVehicleZAngle(GetPlayerVehicleID(playerid),P[3]);
	else
		GetPlayerFacingAngle(playerid,P[3]);

	if (IsNatatorialVehicleModel(vehicleid) && !IsPlayerNearWater(playerid))
		return INVALID_VEHICLE_ID;

    if (replace && GetPlayerVehicleSeat(playerid)==0) {
	vehid=GetPlayerVehicleID(playerid);
	RemovePlayerFromVehicle(playerid);
	RespawnVehicle(vehid);
//	DestroyVehicle(vehid);
    }

	vehicles_GetSpawnOffsetByModel(vehicleid,P[0],P[1],P[2]);

    vehid=CreateVehicle(vehicleid, P[0],P[1],P[2],P[3],-1,-1,600);


    tVehicleSpawned[vehid]=true;
	
	
	new engine, lights, alarm, doors, bonnet, boot, objective;
    GetVehicleParamsEx(vehid, engine, lights, alarm, doors, bonnet, boot, objective);
	SetVehicleParamsEx(vehid, 1, 1, 0, 0, bonnet, boot, objective);
	SetVehicleParamsForPlayer(vehid, playerid, 0, DOOR_OPENED);
	vehicleDoorState[vehid] = DOOR_OPENED;
	vehicleDoorOwner[vehid] = playerid;
	
	vehicleHasNitro[vehid]=false;
	tVehicles[vehid][vo_hasTurbo]=false;

    LinkVehicleToInterior(vehid,GetPlayerInterior(playerid));
    PutPlayerInVehicle(playerid,vehid,0);
	
	SetVehicleHealth(vehid, VEHICLE_DEFAULT_HP);
	
	// pojazdy RC
	
    return vehid;
}

stock RemovePlayerWeapon(playerid, weaponid)
{
	new plyWeapons[12];
	new plyAmmo[12];
	

	for(new slot = 0; slot != 12; slot++)
	{
		new wep, ammo;
		GetPlayerWeaponData(playerid, slot, wep, ammo);
		
		if(wep != weaponid)
			GetPlayerWeaponData(playerid, slot, plyWeapons[slot], plyAmmo[slot]);
	}

	pTemp[playerid][disableWeaponCheck]=true;
	
	ResetPlayerWeapons(playerid);
	for(new slot = 0; slot != 12; slot++)
	{
		GivePlayerWeapon(playerid, plyWeapons[slot], plyAmmo[slot]);
	}
	pTemp[playerid][disableWeaponCheck]=false;
}

stock RemoveDangerousWeapon(playerid)
{
	new plyWeapons[12];
	new plyAmmo[12];

	for(new slot = 0; slot != 12; slot++)
	{
		new wep, ammo;
		GetPlayerWeaponData(playerid, slot, wep, ammo);
		switch(wep){
			case 0,35,36,37,38,39,40,44,45: {
			}
			default:
				GetPlayerWeaponData(playerid, slot, plyWeapons[slot], plyAmmo[slot]);
		}
	}
	pTemp[playerid][disableWeaponCheck]=true;
	ResetPlayerWeapons(playerid);
	
	for(new slot = 0; slot != 12; slot++)
		GivePlayerWeapon(playerid, plyWeapons[slot], plyAmmo[slot]);

	pTemp[playerid][disableWeaponCheck]=false;
}




stock GetVehicleFlags(vehicleid){
	new vm=GetVehicleModel(vehicleid);
	new r;
	
	// airborne, airplanes	------------------------------------------------------
	switch (vm){
		case 425,548,417,487,497,563,469,447:	// sea sparrow
		{
			r+=VF_AIRBORNE;
			r+=VF_HELICOPTER;
		}
		case 460,511,512,520,593,553,519,513:	// samoloty
		{
			r+=VF_AIRBORNE;
			r+=VF_AIRPLANE;
		}
		case 539:								// vortex
			r+=VF_AIRBORNE;
	}
	
	// wojskowe	--------------------------------------------------------------------
	switch(vm){
		case 425,520,432:	// hunter, hydra, rhino
			r+=VF_MILITARY;
	}
	// plywajace ------------------------------------------------------------------
	switch(vm){
		case 472,473,493,595,484,430,453,452,446,454,	// lodzie
			539,		// vortex
			447,460:	// sea sparrow, skimmer
			r+=VF_NATATORIAL;
	}
	// zdalnie sterowane	--------------------------------------------------------
	switch(vm){
		case 441,464,465,501,564,594:	r+=VF_RC;
	}
	// motory i rowery --------------------------------------------------------------
	switch(vm){
		case 471,468,586,463,523,521,461,522,581,448,462,510,481,509: 
			r+=VF_BIKES;
	}
	// drogowe	---------------------------------------------------------------------
	if (vm==539 ||						// vortex
		(r&VF_NATATORIAL!=VF_NATATORIAL	&&	// lodzie
		r&VF_AIRBORNE!=VF_AIRBORNE)		// lotnicze
		) 
			r+=VF_STREET;

	// mozliwe do holowania	----------------------------------------------------------
	switch(vm){
		case 437,544,431,407,408,		// coach, fire truck, bus, fire truck, trashmaster
			570,569,538,537,449,		// przyczepy do pociagow, pociagi, tram
			532,524,					// kombajn, cement truck
			403,514,515,443,			// tiry, packer,
			435,						// przyczepy
			433,432,					// barracks, rhino
			406:						// dumper
		{ }
		default: 
			if (
					r&VF_NATATORIAL!=VF_NATATORIAL	&&	// lodzie
					r&VF_AIRBORNE!=VF_AIRBORNE	&&		// lotnicze
					r&VF_RC!=VF_RC 				&&		// zdalnie sterowane
					r&VF_BIKES!=VF_BIKES
			)		r+=VF_TOWABLE;
		
	}
	return r;
}

stock bool:IsPlayerInAirVehicle(playerid){
	if (!IsPlayerInAnyVehicle(playerid)) return false;
	new vm=GetVehicleModel(GetPlayerVehicleID(playerid));
	switch (vm){
		case 548,417,487,497,563,469,	511,512,593,553,519,460,513:
			return true;
		default:
			return false;
	}
	return false;
}

SyncPlayerGameTime(playerid){
#if defined GAMETIMESYNCTOREAL
	new tt[3];
	gettime(tt[0], tt[1], tt[2]);
	SetPlayerTime(playerid, tt[0], tt[1]);
#else
	// synchronizujemy zegarek z czasem w grze
	new t=floatround(GetTickCount()/1000)+gmTemp[gametime_offset];
    SetPlayerTime(playerid, floatround(t/60)%24, t%60);
#endif
	return;
}
SyncPlayerWeather(playerid){
	if (GetPlayerInterior(playerid)==0)
		SetPlayerWeather(playerid,gmTemp[currentWeather]);
	return;
}



/*
stock bool:VehicleModelCanTeleport(vm){
	switch (vm){
		case 
			537,538,	// pociagi
			577,592,	// at-400, andromeda
			425,520,	// hunter,hydra
			432:		// rhino
			return false;
		
	}
	
}*/

bool:SkinKobiecy(s){
    switch(s){
        case 9,10..13,31,38..41, 53..56, 63, 64, 69, 75..77, 85, 87, 88..93,129, 130, 131,138..141,145,148,150..152,157,169,172,178,190..199,201,205,207,211,214,215,
            216,218,219,224..226,231,232,233,237,238,243,244..246, 251,256,257,263,298:
            return true;
    }
    return false;
}

bool:Kobieta(playerid)
	return SkinKobiecy(GetPlayerSkin(playerid));


SaveReturnPath(playerid,destint){
	retpos[playerid][tpDestInt]=destint;
	retpos[playerid][tpWorld]=GetPlayerVirtualWorld(playerid);
	retpos[playerid][tpInterior]=GetPlayerInterior(playerid);
	retpos[playerid][tpStreamed]=(Streamer_CountVisibleItems(playerid, STREAMER_TYPE_OBJECT)>0)?true:false;
	new Float:PP[3],Float:VV[3];
	GetPlayerPos(playerid,PP[0],PP[1],PP[2]);
	GetPlayerVelocity(playerid,VV[0],VV[1],VV[2]);
	retpos[playerid][tpX]=PP[0]-(VV[0]*3);
	retpos[playerid][tpY]=PP[1]-(VV[1]*3);
	retpos[playerid][tpZ]=PP[2]-(VV[2]*1.5);
	pTemp[playerid][pickupDelay]=GetTickCount();
	GetPlayerFacingAngle(playerid,retpos[playerid][tpA]);
	retpos[playerid][tpA]-=180;
	return 1;
}

UseReturnPath(playerid){
	if (GetPlayerInterior(playerid)!=retpos[playerid][tpDestInt]) // nie mamy informacji gdzie wracac, prawdpodobnie gracz przeteleportowal sie do innego interiora
		return Msg(playerid,COLOR_ERROR,"Zamkniete!");
	else if (retpos[playerid][tpX]==retpos[playerid][tpX]) {
		pTemp[playerid][pickupDelay]=GetTickCount();
		Teleport(T_PLAYER, playerid, retpos[playerid][tpX],retpos[playerid][tpY],retpos[playerid][tpZ], retpos[playerid][tpA], retpos[playerid][tpInterior], retpos[playerid][tpWorld], retpos[playerid][tpStreamed]);
		Audio_StopBGSound(playerid);
	}
	return 1;
}

GetPlayerWeaponList(playerid,uid){
	new buf[127];
	if (!IsPlayerConnected(uid)) return;
	if (!IsPlayerSpawned(uid)) return;
	new wepid,ammo;
	new warn=0;
	new clr;
	for (new slot=0; slot<=12;slot++) {
		GetPlayerWeaponData(uid,slot,wepid,ammo);
		if (ammo==0) continue;
		warn=0;
		if (wepid<=15 && ammo>1) warn++;
		if (wepid<=15 && ammo>10) warn++;
		if (ammo==198) warn++;
		else if (ammo<0) warn++;
		if (!pTemp[uid][wStrefieFULLDM]) {
				if (wepid==38 && pData[uid][pAttraction]!=A_CHOWANY) warn++;	// minigun
				else if (wepid==37) warn++;
				else if (wepid==35) warn++;
		}
		if (wepid==36 || wepid==44 || wepid==45) // hs rpg, gogle
			warn=warn+2;
		// max warn = 3, dla bezpieczenstwa kodujemy na 4 
		clr=0xff - (warn*60);
		format(buf, sizeof buf, "%s {ff%02x%02x}%d-%s(%d)", buf, clr, clr, wepid, weaponName[wepid], ammo);
		if (strlen(buf)>100) {
			SCM(playerid,-1,buf);
			format(buf, sizeof buf, "");
		}
	}
	if (strlen(buf)>0)
		SCM(playerid,-1,buf);
	return;
}

stock strtok(const string[], &index,seperator=' ')
{
	new length = strlen(string);
	new offset = index;
	new sresult[128];
	while ((index < length) && (string[index] != seperator) && ((index - offset) < (sizeof(sresult) - 1)))
	{
		sresult[index - offset] = string[index];
		index++;
	}

	sresult[index - offset] = EOS;
	if ((index < length) && (string[index] == seperator))
	{
		index++;
	}
	return sresult;
}

GetPlayerFPS(playerid)
{
	pTemp[playerid][drunkLevel]=GetPlayerDrunkLevel(playerid);
	if(pTemp[playerid][drunkLevel]<100)
		SetPlayerDrunkLevel(playerid, 2000);
	else{
		if(pTemp[playerid][drunkLevelL]!=pTemp[playerid][drunkLevel]) {
			new FPS=pTemp[playerid][drunkLevelL]-pTemp[playerid][drunkLevel];
			pTemp[playerid][drunkLevelL]=pTemp[playerid][drunkLevel];
			return FPS-1;
		}
	}
	return 0;
}

FindUIDByNameOrID(params[],&uid,playerid=INVALID_PLAYER_ID){
	uid = FindPlayer(params);
	new buffer[128];	
	if(uid == ERROR_NOT_FOUND)
	{
		if(playerid!=INVALID_PLAYER_ID) {
			format(buffer, sizeof buffer, TXT(playerid, 27), params);
			Msg(playerid, COLOR_ERROR, buffer);
		}
		return 0;
		
	}
	if(uid == ERROR_FOUND_MULTIPLE)
	{
		if(playerid!=INVALID_PLAYER_ID) {
			format(buffer, sizeof buffer, TXT(playerid, 28), params);
			Msg(playerid, COLOR_ERROR, buffer);
		}
		return 0;
	}
	return 1;
}

/*bool:DetectIPAddress(text[]){
	//  --- Anti server IP ads ---
	
	new
	 numbers,
	 dots,
	 colons;
	
	for(new i = 0; i < strlen(text); i++)
	{
		if(text[i] >= '0' && text[i] <= '9') numbers++;
		if(text[i] == '.') dots++;
		if(text[i] == ':') colons++;
	}
	
	if(numbers >= 10 && dots >= 3 && colons >= 1)
		return true;
	
	return false;
} */


stock GetDotXY(Float:StartPosX, Float:StartPosY, &Float:NewX, &Float:NewY, Float:alpha, Float:dist)
{
	 NewX = StartPosX + (dist * floatsin(alpha, degrees));
	 NewY = StartPosY + (dist * floatcos(alpha, degrees));
}
