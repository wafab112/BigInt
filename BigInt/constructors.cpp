#include "UBigInt.h"

UBigInt::UBigInt() {
	length = 0;
	bytes = nullptr;
}

UBigInt::UBigInt(const u64& number) {
	from_u64(number);
}

UBigInt::UBigInt(const char* str) {

	const char* end = str;
	while (*end != 0) {
		end++;
	}

	end--;

	length = 1;
	bytes = new u8[length]{0};

	if (*str == '0') {
		switch (*(str + 1)) {
		case 'x': {
			// hexadecimal case
			u64 counter = 0;
			while (*end != 'x') {
				UBigInt pow{ 16 };
				pow.power(counter);

				u8 toAdd;

				if (*end <= '9' && *end >= '0') {
					toAdd = *end - '0';
				} else if (*end <= 'F' && *end >= 'A') {
					toAdd = *end - 'A' + 10;
				} else if (*end <= 'f' && *end >= 'a') {
					toAdd = *end - 'a' + 10;
				}
				else {
					throw std::invalid_argument("The given input is not formatted in hexadecimal");
				}

				add(pow * toAdd);

				counter++;
				end--;
			}
		};
		break;
		case 'X': {
			// hexadecimal case
			u64 counter = 0;
			while (*end != 'X') {
				UBigInt pow{ 16 };
				pow.power(counter);

				u8 toAdd;

				if (*end <= '9' && *end >= '0') {
					toAdd = *end - '0';
				} else if (*end <= 'F' && *end >= 'A') {
					toAdd = *end - 'A' + 10;
				} else if (*end <= 'f' && *end >= 'a') {
					toAdd = *end - 'a' + 10;
				}
				else {
					throw std::invalid_argument("The given input is not formatted in hexadecimal");
				}

				add(pow * toAdd);

				counter++;
				end--;
			}
		};
		break;
		case 'b': {
			// binary case
			u64 counter = 0;
			while (*end != 'b') {
				UBigInt pow{ 2 };
				pow.power(counter);

				u8 toAdd;

				if (*end == '0') {
					toAdd = 0;
				} else if (*end == '1') {
					toAdd = 1;
				} else {
					throw std::invalid_argument("The given input is not formatted in binary");
				}

				add(pow * toAdd);

				counter++;
				end--;
			}
		};
		break;
		default: {
			// octal case
			u64 counter = 0;
			while (end > str) {
				UBigInt pow{ 8 };
				pow.power(counter);

				u8 toAdd;

				if (*end <= '7' && *end >= '0') {
					toAdd = *end - '0';
				} else {
					throw std::invalid_argument("The given input is not formatted in octal");
				}

				add(pow * toAdd);

				counter++;
				end--;
			}
		}
		}

		return;
	}

	u64 counter = 0;
	while (end >= str) {
		UBigInt pow{ 10 };
		pow.power(counter);

		u8 toAdd;

		if (*end <= '9' && *end >= '0') {
			toAdd = *end - '0';
		} else {
			throw std::invalid_argument("The given input is not formatted in decimal");
		}

		add(pow * toAdd);

		counter++;
		end--;
	}
}

UBigInt::UBigInt(const UBigInt& other) {
	bytes = nullptr;
	deep_copy(other);
}

UBigInt::UBigInt(u8* arr, u64 l) {
	bytes = new u8[l]{0};
	length = l;
	std::copy(arr, arr + l, bytes);
}

// TODO: move-constructor

UBigInt::~UBigInt() {
	delete[] this->bytes;
}
