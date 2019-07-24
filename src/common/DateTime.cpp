#include "precompiled.h"

// TODO test components with testplan !!!
DateTime::DateTime()
{

}

DateTime::DateTime(int32 year, int32 month, int32 day)
{
	setDateTime(year, month, day);
}

void DateTime::setDateTime(int32 year, int32 month, int32 day)
{
	const short days_since_beginOfYear[12] = /* Days since beginning of the new year without days of actual month and without leap Day */
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

	long unix_time;
	long years = year - 1970;
	int leapYear = ((year - 1) - 1968) / 4 - ((year - 1) - 1900) / 100 + ((year - 1) - 1600) / 400;

	unix_time = 0 + 60 * 0 + 60 * 60 * 0 +
		(days_since_beginOfYear[month - 1] + day - 1) * 60 * 60 * 24 +
		(years * 365 + leapYear) * 60 * 60 * 24;

	if ((month>2) && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)))
		unix_time += 60 * 60 * 24; /* +leap Day if year is leapYear */
	this->timestamp = unix_time;
}

DateTime::DateTime(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second)
{
	setDateTime(year, month, day, hour, minute, second);
}

void DateTime::setDateTime(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second)
{
	const short days_since_beginOfYear[12] = /* Days since beginning of the new year without days of actual month and without leap Day */
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

	long unix_time;
	long years = year - 1970;
	int leapYear = ((year - 1) - 1968) / 4 - ((year - 1) - 1900) / 100 + ((year - 1) - 1600) / 400;

	unix_time = second + 60 * minute + 60 * 60 * hour +
		(days_since_beginOfYear[month - 1] + day - 1) * 60 * 60 * 24 +
		(years * 365 + leapYear) * 60 * 60 * 24;

	if ((month>2) && (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)))
		unix_time += 60 * 60 * 24; /* +leap Day if year is leapYear */
	this->timestamp = unix_time;
}

void DateTime::addDays(uint8 days)
{
	int64 tempTimestamp = this->timestamp;
	this->timestamp += (days * 60 * 60 * 24);
	this->timestamp += addLeapDays(tempTimestamp, this->timestamp);
}

void DateTime::subDays(uint8 days)
{
	int64 tempTimestamp = this->timestamp;
	this->timestamp -= (days * 60 * 60 * 24);
	this->timestamp += addLeapDays(this->timestamp, tempTimestamp);
}


void DateTime::addMonths(uint8 months)
{
	int64 tempTimestamp = this->timestamp;
	int64 monthsInSeconds = 0;
	int64 month = (this->timestamp + 1970) / 60 / 60 / 24 / 365 / 12;
	const short days_since_beginOfYear[12] = /* Days of every month without leap Day since new year */
		// 1   2   3   4   5   6   7   8   9  10  11  12 //
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	uint8 monthIndex = 0;
	uint64 currentmonth = month;
	while (monthIndex <= months)
	{
		monthsInSeconds += days_since_beginOfYear[currentmonth];
		monthIndex++;
		currentmonth++;
		if (currentmonth == 12)
		{
			currentmonth = 0;
		}
	}
	this->timestamp += (monthsInSeconds * 60 * 60 * 24);
	this->timestamp += addLeapDays(tempTimestamp, this->timestamp);
}

void DateTime::subMonths(uint8 months)
{
	int64 tempTimestamp = this->timestamp;
	int64 monthsInSeconds = 0;
	int64 month = (this->timestamp + 1970) / 60 / 60 / 24 / 365 / 12;
	const short days_since_beginOfYear[12] = /* Days of every month without leap Day since new year */
		// 1   2   3   4   5   6   7   8   9  10  11  12 //
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	uint8 monthIndex = 0;
	uint64 currentmonth = month;
	while (monthIndex <= months)
	{
		monthsInSeconds += days_since_beginOfYear[currentmonth];
		monthIndex++;
		currentmonth--;
		if (currentmonth == 0)
		{
			currentmonth = 11;
		}
	}
	this->timestamp -= (monthsInSeconds * 60 * 60 * 24);
	this->timestamp -= addLeapDays(this->timestamp, tempTimestamp);
}


void DateTime::addYears(uint8 years)
{
	int64 tempTimestamp = this->timestamp;
	this->timestamp += (years * 365 * 24 * 60 * 60);
	this->timestamp += addLeapDays(tempTimestamp, this->timestamp);
}

void DateTime::subYears(uint8 years)
{
	int64 tempTimestamp = this->timestamp;
	this->timestamp -= (years * 365 * 24 * 60 * 60);
	this->timestamp -= addLeapDays(this->timestamp, tempTimestamp);
}

