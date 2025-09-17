/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 02-MAR-2024
*
* MSb 0 BIT NUMBERING
* ^^^^^^^^^^^^^^^^^^^
* With "MSb 0", bits are numbered from 0 onwards.
* Bit 0 is the most significant bit of the first octet.
* This numbering is very natural for an "left-to-right" ordered sequence
* of bits that, starting from the MSb of the first octet, sees the bits
* numbered starting from zero and growing.
*
* All the operations considering an array of bytes as an array of consecutive
* bits will use the MSb 0 bit numbering, where bit 0 is the MSb of the first octet,
* bit 8 the MSb of the second octet and so on.
*
* LSb 0 BIT NUMBERING
* ^^^^^^^^^^^^^^^^^^^
* With "LSb 0", bit 0 is the least significant bit of the first octet.
*
**/
#pragma once
#include "defs.hpp"

namespace dastd {

/// @brief Extract some bits from a word
///
/// Returns 'bits_count' bits from the 'value' word starting from 'bit_num'.
/// bit_num=0 is the least significative bit, while 'bit_num' indicates the
/// most significative bit of the sequence to be extracted. For example,
/// to extract the three-bits value at bits (4,3,2), we must use
/// bit_num_LSb0=4 and bits_count=3.
template<class WORDTYPE>
WORDTYPE get_bits_from_word(WORDTYPE value, unsigned bit_num_LSb0, unsigned bits_count) {
	return (((value) >> ((bit_num_LSb0)-((bits_count)-1))) & (WORDTYPE)((1 << (bits_count))-1));
}

/// @brief Convert an octet/LSb0 pair into a MSb0 single bit position
/// @param src_octet
/// @param src_LSb0
/// @return Returns the position of the bit in MSb0 ordering
constexpr size_t convert_LSb0_to_MSb0(size_t src_octet, size_t src_LSb0) {assert(src_LSb0 <= 7); return src_octet*8 + (7-src_LSb0);}

/// @brief Copy `bits_count` bits from one source buffer to a target one
/// @param tgt Target buffer
/// @param tgt_MSb0 Offset in bits in the target buffer where the first bit goes (MSb 0)
/// @param src Source buffer
/// @param src_MSb0 Offset in bits in the source buffer where the first bit is to be taken (MSb 0)
/// @param bits_count Number of bits to be copied
void copy_bits_MSb0(void* tgt, size_t tgt_MSb0, const void* src, size_t src_MSb0, size_t bits_count);

/// @brief Read one bit at the given position
/// @param src Source buffer
/// @param src_MSb0 Position of the bit (MSb 0)
/// @return Returns the value of the bit.
bool get_bit_MSb0(const void* src, size_t src_MSb0);

/// @brief Write one bit at the given position
/// @param dst Destination buffer
/// @param dst_MSb0 Position of the bit (MSb 0)
/// @param bitval Value of the bit.
void set_bit_MSb0(void* dst, size_t dst_MSb0, bool bitval);

/// @brief Read one bit at the given position
/// @param src Source buffer
/// @param src_octet Octet that contains the required bit (starting from 0)
/// @param src_LSb0 Position of the bit (LSb 0); it must be in range [0..7]
/// @return Returns the value of the bit.
bool get_bit_LSb0(const void* src, size_t src_octet, size_t src_LSb0);

/// @brief Write one bit at the given position
/// @param dst Destination buffer
/// @param dst_octet Octet that contains the required bit (starting from 0)
/// @param dst_LSb0 Position of the bit (MSb 0); it must be in range [0..7]
/// @param bitval Value of the bit.
void set_bit_LSb0(void* dst, size_t dst_octet, size_t dst_LSb0, bool bitval);

/// @brief Copy `bits_count` big-endian bits from one source buffer to an integral value
/// @tparam TYPE Target unsigned integral type
/// @param src Source buffer
/// @param src_MSb0 Offset in the source buffer where the first bit is to be taken (MSb 0)
/// @param bits_count Number of bits to be copied
/// @return Returns the value coded with the machine endianity
///
/// This method considers the values in the source buffer to be big-endian, i.e. the
/// MSb comes first.
template<class TYPE>
TYPE get_integral_MSb0(const void* src, size_t src_MSb0, size_t bits_count);

/// @brief Copy lowest `bits_count` bits from the `src` integral value at `dst` starting from bit `dst_MSb0`
/// @tparam TYPE Target unsigned integral type
/// @param dst Destination buffer
/// @param dst_MSb0 Offset in the destination buffer where the first bit is to be written (MSb 0)
/// @param bits_count Number of bits to be copied
///
/// This method encodes the values in the destination buffer as be big-endian, i.e. the
/// MSb comes first.
template<class TYPE>
void set_integral_MSb0(void* dst, size_t dst_MSb0, TYPE src, size_t bits_count);

/// @brief Copy `bits_count` big-endian bits from one source buffer to an integral value
/// @tparam TYPE Target unsigned integral type
/// @param src Source buffer
/// @param src_octet Octet that contains the required bit (starting from 0)
/// @param src_LSb0 Position of the bit (LSb 0); it must be in range [0..7]
/// @param bits_count Number of bits to be copied
/// @return Returns the value coded with the machine endianity
///
/// This method considers the values in the source buffer to be big-endian, i.e. the
/// MSb comes first.
/// The type `TYPE` can be an enum defined over an integral unsigned type,
/// like for example:
///
///      enum class MyEnum: uint8_t {...};
///
template<class TYPE>
TYPE get_integral_LSb0(const void* src, size_t src_octet, size_t src_LSb0, size_t bits_count);


/*
 _       _ _
(_)_ __ | (_)_ __   ___  ___
| | '_ \| | | '_ \ / _ \/ __|
| | | | | | | | | |  __/\__ \
|_|_| |_|_|_|_| |_|\___||___/

*/
//------------------------------------------------------------------------------
// Read one bit at the given position
//------------------------------------------------------------------------------
inline bool get_bit_MSb0(const void* src, size_t src_MSb0)
{
	uint8_t bit_in_octet = (1 << (7 - (src_MSb0 & 7)));
	uint8_t byte = (((const uint8_t*)src)[src_MSb0 >> 3]);
	return ((byte & bit_in_octet) != 0);
}

//------------------------------------------------------------------------------
// Write one bit at the given position
//------------------------------------------------------------------------------
inline void set_bit_MSb0(void* dst, size_t dst_MSb0, bool bitval)
{
	uint8_t bit_in_octet = (1 << (7 - (dst_MSb0 & 7)));
	uint8_t& byte = (((uint8_t*)dst)[dst_MSb0 >> 3]);
	if (bitval) byte |= bit_in_octet;
	else byte &= ~bit_in_octet;
}

//------------------------------------------------------------------------------
// Copy `bits_count` bits from one source buffer to a target one
//------------------------------------------------------------------------------
inline void copy_bits_MSb0(void* tgt, size_t tgt_MSb0, const void* src, size_t src_MSb0, size_t bits_count)
{
	size_t i;
	for (i=0; i<bits_count; i++) {
		set_bit_MSb0(tgt, tgt_MSb0+i, get_bit_MSb0(src, src_MSb0+i));
	}
}

//------------------------------------------------------------------------------
// Read one bit at the given position
//------------------------------------------------------------------------------
inline bool get_bit_LSb0(const void* src, size_t src_octet, size_t src_LSb0)
{
	return get_bit_MSb0(src, convert_LSb0_to_MSb0(src_octet, src_LSb0));
}

//------------------------------------------------------------------------------
// Write one bit at the given position
//------------------------------------------------------------------------------
inline void set_bit_LSb0(void* dst, size_t dst_octet, size_t dst_LSb0, bool bitval)
{
	set_bit_MSb0(dst, convert_LSb0_to_MSb0(dst_octet, dst_LSb0), bitval);
}


//------------------------------------------------------------------------------
// Concepts used in the bit_manip coding
//------------------------------------------------------------------------------
template <class TYPE>
concept bit_manip_bool = std::is_same_v<TYPE, bool>;

template <class TYPE>
concept bit_manip_enum = std::is_enum_v<TYPE> && std::is_integral_v<std::underlying_type_t<TYPE>> && std::is_unsigned_v<std::underlying_type_t<TYPE>>;

template <class TYPE>
concept bit_manip_integral = std::is_integral_v<TYPE> && std::is_unsigned_v<TYPE> && !std::is_same_v<TYPE, bool>;

//------------------------------------------------------------------------------
// Copy `bits_count` big-endian bits from one source buffer to an integral value
// This version accepts a `bool`
//------------------------------------------------------------------------------
template<bit_manip_bool TYPE>
TYPE get_integral_MSb0(const void* src, size_t src_MSb0, [[maybe_unused]] size_t bits_count)
{
	assert(bits_count==1);
	return (get_bit_MSb0(src, src_MSb0) != 0);
}

//------------------------------------------------------------------------------
// Copy `bits_count` big-endian bits from one source buffer to an integral value
// This version accepts an unsigned integral type
//------------------------------------------------------------------------------
template<bit_manip_integral TYPE>
TYPE get_integral_MSb0(const void* src, size_t src_MSb0, size_t bits_count)
{
	assert(bits_count <= sizeof(TYPE)*8);
	size_t i;
	TYPE tgt = 0;
	for (i=0; i<bits_count; i++) {
		tgt <<= 1;
		if (get_bit_MSb0(src, src_MSb0+i)) tgt |= 1;
	}
	return tgt;
}

//------------------------------------------------------------------------------
// Copy `bits_count` big-endian bits from one source buffer to an integral value
// This version accepts an `enum`
//------------------------------------------------------------------------------
template<bit_manip_enum TYPE>
TYPE get_integral_MSb0(const void* src, size_t src_MSb0, size_t bits_count)
{
	return (TYPE)get_integral_MSb0<std::underlying_type_t<TYPE>>(src, src_MSb0, bits_count);
}

//------------------------------------------------------------------------------
// Copy lowest `bits_count` bits from the `src` integral value at `dst` starting from bit `dst_MSb0`
// This version accepts an bool
//------------------------------------------------------------------------------
template<bit_manip_bool TYPE>
void set_integral_MSb0(void* dst, size_t dst_MSb0, TYPE src)
{
	set_bit_MSb0(dst, dst_MSb0, src);
}

//------------------------------------------------------------------------------
// Copy lowest `bits_count` bits from the `src` integral value at `dst` starting from bit `dst_MSb0`
// This version accepts an unsigned integral type
//------------------------------------------------------------------------------
template<bit_manip_integral TYPE>
void set_integral_MSb0(void* dst, size_t dst_MSb0, TYPE src, size_t bits_count)
{
	assert(bits_count <= sizeof(TYPE)*8);
	size_t i;
	for (i=0; i<bits_count; i++) {
		set_bit_MSb0(dst, dst_MSb0+i, ((src >> (bits_count - 1 - i)) & 1) == 1);
	}
}

//------------------------------------------------------------------------------
// Copy lowest `bits_count` bits from the `src` integral value at `dst` starting from bit `dst_MSb0`
// This version accepts an enum integral type
//------------------------------------------------------------------------------
template<bit_manip_enum TYPE>
void set_integral_MSb0(void* dst, size_t dst_MSb0, TYPE src, size_t bits_count)
{
	set_integral_MSb0<std::underlying_type_t<TYPE>>(dst, dst_MSb0, (std::underlying_type_t<TYPE>)src, bits_count);
}

//------------------------------------------------------------------------------
// Copy `bits_count` big-endian bits from one source buffer to an integral value
//------------------------------------------------------------------------------
template<class TYPE>
TYPE get_integral_LSb0(const void* src, size_t src_octet, size_t src_LSb0, size_t bits_count)
{
	return get_integral_MSb0<TYPE>(src, convert_LSb0_to_MSb0(src_octet, src_LSb0), bits_count);
}

} // namespace dastd
