/////////////////////////////////////////////////////////////////////////////
// Name:	  InBaseString
// Purpose:	  offers many string operations for different types of literals (not wchar)
// Author:	  Dennis Koehler
// Created:	  2015-09-27
// Copyright: Copyright (c) 2014, Nano Game Foundation
//			  This unpublished source code is proprietary to Nano Game Foundation.
//			  The source code herein is considered trade secrets and confidential.
//			  Reproduction or distribution, in part or complete, is prohibited except by permission of Nano Game Foundation. 
//			  All rights reserved.
/////////////////////////////////////////////////////////////////////////////



#ifndef INBASESTRING_H
#define INBASESTRING_H

#define MAX_SZ 65536

template <typename T, typename U>
class InBaseString // HAL_API doesn't work
{
private:
	uint32 mCRCID = 0;
	boost::shared_array<U> mText;
	size_t mLength = 0;
	uint32 mRefCount = 0;

public:
	InBaseString()
	{

	}

	size_t inStrlen(const char *str)
	{
		const char *p = str;
		while (*p)
		{
			++p;
		}
		return size_t(p - str);
	}

	size_t inStrlen(const wchar_t *str)
	{
		const wchar_t *p = str;
		while (*p)
		{
			++p;
		}
		return size_t(p - str);

	}

	InBaseString(T text, size_t length, uint32 CRCID)
	{
		mLength = length;
		mCRCID = CRCID;
		mRefCount = 0;
		mText = boost::shared_array<U>(new U[mLength + 1]);
		memcpy(mText.get(), text, (mLength * sizeof(U)+1));
		mText.get()[mLength] = '\0';
		//std::strcpy(mText.get(), text);
		addRef();
	}

	InBaseString(boost::shared_array<U> text)
	{
		mLength = inStrlen(text.get());
		//mLength = (sizeof(text.get()) * sizeof(U));
		mCRCID = CRC::getCRC32(text.get());
		mRefCount = 0;
		mText = text;
		addRef();
	}

	uint32 GetCRCID()
	{
		return mCRCID;
	}

	T GetContent()
	{
		static U tmp[MAX_SZ];
		//std::strcpy(tmp, mText.get());
		std::memcpy(tmp, mText.get(), (mLength * sizeof(U)) + 1);
		return tmp;
	}

	void addRef()
	{
		mRefCount++;
	}
		
	void subRef()
	{
		mRefCount--;
	}

	size_t GetLength()
	{
		return mLength;
	}
		
	bool operator==(uint32 rValue)
	{
		bool equal = false;
		if (mCRCID == rValue)
		{
			equal = true;
		}
		return equal;
	}

	InBaseString substring(uint32 pos, uint32 len)
	{
		if (pos + len > this->GetLength())
		{
			throw invisionCoreArgumentOutOfRangeException("Argument 'uint32 pos and uint32 len' is out of Range");
		}

		if (pos < 0 || len < 0)
		{
			throw invisionCoreArgumentOutOfRangeException("Argument 'pos or len' is out of Range");
		}

		boost::shared_array<U> tmp(new U[len + 1]);

		for (uint32 i = pos; i < pos + len; i++)
		{
			tmp[i - pos] = mText[i];
		}
		tmp[len] = '\0';
	
	
		InBaseString tmpstr = InBaseString(tmp);

		return tmpstr;
	}

