#pragma once
#include <cstdint>
#include <algorithm>
#include <vector>
#include <string>
#include <stdexcept>
#include "types.h"

namespace wafab {

/// <summary>
/// An unsigned integer, that has a variable number of bytes.
/// </summary>
class UBigInt {
private:
	void reallocate_bytes(u64 newLength);
public:
	u64 length;
	u8* bytes;

	// MARK: constructors
	UBigInt();
	UBigInt(const u64& number);
	UBigInt(const char* str);
	UBigInt(const UBigInt& other);
	UBigInt(u8* arr, u64 length);
	// TODO: move-constructor
	~UBigInt();


	// MARK: arithmetic operations
	UBigInt& add(const UBigInt& rhs);
	UBigInt& multiply(const UBigInt& rhs);
	UBigInt& power(const UBigInt& rhs);
	UBigInt& subtract(const UBigInt& rhs);
	/// <summary>
	///	Divides this UBigInt by another by long division. 
	/// </summary>
	/// <param name="remainder">Must be 0-initialized. Is the remainder of this operation.</param>
	/// <param name="rhs">The UBigInt this is divided by.</param>
	/// <returns>*this</returns>
	UBigInt& divide(UBigInt & remainder, const UBigInt & rhs);
	UBigInt& mod(const UBigInt & rhs);
	UBigInt& add_array_at_index(u8* arr, u64 l, u64 index);
	UBigInt& multiply_array(const u8* arr, u64 l);

	UBigInt& lsh(const u64& rhs);
	UBigInt& rsh(const u64& rhs);
	UBigInt& bit_and(const UBigInt& rhs);
	UBigInt& bit_or(const UBigInt & rhs);
	UBigInt& bit_xor(const UBigInt & rhs);
	UBigInt& bit_not();

	UBigInt& add(u64& rhs);
	UBigInt& multiply(const u64& rhs);
	UBigInt& power(const u64& rhs);
	UBigInt& subtract(const u64& rhs);
	UBigInt& divide(u64 & remainder, u64 rhs);
	UBigInt& bit_and(const u64& rhs);
	UBigInt& bit_or(const u64 & rhs);
	UBigInt& bit_xor(const u64 & rhs);


	// MARK: comparison operations
	bool equals(const u64& rhs) const;
	bool equals(const UBigInt& rhs) const;
	bool not_equals(const u64& rhs) const;
	bool not_equals(const UBigInt& rhs) const;
	bool smaller_than(const u64& rhs) const;
	bool smaller_than(const UBigInt& rhs) const;
	bool bigger_than(const u64& rhs);
	bool bigger_than(const UBigInt& rhs) const;
	bool smaller_or_equal(const u64& rhs) const;
	bool smaller_or_equal(const UBigInt& rhs) const;
	bool bigger_or_equal(const u64& rhs) const;
	bool bigger_or_equal(const UBigInt& rhs) const;


	// MARK: miscellanious methods
	u64 most_significant_bit() const;
	/// <summary>
	///	fetches the nth bit 
	/// </summary>
	/// <param name="n"></param>
	/// <returns>true, if bit is one, false otherwise</returns>
	bool get_bit(u64 n) const;
	std::string to_string(u8 base) const;
	void shrink_to_fit();
	UBigInt& deep_copy(const UBigInt& rhs);
	void to_u64(u64& out) const;
	UBigInt& from_u64(const u64& rhs);


	// MARK: operator overloading
	UBigInt& operator+=(const UBigInt& rhs);
	UBigInt& operator+=(const u64& rhs);
	friend UBigInt operator+(UBigInt lhs, const UBigInt& rhs);
	friend UBigInt operator+(UBigInt lhs, const u64& rhs);

	UBigInt& operator*=(const UBigInt& rhs);
	UBigInt& operator*=(const u64& rhs);
	friend UBigInt operator*(UBigInt lhs, const UBigInt& rhs);
	friend UBigInt operator*(UBigInt lhs, const u64& rhs);

	UBigInt& operator-=(const UBigInt& rhs);
	UBigInt& operator-=(const u64& rhs);
	friend UBigInt operator-(UBigInt lhs, const UBigInt& rhs);
	friend UBigInt operator-(UBigInt lhs, const u64& rhs);

	UBigInt& operator/=(const UBigInt& rhs);
	UBigInt& operator/=(const u64& rhs);
	friend UBigInt operator/(UBigInt lhs, const UBigInt& rhs);
	friend UBigInt operator/(UBigInt lhs, const u64& rhs);

	UBigInt& operator%=(const UBigInt& rhs);
	UBigInt& operator%=(const u64& rhs);
	friend UBigInt operator%(UBigInt lhs, const UBigInt& rhs);
	friend UBigInt operator%(UBigInt lhs, const u64& rhs);
	
	UBigInt& operator++();
	UBigInt& operator++(int);
	UBigInt& operator--();
	UBigInt& operator--(int);

	UBigInt& operator<<=(const u64& rhs);
	friend UBigInt operator<<(UBigInt lhs, const u64& rhs);

	UBigInt& operator>>=(const u64& rhs);
	friend UBigInt operator>>(UBigInt lhs, const u64& rhs);

	UBigInt& operator|=(const u64& rhs);
	friend UBigInt operator|(UBigInt lhs, const u64& rhs);

	UBigInt& operator&=(const u64& rhs);
	friend UBigInt operator&(UBigInt lhs, const u64& rhs);

	UBigInt& operator^=(const u64& rhs);
	friend UBigInt operator^(UBigInt lhs, const u64& rhs);

	UBigInt operator~();


	friend bool operator==(const UBigInt& lhs, const UBigInt& rhs);
	friend bool operator==(const UBigInt& lhs, const u64& rhs);
	friend bool operator!=(const UBigInt& lhs, const UBigInt& rhs);
	friend bool operator!=(const UBigInt& lhs, const u64& rhs);
	friend bool operator<(const UBigInt& lhs, const UBigInt& rhs);
	friend bool operator<(const UBigInt& lhs, const u64& rhs);
	friend bool operator>(const UBigInt& lhs, const UBigInt& rhs);
	friend bool operator>(const UBigInt& lhs, const u64& rhs);
	friend bool operator<=(const UBigInt& lhs, const UBigInt& rhs);
	friend bool operator<=(const UBigInt& lhs, const u64& rhs);
	friend bool operator>=(const UBigInt& lhs, const UBigInt& rhs);
	friend bool operator>=(const UBigInt& lhs, const u64& rhs);


	u8& operator[](u64 index);
	UBigInt& operator=(const UBigInt& rhs);
	UBigInt& operator=(const u64& rhs);

	friend std::ostream& operator<<(std::ostream& os, UBigInt& number);

};

}
