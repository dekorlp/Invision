/////////////////////////////////////////////////////////////////////////////
// Name:	  TimeSpan
// Purpose:	  offers TimeSpan operations
// Author:	  Dennis Koehler
// Created:	  2014-07-14
// Copyright: Copyright (c) 2014, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef TIMESPAN_H
#define TIMESPAN_H

class INVISION_API TimeSpan
{
public:
	TimeSpan();
	TimeSpan(uint64 milliseconds);
	TimeSpan(uint32 hour, uint32 minute, uint32 seconds);
	TimeSpan(uint32 days, uint32 hour, uint32 minute, uint32 seconds);
	TimeSpan(uint32 days, uint32 hour, uint32 minute, uint32 seconds, uint32 milliseconds);
	TimeSpan(DateTime begin, DateTime end);
private:

	uint64 mMiliDays;
	uint64 mMiliHours;
	uint64 mMiliMinutes;
	uint64 mMiliSeconds;
	//real mMilliseconds;

	uint64 mDays;
	uint64 mHours;
	uint64 mMinutes;
	uint64 mSeconds;
	uint64 mMilliseconds;

	real mTotalDays;
	real mTotalHours;
	real mTotalMinutes;
	real mTotalSeconds;
	real mTotalMilliseconds;
};

#endif // TIMESPAN_H