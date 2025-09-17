/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 11-AUG-2023
**/
#pragma once
#include "sink_ch32.hpp"
#include "fmt.hpp"
#include "fmt_string.hpp"

namespace dastd {
	/// @brief sink_ch32 that converts the char32_t characters into a 8-bit
	///        format and writes it to the std::ostream.
	template<fmt_string_format_t FORMAT=fmt_string_f::FMT_DEFAULT>
	class sink_ch32_ostream: public sink_ch32 {
		public:
			/// @brief Constructor
			sink_ch32_ostream(std::ostream& out): m_out(out) {}

		protected:
			/// @brief Put one char32_t character in the sink
			///
			/// @param code_point The character to be written
			virtual void internal_sink_write_char32(char32_t code_point) {print_8bit_format<char32_t,FORMAT>(m_out, code_point);}

		private:
			/// @brief Stream that will receive the data converted into the 8-bit `m_format`
			std::ostream& m_out;
	};


/// @brief Write the content into a 8-bit string
/// @tparam FORMAT One of the various string formats
/// @return Returns the string
template<fmt_string_format_t FORMAT>
std::string flooder_ch32::str() const {
	std::ostringstream str;
	sink_ch32_ostream<FORMAT> sink(str);
	write_to_sink(sink);
	return str.str();
}

/// @brief Return the value encoded into an UTF-8 string
inline std::string flooder_ch32::strUTF8() const {return str<fmt_string_f::UTF8>();}

} // namespace dastd
