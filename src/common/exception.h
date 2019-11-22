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
	class INVISION_API InvisionBaseException
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

	class InvisionException : public InvisionBaseException
	{
	public:
		InvisionException();
		InvisionException(const char* message)
		{
			mMessage = message;
		}

		InvisionException(const char* message, const char* helpLink)
		{
			mMessage = message;
			mHelpLink = helpLink;
		}

		InvisionException(const char* message, const char* helpLink, const char* source)
		{
			mMessage = message;
			mHelpLink = helpLink;
			mSource = source;
		}
	};

	class InvisionArgumentException : public  InvisionBaseException
	{
	public:
		InvisionArgumentException();
		InvisionArgumentException(const char* message)
		{
			mMessage = message;
		}

		InvisionArgumentException(const char* message, const char* helpLink)
		{
			mMessage = message;
			mHelpLink = helpLink;
		}

		InvisionArgumentException(const char* message, const char* helpLink, const char* source)
		{
			mMessage = message;
			mHelpLink = helpLink;
			mSource = source;
		}
	};

	class InvisionArgumentNullException : public InvisionBaseException
	{
	public:
		InvisionArgumentNullException();
		InvisionArgumentNullException(const char* message)
		{
			mMessage = message;
		}

		InvisionArgumentNullException(const char* message, const char* helpLink)
		{
			mMessage = message;
			mHelpLink = helpLink;
		}

		InvisionArgumentNullException(const char* message, const char* helpLink, const char* source)
		{
			mMessage = message;
			mHelpLink = helpLink;
			mSource = source;
		}
	};

	class InvisionArgumentOutOfRangeException : public InvisionBaseException
	{
	public:
		InvisionArgumentOutOfRangeException();
		InvisionArgumentOutOfRangeException(const char* message)
		{
			mMessage = message;
		}

		InvisionArgumentOutOfRangeException(const char* message, const char* helpLink)
		{
			mMessage = message;
			mHelpLink = helpLink;
		}

		InvisionArgumentOutOfRangeException(const char* message, const char* helpLink, const char* source)
		{
			mMessage = message;
			mHelpLink = helpLink;
			mSource = source;
		}
	};

	class InvisionOutOfMemory : public InvisionBaseException
	{
	public:
		InvisionOutOfMemory();
		InvisionOutOfMemory(const char* message)
		{
			mMessage = message;
		}

		InvisionOutOfMemory(const char* message, const char* helpLink)
		{
			mMessage = message;
			mHelpLink = helpLink;
		}

		InvisionOutOfMemory(const char* message, const char* helpLink, const char* source)
		{
			mMessage = message;
			mHelpLink = helpLink;
			mSource = source;
		}
	};

	class InvisionMemoryIsCorrupted : public InvisionBaseException
	{
	public:
		InvisionMemoryIsCorrupted();
		InvisionMemoryIsCorrupted(const char* message)
		{
			mMessage = message;
		}

		InvisionMemoryIsCorrupted(const char* message, const char* helpLink)
		{
			mMessage = message;
			mHelpLink = helpLink;
		}

		InvisionMemoryIsCorrupted(const char* message, const char* helpLink, const char* source)
		{
			mMessage = message;
			mHelpLink = helpLink;
			mSource = source;
		}
	};

	class InvisionMemoryHasWrongAlignment : public InvisionBaseException
	{
	public:
		InvisionMemoryHasWrongAlignment();
		InvisionMemoryHasWrongAlignment(const char* message)
		{
			mMessage = message;
		}

		InvisionMemoryHasWrongAlignment(const char* message, const char* helpLink)
		{
			mMessage = message;
			mHelpLink = helpLink;
		}

		InvisionMemoryHasWrongAlignment(const char* message, const char* helpLink, const char* source)
		{
			mMessage = message;
			mHelpLink = helpLink;
			mSource = source;
		}
	};

	class InvisionMemoryNoLogObject : public InvisionBaseException
	{
	public:
		InvisionMemoryNoLogObject();
		InvisionMemoryNoLogObject(const char* message)
		{
			mMessage = message;
		}

		InvisionMemoryNoLogObject(const char* message, const char* helpLink)
		{
			mMessage = message;
			mHelpLink = helpLink;
		}

		InvisionMemoryNoLogObject(const char* message, const char* helpLink, const char* source)
		{
			mMessage = message;
			mHelpLink = helpLink;
			mSource = source;
		}
	};
}
#endif // EXCEPTION_H