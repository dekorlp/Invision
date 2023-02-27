/////////////////////////////////////////////////////////////////////////////
// Name:	  StopWatch
// Purpose:	  offers functions to start/stop time and read in miliseconds
// Author:	  Dennis Koehler
// Created:	  2014-10-23
// Copyright: Copyright (c) 2014, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef STOPWATCH_H
#define STOPWATCH_H

#include "TimeSpan.h"
namespace Invision
{
	class INVISION_API StopWatch
	{
	public:
		StopWatch();
		void start();
		void stop();
		bool isRunning();
		uint64 getElapsedSeconds();
		uint64 getElapsedMilliseconds();
		TimeSpan getElapsedTimespan();
	private:
		uint64 mMeasuredSeconds;
		uint64 mMeasuredMilliseconds;
		uint64 mMeasuresStartTime;
		TimeSpan mMeasuredTimespan;
		bool mIsRunning;
	};
}
#endif // STOPWATCH_H
