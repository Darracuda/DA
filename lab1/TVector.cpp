#include <string>
#include "TVector.h"
#include "TPair.h"
#include "CDate.h"

template<typename T>
TVector<T>::TVector()
{
	_size = 0;
	 _capacity = 0;
	_data = nullptr;
}

template<typename T>
TVector<T>::TVector(const TVector& other)
{

	_size = other._size;
	_capacity = other._size;;

	// allocate new storage
	_data = new T[_size];

	// copy all items to new storage
	for (size_t i = 0; i < _size; i++)
		_data[i] = other._data[i];
}

template<typename T>
TVector<T>::~TVector()
{
	if (_data != nullptr)
		delete[] _data;
}

template<typename T>
inline size_t TVector<T>::Size()
{
	return _size;
}

template<typename T>
T TVector<T>::GetItem(const size_t index)
{
	return _data[index];
}

template<typename T>
void TVector<T>::PushBack(const T& item)
{
	// increase capacity if needed
	if (_size >= _capacity)
		IncreazeCapacity();
	
	// store new item
	_data[_size] = item;
	_size++;
}

template<typename T>
void TVector<T>::IncreazeCapacity()
{
	// double capacity, at least 1 item
	size_t newCapacity = (_capacity == 0) ? (1) : (2 * _capacity);

	// allocate new storage
	T* newData = new T[newCapacity];

	// copy all items to new storage
	for (size_t i = 0; i < _size; i++)
		newData[i] = _data[i];

	// de-allocate old storage
	if (_data != nullptr)
		delete[] _data;

	// accept new storage
	_data = newData;
	_capacity = newCapacity;
}

template class TVector<int>;
template class TVector<TPair<CDate, string>>;
