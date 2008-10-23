#pragma once

#include "Map.h"
#include "MesonException.h"

#pragma warning(disable:4355)

Meson_Common_Collections_BEGIN

Meson::Common::Memory::MemoryManager* GetHashMapMemoryManager(void);

Meson::Common::Memory::MemoryManager* GetHashMapNodeMemoryManager(void);

ulong HashDataGeneric(const void* p_pData, size_t p_unLength);

template<class TKey>
ulong HashData(const TKey& p_key)
{
	return HashDataGeneric(&p_key, sizeof(TKey));
};

ulong HashData(const Meson::Common::Text::String& p_strKey);

template<class TKey, class TValue>
struct THashMapNode
{
	TKeyValuePair<TKey, TValue> m_keyValuePair;
	THashMapNode* m_pNext;

	THashMapNode(void)
		: m_keyValuePair()
		, m_pNext(NULL)
	{
	}

	THashMapNode(const TKey& p_key, const TValue& p_value)
		: m_keyValuePair(p_key, p_value)
		, m_pNext(NULL)
	{
	}

	void* operator new(size_t p_unSize)
	{
		return GetHashMapNodeMemoryManager()->AllocatePooled(p_unSize);
	}

	void operator delete(void* p_pObject, size_t p_unSize)
	{
		::operator delete(p_pObject);
	}
};

// forward declaration
template<class TKey, class TValue>
class THashMap;

template<class TKey, class TValue>
class THashMapPairEnumerator
	: public TEnumerator< TKeyValuePair<TKey, TValue> >
{
private:
	THashMap<TKey, TValue>* m_pHashMap;
	bool* m_pbEnumerating;
	size_t m_unBucketEnumerator;
	THashMapNode<TKey, TValue>* m_pNodeEnumerator;
	size_t m_unAvailable;

public:
	THashMapPairEnumerator(
		THashMap<TKey, TValue>* p_pHashMap,
		bool* p_pbEnumerating)
		: m_pHashMap(p_pHashMap)
		, m_pbEnumerating(p_pbEnumerating)
		, m_unBucketEnumerator(0)
		, m_pNodeEnumerator(NULL)
		, m_unAvailable(p_pHashMap->Size())
	{
	}

	bool HasMoreElements(void)
	{
		if( !*m_pbEnumerating )
			throw new MesonException("Map was modified during enumeration",
				__FILE__, __LINE__);

		return (m_unBucketEnumerator < m_pHashMap->m_unBucketSize)
			&& (m_pNodeEnumerator != NULL);
	}

	void Reset(void)
	{
		m_unBucketEnumerator = 0;
		m_pNodeEnumerator = NULL;
		while (m_unBucketEnumerator < m_pHashMap->m_unBucketSize)
			if (m_pHashMap->m_pBuckets[m_unBucketEnumerator] == NULL)
				++m_unBucketEnumerator;
			else
			{
				m_pNodeEnumerator = m_pHashMap->m_pBuckets[m_unBucketEnumerator];
				break;
			}

		m_unAvailable = m_pHashMap->Size();
		*m_pbEnumerating = true;
	}

	TKeyValuePair<TKey, TValue>& NextElement(void)
	{
		if( !*m_pbEnumerating )
			throw new MesonException("Map was modified during enumeration",
				__FILE__, __LINE__);

		if( m_pNodeEnumerator == NULL )
			throw new MesonException("No more elements to enumerate.",
				__FILE__, __LINE__);


		TKeyValuePair<TKey, TValue>& keyValuePair
			= m_pNodeEnumerator->m_keyValuePair;

		m_pNodeEnumerator = m_pNodeEnumerator->m_pNext;
		if (m_pNodeEnumerator == NULL)
			while (++m_unBucketEnumerator < m_pHashMap->m_unBucketSize)
				if (m_pHashMap->m_pBuckets[m_unBucketEnumerator] != NULL)
				{
					m_pNodeEnumerator = m_pHashMap->m_pBuckets[m_unBucketEnumerator];
					break;
				}

		--m_unAvailable;
		return keyValuePair;
	}

	size_t Size(void) const
	{
		return m_pHashMap->Size();
	}

	size_t Available(void) const
	{
		return m_unAvailable;
	}
};