void DateTime::addHours(uint32 hours)
{
	int64 tempTimestamp = this->timestamp;
	this->timestamp += (hours * 60 * 60);
	this->timestamp += addLeapDays(tempTimestamp, this->timestamp);
}

void DateTime::subHours(uint32 hours)
{
	int64 tempTimestamp = this->timestamp;
	this->timestamp -= (hours * 60 * 60);
	this->timestamp -= addLeapDays(this->timestamp, tempTimestamp);
}

void DateTime::addMinutes(uint32 minutes)
{
	int64 tempTimestamp = this->timestamp;
	this->timestamp += (minutes * 60);
	this->timestamp += addLeapDays(tempTimestamp, this->timestamp);
}

void DateTime::subMinutes(uint32 minutes)
{
	int64 tempTimestamp = this->timestamp;
	this->timestamp -= (minutes * 60);
	this->timestamp -= addLeapDays(this->timestamp, tempTimestamp);
}

void DateTime::addSeconds(uint32 seconds)
{
	int64 tempTimestamp = this->timestamp;
	this->timestamp += seconds;
	this->timestamp += addLeapDays(tempTimestamp, this->timestamp);
}

void DateTime::subSeconds(uint32 seconds)
{
	int64 tempTimestamp = this->timestamp;
	this->timestamp -= seconds;
	this->timestamp -= addLeapDays(this->timestamp, tempTimestamp);
}


int64 DateTime::addLeapDays(int64 timestamp1, int64 timestamp2)
{
	int64 yearOld = timestamp1 / 60 / 60 / 24 / 365;

	int64 yearNew = timestamp2 / 60 / 60 / 24 / 365;
	int8 monthNew = timestamp2 / 60 / 60 / 24 / 365 % 12;

	int64 LeapDays = 0;

	for (int64 i = yearOld + 1970; i <= yearNew + 1970; ++i)
	{
		if (i % 4 == 0 && (i % 100 != 0 || i % 400 == 0))
		if (i != yearNew + 1971 || monthNew > 2)
			LeapDays += 60 * 60 * 24; /* +leap Day if year is leapYear */
	}
	return LeapDays;
}

void DateTime::setDateTimeToday()
{
	time(&this->timestamp);
}

int DateTime::getTimeZone()
{
	time_t now;
	struct tm l, g;
	int dt;

	time(&now);
	l = *localtime(&now);
	g = *gmtime(&now);

	dt = (int)difftime(now, mktime(&g));
	dt = dt / 3600;

	return dt;
}

LocalDateTimeTable DateTime::GetLocalTime(tm *timestamp)
{
	LocalDateTimeTable table;
	table.year = timestamp->tm_year + 1900;
	table.month = timestamp->tm_mon + 1;
	table.dayOfYear = timestamp->tm_yday;
	table.dayOfMonth = timestamp->tm_mday;
	table.dayOfWeek = timestamp->tm_wday;
	table.hour = timestamp->tm_hour;
	table.minute = timestamp->tm_min;
	table.second = timestamp->tm_sec;
	return table;
}

LocalDateTimeTable DateTime::getDateTimeTable()
{
	LocalDateTimeTable dateTimeTable = GetLocalTime(localtime(&timestamp));
	int8 zone = getTimeZone();

	DateTime *tempDateTime = new DateTime(dateTimeTable.year, dateTimeTable.month, dateTimeTable.dayOfMonth, dateTimeTable.hour, dateTimeTable.minute, dateTimeTable.second);
	int64 tempTimestamp = tempDateTime->getTimestamp();
	dateTimeTable = tempDateTime->GetLocalTime(localtime(&tempTimestamp));
	delete tempDateTime;

	return dateTimeTable;
}

LocalDateTimeTable DateTime::getDateTimeTable(UTC utc)
{
	LocalDateTimeTable dateTimeTable = GetLocalTime(gmtime(&timestamp));
	int8 zone = static_cast<UTC>(utc);
	DateTime *tempDateTime = new DateTime(dateTimeTable.year, dateTimeTable.month, dateTimeTable.dayOfMonth, dateTimeTable.hour, dateTimeTable.minute, dateTimeTable.second);
	if (zone > 0)
	{
		tempDateTime->addHours(zone);
	}
	else
	{
		tempDateTime->subHours(zone);
	}
	int64 tempTimestamp = tempDateTime->getTimestamp();
	dateTimeTable = tempDateTime->GetLocalTime(localtime(&tempTimestamp));
	delete tempDateTime;

	return dateTimeTable;
}

