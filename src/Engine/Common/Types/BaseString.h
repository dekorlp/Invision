#ifndef BASESTRING_H
#define BASESTRING_H

#define MAX_SZ 65536

namespace Invision
{
	template <typename T, typename U>
	class BaseString
	{
		private:
			size_t mCRCID = 0;
			std::shared_ptr<U> mText;
			size_t mLength = 0;
			size_t mRefCount = 0;

		public:
			BaseString()
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

			BaseString(T text, size_t length, size_t CRCID)
			{
				mLength = length;
				mCRCID = CRCID;
				mRefCount = 0;
				mText = std::shared_ptr<U>(new U[mLength + 1]);
				memcpy(mText.get(), text, (mLength * sizeof(U) + 1));
				mText.get()[mLength] = '\0';
				addRef();
			}

			BaseString(std::shared_ptr<U> text)
			{
				mLength = inStrlen(text.get());
				mCRCID = Hash::getHash(text.get());
				mRefCount = 0;
				mText = text;
				addRef();
			}

			size_t GetCRCID()
			{
				return mCRCID;
			}

			T GetContent()
			{
				static U tmp[MAX_SZ];
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

			bool operator==(size_t rValue)
			{
				bool equal = false;
				if (mCRCID == rValue)
				{
					equal = true;
				}
				return equal;
			}

			BaseString substring(size_t pos, size_t len)
			{
				if (pos + len > this->GetLength())
				{
					throw InvisionArgumentOutOfRangeException("Argument 'size_t pos and size_t len' is out of Range");
				}

				if (pos < 0 || len < 0)
				{
					throw InvisionArgumentOutOfRangeException("Argument 'pos or len' is out of Range");
				}

				std::shared_ptr<U> tmp(new U[len + 1]);

				for (size_t i = pos; i < pos + len; i++)
				{
					tmp.get()[i - pos] = mText.get()[i];
				}
				tmp.get()[len] = '\0';


				BaseString tmpstr = BaseString(tmp);

				return tmpstr;
			}

			BaseString trim(U chars[], size_t arrSize)
			{
				size_t realStringBegin = 0;
				size_t realStringEnd = 0;

				std::shared_ptr<U> origText(new U[this->mLength]);
				origText = mText;

				bool isLoopBreaked = false;
				size_t countOfArr = 0;

				// iterate from array front side
				for (size_t i = 0; i < inStrlen(origText.get()); i++)
				{
					countOfArr = 0;
					if (isLoopBreaked == true)
					{
						isLoopBreaked = false;
						break;
					}

					for (size_t j = 0; j < arrSize; j++)
					{
						if (origText.get()[i] == chars[j])
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
				{
					countOfArr = 0;
					if (isLoopBreaked == true)
					{
						isLoopBreaked = false;
						break;
					}

					for (size_t j = 0; j < arrSize; j++)
					{
						if (origText.get()[i] == chars[j])
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

				realStringEnd = realStringEnd - realStringBegin + 1;
				std::shared_ptr<U> tmp(new U[realStringEnd]);
				for (size_t j = 0; j < realStringEnd; j++)
				{
					tmp.get()[j] = origText.get()[j + realStringBegin];
				}
				tmp.get()[realStringEnd] = '\0';

				BaseString tmpstr = BaseString(tmp);
				return tmpstr;
			}

			std::vector<BaseString<T, U>> split(U chars[], size_t arrSize)
			{
				std::vector<BaseString<T, U>> splitedStrings;
				std::vector<size_t> splitedStringIndizes;

				for (size_t i = 0; i < inStrlen(mText.get()); i++)
				{
					for (size_t j = 0; j < arrSize; j++)
					{
						if (mText.get()[i] == chars[j])
						{
							splitedStringIndizes.push_back(i);
						}

					}
				}

				static U tmp[MAX_SZ];
				size_t splitedStringIndex = 0;
				size_t origTextCursor = 0;
				for (size_t i = 0; i < inStrlen(mText.get()); i++)
				{
					if (i != splitedStringIndizes[splitedStringIndex])
					{
						tmp[origTextCursor] = mText.get()[i];
						origTextCursor++;
					}
					else
					{
						tmp[origTextCursor] = '\0';
						splitedStringIndex++;
						origTextCursor = 0;
						splitedStrings.push_back(BaseString(tmp, inStrlen(tmp), Hash::getHash(tmp)));
					}
				}

				return splitedStrings;
			}

			bool contains(T value)
			{
				bool containsValue = false;
				size_t countOfEqualChars = 0;
				for (size_t i = 0; i < inStrlen(mText.get()); i++)
				{
					if (mText.get()[i] == value[countOfEqualChars] && countOfEqualChars != inStrlen(value))
					{
						countOfEqualChars++;
						if (countOfEqualChars == inStrlen(value))
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

			BaseString replace(T oldValue, T newValue)
			{
				if (oldValue == NULL || newValue == NULL)
				{
					throw InvisionArgumentNullException("Argument 'oldValue or newValue' is Null");
				}

				if (inStrlen(oldValue) == 0)
				{
					throw InvisionArgumentException("Argument 'oldValue' is empty");
				}

				size_t inCounter = 0;
				size_t countOfOldValues = 0;

				for (size_t i = 0; i < inStrlen(mText.get()); i++)
				{
					if ((mText.get()[i] == oldValue[inCounter]) && (inCounter != inStrlen(oldValue)))
					{
						inCounter++;
						if (inCounter == inStrlen(oldValue))
						{
							countOfOldValues++;
						}
					}
					else
					{
						inCounter = 0;
					}
				}


				std::shared_ptr<U> origText(new U[this->mLength + (inStrlen(newValue) - inStrlen(oldValue)) * countOfOldValues + 1]);
				size_t k = 0; // old text's iterator

				for (size_t i = 0; i < inStrlen(mText.get()); i++)
				{
					if (mText.get()[i] == oldValue[0])
					{
						for (size_t j = 0; j < inStrlen(oldValue); j++)
						{

							if (mText.get()[i + j] != oldValue[j])
							{

								break;
							}

							if (j + 1 == inStrlen(oldValue))
							{
								for (size_t m = 0; m < inStrlen(newValue); m++)
								{
									origText.get()[k] = newValue[m];
									k++;
								}
								i = i + inStrlen(oldValue) - 1; // if is true, else break -> line don't run!
							}
						}

					}
					else
					{
						origText.get()[k] = this->mText.get()[i];
						k++;
					}
				}
				origText.get()[this->mLength + (inStrlen(newValue) - inStrlen(oldValue)) * countOfOldValues] = '\0';
				BaseString<T, U> replacedString = BaseString<T, U>(origText);
				return replacedString;
			}

			BaseString remove(size_t startIndex, size_t count)
			{
				if (startIndex + count > this->GetLength())
				{
					throw InvisionArgumentOutOfRangeException("Argument 'size_t pos and size_t len' is out of Range");
				}

				if (startIndex < 0 || count < 0)
				{
					throw InvisionArgumentOutOfRangeException("Argument 'pos or len' is out of Range");
				}

				std::shared_ptr<U> origText(new U[this->mLength - count + 1]);
				size_t newTextCounter = 0;

				for (size_t i = 0; i < inStrlen(mText.get()); i++)
				{
					if (i < startIndex || i >= (startIndex + count))
					{
						origText.get()[newTextCounter] = mText.get()[i];
						newTextCounter++;
					}
				}

				origText.get()[this->mLength - count] = '\0';

				BaseString<T, U> changedString = BaseString<T, U>(origText);
				return changedString;
			}

			size_t indexOf(T value, size_t Startindex)
			{
				if (value == NULL)
				{
					throw InvisionArgumentNullException("Argument 'T value' is NULL");
				}

				if (Startindex < 0 || Startindex > this->GetLength())
				{
					throw InvisionArgumentOutOfRangeException("Argument 'startIndex' is out of Range");
				}

				size_t Index = -1;
				size_t found = 0;
				for (size_t i = Startindex; i < inStrlen(mText.get()); i++)
				{
					if (mText.get()[i] == value[found])
					{
						if (mText.get()[i] == value[0])
						{
							Index = i;
						}

						found++;
						if (inStrlen(value) == found)
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

			size_t indexOf(U value, size_t startIndex)
			{
				if (value == NULL)
				{
					throw InvisionArgumentNullException("Argument 'T value' is NULL");
				}

				if (startIndex < 0 || startIndex > this->GetLength())
				{
					throw InvisionArgumentOutOfRangeException("Argument 'startIndex' is out of Range");
				}

				size_t Index = -1;
				for (size_t i = startIndex; i < inStrlen(mText.get()); i++)
				{
					if (mText.get()[i] == value)
					{
						Index = i;
						break;
					}
				}

				return Index;
			}

			size_t lastIndexOf(T value, size_t Startindex)
			{
				if (value == NULL)
				{
					throw InvisionArgumentNullException("Argument 'T value' is NULL");
				}

				if (Startindex < 0 || Startindex > this->GetLength())
				{
					throw InvisionArgumentOutOfRangeException("Argument 'startIndex' is out of Range");
				}

				size_t Index = -1;
				size_t found = inStrlen(value) - 1;
				for (size_t i = Startindex; i > 0; i--)
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
						Index = -1;
					}

				}

				return Index;
			}

			size_t lastIndexOf(U value, size_t startIndex)
			{
				if (value == NULL)
				{
					throw InvisionArgumentNullException("Argument 'T value' is NULL");
				}

				if (startIndex < 0 || startIndex > this->GetLength())
				{
					throw InvisionArgumentOutOfRangeException("Argument 'startIndex' is out of Range");
				}

				size_t Index = -1;
				for (size_t i = startIndex; i > 0; i--)
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
					throw InvisionArgumentNullException("Argument 'T value' is NULL");
				}

				bool bEndsWith = false;
				size_t countOfval = 0;
				for (size_t i = inStrlen(mText.get()) - inStrlen(value); i < inStrlen(mText.get()); i++)
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
					throw InvisionArgumentNullException("Argument 'T value' is NULL");
				}

				bool bEndsWith = false;
				size_t countOfval = 0;

				for (size_t i = 0; i < inStrlen(value); i++)
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

			BaseString padLeft(size_t totalWidth, U paddingChar)
			{
				if (paddingChar == NULL)
				{
					throw InvisionArgumentNullException("Argument 'T value' is NULL");
				}

				if (totalWidth < 0)
				{
					throw InvisionArgumentOutOfRangeException("Argument 'startIndex' is out of Range");
				}

				std::shared_ptr<U> origText(new U[this->mLength + totalWidth + 1]);

				for (size_t i = 0; i < totalWidth; i++)
				{
					origText.get()[i] = paddingChar;
				}

				size_t textCounter = totalWidth;
				for (size_t i = 0; i < this->mLength; i++)
				{
					origText.get()[textCounter] = mText.get()[i];
					textCounter++;
				}

				origText.get()[this->mLength + totalWidth] = '\0';

				BaseString<T, U> padedString = BaseString<T, U>(origText);
				return padedString;
			}

			BaseString padRight(size_t totalWidth, U paddingChar)
			{
				if (paddingChar == NULL)
				{
					throw InvisionArgumentNullException("Argument 'T value' is NULL");
				}

				if (totalWidth < 0)
				{
					throw InvisionArgumentOutOfRangeException("Argument 'startIndex' is out of Range");
				}

				std::shared_ptr<U> origText(new U[this->mLength + totalWidth + 1]);

				for (size_t i = 0; i < this->mLength; i++)
				{
					origText.get()[i] = mText.get()[i];

				}

				size_t textCounter = inStrlen(mText.get());
				for (size_t i = 0; i < totalWidth; i++)
				{
					origText.get()[textCounter] = paddingChar;
					textCounter++;
				}

				origText.get()[this->mLength + totalWidth] = '\0';

				BaseString<T, U> padedString = BaseString<T, U>(origText);
				return padedString;
			}

			BaseString insert(size_t startIndex, T value)
			{
				if (value == NULL)
				{
					throw InvisionArgumentNullException("Argument 'T value' is Null");
				}

				if (startIndex < 0 || startIndex > this->mLength)
				{
					throw InvisionArgumentOutOfRangeException("Argument 'size_t startIndex' is out of Range");
				}

				std::shared_ptr<U> origText(new U[this->mLength + inStrlen(value) + 1]);
				for (size_t i = 0; i < this->mLength; i++)
				{
					origText.get()[i] =  mText.get()[i];
				}


				for (size_t i = mLength; i < mLength + inStrlen(value); i++)
				{
					origText.get()[i] = value[i - mLength];
				}

				origText.get()[this->mLength + inStrlen(value)] = '\0';
				BaseString<T, U> addedString = BaseString<T, U>(origText);

				return addedString;
			}
			
			BaseString toUpper(bool isWideChar)
			{
				std::shared_ptr<U> origText(new U[this->mLength + 1]);
			
				for (size_t i = 0; i < inStrlen(mText.get()); i++)
				{
					if (isWideChar)
					{
						origText.get()[i] = toupper(mText.get()[i]);
					}
					else
					{
						origText.get()[i] = static_cast<char>(towupper(mText.get()[i]));
					}
				}
			
				origText.get()[this->mLength] = '\0';
				BaseString<T, U>convertedString = BaseString<T, U>(origText);
			
				return convertedString;
			}
			
			BaseString toLower(bool isWideChar)
			{
				std::shared_ptr<U> origText(new U[this->mLength + 1]);
			
				for (size_t i = 0; i < inStrlen(mText.get()); i++)
				{
					if (isWideChar)
					{
						origText.get()[i] = tolower(mText.get()[i]);
					}
					else
					{
						origText.get()[i] = static_cast<char>(towlower(mText.get()[i]));
					}
				}
			
				origText.get()[this->mLength] = '\0';
			
				BaseString<T, U>convertedString = BaseString<T, U>(origText);
			
				return convertedString;
			}
	};

	
}
#endif // BASESTRING_H