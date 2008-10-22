#pragma once

#include "List.h"
#include "MesonException.h"

Meson_Common_Collections_BEGIN

Meson::Common::Memory::MemoryManager *GetLinkedListMemoryManager(void);

Meson::Common::Memory::MemoryManager *GetLinkedListNodeMemoryManager(void);

template<class TElement>
struct TLinkedListNode
{
	TElement m_element;
	TLinkedListNode* m_pNext;
	TLinkedListNode* m_pPrevious;

	void* operator new(size_t p_unSize)
	{
		return GetLinkedListNodeMemoryManager()->AllocatePooled(p_unSize);
	}

	void operator delete(void* p_pObject, size_t p_unSize)
	{
		::operator delete(p_pObject);
	}							
};

template< class TElement>
class TLinkedList;

template< class TElement>
class TLinkedListEnumerator
	: public TEnumerator<TElement>
{
private:
	TLinkedList<TElement>* m_pLinkedList;
	TLinkedListNode<TElement>* m_pEnumerator;
	size_t m_unAvailable;
	bool* m_pbEnumerating;

public:
	TLinkedListEnumerator(TLinkedList<TElement>* p_pLinkedList, bool* p_pbEnumerating)
		: m_pLinkedList(p_pLinkedList)
		, m_pEnumerator(p_pLinkedList->m_pHead)
		, m_unAvailable(p_pLinkedList->Size())
		, m_pbEnumerating(p_pbEnumerating)
	{
	}

	bool HasMoreElements(void)
	{
		if (!*m_pbEnumerating)
			throw new MesonException("List was modified during enumeration",
				__FILE__, __LINE__);

		return m_pEnumerator != NULL;
	}

	void Reset(void)
	{
		m_pEnumerator = m_pLinkedList->m_pHead;
		m_unAvailable = m_pLinkedList->Size();
	}

	TElement& NextElement(void)
	{
		if (!*m_pbEnumerating)
			throw new MesonException("List was modified during enumeration",
				__FILE__, __LINE__);

		if( m_pEnumerator == NULL )
			throw new MesonException( "No more elements to enumerate.", __FILE__, __LINE__ );

		TLinkedListNode<TElement>* m_pNode = m_pEnumerator;
		m_pEnumerator = m_pEnumerator->m_pNext;
		--m_unAvailable;
		return m_pNode->m_element;
	}

	size_t Size( void ) const
	{
		return m_pLinkedList->Size();
	}

	size_t Available( void ) const
	{
		return m_unAvailable;
	}
};

// use of 'this' in array list enumerator constructor
#pragma warning( disable : 4355 )

