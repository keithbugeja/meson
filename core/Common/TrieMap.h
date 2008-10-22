#pragma once

//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include <memory>

#include "Map.h"
#include "BufferList.h"

Meson_Common_Collections_BEGIN

Meson::Common::Memory::MemoryManager *GetTrieMapMemoryManager(void);

Meson::Common::Memory::MemoryManager *GetTrieMapNodeMemoryManager(void);

Meson::Common::Memory::MemoryManager *GetTrieMapPairMemoryManager(void);

#pragma region TrieNode

// Forward declaration
template< typename TKey, typename TValue > 
struct TTriePair;



template< typename TKey, typename TValue >
struct TTrieNode
{
protected:
	TTriePair< TKey, TValue >* m_pTriePair[ 0x10 ];

public:
	TTrieNode( void )
	{
		memset( m_pTriePair, 0 , sizeof(TTriePair< TKey, TValue >*) * 0x10 );
	}

	TTrieNode( const TTrieNode &p_trieNode )
	{
		memcpy( m_pTriePair, p_trieNode.m_pTriePair, sizeof(TTriePair< TKey, TValue >*) * 0x10 );
	}

	~TTrieNode( void ) 
	{ 
		Clear(); 
	}


	void* operator new(size_t p_unSize)
	{
		return GetTrieMapNodeMemoryManager()->AllocatePooled(p_unSize);
	}

	void operator delete(void* p_pObject, size_t p_unSize)
	{
		::operator delete(p_pObject);
	}

	inline void Clear( void )
	{
		if( m_pTriePair[0x00] ) delete m_pTriePair[0x00];
		if( m_pTriePair[0x01] ) delete m_pTriePair[0x01];
		if( m_pTriePair[0x02] ) delete m_pTriePair[0x02];
		if( m_pTriePair[0x03] ) delete m_pTriePair[0x03];
		if( m_pTriePair[0x04] ) delete m_pTriePair[0x04];
		if( m_pTriePair[0x05] ) delete m_pTriePair[0x05];
		if( m_pTriePair[0x06] ) delete m_pTriePair[0x06];
		if( m_pTriePair[0x07] ) delete m_pTriePair[0x07];
		if( m_pTriePair[0x08] ) delete m_pTriePair[0x08];
		if( m_pTriePair[0x09] ) delete m_pTriePair[0x09];
		if( m_pTriePair[0x0A] ) delete m_pTriePair[0x0A];
		if( m_pTriePair[0x0B] ) delete m_pTriePair[0x0B];
		if( m_pTriePair[0x0C] ) delete m_pTriePair[0x0C];
		if( m_pTriePair[0x0D] ) delete m_pTriePair[0x0D];
		if( m_pTriePair[0x0E] ) delete m_pTriePair[0x0E];
		if( m_pTriePair[0x0F] ) delete m_pTriePair[0x0F];

		memset( m_pTriePair, 0 , sizeof(TTriePair< TKey, TValue >*) * 0x10 );
	}

	inline TTriePair< TKey, TValue >*& operator[]( byte p_byIndex )
	{
		return *( m_pTriePair + p_byIndex );
	}

	inline TTriePair< TKey, TValue >* operator[]( byte p_byIndex ) const
	{
		return *( m_pTriePair + p_byIndex );
	}

};

#pragma endregion


#pragma region TriePair

