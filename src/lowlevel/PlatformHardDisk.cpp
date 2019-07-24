#include "precompiled.h"
#include "PlatformHardDisk.h"

	uint32 PlatformHardDisk::CalculateTotalSpace(uint8 drive)
	{
		struct _diskfree_t diskfree;
		_getdiskfree(drive, &diskfree);
		uint32 size = (diskfree.total_clusters / 1024)*diskfree.sectors_per_cluster*diskfree.bytes_per_sector;
		return (size / 1024);

	}
	uint32 PlatformHardDisk::CalculateFreeSpace(uint8 drive)
	{
		struct _diskfree_t diskfree;
		_getdiskfree(drive, &diskfree);
		uint32 size = (diskfree.avail_clusters / 1024)*diskfree.sectors_per_cluster*diskfree.bytes_per_sector;
		return (size / 1024);

	}
