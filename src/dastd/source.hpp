/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 09-APR-2024
**/
#pragma once
#include "defs.hpp"

namespace dastd {

/// @brief Simple binary data source
///
/// This class represents the most basic interface for those classes able
/// to produce a flow of characters until reaching an end.
template<concept_integral CHARTYPE>
class source {
	public:
		/// @brief Destructor
		virtual ~source() {}

		/// @brief Fetch one byte
		/// @param data   Character read from the stream
		/// @return       Returns `true` if one character has been fetched; returns `false` if no
		///               more characters are available and `data` is not valid
		/// @throw        It can throw a std::exception or one of its derivatives
		virtual bool tentative_read_char(CHARTYPE& data) {return (tentative_read(&data, 1) == 1);}

		/// @brief Non-blocking method to read data from the source
		///
		/// This method will attempt to read up to `data_size` characters.
		/// If less (or even zero) characters are available, it will read them
		/// and return.
		///
		/// @param data      Pointer to the buffer that will host the data read
		/// @param data_size Max amount of characters it should try to read
		/// @return          Returns the number of characters actually read. It can be zero.
		/// @throw           It can throw a std::exception or one of its derivatives
		virtual size_t tentative_read(CHARTYPE* data, size_t data_size) = 0;

		/// @brief Non-blocking method to read data from the source
		///
		/// This method will attempt to read up to `data_size` characters.
		/// If less (or even zero) characters are available, it will read them
		/// and return.
		///
		/// @param data      Available data will be appended to the `data` string
		/// @param data_size Max amount of characters it should try to read
		/// @return          Returns the number of characters actually read. It can be zero.
		/// @throw           It can throw a std::exception or one of its derivatives
		size_t tentative_read(std::basic_string<CHARTYPE>& data, size_t data_size);

		/// @brief Non-blocking method to discard data from the source
		///
		/// This method will attempt to discard up to `data_size` characters.
		/// If less (or even zero) characters are available, it will discard them
		/// and return.
		///
		/// @param data_size Max amount of characters it should try to read
		/// @return          Returns the number of characters actually read. It can be zero.
		/// @throw           It can throw a std::exception or one of its derivatives
		///
		/// Default implementation calls `tentative_read` in a dummy buffer.
		/// Other implementations can optimize this.
		virtual size_t tentative_discard(size_t data_size);
};

//------------------------------------------------------------------------------
// (brief) Non-blocking method to read data from the source
//
// This method will attempt to read up to `data_size` characters.
// If less (or even zero) characters are available, it will read them
// and return.
//
// Default implementation is to repeatedly call `tentative_read_char`.
// However, optimized implementations are highly recommended.
//
// (param) data      Pointer to the buffer that will host the data read
// (param) data_size Max amount of characters it should try to read
// (return)          Returns the number of characters actually read. It can be zero.
// (throw)           It can throw a std::exception or one of its derivatives
//------------------------------------------------------------------------------
template<concept_integral CHARTYPE>
size_t source<CHARTYPE>::tentative_read(CHARTYPE* data, size_t data_size)
{
	size_t i;
	for (i=0; i<data_size; i++) {
		if (!tentative_read_char(data[i])) return i;
	}
	return data_size;
}


//------------------------------------------------------------------------------
// (brief) Non-blocking method to discard data from the source
//
// This method will attempt to discard up to `data_size` characters.
// If less (or even zero) characters are available, it will discard them
// and return.
//
// (param) data_size Max amount of characters it should try to read
// (return)          Returns the number of characters actually read. It can be zero.
// (throw)           It can throw a std::exception or one of its derivatives
//
// Default implementation calls `tentative_read` in a dummy buffer.
// Other implementations can optimize this.
//------------------------------------------------------------------------------
template<concept_integral CHARTYPE>
size_t source<CHARTYPE>::tentative_discard(size_t data_size)
{
	constexpr size_t BUFSIZE = 256;
	CHARTYPE buf[BUFSIZE];
	size_t totsize = 0;
	while (totsize < data_size) {
		size_t reqsize = std::min(data_size, BUFSIZE);
		size_t readsize = tentative_read(buf, reqsize);
		totsize += readsize;
		if (reqsize > readsize) break;
	}
	return totsize;
}

//------------------------------------------------------------------------------
// (brief) Non-blocking method to read data from the source
//
// This method will attempt to read up to `data_size` characters.
// If less (or even zero) characters are available, it will read them
// and return.
//
// (param) data      Available data will be appended to the `data` string
// (param) data_size Max amount of characters it should try to read
// (return)          Returns the number of characters actually read. It can be zero.
// (throw)           It can throw a std::exception or one of its derivatives
//------------------------------------------------------------------------------
template<concept_integral CHARTYPE>
size_t source<CHARTYPE>::tentative_read(std::basic_string<CHARTYPE>& data, size_t data_size)
{
	size_t offset = data.size();
	data.resize(offset + data_size);
	// Try to read the number of characters returned by "tentative_count".
	// The value could be different.
	size_t characters_read = tentative_read(data.data()+offset, data_size);
	assert(characters_read <= data_size);
	if (characters_read < data_size) {
		data.resize(data.size() - (data_size - characters_read));
	}
	return characters_read;
}


} // namespace dastd