template< typename TKey, typename TValue >
struct TTriePair
	: public TKeyValuePair< TKey, TValue >
{
protected:
	TTrieNode< TKey, TValue >** m_ppTrieNode;
	TTriePair* m_pNext;
	TTriePair* m_pPrev;

public:
	TTriePair( void ) :  TKeyValuePair< TKey, TValue >(), m_ppTrieNode( NULL ), m_pNext( NULL ), m_pPrev( NULL ) 
	{}
	
	TTriePair( const TTriePair& p_triePair ) : TKeyValuePair< TKey, TValue >( p_triePair ), m_ppTrieNode( NULL ), m_pNext( NULL ), m_pPrev( NULL ) 
	{}
	
	TTriePair( const TKey& p_key, const TValue& p_value ) : TKeyValuePair< TKey, TValue >( p_key, p_value ), m_ppTrieNode( NULL ), m_pNext( NULL ), m_pPrev( NULL ) 
	{}

	~TTriePair( void )
	{
		Clear();
		if( m_ppTrieNode )
			delete[] m_ppTrieNode;
	}


	void* operator new(size_t p_unSize)
	{
		return GetTrieMapPairMemoryManager()->AllocatePooled(p_unSize);
	}

	void operator delete(void* p_pObject, size_t p_unSize)
	{
		::operator delete(p_pObject);
	}

	inline void Clear( void )
	{
		m_pNext = NULL;
		m_pPrev = NULL;
		if( m_ppTrieNode )
		{
			if( m_ppTrieNode[0x00] ) delete m_ppTrieNode[0x00];
			if( m_ppTrieNode[0x01] ) delete m_ppTrieNode[0x01];
			if( m_ppTrieNode[0x02] ) delete m_ppTrieNode[0x02];
			if( m_ppTrieNode[0x03] ) delete m_ppTrieNode[0x03];
			if( m_ppTrieNode[0x04] ) delete m_ppTrieNode[0x04];
			if( m_ppTrieNode[0x05] ) delete m_ppTrieNode[0x05];
			if( m_ppTrieNode[0x06] ) delete m_ppTrieNode[0x06];
			if( m_ppTrieNode[0x07] ) delete m_ppTrieNode[0x07];
			if( m_ppTrieNode[0x08] ) delete m_ppTrieNode[0x08];
			if( m_ppTrieNode[0x09] ) delete m_ppTrieNode[0x09];
			if( m_ppTrieNode[0x0A] ) delete m_ppTrieNode[0x0A];
			if( m_ppTrieNode[0x0B] ) delete m_ppTrieNode[0x0B];
			if( m_ppTrieNode[0x0C] ) delete m_ppTrieNode[0x0C];
			if( m_ppTrieNode[0x0D] ) delete m_ppTrieNode[0x0D];
			if( m_ppTrieNode[0x0E] ) delete m_ppTrieNode[0x0E];
			if( m_ppTrieNode[0x0F] ) delete m_ppTrieNode[0x0F];
			memset( m_ppTrieNode, 0, sizeof(TTrieNode< TKey, TValue >*) * 0x10 );
		}
	}
	
	inline TValue GetValue( void ) const
	{
		return Value;
	}

	inline void SetValue( const TValue &p_value )
	{
		m_value = p_value;
	}

	inline bool IsActive( void )
	{
		return ( m_pNext != NULL ) || ( m_pPrev != NULL );
	}

	inline bool IsInactive( void )
	{
		return ( m_pNext == NULL ) && ( m_pPrev == NULL );
	}

	inline TTriePair* Remove( void )
	{
		TTriePair* pPrev = GetPrev();
		TTriePair* pNext = GetNext();

		if( pPrev )
			pPrev->SetNext( pNext );
		if( pNext )
			pNext->SetPrev( pPrev );

		SetNext( NULL );
		SetPrev( NULL );

		return pNext;
	}

	inline TTriePair* GetNext( void )
	{
		return m_pNext;
	}

	inline TTriePair* GetPrev( void )
	{
		return m_pPrev;
	}

	inline void SetNext( TTriePair* p_pNext )
	{
		m_pNext = p_pNext;
	}

	inline void SetPrev( TTriePair* p_pPrev )
	{
		m_pPrev = p_pPrev;
	}

	inline TTrieNode< TKey, TValue >*& operator[]( byte p_byIndex )
	{
		if( !m_ppTrieNode )
		{
			m_ppTrieNode = new TTrieNode< TKey, TValue >*[0x10];
			memset( m_ppTrieNode, 0 , sizeof(TTrieNode< TKey, TValue >*) * 0x10 );
		}

		return m_ppTrieNode[ p_byIndex ];
	}

	inline TTrieNode< TKey, TValue >* operator[]( byte p_byIndex ) const
	{
		if( !m_ppTrieNode )
		{
			m_ppTrieNode = new TTrieNode< TKey, TValue >*[0x10];
			memset( m_ppTrieNode, 0 , sizeof(TTrieNode< TKey, TValue >*) * 0x10 );
		}

		return m_ppTrieNode[ p_byIndex ];
	}

	TTriePair& operator=( const TValue &p_value )
	{
		SetValue( p_value );
		return *this;
	}

};

#pragma endregion

#pragma region TrieEnumerators


// Forward Referencing
template< typename TKey, typename TValue >
class TTrieMap;



