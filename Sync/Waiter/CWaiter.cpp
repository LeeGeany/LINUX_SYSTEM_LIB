/**
 * @file CWaiter.cpp
 * @author Jinhee.Lee (tjrgl@naver.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "CWaiter.h"

namespace sys
{
    CWaiter::CWaiter() noexcept 
    : m_is_released(false) {}
    CWaiter::~CWaiter() noexcept {}

    void CWaiter::wait()
    {
        std::unique_lock<std::mutex> lock(m_mtx);
        m_cv.wait(lock, [this]() noexcept { return m_is_released; });
        m_is_released = false; // 재사용 가능
    }

//    bool CWaiter::wait_for(std::chrono::milliseconds timeout)
//    {
//        std::unique_lock<std::mutex> lock(m_mtx);
//        const bool signaled = m_cv.wait_for(lock, timeout, [&]{return m_is_released;});
//
//        if (signaled)
//        {
//            m_is_released = false;
//        }
//        return signaled;
//    }

    void CWaiter::release() noexcept
    {
        {
            std::lock_guard<std::mutex> lock(m_mtx);
            m_is_released = true;
        }
        m_cv.notify_one();
    }

    void CWaiter::reset() noexcept
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_is_released = false;
    }
} /* namespace sys */
