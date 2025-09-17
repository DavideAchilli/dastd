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
#pragma once
#include "defs.hpp"
#include "ostream_utf8__class.hpp"
#include "flooder_ch32.hpp"
#include "ostream_basic.hpp"
#include "utf8.hpp"
#include "sink_ch32.hpp"

/*
* NOTICE: this include is the same for fmt32.hpp, ostream_utf8.hpp and sink_ch32.hpp
* because they have circular references in their implementation
*/

#define INCLUDE_dastd_fmt32_class
#include "fmt32__class.hpp"
#undef INCLUDE_dastd_fmt32_class

#define INCLUDE_dastd_ostream_utf8_class
#include "ostream_utf8__class.hpp"
#undef INCLUDE_dastd_ostream_utf8_class

#define INCLUDE_dastd_sink_ch32_class
#include "sink_ch32__class.hpp"
#undef INCLUDE_dastd_sink_ch32_class

#define INCLUDE_dastd_fmt32_inline
#include "fmt32__inline.hpp"
#undef INCLUDE_dastd_fmt32_inline

#define INCLUDE_dastd_ostream_utf8_inline
#include "ostream_utf8__inline.hpp"
#undef INCLUDE_dastd_ostream_utf8_inline

#define INCLUDE_dastd_sink_ch32_inline
#include "sink_ch32__inline.hpp"
#undef INCLUDE_dastd_sink_ch32_inline

