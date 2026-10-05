/**
 * @file CTimer.cpp
 * @author Jinhee.Lee (tjrgl@naver.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "CTimer.h"

namespace sys
{
    CTimer::CTimer()
        : mutex_initialized_(false),
        condition_initialized_(false),
        thread_created_(false),
        running_(false),
        stop_requested_(false),
        period_ms_(0U),
        callback_()
    {
        int32_t result;

        result = static_cast<int32_t>(
            pthread_mutex_init(&mutex_, 0));

        if (result == 0)
        {
            mutex_initialized_ = true;
        }
        else
        {
            mutex_initialized_ = false;
        }

        if (mutex_initialized_)
        {
            result = static_cast<int32_t>(
                pthread_cond_init(&condition_, 0));

            if (result == 0)
            {
                condition_initialized_ = true;
            }
            else
            {
                condition_initialized_ = false;
            }
        }
    }

    CTimer::~CTimer()
    {
        (void)Stop();

        if (condition_initialized_)
        {
            (void)pthread_cond_destroy(&condition_);
            condition_initialized_ = false;
        }

        if (mutex_initialized_)
        {
            (void)pthread_mutex_destroy(&mutex_);
            mutex_initialized_ = false;
        }
    }

    bool CTimer::Start(
        uint32_t period_ms,
        const Callback& callback)
    {
        bool result = false;
        int32_t mutex_result;
        int32_t thread_result;

        if ((period_ms > 0U) &&
            static_cast<bool>(callback) &&
            mutex_initialized_ &&
            condition_initialized_)
        {
            mutex_result = static_cast<int32_t>(
                pthread_mutex_lock(&mutex_));

            if (mutex_result == 0)
            {
                if (!running_)
                {
                    period_ms_ = period_ms;
                    callback_ = callback;
                    stop_requested_ = false;

                    thread_result = static_cast<int32_t>(
                        pthread_create(
                            &thread_,
                            0,
                            &CTimer::ThreadEntry,
                            this));

                    if (thread_result == 0)
                    {
                        thread_created_ = true;
                        running_ = true;
                        result = true;
                    }
                }

                (void)pthread_mutex_unlock(&mutex_);
            }
        }

        return result;
    }

    bool CTimer::Stop()
    {
        bool result = false;
        bool thread_created_local;
        int32_t mutex_result;

        if (mutex_initialized_)
        {
            mutex_result = static_cast<int32_t>(
                pthread_mutex_lock(&mutex_));

            if (mutex_result == 0)
            {
                stop_requested_ = true;
                thread_created_local = thread_created_;

                (void)pthread_cond_signal(&condition_);

                (void)pthread_mutex_unlock(&mutex_);

                if (thread_created_local)
                {
                    (void)pthread_join(thread_, 0);
                    thread_created_ = false;
                }

                mutex_result = static_cast<int32_t>(
                    pthread_mutex_lock(&mutex_));

                if (mutex_result == 0)
                {
                    running_ = false;
                    callback_ = Callback();

                    (void)pthread_mutex_unlock(&mutex_);

                    result = true;
                }
            }
        }

        return result;
    }

    bool CTimer::IsRunning() const
    {
        return running_;
    }

    void* CTimer::ThreadEntry(void* argument)
    {
        CTimer* timer;

        timer = static_cast<CTimer*>(argument);

        if (timer != 0)
        {
            timer->ThreadFunction();
        }

        return 0;
    }

    bool CTimer::GetStopRequested()
    {
        bool result = true;
        int32_t mutex_result;

        mutex_result = static_cast<int32_t>(
            pthread_mutex_lock(&mutex_));

        if (mutex_result == 0)
        {
            result = stop_requested_;

            (void)pthread_mutex_unlock(&mutex_);
        }

        return result;
    }

    void CTimer::ThreadFunction()
    {
        struct timespec request_time;
        struct timespec remain_time;

        uint32_t period_sec;
        uint32_t period_nsec;

        int32_t sleep_result;

        period_sec = period_ms_ / 1000U;
        period_nsec = (period_ms_ % 1000U) * 1000000U;

        request_time.tv_sec =
            static_cast<time_t>(period_sec);

        request_time.tv_nsec =
            static_cast<long>(period_nsec);

        while (!GetStopRequested())
        {
            remain_time = request_time;

            do
            {
                sleep_result = static_cast<int32_t>(
                    nanosleep(
                        &remain_time,
                        &remain_time));

            } while ((sleep_result != 0) &&
                    (errno == EINTR) &&
                    (!GetStopRequested()));

            if (!GetStopRequested())
            {
                if (callback_)
                {
                    callback_();
                }
            }
        }

        if (mutex_initialized_)
        {
            if (pthread_mutex_lock(&mutex_) == 0)
            {
                running_ = false;

                (void)pthread_mutex_unlock(&mutex_);
            }
        }
    }
} /* namespace sys  */
