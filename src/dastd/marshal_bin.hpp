/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 23-AUG-2023
**/
/*!
	@page marshaling_bin_format Marshaling binary encoding

	The binary marshaling encoding uses a compact binary encoding form based on little-endian
	values.

	ENCODING RULES
	==============
	This chapter describes how complex data like structures, arrays and polymorphic
	objects, is encoded.

	Size indicator
	--------------
	The complex types like structures, arrays and polymorphic objects might need
	a size indicator encoded at the beginning. This value is encoded as a
	fixed size u32 value.

	Structures
	----------
	Structures are encoded by writing the single fields one after the other in the
	order they are presented, without any indication about the field name.
	When decoding, the decoder will present the field-ids by using the
	user provided `field_ids` parameter.

	In case the structure is declared `extensible=true`, it will encode a size
	indicator telling how many bytes are following before the structure.

	Arrays
	------
	Arrays are encoded by writing the size indicator telling the number of elements
	followed by the elements.

	Typed objects
	-------------
	Typed objects are encoded by writing the type as a u32 value followed by the
	data. If the typed object is saved as `extensible=true`, it will encode
	the type as u32, followed by the length in bytes (u32) and the encoded data.


	@link marshaling_main Marshaling page @endlink
	@see dastd::marshal_dec_bin
	@see dastd::marshal_enc_bin

**/
#pragma once
#include <iostream>
namespace dastd {
	/// @brief Type of element
	enum class marshal_bin_element_type {
		STRUCT, FIELD, FIELD_MISSING, ARRAY, ARRAY_ELEMENT, DICTIONARY, DICTIONARY_ELEMENT, TYPED
	};

	inline std::ostream& operator<<(std::ostream& o, marshal_bin_element_type v) {
		switch(v) {
			case marshal_bin_element_type::STRUCT: o << "STRUCT"; break;
			case marshal_bin_element_type::FIELD: o << "FIELD"; break;
			case marshal_bin_element_type::FIELD_MISSING: o << "FIELD_MISSING"; break;
			case marshal_bin_element_type::ARRAY: o << "ARRAY"; break;
			case marshal_bin_element_type::ARRAY_ELEMENT: o << "ARRAY_ELEMENT"; break;
			case marshal_bin_element_type::DICTIONARY: o << "DICTIONARY"; break;
			case marshal_bin_element_type::DICTIONARY_ELEMENT: o << "DICTIONARY_ELEMENT"; break;
			case marshal_bin_element_type::TYPED: o << "TYPED"; break;
			default: o << "UNKNOWN(" << (uint32_t)v << ")";
		}
		return o;
	}
}
