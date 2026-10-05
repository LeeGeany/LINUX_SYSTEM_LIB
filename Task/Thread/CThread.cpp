/**
 * @file CThread.cpp
 * @author Jinhee.Lee (tjrgl@naver.com)
 * @brief 
 * @version 0.1
 * @date 2025-07-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "CThread.h"

namespace sys 
{
    CThread::CThread(uint32_t              _uiThreadID,
                     EC_THREAD_RUN_TYPE_T  _ecThreadRunType) noexcept
    : m_threadID(_uiThreadID)
    , m_threadType(_ecThreadRunType)
    {
        
    }

    CThread::~CThread() noexcept
    {
        Stop();
    }

    bool CThread::Start()
    {
        bool ret = true;
        if (m_status != EC_THREAD_STATUS_T::NEW_T)
        {
            ret = false;
        }
        else
        {
            m_status = EC_THREAD_STATUS_T::RUNNING_T;
            m_thread = std::thread(&CThread::Runnable, this);
        }
        return ret;
    }

    bool CThread::Stop()
    {
        m_status = EC_THREAD_STATUS_T::TERMINATED_T;
        m_cv.notify_all(); // 일시정지 중일 수 있으므로 깨움

        // [Fix 1] 자기 자신 스레드가 Stop()을 불렀을 때는 join() 대신 detach() 처리
        if (std::this_thread::get_id() == m_thread.get_id())
        {
            m_thread.detach();
        }
        else
        {
            m_thread.join();
        }
        return true;
    }

    void CThread::Pause()
    {
        // [Fix 2] Atomic CAS 연산으로 원자성 보장
        EC_THREAD_STATUS_T expected = EC_THREAD_STATUS_T::RUNNING_T;
        m_status.compare_exchange_strong(expected, EC_THREAD_STATUS_T::PAUSED_T);
    }

    void CThread::Resume() 
    {
        EC_THREAD_STATUS_T expected = EC_THREAD_STATUS_T::PAUSED_T;
        if (m_status.compare_exchange_strong(expected, EC_THREAD_STATUS_T::RUNNING_T))
        {
            m_cv.notify_all();
        }
    }

    uint32_t CThread::getID() const
    {
        return m_threadID;
    }

    EC_THREAD_STATUS_T CThread::getStatus()
    {
        return m_status;
    }

    std::thread & CThread::native_handle()
    {
        return m_thread;
    }

    void CThread::PreOperate()
    {
        std::cout << "PreOperate\n";
    }

    void CThread::Operate()
    {
        std::cout << m_threadID << " is Running!!\n";
    }

    void CThread::PostOperate()
    {
        std::cout << "PostOperate\n";
    }

    void CThread::Runnable()
    {
        try 
        { 
            PreOperate(); 
        } 
        catch (const std::exception& e) 
        { 
            std::cerr << "[PreOperate Error] " << e.what() << '\n'; 
        }

        while (m_status != EC_THREAD_STATUS_T::TERMINATED_T) 
        {
            // PAUSED 상태일 때는 CPU를 쓰지 않고 condition_variable로 대기
            if (m_status == EC_THREAD_STATUS_T::PAUSED_T) 
            {
                std::unique_lock<std::mutex> lock(m_cvMtx);
                m_cv.wait(lock, [this] { 
                    return m_status != EC_THREAD_STATUS_T::PAUSED_T; 
                });
                
                // [Fix 3] 깨어났을 때 TERMINATED 상태라면 즉시 탈출 (Operate 실행 방지)
                if (m_status == EC_THREAD_STATUS_T::TERMINATED_T)
                {
                    break;
                }
            }

            // 실제 작업 수행 (CEpoll 루프 등)
            try 
            {
                Operate();
            } 
            catch (const std::exception& e) 
            {
                std::cerr << "[" << m_threadID<< " Operate Error] " << e.what() << '\n';
            }

            if (m_threadType == EC_THREAD_RUN_TYPE_T::ONCE_T) 
            {
                m_status = EC_THREAD_STATUS_T::TERMINATED_T;
                break;
            }
        }

        try 
        { 
            PostOperate(); 
        } 
        catch (const std::exception& e) 
        { 
            std::cerr << "[PostOperate Error] " << e.what() << '\n';
        }
    }
} /* namespace sys */