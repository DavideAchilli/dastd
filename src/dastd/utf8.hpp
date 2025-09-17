/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 04-AUG-2023
**/
#pragma once
#include "defs.hpp"

namespace dastd {

// Maximum length of a UTF-8 sequence
constexpr size_t UTF8_CHAR_MAX_LEN = 5;

// Returns the position of the given character in a UTF-8 sequence.
enum utf8_char_type {
	utf8_NONE,  // Not a UTF-8 character
	utf8_FIRST, // First character of a UTF-8 sequence
	utf8_NEXT   // Following character of a UTF-8 sequence
};

/// @brief Count the number of UTF-8 characters
///
/// Given a character, returns the number of extra characters that
/// are needed to form a valid UTF-8 character.
///
/// @param ch Character to be analyzed
///
/// @return Return 0 if the character submitted is to be used "as is"
/// or the number of extra characters that are needed to form a
/// valid UTF-8 character.
inline size_t count_utf8_following_chars(uint8_t ch)
{
	if ((ch & 0xE0) == 0xC0) return 1;
	if ((ch & 0xF0) == 0xE0) return 2;
	if ((ch & 0xF8) == 0xF0) return 3;
	return 0;
}

/// @brief Write a codepoint into a UTF-8 string.
///
/// @param utf8 The target string; it must be allocated of UTF8_CHAR_MAX_LEN bytes.
/// @param code_point codepoint to be encoded
///
/// @return Returns the number of characters in the string (not including the zero
/// terminator) or zero in case of invalid codepoint.
/// The returned string is always zero terminated even if empty.
size_t write_utf8_asciiz(char* utf8, char32_t code_point);

/// @brief Calculates the length in UTF-8 of a char32 string
///
/// Note: it skips the characters that can not be calculated
///
/// @return Returns the length in bytes
size_t calc_utf8_length(const char32_t* string, size_t length);

/// @brief Calculates the length in UTF-8 of a char32
/// @return Returns the length in bytes; returns 0 if the character can not be encoded in UTF8
inline size_t calc_utf8_char_length(char32_t code_point)
{
	if (code_point < 0x0080) return 1;
	if (code_point < 0x0800) return 2;
	if (code_point < 0x10000) return 3;
	if (code_point < 0x10FFFF) return 4;
	return 0;
}

/// @brief Decodes the utf8string into the utf32string
///
/// @param utf8string  UTF-8 encoded string to be decoded
/// @param length      Number of bytes in the `utf8string`
/// @param utf32string Buffer big enough to host the string; if `nullptr` it will not be written
///
/// Use `utf32string=nullptr` to calculate the size first. Then pass an
/// adeguately allocated buffer.
///
/// @return Returns the length in char32_t characters;
size_t write_utf8_to_utf32(const char* utf8string, size_t length, char32_t* utf32string=nullptr);

/// @brief Decodes the utf8string into the std::u32string
void write_utf8_to_u32string(const char* utf8string, size_t length, std::u32string& str);


/// @brief Returns the position of the given character in a UTF-8 sequence.
inline utf8_char_type detect_utf8_char(char ch)
{
	// Detect the sequence 110xxxxx, 2 bytes UTF-8
	if ((ch & 0xE0) == 0xC0) return utf8_FIRST;

	// Detect the sequence 1110xxxx, 3 bytes UTF-8
	if ((ch & 0xF0) == 0xE0) return utf8_FIRST;

	// Detect the sequence 11110xxx, 4 bytes UTF-8
	if ((ch & 0xF8) == 0xF0) return utf8_FIRST;

	// Detect the sequence 10xxxxxx, character after first
	if ((ch & 0xC0) == 0x80) return utf8_NEXT;

	return utf8_NONE;
}

/// @brief Convert a UTF-8 ASCIIZ sequence into a UNICODE-32
///
/// Reads from 1 to 4 characters from the UTF-8 input buffer and converts
/// them into a UNICODE-32 code point
///
/// @return Returns the number of characters read or zero in case the character
///         is not a valid UTF-8 sequence.
inline size_t read_utf8_asciiz(const void* p_chars, char32_t& code_point)
{
	size_t count = 0;
	#define chars ((const uint8_t*)p_chars)
	#define UTF8(n) (chars[(n)])
	#define CHECK_UTF_BYTE(n) if ((chars[(n)] & 0xC0) != 0x80) {code_point=CHAR32_INVALID; return 0;}

	#if defined __GNUC__
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Warray-bounds"
	#endif

	if (UTF8(0) == '\0') {code_point=CHAR32_INVALID; return 0;}

	// Case 1: 7-bits character
	if (DASTD_NONEISSET(UTF8(0), 0x80)) {
		code_point = UTF8(0);
		count = 1;
	}
	else {
		code_point=0;

		// Case 2: 11-bits character
		if ((chars[0] & 0xE0) == 0xC0) {
			//FETCH(2)
			CHECK_UTF_BYTE(1);
			code_point = DASTD_BINSET(code_point, 6, 5, DASTD_BINGET(UTF8(0), 0, 5));
			code_point = DASTD_BINSET(code_point, 0, 6, DASTD_BINGET(UTF8(1), 0, 6));
			count=2;
		}
		// Case 3: 11-bits character
		else if ((chars[0] & 0xF0) == 0xE0) {
			//FETCH(3)
			// Invalid UTF-8
			CHECK_UTF_BYTE(1);
			CHECK_UTF_BYTE(2);
			code_point = DASTD_BINSET(code_point, 12, 4, DASTD_BINGET(UTF8(0), 0, 4));
			code_point = DASTD_BINSET(code_point,  6, 6, DASTD_BINGET(UTF8(1), 0, 6));
			code_point = DASTD_BINSET(code_point,  0, 6, DASTD_BINGET(UTF8(2), 0, 6));
			count=3;
		}
		// Case 3: 21-bits character
		else if ((chars[0] & 0xF8) == 0xF0) {
			//FETCH(4)
			// Invalid UTF-8
			CHECK_UTF_BYTE(1);
			CHECK_UTF_BYTE(2);
			CHECK_UTF_BYTE(3);
			code_point = DASTD_BINSET(code_point, 18, 3, DASTD_BINGET(UTF8(0), 0, 3));
			code_point = DASTD_BINSET(code_point, 12, 6, DASTD_BINGET(UTF8(1), 0, 6));
			code_point = DASTD_BINSET(code_point,  6, 6, DASTD_BINGET(UTF8(2), 0, 6));
			code_point = DASTD_BINSET(code_point,  0, 6, DASTD_BINGET(UTF8(3), 0, 6));
			count=4;
		}
		else {
			code_point = chars[0];
			count = 1;
		}
	}
	return count;
	#undef chars
	#undef UTF8
	#undef CHECK_UTF_BYTE
	#if defined __GNUC__
	#pragma GCC diagnostic pop
	#endif
}

/// @brief Write a codepoint into a UTF-8 string.
///
/// @param utf8 The target string must be allocated of UTF8_CHAR_MAX_LEN bytes.
/// @param code_point Ths source codepoint
/// @return Returns the number of characters in the string (not including the zero
///     terminator) or zero in case of invalid codepoint.
///     The returned string is always zero terminated even if empty.
inline size_t write_utf8_asciiz(char* utf8, char32_t code_point)
{
	size_t written;
	// 1-byte value
	if (code_point < 0x0080) {
		utf8[0] = (char)code_point;
		written=1;
	}
	// 2-bytes value
	else if (code_point < 0x0800) {
		utf8[0] = (char)(0xC0 | DASTD_BINGET(code_point, 6, 5));
		utf8[1] = (char)(0x80 | DASTD_BINGET(code_point, 0, 6));
		written=2;
	}
	// 3-bytes value
	else if (code_point < 0x10000) {
		utf8[0] = (char)(0xE0 | DASTD_BINGET(code_point, 12, 4));
		utf8[1] = (char)(0x80 | DASTD_BINGET(code_point, 6, 6));
		utf8[2] = (char)(0x80 | DASTD_BINGET(code_point, 0, 6));
		written=3;
	}
	// 4-bytes value
	else if (code_point < 0x10FFFF) {
		utf8[0] = (char)(0xF0 | DASTD_BINGET(code_point, 18, 3));
		utf8[1] = (char)(0x80 | DASTD_BINGET(code_point, 12, 6));
		utf8[2] = (char)(0x80 | DASTD_BINGET(code_point, 6, 6));
		utf8[3] = (char)(0x80 | DASTD_BINGET(code_point, 0, 6));
		written=4;
	}
	else {
		// Invalid UTF-8 VALUE (too big). Skipped.
		written=0;
	}
	utf8[written] = '\0';
	return written;
}

// Calculates the length in UTF-8 of a char32 string
inline size_t calc_utf8_length(const char32_t* string, size_t length)
{
	size_t ret=0;
	size_t i;
	char buf[UTF8_CHAR_MAX_LEN];
	for (i=0; i<length; i++) {
		ret += write_utf8_asciiz(buf, string[i]);
	}
	return ret;
}

/// @brief Writes a char32 string in UTF-8 of the given stream
///
/// Note: it skips the characters that can not be calculated
///
/// @return Returns the length in bytes
inline size_t write_utf8(std::ostream& out, const char32_t* string, size_t length)
{
	size_t ret=0;
	size_t i;
	char buf[UTF8_CHAR_MAX_LEN];
	for (i=0; i<length; i++) {
		size_t len = write_utf8_asciiz(buf, string[i]);
		ret += len;
		out.write(buf, len);
	}
	return ret;
}

/// @brief Writes a char32 string in UTF-8 of the given string
///
/// Note: it skips the characters that can not be calculated
/// It never writes more than "max_output_length" characters.
/// It does not add a terminating zero.
///
/// @param output             Target buffer
/// @param max_output_length    Maximum number of bytes that can be written in the target buffer
/// @param bytes_written       Number of bytes actually written to the target buffer
/// @param chars_written       Number of characters from the input string actually written to the target buffer
/// @param input_string        Input string
/// @param input_length        Numer of char32 characters in the input string
///
/// @return Returns the number of bytes required to write the entire input string. This
///         value can be greater than `bytes_written` in case the output buffer is not big enough.
///
inline size_t write_utf8_to_char_buffer(char* output, size_t max_output_length, size_t& bytes_written, size_t& chars_written, const char32_t* input_string, size_t input_length)
{
	size_t ret=0;
	size_t i;
	size_t len;

	bytes_written = 0;
	chars_written = 0;
	for (i=0; i<input_length; i++) {
		len = calc_utf8_char_length(input_string[i]);
		if (len > max_output_length) break;
		write_utf8_asciiz(output, input_string[i]);
		output += len;
		max_output_length -= len;
		ret += len;
		chars_written ++;
	}
	bytes_written = ret;
	for (; i<input_length; i++) {
		ret += calc_utf8_char_length(input_string[i]);
	}

	return ret;

}


//------------------------------------------------------------------------------
// (brief) Decodes the utf8string into the utf32string
//
// (param) utf8string  UTF-8 encoded string to be decoded
// (param) length      Number of bytes in the `utf8string`
// (param) utf32string Buffer big enough to host the string; if `nullptr` it will not be written
//
// Use `utf32string=nullptr` to calculate the size first. Then pass an
// adeguately allocated buffer.
//
// (return) Returns the length in char32_t characters;
//------------------------------------------------------------------------------
inline size_t write_utf8_to_utf32(const char* utf8string, size_t length, char32_t* utf32string)
{
	size_t inp=0;
	size_t out=0;
	for(;;) {
		char32_t code_point;
		size_t len = count_utf8_following_chars(utf8string[inp]);
		if (inp+len > length) break;

		len = read_utf8_asciiz(utf8string+inp, code_point);
		if (len == 0) break;
		inp += len;
		if (utf32string) utf32string[out] = code_point;
		out++;
	}
	return out;
}


//------------------------------------------------------------------------------
// (brief) Decodes the utf8string into the std::u32string
//------------------------------------------------------------------------------
inline void write_utf8_to_u32string(const char* utf8string, size_t length, std::u32string& str)
{
	size_t len = write_utf8_to_utf32(utf8string, length, nullptr);
	str.resize(len);
	write_utf8_to_utf32(utf8string, length, str.data());
}

}
