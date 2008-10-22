#pragma once

//----------------------------------------------------------------------------------------------
//	
//	
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Gordon Mangion 	07/09/2007	1.0.0		Initial version.
//----------------------------------------------------------------------------------------------

#include "TrieMap.h"

Meson_Common_Collections_BEGIN

template< typename TValue > class TStringMap
	: public TTrieMap< Meson::Common::Text::String, TValue >
{
public:
	inline TValue& operator[]( const Meson::Common::Text::String& p_strKey )
	{
		TriePair* pPair = GetPair( (const byte*)p_strKey.GetBuffer(), p_strKey.Size() );

		if( pPair && pPair->IsActive() )
			return pPair->Value;
		else
			throw new MesonException("Key not found in TrieMap.", __FILE__, __LINE__);
	}

	inline TValue& operator()( const Meson::Common::Text::String& p_strKey )
	{
		TriePair* pPair = GetPair( (const byte*)p_strKey.GetBuffer(), p_strKey.Size() );

		if( pPair && pPair->IsActive() )
			return pPair->Value;
		else
			throw new MesonException("Key not found in TrieMap.", __FILE__, __LINE__);
	}
	

	inline TTrieMap< Meson::Common::Text::String, TValue >& operator=( const TMap< Meson::Common::Text::String, TValue >& p_map )
	{
		if( &p_map == this ) 
			return *this;

		Clear();
		InsertMap( p_map );
		return *this;
	}

	inline TTrieMap< Meson::Common::Text::String, TValue >& operator=( const TTrieMap< Meson::Common::Text::String, TValue >& p_map )
	{
		if( &p_map == this ) 
			return *this;

		Clear();
		InsertMap( p_map );
		return *this;
	}


	inline void Insert( const Meson::Common::Text::String& p_strKey, const TValue& p_value )
	{
		TriePair* pPair = PutPair( (const byte*)p_strKey.GetBuffer(), p_strKey.Size() );

		pPair->Key = p_strKey;
		pPair->Value = p_value;

		if( pPair->IsInactive() )
		{
			pPair->SetPrev( m_pLastPair );
			m_pLastPair->SetNext( pPair );
			m_pLastPair = pPair;
		}
		m_unSize++;
	}

	inline void RemoveKey( const Meson::Common::Text::String& p_strKey )
	{
		TriePair* pPair = GetPair( (const byte*)p_strKey.GetBuffer(), p_strKey.Size() );

		if( pPair && pPair->IsActive() )
		{
			pPair->Remove();
			m_unSize--;
		}
	}

public: // TImmutableMap
	inline bool ContainsKey( const Meson::Common::Text::String& p_strKey ) const
	{
		TriePair* pPair = ((TStringMap*)this)->GetPair( (const byte*)p_strKey.GetBuffer(), p_strKey.Size() );

		return ( pPair && pPair->IsActive() );
	}

	inline bool ContainsKey( const Meson::Common::Text::String& p_strKey, TValue& p_value ) const
	{
		TriePair* pPair = ((TStringMap*)this)->GetPair( (const byte*)p_strKey.GetBuffer(), p_strKey.Size() );
		
		if( pPair && pPair->IsActive() )
		{
			p_value = pPair->Value;
			return true;
		}
		
		return false;
	}

	inline const TValue& ValueAt( const Meson::Common::Text::String& p_strKey ) const
	{
		TriePair* pPair = ((TStringMap*)this)->GetPair( (const byte*)p_strKey.GetBuffer(), p_strKey.Size() );

		if( pPair && pPair->IsActive() )
			return pPair->Value;
		else
			throw new MesonException("Key not found in TrieMap.", __FILE__, __LINE__);
	}
};

Meson_Common_Collections_END
