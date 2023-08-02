#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>

//template<typename T>
//using only_if_string = std::enable_if<std::is_same<T, std::string>::value,T>::type;

constexpr auto t_value = static_cast<std::uint8_t>('T');
constexpr auto f_value = static_cast<std::uint8_t>('F');

template<typename T, typename U = void>
class StreamSerializer
{
public:
	static size_t serialize(std::ostream& os, const T& value);
	static size_t deserialize(std::istream& is, T& value);

};

template<typename T>
class StreamSerializer<T, typename std::enable_if<std::is_same<T, std::string>::value, T>::type>
{
public:
	template<typename T>
	static size_t serialize(std::ostream& os, const T& value)
	{
		const auto pos = os.tellp();
		const auto len = static_cast<std::uint32_t>(value.size());
		os.write(reinterpret_cast<const char*>(&len), sizeof(len));
		if (len > 0)
			os.write(value.data(), len);
		return static_cast<std::size_t>(os.tellp() - pos);
	};

	template<typename T>
	static size_t deserialize(std::istream& is, T& value)
	{
		std::ostringstream os;
		std::uint32_t len;
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		if (len > 0)
		{
			os << is.rdbuf();
			value = os.str();
		}
			//is.read(reinterpret_cast<char*>(const_cast<char*>(value.data())), len);
		return static_cast<std::size_t>(len);
	};
};


template<typename T, typename U = void>
size_t StreamSerializer<T,U>::serialize(std::ostream& os, const T& value)
{
	const auto pos = os.tellp();
	os.write(reinterpret_cast<const char*>(&value), sizeof(value));
	return static_cast<std::size_t>(os.tellp() - pos);
}

template<typename T, typename U = void>
size_t StreamSerializer<T, U>::deserialize(std::istream& is, T& value)
{
	const auto pos = is.tellg();
	is.read(reinterpret_cast<char*>(&value), sizeof(value));
	return static_cast<std::size_t>(is.tellg() - pos);
}




template<>
size_t StreamSerializer<bool>::serialize(std::ostream& os,const bool& balue)
{
	const auto pos = os.tellp();
	const auto tmp = (balue) ? t_value : f_value;
	os.write(reinterpret_cast<const char*>(&tmp), sizeof(tmp));
	return static_cast<std::size_t>(os.tellp() - pos);
}

template<>
size_t StreamSerializer<bool>::deserialize(std::istream& is, bool& balue)
{
	char tmp;
	is.read(&tmp, sizeof(tmp));
	switch (tmp)
	{
	case t_value:
		balue = true;
		break;
	case f_value:
		balue = false;
		break;
	default: // ошибку ловят тесты
		break;
	}
	return static_cast<std::size_t>(tmp); // тут возвращаем не размер в потоке, а саму переменную tmp, чтобы тесты могли проверить её н f_value и t_value
}





