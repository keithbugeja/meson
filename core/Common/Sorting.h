#pragma once

#include "List.h"

Meson_Common_Collections_BEGIN

template<typename TElement>
int DefaultCompare(const TElement& p_element1, const TElement& p_element2)
{
	if (p_element1 < p_element2) return -1;
	if (p_element2 < p_element1) return 1;
	return 0;
};

template<class TElement>
class TSorting
{
private:
	typedef int (*ComparatorFunction)(const TElement& p_element1, const TElement& p_element2);

private: // private static methods
	inline static void Swap(TElement& element1, TElement& element2)
	{
		TElement elementTemp = element1;
		element1 = element2;
		element2 = elementTemp;
	}

	static size_t QuickSortPartition(
		TList<TElement>& p_list, size_t p_unLeft,
		size_t p_unRight, size_t p_unPivotIndex,	
		ComparatorFunction pComparatorFunction)
	{
		TElement elementPivotValue = p_list(p_unPivotIndex);
		Swap(p_list(p_unPivotIndex), p_list(p_unRight));
		size_t unNewPivotIndex = p_unLeft;
		for (size_t unIndex = p_unLeft; unIndex < p_unRight; unIndex++)
			if (pComparatorFunction(p_list(unIndex), elementPivotValue) <= 0)
			{
				Swap(p_list(unNewPivotIndex), p_list(unIndex));
				++unNewPivotIndex;
			}
		Swap(p_list(p_unRight), p_list(unNewPivotIndex));
		return unNewPivotIndex;
	}

	static void QuickSort(TList<TElement>& p_list,
		size_t p_unLeft, size_t p_unRight,
		ComparatorFunction pComparatorFunction)
	{
		if (p_unLeft >= p_unRight) return;

		size_t unPivotIndex = p_unLeft;
		size_t unNewPivotIndex = QuickSortPartition(
			p_list, p_unLeft, p_unRight, unPivotIndex,
			pComparatorFunction);

		if (unNewPivotIndex > 0)
			QuickSort(p_list, p_unLeft, unNewPivotIndex - 1,
			pComparatorFunction);

		QuickSort(p_list, unNewPivotIndex + 1, p_unRight,
			pComparatorFunction);
	}

	static void MergeSortMerge(
		TList<TElement>& p_list,
		size_t unIndex1, size_t unLength1,
		size_t unIndex2, size_t unLength2,
		ComparatorFunction pComparatorFunction)
	{
		while (unLength1 > 0 && unLength2 > 0)
		{
			if (pComparatorFunction(p_list(unIndex1), p_list(unIndex2)) <= 0)
			{
				++unIndex1;
				--unLength1;
			}
			else
			{
				TElement element2 = p_list(unIndex2);
				p_list.RemoveAt(unIndex2);
				p_list.Insert(unIndex1, element2);
				++unIndex1;
				++unIndex2;
				--unLength2;
			}
		}
	}

	static void MergeSort(TList<TElement>& p_list,
		size_t unIndex, size_t unLength,
		ComparatorFunction pComparatorFunction)
	{
		if (unLength <= 1) return;

		size_t unHalfLength2 = unLength >> 1;
		size_t unHalfLength1 = unLength - unHalfLength2;
		size_t unIndex1 = unIndex;
		size_t unIndex2 = unIndex + unHalfLength1;

		MergeSort(p_list, unIndex1, unHalfLength1, pComparatorFunction);
		MergeSort(p_list, unIndex2, unHalfLength2, pComparatorFunction);

		MergeSortMerge(p_list,
			unIndex1, unHalfLength1, unIndex2, unHalfLength2,
			pComparatorFunction);
	}

	inline static void InsertionSortInsert(
		TList<TElement>& p_list, size_t p_unLength, TElement p_element,
		ComparatorFunction pComparatorFunction)
	{
		size_t unIndex = p_unLength - 1;
		while (pComparatorFunction(p_list(unIndex), p_element) > 0)
		{
			p_list(unIndex + 1) = p_list(unIndex);
			if (unIndex == 0)
			{
				p_list(unIndex) = p_element;
				return;
			}
			unIndex--;
		}
		 
		p_list(unIndex + 1) = p_element;
	}

private: // private methods
	TSorting(void);
	~TSorting(void);

public: // public static methods
	inline static void QuickSort(TList<TElement>& p_list,
		ComparatorFunction pComparatorFunction = &DefaultCompare<TElement>)
	{
		if (p_list.Size() <= 1) return;
		QuickSort(p_list, 0, p_list.Size() - 1, pComparatorFunction);
	}

	inline static void MergeSort(TList<TElement>& p_list,
		ComparatorFunction pComparatorFunction = &DefaultCompare<TElement>)
	{
		if (p_list.Size() <= 1) return;
		MergeSort(p_list, 0, p_list.Size(), pComparatorFunction);
	}

	inline static void InsertionSort(TList<TElement>& p_list,
		ComparatorFunction pComparatorFunction = &DefaultCompare<TElement>)
	{
		for (size_t unIndex = 1; unIndex < p_list.Size(); unIndex++)
			InsertionSortInsert(p_list, unIndex, p_list(unIndex),
				pComparatorFunction);
	}

	static void ShellSort(TList<TElement>& p_list,
		ComparatorFunction pComparatorFunction = &DefaultCompare<TElement>)
	{
		size_t increment = p_list.Size() / 2;
		  
		while (increment > 0)
		{
			for (size_t unIndexOuter=increment;
				unIndexOuter < p_list.Size(); unIndexOuter++)
			{
				size_t unIndexInner = unIndexOuter;
				TElement elementTemp = p_list(unIndexOuter);
				TElement elementPrevious;
				while ((unIndexInner >= increment)
					&& (pComparatorFunction(
						elementPrevious = p_list(unIndexInner - increment),
						elementTemp) > 0))
				{
					p_list(unIndexInner) = elementPrevious;
					unIndexInner -= increment;
				}

				p_list(unIndexInner) = elementTemp;
			}

			if (increment == 2)
				increment = 1;
			else 
				increment = (int) (increment / 2.2);
		}
	}
};

Meson_Common_Collections_END
