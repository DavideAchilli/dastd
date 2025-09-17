/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 11-AUG-2023
**/
#pragma once
#include "flooder_ch32.hpp"

namespace dastd {
	class sink_ch32;

	/// @brief Flooder that contains a shared_ptr to another flooder
	class flooder_ch32_relay: public flooder_ch32 {
		private:
			/// @brief Pointer to the target flooder
			std::shared_ptr<flooder_ch32> m_target_flooder;

		public:
			/// @brief Constructor
			flooder_ch32_relay() {}

			/// @brief Constructor
			flooder_ch32_relay(std::shared_ptr<flooder_ch32> target_flooder): m_target_flooder(target_flooder) {}

			/// @brief Copy constructor
			flooder_ch32_relay(const flooder_ch32_relay& o): m_target_flooder(o.m_target_flooder) {}

			/// @brief Operator=
			flooder_ch32_relay& operator=(const flooder_ch32_relay& o) {m_target_flooder = o.m_target_flooder; return *this;}

			/// @brief Operator= assigning anoter shared_ptr
			flooder_ch32_relay& operator=(std::shared_ptr<flooder_ch32> target_flooder) {m_target_flooder = target_flooder; return *this;}

			/// @brief Returns true if the target flooder is valid
			bool valid() const {return m_target_flooder != nullptr;}
			operator bool() const {return valid();}

			/// @brief Write on a sink_ch32
			/// @param sink Target `sink_ch32`
			virtual void write_to_sink(sink_ch32& sink) const override {if (m_target_flooder) m_target_flooder->write_to_sink(sink);}
	};

} // namespace dastd
