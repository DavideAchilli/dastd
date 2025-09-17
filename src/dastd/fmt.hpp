/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 04-AUG-2023
*
* @brief   This module implements a set of formatters that can be used to print
*          on regular std::ostream streams.
*          Temporary instances of these objects take a source object and print
*          it using a given format.
*
**/
#pragma once
#include "defs.hpp"
#include "fmt32.hpp"
#include "string_tools.hpp"
#include <ostream>
#include <sstream>
namespace dastd {

class char32string;
class flooder_ch32;


/// @brief Helper class that allows formatting a number while printing on a stream
///
/// The `fmt` helper class can be embedded in a stream << sequence:
///
///     o << "Value 0x" << fmt(number, 16, 4) << std::endl;
///
template<class NUMTYPE>
class fmt {
	static_assert(std::is_integral<NUMTYPE>::value, "Integral required");
	static constexpr size_t buf_size = 66;
	inline static const char* conv_set_uc = "0123456789ABCDEFGHIJKLMNOPQRSTUVXYZ";
	inline static const char* conv_set_lc = "0123456789abcdefghijklmnopqrstuvxyz";
	public:
		/// @brief Constructor
		///
		/// @param value Value to be formatted
		/// @param base Formatting base; valid values range from 2 to 36
		/// @param zero_pad Minimum size that will trigger padding with '0'; if negative, pads with spaces
		/// @param lower_case Set to 'true' to have the letters 'a...z' written in lower case
		///     instead of the default upper case.
		fmt(NUMTYPE value, std::make_unsigned_t<NUMTYPE> base=10, int zero_pad=0, bool lower_case=false):
			m_value(value), m_base(base), m_zero_pad(zero_pad), m_conv_set(lower_case ? conv_set_lc : conv_set_uc)
			{
				assert((base >= 2) && (base < 36));
				assert((zero_pad < 0) ? ((size_t)(-zero_pad)) < buf_size : ((size_t)(zero_pad)) < buf_size);
			}

		/// @brief Print the formatted value on a ostream
		///
		/// @param o Target stream
		void print(std::ostream& o) const;

		/// @brief Return in string form
		std::string str() const {std::ostringstream s; print(s); return s.str();}

	private:
		NUMTYPE m_value;
		std::make_unsigned_t<NUMTYPE> m_base;
		int m_zero_pad;
		const char* m_conv_set;
};

/// Print the formatted value on a ostream
template<class NUMTYPE>
void fmt<NUMTYPE>::print(std::ostream& o) const
{
	char buf[buf_size];
	char* buf_start = buf;

	// Always unsigned number
	std::make_unsigned_t<NUMTYPE> num;

	// Calculate the zero_pad information
	unsigned zero_pad;
	char pad = (m_zero_pad < 0 ? ' ' : '0');
	zero_pad = (m_zero_pad < 0 ? -m_zero_pad : m_zero_pad);

	// True if the number is negative. If this flag is set, the
	// number will be printed with a "-" in front.
	if constexpr (std::is_signed<NUMTYPE>::value) {
		if (m_value < 0) {
			num = -m_value;
			buf_start[0] = '-';
			buf_start++;
			if (zero_pad > 0) zero_pad--;
		}
		else {num = m_value;}
	}
	else {num = m_value;}

	if (num == 0) {
		if (zero_pad > 0) {
			memset(buf_start, pad, zero_pad);
			buf_start[zero_pad] = '\0';
			buf_start[zero_pad-1] = '0';
		}
		else {
			buf_start[0] = '0';
			buf_start[1] = '\0';
		}
		o << buf;
		return;
	}

	char* p = buf_start;
	while (num > 0) {
		(*p) = m_conv_set[num % m_base];
		num /= m_base;
		p++;
	}

	while ((int)(p-buf_start) < (int)zero_pad) {
		(*p) = pad;
		p++;
	}

	(*p) = '\0';
	reverse_string(buf_start);
	o << buf;
}

// Stream on a std::ostream
template<class NUMTYPE>
std::ostream& operator<<(std::ostream& o, const fmt<NUMTYPE>& n) {n.print(o); return o;}

// Stream on a sink_ch32
template<class NUMTYPE>
sink_ch32& operator<<(sink_ch32& sink, const fmt<NUMTYPE>& s) {fmt32utf8<const fmt<NUMTYPE>&> fm(s); fm.write_to_sink(sink); return sink;}


} // namespace dastd