template<class TKey, class TValue>
class THashMapKeyEnumerator
	: public TEnumerator<TKey>
{
private:
	THashMapPairEnumerator<TKey, TValue> m_hashMapPairEnumerator;

public:
	THashMapKeyEnumerator(
		THashMap<TKey, TValue>* p_pHashMap, bool* p_pbEnumerating)
		: m_hashMapPairEnumerator(p_pHashMap, p_pbEnumerating)
	{
	}

	~THashMapKeyEnumerator(void)
	{
	}

	inline bool HasMoreElements(void)
	{
		return m_hashMapPairEnumerator.HasMoreElements();
	}

	inline void Reset(void)
	{
		return m_hashMapPairEnumerator.Reset();
	}

	inline TKey& NextElement(void)
	{
		return m_hashMapPairEnumerator.NextElement().Key;
	}

	inline size_t Size(void) const
	{
		return m_hashMapPairEnumerator.Size();
	}

	inline size_t Available(void) const
	{
		return m_hashMapPairEnumerator.Available();
	}
};

template<class TKey, class TValue>
class THashMapValueEnumerator
	: public TEnumerator<TValue>
{
private:
	THashMapPairEnumerator<TKey, TValue> m_hashMapPairEnumerator;

public:
	THashMapValueEnumerator(
		THashMap<TKey, TValue>* p_pHashMap, bool* p_pbEnumerating)
		: m_hashMapPairEnumerator(p_pHashMap, p_pbEnumerating)
	{
	}

	~THashMapValueEnumerator(void)
	{
	}

	bool HasMoreElements(void)
	{
		return m_hashMapPairEnumerator.HasMoreElements();
	}

	void Reset(void)
	{
		return m_hashMapPairEnumerator.Reset();
	}

	TValue& NextElement(void)
	{
		return m_hashMapPairEnumerator.NextElement().Value;
	}

	size_t Size(void) const
	{
		return m_hashMapPairEnumerator.Size();
	}

	inline size_t Available(void) const
	{
		return m_hashMapPairEnumerator.Available();
	}
};