	InBaseString trim(U chars[], uint32 arrSize)
	{
		size_t realStringBegin = 0;
		size_t realStringEnd = 0;

		boost::shared_array<U> origText(new U[this->mLength]);
		origText = mText;

		bool isLoopBreaked = false;
		uint32 countOfArr = 0;

		// iterate from array front side
		for (uint32 i = 0; i < inStrlen(origText.get()); i++)
		//for (uint32 i = 0; i < mLength * sizeof(U); i++)
		{
			countOfArr = 0;
			if (isLoopBreaked == true)
			{
				isLoopBreaked = false;
				break;
			}

			for (uint32 j = 0; j < arrSize; j++)
			{
				if (origText[i] == chars[j])
				{
					break;
				}
				else
				{
					countOfArr++;
					if (countOfArr == arrSize)
					{

						isLoopBreaked = true;
						realStringBegin = i;
						break;
					}
				}
			}
		}

		// iterate from array back side
		for (size_t i = inStrlen(origText.get()) - 1; i > 0; i--)
		//for (uint32 i = (mLength * sizeof(U)) - 1; i > 0; i--)
		{
			countOfArr = 0;
			if (isLoopBreaked == true)
			{
				isLoopBreaked = false;
				break;
			}

			for (uint32 j = 0; j < arrSize; j++)
			{
				if (origText[i] == chars[j])
				{
					break;
				}
				else
				{
					countOfArr++;
					if (countOfArr == arrSize)
					{

						isLoopBreaked = true;
						realStringEnd = i;
						break;
					}
				}
			}
		}

		realStringEnd = realStringEnd-realStringBegin + 1;
		boost::shared_array<U> tmp(new U[realStringEnd]);
		for (uint32 j = 0; j < realStringEnd; j++)
		{
			tmp[j] = origText[j+realStringBegin];
		}
		tmp[realStringEnd] = '\0';

		InBaseString tmpstr = InBaseString(tmp);
		return tmpstr;
	}

	std::vector<InBaseString<T, U>> split(U chars[], uint32 arrSize)
	{
		std::vector<InBaseString<T,U>> splitedStrings;
		std::vector<uint32> splitedStringIndizes;
		
		for (uint32 i = 0; i < inStrlen(mText.get()); i++)
		//for (uint32 i = 0; i < (mLength * sizeof(U)); i++)
		{
			for (uint32 j = 0; j < arrSize; j++)
			{
				if (mText[i] == chars[j])
				{
					splitedStringIndizes.push_back(i);
				}

			}
		}

		static U tmp[MAX_SZ];
		uint32 splitedStringIndex = 0;
		uint32 origTextCursor = 0;
		for (uint32 i = 0; i < inStrlen(mText.get()); i++)
		//for (uint32 i = 0; i < (mLength * sizeof(U)); i++)
		{
			if (i != splitedStringIndizes[splitedStringIndex])
			{
				tmp[origTextCursor] = mText[i];
				origTextCursor++;
			}
			else
			{
				tmp[origTextCursor] = '\0';
				splitedStringIndex++;
				origTextCursor = 0;
				splitedStrings.push_back(InBaseString(tmp, inStrlen(tmp), CRC::getCRC32(tmp)));
				//splitedStrings.push_back(InBaseString(tmp, (sizeof(tmp) * sizeof(U)), CRC::getCRC32(tmp)));
			}
		}
		
		return splitedStrings;
	}
	
	bool contains(T value)
	{
		bool containsValue = false;
		uint32 countOfEqualChars = 0;
		for (uint32 i = 0; i < inStrlen(mText.get()); i++)
		//for (uint32 i = 0; i < mLength * sizeof(U); i++)
		{
			if (mText[i] == value[countOfEqualChars] && countOfEqualChars != inStrlen(value))
			//if (mText[i] == value[countOfEqualChars] && countOfEqualChars != (sizeof(value) * sizeof(U)))
			{
				countOfEqualChars++;
				if (countOfEqualChars == inStrlen(value))
				//if (countOfEqualChars == (sizeof(value) * sizeof(U)))
				{ 
					containsValue = true;
					break;
				}

			}
			else
			{
				countOfEqualChars = 0;
			}

		}

		return containsValue;
	}

