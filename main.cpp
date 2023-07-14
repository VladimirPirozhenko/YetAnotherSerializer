
#include "Serializer.h"


int main(int argc, char *argv[])
{
	int  TestNumber{ 442231 };
	std::ofstream ost;
	ost.open("test.txt");
	auto sizeOut = StreamSerializer<int>::serialize(ost, TestNumber);
	ost.close();
	std::ifstream ist("test.txt");
	ist.open("test.txt");
	auto sizeIn = StreamDeserializer<int>::deserialize(ist, TestNumber);
	ist.close();
 	return 0; //hihishka
}