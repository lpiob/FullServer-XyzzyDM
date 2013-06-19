/*----------------------------------------------------------------------------*-
  			      =WERSJA ZMODYFIKOWANA PREZEZ EIDERA=
					  ====================================
					   y_timers - Run timers efficiently. 
					  ====================================
Description:
	Sets up repeating timers without requiring any SetTimers and arranges them
	so that they will be very unlikely to meet (at least for a long time) using
	scheduling algorithms to get timers with the same period to be offset.
Legal:
	Version: MPL 1.1
	
	The contents of this file are subject to the Mozilla Public License Version 
	1.1 (the "License"); you may not use this file except in compliance with 
	the License. You may obtain a copy of the License at 
	http://www.mozilla.org/MPL/
	
	Software distributed under the License is distributed on an "AS IS" basis,
	WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
	for the specific language governing rights and limitations under the
	License.
	
	The Original Code is the SA:MP script information include.
	
	The Initial Developer of the Original Code is Alex "Y_Less" Cole.
	Portions created by the Initial Developer are Copyright (C) 2008
	the Initial Developer. All Rights Reserved.
	
	Contributors:
		ZeeX, koolk
	
	Thanks:
		Peter, Cam - Support.
		ZeeX - Very productive conversations.
		koolk - IsPlayerinAreaEx code.
		TheAlpha - Danish translation.
		breadfish - German translation.
		Fireburn - Dutch translation.
		yom - French translation.
		50p - Polish translation.
		Zamaroht - Spanish translation.
		Dracoblue, sintax, mabako, Xtreme, other coders - Producing other modes
			for me to strive to better.
		Pixels^ - Running XScripters where the idea was born.
		Matite - Pestering me to release it and using it.
	
	Very special thanks to:
		Thiadmer - PAWN.
		Kye/Kalcor - SA:MP.
		SA:MP Team past, present and future - SA:MP.
Version:
	1.0
Changelog:
	26/10/10:
		Officially added simple calling.
		Added "delay" functions.
	12/10/10:
		Rewrote for YSI 1.0 using y_scripting.
	11/08/07:
		Removed millions of defines to reduce pre-processing.
		Added pickups.
	03/08/07:
		First version.
-*----------------------------------------------------------------------------*/

native O@(const function[], time, repeat, const format[], {Float, File, Bit, PlayerText3D, Text, Text3D, Menu, DB, DBResult, Style, XML, Bintree, Group, _}:...) = SetTimerEx;

#define Timer:%0[%1](%2) forward @yT_%1_%0();stock %0() @yT_%1_%0();@yT_%1_%0()

#define Delay:%0[%1,%2](%3) stock %0(%3)return O@(#@#%0,%1,0,#%2#x,%3);stock v@%0(_:_o@,%3)return O@(#@#%0,_o@,0,#%2#x,%3);forward @%0(%3);@%0(%3)

#define FixDelay:%0[%1,%2](%3) stock %0(%3)return O@("@_"#%0,%1,0,#%2#x,%3);forward @_%0(%3);@_%0(%3)

#define Scripting_FastString(%1,%2,%3,%4) \
	(((%1) << 0) | ((%2) << 8) | ((%3) << 16) | ((%4) << 24))

#define skip:%0(%3) @%0(%3)
#define delay:%0[%1](%3) v@%0(_:%1,%3)

// This defines the number of different periods that timers can have.  This
// number is a HUGE over-estimate, you would need to have over 256 timers, none
// of them with the same period, for this number to be too small!
#define MAX_TIMER_VARIATIONS            (256)

forward Timer_Start(timer, delay);

/*----------------------------------------------------------------------------*-
Function:
	OnScriptInit
Params:
	-
Return:
	-
Notes:
	-
-*----------------------------------------------------------------------------*/

#if defined FILTERSCRIPT
	public OnFilterScriptInit()
#else
	public OnGameModeInit()
#endif
{
	new
		buffer[32],
		idx = 0,
		timers[MAX_TIMER_VARIATIONS][2],//[3],
		periods = 0;
	// 0 = time, 1 = count, 2 = offset.
	// First loop - get all the different periods.
	// Get the list of timers from the list of publics.
	while ((idx = Scripting_GetPublicFast(idx, buffer, (Scripting_FastString('@', 'y', 'T', '_')))))
	{
		// Get the time associated with the timer.  We know this starts at index
		// position 4, because we always set it to be there.
		new
			time = strval(buffer[4]);
		if (time)
		{
			// Valid time, find this in the array.
			for (new i = 0; ; ++i)
			{
				if (i == periods)
				{
					timers[i][0] = time;
					timers[i][1] = 1;
					++periods;
					break;
				}
				else if (timers[i][0] == time)
				{
					++timers[i][1];
					break;
				}
			}
			if (periods == MAX_TIMER_VARIATIONS)
			{
				break;
			}
		}
	}
	// Now we know how many of each period there are we can try arrange them so
	// that they execute at very different times.
	// [1] contains the total number of timers on similar periods.
	for (new i = 0; i != periods; ++i)
	{
		// First calculate the gap between the timers.
		new
			time = timers[i][0],
			offset = time / timers[i][1];
		// Now start all the timers with this time at that offset.
		idx = 0;
		new
			last = 0,
			curo = offset;
		while ((idx = Scripting_GetPublicFast(idx, buffer, (Scripting_FastString('@', 'y', 'T', '_')))))
		{
			if (strval(buffer[4]) == time)
			{
				// That's the old start code, which uses 7ms offsets to try get
				// as close as possible to different server frames (5ms).
				SetTimerEx("Timer_Start", curo + (random(14) - 7), 0, "ii", last, time);
				//SetTimerEx("Timer_Start", curo, 0, "ii", last, time);
				curo += offset;
			}
			// So that the first found timer in the next function is correct.
			last = idx;
		}
	}
	CallLocalFunction("Timers_OnScriptInit", "");
}