template< class TKey, class TValue >
class TTriePairEnumerator
	: public TEnumerator< TKeyValuePair< TKey, TValue > >
{
private:
	TTrieMap< TKey, TValue >* m_pMap;
	TTriePair< TKey, TValue >* m_pPair;
	size_t m_unSize;
	size_t m_unAvailable;

public:
	TTriePairEnumerator( TTrieMap< TKey, TValue >* p_pMap ) 
		: m_pMap(p_pMap)
		, m_pPair(p_pMap->GetRoot())
		, m_unSize(p_pMap->Size())
		, m_unAvailable(p_pMap->Size())
	{
	}

	~TTriePairEnumerator(void) 
	{
	}

	void* operator new(size_t p_unSize)
	{
		return GetTrieMapMemoryManager()->AllocatePooled(p_unSize);
	}

	void operator delete(void* p_pObject, size_t p_unSize)
	{
		::operator delete(p_pObject);
	}

	inline bool HasMoreElements( void )
	{
		if( m_unSize != m_pMap->Size() )
			throw new MesonException("TrieMap was modified during enumeration",
				__FILE__, __LINE__);

		return ( m_pPair->GetNext() != NULL );
	}

	inline void Reset( void )
	{
		m_pPair = m_pMap->GetRoot();
		m_unSize = m_pMap->Size();
		m_unAvailable = m_unSize;
	}

	inline TKeyValuePair< TKey, TValue >& NextElement( void )
	{
		// CV - what if item added and then deleted?
		if( m_unSize != m_pMap->Size() )
			throw new MesonException("Trie Map was modified during enumeration",
				__FILE__, __LINE__);

		m_pPair = m_pPair->GetNext();

		if( m_pPair == NULL )
			throw new MesonException("No more elements to enumerate.",
				__FILE__, __LINE__);

		--m_unAvailable;
		return *m_pPair;
	}

	inline size_t Size( void ) const
	{
		return m_unSize;
	}

	inline size_t Available( void ) const
	{
		return m_unAvailable;
	}
};


template<class TKey, class TValue>
class TTrieKeyEnumerator
	: public TEnumerator< TKey >
{
private:
	TTriePairEnumerator< TKey, TValue > m_pairEnumerator;

public:
	TTrieKeyEnumerator( TTrieMap< TKey, TValue >* p_pMap ) : m_pairEnumerator( p_pMap ) 
	{
	}

	~TTrieKeyEnumerator( void )
	{
	}
	
	inline bool HasMoreElements( void )
	{
		return m_pairEnumerator.HasMoreElements();
	}

	inline void Reset( void )
	{
		m_pairEnumerator.Reset();
	}

	inline TKey& NextElement( void )
	{
		return m_pairEnumerator.NextElement().Key;
	}

	inline size_t Size( void ) const
	{
		return m_pairEnumerator.Size();
	}

	inline size_t Available( void ) const
	{
		return m_pairEnumerator.Available();
	}
};

template<class TKey, class TValue>
class TTrieValueEnumerator
	: public TEnumerator< TValue >
{
private:
	TTriePairEnumerator< TKey, TValue > m_pairEnumerator;

public:
	TTrieValueEnumerator( TTrieMap< TKey, TValue >* p_pMap ) : m_pairEnumerator( p_pMap ) 
	{
	}

	~TTrieValueEnumerator( void )
	{
	}

	inline bool HasMoreElements( void )
	{
		return m_pairEnumerator.HasMoreElements();
	}

	inline void Reset( void )
	{
		m_pairEnumerator.Reset();
	}

	inline TValue& NextElement( void )
	{
		return m_pairEnumerator.NextElement().Value;
	}

	inline size_t Size( void ) const
	{
		return m_pairEnumerator.Size();
	}

	inline size_t Available( void ) const
	{
		return m_pairEnumerator.Available();
	}
};

#pragma endregion 

#pragma region TrieMap

