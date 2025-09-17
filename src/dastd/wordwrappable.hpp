/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 13-AUG-2023
**/
#pragma once
#include "defs.hpp"
#include "string_or_vector.hpp"
#include <vector>

namespace dastd {

/// @brief Indication of the type of character that is being processed
enum class wordwrap_chartype {
	CHR_REGULAR      , ///< The character is a regular one
	CHR_REGULAR_SPLIT, ///< The character is a regular one and after this one there is a split point
	CHR_SPACE        , ///< The character is a space, i.e. it must be removed if at the beginning of a line;
	CHR_NEWLINE      , ///< The character triggers a new line
	CHR_EOF          , ///< There are no more characters
};

/// @brief Indicates a range of characters
struct wordwrap_range {
	size_t m_first=0;
	size_t m_length=0;
};

/// @brief Represent a string of some kind that can be wrapped
///
/// This class does not care of the nature of the string. Just requires
/// the string to be made of items with an absolute integral position.
class wordwrappable {
	public:
		/// @brief Destructor
		virtual ~wordwrappable() {}

		/// @brief Return the length of the string
		virtual size_t length() const = 0;

		/// @brief Return information about the character at the given position
		virtual wordwrap_chartype get_char_type(size_t pos) const = 0;

		/// @brief Execute a worwrap on the indicated wordwrappable
		void do_wordwrap(size_t columns, std::vector<wordwrap_range>& ranges) const;
};

/// @brief Represent a string of some kind that can be wrapped
///
/// This contains a standard implementation based on simple rules.
class wordwrappable_std: public wordwrappable {
	public:
		/// @brief Destructor
		virtual ~wordwrappable_std() {}

		/// @brief Return the length of the string
		virtual size_t length() const = 0;

		/// @brief Determine if the indicated character is a space
		/// @param pos Position of the character to be checked (always < length())
		/// @return Return true if the character at `pos` is a space
		virtual bool is_space(size_t pos) const = 0;

		/// @brief Determine if the indicated character is a new-line
		/// @param pos Position of the character to be checked (always < length())
		/// @return Return true if the character at `pos` is a new-line
		virtual bool is_newline(size_t pos) const = 0;

		/// @brief Determine if the position between the characters
		///        at 'pos' and 'pos+1' is a split point (both always < length()-1)
		/// @param pos Position of the character on the left
		/// @return Return true if it is a split point
		virtual bool is_split_point(size_t pos) const {return (!is_space(pos) && is_space(pos+1));}

		/// @brief Return information about the character at the given position
		virtual wordwrap_chartype get_char_type(size_t pos) const;
};

/// @brief Represent a string of some kind that can be wrapped
///
/// This contains a standard implementation based strings
template<class CHARTYPE>
class wordwrappable_string: public wordwrappable_std {
	public:
		/// @brief Constructor
		wordwrappable_string(const string_or_vector<CHARTYPE>& string): m_string(string) {}

		/// @brief Return the length of the string
		virtual size_t length() const override {return m_string.length();}

		/// @brief Determine if the indicated character is a space
		/// @param pos Position of the character to be checked (always < length())
		/// @return Return true if the character at `pos` is a space
		virtual bool is_space(size_t pos) const override  {return (m_string[pos] == ' ') || (m_string[pos] == '\t');}

		/// @brief Determine if the indicated character is a new-line
		/// @param pos Position of the character to be checked (always < length())
		/// @return Return true if the character at `pos` is a new-line
		virtual bool is_newline(size_t pos) const override {return m_string[pos] == '\n';}

		/// @brief Return the string
		const string_or_vector<CHARTYPE>& str() const {return m_string;}

