/////////////////////////////////////////////////////////////////////////////
// Name:	  Exception
// Purpose:	  offers exception handling operations
// Author:	  Dennis Koehler
// Created:	  2015-09-27
// Copyright: Copyright (c) 2014, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef EXCEPTION_H
#define EXCEPTION_H
namespace Invision
{
	class invisionException
	{
	protected:
		const char* mHelpLink;
		const char* mMessage;
		const char* mSource;
	public:
		virtual const char* what() const
		{
			return mMessage;
		}
	};

	class invisionCoreException : protected invisionException
	{
	public:
		invisionCoreException();
		invisionCoreException(const char* message)
		{
			mMessage = message;
		}

		invisionCoreException(const char* message, const char* helpLink)
		{
			mMessage = message;
			mHelpLink = helpLink;
		}

		invisionCoreException(const char* message, const char* helpLink, const char* source)
		{
			mMessage = message;
			mHelpLink = helpLink;
			mSource = source;
		}
	};

	class invisionCoreArgumentException : protected  invisionException
	{
	public:
		invisionCoreArgumentException();
		invisionCoreArgumentException(const char* message)
		{
			mMessage = message;
		}

		invisionCoreArgumentException(const char* message, const char* helpLink)
		{
			mMessage = message;
			mHelpLink = helpLink;
		}

		invisionCoreArgumentException(const char* message, const char* helpLink, const char* source)
		{
			mMessage = message;
			mHelpLink = helpLink;
			mSource = source;
		}
	};

	class invisionCoreArgumentNullException : protected invisionException
	{
	public:
		invisionCoreArgumentNullException();
		invisionCoreArgumentNullException(const char* message)
		{
			mMessage = message;
		}

		invisionCoreArgumentNullException(const char* message, const char* helpLink)
		{
			mMessage = message;
			mHelpLink = helpLink;
		}

		invisionCoreArgumentNullException(const char* message, const char* helpLink, const char* source)
		{
			mMessage = message;
			mHelpLink = helpLink;
			mSource = source;
		}
	};

	class invisionCoreArgumentOutOfRangeException : protected invisionException
	{
	public:
		invisionCoreArgumentOutOfRangeException();
		invisionCoreArgumentOutOfRangeException(const char* message)
		{
			mMessage = message;
		}

		invisionCoreArgumentOutOfRangeException(const char* message, const char* helpLink)
		{
			mMessage = message;
			mHelpLink = helpLink;
		}

		invisionCoreArgumentOutOfRangeException(const char* message, const char* helpLink, const char* source)
		{
			mMessage = message;
			mHelpLink = helpLink;
			mSource = source;
		}
	};

	class invisionCoreOutOfMemory : protected invisionException
	{
	public:
		invisionCoreOutOfMemory();
		invisionCoreOutOfMemory(const char* message)
		{
			mMessage = message;
		}

		invisionCoreOutOfMemory(const char* message, const char* helpLink)
		{
			mMessage = message;
			mHelpLink = helpLink;
		}

		invisionCoreOutOfMemory(const char* message, const char* helpLink, const char* source)
		{
			mMessage = message;
			mHelpLink = helpLink;
			mSource = source;
		}
	};

	class invisionCoreMemoryIsCorrupted : protected invisionException
	{
	public:
		invisionCoreMemoryIsCorrupted();
		invisionCoreMemoryIsCorrupted(const char* message)
		{
			mMessage = message;
		}

		invisionCoreMemoryIsCorrupted(const char* message, const char* helpLink)
		{
			mMessage = message;
			mHelpLink = helpLink;
		}

		invisionCoreMemoryIsCorrupted(const char* message, const char* helpLink, const char* source)
		{
			mMessage = message;
			mHelpLink = helpLink;
			mSource = source;
		}
	};

	class invisionCoreMemoryHasWrongAlignment : protected invisionException
	{
	public:
		invisionCoreMemoryHasWrongAlignment();
		invisionCoreMemoryHasWrongAlignment(const char* message)
		{
			mMessage = message;
		}

		invisionCoreMemoryHasWrongAlignment(const char* message, const char* helpLink)
		{
			mMessage = message;
			mHelpLink = helpLink;
		}

		invisionCoreMemoryHasWrongAlignment(const char* message, const char* helpLink, const char* source)
		{
			mMessage = message;
			mHelpLink = helpLink;
			mSource = source;
		}
	};

	class invisionCoreMemoryNoLogObject : protected invisionException
	{
	public:
		invisionCoreMemoryNoLogObject();
		invisionCoreMemoryNoLogObject(const char* message)
		{
			mMessage = message;
		}

		invisionCoreMemoryNoLogObject(const char* message, const char* helpLink)
		{
			mMessage = message;
			mHelpLink = helpLink;
		}

		invisionCoreMemoryNoLogObject(const char* message, const char* helpLink, const char* source)
		{
			mMessage = message;
			mHelpLink = helpLink;
			mSource = source;
		}
	};
}
#endif // EXCEPTION_H