/////////////////////////////////////////////////////////////////////////////
// Name:	  PlatformHardDisk
// Purpose:	  reads information of the HardDisk
// Author:	  Dennis Koehler
// Created:	  2014-06-18
// Copyright: Copyright (c) 2015, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef PLATFORMHARDDISC_H
#define PLATFORMHARDDISC_H

class INVISION_API PlatformHardDisk
{
private:

public:
	uint32 CalculateTotalSpace(uint8 drive);
	uint32 CalculateFreeSpace(uint8 drive);


};
#endif //PLATFORMHARDDISC_H