/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 11-AUG-2023
**/
#pragma once
#include "flooder_ch32.hpp"
#include "flooder_ch32_conststr.hpp"
#include "flooder_ch32_relay.hpp"

namespace dastd {
	typedef uint32_t string_id;

	/// @brief String-id associated with a default text to be used if the default is not found
	struct string_id_with_default {
		string_id sid;
		const char32_t* default_text;
	};

	/// @brief Base class for objects that map a string_id to a flooder_ch32
	///
	/// This class is used mainly for multiple language strings. The caller indicates
	/// a `string_id` (an integral value) and retrieves a pointer to a constant
	/// flooder_ch32 or nullptr if not found.
	/// The implementation can provide with different strings according to the
	/// current language.
	/// The returned `flooder_ch32` can be fed to a `char32parametric` for parameters
	/// replacement.
	class flooder_ch32_set {
		public:
			/// @brief Virtual destructor
			virtual ~flooder_ch32_set() {}

			/// @brief Given a string_id, returns the related string
			///
			/// @param strid String-id of the desired string
			///
			/// @return Returned string; the returned pointer is 'nullptr' if the requested `strid` has not been found.
			virtual flooder_ch32_relay get(string_id strid) const = 0;

			/// @brief Returns the string_id associated string if found or the default otherwise
			///
			/// @param strid String-id of the desired string
			///
			/// @return Returned string; the returned pointer is 'nullptr' if the requested `strid` has not been found.
			flooder_ch32_relay get(const string_id_with_default& strid_def) const;
};

/// Returns the string_id associated string if found or the default otherwise
inline flooder_ch32_relay flooder_ch32_set::get(const string_id_with_default& strid_def) const
{
	flooder_ch32_relay ret = get(strid_def.sid);
	if (!ret) ret = std::make_unique<flooder_ch32_conststr<char32_t>>(strid_def.default_text);
	return ret;
}

} // namespace dastd
