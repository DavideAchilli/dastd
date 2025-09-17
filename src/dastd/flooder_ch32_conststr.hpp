/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 11-AUG-2023
**/
#pragma once
#include "flooder_ch32.hpp"
#include "sink_ch32.hpp"

namespace dastd {

	/// @brief Class that handles as a source a constant string
	/// @tparam CHARTYPE Type of the character of the constant string
	template<class CHARTYPE>
	class flooder_ch32_conststr: public flooder_ch32 {
		private:
			static constexpr size_t ZERO_TERMINATED = (size_t)(-1);
		public:
			/// @brief Constructor
			flooder_ch32_conststr(const CHARTYPE* str, size_t len=ZERO_TERMINATED): m_str(str), m_len(len) {}

			/// @brief Write on a sink_ch32
			/// @param sink Target `sink_ch32`
			virtual void write_to_sink(sink_ch32& sink) const override {if (m_len==ZERO_TERMINATED) sink.sink_write(m_str); else sink.sink_write(m_str, m_len);}

		private:
			/// @brief Constant string
			const CHARTYPE* m_str;

			/// @brief Length or ZERO_TERMINATED
			size_t m_len;
	};

} // namespace dastd
