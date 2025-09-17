/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 09-APR-2024
**/
#pragma once
#include "source.hpp"

namespace dastd {

/// @brief Binary data source that feeds data from an internal copy of a string_or_vector
template<class CHARTYPE>
class source_with_peek: public source<CHARTYPE> {
	public:
		/// @brief Return the minimal number of characters that can be read at the next calls
		///
		/// It returns the number of characters currently available in the buffer.
		/// It uses the ability of "peeking" ahead of these classes.
		virtual size_t tentative_count() const = 0;

		/// @brief Non-blocking method to read data from the source without extracting it
		///
		/// This method will attempt to read up to `data_size` characters.
		/// If less (or even zero) characters are available, it will read them
		/// and return. The data will not be extracted from the buffer so the
		/// next time the same data will be returned again.
		/// Call `tentative_discard` to discard the data that has been actually used.
		///
		/// @param data      Pointer to the buffer that will host the data read
		/// @param data_size Max amount of characters it should try to read
		/// @return          Returns the number of characters actually read. It can be zero.
		/// @throw           It can throw a std::exception or one of its derivatives
		virtual size_t tentative_peek(CHARTYPE* data, size_t data_size) = 0;

		/// @brief Fetch one byte
		/// @param data   Character read from the stream
		/// @return       Returns `true` if one character has been fetched; returns `false` if no
		///               more characters are available and `data` is not valid
		/// @throw        It can throw a std::exception or one of its derivatives
		virtual bool tentative_peek_char(CHARTYPE& data) {return (tentative_peek(&data, 1) == 1);}
};

} // namespace dastd
