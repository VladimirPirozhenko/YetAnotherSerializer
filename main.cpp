
#include "Serializer.h"
#include <iostream>
#include <ios>
#include <sstream>
#include <string>
#include <type_traits>
int main(int argc, char *argv[])
{
	long long  testNumber{ 3425624773476 };
	short testShort{ 255 };
	char  testChar{ 'x' };
	std::string  testStr{ "hehe" };
	std::stringstream strStream;
	//ost.open("test.txt");
	auto sizeOut = StreamSerializer<long long>::serialize(strStream, testNumber);
	StreamSerializer<short>::serialize(strStream, testShort);
	StreamSerializer<char>::serialize(strStream, testChar);
	StreamSerializer<std::string>::serialize(strStream, testStr);
	//ost.close();

	long long testOutLong = 0;
	short testOutShort = 0;
	char testOutChar = ' ' ;
	std::string testOutStr;
	//std::stringstream ost;
	//std::ifstream ist("test.txt");
	size_t sizeIn = StreamSerializer<long long>::deserialize(strStream, testOutLong);
	sizeIn = StreamSerializer<short>::deserialize(strStream, testOutShort);
	sizeIn = StreamSerializer<char>::deserialize(strStream, testOutChar);
	sizeIn = StreamSerializer<std::string>::deserialize(strStream, testOutStr);
	std::cout << "Size:" << sizeIn << " Values: " << testOutLong << '\t' << testOutShort << '\t' << testOutChar << '\t' << testOutStr << '\n';
	int in;
	std::cin >> in;
//	ist.close();
 	return 0; //hihishka
}