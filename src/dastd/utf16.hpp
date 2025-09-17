/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 04-MAY-2024
**/
#pragma once
#include "defs.hpp"
#include "utf8.hpp"

namespace dastd {

// Maximum length of a UTF-16 sequence
constexpr size_t UTF16_CHAR_MAX_LEN = 3;

// Returns the position of the given character in a UTF-16 sequence.
enum utf16_char_type {
	utf16_NONE,  // Not a UTF-16 character
	utf16_FIRST, // First character of a UTF-16 surrogate sequence
	utf16_SECOND // Second character of a UTF-16 surrogate sequence
};

/// @brief Count the number of UTF-16 characters
///
/// Given a character, returns the number of extra characters that
/// are needed to form a valid UTF-16 sequence.
///
/// @param ch Character to be analyzed
///
/// @return Return 0 if the character submitted is to be used "as is"
/// or the number of extra characters that are needed to form a
/// valid UTF-16 sequence.
inline unsigned count_utf16_following_chars(char16_t ch)
{
	if ((ch & 0xD800) == 0xD800) return 1;
	return 0;
}

/// @brief Write a codepoint into a UTF-16 string.
///
/// @param utf16 The target string; it must be allocated of UTF16_CHAR_MAX_LEN char16_t characters.
/// @param code_point codepoint to be encoded
///
/// @return Returns the number of characters in the string (not including the zero
/// terminator) or zero in case of invalid codepoint.
/// The returned string is always zero terminated even if empty.
size_t write_utf32_to_utf16(char16_t* utf16, char32_t code_point);

/// @brief Calculates the length in UTF-16 of a char32 string
///
/// Note: it skips the characters that can not be calculated
///
/// @return Returns the length in char16_t characters
size_t calc_utf16_length(const char32_t* string, size_t length);
inline size_t calc_utf16_length(const std::u32string& str) {return calc_utf16_length(str.data(), str.size());}

/// @brief Calculates the length in UTF-16 of a char32
/// @return Returns the number of char16_t characters required; returns 0 if the character can not be encoded in UTF-16
inline size_t calc_utf16_char_length(char32_t code_point)
{
	if (code_point < 0x10080) return 1;
	if (code_point <= 0x10FFFF) return 2;
	return 0;
}

/// @brief Decodes the utf16string into the utf32string
///
/// @param utf16string  UTF-16 encoded string to be decoded
/// @param length      Number of char16_t characters in the `utf16string`
/// @param utf32string Buffer big enough to host the string; if `nullptr` it will not be written
///
/// Use `utf32string=nullptr` to calculate the size first. Then pass an
/// adeguately allocated buffer.
///
/// @return Returns the length in char32_t characters;
size_t write_utf16_to_utf32(const char16_t* utf16string, size_t length, char32_t* utf32string=nullptr);

/// @brief Decodes the utf16string into the std::u32string
void write_utf16_to_u32string(const char16_t* utf16string, size_t length, std::u32string& str);
inline void write_utf16_to_u32string(const std::u16string& utf16string, std::u32string& str) {write_utf16_to_u32string(utf16string.data(), utf16string.size(), str);}

/// @brief Writes an UTF-32 string into a UTF-16 string
void write_u32string_to_utf16(const std::u32string& src, std::u16string& dst);


/// @brief Returns the position of the given character in a UTF-16 sequence.
inline utf16_char_type detect_utf16_char(char16_t ch)
{
	// Detect the sequence 110110yyyyyyyyyy
	if ((ch & 0xFC00) == 0xD800) return utf16_FIRST;

	// Detect the sequence 110111xxxxxxxxxx
	if ((ch & 0xFC00) == 0xDC00 ) return utf16_SECOND;

	return utf16_NONE;
}

/// @brief Converts the indicated UTF-8 string into a single UTF-16 character
/// @param utf8_string            The input UTF-8 string (does not need to be zero terminated)
/// @param utf8_string_length     The length of the UTF-8 string, not including the zero terminator if any
/// @param utf8_string_read       The number of characters read from the `utf8_string` to parse one character
/// @param utf16_string           The output UTF-16 single or double character
/// @param utf16_string_written   The number of characters written to `utf16_string` (either 1 or 2)
/// @return                       Returns `true` if ok, `false` if the input string was not a valid UTF-8 string.
bool write_utf8_to_utf16(const char* utf8_string, size_t utf8_string_length, size_t& utf8_string_read, char16_t utf16_string[UTF16_CHAR_MAX_LEN], size_t& utf16_string_written);


/// @brief Converts the indicated UTF-8 string into a single UTF-16 character
/// @param utf8_string            The input UTF-8 string (does not need to be zero terminated)
/// @param utf8_string_length     The length of the UTF-8 string, not including the zero terminator if any
/// @param utf16_string           The output UTF-16 string
/// @return                       Returns `true` if ok, `false` if the input string was not a valid UTF-8 string.
bool write_utf8_to_utf16string(const char* utf8_string, size_t utf8_string_length, std::u16string& utf16_string);



/// @brief Convert a UTF-16 ASCIIZ sequence into a UNICODE-32
///
/// Reads from 1 to 4 characters from the UTF-16 input buffer and converts
/// them into a UNICODE-32 code point
///
/// @return Returns the number of characters read or zero in case the character
///         is not a valid UTF-16 sequence.
inline size_t read_utf16_asciiz(const char16_t* utf16string, char32_t& code_point)
{
	switch(detect_utf16_char(utf16string[0])) {
		case utf16_NONE: {
			code_point = (char32_t)utf16string[0];
			return 1;
		}
		case utf16_FIRST: {
			// Unpaired surrogate. This is not supported by the standard but widely
			// accepted: if a high surrogate code is not followed by a low one,
			// it is treated as a normal character.S
			if (detect_utf16_char(utf16string[1]) != utf16_SECOND) {
				code_point=(char32_t)utf16string[0];
				return 1;
			}
			code_point = 0;
			code_point = DASTD_BINSET(code_point, 10, 10, DASTD_BINGET(utf16string[0], 0, 10));
			code_point = DASTD_BINSET(code_point,  0, 10, DASTD_BINGET(utf16string[1], 0, 10));
			code_point += 0x10000;
			return 2;
		}
		// Unpaired surrogate. This is not supported by the standard but widely
		// accepted: if a high surrogate code is not followed by a low one,
		// it is treated as a normal character.S
		default: code_point=(char32_t)utf16string[0]; return 1;
	}
}

//------------------------------------------------------------------------------
// Write a codepoint into a UTF-16 string.
// The target string must be allocated of UTF16_CHAR_MAX_LEN char16_t characters.
// Returns the number of characters in the string (not including the zero
// terminator) or zero in case of invalid codepoint.
// The returned string is always zero terminated even if empty.
//------------------------------------------------------------------------------
inline size_t write_utf32_to_utf16(char16_t* utf16, char32_t code_point)
{
	// If the character is below 0x10000 and it is not a surrogate, encode
	// it as is.
	if ((code_point < 0x10000) && (detect_utf16_char((char16_t)code_point) == utf16_NONE)) {
		utf16[0] = (char16_t)code_point;
		utf16[1] = 0;
		return 1;
	}
	// Code point is out of range
	if (code_point > 0x10FFFF) {
		utf16[0] = 0;
		return 0;
	}
	if(code_point >= 0x10000) {
		code_point -= 0x10000;
		utf16[0] = (char16_t)(0xD800 | DASTD_BINGET(code_point, 10, 10));
		utf16[1] = (char16_t)(0xDC00 | DASTD_BINGET(code_point, 0,  10));
		utf16[2] = 0;
		return 2;
	}
	return 0;
}

// Calculates the length in UTF-16 of a char32 string
inline size_t calc_utf16_length(const char32_t* string, size_t length)
{
	size_t ret=0;
	size_t i;
	char16_t buf[UTF16_CHAR_MAX_LEN];
	for (i=0; i<length; i++) {
		ret += write_utf32_to_utf16(buf, string[i]);
	}
	return ret;
}

/// @brief Writes a char32 string in UTF-16 of the given string
///
/// Note: it skips the characters that can not be calculated
/// It never writes more than "max_output_length" characters.
/// It does not add a terminating zero.
///
/// @param output              Target buffer
/// @param max_output_length   Maximum number of char16_t that can be written in the target buffer
/// @param char16_written      Number of char16_t actually written to the target buffer
/// @param chars32_written     Number of char32_t characters from the input string actually written to the target buffer
/// @param input_string        Input string
/// @param input_length        Numer of char32_t characters in the input string
///
/// @return Returns the number of char16_t characters required to write the entire input string. This
///         value can be greater than `char16_written` in case the output buffer is not big enough.
///
inline size_t write_utf16_to_char_buffer(char16_t* output, size_t max_output_length, size_t& char16_written, size_t& chars32_written, const char32_t* input_string, size_t input_length)
{
	size_t ret=0;
	size_t i;
	size_t len;

	char16_written = 0;
	chars32_written = 0;
	for (i=0; i<input_length; i++) {
		len = calc_utf16_char_length(input_string[i]);
		if (len > max_output_length) break;
		write_utf32_to_utf16(output, input_string[i]);
		output += len;
		max_output_length -= len;
		ret += len;
		chars32_written ++;
	}
	char16_written = ret;
	for (; i<input_length; i++) {
		ret += calc_utf16_char_length(input_string[i]);
	}

	return ret;

}


//------------------------------------------------------------------------------
// (brief) Decodes the utf16string into the utf32string
//
// (param) utf16string  UTF-16 encoded string to be decoded
// (param) length      Number of char16_t characters in the `utf16string`
// (param) utf32string Buffer big enough to host the string; if `nullptr` it will not be written
//
// Use `utf32string=nullptr` to calculate the size first. Then pass an
// adeguately allocated buffer.
//
// (return) Returns the length in char32_t characters;
//------------------------------------------------------------------------------
inline size_t write_utf16_to_utf32(const char16_t* utf16string, size_t length, char32_t* utf32string)
{
	size_t inp=0;
	size_t out=0;
	for(;;) {
		char32_t code_point;
		size_t len = count_utf16_following_chars(utf16string[inp]);
		if (inp+len > length) break;

		len = read_utf16_asciiz(utf16string+inp, code_point);
		if (len == 0) break;
		inp += len;
		if (utf32string) utf32string[out] = code_point;
		out++;
	}
	return out;
}


//------------------------------------------------------------------------------
// (brief) Decodes the utf16string into the std::u32string
//------------------------------------------------------------------------------
inline void write_utf16_to_u32string(const char16_t* utf16string, size_t length, std::u32string& str)
{
	size_t len = write_utf16_to_utf32(utf16string, length, nullptr);
	if (len > 0) len--;
	str.resize(len);
	write_utf16_to_utf32(utf16string, length, str.data());
}


//------------------------------------------------------------------------------
// (brief) Writes an UTF-32 string into a UTF-16 string
//------------------------------------------------------------------------------
inline void write_u32string_to_utf16(const std::u32string& src, std::u16string& dst)
{
	char16_t buf[UTF16_CHAR_MAX_LEN];
	size_t ch16_count = calc_utf16_length(src);
	dst.reserve(ch16_count + dst.size());
	for(auto ch32: src) {
		size_t len = write_utf32_to_utf16(buf, ch32);
		dst.append(buf, len);
	}
}

//------------------------------------------------------------------------------
// (brief) Converts the indicated UTF-8 string into a single UTF-16 character
//------------------------------------------------------------------------------
inline bool write_utf8_to_utf16(const char* utf8_string, size_t utf8_string_length, size_t& utf8_string_read, char16_t utf16_string[UTF16_CHAR_MAX_LEN], size_t& utf16_string_written)
{
	char32_t u32_code_point;
	size_t n = count_utf8_following_chars(utf8_string[0])+1;
	if (n > utf8_string_length) return false;
	utf8_string_read = read_utf8_asciiz(utf8_string, u32_code_point);
	assert(utf8_string_read == n);
	utf16_string_written = write_utf32_to_utf16(utf16_string, u32_code_point);
	return true;
}

//------------------------------------------------------------------------------
// (brief) Converts the indicated UTF-8 string into a single UTF-16 character
//------------------------------------------------------------------------------
inline bool write_utf8_to_utf16string(const char* utf8_string, size_t utf8_string_length, std::u16string& utf16_string)
{
	char16_t utf16_temp_string[UTF16_CHAR_MAX_LEN];
	size_t utf16_temp_string_len;
	size_t utf8_string_read;
	size_t utf8_string_offset=0;

	while(utf8_string_offset < utf8_string_length) {
		if (!write_utf8_to_utf16(utf8_string+utf8_string_offset, utf8_string_length-utf8_string_offset, utf8_string_read, utf16_temp_string, utf16_temp_string_len)) return false;
		utf16_string.append(utf16_temp_string, utf16_temp_string_len);
		utf8_string_offset += utf8_string_read;
	}
	return true;
}

}
