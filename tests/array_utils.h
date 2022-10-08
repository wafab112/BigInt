#pragma once
#include <cstdint>
#include "types.h"

namespace utils {

	u64 most_significant_bit(const u8* arr, u64 length) {
		const u8* end = arr + length - 1;

		while (*end == 0 && end > arr) {
			end--;
		}

		// index within the array of bytes
		u64 byteIndex = end - arr;
		u64 bitIndex = byteIndex * 8;

		for (int i = 7; i >= 0; i--) {
			if (((*end >> i) & 1) == 1) return bitIndex + i;
		}

		return bitIndex;
	}

	bool get_bit(const u8* arr, u64 n) {
		u64 byteIndex = n / 8;
		u64 bitIndex = n % 8;

		u64 bit = (arr[byteIndex] >> bitIndex) & 1;

		if (bit == 1) return true;
		return false;
	}

}
