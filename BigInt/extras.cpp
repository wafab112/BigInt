#include "UBigInt.h"
#include <string>
#include <iostream>

bool isLittleEndian() {
	volatile u32 i = 0x01234567;
	return (*((u8*)(&i))) == 0x67;
}

void UBigInt::reallocate_bytes(u64 newLength) {
	delete[] bytes;
	bytes = new u8[newLength]{ 0 };
	length = newLength;
}

u64 UBigInt::most_significant_bit() const {
	if (length == 0) {
		return 0;
	}
	u8* end = bytes + length - 1;

	while (*end == 0 && end > bytes) {
		end--;
	}

	// index within the array of bytes
	u64 byteIndex = end - bytes;
	u64 bitIndex = byteIndex * 8;

	for (int i = 7; i >= 0; i--) {
		if (((*end >> i) & 1) == 1) return bitIndex + i;
	}

	return bitIndex;
}

bool UBigInt::get_bit(u64 n) const {
	u64 byteIndex = n / 8;
	u64 bitIndex = n % 8;

	u64 bit = (bytes[byteIndex] >> bitIndex) & 1;

	if (bit == 1) return true;
	return false;
}

std::string UBigInt::to_string(u8 base = 10) const {
	UBigInt puffer{ *this };
	u64 remainder = 0;

	std::vector<char> v;

	while (puffer != 0) {
		puffer.divide(remainder, base);
		remainder += '0';
		v.insert(v.begin(), (u8) remainder);
	}

	v.push_back(0);
	v.shrink_to_fit();

	std::string str;

	str.append(v.data(), v.size());

	return str;
}

void UBigInt::shrink_to_fit() {
	int firstEmpty = -1;

	for (int i = 0; i < length; i++) {
		if (!bytes[i]) {
			if (firstEmpty == -1) firstEmpty = i;
		}
		else {
			firstEmpty = -1;
		}
	}
	
	if (firstEmpty == -1) return;

	if (firstEmpty == 0) {
		delete[] bytes;
		bytes = new u8[1]{0};
		length = 1;
		return;
	}

	u8* newArr = new u8[firstEmpty]{};
	
	std::copy(bytes, bytes + firstEmpty, newArr);

	delete[] bytes;
	bytes = newArr;
	length = firstEmpty;
}

UBigInt& UBigInt::deep_copy(const UBigInt& rhs) {
	length = rhs.length;
	delete[] bytes;
	bytes = new u8[length];
	std::copy(rhs.bytes, rhs.bytes + length, bytes);
	return *this;
}

/// <summary>
/// Converts the eight least signigicant bits of this UBigInt into a u64.
/// Perfomance:
///		- 0 malloc
///		- 1 array copy
///		- O(n), where n is the length of this, up to <i>sizeof(out)</i>
/// </summary>
/// <param name="out">Has to be an initialized value.</param>
void UBigInt::to_u64(u64& out) const {
	u8* arr = (u8*) &out;

	if (length < sizeof(out)) {
		std::copy(bytes, bytes + length, arr);
		return;
	}

	std::copy(bytes, bytes + sizeof(out), arr);
}

/// <summary>
///	Creates a UBigInt from a unsigned number. 
/// Performance:
///		- 1 malloc
///		- 1 array copy
///		- O(n), where n is number of used bits in rhs
/// </summary>
/// <param name="rhs">The number, which should be created as a UBigInt.</param>
/// <returns></returns>
UBigInt& UBigInt::from_u64(const u64& rhs) {
	u64 l = sizeof(rhs);

	const u8* numberArr = static_cast<const u8*>(static_cast<const void*>(&rhs));
	const u8* end = numberArr + l - 1;

	while (*end == 0 && end > numberArr) {
		end--;
	}

	u64 diff = end - numberArr;

	length = diff + 1;
	bytes = new u8[length]{0};

	std::copy(numberArr, numberArr + length, bytes);

	return *this;
}

u8& UBigInt::operator[](u64 index) {
	if (index >= length) throw std::invalid_argument("Index too big");

	return bytes[index];
}

UBigInt& UBigInt::operator=(const UBigInt& rhs) {
	deep_copy(rhs);
	return *this;
}

UBigInt& UBigInt::operator=(const u64& rhs) {
	from_u64(rhs);
	return *this;
}

std::ostream& operator<<(std::ostream& os, UBigInt& number) {
	os << number.to_string();
	return os;
}
