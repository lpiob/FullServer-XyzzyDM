CMD:help(playerid)
{
	return cmd_pomoc(playerid);
}

CMD:register(playerid, params[])
{
	return cmd_rejestracja(playerid, params);
}

/*CMD:language(playerid)
{
	return cmd_zmienjezyk(playerid);
}*/

CMD:newnick(playerid, params[])
{
	return cmd_nowynick(playerid, params);
}

CMD:k(playerid, params[])
{
	return cmd_kick(playerid, params);
}


CMD:lastskin(playerid)
{
	return cmd_mojskin(playerid);
}

CMD:ms(playerid)
{
	return cmd_mojskin(playerid);
}


CMD:teleports(playerid)
{
	return cmd_teleporty(playerid);
}

CMD:stat(playerid)
{
	return cmd_staty(playerid);
}

CMD:stats(playerid)
{
	return cmd_staty(playerid);
}

CMD:toplista(playerid)
{
	return cmd_top(playerid);
}

CMD:toplist(playerid)
{
	return cmd_top(playerid);
}

CMD:wiez(playerid, params[])
{
	return cmd_jail(playerid, params);
}


CMD:zamknij(playerid)
{
	return cmd_lock(playerid);
}

CMD:otworz(playerid)
{
	return cmd_unlock(playerid);
}

CMD:zlecenie(playerid, params[])
{
	return cmd_hitman(playerid, params);
}

CMD:dajkase(playerid, params[])
{
	return cmd_givecash(playerid, params);
}

CMD:komendy(playerid)
{
	return cmd_cmd(playerid);
}

/*CMD:enter(playerid)
{
	return cmd_wejdz(playerid);
}

CMD:exit(playerid)
{
	return cmd_wyjdz(playerid);
}*/

/*CMD:openhouse(playerid)
{
	return cmd_otworzdom(playerid);
}

CMD:lockhouse(playerid)
{
	return cmd_zamknijdom(playerid);
}

CMD:previewhouse(playerid)
{
	return cmd_zobaczdom(playerid);
}

CMD:housecar(playerid)
{
	return cmd_autodom(playerid);
}

CMD:changecar(playerid)
{
	return cmd_zmienauto(playerid);
}

CMD:changecolor(playerid)
{
	return cmd_zmienkolor(playerid);
}

CMD:tphouse(playerid)
{
	return cmd_tpdom(playerid);
}
*/
CMD:tdpanel(playerid)
{
	return cmd_hud(playerid);
}

CMD:newpassword(playerid, params[])
{
	return cmd_zmienhaslo(playerid, params);
}

CMD:cz(playerid)
{
	return cmd_clearchat(playerid);
}

CMD:report(playerid, params[])
{
	return cmd_raport(playerid, params);
}

CMD:admini(playerid)
{
	return cmd_admini(playerid);
}

CMD:vtp(playerid, params[])
{
	return cmd_tp(playerid, params);
}

CMD:vipy(playerid)
{
	return cmd_vips(playerid);
}

CMD:authors(playerid)
	return cmd_authors(playerid);

CMD:author(playerid)
	return cmd_credits(playerid);

CMD:autor(playerid)
	return cmd_credits(playerid);

CMD:autorzy(playerid)
	return cmd_credits(playerid);

CMD:respect(playerid)
{
	return cmd_respekt(playerid);
}

CMD:church(playerid)
	return cmd_kosciol(playerid);

CMD:parachute(playerid)
	return cmd_spadochron(playerid);