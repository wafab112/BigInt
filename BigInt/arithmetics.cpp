#include "array_utils.h"
#include "UBigInt.h"

// goal: not having any 0-most_significant_bytes (non 0-leading numbers)

// provided goal is fullfilled, add fullfills the goal
UBigInt& UBigInt::add(const UBigInt& rhs) {
	if (rhs.length > length) {
		u8* newBytes = new u8[rhs.length]{0};
		std::copy(bytes, bytes + length, newBytes);
		delete[] bytes;
		bytes = newBytes;
		length = rhs.length;
	}

	u8 remainder = 0;
	for (int i = 0; i < rhs.length; i++) {
		u8 result = bytes[i] + rhs.bytes[i] + remainder;
		if (result < bytes[i] && result < rhs.bytes[i]) {
			remainder = 1;
		}
		else {
			remainder = 0;
		}
		bytes[i] = result;
	}

	if (rhs.length == length) {
		if (remainder == 1) {
			u8* newBytes = new u8[length + 1]{ 0 };
			std::copy(bytes, bytes + length, newBytes);
			delete[] bytes;
			bytes = newBytes;
			length++;
			bytes[length - 1] = 1;
			return *this;
		}
	}

	u64 cursor = 0;
	while (remainder == 1) {
		if (rhs.length + cursor >= length) {
			u8* newArr = new u8[length + 1]{0};
			std::copy(bytes, bytes + length, newArr);
			delete[] bytes;
			bytes = newArr;
			length++;
		}
		bytes[rhs.length + cursor] += 1;
		if (bytes[rhs.length + cursor] != 0) remainder = 0;
		cursor++;
	}

	return *this;

}

UBigInt& UBigInt::multiply(const UBigInt& rhs) {
	return multiply_array(rhs.bytes, rhs.length);
}

UBigInt& UBigInt::add_array_at_index(u8* arr, u64 l, u64 index) {
	if (l + index >= length) {
		u8* newArr = new u8[index + l]{0};
		std::copy(bytes, bytes + length, newArr);

		delete[] bytes;

		bytes = newArr;
		length = index + l;
	}

	u8 remainder = 0;
	for (int i = index; i < index + l; i++) {
		u8 puffer = bytes[i];
		u8 arrPuffer = arr[i-index];
		u8 result = puffer + arrPuffer + remainder;

		bytes[i] = result;
		if ((result < puffer) || (result < arrPuffer)) {
			remainder = 1;
		}
		else {
			remainder = 0;
		}
	}

	if (this->length == index + l) {
		if (remainder == 1) {
			u8* newBytes = new u8[length + 1]{ 0 };
			std::copy(bytes, bytes + length, newBytes);
			delete[] bytes;
			bytes = newBytes;
			length++;
			bytes[length - 1] = 1;
			return *this;
		}
	}

	u64 cursor = 0;
	while (remainder == 1) {
		if (index + l + cursor >= length) {
			u8* newArr = new u8[length + 1]{0};
			std::copy(bytes, bytes + length, newArr);
			delete[] bytes;
			bytes = newArr;
			length++;
		}
		bytes[index + l + cursor] += 1;
		if (bytes[index + l + cursor] != 0) remainder = 0;
		cursor++;
	}

	return *this;
}

