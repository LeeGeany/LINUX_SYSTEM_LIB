/**
 * @file CMsgQ_S.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-11-25
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __EWLIB_COMMUNICATION_IPC_MSGQ_CQEUEU_H__
#define __EWLIB_COMMUNICATION_IPC_MSGQ_CQEUEU_H__

#include "ewlib/stdC++.h"

namespace sys
{
    inline constexpr size_t MSG_DEFAULT_BUFFER_SIZE = 1024;

    template<typename T>
    class CQueue
    {
    public:
        explicit CQueue(size_t maxSize = 1024) noexcept
        : m_maxSize(maxSize), m_bStop(false) {}

        ~CQueue() noexcept { Stop(); }

        void Stop() noexcept
        {
            {
                std::lock_guard<std::mutex> lock(m_mtx);
                m_bStop = true;
            }
            m_cv.notify_all();
        }
        
        bool Push(const T& item)
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            if (m_bStop) return false;

            // (선택 사항) m_maxSize를 넘어가면 Push 거부 처리 추가 가능
            if (m_q.size() >= m_maxSize) return false; 

            m_q.push(item);
            m_cv.notify_one();
            return true;
        }

        bool Push(T&& item)
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            if (m_bStop || m_q.size() >= m_maxSize) return false;

            m_q.push(std::move(item));
            m_cv.notify_one();
            return true;
        }

        bool Pop(T& outItem)
        {
            std::unique_lock<std::mutex> lock(m_mtx);
            m_cv.wait(lock, [this] {
                return !m_q.empty() || m_bStop;
            });

            if (m_bStop && m_q.empty()) return false;

            outItem = std::move(m_q.front());
            m_q.pop();
            return true;
        }

    private:
        std::mutex m_mtx;
        std::condition_variable m_cv;
        std::queue<T> m_q;
        size_t m_maxSize;
        bool m_bStop;
    }; /* class CQueue */
} /* namespace sys */
#endif /* __EWLIB_COMMUNICATION_IPC_MSGQ_CQEUEU_H__ */