uint8 DateTime::getSeconds()
{
	return getDateTimeTable().second;
}

uint8 DateTime::getMinutes()
{
	return getDateTimeTable().minute;
}

uint8 DateTime::getHour()
{
	return getDateTimeTable().hour;
}

uint8 DateTime::getMonth()
{
	return getDateTimeTable().month;
}

uint16 DateTime::getYear()
{
	return getDateTimeTable().year;
}

uint8 DateTime::getDayOfWeek()
{
	return getDateTimeTable().dayOfWeek;
}

uint16 DateTime::getDayOfYear()
{
	return getDateTimeTable().dayOfYear;
}


uint8 DateTime::getSeconds(UTC utc)
{
	return getDateTimeTable(utc).second;
}

uint8 DateTime::getMinutes(UTC utc)
{
	return getDateTimeTable(utc).minute;
}

uint8 DateTime::getHour(UTC utc)
{
	return getDateTimeTable(utc).hour;
}


uint8 DateTime::getMonth(UTC utc)
{
	return getDateTimeTable(utc).month;
}

uint16 DateTime::getYear(UTC utc)
{
	return getDateTimeTable(utc).year;
}

uint8 DateTime::getDayOfWeek(UTC utc)
{
	return getDateTimeTable(utc).dayOfWeek;
}

uint16 DateTime::getDayOfYear(UTC utc)
{
	return getDateTimeTable(utc).dayOfYear;
}

int64 DateTime::getTimestamp()
{
	return timestamp;
}

std::string DateTime::ToString(std::string FormatProvider)
{
	// UTC = LOCALTIME
	return(printDate(FormatProvider, getDateTimeTable()));
}

std::string DateTime::ToString(std::string FormatProvider, UTC utc)
{
	return(printDate(FormatProvider, getDateTimeTable(utc)));
}

