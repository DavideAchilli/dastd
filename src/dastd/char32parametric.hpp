/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 11-AUG-2023
**/
#pragma once
#include "container.hpp"
#include <map>
#include "char32string.hpp"

namespace dastd {

#ifndef DASTD_CHAR32PARAMETRIC_MAX_NESTING
#define DASTD_CHAR32PARAMETRIC_MAX_NESTING 16
#endif // DASTD_CHAR32PARAMETRIC_MAX_NESTING

// Unicode characters that can be used to represent parameters in
// a unicode string.
#define DASTD_UNICODE_PARAMETER_BASE 0xF800
#define DASTD_UNICODE_PARAMETER_MAX  0xF8FF
#define DASTD_UNICODE_PARAMETER_COUNT ((DASTD_UNICODE_PARAMETER_MAX)-(DASTD_UNICODE_PARAMETER_BASE)+1)

// These macros can be used to easely create static strings.
// For example: const char32_t* text = U"Your name is " DASTD_P0 " and my is " DASTD_P1;
#define DASTD_P0 U"\uF800"
#define DASTD_P1 U"\uF801"
#define DASTD_P2 U"\uF802"
#define DASTD_P3 U"\uF803"
#define DASTD_P4 U"\uF804"
#define DASTD_P5 U"\uF805"
#define DASTD_P6 U"\uF806"
#define DASTD_P7 U"\uF807"


/// @brief char32 string that supports internal parameter substitutions
///
/// This class manages a char32 string, represented by a flooder_ch32, that
/// contains custom UNICODE characters that act as markers for parameters.
/// This object acts as a flooder_ch32 itself, replacing the markers with the
/// output of the related additional flooder_ch32 objects.
class char32parametric: public flooder_ch32 {
	public:
		/// @brief Empty constructor
		char32parametric() {}

		/// @brief Constructor starting from a string
		char32parametric(const flooder_ch32& root) {root.write_to_sink(m_root);}

		/// @brief Constructor starting from a string
		char32parametric(const flooder_ch32* root) {root->write_to_sink(m_root);}

		/// @brief Copy constructor
		char32parametric(const char32parametric& o) {operator=(o);}

		/// @brief Operator=
		char32parametric& operator=(const char32parametric& o);

		/// @brief Destructor
		virtual ~char32parametric() {clear();}

		/// @brief Write on the indicate sink_ch32
		virtual void write_to_sink(sink_ch32& sink) const override;

		/// @brief Delete all contents, leaving the object empty
		void clear();

		/// @brief Returns the reference code for the indicated parameter
		static char32_t param(uint32_t n) {assert(n<DASTD_UNICODE_PARAMETER_COUNT); return DASTD_UNICODE_PARAMETER_BASE+n;}

		/// @brief Set the n-th parameter
		///
		/// @param n Parameter number
		/// @param param_src char32 source for the parameter
		void set_param(uint32_t n, const flooder_ch32& param_src) {m_params.emplace(n, param_src);}

		/// @brief Set the n-th parameter
		///
		/// @param n Parameter number
		/// @return Returns a writable sink
		sink_ch32& set_param(uint32_t n);

	private:
		/// @brief Master string
		char32string m_root;

		/// @brief Map that contains the parameter number mapped to the related
		///        flooder_ch32.
		std::map<uint32_t, char32string> m_params;

		friend class char32parametric_sink;
};

/// @brief Delete all contents, leaving the object empty
inline void char32parametric::clear()
{
	m_root.clear();
	m_params.clear();
}


// Operator=
inline char32parametric& char32parametric::operator=(const char32parametric& o)
{
	if (this == &o) return *this;
	clear();
	m_root = o.m_root;
	m_params = o.m_params;

	return *this;
}

// Delegated sink
class char32parametric_sink: public sink_ch32 {
	public:
		// Constructor
		char32parametric_sink(const char32parametric& parametric, sink_ch32& sink, uint32_t nesting=0):
			m_parametric(parametric), m_sink(sink), m_nesting(nesting) {}

	protected:
		// Write one character
		virtual void internal_sink_write_char32(char32_t code_point) override;

	private:
		const char32parametric& m_parametric;
		sink_ch32& m_sink;
		uint32_t m_nesting;
};

// Write one character
inline void char32parametric_sink::internal_sink_write_char32(char32_t code_point)
{
	// If the character is a parameter placeholder, replace
	// the place holder (if existing)
	if ((code_point >= DASTD_UNICODE_PARAMETER_BASE) && (code_point <= DASTD_UNICODE_PARAMETER_MAX)) {
		uint32_t place_holder_pos = (uint32_t)(code_point - DASTD_UNICODE_PARAMETER_BASE);
		if (m_nesting < DASTD_CHAR32PARAMETRIC_MAX_NESTING) {
			auto iter = m_parametric.m_params.find(place_holder_pos);
			if (iter != m_parametric.m_params.end()) {
				char32parametric_sink nested_sink(m_parametric, m_sink, m_nesting+1);
				iter->second.write_to_sink(nested_sink);
			}
			else {
				m_sink << "?";
			}
		}
	}
	else {
		m_sink.sink_write(code_point);
	}
}

// Write on the indicate sink_ch32
inline void char32parametric::write_to_sink(sink_ch32& sink) const
{
	char32parametric_sink out_sink(*this, sink);
	m_root.write_to_sink(out_sink);
}

/// Set the n-th parameter
inline sink_ch32& char32parametric::set_param(uint32_t n)
{
	return m_params.emplace(n, "").first->second;
}

} // namespace dastd
