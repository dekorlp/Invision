/////////////////////////////////////////////////////////////////////////////
// Name:	  DateTime
// Purpose:	  offers Datetime operations
// Author:	  Dennis Koehler
// Created:	  2014-06-24
// Copyright: Copyright (c) 2014, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////



#ifndef DATETIME_H
#define DATETIME_H

enum UTC {
	UTCP12 = 12,
	UTCP11 = 11,
	UTCP10 = 10,
	UTCP09 = 9,
	UTCP08 = 8,
	UTCP07 = 7,
	UTCP06 = 6,
	UTCP05 = 5,
	UTCP04 = 4,
	UTCP03 = 3,
	UTCP02 = 2,
	UTCP01 = 1,
	UTCP00 = 0,
	UTCM01 = -1,
	UTCM02 = -2,
	UTCM03 = -3,
	UTCM04 = -4,
	UTCM05 = -5,
	UTCM06 = -6,
	UTCM07 = -7,
	UTCM08 = -8,
	UTCM09 = -9,
	UTCM10 = -10,
	UTCM11 = -11,
	UTCM12 = -12,
};

struct LocalDateTimeTable
{
	int32 year;
	int32 dayOfYear;
	int32 month;
	int32 dayOfMonth;
	int32 dayOfWeek;
	int32 hour;
	int32 minute;
	int32 second;

	bool isLeapYear;
};

class HAL_API DateTime
{
public:
	DateTime();
	DateTime(int32 year, int32 month, int32 day);
	DateTime(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second);

	void setDateTimeToday();
	void setDateTime(int32 year, int32 month, int32 day);
	void setDateTime(int32 year, int32 month, int32 day, int32 hour, int32 minute, int32 second);

	void addDays(uint8 days);
	void subDays(uint8 days);
	void addMonths(uint8 months);
	void subMonths(uint8 months);
	void addYears(uint8 years);
	void subYears(uint8 years);

	void addHours(uint32 hours);
	void subHours(uint32 hours);
	void addMinutes(uint32 minutes);
	void subMinutes(uint32 minutes);
	void addSeconds(uint32 seconds);
	void subSeconds(uint32 seconds);

	int getTimeZone();
	int64 getTimestamp();

	uint8 getSeconds();
	uint8 getMinutes();
	uint8 getHour();
	uint8 getMonth();
	uint16 getYear();
	uint8 getDayOfWeek();
	uint16 getDayOfYear();

	uint8 getSeconds(UTC utc);
	uint8 getMinutes(UTC utc);
	uint8 getHour(UTC utc);
	uint8 getMonth(UTC utc);
	uint16 getYear(UTC utc);
	uint8 getDayOfWeek(UTC utc);
	uint16 getDayOfYear(UTC utc);

	LocalDateTimeTable getDateTimeTable();
	LocalDateTimeTable getDateTimeTable(UTC utc);

	std::string ToString(std::string FormatProvider);
	std::string ToString(std::string FormatProvider, UTC utc);
private:


	int64 timestamp;

	int64 addLeapDays(int64 timestamp1, int64 timestamp2);
	LocalDateTimeTable GetLocalTime(tm *timestamp);
	std::string printDate(std::string FormatProvider, LocalDateTimeTable dateTimeTable);
};

#endif // QUA_DATETIME