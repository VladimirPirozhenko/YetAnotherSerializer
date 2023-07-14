
#include "Serializer.h"


template<typename T, typename U>
int StreamSerializer<T, U>::serialize(std::ofstream& os, const T& value)
{
	const auto pos = os.tellp();
	os.write(reinterpret_cast<const char*>(&value), sizeof(value));
	return static_cast<std::size_t>(os.tellp() - pos);
}



template<typename T, typename U>
int StreamSerializer<T, U>::serialize(std::ofstream& os, bool& value)
{
	const auto pos = os.tellp();
	const auto tmp = (value) ? t_value : f_value;
	os.write(reinterpret_cast<const char*>(&tmp), sizeof(tmp));
	return static_cast<std::size_t>(os.tellp() - pos);
}

template<typename T, typename U>
int StreamSerializer<T, U>::serialize(std::ofstream& os, only_if_string<T>& value)
{
	const auto pos = os.tellp();
	const auto len = static_cast<std::uint32_t>(value.size());
	os.write(reinterpret_cast<const char*>(&len), sizeof(len));
	if (len > 0)
		os.write(value.data(), len);
	return static_cast<std::size_t>(os.tellp() - pos);
}



template<typename T, typename U>
int StreamDeserializer<T, U>::deserialize(std::ifstream& is, const T& value)
{
	const auto pos = is.tellp();
	is.read(reinterpret_cast<const char*>(&value), sizeof(value));
	return static_cast<std::size_t>(is.tellp() - pos);
}


template<typename T, typename U>
int StreamDeserializer<T, U>::deserialize(std::ifstream& is, bool& value)
{
	const auto pos = is.tellp();
	is.read(reinterpret_cast<const char*>(&tmp), sizeof(tmp));
	if (tmp == t_value)
		value = true;
	else if (tmp == f_value)
		value = false;
	return static_cast<std::size_t>(is.tellp() - pos);
}


// а как со строками?
template<typename T, typename U>
int StreamDeserializer<T, U>::deserialize(std::ifstream& is, only_if_string<T>& value)
{
	const auto pos = os.tellp();
	std::uint32_t len;
	is.read(reinterpret_cast<const char*>(&len), sizeof(len));
	if (len > 0)
		is.write(value.data(), len);
	return static_cast<std::size_t>(is.tellp() - pos);
}