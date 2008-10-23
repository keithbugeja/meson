#pragma once

#include "Namespaces.h"

Meson_Common_Collections_BEGIN

template<class TElement1, class TElement2>
struct TPair
{
	TElement1 Element1;
	TElement2 Element2;

	TPair(void) : Element1() , Element2() { }

	TPair(const TPair &p_pair) : Element1(p_pair.Element1) , Element2(p_pair.Element2) { }

	TPair(const TElement1& p_element1, const TElement2& p_element2)
		: Element1(p_element1) , Element2(p_element2) { }

	TPair &operator=(const TPair& p_pair)
	{
		Element1 = p_pair.Element1;
		Element2 = p_pair.Element2;
		return *this;
	}

	const bool operator==(const TPair& p_pair) const
	{
		return (Element1 == p_pair.Element1)
			&& (Element2 == p_pair.Element2);
	}

	const bool operator<(const TPair& p_pair) const
	{
		if (Element1 < p_pair.Element1) return true;

		if (Element1 == p_pair.Element1)
			return Element2 < p_pair.Element2;

		return false;
	}
};

Meson_Common_Collections_END
