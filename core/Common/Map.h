#pragma once

#include "ImmutableMap.h"
#include "Memory.h"
#include "InputStream.h"
#include "OutputStream.h"

Meson_Common_Collections_BEGIN

Meson::Common::Memory::MemoryManager *GetMapMemoryManager(void);

template<class TKey, class TValue>
class TMap
	: public TImmutableMap<TKey, TValue>
{
public:
	virtual void Clear(void) = 0;
	virtual TValue& operator[](const TKey& p_key) = 0;
	virtual TValue& operator()(const TKey& p_key) = 0;
	virtual void Insert(const TKey& p_key, const TValue& p_value) = 0;
	virtual void InsertMap(const TImmutableMap<TKey, TValue>& p_map) = 0;
	virtual void RemoveKey(const TKey& p_key) = 0;
	virtual void RemoveValue(const TValue& p_value) = 0;
	virtual TImmutableMap<TKey, TValue>& AsImmutableMap(void) const = 0;
};

Meson_Common_Collections_END

template<class TKey, class TValue>
bool operator==(
	Meson::Common::Collections::TImmutableMap<TKey, TValue>& p_map1,
	Meson::Common::Collections::TImmutableMap<TKey, TValue>& p_map2)
{
	if (p_map1.Size() != p_map2.Size()) return false;

	if (p_map1.Size() == 0) return true;

	Meson::Common::Collections::TEnumerator<TKey>& enumerator1
		= p_map1.GetKeyEnumerator();
	Meson::Common::Collections::TEnumerator<TKey>& enumerator2
		= p_map2.GetKeyEnumerator();
	for (; enumerator1.HasMoreElements();)
	{
		TKey& key = enumerator1.NextElement();
		if (!p_map2.ContainsKey(key)) return false;

		if (p_map1(key) != p_map2(key))
			return false;
	}
	return true;
};

template<class TKey, class TValue>
Meson::Common::Collections::TMap<TKey, TValue>& operator+=(
	Meson::Common::Collections::TMap<TKey, TValue>& p_map,
	const Meson::Common::Collections::TImmutableMap<TKey, TValue>& p_mapToAdd)
{
	p_map.InsertMap(p_mapToAdd);
	return p_map;
};

template<class TKey, class TValue>
Meson::Common::Collections::TMap<TKey, TValue>& operator-=(
	Meson::Common::Collections::TMap<TKey, TValue>& p_map,
	const TKey& p_key)
{
	p_map.RemoveLey(p_key);
	return p_map;
};

template<class TKey, class TValue>
Meson::Common::IO::IOutputStream& operator<<(
	Meson::Common::IO::IOutputStream& p_outputStream,
	const Meson::Common::Collections::TImmutableMap<TKey, TValue>& p_map)
{
	p_outputStream << (uint) p_map.Size();

	Meson::Common::Collections::TEnumerator<TKey> &enumerator
		= ((Meson::Common::Collections::TMap<TKey, TValue> &)
			p_map).GetKeyEnumerator();

	for (; enumerator.HasMoreElements();)
	{
		TKey &key = enumerator.NextElement();
		p_outputStream << key;
		p_outputStream << p_map(key);
	}
	
	return p_outputStream;
}

template<class TKey, class TValue>
Meson::Common::IO::IInputStream& operator>>(
	Meson::Common::IO::IInputStream& p_inputStream,
	Meson::Common::Collections::TImmutableMap<TKey, TValue>& p_map)
{
	uint unSize;
	p_inputStream >> unSize;

	TKey key;
	TValue value;
	while (unSize-- > 0)
	{
		p_inputStream >> key;
		p_inputStream >> value;
		p_map(key) = value;
	}

	return p_inputStream;
}

Meson_Common_Collections_BEGIN

template<typename TKey, typename TPointer>
void FreePointers(Meson::Common::Collections::TMap<TKey, TPointer>& p_mapPointers)
{
	TEnumerator<TPointer> &enumPointers
		= p_mapPointers.GetValueEnumerator();
	for (TPointer pointer = NULL; enumPointers.HasMoreElements();
		pointer = enumPointers.NextElement())
		delete pointer;
};

Meson_Common_Collections_END