#pragma once
template <typename T>
class TVector
{
private:
	size_t _size;
	size_t _capacity;
	T* _data;

public:
	TVector();
	TVector(const TVector& other);
	virtual ~TVector();

	void PushBack(const T& item);
	T GetItem(const size_t index);
	size_t Size();

private:
	void IncreazeCapacity();
};
