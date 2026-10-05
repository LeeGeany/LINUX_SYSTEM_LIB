/**
 * @file    CSignal.cpp
 * @author  jinhee.lee
 * @date    2024.07.09
 * @brief   Class of Signal Source
 * 
 * @copyright jinhee.lee
 */

#include "CSignal.h"

namespace sys
{
	std::function<void(int)> CSignal::m_callback;
    
    CSignal::CSignal() noexcept {
        sigemptyset(&m_Set);
    }
    CSignal::~CSignal() noexcept{}

    void CSignal::Insert(int SigType, std::function<void(int)> _callback)
    {
        int Ret = 0;
        sigemptyset(&m_Set);
        Ret = sigaddset(&m_Set, SigType);
        
        m_callback = std::move(_callback);
        
        if(Ret == 0)
        {
            sigprocmask(SIG_SETMASK, &m_Set, NULL);
            signal(SigType, Callback);
        }
    }

    void CSignal::Delete(int SigType)
    {
        (int)sigdelset(&m_Set, SigType);

        sigprocmask(SIG_SETMASK, &m_Set, NULL);
    }

    int CSignal::Block(int SigType)
    {
        int Ret = 0;
        sigset_t tSet;

        Ret = sigaddset(&tSet, SigType);
        sigprocmask(SIG_BLOCK, &tSet, NULL);

        return Ret;
    }

    int CSignal::BlockAll()
    {
        int ret = sigprocmask(SIG_BLOCK, &m_Set, NULL);
        return ret;
    }

    int CSignal::Release(int SigType)
    {
        int Ret = 0;
        sigset_t tSet;
        
        Ret = sigaddset(&tSet, SigType);
        sigprocmask(SIG_BLOCK, &tSet, NULL);

        return Ret;
    }

    int CSignal::ReleaseAll()
    {
        int ret = sigprocmask(SIG_UNBLOCK, &m_Set, NULL);
        return ret;
    }

    void CSignal::Callback(int sig)
    {
        (void)m_callback(sig);
    }
} /* namespace jlib */
