#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <array>
#include <stdexcept>
#include <chrono>

static std::array<std::string const, 4> const fnames = {"a", "b", "c", "d"};  

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

	if (in.bad()) throw std::runtime_error ("Something went wrong while reading the file");

	return true;
}

std::size_t filesort (std::string const & filename) {
	if (filename.empty()) throw std::invalid_argument ("The filename cannot be an empty string");

	std::ifstream in (filename);

	std::array <std::fstream, 4> f {}; // alternatively std::array<std::array<std::fstream, 2>, 2> f

	{ // Checking whether the input file is empty
		int t;
		if ((in >> t).eof()) throw std::invalid_argument ("The input file cannot be empty");
		in.seekg(0).sync();
	}
	
	{ // "Division" phase : dividing the input file "in" to two files "f[0]" and "f[1]"
		int prev, next;
		std::size_t i = 0;

		f[0].open(fnames[0], std::ios_base::out | std::ios_base::trunc);
		f[1].open(fnames[1], std::ios_base::out | std::ios_base::trunc);

		in >> prev;
		f[i] << prev << '\n';
		while (in >> next) {
			if (prev > next) i ^= 1;
			prev = next;
			f[i] << prev << '\n';
		}

		if (not (in or f[0] or f[1])) throw std::runtime_error ("Something went wrong while working with input file and/or the files a, b");
		
		in.close();
		f[0].close();
		f[1].close();
	}

	{ // Merge phase : iteratively merge ordered sequences from one pair of files to the other and around until one of the resulting files is empty
		#define to (from ^ 2)
		std::size_t from = 0;

		while (true) { // swap the file pairs
			f[from+0].open (fnames[from+0], std::ios_base::in);
			f[from+1].open (fnames[from+1], std::ios_base::in);
			f[to  +0].open (fnames[to  +0], std::ios_base::out | std::ios_base::trunc);
			f[to  +1].open (fnames[to  +1], std::ios_base::out | std::ios_base::trunc);

			{ int t; if ((f[from+1] >> t).eof()) return from; f[from+1].seekg(0).sync(); } // check the algorithm's termination condition

			int i = 0, j = 0, ip = 0, jp = 0;

			f[from+0] >> i;
			f[from+1] >> j;

			std::size_t which = 0;
			while (true) { // step through the ordseqs of the two 'from' files
				do { // merge the current two ordrered sequences to other files
					if (i <= j) {
						f[to+which] << i << "\n";
						ip = i;
						f[from+0] >> i;
					} else {
						f[to+which] << j << "\n";
						jp = j;
						f[from+1] >> j;
					}

					//
					//f[to+which].flush();
					
					if (ip > i) {
						while (jp <= j and f[from+1]) {
							f[to+which] << j << "\n";
							jp = j;
							f[from+1] >> j;
						}
						jp = 0;
						ip = 0;
						which ^= 1;
					} else if (jp > j) { // jp >= j
							while (ip < i and f[from+0]) {
							f[to+which] << i << "\n";
							ip = i;
							f[from+0] >> i;
						}
						ip = 0;
						jp = 0;
						which ^= 1;
					}

					//
					//f[to+which].flush();

				} while (f[from+0] and f[from+1]);

				if (f[from+0].eof()) {
					while (jp <= j and f[from+1]) {
							f[to+which] << j << "\n";
							jp = j;
							f[from+1] >> j;
					}
					which ^= 1;
					while (f[from+1]) {
						f[to+which] << j << "\n";
						f[from+1] >> j;
					}
					break;
				} else {
					while (ip <= i and f[from+0]) {
							f[to+which] << i << "\n";
							ip = i;
							f[from+0] >> i;
					}
					which ^= 1;
					while (f[from+0]) {
						f[to+which] << i << "\n";
						f[from+0] >> i;
					}
					break;
				}
				//
				//f[to+which].flush();
			}

			from = to;
			for (auto & file : f) file.close(); //f[0].close(), f[1].close(), f[2].close(), f[3].close();
		}
		#undef to
	}
}


void test________ () {
	static std::string const in_filename = "in";
	static int const number_count = 100000;
	static int const max_number_value = 100000;
	static int const test_runs_count = 10; 
	
	int total = 0; bool contains;
	for (int i = 0; i < test_runs_count; i++) {
		createFileWithRandomNumbers (in_filename, number_count, max_number_value);
		std::cout << (contains = isFileContainsSortedArray(fnames[filesort(in_filename)])) << "\n";
		total += static_cast <int> (contains);
	}

	std::cout << "Total: " << total << " out of " << test_runs_count << std::endl;
}

void test () {
	auto start = std::chrono::steady_clock::now();
	test________ ();
	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> interval = end - start;
	std::cout << "The tests took " << interval.count() << " seconds" << std::endl;
}

int main () {
	test();
}