	InBaseString replace(T oldValue, T newValue)
	{
		if (oldValue == NULL || newValue == NULL)
		{
			throw invisionCoreArgumentNullException("Argument 'oldValue or newValue' is Null");
		}

		//if (oldValue = "")
		if (inStrlen(oldValue) == 0)
		{
			throw invisionCoreArgumentException("Argument 'oldValue' is empty");
		}

		uint32 inCounter = 0;		
		uint32 countOfOldValues = 0;

		for (uint32 i = 0; i < inStrlen(mText.get()); i++)
		//for (uint32 i = 0; i < (mLength * sizeof(U)); i++)
		{
			if ((mText[i] == oldValue[inCounter]) && (inCounter != inStrlen(oldValue)))
			//if ((mText[i] == oldValue[inCounter]) && (inCounter != sizeof(oldValue) * sizeof(U)))
			{
				inCounter++;
				if (inCounter == inStrlen(oldValue))
				//if (inCounter == (sizeof(oldValue) * sizeof(U)))
				{
					countOfOldValues++;
				}
			}
			else
			{
				inCounter = 0;
			}
		}


		boost::shared_array<U> origText(new U[this->mLength + (inStrlen(newValue) - inStrlen(oldValue)) * countOfOldValues +1]);
		//boost::shared_array<U> origText(new U[this->mLength + (sizeof(newValue) * sizeof(U) - sizeof(oldValue) * sizeof(U)) * countOfOldValues + 1]);
		uint32 k = 0; // old text's iterator

		for (size_t i = 0; i < inStrlen(mText.get()); i++)
		//for (uint32 i = 0; i < mLength * sizeof(U); i++)
		{
			if (mText.get()[i] == oldValue[0])
			{
				for (uint32 j = 0; j < inStrlen(oldValue); j++)
				//for (uint32 j = 0; j < sizeof(oldValue) * sizeof(U); j++)
				{

					if (mText.get()[i + j] != oldValue[j])
					{

						break;
					}

					if (j+1 == inStrlen(oldValue))
					//if (j + 1 == sizeof(oldValue) * sizeof(U))
					{
						for (uint32 m = 0; m < inStrlen(newValue); m++)
						//for (uint32 m = 0; m < sizeof(newValue) * sizeof(U); m++)
						{
							origText.get()[k] = newValue[m];
							k++;
						}
						i = i + inStrlen(oldValue)-1; // if is true, else break -> line don't run!
						//i = i + sizeof(oldValue) * (sizeof(U)) - 1; // if is true, else break -> line don't run!
					}
				}
				
			}
			else
			{
				origText.get()[k] = this->mText.get()[i];
				k++;
			}
		}
		origText[this->mLength + (inStrlen(newValue) - inStrlen(oldValue)) * countOfOldValues] = '\0';
		//origText[this->mLength + (sizeof(newValue) * sizeof(U) - sizeof(oldValue) * sizeof(U)) * countOfOldValues] = '\0';
		InBaseString<T, U> replacedString = InBaseString<T, U>(origText);
		return replacedString;
	}
	
	InBaseString remove(uint32 startIndex, uint32 count)
	{
		if (startIndex + count > this->GetLength())
		{
			throw invisionCoreArgumentOutOfRangeException("Argument 'uint32 pos and uint32 len' is out of Range");
		}

		if (startIndex < 0 || count < 0)
		{
			throw invisionCoreArgumentOutOfRangeException("Argument 'pos or len' is out of Range");
		}

		boost::shared_array<U> origText(new U[this->mLength - count + 1]);
		uint32 newTextCounter = 0;

		for (uint32 i = 0; i < inStrlen(mText.get());i++)
		//for (uint32 i = 0; i < mLength * sizeof(U); i++)
		{
			if (i < startIndex || i >= (startIndex + count))
			{
				origText[newTextCounter] = mText.get()[i];
				newTextCounter++;
			}
		}

		origText[this->mLength - count] = '\0';
		
		InBaseString<T, U> changedString = InBaseString<T, U>(origText);
		return changedString;
	}

	int32 indexOf(T value, uint32 Startindex)
	{
		if (value == NULL)
		{
			throw invisionCoreArgumentNullException("Argument 'T value' is NULL");
		}

		if (Startindex < 0 || Startindex > this->GetLength())
		{
			throw invisionCoreArgumentOutOfRangeException("Argument 'startIndex' is out of Range");
		}

		uint32 Index = -1;
		uint32 found = 0;
		for (uint32 i = Startindex; i < inStrlen(mText.get()); i++)
		//for (uint32 i = Startindex; i < mLength * sizeof(U); i++)
		{
			if (mText.get()[i] == value[found])
			{
				if (mText.get()[i] == value[0])
				{
					Index = i;
				}

				found++;
				if (inStrlen(value) == found)
				//if (sizeof(value) * sizeof(U) == found)
				{
					break;
				}
				
			}
			else
			{
				Index = -1;
			}

		}

		return Index;
	}

