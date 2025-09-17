/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 09-AUG-2023
**/
#pragma once
#include "ostream_basic.hpp"

namespace dastd {

/// @brief Implementation of a `ostream` that writes to a fixed char[] buffer
///
/// It writes up to buffer_size characters including the terminating zero.
/// The extra characters are discarded.
class ostream_charbuf: public ostream_basic {
	public:
		/// @brief Constructor
		///
		/// @param buffer     Target buffer
		/// @param buffer_size Size of the buffer; it will contain at most buffer_size-1 characters
		///                   and the '\0' terminator.
		ostream_charbuf(char_type* buffer, size_t buffer_size): m_ring_buffer(buffer), m_ring_buffer_size(buffer_size) {}

	protected:
		/// @brief Write one character to the target stream
		virtual void write_char(char_type c) override {if (m_chars_written+1 < m_ring_buffer_size) {m_ring_buffer[m_chars_written]=c; m_ring_buffer[m_chars_written+1]='\0'; m_chars_written++;}}

		/// @brief Buffer pointer
		char_type* m_ring_buffer;

		/// @brief Size of the buffer
		size_t m_ring_buffer_size;

		/// @brief Number of characters written so far
		size_t m_chars_written = 0;
};


} // namespace dastd
