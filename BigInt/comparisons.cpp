#include "UBigInt.h"

namespace wafab {

/// <summary>
/// Performance (Let n:=max{length, rhs.lenght}):
///		- Best-case:	Numbers differ in least significant bytes => O(1)
///		- Worst-case:	Numbers are equal => O(n) 
/// </summary>
/// <param name="rhs">The UBigInt to compare to.</param>
/// <returns>Whether *this and rhs represent the same number.</returns>
bool UBigInt::equals(const UBigInt& rhs) const {
	bool hasSmallerLength = length < rhs.length;

	u64 smallestLength = hasSmallerLength ? length : rhs.length;

	for (int i = 0; i < smallestLength; i++) {
		if (bytes[i] != rhs.bytes[i]) return false;
	}

	if (hasSmallerLength) {
		for (int i = smallestLength; i < length; i++) {
			if (bytes[i] != 0) return false;
		}
	}

	if (!hasSmallerLength) {
		for (int i = smallestLength; i < rhs.length; i++) {
			if (rhs.bytes[i] != 0) return false;
		}
	}

	return true;
}

bool UBigInt::not_equals(const UBigInt& rhs) const {
	return !equals(rhs);
}

bool UBigInt::smaller_than(const UBigInt& rhs) const {
	u64 minLength = length <= rhs.length ? length : rhs.length;

	u64 msbLhs = most_significant_bit() / 8;


	// problem wenn length = 0
	u64 msbRhs = rhs.most_significant_bit() / 8;

	if (msbLhs < msbRhs) return true;
	if (msbLhs > msbRhs) return false;

	u8* endLhs = bytes + minLength - 1;
	u8* endRhs = rhs.bytes + minLength - 1;

	for (int i = 0; i < minLength; i++) {
		u8 currentLhs = *(endLhs - i);
		u8 currentRhs = *(endRhs - i);
		if (currentLhs < currentRhs) return true;
		if (currentLhs > currentRhs) return false;
	}

	return false;
}

bool UBigInt::bigger_than(const UBigInt& rhs) const {
	if (smaller_than(rhs)) return false;
	if (equals(rhs)) return false;
	return true;
}

bool UBigInt::smaller_or_equal(const UBigInt& rhs) const {
	return !bigger_than(rhs);
}

bool UBigInt::bigger_or_equal(const UBigInt& rhs) const {
	return !smaller_than(rhs);
}

bool UBigInt::equals(const u64& rhs) const {
	if (length > sizeof(rhs)) {
		for (int i = sizeof(rhs); i < length; i++) {
			if (bytes[i] != 0) return false;
		}
	}

	u64 this64{0};
	to_u64(this64);

	return rhs == this64;
}

bool UBigInt::not_equals(const u64& rhs) const {
	return !equals(rhs);
}

bool UBigInt::smaller_than(const u64& rhs) const {
	if (length > sizeof(rhs)) {
		for (int i = sizeof(rhs); i < length; i++) {
			if (bytes[i] != 0) return false;
		}
	}

	u64 this64{0};
	to_u64(this64);

	return this64 < rhs;
}

bool UBigInt::bigger_than(const u64& rhs) {
	if (length > sizeof(rhs)) {
		for (int i = sizeof(rhs); i < length; i++) {
			if (bytes[i] != 0) return true;
		}
	}

	u64 this64{0};
	to_u64(this64);

	return this64 > rhs;
}

bool UBigInt::smaller_or_equal(const u64& rhs) const {
	return !bigger_than(rhs);
}

bool UBigInt::bigger_or_equal(const u64& rhs) const {
	return !smaller_than(rhs);
}

bool operator==(const UBigInt& lhs, const UBigInt& rhs) {
	return lhs.equals(rhs);
}

bool operator==(const UBigInt& lhs, const u64& rhs) {
	return lhs.equals(rhs);
}

bool operator!=(const UBigInt& lhs, const UBigInt& rhs) {
	return lhs.not_equals(rhs);
}

bool operator!=(const UBigInt& lhs, const u64& rhs) {
	return lhs.not_equals(rhs);
}

bool operator<(const UBigInt& lhs, const UBigInt& rhs) {
	return lhs.smaller_than(rhs);
}

bool operator<(const UBigInt& lhs, const u64& rhs) {
	return lhs.smaller_than(rhs);
}

bool operator>(const UBigInt& lhs, const UBigInt& rhs) {
	return lhs.bigger_than(rhs);
}

bool operator>(const UBigInt& lhs, const u64& rhs) {
	return lhs.bigger_than(rhs);
}

bool operator<=(const UBigInt& lhs, const UBigInt& rhs) {
	return lhs.smaller_or_equal(rhs);
}

bool operator<=(const UBigInt& lhs, const u64& rhs) {
	return lhs.smaller_or_equal(rhs);
}

bool operator>=(const UBigInt& lhs, const UBigInt& rhs) {
	return lhs.bigger_or_equal(rhs);
}

bool operator>=(const UBigInt& lhs, const u64& rhs) {
	return lhs.bigger_or_equal(rhs);
}

}
