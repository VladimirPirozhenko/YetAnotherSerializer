#pragma once
#include <fstream>
#include <iostream>
template<typename T>
using only_if_string =
std::enable_if_t<std::is_same_v<T, std::string>>;

constexpr auto t_value = static_cast<std::uint8_t>('T');
constexpr auto f_value = static_cast<std::uint8_t>('F');

template<typename T, typename U = void>
class StreamSerializer
{
public:
	static int serialize(std::ostream& os, const T& value);
	static int serialize(std::ostream& os, only_if_string<T>& value);
	static int serialize(std::ostream& os, bool& value);

	static T deserialize(std::istream& is);
	static T deserialize(std::istream& is, only_if_string<T>& value);
	static T deserialize(std::istream& is, bool& value);
};

template<typename T, typename U = void>
class StreamDeserializer
{
public:
	//static int deserialize(std::istream& is, const T& value);
	//static int deserialize(std::istream& is, only_if_string<T>& value);
	//static int deserialize(std::istream& is, bool& value);
};

template<typename T, typename U>
int StreamSerializer<T, U>::serialize(std::ostream& os, const T& value)
{
	const auto pos = os.tellp();
	os.write(reinterpret_cast<const char*>(&value), sizeof(value));
	return static_cast<std::size_t>(os.tellp() - pos);
}



template<typename T, typename U>
int StreamSerializer<T, U>::serialize(std::ostream& os, bool& value)
{
	const auto pos = os.tellp();
	const auto tmp = (value) ? t_value : f_value;
	os.write(reinterpret_cast<const char*>(&tmp), sizeof(tmp));
	return static_cast<std::size_t>(os.tellp() - pos);
}

template<typename T, typename U>
int StreamSerializer<T, U>::serialize(std::ostream& os, only_if_string<T>& value)
{
	const auto pos = os.tellp();
	const auto len = static_cast<std::uint32_t>(value.size());
	os.write(reinterpret_cast<const char*>(&len), sizeof(len));
	if (len > 0)
		os.write(value.data(), len);
	return static_cast<std::size_t>(os.tellp() - pos);
}



template<typename T, typename U>
T StreamSerializer<T, U>::deserialize(std::istream& is)
{
	T value;
	is.read(reinterpret_cast<char*>(&value), sizeof(value));
	return value;
}


template<typename T, typename U>
T StreamSerializer<T, U>::deserialize(std::istream& is, bool& value)
{
	//const auto pos = is.tellp();

	is.read(reinterpret_cast<const char*>(&tmp), sizeof(tmp));
	if (tmp == t_value)
		value = true;
	else if (tmp == f_value)
		value = false;
	return static_cast<std::size_t>(sizeof(tmp));
}


// а как со строками?
template<typename T, typename U>
T StreamSerializer<T, U>::deserialize(std::istream& is, only_if_string<T>& value)
{
	//const auto pos = os.tellp();

	std::uint32_t len;
	is.read(reinterpret_cast<const char*>(&len), sizeof(len));
	if (len > 0)
		is.write(value.data(), len);
	return static_cast<std::size_t>(sizeof(len));
}