UBigInt& UBigInt::multiply_array(const u8* arr, u64 l) {
	bool arrIs0{ true };

	for (int i = 0; i < l; i++) {
		if (arr[i]) arrIs0 = false;
	}

	if (equals(0) || arrIs0 || l == 0) {
		length = 1;
		delete[] bytes;
		bytes = new u8[1]{ 0 };
		return *this;
	}

	UBigInt newInt{};

	u64 this_msb = most_significant_bit();
	u64 rhs_msb = utils::most_significant_bit(arr, l);

	bool thisBit{0};
	bool rhsBit{0};

	if (this_msb != 0) {
		thisBit = get_bit(this_msb - 1);
	}
	if (rhs_msb != 0) {
		rhsBit = utils::get_bit(arr, rhs_msb - 1);
	}

	u64 new_msb = this_msb + rhs_msb + 1;

	if (!(thisBit && rhsBit)) {
		new_msb--;
	}

	u64 new_length{0};

	if (new_msb % 8 != 0) {
		new_length = new_msb / 8 + 1;
	}
	else {
		new_length = new_msb / 8;
	}

	if (new_length == 0) new_length = 1;

	newInt.length = new_length;
	newInt.bytes = new u8[new_length]{0};

	for (u64 i = 0; i < length; i++) {
		for (u64 j = 0; j < l; j++) {
			u16 zwischen = bytes[i] * arr[j];

			u8* newArr = static_cast<u8*>(static_cast<void*>(&zwischen));

			if (newArr[1] == 0) {
				newInt.add_array_at_index(newArr, 1, i + j);
			}
			else {
				newInt.add_array_at_index(newArr, 2, i + j);
			}
		}
	}

	delete[] bytes;
	bytes = new u8[newInt.length]{0};
	std::copy(newInt.bytes, newInt.bytes + newInt.length, bytes);
	length = newInt.length;

	return *this;
}

UBigInt& UBigInt::power(const UBigInt& rhs) {
	if (rhs == 0) {
		operator=(1);
		return *this;
	}

	UBigInt counter{ 1 };

	UBigInt old = *this;
	while (counter < rhs) {
		multiply(old);
		counter++;
	}

	return *this;
}

UBigInt& UBigInt::subtract(const UBigInt& rhs) {

	bool is_smaller = smaller_than(rhs);
	if (is_smaller) {
		operator=(0);
		return *this;
	}

	u8 remainder = 0;

	for (u64 i = 0; i < rhs.length; i++) {
		u8 this_remainder = remainder;
		remainder = 0;

		if (this_remainder + rhs.bytes[i] < this_remainder) {
			// remainder + rhs.bytes overflow u8
			remainder++;
		}

		if (bytes[i] < rhs.bytes[i] + this_remainder) {
			remainder++;
		}

		bytes[i] -= rhs.bytes[i] + this_remainder;

	}

	if (remainder != 0) {
		bytes[rhs.length] -= remainder;
	}

	return *this;
}

UBigInt& UBigInt::divide(UBigInt& remainder, const UBigInt& rhs) {
	if (smaller_than(rhs)) {
		remainder = *this;
		operator=(0);
		return *this;
	}

	UBigInt puffer = *this;
	// NOTE: dauert vielleicht lange
	operator=(0);

	while (puffer >= rhs) {
		UBigInt counter{ rhs };
		u64 offset = 0;

		while (counter <= puffer) {
			counter <<= 1;
			offset++;
		}

		offset--;

		puffer -= rhs << offset;

		u64 byteOffset = offset / 8;
		u64 bitOffset = offset % 8;

		u8 base = 1 << bitOffset;
		add_array_at_index(&base, 1, byteOffset);
	}

	remainder = puffer;

	return *this;
}

UBigInt& UBigInt::mod(const UBigInt& rhs) {
	UBigInt r{};

	divide(r, rhs);
	operator=(r);

	return *this;
}

UBigInt& UBigInt::lsh(const u64& rhs) {	
	u64 byteOffset = rhs / 8;
	u8 bitOffset = rhs % 8;

	UBigInt newInt{};

	for (int i = length - 1; i >= 0; i--) {
		u16 puffer = *(bytes + i);
		puffer <<= bitOffset;

		u8* pufferArr = static_cast<u8*>(static_cast<void*>(&puffer));

		newInt.add_array_at_index(pufferArr, 2, i + byteOffset);
	}

	delete[] bytes;
	bytes = new u8[newInt.length];
	std::copy(newInt.bytes, newInt.bytes + newInt.length, bytes);
	length = newInt.length;
	return *this;
}

UBigInt& UBigInt::add(u64& rhs) {
	UBigInt o{rhs};
	return add(o);
}

