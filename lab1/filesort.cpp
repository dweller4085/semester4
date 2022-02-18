#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <array>
#include <stdexcept>

bool createFileWithRandomNumbers (std::string const & filename, int const numbersCount, int const maxNumberValue) {
	if (filename.empty() or numbersCount < 1 or maxNumberValue < 1) return false;
	
	std::ofstream out (filename, std::ios_base::trunc);
	
	static std::random_device rd;
	static std::mt19937 gen (rd ());
	std::uniform_int_distribution <int> distr (0, maxNumberValue);

	for (int i = 0; i < numbersCount; i++) out << distr(gen) << "\n";
	
	return bool(out);
}

// Assuming ascending order as the correct order
bool isFileContainsSortedArray (std::string const & filename) {
	if (filename.empty()) throw std::invalid_argument ("The filename cannot be an empty string");

	std::ifstream in (filename);

	int prev, next;
	if (not (in >> prev)) return true;

	while (in >> next) {
		if (prev > next) return false;
		prev = next;
	}

	if (in.fail()) throw std::runtime_error ("Something went wrong while reading the file");

	return true;
}

void filesort (std::string const & filename) {
	if (filename.empty()) throw std::invalid_argument ("The filename cannot be an empty string");

	std::ifstream in (filename);

	std::array <std::fstream, 4> f = {
		std::fstream("a.txt", std::ios_base::in | std::ios_base::out | std::ios_base::trunc),
		std::fstream("b.txt", std::ios_base::in | std::ios_base::out | std::ios_base::trunc),
		std::fstream("c.txt", std::ios_base::in | std::ios_base::out | std::ios_base::trunc),
		std::fstream("d.txt", std::ios_base::in | std::ios_base::out | std::ios_base::trunc)
	};

	{ // Checking whether the input file is empty
		int t;
		if ((in >> t).eof()) throw std::invalid_argument ("The input file cannot be empty");
		in.seekg(0).sync();
	}
	
	{ // "Division" phase : dividing the input file "in" to two files "f[0]" and "f[1]"
		int prev, next;
		std::size_t i = 0;
		in >> prev;
		f[i] << prev << '\n';
		while (in >> next) {
			if (prev > next) i ^= 1;
			prev = next;
			f[i] << prev << '\n';
		}
	}

	if (not (in or f[0] or f[1])) throw std::runtime_error ("Something went wrong while working with input file or a.txt or b.txt");


}

int main (int argc, char ** argv) {
	std::string const filename = "in.txt";
	//createFileWithRandomNumbers (filename, 10, 9);
	filesort(filename);
	//std::cout << isFileContainsSortedArray (filename) << std::endl;

}