#if defined FILTERSCRIPT
	#if defined _ALS_OnFilterScriptInit
		#undef OnFilterScriptInit
	#else
		#define _ALS_OnFilterScriptInit
	#endif
	#define OnFilterScriptInit Timers_OnScriptInit
#else
	#if defined _ALS_OnGameModeInit
		#undef OnGameModeInit
	#else
		#define _ALS_OnGameModeInit
	#endif
	#define OnGameModeInit Timers_OnScriptInit
#endif

forward Timers_OnScriptInit();

/*----------------------------------------------------------------------------*-
Function:
	Timer_Start
Params:
	timer - Approximate public function index.
	delay - How often it's called.
Return:
	-
Notes:
	Only needed for more than one timer.  Offsets calls from each other for
	better server load distribution.
-*----------------------------------------------------------------------------*/

public Timer_Start(timer, delay)
{
	new
		buffer[32];
	Scripting_GetPublicFast(timer, buffer, (Scripting_FastString('@', 'y', 'T', '_')));
	SetTimer(buffer, delay, 1);
}

/*----------------------------------------------------------------------------*-
Function:
	Scripting_GetPublicFast
Params:
	idx - Index of the public to start at.
	buffer[32] - Buffer to store the name in.
	search - Pattern matching parameter.
Return:
	Index of next public.
Notes:
	Gets a string starting with a known 4 character sequence.

native Scripting_GetPublicFast(idx, buffer[32], search);

-*----------------------------------------------------------------------------*/

stock Scripting_GetPublicFast(idx, buffer[32], search)
{
	new
		count = Scripting_GetPublicsCount(),
		base = Scripting_GetBase(),
		pos = idx * 8 + 4 + Scripting_GetPublics(),
		addr;
	while (idx < count)
	{
		// Get the offset in the public records.
		// Get the pointer to the string.
		//pos += publics;
		// Get the address of the string.
		#emit LREF.S.pri                pos
		#emit STOR.S.pri                addr
		//printf("%s", Scripting_GetString(Scripting_GetBase() + pos));
		//new
		//	str[32] = Scripting_GetString(base + pos));
		++idx;
		addr += base;
		new
			chars;
		#emit LREF.S.pri                addr
		#emit STOR.S.pri                chars
		if (chars == search)
		{
			buffer = Scripting_GetStringFast(addr, chars);
			return idx;
		}
		pos += 8;
	}
	buffer[0] = '\0';
	return 0;
}

/*----------------------------------------------------------------------------*-
Function:
	Scripting_GetPublicsCount
Params:
	-
Return:
	Number of public functions in a script.
Notes:
	-

native Scripting_GetPublicsCount();

-*----------------------------------------------------------------------------*/

stock Scripting_GetPublicsCount()
{
	static
		s_count;
	if (!s_count)
	{
		// Get the difference in pointers.
		s_count = (Scripting_GetNatives() - Scripting_GetPublics()) / 8;
	}
	return s_count;
}

//

static stock Scripting_GetStringFast(addr, buffer)
{
	new
		str[32],
		ch,
		idx;
	do
	{
		// Write a character to the buffer.
		ch = buffer & 0xFF;
		str[idx++] = ch;
		// Check for the end of the string.
		if (!ch) break;
		// Write a character to the buffer.
		ch = buffer >> 8 & 0xFF;
		str[idx++] = ch;
		// Check for the end of the string.
		if (!ch) break;
		// Write a character to the buffer.
		ch = buffer >> 16 & 0xFF;
		str[idx++] = ch;
		// Check for the end of the string.
		if (!ch) break;
		// Write a character to the buffer.
		ch = buffer >>> 24;
		str[idx++] = ch;
		// Check for the end of the string.
		if (!ch) break;
		// Get the next address.
		addr += 4;
		// Read 4 bytes.
		#emit LREF.S.pri                addr
		#emit STOR.S.pri                buffer
	}
	while (idx < 32);
	return str;
}

static stock Scripting_GetBase()
{
	static
		s_addr;
	if (!s_addr)
	{
		// Get the data offset.
		#emit LCTRL                     1
		#emit STOR.pri                  s_addr
		// Invert to get the prefix offset relative to the data.
		s_addr = -s_addr;
	}
	return s_addr;
}

static stock Scripting_GetPublics()
{
	static
		s_addr;
	if (!s_addr)
	{
		new
			addr;// = Scripting_GetBase();
		// Get the data offset.
		#emit LCTRL                     1
		#emit STOR.S.pri                addr
		// Invert to get the prefix offset relative to the data.
		addr = -addr;
		// Get the pointer to the publics.
		addr += 32;
		#emit LREF.S.pri                addr
		#emit STOR.pri                  s_addr
		// Adjust to get the relative offset.
		s_addr = s_addr + addr - 32;
	}
	return s_addr;
}

static stock Scripting_GetNatives()
{
	static
		s_addr;
	if (!s_addr)
	{
		new
			addr;// = Scripting_GetBase();
		// Get the data offset.
		#emit LCTRL                     1
		#emit STOR.S.pri                addr
		// Invert to get the prefix offset relative to the data.
		addr = -addr;
		// Get the pointer to the publics.
		addr += 36;
		#emit LREF.S.pri                addr
		#emit STOR.pri                  s_addr
		// Adjust to get the relative offset.
		s_addr = s_addr + addr - 36;
	}
	return s_addr;
}
