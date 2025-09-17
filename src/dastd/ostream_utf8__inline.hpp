/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 04-AUG-2023
**/
#if defined INCLUDE_dastd_ostream_utf8_inline && !defined dastd_ostream_utf8_inline
#define dastd_ostream_utf8_inline

namespace dastd {

// Write one character to the target stream
inline void ostream_utf8::write_char(char_type c)
{
	if (m_utf8seq_left == 0) {
		m_utf8seq_left = count_utf8_following_chars(c);
		if (m_utf8seq_left) m_utf8seq_left++;
	}

	if (m_utf8seq_left) {
		m_utf8seq[m_utf8seq_len] = c;
		m_utf8seq_len++;
		m_utf8seq_left--;

		if (m_utf8seq_left == 0) {
			assert(m_utf8seq_len < UTF8_CHAR_MAX_LEN);
			m_utf8seq[m_utf8seq_len] = '\0';

			char32_t code_point = 0;
			read_utf8_asciiz(m_utf8seq, code_point);
			m_target.sink_write(code_point);
			m_utf8seq_len = 0;
			return;
		}
	}
	else {
		m_target.sink_write((char32_t)c);
	}
}


} // namespace dastd

#endif
