/**
 * @file CRingBuffer.h
 * @author Jinhee.Lee (tjrgl@naver.com)
 * @brief 
 * @version 0.1
 * @date 2025-09-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __EWLIB_CONTAINER_CRINGBUFFER_H__
#define __EWLIB_CONTAINER_CRINGBUFFER_H__

#include "ewlib/stdC++.h"

namespace sys
{
    template <typename T, size_t Capacity>
    class CRingBuffer 
    {
    public:
        CRingBuffer() = default;
        virtual ~CRingBuffer() = default;
        CRingBuffer(const CRingBuffer&) = delete;
        CRingBuffer& operator=(const CRingBuffer&) = delete;

        bool push(T item) 
        {
            bool ret = true;
            const size_t current_tail = m_tail.load(std::memory_order_relaxed);
            const size_t next_tail = (current_tail + 1) % BufferSize;

            if (next_tail == m_head.load(std::memory_order_acquire)) 
            {
                ret = false;
            }
            else
            {
                m_buffer[current_tail] = std::move(item);
                m_tail.store(next_tail, std::memory_order_release);
            }
            return ret;
        }

        bool push() 
        {
            bool ret = true;
            const size_t current_tail = m_tail.load(std::memory_order_relaxed);
            const size_t next_tail = (current_tail + 1) % BufferSize;

            if (next_tail == m_head.load(std::memory_order_acquire)) 
            {
                ret =  false; // Queue Full
            }
            else
            {
                m_tail.store(next_tail, std::memory_order_release);
            }
            return ret;
        }

        T* front() 
        {
            const size_t current_head = m_head.load(std::memory_order_relaxed);

            if (current_head == m_tail.load(std::memory_order_acquire)) 
            {
                return nullptr;
            }
            return &m_buffer[current_head];
        }

        bool pop(T& value) 
        {
            bool ret = true;
            const size_t current_head = m_head.load(std::memory_order_relaxed);

            if (current_head == m_tail.load(std::memory_order_acquire)) 
            {
                ret = false; // Queue Empty
            }
            else
            {
                value = std::move(m_buffer[current_head]);
                m_head.store((current_head + 1) % BufferSize, std::memory_order_release);
            }
            return ret;
        }

        bool empty() const 
        {
            return m_head.load(std::memory_order_relaxed) == m_tail.load(std::memory_order_relaxed);
        }

        [[nodiscard]] size_t size() const noexcept 
        {
            if (m_tail >= m_head) {
                return m_tail - m_head;
            }
            return BufferSize - (m_head - m_tail);
        }

        constexpr size_t getCapacity() const 
        {
            return Capacity;
        }

    private:
        static constexpr size_t BufferSize = Capacity + 1;
        std::array<T, Capacity + 1> m_buffer;
        std::atomic<size_t> m_head{0};
        std::atomic<size_t> m_tail{0};
    }; /* class CRingBuffer */
} /* namespace sys */
#endif /* __EWLIB_CONTAINER_CRINGBUFFER_H__ */