template< typename TKey, typename TValue >
class TTrieMap
	: public TMap< TKey, TValue >
{

protected:
	typedef TTrieNode< TKey, TValue> TrieNode;
	typedef TTriePair< TKey, TValue> TriePair;

protected:
	size_t m_unSize;
	TriePair m_root;
	TriePair* m_pLastPair;

	TTriePairEnumerator< TKey, TValue >  m_pairEnumerator;
	TTrieKeyEnumerator< TKey, TValue >   m_keyEnumerator;
	TTrieValueEnumerator< TKey, TValue > m_valueEnumerator;


public:
	TTrieMap( void ) 
		: m_unSize( 0 )
		, m_root()
		, m_pLastPair( &m_root )
		, m_pairEnumerator( this )
		, m_keyEnumerator( this )
		, m_valueEnumerator( this )
	{
	}

	TTrieMap( const TImmutableMap< TKey, TValue >& p_map ) 
		: m_unSize( 0 )
		, m_root()
		, m_pLastPair( &m_root )
		, m_pairEnumerator( this )
		, m_keyEnumerator( this )
		, m_valueEnumerator( this )
	{
		InsertMap( p_map );
	}

	TTrieMap( const TMap< TKey, TValue >& p_map ) 
		: m_unSize( 0 )
		, m_root()
		, m_pLastPair( &m_root )
		, m_pairEnumerator( this )
		, m_keyEnumerator( this )
		, m_valueEnumerator( this )
	{
		InsertMap( p_map );
	}

	TTrieMap( const TTrieMap< TKey, TValue >& p_map ) 
		: m_unSize( 0 )
		, m_root()
		, m_pLastPair( &m_root )
		, m_pairEnumerator( this )
		, m_keyEnumerator( this )
		, m_valueEnumerator( this )
	{
		InsertMap( p_map );
	}



	~TTrieMap( void )
	{

	}

protected:// Internal Methods
	inline TriePair* GetPair( const byte* p_pbyKey, size_t p_unKeySize )
	{
		byte byIndex;
		
		// Root Node
		TrieNode* pNode = NULL;
		TriePair* pPair = &m_root;

		for( ; p_unKeySize > 0; p_unKeySize--, p_pbyKey++ )
		{
			byIndex = ( (*p_pbyKey) & 0xF0 ) >> 4;

			pNode = (*pPair)[ byIndex ];
			if( !pNode )
				return NULL;

			byIndex = ( (*p_pbyKey) & 0x0F );
			pPair = (*pNode)[ byIndex ];
			if( !pPair )
				return NULL;
		}

		return pPair;
	}

	inline TriePair* PutPair( const byte* p_pbyKey, size_t p_unKeySize )
	{
		byte byIndex;
		
		// Root Node
		TrieNode* pNode = NULL;
		TriePair* pPair = &m_root;
		
		for( ; p_unKeySize > 0; p_unKeySize--, p_pbyKey++ )
		{
			byIndex = ( (*p_pbyKey) & 0xF0 ) >> 4;

			pNode = (*pPair)[ byIndex ];

			if( pNode )
			{
				byIndex = ( (*p_pbyKey) & 0x0F );
				pPair = (*pNode)[ byIndex ];
				if(!pPair)
				{
					pPair = new TriePair();
					(*pNode)[ byIndex ] = pPair;
					continue;
				}
			}
			else
			{
				pNode = new TrieNode();
				(*pPair)[ byIndex ] = pNode;

				byIndex = ( (*p_pbyKey) & 0x0F );
				pPair = new TriePair();
				(*pNode)[ byIndex ] = pPair;
			}

		}

		return pPair;
	}



public: // TMap
	inline TriePair* GetRoot()
	{
		return &m_root;
	}

	inline void Clear( void )
	{
		m_root.Clear();
		m_pLastPair = &m_root;
		m_unSize = 0;
	}

	inline TValue& operator[]( const TKey& p_key )
	{
		size_t unSize = 0;
		const byte* pbyKey = FlattenKey( p_key, unSize );

		TriePair* pPair = GetPair( pbyKey, unSize );

		if( pPair && pPair->IsActive() )
			return pPair->Value;
		else
			throw new MesonException("Key not found in TrieMap.", __FILE__, __LINE__);
	}

	inline TValue& operator()( const TKey& p_key )
	{
		size_t unSize = 0;
		const byte* pbyKey = FlattenKey( p_key, unSize );

		TriePair* pPair = GetPair( pbyKey, unSize );

		if( pPair && pPair->IsActive() )
			return pPair->Value;
		else
			throw new MesonException("Key not found in TrieMap.", __FILE__, __LINE__);
	}
	

	inline TTrieMap< TKey, TValue >& operator=( const TMap< TKey, TValue >& p_map )
	{
		if( &p_map == this ) 
			return *this;

		Clear();
		InsertMap( p_map );
		return *this;
	}

	inline TTrieMap< TKey, TValue >& operator=( const TTrieMap< TKey, TValue >& p_map )
	{
		if( &p_map == this ) 
			return *this;

		Clear();
		InsertMap( p_map );
		return *this;
	}

	inline TTrieMap< TKey, TValue > operator-( const TKey& p_key )
	{
		TTrieMap< TKey, TValue > mapNew;
		mapNew.InsertMap( *this );
		mapNew.Remove( p_key );
		return mapNew;
	}



	inline void Insert( const TKey& p_key, const TValue& p_value )
	{
		size_t unSize = 0;
		const byte* pbyKey = FlattenKey( p_key, unSize );

		TriePair* pPair = PutPair( pbyKey, unSize );

		pPair->Key = p_key;
		pPair->Value = p_value;

		if( pPair->IsInactive() )
		{
			pPair->SetPrev( m_pLastPair );
			m_pLastPair->SetNext( pPair );
			m_pLastPair = pPair;
		}
		m_unSize++;
	}

	inline void InsertMap( const TImmutableMap<TKey, TValue>& p_map )
	{
		TEnumerator<TKey> &enumerator = ((TImmutableMap<TKey, TValue> &)p_map).GetKeyEnumerator();
		for( ; enumerator.HasMoreElements(); )
		{
			TKey &key = enumerator.NextElement();
			Insert(key, p_map.ValueAt(key));
		}
	}
	
	inline void RemoveKey( const TKey& p_key )
	{
		size_t unSize = 0;
		const byte* pbyKey = FlattenKey( p_key, unSize );

		TriePair* pPair = GetPair( pbyKey, unSize );

		if( pPair && pPair->IsActive() )
		{
			pPair->Remove();
			m_unSize--;
		}
	}

	inline void RemoveValue( const TValue& p_value )
	{
		TriePair* pNext = m_root.GetNext();

		while( pNext )
		{
			if( pNext->GetValue() == p_value )
				pNext = pNext->Remove();
			else
				pNext = pNext->GetNext();
		}
	}
	
	inline TImmutableMap<TKey, TValue>& AsImmutableMap( void ) const
	{
		return ( TImmutableMap< TKey, TValue >& ) *this;
	}


public: // TImmutableMap
	inline bool ContainsKey( const TKey& p_key ) const
	{
		size_t unSize = 0;
		const byte* pbyKey = FlattenKey( p_key, unSize );

		TriePair* pPair = ((TTrieMap*)this)->GetPair( pbyKey, unSize );

		return ( pPair && pPair->IsActive() );
	}

	inline bool ContainsKey( const TKey& p_key, TValue& p_value ) const
	{
		size_t unSize = 0;
		const byte* pbyKey = FlattenKey( p_key, unSize );

		TriePair* pPair = ((TTrieMap*)this)->GetPair( pbyKey, unSize );
		
		if( pPair && pPair->IsActive() )
		{
			p_value = pPair->Value;
			return true;
		}
		
		return false;
	}

	inline const TValue& ValueAt( const TKey& p_key ) const
	{
		size_t unSize = 0;
		const byte* pbyKey = FlattenKey( p_key, unSize );

		TriePair* pPair = ((TTrieMap*)this)->GetPair( pbyKey, unSize );

		if( pPair && pPair->IsActive() )
			return pPair->Value;
		else
			throw new MesonException("Key not found in TrieMap.", __FILE__, __LINE__);
	}

	inline TEnumerator<TKey>& GetKeyEnumerator( bool p_bReset = true )
	{
		if( p_bReset )
			m_keyEnumerator.Reset();
		
		return m_keyEnumerator;
	}

	inline typename TEnumerator<TKey>::Ptr CreateKeyEnumerator( void )
	{
		return TEnumerator< TKey >::Ptr(
			new TTrieKeyEnumerator<TKey, TValue>( this ) );
	}

	inline TEnumerator<TValue>& GetValueEnumerator( bool p_bReset = true )
	{
		if( p_bReset )
			m_valueEnumerator.Reset();
		
		return m_valueEnumerator;
	}

	inline typename TEnumerator<TValue>::Ptr CreateValueEnumerator( void )
	{
		return TEnumerator< TValue >::Ptr(
			new TTrieValueEnumerator<TKey, TValue>( this ) );
	}

	inline size_t Size( void ) const
	{
		return m_unSize;
	}

	inline TEnumerator< TKeyValuePair< TKey, TValue > >& GetEnumerator( bool p_bReset = true )
	{
		if( p_bReset )
			m_pairEnumerator.Reset();
		
		return m_pairEnumerator;
	}

	inline typename TEnumerator< TKeyValuePair< TKey, TValue > >::Ptr CreateEnumerator( void )
	{
		return TEnumerator< TKeyValuePair< TKey, TValue > >::Ptr(
			new TTriePairEnumerator<TKey, TValue>( this ) );
	}

};

#pragma endregion


Meson_Common_Collections_END


template< class TKey >
inline const byte* FlattenKey( const TKey& p_key, size_t& p_size )
{
	p_size = sizeof( TKey );
	return (const byte*)&p_key;
}

template< typename TKey >
inline const byte* FlattenKey( const TKey* p_pKey, size_t& p_size )
{
	p_size = sizeof( TKey* );
	return (const byte*)p_pKey;
}

inline const byte* FlattenKey( const Meson::Common::Text::String& p_strKey, size_t& p_size )
{
	p_size = p_strKey.Size();
	return (const byte*)(p_strKey.GetBuffer());
}

