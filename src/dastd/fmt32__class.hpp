/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 11-AUG-2023
*
* @brief   This modules implements a set of formatters that can be used to print
*          on dastd::sink_ch32 objects.
*          Temporary instances of these objects take a source object and print
*          it using a given format.
*
**/
#if defined INCLUDE_dastd_fmt32_class && !defined dastd_fmt32_class
#define dastd_fmt32_class

namespace dastd {
	/// @brief Considers an object as an UTF-8 source
	///
	/// Takes any object that can be written on a `std::ostream` and
	/// uses it to extract a sequence of UTF-8 characters that are converted
	/// in char32_t and written to the `sink_ch32`.
	class fmt32utf8_base: public flooder_ch32 {
		public:
			/// @brief Destructor
			virtual ~fmt32utf8_base() {}

			/// @brief Print on a sink_ch32
			void write_to_sink(sink_ch32& sink) const;

		protected:
			/// @brief Redefine this method and write the associated data on `o`
			virtual void write(std::ostream& o) const = 0;
	};

	/// @brief Considers an object as an UTF-8 source
	///
	/// Takes any object that has a << operator with a `std::ostream` and
	/// uses it to extract a sequence of UTF-8 characters that are converted
	/// in char32_t and written to the `sink_ch32`.
	template<class SRC>
	class fmt32utf8: public fmt32utf8_base {
		public:
			/// @brief Constructor
			fmt32utf8(SRC src): m_src(src) {}

			/// @brief Copy constructor
			fmt32utf8(const fmt32utf8& o): m_src(o.m_src) {}

		protected:
			/// @brief Redefine this method and write the associated data on `o`
			virtual void write(std::ostream& o) const {o << m_src;}

		private:
			/// @brief Source of characters
			SRC m_src;
	};

	#define DASTD_DEF_OSTREAM_TO_SINK(type) inline sink_ch32& operator<<(sink_ch32& sink, const type& s) {fmt32utf8<const type&> fm(s); fm.write_to_sink(sink); return sink;}


} // namespace dastd

#endif
