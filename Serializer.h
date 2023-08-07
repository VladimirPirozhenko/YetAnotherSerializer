#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
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

// ÁÀÇÎÂÀß ĞÅÀËÈÇÀÖÈß
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

// ÑÏÅÖÈÀËÈÇÀÖÈß ÄËß ÁÓËÎÂ
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
		default: // îøèáêó ëîâÿò òåñòû
			break;
		}
		return static_cast<std::size_t>(tmp); // òóò âîçâğàùàåì íå ğàçìåğ â ïîòîêå, à ñàìó ïåğåìåííóş tmp, ÷òîáû òåñòû ìîãëè ïğîâåğèòü å¸ í f_value è t_value
	};
};

// ÑÏÅÖÈÀËÈÇÀÖÈß ÄËß ÑÒĞÎÊ
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

//ÑÏÅÖÈÀËÈÇÀÖÈß ÄËß ÂÅÊÒÎĞÎÂ
template<typename T>
class StreamSerializer<std::vector<T>>
{
public:
	static size_t serialize(std::ostream& os, const std::vector<T>& value)
	{
		const auto pos = os.tellp();
		const auto len = static_cast<std::uint32_t>(value.size());
		os.write(reinterpret_cast<const char*>(&len), sizeof(len));
		os.write(reinterpret_cast<const char*>(&value), sizeof(value));
		return static_cast<std::size_t>(os.tellp() - pos);
	}

	static	size_t deserialize(std::istream& is, std::vector<T>& value)
	{
		const auto pos = is.tellg();
		std::uint32_t len;
		auto size = sizeof(T);
		is.read(reinterpret_cast<char*>(&len), sizeof(len));
		value.resize(len);
		is.read(reinterpret_cast<char*>(value.data()), len * size);
		return static_cast<std::size_t>(is.tellg() - pos);
	}
};


