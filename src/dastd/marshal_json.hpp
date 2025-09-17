/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 23-AUG-2023
**/
/*!
	@page marshaling_json_format Marshaling JSON encoding

**/
#pragma once
#include <iostream>
namespace dastd {
	/// @brief Type of element
	enum class marshal_json_element_type {
		STRUCT, FIELD, ARRAY, ARRAY_ELEMENT, DICTIONARY, DICTIONARY_ELEMENT, TYPED
	};

	inline std::ostream& operator<<(std::ostream& o, marshal_json_element_type v) {
		switch(v) {
			case marshal_json_element_type::STRUCT: o << "STRUCT"; break;
			case marshal_json_element_type::FIELD: o << "FIELD"; break;
			case marshal_json_element_type::ARRAY: o << "ARRAY"; break;
			case marshal_json_element_type::ARRAY_ELEMENT: o << "ARRAY_ELEMENT"; break;
			case marshal_json_element_type::DICTIONARY: o << "DICTIONARY"; break;
			case marshal_json_element_type::DICTIONARY_ELEMENT: o << "DICTIONARY_ELEMENT"; break;
			case marshal_json_element_type::TYPED: o << "TYPED"; break;
			default: o << "UNKNOWN(" << (uint32_t)v << ")";
		}
		return o;
	}

	/// @brief Defines how polymorphic objects are encoded
	///
	/// JSON does not support a standard native way of encoding polymorphic objects, i.e.
	/// structures whose fields depend on their type which can be one among a fixed kwonw set.
	/// The DASTD marshal JSON library supports the following ways of encoding them.
	enum class marshal_json_polymorphic_encoding {
		/// @brief Encode the type name as a field name
		///
		/// @note When using this type of encoding, inside a `encode_typed_begin/encode_typed_end` call pair,
		/// the encoder can encode any type (structures, arrays or any terminal).
		///
		/// Example: the type is "MyType1". The encoding would be:
		///
		///    {
		///       "MyType1":{...contents...}
		///    }
		///
		TYPEID_AS_FIELD_NAME,

		/// @brief Encode the type name as the value of a custom field that is
		///        always the first one.
		///
		/// @note When using this type of encoding, inside a `encode_typed_begin/encode_typed_end` call pair,
		/// the encoder can encode only a struct. This means that a `encode_typed_begin` must be immediately
		/// followed by a `encode_struct_begin`.
		///
		/// Example: the type is "MyType1" and the custom field name is "$type". The encoding would be:
		///
		///    {
		///       "$type":"MyType1",
		///       ...the fields of MyType1...
		///    }
		TYPEID_AS_STRUCT_FIELD,
	};

	inline std::ostream& operator<<(std::ostream& o, marshal_json_polymorphic_encoding v) {
		switch(v) {
			case marshal_json_polymorphic_encoding::TYPEID_AS_FIELD_NAME: o << "TYPEID_AS_FIELD_NAME"; break;
			case marshal_json_polymorphic_encoding::TYPEID_AS_STRUCT_FIELD: o << "TYPEID_AS_STRUCT_FIELD"; break;
			default: o << "UNKNOWN(" << (uint32_t)v << ")";
		}
		return o;
	}
}
