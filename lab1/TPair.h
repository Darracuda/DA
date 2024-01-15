#pragma once

template <typename TKey, typename TValue>
class TPair
{
private:
	TKey _key;
	TValue _value;

public:
	TPair() = default;
	TPair(const TPair& other);
	TPair(const TKey& key, const TValue& value);
	virtual ~TPair();

	TKey GetKey();
	TValue GetValue();
};

