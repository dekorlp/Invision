#include "precompiled.h"

TimeSpan::TimeSpan()
{

}

TimeSpan::TimeSpan(uint64 milliseconds)
{
	mMilliseconds = milliseconds;
	mMiliSeconds = 0;
	mMiliMinutes = 0;
	mMiliHours = 0;
	mMiliDays = 0;

	mTotalMilliseconds = real(mMilliseconds + mMiliSeconds + mMiliMinutes + mMiliHours + mMiliDays);
	mTotalSeconds = mTotalMilliseconds / 1000;
	mTotalMinutes = mTotalSeconds / 60;
	mTotalHours = mTotalMinutes / 60;
	mTotalDays = mTotalHours / 24;

	mMilliseconds = (int)mTotalMilliseconds * 1000 % 1000;
	mSeconds = (int)mTotalSeconds % 60;
	mMinutes = (int)mTotalMinutes % 60;
	mHours = (int)mTotalHours % 24;
	mDays = (int)mTotalDays;
}

TimeSpan::TimeSpan(uint32 hour, uint32 minute, uint32 seconds)
{
	mMilliseconds = 0;
	mMiliSeconds = (uint64)seconds * 1000;
	mMiliMinutes = (uint64)minute * 60 * 1000;
	mMiliHours = (uint64)hour * 60 * 60 * 1000;
	mMiliDays = 0;

	mTotalMilliseconds = real(mMilliseconds + mMiliSeconds + mMiliMinutes + mMiliHours + mMiliDays);
	mTotalSeconds = mTotalMilliseconds / 1000;
	mTotalMinutes = mTotalSeconds / 60;
	mTotalHours = mTotalMinutes / 60;
	mTotalDays = mTotalHours / 24;

	mMilliseconds = (int)mTotalMilliseconds * 1000 % 1000;
	mSeconds = (int)mTotalSeconds % 60;
	mMinutes = (int)mTotalMinutes % 60;
	mHours = (int)mTotalHours % 24;
	mDays = (int)mTotalDays;

}

TimeSpan::TimeSpan(uint32 days, uint32 hour, uint32 minute, uint32 seconds)
{
	mMilliseconds = 0;
	mMiliSeconds = (uint64)seconds * 1000;
	mMiliMinutes = (uint64)minute * 60 * 1000;
	mMiliHours = (uint64)hour * 60 * 60 * 1000;
	mMiliDays = (uint64)days * 24 * 60 * 60 * 1000;

	mTotalMilliseconds = real(mMilliseconds + mMiliSeconds + mMiliMinutes + mMiliHours + mMiliDays);
	mTotalSeconds = mTotalMilliseconds / 1000;
	mTotalMinutes = mTotalSeconds / 60;
	mTotalHours = mTotalMinutes / 60;
	mTotalDays = mTotalHours / 24;

	mMilliseconds = (int)mTotalMilliseconds * 1000 % 1000;
	mSeconds = (int)mTotalSeconds % 60;
	mMinutes = (int)mTotalMinutes % 60;
	mHours = (int)mTotalHours % 24;
	mDays = (int)mTotalDays;
}

TimeSpan::TimeSpan(uint32 days, uint32 hour, uint32 minute, uint32 seconds, uint32 milliseconds)
{
	mMilliseconds = (uint64)milliseconds;
	mMiliSeconds = (uint64)seconds * 1000;
	mMiliMinutes = (uint64)minute * 60 * 1000;
	mMiliHours = (uint64)hour * 60 * 60 * 1000;
	mMiliDays = (uint64)days * 24 * 60 * 60 * 1000;

	mTotalMilliseconds = real(mMilliseconds + mMiliSeconds + mMiliMinutes + mMiliHours + mMiliDays);
	mTotalSeconds = mTotalMilliseconds / 1000;
	mTotalMinutes = mTotalSeconds / 60;
	mTotalHours = mTotalMinutes / 60;
	mTotalDays = mTotalHours / 24;

	mMilliseconds = (int)mTotalMilliseconds * 1000 % 1000;
	mSeconds = (int)mTotalSeconds % 60;
	mMinutes = (int)mTotalMinutes % 60;
	mHours = (int)mTotalHours % 24;
	mDays = (int)mTotalDays;

}

TimeSpan::TimeSpan(DateTime begin, DateTime end)
{
	mMilliseconds = (uint64)end.getTimestamp() * 1000 - (uint64)begin.getTimestamp() * 1000;
	mMiliSeconds = 0;
	mMiliMinutes = 0;
	mMiliHours = 0;
	mMiliDays = 0;

	mTotalMilliseconds = real(mMilliseconds + mMiliSeconds + mMiliMinutes + mMiliHours + mMiliDays);
	mTotalSeconds = mTotalMilliseconds / 1000;
	mTotalMinutes = mTotalSeconds / 60;
	mTotalHours = mTotalMinutes / 60;
	mTotalDays = mTotalHours / 24;

	mMilliseconds = (int)mTotalMilliseconds * 1000 % 1000;
	mSeconds = (int)mTotalSeconds % 60;
	mMinutes = (int)mTotalMinutes % 60;
	mHours = (int)mTotalHours % 24;
	mDays = (int)mTotalDays;


	int16 yearOld = begin.getYear();

	int16 yearNew = end.getYear();

	int8 monthNew = end.getMonth();

	int64 LeapDays = 0;

	for (int16 i = yearOld; i <= yearNew; ++i)
	{
		if (i % 4 == 0 && (i % 100 != 0 || i % 400 == 0))
		if (i != yearNew + 1971 || monthNew > 2)
			LeapDays++; /* +leap Day if year is leapYear */
	}
	mDays = mDays - LeapDays - 1;
}