template<class TElement>
class TLinkedList
	: public TList<TElement>
{
	friend class TLinkedListEnumerator<TElement>;

private: // private variables
	TLinkedListNode<TElement> *m_pHead, *m_pTail, *m_pLastAccessed;
	size_t m_unSize;
	size_t m_unLastAccessed;
	bool m_bEnumerating;
	TLinkedListEnumerator<TElement> m_linkedListEnumerator;

private: // private methods
	inline TLinkedListNode<TElement>* NodeAt(size_t p_unIndex)
	{
		if (m_pLastAccessed == NULL)
		{
			m_pLastAccessed = m_pHead;
			m_unLastAccessed = 0;
		}

		// repeated access
		if (p_unIndex == m_unLastAccessed)
			return m_pLastAccessed;

		bool bForward = false;
		size_t unDelta = 0;

		if (p_unIndex < m_unLastAccessed)
		{
			unDelta = m_unLastAccessed - p_unIndex;
			// between head and last accessed
			if (p_unIndex < unDelta)
			{
				// closer to head than last accessed
				bForward = true;
				unDelta = p_unIndex;
				m_pLastAccessed = m_pHead;
				m_unLastAccessed = 0;
			}
		}
		else
		{
			// between last accessed and tail
			unDelta = m_unSize - p_unIndex - 1;
			if (p_unIndex - m_unLastAccessed < unDelta)
			{
				// closer to last accessed than tail
				bForward = true;
				unDelta = p_unIndex - m_unLastAccessed;
			}
			else
			{
				m_pLastAccessed = m_pTail;
				m_unLastAccessed = m_unSize - 1;
			}
		}

		if (bForward)
		{
			m_unLastAccessed += unDelta;
			for (; unDelta > 0; unDelta--, m_pLastAccessed = m_pLastAccessed->m_pNext);
		}
		else
		{
			m_unLastAccessed -= unDelta;
			for (; unDelta > 0; unDelta--, m_pLastAccessed = m_pLastAccessed->m_pPrevious);
		}

		return m_pLastAccessed;
	}

public: // public methods
	TLinkedList(void)
		: m_pHead(NULL)
		, m_pTail(NULL)
		, m_unSize(0)
		, m_pLastAccessed(NULL)
		, m_unLastAccessed(0)
		, m_bEnumerating(false)
		, m_linkedListEnumerator(this, &m_bEnumerating)
	{
	}

	TLinkedList(const TImmutableList<TElement>& p_list)
		: m_pHead(NULL)
		, m_pTail(NULL)
		, m_unSize(0)
		, m_bEnumerating(false)
		, m_linkedListEnumerator(this, &m_bEnumerating)
	{
		AddList(p_list);
	}

	TLinkedList(const TLinkedList<TElement>& p_list)
		: m_pHead(NULL)
		, m_pTail(NULL)
		, m_unSize(0)
		, m_pLastAccessed(NULL)
		, m_unLastAccessed(0)
		, m_bEnumerating(false)
		, m_linkedListEnumerator(this, &m_bEnumerating)
	{
		AddList(p_list);
	}

	~TLinkedList(void)
	{
		Clear();
	}

	void* operator new(size_t p_unSize)
	{
		return GetLinkedListMemoryManager()->AllocatePooled(p_unSize);
	}

	void operator delete(void* p_pObject, size_t p_unSize)
	{
		::operator delete(p_pObject);
	}							

	inline TElement& operator[](size_t p_unIndex) const
	{
		return GetElement(p_unIndex);
	}

	TElement& operator()(size_t p_unIndex) const
	{
		TLinkedListNode<TElement>* pNode
			= ((TLinkedList<TElement>*)this)->NodeAt(p_unIndex);

		return pNode->m_element;
	}

	inline size_t Size(void) const
	{
		return m_unSize;
	}

	inline TElement& GetElement(size_t p_unIndex) const
	{
		if (p_unIndex >= m_unSize)
			throw new MesonException("List index out of range.",
				__FILE__, __LINE__);

		TLinkedListNode<TElement>* pNode
			= ((TLinkedList<TElement>*)this)->NodeAt(p_unIndex);

		return pNode->m_element;
	}


	inline void SetElement(size_t p_unIndex, const TElement& p_element)
	{
		if (p_unIndex >= m_unSize)
			throw new MesonException("List index out of range.",
				__FILE__, __LINE__);

		TLinkedListNode<TElement>* pNode
			= ((TLinkedList<TElement>*)this)->NodeAt(p_unIndex);

		pNode->m_element = p_element;
	}

	inline int IndexOf(const TElement& p_element) const
	{
		int nIndex = 0;
		for(TLinkedListNode<TElement>* pNode = m_pHead;
			pNode != NULL; pNode = pNode->m_pNext)
		{
			if (pNode->m_element == p_element)
				return nIndex;
			++nIndex;
		}
		return -1;
	}

	inline TLinkedList<TElement>& operator=(const TList<TElement>& p_list)
	{
		if (&p_list == this) return *this;

		Clear();
		AddList(p_list);
		return *this;
	}

	inline TLinkedList<TElement>& operator=(const TLinkedList<TElement>& p_list)
	{
		if (&p_list == this) return *this;

		Clear();
		AddList(p_list);
		return *this;
	}

	inline TLinkedList<TElement> operator+(const TElement& p_element)
	{
		TLinkedList<TElement> listNew;
		listNew.AddList(*this);
		listNew.Add(p_element);
		return listNew;
	}

	inline TLinkedList<TElement> operator-(const TElement& p_element)
	{
		TLinkedList<TElement> listNew;
		listNew.AddList(*this);
		listNew.Remove(p_element);
		return listNew;
	}

	TLinkedList<TElement> SubList(size_t p_unIndex, size_t p_unLength) const
	{
		if (p_unIndex >= m_unSize)
			throw new MesonException("List index out of range.",
				__FILE__, __LINE__);

		if (p_unIndex + p_unLength > m_unSize)
			throw new MesonException("List length out of range.",
				__FILE__, __LINE__);

		TLinkedList<TElement> listSub;
		TLinkedListNode<TElement>* pNode
			= ((TLinkedList<TElement> *)this)->NodeAt(p_unIndex);
		for (; p_unLength > 0; p_unLength--, pNode = pNode->m_pNext)
			listSub.Add(pNode->m_element);

		return listSub;
	}

	inline void Clear(void)
	{
		while (m_pHead != NULL)
		{
			TLinkedListNode<TElement>* pNode = m_pHead;
			m_pHead = m_pHead->m_pNext;
			delete pNode;
		}
		m_pTail = NULL;
		m_unSize = 0;
		m_pLastAccessed = NULL;
		m_unLastAccessed = 0;
		m_bEnumerating = false;
	}

	void Add(const TElement& p_element)
	{
		if (m_pHead == NULL)
		{
			m_pHead = new TLinkedListNode<TElement>();
			m_pHead->m_element = p_element;
			m_pHead->m_pPrevious = NULL;
			m_pHead->m_pNext = NULL;
			m_pTail = m_pHead;
		}
		else
		{
			TLinkedListNode<TElement>* pNode
				= new TLinkedListNode<TElement>();
			m_pTail->m_pNext = pNode;
			pNode->m_element = p_element;
			pNode->m_pPrevious = m_pTail;
			pNode->m_pNext = NULL;
			m_pTail = pNode;
		}
		++m_unSize;
		m_bEnumerating = false;
	}

	void AddList(const TImmutableList<TElement>& p_list)
	{
		if (p_list.Size() == 0) return;

		for (TEnumerator<TElement> &enumerator
			= ((TList<TElement>&)p_list).GetEnumerator();
			enumerator.HasMoreElements();)
		{
			TLinkedListNode<TElement> *pNode
				= new TLinkedListNode<TElement>();
			pNode->m_element = enumerator.NextElement();

			if (m_pTail == NULL)
			{
				pNode->m_pPrevious = NULL;
				m_pHead = pNode;
			}
			else
			{
				m_pTail->m_pNext = pNode;
				pNode->m_pPrevious = m_pTail;
			}

			m_pTail = pNode;
		}

		m_pTail->m_pNext = NULL;

		m_bEnumerating = false;
		m_unSize += p_list.Size();
	}

	void Insert(size_t p_unIndex, const TElement& p_element)
	{
		if (p_unIndex >= m_unSize)
			throw new MesonException("List index out of range.",
				__FILE__, __LINE__);

		if (p_unIndex == 0)
		{
			TLinkedListNode<TElement>* pNodeNew
				= new TLinkedListNode<TElement>();
			pNodeNew->m_element = p_element;
			pNodeNew->m_pPrevious = NULL;
			pNodeNew->m_pNext = m_pHead;
			m_pHead->m_pPrevious = pNodeNew;
			m_pHead = pNodeNew;
			if (m_pTail == NULL)
				m_pTail = m_pHead;
			m_pLastAccessed = NULL;
			m_unLastAccessed = 0;
		}
		else
		{
			TLinkedListNode<TElement>* pNode = NodeAt(p_unIndex)->m_pPrevious;

			TLinkedListNode<TElement>* pNodeNew
				= new TLinkedListNode<TElement>();

			pNodeNew->m_element = p_element;
			pNodeNew->m_pPrevious = pNode;
			pNodeNew->m_pNext = pNode->m_pNext;
			pNode->m_pNext = pNodeNew;
			if (pNodeNew->m_pNext != NULL)
				pNodeNew->m_pNext->m_pPrevious = pNodeNew;

			if (p_unIndex <= m_unLastAccessed)
			{
				m_pLastAccessed = NULL;
				m_unLastAccessed = 0;
			}
		}

		++m_unSize;
		m_bEnumerating = false;
	}

	inline void InsertList(size_t p_unIndex, const TImmutableList<TElement>& p_list)
	{
		if (p_unIndex >= m_unSize)
			throw new MesonException("List index out of range.",
				__FILE__, __LINE__);

		for (TEnumerator<TElement>& enumerator
			= ((TList<TElement>&)p_list).GetEnumerator();
			enumerator.HasMoreElements();)
			Insert(p_unIndex++, enumerator.NextElement());
	}

	inline void InsertEnumerator(size_t p_unIndex, TEnumerator<TElement>& p_enumerator)
	{
		if (p_unIndex >= m_unSize)
			throw new MesonException("List index out of range.",
				__FILE__, __LINE__);

		for (p_enumerator.Reset(); p_enumerator.HasMoreElements();)
			Insert(p_unIndex++, p_enumerator.NextElement());
	}

	void RemoveAt(size_t p_unIndex)
	{
		if (p_unIndex >= m_unSize)
			throw new MesonException("List index out of range.",
				__FILE__, __LINE__);

		if (p_unIndex == 0)
		{
			if (m_pTail == m_pHead)
				m_pTail = NULL;
			TLinkedListNode<TElement>* pNode = m_pHead;
			m_pHead = m_pHead->m_pNext;
			if (m_pHead != NULL)
				m_pHead->m_pPrevious = NULL;
			delete pNode;
			m_pLastAccessed = NULL;
			m_unLastAccessed = 0;
		}
		else
		{
			TLinkedListNode<TElement>* pNode = NodeAt(p_unIndex);

			if (m_pTail == pNode)
				m_pTail = pNode->m_pPrevious;

			pNode->m_pPrevious->m_pNext = pNode->m_pNext;
			if (pNode->m_pNext != NULL)
				pNode->m_pNext->m_pPrevious = pNode->m_pPrevious;

			if (p_unIndex <= m_unLastAccessed)
			{
				m_pLastAccessed = NULL;
				m_unLastAccessed = 0;
			}

			delete pNode;
		}

		--m_unSize;
		m_bEnumerating = false;
	}

	void Remove(const TElement& p_element)
	{
		for(TLinkedListNode<TElement>* pNode = m_pHead;
			pNode != NULL; )
		{
			if (!(pNode->m_element == p_element))
			{
				pNode = pNode->m_pNext;
				continue;
			}

			if (pNode == m_pTail)
				m_pTail = m_pTail->m_pPrevious;

			if (pNode == m_pHead)
			{
				m_pHead = m_pHead->m_pNext;
				m_pHead->m_pPrevious = NULL;
			}
			else
			{
				pNode->m_pPrevious->m_pNext = pNode->m_pNext;
				if (pNode->m_pNext != NULL)
					pNode->m_pNext->m_pPrevious = pNode->m_pPrevious;
			}

			TLinkedListNode<TElement>* pNodeToDelete = pNode;
			pNode = pNode->m_pNext;
			delete pNodeToDelete;
			--m_unSize;
		}

		m_pLastAccessed = NULL;
		m_unLastAccessed = 0;

		m_bEnumerating = false;
	}

	void RemoveRange(size_t p_unIndex, size_t p_unLength)
	{
		if (p_unIndex >= m_unSize)
			throw new MesonException("List index out of range.",
				__FILE__, __LINE__);

		if (p_unIndex + p_unLength > m_unSize)
			throw new MesonException("List length out of range.",
				__FILE__, __LINE__);

		m_unSize -= p_unLength;

		TLinkedListNode<TElement>* pNode = NodeAt(p_unIndex);

		while (p_unLength-- > 0)
		{
			if (pNode == m_pTail)
				m_pTail = m_pTail->m_pPrevious;

			if (pNode->m_pPrevious != NULL)
				pNode->m_pPrevious->m_pNext = pNode->m_pNext;
			if (pNode->m_pNext != NULL)
				pNode->m_pNext->m_pPrevious = pNode->m_pPrevious;

			TLinkedListNode<TElement>* pNodeToDelete = pNode;
			pNode = pNode->m_pNext;
			delete pNodeToDelete;
		}

		m_pLastAccessed = NULL;
		m_unLastAccessed = 0;

		m_bEnumerating = false;
	}

	void Reverse(void)
	{
		if (m_unSize <= 1) return;

		m_pTail = m_pHead;

		TLinkedListNode<TElement>* pNode = m_pHead;
		while(pNode != NULL)
		{
			m_pHead = pNode;
			pNode = pNode->m_pNext;
			m_pHead->m_pNext = m_pHead->m_pPrevious;
			m_pHead->m_pPrevious = pNode;
		}

		m_pLastAccessed = NULL;
		m_unLastAccessed = 0;

		m_bEnumerating = false;
	}

	inline TImmutableList<TElement>& AsImmutableList(void) const
	{
		return (TImmutableList<TElement>&) *this;
	}

	inline TEnumerator<TElement>& GetEnumerator(bool p_bReset = true)
	{
		if (p_bReset || !m_bEnumerating)
			m_linkedListEnumerator.Reset();
		m_bEnumerating = true;
		return m_linkedListEnumerator;
	}

	inline typename TEnumerator<TElement>::Ptr CreateEnumerator(void)
	{
		m_bEnumerating = true;
		return TEnumerator<TElement>::Ptr(new TLinkedListEnumerator<TElement>(this, &m_bEnumerating));
	}

	TLinkedList<TElement> operator+(const TList<TElement>& p_list)
	{
		TLinkedList<TElement> listResult(*this);
		listResult.AddList(p_list);
		return listResult;
	}
};

Meson_Common_Collections_END

template<class TElement>
Meson::Common::Collections::TLinkedList<TElement>
	operator+(
		const TElement& p_element,
		const Meson::Common::Collections::TLinkedList<TElement>& p_list)
{
	Meson::Common::Collections::TLinkedList<TElement> listResult(p_list);
	listResult.Insert(0, p_element);
	return listResult;
};
