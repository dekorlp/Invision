#include "precompiled.h"

#include "PointerUtil.h"
#include "TrackingStruct.h"

#include "AdvancedMemoryTracking.h"

void AdvancedMemoryTracking::setMemoryTrackingTag(void* memory, int fileNumber, char *fileName, int offset)
{
	trackingStruct temp;
	temp.filename = fileName;
	temp.filenumber = fileNumber;

	trackingStruct* trackingDates = (trackingStruct*)PointerUtil::subtract(memory, offset);
	*trackingDates = temp;
}