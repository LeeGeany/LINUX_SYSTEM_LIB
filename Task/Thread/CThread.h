/**
 * @file CThread.h
 * @author Jinhee.Lee (tjrgl@naver.com)
 * @brief 
 * @version 0.1
 * @date 2025-07-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __EWLIB_TASK_CTHREAD_H__
#define __EWLIB_TASK_CTHREAD_H__

#include "ewlib/stdC++.h"

#include "IOperator.h"

namespace sys 
{
    enum class EC_THREAD_RUN_TYPE_T { ONCE_T, LOOP_T};
    enum class EC_THREAD_STATUS_T   { NEW_T, RUNNING_T, PAUSED_T, TERMINATED_T };

    class CThread : public IOperator
    {
    public:
        explicit CThread(uint32_t              _uiThreadID,
                         EC_THREAD_RUN_TYPE_T  _ecThreadRunType=EC_THREAD_RUN_TYPE_T::LOOP_T) noexcept;
        virtual ~CThread() noexcept;

    public:
        virtual bool Start() override;
        virtual bool Stop() override;
        void Pause();
        void Resume();

    public:
        uint32_t            getID()             const;
        EC_THREAD_STATUS_T  getStatus();
        std::thread &       native_handle();

    protected:
        virtual void PreOperate() override;
        virtual void Operate() override;
        virtual void PostOperate() override;

    private:
        void Runnable();

    protected:
        const uint32_t m_threadID;
        const EC_THREAD_RUN_TYPE_T m_threadType;

        std::mutex m_cvMtx;
        std::condition_variable m_cv;
        std::atomic<EC_THREAD_STATUS_T> m_status{EC_THREAD_STATUS_T::NEW_T};

        std::thread m_thread;
    }; /* class CThread */
} /* namespace sys */
#endif /* __EWLIB_TASK_CTHREAD_H__ */