UBigInt& UBigInt::multiply(const u64& rhs) {
	const u8* arr = static_cast<const u8*>(static_cast<const void*>(&rhs));
	int cursor = 8;
	for (int i = 7; i >= 0; i--) {
		if (arr[i] == 0) cursor = i;
		else break;
	}
	return multiply_array(arr, cursor);
}

UBigInt& UBigInt::power(const u64& rhs) {
	UBigInt real{ rhs };
	return power(real);
}

UBigInt& UBigInt::subtract(const u64& rhs) {
	UBigInt real{ rhs };
	return subtract(real);
}

UBigInt& UBigInt::divide(u64& remainder, u64 rhs) {
	UBigInt real{ rhs };
	UBigInt r{ remainder };
	divide(r, real);
	r.to_u64(remainder);
	return *this;
}

//	ii.		>>=, >> (TODO)
//	iii.	&=, & (TODO)
//	iv.		|=, | (TODO)
//	v.		~ (TODO)
//	vi.		^=, ^ (TODO)

UBigInt& UBigInt::operator+=(const UBigInt& rhs) {
	return add(rhs);
}

UBigInt& UBigInt::operator+=(const u64& rhs) {
	return add(rhs);
}

UBigInt operator+(UBigInt lhs, const UBigInt& rhs) {
	lhs += rhs;
	return lhs;
}

UBigInt operator+(UBigInt lhs, const u64& rhs) {
	lhs += rhs;
	return lhs;
}

UBigInt& UBigInt::operator*=(const UBigInt& rhs) {
	return multiply(rhs);
}

UBigInt& UBigInt::operator*=(const u64& rhs) {
	return multiply(rhs);
}

UBigInt operator*(UBigInt lhs, const UBigInt& rhs) {
	lhs *= rhs;
	return lhs;
}

 UBigInt operator*(UBigInt lhs, const u64& rhs) {
	lhs *= rhs;
	return lhs;
}

UBigInt& UBigInt::operator-=(const UBigInt& rhs) {
	return subtract(rhs);
}

UBigInt& UBigInt::operator-=(const u64& rhs) {
	return subtract(rhs);
}

UBigInt operator-(UBigInt lhs, const UBigInt& rhs) {
	lhs -= rhs;
	return lhs;
}

UBigInt operator-(UBigInt lhs, const u64& rhs) {
	lhs -= rhs;
	return lhs;
}

// TODO: will be changed to u64, as I figured out how
// TODO: also for UBigInt
UBigInt& UBigInt::operator/=(const UBigInt& rhs) {
	UBigInt r{};
	return divide(r, rhs);
}

UBigInt& UBigInt::operator/=(const u64& rhs) {
	u64 r{};
	return divide(r, rhs);
}

UBigInt operator/(UBigInt lhs, const UBigInt& rhs) {
	lhs /= rhs;
	return lhs;
}

UBigInt operator/(UBigInt lhs, const u64& rhs) {
	lhs /= rhs;
	return lhs;
}

UBigInt& UBigInt::operator%=(const UBigInt& rhs) {
	return mod(rhs);
}

UBigInt operator%(UBigInt lhs, const UBigInt& rhs) {
	lhs %= rhs;
	return lhs;
}

UBigInt& UBigInt::operator%=(const u64& rhs) {
	UBigInt real{ rhs };
	return mod(real);
}

 UBigInt operator%(UBigInt lhs, const u64& rhs) {
	if (lhs.equals(0)) {
		int l = 0;
	}
	lhs %= rhs;
	return lhs;
}

UBigInt& UBigInt::operator++() {
	add(1);
	return *this;
}

UBigInt& UBigInt::operator++(int) {
	UBigInt old = *this;
	operator++();
	return old;
}

UBigInt& UBigInt::operator--() {
	subtract(1);
	return *this;
}

UBigInt& UBigInt::operator--(int) {
	UBigInt old = *this;
	operator--();
	return old;
}

UBigInt& UBigInt::operator<<=(const u64& rhs) {
	return lsh(rhs);
}

UBigInt operator<<(UBigInt lhs, const u64& rhs) {
	lhs <<= rhs;
	return lhs;
}
