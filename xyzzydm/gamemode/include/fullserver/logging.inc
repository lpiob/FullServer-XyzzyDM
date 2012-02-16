
bool:OutputLog(logId, message[])
{
//	new
//	 buffer[MAX_LOG_STRING_LENGTH],
//	 buffer2[MAX_LOG_STRING_LENGTH+100];
//	mysql_real_escape_string(message, buffer);
//	format(buffer2, sizeof buffer2, "INSERT INTO fs_logs SET facility=%d,value='%s'", logId, buffer);
//	mysql_query(buffer2);
	new l_time[3];
	gettime(l_time[0], l_time[1], l_time[2]);
	printf("[%d-%02d:%02d:%02d] %s", logId, l_time[0], l_time[1], l_time[2], message);
	return true;
}



/*new logging_curSE;


logging_init(){
	for (new logId=0; logId<sizeof fh_logs; logId++)
		logging_openlogfile(logId);
	return true;
}

logging_openlogfile(logId,bool:ts=true) {
	new path[64], l_time[3],l_date[3], buffer[64];

	getdate(l_date[0],l_date[1],l_date[2]);
	gettime(l_time[0], l_time[1], l_time[2]);

	copy(PATH_logBase,path);
	switch(logId)
		{
			case LOG_SYSTEM: strcat(path,PATH_logSystem);
			case LOG_COMMANDS: strcat(path,PATH_logCmd);
			case LOG_CHAT: strcat(path,PATH_logChat);
			case LOG_PM: strcat(path,PATH_logPM);
			case LOG_OCHAT: strcat(path,PATH_logOtherChats);
			case LOG_ANN: strcat(path,PATH_logAnnounce);
			default: 
				strcat(path,"unknown");
	}

	format(path,sizeof path,"%s-%04d%02d.txt", path,l_date[0],l_date[1]);	// FullServer/log/commands-201104.txt
	fh_logs[logId] = fopen(path, io_append);
	if(!fh_logs[logId])
			printf("[IMPORTANT ERROR] Unable to open logfile %s",path);
	else
		logsFlushed[logId]=GetTickCount();

	if (ts) {
		format(buffer,sizeof buffer,"\n\nLOGFILE OPENED %04d%02d%02d %02d:%02d\n\n", l_date[0],l_date[1],l_date[2], l_time[0], l_time[1]);
		fwrite(fh_logs[logId], buffer);
	}
}


logging_exit(){
	for (new logId=0; logId<sizeof fh_logs; logId++)
		if (fh_logs[logId])
			fclose(fh_logs[logId]);
	return true;
}

logging_reload(bool:full=true){
	if (full) {
		printf("Reloading gamemode logfiles...");
		logging_exit();
		logging_init();
	} else {
		logging_curSE++;
		if (logging_curSE > sizeof fh_logs)
			logging_curSE=0;
		if (GetTickCount()-logsFlushed[logging_curSE]>(1000*60*5)) {
			fclose(fh_logs[logging_curSE]);
			logging_openlogfile(logging_curSE,false);
		}
	}
	return true;	
}

old outputlog
//	if (GetTickCount()-logsFlushed[logging_curSE]>(1000*60*5)) {
//		fclose(fh_logs[logId]);
//		logging_openlogfile(logId,false);
//	}

	if(!fh_logs[logId]) return false;
	if(displayDateAndTime)
	{
		getdate(l_date[2], l_date[1], l_date[0]);
		gettime(l_time[0], l_time[1], l_time[2]);
		format(buffer_dt, sizeof buffer_dt, "[%02i-%02i-%i %02i:%02i:%02i] ", l_date[0], l_date[1], l_date[2], l_time[0], l_time[1], l_time[2]);
	}
		
	format(buffer, sizeof buffer, "%s%s%s", buffer_dt, message, (breakNewLine) ? ("\r\n") : (""));
	
	fwrite(fh_logs[logId], buffer);
	
	return true;
*/

