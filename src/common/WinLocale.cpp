#include "precompiled.h"

uint32 WinLocale::IdentifyCID()
{
	// ISO Standard 3166 code (DE, US, etc)
	LCID id = GetSystemDefaultLCID();
	return id;
}


uint32 WinLocale::IdentifyCID(WCHAR *langISO3166)
{
	// ISO Standard 3166 code (DE, US, etc)
	LCID id = LocaleNameToLCID(langISO3166, 0);
	return id;
}

const char* WinLocale::GetCountry()
{

	GEOID GeoId = GetUserGeoID(GEOCLASS_NATION);
	char *geoCountry  = new char[256];
	malloc(sizeof(geoCountry));
	GetGeoInfo(GeoId, GEO_ISO2, geoCountry, sizeof(geoCountry), GetUserDefaultLangID());
	return geoCountry;
}

const char* WinLocale::GetCountry(WCHAR *langISO3166)
{
	// ISO Standard 3166 code (DE, US, etc)
	char *geoCountry = new char[256];
	malloc(sizeof(geoCountry));
	GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SISO3166CTRYNAME, geoCountry, sizeof(geoCountry));
	return geoCountry;
}

const char* WinLocale::IdentifySystemLanguage()
{
	// ISO Standard 639 code (de, en, etc)
	char *systemLanguage = new char[256];
	GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SISO639LANGNAME, systemLanguage, sizeof(systemLanguage));
	return systemLanguage;
}

const char* WinLocale::IdentifyLanguage(uint32 langId)
{
	char *language = new char[256];
	GetLocaleInfo(langId, LOCALE_SISO639LANGNAME, language, sizeof(language));
	return language;
}