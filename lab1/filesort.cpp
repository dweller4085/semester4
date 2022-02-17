#include <iostream>
#include <fstream>
#include <string>
#include <random>

bool createFileWithRandomNumbers (std::string const & filename, int const numbersCount, int const maxNumberValue) {
	if (filename.empty() or numbersCount < 1 or maxNumberValue < 1) return false;
	
	std::ofstream out (filename);
	
	static std::random_device rd;
	static std::mt19937 gen (rd ());
	std::uniform_int_distribution <int> distr (0, maxNumberValue);

	for (int i = 0; i < numbersCount; i++) out << distr(gen) << "\n";
	
	return out.good();
}

// taking ascending order as the correct order
bool isFileContainsSortedArray (std::string const & filename) {
	if (filename.empty()) return false;

	std::ifstream in (filename);

	int prev, next;
	if (not (in >> prev)) return true;

	while (in >> next) {
		if (prev > next) return false;
		prev = next;
	}

	return true;
}

void filesort (std::string const & filename) {
	;
}

int main (int argc, char ** argv) {
	//createFileWithRandomNumbers ("test.txt", 10, 9);
	std::cout << isFileContainsSortedArray ("test.txt") << std::endl;
}