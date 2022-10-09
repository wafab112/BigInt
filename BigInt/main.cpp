#include "UBigInt.h"
#include <iostream>
#include <string>
#include <random>
#include <chrono>

// finding the lenght of the period, of the reciprical of n
void period_length(const UBigInt& n, UBigInt& out) {
    wafab::UBigInt rest{};
    wafab::UBigInt t{ 1 };
    wafab::UBigInt counter{};

	while (rest != 1) {

		while (t < n) {
			t *= 10;
			counter++;
		}

		rest = t % n;
		rest.shrink_to_fit();
		t = rest;
	}

	out = counter;
}

int main() {

	// Just testing some random operations
	auto t1 = std::chrono::high_resolution_clock::now();
    wafab::UBigInt out{};
    wafab::UBigInt in{ "100019\0" };

	period_length(in, out);
	auto t2 = std::chrono::high_resolution_clock::now();

	auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

	std::cout << "Solution: " << out << "\n";
	std::cout << "Duration: " << ms_int.count();

	return 0;
}

