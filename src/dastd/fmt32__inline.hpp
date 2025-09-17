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
#if defined INCLUDE_dastd_fmt32_inline && !defined dastd_fmt32_inline
#define dastd_fmt32_inline

namespace dastd {

//------------------------------------------------------------------------------
// (brief) Print on a sink_ch32
//------------------------------------------------------------------------------
inline void fmt32utf8_base::write_to_sink(sink_ch32& sink) const
{
	ostream_utf8 out(sink);
	write(out);
}


} // namespace dastd

#endif
