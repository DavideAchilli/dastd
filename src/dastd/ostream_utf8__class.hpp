/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 04-AUG-2023
**/
#if defined INCLUDE_dastd_ostream_utf8_class && !defined dastd_ostream_utf8_class
#define dastd_ostream_utf8_class

namespace dastd {

class sink_ch32;

/// @brief Implementation of a `std::ostream` that converts UTF-8 to char32_t
///
/// Reads a sequence of regular 8-bit characters encoded as UTF-8
/// and writes them as char32_t to a sink_ch32
class ostream_utf8: public ostream_basic {
	public:
		// @brief Constructor
		//
		// @param target Stream that will receive the actual output
		// @param last_wasEOL If true, it will start immediately with an indent; otherwise, the first line will not receive the indent.
		ostream_utf8(sink_ch32& target): m_target(target) {}

	protected:
		// @brief Write one character to the target stream
		virtual void write_char(char_type c) override;

	private:
		// Target sink, i.e. where the actual output will go
		sink_ch32& m_target;

		// Array of characters used to store the UTF-8 sequence
		char_type m_utf8seq[UTF8_CHAR_MAX_LEN] = {0};

		// Number of characters in m_utf8seq
		size_t m_utf8seq_len = 0;

		// Number of characters still to be stored in the utf8seq before recognition
		size_t m_utf8seq_left = 0;

};

} // namespace dastd
#endif