	int32 indexOf(U value, uint32 startIndex)
	{
		if (value == NULL)
		{
			throw invisionCoreArgumentNullException("Argument 'T value' is NULL");
		}

		if (startIndex < 0 || startIndex > this->GetLength())
		{
			throw invisionCoreArgumentOutOfRangeException("Argument 'startIndex' is out of Range");
		}

		uint32 Index = -1;
		for (uint32 i = startIndex; i < inStrlen(mText.get()); i++)
		//for (uint32 i = startIndex; i < mLength * sizeof(U); i++)
		{
			if (mText.get()[i] == value)
			{
				Index = i;
				break;
			}
		}

		return Index;
	}

	int32 lastIndexOf(T value, uint32 Startindex)
	{
		if (value == NULL)
		{
			throw invisionCoreArgumentNullException("Argument 'T value' is NULL");
		}

		if (Startindex < 0 || Startindex > this->GetLength())
		{
			throw invisionCoreArgumentOutOfRangeException("Argument 'startIndex' is out of Range");
		}

		uint32 Index = -1;
		size_t found = inStrlen(value)-1;
		//uint32 found = sizeof(value) * sizeof(U) - 1;
		for (uint32 i = Startindex; i > 0; i--)
		{
			if (mText.get()[i] == value[found])
			{
				
				if (found == 0)
				{
					Index = i;
					break;
				}
				found--;
			}
			else
			{
				size_t found = inStrlen(value);
				//uint32 found = sizeof(value) * sizeof(U);
				Index = -1;
			}

		}

		return Index;
	}

	int32 lastIndexOf(U value, uint32 startIndex)
	{
		if (value == NULL)
		{
			throw invisionCoreArgumentNullException("Argument 'T value' is NULL");
		}

		if (startIndex < 0 || startIndex > this->GetLength())
		{
			throw invisionCoreArgumentOutOfRangeException("Argument 'startIndex' is out of Range");
		}

		uint32 Index = -1;
		for (uint32 i = startIndex; i > 0; i--)
		{
			if (mText.get()[i] == value)
			{
				Index = i;
				break;
			}
		}

		return Index;
	}

	bool endsWith(T value)
	{
		if (value == NULL)
		{
			throw invisionCoreArgumentNullException("Argument 'T value' is NULL");
		}

		bool bEndsWith = false;
		uint32 countOfval = 0;
		for (size_t i = inStrlen(mText.get()) - inStrlen(value); i < inStrlen(mText.get()); i++)
		//for (uint32 i = mLength * sizeof(U) - sizeof(value) * sizeof(U); i < mLength * sizeof(U); i++)
		{
			if (mText.get()[i] == value[countOfval])
			{
				bEndsWith = true;
			}
			else
			{
				bEndsWith = false;
				break;
			}
			countOfval++;
		}
		return bEndsWith;
	}

	bool startsWith(T value)
	{
		if (value == NULL)
		{
			throw invisionCoreArgumentNullException("Argument 'T value' is NULL");
		}

		bool bEndsWith = false;
		uint32 countOfval = 0;

		for (uint32 i = 0; i < inStrlen(value); i++)
		//for (uint32 i = 0; i < sizeof(value) * sizeof(U); i++)
		{
			if (mText.get()[i] == value[countOfval])
			{
				bEndsWith = true;
			}
			else
			{
				bEndsWith = false;
				break;
			}
			countOfval++;
		}

		return bEndsWith;
	}

