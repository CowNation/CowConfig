#include "../Source/cpp-CowConfig.h"
#include <string>

int main() {
	CowConfig cfg("Examples/test.txt");
	cfg.Section("Ints");
	cfg.WriteLine("three: ", "3");
	cfg.Section("Strings");
	cfg.WriteLine("ell: ", "L");
	cfg.WriteLine("tee: ", "T");
	std::cout << cfg.Read<std::string>("Strings", "ell: ") << std::endl;
	std::cout << cfg.Read<int>("Ints", "three: ") << std::endl;
	std::cout << cfg.Read<int>("Ints", "three: ") << std::endl;
	std::cout << cfg.Read<std::string>("Strings", "tee: ") << std::endl;
}