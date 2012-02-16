#if defined USE_REGEX
#include "regex"


stock ContainsIP(const string[])
{
    static
        RegEx:rCIP,
		RegEx:rTOS;

    if ( !rCIP )
        rCIP = regex_build(".*[0-9]{1,3}[^0-9]{1,3}[0-9]{1,3}[^0-9]{1,3}[0-9]{1,3}[^0-9]{1,3}[0-9]{1,3}[^0-9]{1,7}[0-9]{3,5}.*");
	if ( !rTOS )
		rTOS = regex_build(".*( TOS| tos| ToS|theotherside|TheOtherSide|THEOTHERSIDE|THEotherside|Theotherside| Tos).*");

    if (regex_match_exid(string, rCIP)) return true;
	else if (regex_match_exid(string, rTOS)) return true;
	else return false;
}
#else

stock ContainsIP(const string[])
	return false;

#endif


