#include "UBigInt.h"
#include <iostream>
#include <string>
#include <random>
#include <chrono>

void period_length(const UBigInt& n, UBigInt& out) {
	UBigInt rest{};
	UBigInt t{ 1 };
	UBigInt counter{};

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
	auto t1 = std::chrono::high_resolution_clock::now();
	UBigInt out{};
	UBigInt in{ "100019\0" };

	period_length(in, out);
	auto t2 = std::chrono::high_resolution_clock::now();

	auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

	std::cout << "Solution: " << out << "\n";
	std::cout << "Duration: " << ms_int.count();

	//UBigInt test{ 65535 };
	//test.lsh(0);

	/*
	uint8_t arrX[]{ 219, 6, 0 };
	UBigInt x{arrX, 3};
	uint8_t arrY[]{ 255, 255, 0, 0, 0, 0, 0, 0, 0 };
	UBigInt y{arrY, 9};

	x.smaller_than(y);
	*/
	return 0;
}

