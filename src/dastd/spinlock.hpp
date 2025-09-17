/**
* @author Davide Achilli
* @copyright Apache 2.0 License
* @date 04-AUG-2023
*

**/
#pragma once
#include "defs.hpp"
#include <atomic>
/*
* Define the DASTD_KSPINLOCK_DEBUG macro to enable the "is_locked_by_this_thread()" method.
* This method can be used to detect wether the semaphore is locked by the
* current thread, for example in assertions.
*/
#ifdef DASTD_KSPINLOCK_DEBUG
#include <thread>
#include <cassert>
#define DASTD_KSPINLOCK_ASSERT_IS_NOT_LOCKED_BY_THIS_THREAD(spinlock) assert(!spinlock.is_locked_by_this_thread())
#define DASTD_KSPINLOCK_ASSERT_IS_LOCKED_BY_THIS_THREAD(spinlock) assert(spinlock.is_locked_by_this_thread())
#else
#define DASTD_KSPINLOCK_ASSERT_IS_NOT_LOCKED_BY_THIS_THREAD(spinlock)
#define DASTD_KSPINLOCK_ASSERT_IS_LOCKED_BY_THIS_THREAD(spinlock)
#endif

namespace dastd
{

/// @brief Fast user-space spinlock.
///
/// This mutex semaphore works in user-space and it is very
/// fast in case of no conflicts. Since it busy-waits until
/// the lock is acquired, it is suited to protect very
/// fast operations in a multi-cored environment.
class spinlock {
public:
	/// @brief Request exclusive access to the resource. Busy-waits until acquired.
	void lock() noexcept {
		#ifdef DASTD_KSPINLOCK_DEBUG
		// This lock does not support nested locks
		assert(!is_locked_by_this_thread());
		#endif
		while (m_flag.test_and_set(std::memory_order_acquire));
		#ifdef DASTD_KSPINLOCK_DEBUG
		m_locking_thread_id = std::this_thread::get_id();
		#endif
	}

	/// @brief Release the exclusive access to the resource
	void unlock() noexcept {
		#ifdef DASTD_KSPINLOCK_DEBUG
		// Only the owner thread is supposed to release the lock.
		assert(is_locked_by_this_thread());
		m_locking_thread_id = std::thread::id();
		#endif
		m_flag.clear(std::memory_order_release);
	}

	#ifdef DASTD_KSPINLOCK_DEBUG
	/// @brief Returns true if locked by this thread.
	///
	/// Available only if DASTD_KSPINLOCK_DEBUG is defined
	bool is_locked_by_this_thread() const {return m_locking_thread_id == std::this_thread::get_id();}
	#endif

private:
	/// @brief Internal atomic m_flag
	///
	/// The "std::atomic_flag" object is guaranteed by C++11 to be
	/// lock-free.
	std::atomic_flag m_flag = ATOMIC_FLAG_INIT;

	#ifdef DASTD_KSPINLOCK_DEBUG
	std::thread::id m_locking_thread_id;
	#endif
};

}
