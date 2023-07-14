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
	static int serialize(std::ofstream& os, const T& value);
	static int serialize(std::ofstream& os, only_if_string<T>& value);
	static int serialize(std::ofstream& os, bool& value);
};

template<typename T, typename U = void>
class StreamDeserializer
{
public:
	static int deserialize(std::ifstream& is, const T& value);
	static int deserialize(std::ifstream& is, only_if_string<T>& value);
	static int deserialize(std::ifstream& is, bool& value);
};