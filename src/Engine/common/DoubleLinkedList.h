#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

namespace Invision
{

	template <class T>
	struct LinkedListNode
	{
		LinkedListNode<T>* previous;
		LinkedListNode<T>* next;
		T mData;
	};

	template <class T>
	class DoubleLinkedList
	{
	public:

		INVISION_API DoubleLinkedList()
		{
			mFront = nullptr;
			mBack = nullptr;
			mCountElements = 0;
		}

		INVISION_API DoubleLinkedList(DoubleLinkedList<T>& x)
		{
			LinkedListNode<T>* ptr = x.getFront();
			if (x.size() == 0) {
				mFront = mBack = nullptr;
				return;
			}
			LinkedListNode<T>* ptr2 = new LinkedListNode<T>;
			ptr2->mData = ptr->mData;
			ptr2->previous = ptr2->next = nullptr;
			mFront = ptr2;
			ptr = ptr->next;
			while (ptr != nullptr)
			{
				LinkedListNode<T>* temp = new LinkedListNode<T>;
				temp->mData = ptr->mData;
				temp->next = nullptr;
				temp->previous = ptr2;
				ptr2->next = temp;
				ptr2 = temp;
				ptr = ptr->next;
			}
			mBack = ptr2;
			mCountElements = x.mCountElements;
		}

		~DoubleLinkedList()
		{
			LinkedListNode<T> *ptr = mFront;
			while (ptr != nullptr)
			{
				LinkedListNode<T> *ptr2 = ptr->next;
				delete ptr;
				ptr = ptr2;
			}
		}

		void* pushBack(T data)
		{
			LinkedListNode<T> *node = new LinkedListNode<T>;

			if (isEmpty())
			{
				mFront = node;
				mBack = node;

				//node->previous = mFront;
				//node->next = mBack;
				node->previous = nullptr;
				node->next = nullptr;
				node->mData = data;
			}
			else
			{
				LinkedListNode<T> *prev = mBack;
				prev->next = node;
				mBack = node;

				node->previous = prev;
				//node->next = mBack;
				node->next = nullptr;
				node->mData = data;
			}

			mCountElements++;
			return node;
		}

		void* pushFront(T data)
		{
			LinkedListNode<T> *node = new LinkedListNode<T>;

			if (isEmpty())
			{


				mFront = node;
				mBack = node;

				//node->previous = mFront;
				//node->next = mBack;
				node->previous = nullptr;
				node->next = nullptr;
				node->mData = data;
			}
			else
			{
				node->next = mFront;
				mFront->previous = node;
				mFront = node;

				//node->previous = mFront;
				node->previous = nullptr;
				node->mData = data;
			}

			mCountElements++;
			return node;
		}

		INVISION_API bool isEmpty()
		{
			if (mFront == nullptr && mBack == nullptr)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		INVISION_API LinkedListNode<T>* getFront()
		{
			return mFront;
		}

		INVISION_API LinkedListNode<T>* getBack()
		{
			return mBack;
		}

		void remove(void* node)
		{
			LinkedListNode<T> *selected = (LinkedListNode<T> *)node;
			// at front
			if (selected->previous == nullptr)
			{
				mFront = selected->next;
				selected->next->previous = nullptr;
				delete selected;
			}
			// at back
			else if (selected->next == nullptr)
			{
				mBack = selected->previous;
				selected->previous->next = nullptr;
				delete selected;
			}
			// is between
			else
			{
				selected->next->previous = selected->previous;
				selected->previous->next = selected->next;
				delete selected;
			}

			mCountElements--;
		}

		INVISION_API std::size_t size()
		{
			return mCountElements;
		}

		class Iterator {
			LinkedListNode<T> *ptr;
		public:
			INVISION_API Iterator() { ptr = nullptr; }
			INVISION_API Iterator(LinkedListNode<T>* p) { ptr = p; }
			INVISION_API LinkedListNode<T>* get_ptr() { return ptr; }
			INVISION_API T* operator*() const { ptr->mData; }
			INVISION_API void operator=(Iterator iter) { ptr = iter.get_ptr(); }
			INVISION_API bool operator==(Iterator iter) { return ptr == iter.get_ptr(); }
			INVISION_API void operator++() { if (ptr != nullptr) ptr = ptr->next; }
			INVISION_API void operator--() { if (ptr != nullptr) ptr = ptr->previous; }
			INVISION_API bool operator!=(Iterator iter) { return ptr != iter.get_ptr(); }
			INVISION_API T*  operator->() const { return(&ptr->mData); };
		};

		INVISION_API Iterator begin()
		{
			return Iterator(mFront);
		}

		INVISION_API Iterator end()
		{
			return Iterator(nullptr);
		}

	private:
		LinkedListNode<T>* mFront;
		LinkedListNode<T>* mBack;
		std::size_t mCountElements;

	};
}
#endif // DOUBLE_LINKED_LIST_H