	InBaseString padLeft(uint32 totalWidth, U paddingChar)
	{
		if (paddingChar == NULL)
		{
			throw invisionCoreArgumentNullException("Argument 'T value' is NULL");
		}

		if (totalWidth < 0)
		{
			throw invisionCoreArgumentOutOfRangeException("Argument 'startIndex' is out of Range");
		}

		boost::shared_array<U> origText(new U[this->mLength + totalWidth + 1]);

		for (uint32 i = 0; i < totalWidth; i++)
		{
			origText[i] = paddingChar;
		}

		uint32 textCounter = totalWidth;
		for (uint32 i = 0; i < this->mLength; i++)
		{
			origText[textCounter] = mText.get()[i];
			textCounter++;
		}

		origText[this->mLength + totalWidth] = '\0';

		InBaseString<T, U> padedString = InBaseString<T, U>(origText);
		return padedString;
	}

	InBaseString padRight(uint32 totalWidth, U paddingChar)
	{
		if (paddingChar == NULL)
		{
			throw invisionCoreArgumentNullException("Argument 'T value' is NULL");
		}

		if (totalWidth < 0)
		{
			throw invisionCoreArgumentOutOfRangeException("Argument 'startIndex' is out of Range");
		}

		boost::shared_array<U> origText(new U[this->mLength + totalWidth + 1]);
		
		for (uint32 i = 0; i < this->mLength; i++)
		{
			origText[i] = mText.get()[i];
			
		}

		size_t textCounter = inStrlen(mText.get());
		//uint32 textCounter = mLength * sizeof(U);
		for (uint32 i = 0; i < totalWidth; i++)
		{
			origText[textCounter] = paddingChar;
			textCounter++;
		}

		origText[this->mLength + totalWidth] = '\0';

		InBaseString<T, U> padedString = InBaseString<T, U>(origText);
		return padedString;
	}

	InBaseString insert(uint32 startIndex, T value)
	{
		if (value == NULL)
		{
			throw invisionCoreArgumentNullException("Argument 'T value' is Null");
		}

		if (startIndex < 0 || startIndex > this->mLength)
		{
			throw invisionCoreArgumentOutOfRangeException("Argument 'uint32 startIndex' is out of Range");
		}

		boost::shared_array<U> origText(new U[this->mLength + inStrlen(value) + 1]);
		for (uint32 i = 0; i < this->mLength; i++)
		{
			origText[i] = this->mText[i];
		}


		for (size_t i = mLength; i < mLength + inStrlen(value); i++)
		{
			origText[i] = value[i - mLength];
		}
		
		origText[this->mLength + inStrlen(value)] = '\0';
		InBaseString<T, U> addedString = InBaseString<T, U>(origText);

		return addedString;
	}

	InBaseString toUpper(bool isWideChar)
	{
#pragma warning( push )
#pragma warning( disable : 4244 ) // turn off 'warning C4244: '=' : conversion from 'wint_t' to 'char', possible loss of data'
		boost::shared_array<U> origText(new U[this->mLength + 1]);

		for (uint32 i = 0; i < inStrlen(mText.get()); i++)
		//for (uint32 i = 0; i < mLength * sizeof(U); i++)
		{
			if (isWideChar)
			{
				origText[i] = toupper(mText.get()[i]);
			}
			else
			{
				origText[i] = towupper(mText.get()[i]);
			}
		}

		origText[this->mLength] = '\0';
		InBaseString<T, U>convertedString = InBaseString<T, U>(origText);

		return convertedString;
#pragma warning( pop ) 
	}

	InBaseString toLower(bool isWideChar)
	{
#pragma warning( push )
#pragma warning( disable : 4244 ) // turn off 'warning C4244: '=' : conversion from 'wint_t' to 'char', possible loss of data'
		boost::shared_array<U> origText(new U[this->mLength + 1]);

		for (uint32 i = 0; i < inStrlen(mText.get()); i++)
		//for (uint32 i = 0; i < mLength * sizeof(U); i++)
		{	
			if (isWideChar)
			{
				origText[i] = tolower(mText.get()[i]);
			}
			else
			{
				origText[i] = towlower(mText.get()[i]);
			}
		}

		origText[this->mLength] = '\0';
		
		InBaseString<T, U>convertedString = InBaseString<T, U>(origText);

		return convertedString;
#pragma warning( pop ) 
	}
};
#endif // INBASESTRING_H