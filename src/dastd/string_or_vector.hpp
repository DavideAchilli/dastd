/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 02-MAY-2024
*
**/
#pragma once
#include "defs.hpp"
#include <string>
#include <vector>

namespace dastd {

/// @brief Becomes a `std::basic_string` or a `std::vector` according to CHARTYPE
/// @tparam CHARTYPE Type of each character of the string
///
/// It allows defining a string whose characters can either be regular characters
/// like "char" or "char32_t", or structures containing additional information.
/// In the first case, they will be implemented as a regular std::basic_string (i.e.
/// as a `std::string` if CHARTYPE is `char`).
/// In case of structures, it will be implemented as a `std::vector`.
template<class CHARTYPE>
using string_or_vector = typename std::conditional_t<is_char_v<CHARTYPE>, std::basic_string<CHARTYPE>, std::vector<CHARTYPE>>;


} // namespace dastd
