/**
 * @file CSpinlock.cpp
 * @author Jinhee.Lee (tjrgl@naver.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "CSpinlock.h"

namespace sys
{
    CSpinlock::CSpinlock() noexcept
    : m_flag(ATOMIC_FLAG_INIT)
    {
    }

    CSpinlock::~CSpinlock() noexcept
    {
    }

    void CSpinlock::lock() noexcept
    {
        while(m_flag.test_and_set(std::memory_order_acquire))
        {      

        }
    }

    void CSpinlock::unlock() noexcept
    {
        m_flag.clear(std::memory_order_release);
    }

    bool CSpinlock::try_lock() noexcept
    {
        return !m_flag.test_and_set(std::memory_order_acquire);
    }
} /* namespace sys */
