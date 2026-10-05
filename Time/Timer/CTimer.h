/**
 * @file CTimer.h
 * @author Jinhee.Lee (tjrgl@naver.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __EWLIB_TIME_TIMER_CTIMER_H__
#define __EWLIB_TIME_TIMER_CTIMER_H__

#include "ewlib/stdC++.h"

namespace sys
{
    class CTimer
    {
    public:
        using Callback = std::function<void(void)>;

        CTimer();
        ~CTimer();

        bool Start(uint32_t period_ms, const Callback& callback);
        bool Stop();

        bool IsRunning() const;

    private:
        CTimer(const CTimer&);
        CTimer& operator=(const CTimer&);

        static void* ThreadEntry(void* argument);

        void ThreadFunction();

        bool GetStopRequested();

        pthread_t thread_;
        pthread_mutex_t mutex_;
        pthread_cond_t condition_;

        bool mutex_initialized_;
        bool condition_initialized_;
        bool thread_created_;

        bool running_;
        bool stop_requested_;

        uint32_t period_ms_;

        Callback callback_;
    }; /* class CTimer */
} /* namespace sys */
#endif /* __EWLIB_TIME_TIMER_CTIMER_H__ */
