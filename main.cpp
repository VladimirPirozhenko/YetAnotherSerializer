
#include "Serializer.h"


int main(int argc, char *argv[])
{
	int  TestNumber{ 4231 };
	std::ofstream ost;
	ost.open("test.txt");
	auto sizeOut = StreamSerializer<int>::serialize(ost, TestNumber);
	ost.close();

	int testOut = 0;
	std::ifstream ist("test.txt");
	testOut = StreamSerializer<int>::deserialize(ist);
	std::cout << testOut << '\n';
	int in;
	std::cin >> in;
	ist.close();
 	return 0; //hihishka
}