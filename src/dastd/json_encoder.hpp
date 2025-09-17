/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 11-AUG-2023
*
* Implementation of a RFC 8259 compliant JSON decoder.
**/
#pragma once
#include "defs.hpp"
#include "fmt.hpp"
#include <iostream>

namespace dastd {


/// @brief Encodes a char32_t character in ASCII escaped JSON form
inline void json_encode_char(std::ostream& s, char32_t code_point)
{
	switch(code_point) {
		case 0x22: s << "\\\""; break; // quotation mark  U+0022
		case 0x5C: s << "\\\\"; break; // reverse solidus U+005C
		case 0x08: s << "\\b";  break; // backspace       U+0008
		case 0x0C: s << "\\f";  break; // form feed       U+000C
		case 0x0A: s << "\\n";  break; // line feed       U+000A
		case 0x0D: s << "\\r";  break; // carriage return U+000D
		case 0x09: s << "\\t";  break; // tab             U+0009
		default: {
			if (code_point >= 32 && code_point <= 126) {
				s << ((char)code_point);
			}
			else {
				// Standard UTF-16 characters excluding the surrogate pairs
				if (code_point <= 0xD7FF || (code_point >= 0xE000 && code_point <= 0xFFFF)) {
					s << "\\u" << fmt(code_point, 16, 4);
				}
				// Code points from the other planes (called Supplementary Planes) are encoded
				// as two 16-bit code units called a surrogate pair
				else if (code_point >= 0x010000 && code_point <= 0x10FFFF) {
					// Subtract 0x10000 from the codepoint
					code_point -= 0x10000;
					s
						// Encode the higher 10-bits using the 0xD800 surrogate
						<< "\\u" << fmt(0xD800 + ((code_point >> 10) & 0x3FF), 16, 4)
						// Encode the lower 10-bits using the 0xDC00 surrogate
						<< "\\u" << fmt(0xDC00 + (code_point & 0x3FF), 16, 4)
					;
				}
				else {
					// The remaining codepoints can not be encoded in UNICODE
					assert(0);
				}
			}
		}
	}
}

/// @brief Encodes a char32_t string in ASCII escaped JSON form
inline void json_encode_string(std::ostream& s, const std::u32string& string) {for(auto ch32: string) json_encode_char(s, ch32);}

/// @brief Encodes an UTF-8 string into JSON format
/// @param s Target stream (contains the encoded JSON string, without the double quotes)
/// @param str Input string
/// @param len Length of the input string
/// @return Returns `true` if ok, `false` in case of errors in the UTF-8 sequence
inline bool json_encode_string_from_UTF8(std::ostream& s, const char* str, size_t len) {
	size_t offs=0;
	char32_t ch32;
	for(; offs<len;) {
		size_t remaining = len-offs;
		if (count_utf8_following_chars(str[offs]) > remaining) return false;
		size_t p = read_utf8_asciiz(str+offs, ch32);
		if (p == 0) return false;
		json_encode_char(s, ch32);
		offs += p;
	}
	return true;
}

inline bool json_encode_string_from_UTF8(std::ostream& s, const std::string& utf8_string) {return json_encode_string_from_UTF8(s, utf8_string.c_str(), utf8_string.size());}


} // namespace dastd
