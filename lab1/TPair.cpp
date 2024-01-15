#include <string>
#include "TPair.h"
#include "CDate.h"

using namespace std;

template<typename TKey, typename TValue>
TPair<TKey, TValue>::TPair(const TPair& other)
{
	_key = other._key;
	_value = other._value;
}

template<typename TKey, typename TValue>
TPair<TKey, TValue>::TPair(const TKey& key, const TValue& value)
{
	_key = key;
	_value = value;
}

template<typename TKey, typename TValue>
TPair<TKey, TValue>::~TPair()
{
}

template<typename TKey, typename TValue>
TKey TPair<TKey, TValue>::GetKey()
{
	return _key;
}

template<typename TKey, typename TValue>
TValue TPair<TKey, TValue>::GetValue()
{
	return _value;
}

template class TPair<int, string>;
template class TPair<CDate, string>;