	protected:
		/// @brief Associated string
		const string_or_vector<CHARTYPE> m_string;
};

/// @brief Returns a word-wrapped string
/// @tparam CHARTYPE Type of the string (char, char32_t, etc.)
/// @param source    String to be wordwrapped
/// @param columns   Number of columns
/// @return          The wordwrapped string
template<class CHARTYPE>
string_or_vector<CHARTYPE> wordwrap_string(const string_or_vector<CHARTYPE>& source, size_t columns)
{
	wordwrappable_string<CHARTYPE> wws(source);
	std::vector<wordwrap_range> ranges;
	wws.do_wordwrap(columns, ranges);
	string_or_vector<CHARTYPE> wrapped_string;

	bool is_next = false;
	for (auto & range: ranges) {
		if (is_next) wrapped_string.append(1, '\n');
		is_next=true;
		wrapped_string.append(source, range.m_first, range.m_length);
	}
	return wrapped_string;
}

//------------------------------------------------------------------------------
// Execute a worwrap on the indicated wordwrappable
//------------------------------------------------------------------------------
inline void wordwrappable::do_wordwrap(size_t columns, std::vector<wordwrap_range>& ranges) const
{
	size_t pos=0;
	unsigned state=2;
	size_t start=0, end=0;
	bool continue_looping = true;

	for(pos=0; continue_looping; pos++) {
		wordwrap_chartype ct = (pos < length() ? get_char_type(pos) : wordwrap_chartype::CHR_EOF);

		switch(state) {
			//----------------------------------------------------------------
			// BEGINNING OF A LINE
			//
			// Needs to skip the leading spaces
			//----------------------------------------------------------------
			case 1: {
				switch(ct) {
					// The character is a regular one: add it to the buffer
					case wordwrap_chartype::CHR_REGULAR_SPLIT:
					case wordwrap_chartype::CHR_REGULAR: {
						start = pos;
						end = pos;
						state = 2;
						break;
					}

					// The character is a space, i.e. it must be removed if at the beginning of a line;
					case wordwrap_chartype::CHR_SPACE: {
						break;
					}

					// The character triggers a new line
					// Do not count it and trigger a flush
					case wordwrap_chartype::CHR_NEWLINE: {
						wordwrap_range& range = ranges.emplace_back();
						range.m_first = pos;
						range.m_length = 0;
						break;
					}

					// There are no more characters
					case wordwrap_chartype::CHR_EOF: {
						continue_looping = false;
						break;
					}

					default: assert(0);
				}
				break;
			}
			//----------------------------------------------------------------
			// CONTINUATION OF A LINE
			//----------------------------------------------------------------
			case 2: {
				switch(ct) {
					// The character is a regular one: add it to the buffer
					case wordwrap_chartype::CHR_SPACE:
					case wordwrap_chartype::CHR_REGULAR: {
						if (pos-start >= columns) {
							wordwrap_range& range = ranges.emplace_back();
							range.m_first = start;

							// If no split points have been found, truncate the line
							// If there is split point: take it
							range.m_length = ((end > start) ? (end-start+1) : columns);
							pos = start+range.m_length-1;
							state = 1;
						}
						break;
					}

					// The character is a regular one and after this one there is a split point.
					// Add the character and trigger a buffer flush.
					case wordwrap_chartype::CHR_REGULAR_SPLIT: {
						if (pos-start < columns) end = pos;
						break;
					}

					// The character triggers a new line
					// Do not count it and trigger a flush
					case wordwrap_chartype::CHR_NEWLINE: {
						wordwrap_range& range = ranges.emplace_back();
						range.m_first = start;
						range.m_length = pos-start;
						state = 1;
						break;
					}

					// There are no more characters
					case wordwrap_chartype::CHR_EOF: {
						wordwrap_range& range = ranges.emplace_back();
						range.m_first = start;
						range.m_length = pos-start;
						continue_looping=false;
						break;
					}

					default: assert(0);
				}
				break;
			}
		}
	}
}

//------------------------------------------------------------------------------
// (brief) Return information about the character at the given position
//------------------------------------------------------------------------------
inline wordwrap_chartype wordwrappable_std::get_char_type(size_t pos) const
{
	assert(pos < length());
	wordwrap_chartype chartype;
	if (is_space(pos)) {
		chartype = wordwrap_chartype::CHR_SPACE;
	}
	else if (is_newline(pos)) {
		chartype = wordwrap_chartype::CHR_NEWLINE;
	}
	else if (pos+1 < length()) {
		chartype = (is_split_point(pos) ? wordwrap_chartype::CHR_REGULAR_SPLIT : wordwrap_chartype::CHR_REGULAR);
	}
	else {
		chartype = wordwrap_chartype::CHR_REGULAR;
	}
	return chartype;
}

} // namespace dastd
