#include "precompiled.h"

StopWatch::StopWatch()
{

}

void StopWatch::start()
{
	mMeasuresStartTime = clock();
	mIsRunning = true;
}

void StopWatch::stop()
{
	mMeasuredMilliseconds = clock() - mMeasuresStartTime;
	mMeasuredSeconds = mMeasuredMilliseconds / CLOCKS_PER_SEC;
	mMeasuredTimespan = TimeSpan(mMeasuredMilliseconds);
	mIsRunning = false;
}

uint64 StopWatch::getElapsedSeconds()
{
	return (mIsRunning != true) ? mMeasuredSeconds : 0;
}

uint64 StopWatch::getElapsedMilliseconds()
{
	return (mIsRunning != true) ? mMeasuredMilliseconds : 0;
}

TimeSpan StopWatch::getElapsedTimespan()
{
	return (mIsRunning != true) ? mMeasuredTimespan : 0;
}

bool StopWatch::isRunning()
{
	return mIsRunning;
}