#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <type_traits>
#include "Profiler.h"


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

// Ѕј«ќ¬јя –≈јЋ»«ј÷»я
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

// —ѕ≈÷»јЋ»«ј÷»я ƒЋя Ѕ”Ћќ¬
 template<>
class StreamSerializer<bool>
{
public:
	static size_t serialize(std::ostream& os, const bool& balue)
	{
		const auto pos = os.tellp();
		const auto tmp = (balue) ? t_value : f_value;
		os.write(reinterpret_cast<const char*>(&tmp), sizeof(tmp));
		return static_cast<std::size_t>(os.tellp() - pos);
	};

	static size_t deserialize(std::istream& is, bool& balue)
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
		default: // ошибку лов€т тесты
			break;
		}
		return static_cast<std::size_t>(tmp); // тут возвращаем не размер в потоке, а саму переменную tmp, чтобы тесты могли проверить еЄ н f_value и t_value
	};
};

// —ѕ≈÷»јЋ»«ј÷»я ƒЋя —“–ќ 
template<typename T>
class StreamSerializer<T, typename std::enable_if<std::is_same<T, std::string>::value, T>::type>
{
public:

	static size_t serialize(std::ostream& os, const T& value)
	{
		const auto pos = os.tellp();
		const auto len = static_cast<std::uint32_t>(value.size());
		os.write(reinterpret_cast<const char*>(&len), sizeof(len));
		if (len > 0)
			os.write(value.data(), len);
		return static_cast<std::size_t>(os.tellp() - pos);
	};

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

//—ѕ≈÷»јЋ»«ј÷»я ƒЋя ¬≈ “ќ–ќ¬
template<typename T>
class StreamSerializer<std::vector<T>>
{
public:
	static size_t serialize(std::ostream& os, const std::vector<T>& value)
	{
		const auto pos = os.tellp();
		const auto len = value.size();
		os.write(reinterpret_cast<char const*>(&len), sizeof(len));
		os.write(reinterpret_cast<char const*>(value.data()), len * sizeof(T));
// 		for (auto& elem : value)
// 		{
// 			auto size = sizeof(elem);
// 			os.write(reinterpret_cast<char const*>(&elem), size);
// 		}
		return static_cast<std::size_t>(os.tellp() - pos);
	}

	static	size_t deserialize(std::istream& is, std::vector<T>& value)
	{
		const auto pos = is.tellg();
		size_t len;
		//auto size = sizeof(T);
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		value.resize(len);
		size_t vecSize = len * sizeof(T);
		is.read(reinterpret_cast<char*>(value.data()), vecSize);
// 		for (auto& elem : value)
// 		{
// 			auto size = sizeof(elem);
// 			is.read(reinterpret_cast<char*>(&elem), size);
// 		}
		return static_cast<std::size_t>(is.tellg() - pos);
	}
};


//static	size_t deserialize(std::istream& is, std::vector<int>& value)
//{
//	const auto pos = is.tellg();
//	size_t len;
//	is.read(reinterpret_cast<char*>(&len), sizeof(len));
//	value.resize(len);
//	size_t vecSize = len * sizeof(int);
//	is.read(reinterpret_cast<char*>(value.data()), vecSize);
//
//	return static_cast<std::size_t>(is.tellg() - pos);
//}

