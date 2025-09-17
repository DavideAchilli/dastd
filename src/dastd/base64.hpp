/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 02-MAR-2024
**/
#pragma once
#include "defs.hpp"
#include "bit_manip.hpp"
#include <istream>

namespace dastd {

inline constexpr char base64_enc_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

inline constexpr uint8_t base64_BAD = 0xFF;
inline constexpr size_t base64_min = 43;
inline constexpr size_t base64_max = 122;

inline constexpr uint8_t base64_dec_chars[base64_max-base64_min+1] = {
	/*   0 ASCII='+'  43 0x2B */ 62,
	/*   1 ASCII=','  44 0x2C */ base64_BAD,
	/*   2 ASCII='-'  45 0x2D */ base64_BAD,
	/*   3 ASCII='.'  46 0x2E */ base64_BAD,
	/*   4 ASCII='/'  47 0x2F */ 63,
	/*   5 ASCII='0'  48 0x30 */ 52,
	/*   6 ASCII='1'  49 0x31 */ 53,
	/*   7 ASCII='2'  50 0x32 */ 54,
	/*   8 ASCII='3'  51 0x33 */ 55,
	/*   9 ASCII='4'  52 0x34 */ 56,
	/*  10 ASCII='5'  53 0x35 */ 57,
	/*  11 ASCII='6'  54 0x36 */ 58,
	/*  12 ASCII='7'  55 0x37 */ 59,
	/*  13 ASCII='8'  56 0x38 */ 60,
	/*  14 ASCII='9'  57 0x39 */ 61,
	/*  15 ASCII=':'  58 0x3A */ base64_BAD,
	/*  16 ASCII=';'  59 0x3B */ base64_BAD,
	/*  17 ASCII='<'  60 0x3C */ base64_BAD,
	/*  18 ASCII='='  61 0x3D */ base64_BAD,
	/*  19 ASCII='>'  62 0x3E */ base64_BAD,
	/*  20 ASCII='?'  63 0x3F */ base64_BAD,
	/*  21 ASCII='@'  64 0x40 */ base64_BAD,
	/*  22 ASCII='A'  65 0x41 */ 0,
	/*  23 ASCII='B'  66 0x42 */ 1,
	/*  24 ASCII='C'  67 0x43 */ 2,
	/*  25 ASCII='D'  68 0x44 */ 3,
	/*  26 ASCII='E'  69 0x45 */ 4,
	/*  27 ASCII='F'  70 0x46 */ 5,
	/*  28 ASCII='G'  71 0x47 */ 6,
	/*  29 ASCII='H'  72 0x48 */ 7,
	/*  30 ASCII='I'  73 0x49 */ 8,
	/*  31 ASCII='J'  74 0x4A */ 9,
	/*  32 ASCII='K'  75 0x4B */ 10,
	/*  33 ASCII='L'  76 0x4C */ 11,
	/*  34 ASCII='M'  77 0x4D */ 12,
	/*  35 ASCII='N'  78 0x4E */ 13,
	/*  36 ASCII='O'  79 0x4F */ 14,
	/*  37 ASCII='P'  80 0x50 */ 15,
	/*  38 ASCII='Q'  81 0x51 */ 16,
	/*  39 ASCII='R'  82 0x52 */ 17,
	/*  40 ASCII='S'  83 0x53 */ 18,
	/*  41 ASCII='T'  84 0x54 */ 19,
	/*  42 ASCII='U'  85 0x55 */ 20,
	/*  43 ASCII='V'  86 0x56 */ 21,
	/*  44 ASCII='W'  87 0x57 */ 22,
	/*  45 ASCII='X'  88 0x58 */ 23,
	/*  46 ASCII='Y'  89 0x59 */ 24,
	/*  47 ASCII='Z'  90 0x5A */ 25,
	/*  48 ASCII='['  91 0x5B */ base64_BAD,
	/*  49 ASCII='\'  92 0x5C */ base64_BAD,
	/*  50 ASCII=']'  93 0x5D */ base64_BAD,
	/*  51 ASCII='^'  94 0x5E */ base64_BAD,
	/*  52 ASCII='_'  95 0x5F */ base64_BAD,
	/*  53 ASCII='`'  96 0x60 */ base64_BAD,
	/*  54 ASCII='a'  97 0x61 */ 26,
	/*  55 ASCII='b'  98 0x62 */ 27,
	/*  56 ASCII='c'  99 0x63 */ 28,
	/*  57 ASCII='d' 100 0x64 */ 29,
	/*  58 ASCII='e' 101 0x65 */ 30,
	/*  59 ASCII='f' 102 0x66 */ 31,
	/*  60 ASCII='g' 103 0x67 */ 32,
	/*  61 ASCII='h' 104 0x68 */ 33,
	/*  62 ASCII='i' 105 0x69 */ 34,
	/*  63 ASCII='j' 106 0x6A */ 35,
	/*  64 ASCII='k' 107 0x6B */ 36,
	/*  65 ASCII='l' 108 0x6C */ 37,
	/*  66 ASCII='m' 109 0x6D */ 38,
	/*  67 ASCII='n' 110 0x6E */ 39,
	/*  68 ASCII='o' 111 0x6F */ 40,
	/*  69 ASCII='p' 112 0x70 */ 41,
	/*  70 ASCII='q' 113 0x71 */ 42,
	/*  71 ASCII='r' 114 0x72 */ 43,
	/*  72 ASCII='s' 115 0x73 */ 44,
	/*  73 ASCII='t' 116 0x74 */ 45,
	/*  74 ASCII='u' 117 0x75 */ 46,
	/*  75 ASCII='v' 118 0x76 */ 47,
	/*  76 ASCII='w' 119 0x77 */ 48,
	/*  77 ASCII='x' 120 0x78 */ 49,
	/*  78 ASCII='y' 121 0x79 */ 50,
	/*  79 ASCII='z' 122 0x7A */ 51
};

/// @brief	Given a binary input stream "in", encodes base-64 in the output stream
///	using the rfc4648 section 4 standard character set.
inline void base64_encode(std::istream& in, std::ostream& out)
{
	size_t bytes_read;
	uint8_t block[3];
	uint32_t word;
	size_t i;

	for (;;) {
		block[1] = '\0';
		block[2] = '\0';
		in.read((char*)block, 3);
		bytes_read = (size_t)in.gcount();
		if (bytes_read == 0) return;

		word = (((uint32_t)block[0]) << 16) | (((uint32_t)block[1]) << 8) | ((uint32_t)block[2]);
		for (i=0; i<=bytes_read; i++) {
			uint8_t val6bit = (uint8_t)get_bits_from_word(word, (unsigned)(23-i*6), 6);
			assert(val6bit < 64);
			out << base64_enc_chars[val6bit];
		}
		if (bytes_read < 3) {
			for (i=bytes_read; i<3; i++) {
				out << '=';
			}
			return;
		}
	}
}

/// @brief Given a binary input stream "in", decodes base-64 in the output stream
/// using the rfc4648 section 4 standard character set.
///
/// @return Returns true if ok, false if it encountered some invalid character or
/// an invalid overall length.
inline bool base64_decode(std::istream& in, std::ostream& out)
{
	uint8_t bytes[4];
	uint32_t conv[4];
	uint32_t binary;
	uint32_t i;
	uint32_t count=4;

	for (;;) {
		binary = 0;
		in.read((char*)bytes, 4);
		size_t bytes_read = (size_t)in.gcount();
		if (bytes_read == 0) return true;
		if (bytes_read != 4) return false; // The input data must be mulstiple of 4 bytes

		if (bytes[3] == '=') count = (bytes[2] == '=' ? 2 : 3);

		// Convert the 4 encoded character into their correspondent bytes
		// and check for errors
		for (i=0; i<count; i++) {
			conv[i] = ((bytes[i]<base64_min) || (bytes[i]>base64_max) ? base64_BAD : base64_dec_chars[bytes[i]-base64_min]);
			if (conv[i] == base64_BAD) return false; // Invalid characters are not admitted
		}

		switch(count) {
			// In this case, we have three full bytes into 4 6-bit values
			case 4: {
				binary = (conv[0] << 18) | (conv[1] << 12) | (conv[2] << 6) | conv[3];
				for (i=0; i<3; i++) {
					out.put((uint8_t)get_bits_from_word(binary, (unsigned)(23-i*8), 8));
				}
				break;
			}

			// In this case, we have two full bytes into 3 6-bit values; we have to
			// discard two trailing bits.
			case 3: {
				binary = (conv[0] << 10) | (conv[1] << 4) | (conv[2] >> 2);
				for (i=0; i<2; i++) {
					out.put((uint8_t)get_bits_from_word(binary, (unsigned)(15-i*8), 8));
				}
				return (in.peek() == EOF);
			}

			// In this case, we have one single byte into 2 6-bit values; we have to
			// discard four trailing bits.
			case 2: {
				binary = (conv[0] << 2) | (conv[1] >> 4);
				out.put((uint8_t)binary);
				return (in.peek() == EOF);
			}

			// Default: unexpected
			default: assert(0);
		}
	}
}

} // namespace dastd
