/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 11-AUG-2023
**/
#pragma once
#include "fmt_string_f.hpp"

namespace dastd {
	class sink_ch32;

	/// @brief Base class for objects that are able to write on a sink_ch32
	///
	/// Base class for objects that are able to write UNICODE char32_t characters
	/// to a sink_ch32.
	///
	/// A 'flooder' is an object able to write a given string to a sink. It can
	/// be invoked any number of times and it will write its contents to the sink.
	class flooder_ch32 {
		public:
			/// @brief Destructor
			virtual ~flooder_ch32() {}

			/// @brief Write on a sink_ch32
			/// @param sink Target `sink_ch32`
			virtual void write_to_sink(sink_ch32& sink) const = 0;

			/// @brief Write the content into a 8-bit string
			/// @tparam FORMAT One of the various string formats
			/// @return Returns the string
			template<fmt_string_format_t FORMAT=fmt_string_f::FMT_DEFAULT>
			std::string str() const;

			/// @brief Return the value encoded into an UTF-8 string
			std::string strUTF8() const;
	};

	/// @brief Regular std::ostream `<<` operator for the `flooder_ch32` type
	inline sink_ch32& operator<<(sink_ch32& sink, const flooder_ch32& f) {f.write_to_sink(sink); return sink;}

} // namespace dastd
