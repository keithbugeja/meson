#pragma once

#include "Collection.h"

// Disable DLL export warnings
#pragma warning (disable:4251)

Meson_Common_Collections_BEGIN

template<class TKey, class TValue>
class TKeyValuePair
{
public:
	TKey Key;
	TValue Value;

	TKeyValuePair(void) : Key() , Value() { }

	TKeyValuePair(const TKeyValuePair& p_keyValuePair) : Key(p_keyValuePair.Key) , Value(p_keyValuePair.Value) { }

	TKeyValuePair(const TKey& p_key, const TValue& p_value)
		: Key(p_key) , Value(p_value) { }

	TKeyValuePair& operator=(const TKeyValuePair& p_keyValuePair)
	{
		Key = p_keyValuePair.Key;
		Value = p_keyValuePair.Value;
		return *this;
	}

	bool operator==(const TKeyValuePair& p_keyValuePair)
	{
		return Key == p_keyValuePair.Key;
	}
};

template<class TKey, class TValue>
class TImmutableMap
	: public TCollection< TKeyValuePair<TKey, TValue> >
{
public:
	virtual ~TImmutableMap(void) { }
	virtual bool ContainsKey(const TKey& p_key) const = 0; 
	virtual bool ContainsKey(const TKey& p_key, TValue& p_value) const = 0; 
	virtual const TValue& ValueAt(const TKey& p_key) const = 0;

	virtual TEnumerator<TKey>& GetKeyEnumerator(bool p_bReset = true) = 0;
	virtual typename TEnumerator<TKey>::Ptr CreateKeyEnumerator(void) = 0;

	virtual TEnumerator<TValue>& GetValueEnumerator(bool p_bReset = true) = 0;
	virtual typename TEnumerator<TValue>::Ptr CreateValueEnumerator(void) = 0;
};

Meson_Common_Collections_END