std::string DateTime::printDate(std::string FormatProvider, LocalDateTimeTable dateTimeTable)
{

	std::stringstream dateTime;
	const std::string formatPatterns[] = { "d", "dd", "ddd", "dddd", "M", "MM", "MMM", "MMMM", "y", "yy", "yyy", "yyyy", "gg", "h",
		"hh", "H", "HH", "m", "mm", "s", "ss", "t", "tt", "z", "zz", "zzz" };
	std::string selectedLetter;
	bool isLetterSelected;

	for (uint16 i = 0; i < FormatProvider.length(); i++)
	{
		selectedLetter = FormatProvider[i];
		while (FormatProvider[i + 1] == selectedLetter[0])
		{
			selectedLetter.append(FormatProvider.substr(i, 1));
			i = i + 1;
		}

		for (uint16 j = 0; j <(sizeof(formatPatterns) / sizeof(std::string)); j++)
		{
			if (formatPatterns[j] == selectedLetter)
			{
				switch (j)
				{
				case 0: // d
					dateTime << dateTimeTable.dayOfMonth;
					break;
				case 1: // dd
					if (dateTimeTable.dayOfMonth < 10) { dateTime << "0"; dateTime << dateTimeTable.dayOfMonth; }
					else { dateTime << dateTimeTable.dayOfMonth; }
					break;
				case 2: // ddd
					switch (dateTimeTable.dayOfWeek)
					{
					case 1:
						dateTime << "Mon";
						break;
					case 2:
						dateTime << "Tue";
						break;
					case 3:
						dateTime << "Wed";
						break;
					case 4:
						dateTime << "Thu";
						break;
					case 5:
						dateTime << "Fri";
						break;
					case 6:
						dateTime << "Sat";
						break;
					case 7:
						dateTime << "Sun";
						break;
					}
					break;
				case 3: // dddd
					switch (dateTimeTable.dayOfWeek)
					{
					case 1:
						dateTime << "Monday";
						break;
					case 2:
						dateTime << "Tuesday";
						break;
					case 3:
						dateTime << "Wednesdy";
						break;
					case 4:
						dateTime << "Thursday";
						break;
					case 5:
						dateTime << "Friday";
						break;
					case 6:
						dateTime << "Saturday";
						break;
					case 7:
						dateTime << "Sunday";
						break;
					}
					break;
				case 4: // M
					dateTimeTable.month;
					break;
				case 5: // MM
					if (dateTimeTable.month < 10) { dateTime << "0"; dateTime << dateTimeTable.month; }
					else { dateTime << dateTimeTable.month; }
					break;
				case 6: // MMM
					switch (dateTimeTable.month)
					{
					case 1:
						dateTime << "Jan";
						break;
					case 2:
						dateTime << "Feb";
						break;
					case 3:
						dateTime << "Mar";
						break;
					case 4:
						dateTime << "Apr";
						break;
					case 5:
						dateTime << "Mai";
						break;
					case 6:
						dateTime << "Jun";
						break;
					case 7:
						dateTime << "Jul";
						break;
					case 8:
						dateTime << "Aug";
						break;
					case 9:
						dateTime << "Sep";
						break;
					case 10:
						dateTime << "Oct";
						break;
					case 11:
						dateTime << "Nov";
						break;
					case 12:
						dateTime << "Dec";
						break;
					}
					break;
				case 7: // MMMM
					switch (dateTimeTable.month)
					{
					case 1:
						dateTime << "January";
						break;
					case 2:
						dateTime << "February";
						break;
					case 3:
						dateTime << "March";
						break;
					case 4:
						dateTime << "April";
						break;
					case 5:
						dateTime << "Mai";
						break;
					case 6:
						dateTime << "June";
						break;
					case 7:
						dateTime << "July";
						break;
					case 8:
						dateTime << "August";
						break;
					case 9:
						dateTime << "September";
						break;
					case 10:
						dateTime << "October";
						break;
					case 11:
						dateTime << "November";
						break;
					case 12:
						dateTime << "December";
						break;
					}
					break;
				case 8: // y
					dateTime << dateTimeTable.year % 100;
					break;
				case 9: // yy
					if (dateTimeTable.year < 10) { dateTime << "0"; dateTime << dateTimeTable.year; }
					else { dateTime << dateTimeTable.year; }
					break;
				case 10: // yyy
					if (dateTimeTable.year < 10) { dateTime << "00"; dateTime << dateTimeTable.year; }
					else if (dateTimeTable.year < 100) { dateTime << "0";  dateTime << dateTimeTable.year; }
					else { dateTime << dateTimeTable.year; }
					break;
				case 11: // yyyy
					dateTime << dateTimeTable.year;
					break;
				case 12: // gg
					if (dateTimeTable.year > 0)
					{
						dateTime << "AD";
					}
					else
					{
						dateTime << "BC";
					}
					break;
				case 13: // h
					if (dateTimeTable.hour > 12)
					{
						dateTime << dateTimeTable.hour - 12;
					}
					else
					{
						dateTime << dateTimeTable.hour;
					}
					break;
				case 14: // hh
					if (dateTimeTable.hour > 12)
					{
						if ((dateTimeTable.hour - 12) < 10) { dateTime << "0"; dateTime << dateTimeTable.hour - 12; }
						else { dateTime << dateTimeTable.hour - 12; }
					}
					else
					{
						if (dateTimeTable.hour < 10) { dateTime << "0"; dateTime << dateTimeTable.hour; }
						else { dateTime << dateTimeTable.hour; }
					}
					break;
				case 15: // H
					dateTime << dateTimeTable.hour;
					break;
				case 16: // HH
					if (dateTimeTable.hour < 10) { dateTime << "0"; dateTime << dateTimeTable.hour; }
					else { dateTime << dateTimeTable.hour; }
					break;
				case 17: // m
					dateTimeTable.minute;
					break;
				case 18: // mm
					if (dateTimeTable.minute < 10) { dateTime << "0"; dateTime << dateTimeTable.minute; }
					else { dateTime << dateTimeTable.minute; }
					break;
				case 19: // s
					dateTime << dateTimeTable.second;
					break;
				case 20: // ss
					if (dateTimeTable.second < 10) { dateTime << "0"; dateTime << dateTimeTable.second; }
					else { dateTime << dateTimeTable.second; }
					break;
				case 21: // t
					if (dateTimeTable.hour <= 12)
					{
						dateTime << "A";
					}
					else
					{
						dateTime << "P";
					}
					break;
				case 22: // tt
					if (dateTimeTable.hour <= 12)
					{
						dateTime << "AM";
					}
					else
					{
						dateTime << "PM";
					}
					break;
				case 23: // z
					// TODO implement Windows Function
					break;
				case 24: // zz
					// TODO implement Windows Function
					break;
				case 25: // zzz
					// TODO implement Windows Function
					break;
				default:
					// look at if (isLetterSelected == false)
					break;
				}


				isLetterSelected = true;
				break;
			}
			else if (j == (sizeof(formatPatterns) / sizeof(std::string)))
			{
				isLetterSelected = false;
			}
		}

		if (isLetterSelected == false)
		{
			dateTime << selectedLetter;
		}

		isLetterSelected = false;
	}

	return dateTime.str();
}