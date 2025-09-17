/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 21-AUG-2025
*
**/
#pragma once
#include "defs.hpp"
#include "sysrecog.hpp"
#include <cstdint>

namespace dastd {

#ifdef DASTD_LINUX
#include <pthread.h>
#include <sched.h>
#endif
#ifdef DASTD_WINANY
#define VC_EXTRALEAN
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

/// @brief Set the current thread affinity to the indicated core
/// @param core_id The core number, starting from 0
/// @return Returns `true` if ok, `false` in case of failure
inline bool pin_thread_to_core(uint32_t core_id) {
	#ifdef DASTD_LINUX
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(core_id, &cpuset);

	pthread_t current_thread = pthread_self();
	return (pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset) == 0);
	#elif defined DASTD_WINANY
    HANDLE hThread = GetCurrentThread();
    DWORD_PTR result = SetThreadAffinityMask(hThread, ((DWORD_PTR)1) << core_id);
		return (result != 0);
	#else
		return false;
	#endif
}

/// @brief Return the current
/// @return 
inline uint32_t get_current_core_id()
{
	#ifdef DASTD_LINUX
		return (unsigned)sched_getcpu();
	#elif defined DASTD_WINANY
    return GetCurrentProcessorNumber();
	#else
		return 0;
	#endif
}

} // namespace dastd