template<class TKey, class TValue>
class THashMap
	: public TMap<TKey, TValue>
{
	friend THashMapPairEnumerator<TKey, TValue>;

private: // private variables
	size_t m_unBucketSize;
	float m_fLoadFactor;
	THashMapNode<TKey, TValue>** m_pBuckets;
	size_t m_unSize;
	bool m_bEnumerating;
	THashMapKeyEnumerator<TKey, TValue> m_keyEnumerator;
	THashMapValueEnumerator<TKey, TValue> m_valueEnumerator;
	THashMapPairEnumerator<TKey, TValue> m_pairEnumerator;

private: // private methods
	THashMapNode<TKey, TValue>* CloneNode(THashMapNode<TKey, TValue>* p_pNode)
	{
		THashMapNode<TKey, TValue>* pNodeClone
			= new THashMapNode<TKey, TValue>(p_pNode->m_key, p_pNode->m_value);
		if (p_pNode->m_pNext != NULL)
			pNodeClone->m_pNext = CloneNode(p_pNode->m_pNext);
		return pNodeClone;
	}

	void Rehash(size_t p_unNewBucketSize)
	{
		THashMapNode<TKey, TValue>** pBuckets
			= new THashMapNode<TKey, TValue>* [p_unNewBucketSize];
		for (size_t unIndex = 0; unIndex < p_unNewBucketSize; unIndex++)
			pBuckets[unIndex] = NULL;

		for (size_t unIndex = 0; unIndex < m_unBucketSize; unIndex++)
		{
			THashMapNode<TKey, TValue>* pNode = m_pBuckets[unIndex];
			if (pNode == NULL) continue;

			while (pNode != NULL)
			{
				THashMapNode<TKey, TValue>* pNodeNext = pNode->m_pNext;
				pNode->m_pNext = NULL;
				uint unNewBucket
					= HashData(pNode->m_keyValuePair.Key) % p_unNewBucketSize;

				THashMapNode<TKey, TValue>* pNewHead
					= pBuckets[unNewBucket];

				if (pNewHead == NULL)
					pBuckets[unNewBucket] = pNode;
				else
				{
					while (pNewHead->m_pNext != NULL)
						pNewHead = pNewHead->m_pNext;
					pNewHead->m_pNext = pNode;
				}

				pNode = pNodeNext;
			}
		}

		delete m_pBuckets;
		m_pBuckets = pBuckets;
		m_unBucketSize = p_unNewBucketSize;
	}

public: // public methods
	THashMap(void)
		: m_unBucketSize(16)
		, m_fLoadFactor(0.75f)
		, m_unSize(0)
		, m_bEnumerating(false)
		, m_keyEnumerator(this, &m_bEnumerating)
		, m_valueEnumerator(this, &m_bEnumerating)
		, m_pairEnumerator(this, &m_bEnumerating)
	{
		m_pBuckets = new THashMapNode<TKey, TValue>*[m_unBucketSize];
		for (size_t unIndex = 0; unIndex < m_unBucketSize; unIndex++)
			m_pBuckets[unIndex] = NULL;
	}

	THashMap(size_t p_unBucketSize)
		: m_unBucketSize(p_unBucketSize)
		, m_fLoadFactor(0.75f)
		, m_unSize(0)
		, m_bEnumerating(false)
		, m_keyEnumerator(this, &m_bEnumerating)
		, m_valueEnumerator(this, &m_bEnumerating)
		, m_pairEnumerator(this, &m_bEnumerating)
	{
		m_pBuckets = new THashMapNode<TKey, TValue>*[m_unBucketSize];
		for (size_t unIndex = 0; unIndex < m_unBucketSize; unIndex++)
			m_pBuckets[unIndex] = NULL;
	}

	THashMap(size_t p_unBucketSize, float p_fLoadFactor)
		: m_unBucketSize(p_unBucketSize)
		, m_fLoadFactor(p_fLoadFactor)
		, m_unSize(0)
		, m_bEnumerating(false)
		, m_keyEnumerator(this, &m_bEnumerating)
		, m_valueEnumerator(this, &m_bEnumerating)
		, m_pairEnumerator(this, &m_bEnumerating)
	{
		m_pBuckets = new THashMapNode<TKey, TValue>*[m_unBucketSize];
		for (size_t unIndex = 0; unIndex < m_unBucketSize; unIndex++)
			m_pBuckets[unIndex] = NULL;
	}

	THashMap(const TImmutableMap<TKey, TValue>& p_map)
		: m_unBucketSize((size_t) (p_map.Size() * 0.75))
		, m_fLoadFactor(0.75f)
		, m_unSize(0)
		, m_bEnumerating(false)
		, m_keyEnumerator(this, &m_bEnumerating)
		, m_valueEnumerator(this, &m_bEnumerating)
		, m_pairEnumerator(this, &m_bEnumerating)
	{
		m_pBuckets = new THashMapNode<TKey, TValue>*[m_unBucketSize];
		for (size_t unIndex = 0; unIndex < m_unBucketSize; unIndex++)
			m_pBuckets[unIndex] = NULL;
		InsertMap(p_map);
	}

	THashMap(const TMap<TKey, TValue>& p_map)
		: m_unBucketSize((size_t) (p_map.Size() * 0.75))
		, m_fLoadFactor(0.75f)
		, m_unSize(0)
		, m_bEnumerating(false)
		, m_keyEnumerator(this, &m_bEnumerating)
		, m_valueEnumerator(this, &m_bEnumerating)
		, m_pairEnumerator(this, &m_bEnumerating)
	{
		m_pBuckets = new THashMapNode<TKey, TValue>*[m_unBucketSize];
		for (size_t unIndex = 0; unIndex < m_unBucketSize; unIndex++)
			m_pBuckets[unIndex] = NULL;
		InsertMap(p_map);
	}

	THashMap(const THashMap<TKey, TValue>& p_map)
		: m_unBucketSize((size_t) (p_map.Size() * 0.75))
		, m_fLoadFactor(0.75f)
		, m_unSize(0)
		, m_bEnumerating(false)
		, m_keyEnumerator(this, &m_bEnumerating)
		, m_valueEnumerator(this, &m_bEnumerating)
		, m_pairEnumerator(this, &m_bEnumerating)
	{
		m_pBuckets = new THashMapNode<TKey, TValue>* [m_unBucketSize];
		for (size_t unIndex = 0; unIndex < m_unBucketSize; unIndex++)
			m_pBuckets[unIndex] = NULL;
		InsertMap(p_map);
	}

	~THashMap(void)
	{
		Clear();
	}

	void* operator new(size_t p_unSize)
	{
		return GetHashMapMemoryManager()->AllocatePooled(p_unSize);
	}

	void operator delete(void* p_pObject, size_t p_unSize)
	{
		::operator delete(p_pObject);
	}							

	inline size_t Size(void) const
	{
		return m_unSize;
	}

	inline bool ContainsKey(const TKey& p_key) const
	{
		uint nBucket
			= HashData(p_key) % m_unBucketSize;

		THashMapNode<TKey, TValue>* pHead = m_pBuckets[nBucket];
		while (pHead != NULL)
		{
			if (pHead->m_keyValuePair.Key == p_key) return true;
			pHead = pHead->m_pNext;
		}

		return false;
	}

	inline bool ContainsKey(const TKey& p_key, TValue& p_value) const
	{
		uint nBucket
			= HashData(p_key) % m_unBucketSize;

		THashMapNode<TKey, TValue>* pHead = m_pBuckets[nBucket];
		while (pHead != NULL)
		{
			if (pHead->m_keyValuePair.Key == p_key)
			{
				p_value = pHead->m_keyValuePair.Value;
				return true;
			}
			pHead = pHead->m_pNext;
		}

		return false;
	}

	inline const TValue& ValueAt(const TKey& p_key) const
	{
		uint nBucket
			= HashData(p_key) % m_unBucketSize;

		THashMapNode<TKey, TValue>* pHead = m_pBuckets[nBucket];
		while (pHead != NULL)
		{
			if (pHead->m_keyValuePair.Key == p_key)
				return pHead->m_keyValuePair.Value;
			pHead = pHead->m_pNext;
		}
		
		throw new MesonException("Key not found in HashMap.",
			__FILE__, __LINE__);
	}

	TEnumerator<TKey>& GetKeyEnumerator(bool p_bReset = true)
	{
		if (p_bReset || !m_bEnumerating)
			m_keyEnumerator.Reset();
		return m_keyEnumerator;
	}

	inline typename TEnumerator<TKey>::Ptr CreateKeyEnumerator(void)
	{
		m_bEnumerating = true;
		return TEnumerator<TKey>::Ptr(
			new THashMapKeyEnumerator<TKey, TValue>(this, &m_bEnumerating));
	}

	TEnumerator<TValue>& GetValueEnumerator(bool p_bReset = true)
	{
		if (p_bReset || !m_bEnumerating)
			m_valueEnumerator.Reset();
		return m_valueEnumerator;
	}

	inline typename TEnumerator<TValue>::Ptr CreateValueEnumerator(void)
	{
		m_bEnumerating = true;
		return TEnumerator<TValue>::Ptr(
			new THashMapValueEnumerator<TKey, TValue>(this, &m_bEnumerating));
	}

	TEnumerator< TKeyValuePair<TKey, TValue> >& GetEnumerator(bool p_bReset = true)
	{
		if (p_bReset || !m_bEnumerating)
			m_pairEnumerator.Reset();
		return m_pairEnumerator;
	}

	inline typename TEnumerator<TKeyValuePair<TKey, TValue>>::Ptr CreateEnumerator(void)
	{
		m_bEnumerating = true;
		return TEnumerator< TKeyValuePair<TKey, TValue> >::Ptr(
			new THashMapPairEnumerator<TKey, TValue>(this, &m_bEnumerating));
	}

	inline TMap<TKey, TValue>& operator=(const TMap<TKey, TValue>& p_map)
	{
		if (&p_map == this) return *this;

		Clear();
		delete[] m_pBuckets;
		m_unBucketSize = p_map.Size() * m_fLoadFactor;
		m_pBuckets = new THashMapNode<TKey, TValue>*[m_unBucketSize];
		for (size_t unIndex = 0; unIndex < m_unBucketSize; unIndex++)
			m_pBuckets[unIndex] = NULL;

		AddList(p_map);
		return *this;
	}

	inline THashMap<TKey, TValue> operator-(const TKey& p_key)
	{
		THashMap<TKey, TValue> mapNew;
		mapNew.AddList(*this);
		mapNew.Remove(p_key);
		return mapNew;
	}

	inline void Clear(void)
	{
		for (size_t unIndex = 0; unIndex < m_unBucketSize; unIndex++)
		{
			THashMapNode<TKey, TValue> *pNode
				= m_pBuckets[unIndex];
			if (pNode == NULL) continue;
			while (pNode != NULL)
			{
				THashMapNode<TKey, TValue>* pNodeToDelete = pNode;
				pNode = pNode->m_pNext;
				delete pNodeToDelete;
			}
			m_pBuckets[unIndex] = NULL;
		}

		m_unSize = 0;
		m_bEnumerating = false;
	}

	inline TValue& operator[](const TKey& p_key)
	{
		uint nBucket
			= HashData(p_key) % m_unBucketSize;

		THashMapNode<TKey, TValue> *pHead = m_pBuckets[nBucket];
		while (pHead != NULL)
		{
			if (pHead->m_keyValuePair.Key == p_key)
				return pHead->m_keyValuePair.Value;
			pHead = pHead->m_pNext;
		}

		TValue value;
		Insert(p_key, value);

		// recompute bucket in case of expansion
		nBucket = HashData(p_key) % m_unBucketSize;

		pHead = m_pBuckets[nBucket];
		while (true)
		{
			if (pHead->m_keyValuePair.Key == p_key)
				return pHead->m_keyValuePair.Value;
			pHead = pHead->m_pNext;
		}
	}

	inline TValue& operator()(const TKey& p_key)
	{
		uint nBucket
			= HashData(p_key) % m_unBucketSize;

		THashMapNode<TKey, TValue>* pHead = m_pBuckets[nBucket];
		while (pHead != NULL)
		{
			if (pHead->m_keyValuePair.Key == p_key)
				return pHead->m_keyValuePair.Value;
			pHead = pHead->m_pNext;
		}

		TValue value;
		Insert(p_key, value);

		// recompute bucket in case of expansion
		nBucket = HashData(p_key) % m_unBucketSize;

		pHead = m_pBuckets[nBucket];
		while (true)
		{
			if (pHead->m_keyValuePair.Key == p_key)
				return pHead->m_keyValuePair.Value;
			pHead = pHead->m_pNext;
		}
	}

	void Insert(const TKey& p_key, const TValue& p_value)
	{
		if (m_unSize > (size_t)(m_unBucketSize * m_fLoadFactor))
		{
			Rehash(m_unBucketSize * 2);
		}

		uint nBucket
			= HashData(p_key) % m_unBucketSize;

		THashMapNode<TKey, TValue>* pHead = m_pBuckets[nBucket];

		if (pHead == NULL)
		{
			pHead = new THashMapNode<TKey, TValue>(p_key, p_value);
			m_pBuckets[nBucket] = pHead;
			++m_unSize;
			m_bEnumerating = false;
			return;
		}

		while (pHead->m_pNext != NULL)
		{
			pHead = pHead->m_pNext;
			if (pHead->m_keyValuePair.Key == p_key)
			{
				pHead->m_keyValuePair.Value = p_value;
				m_bEnumerating = false;
				return;
			}
		}

		pHead->m_pNext
			= new THashMapNode<TKey, TValue>(p_key, p_value);
		++m_unSize;
		m_bEnumerating = false;
	}

	inline void InsertMap(const TImmutableMap<TKey, TValue>& p_map)
	{
		for (TEnumerator<TKey>& enumerator
			= ((TImmutableMap<TKey, TValue>&)p_map).GetKeyEnumerator();
			enumerator.HasMoreElements();)
		{
			TKey& key = enumerator.NextElement();
			Insert(key, p_map.ValueAt(key));
		}
	}

	void RemoveKey(const TKey& p_key)
	{
		uint nBucket
			= HashData(p_key) % m_unBucketSize;

		THashMapNode<TKey, TValue>* pHead = m_pBuckets[nBucket];

		if (pHead == NULL) return;

		THashMapNode<TKey, TValue>* pPrevious = NULL;
		while (pHead != NULL)
		{
			if (pHead->m_keyValuePair.Key == p_key)
			{
				if (pPrevious == NULL)
					m_pBuckets[nBucket] = NULL;
				else
					pPrevious->m_pNext = pHead->m_pNext;
				delete pHead;
				--m_unSize;
				m_bEnumerating = false;
				return;
			}
			pPrevious = pHead;
			pHead = pHead->m_pNext;
		}
	}

	void RemoveValue(const TValue& p_value)
	{
		TEnumerator<TKey>& enumKeys = GetKeyEnumerator();
		while(enumKeys.HasMoreElements())
		{
			TKey& key = enumKeys.NextElement();
			if (ValueAt(key) == p_value)
			{
				RemoveKey(key);
				enumKeys.Reset();
			}
		}
	}

	inline TImmutableMap<TKey, TValue>& AsImmutableMap(void) const
	{
		return (TImmutableMap<TKey, TValue>&) *this;
	}

	TKeyValuePair<TKey, TValue>& NextElementPair(void)
	{
		if( !m_bEnumerating )
			throw new MesonException("List was modified during enumeration",
				__FILE__, __LINE__);

		if( !HasMoreElements() )
			throw new MesonException("No more elements to enumerate.",
				__FILE__, __LINE__);

		TKeyValuePair<TKey, TValue>& keyValuePair
			= m_pNodeEnumerator->m_keyValuePair;

		m_pNodeEnumerator = m_pNodeEnumerator->m_pNext;
		if (m_pNodeEnumerator == NULL)
			while (++m_unBucketEnumerator < m_unBucketSize)
				if (m_pBuckets[m_unBucketEnumerator] != NULL)
				{
					m_pNodeEnumerator = m_pBuckets[m_unBucketEnumerator];
					break;
				}

		return keyValuePair;
	};

	TKey& NextElement(void)
	{
		return NextElementPair().m_key;
	};
};

Meson_Common_Collections_END
