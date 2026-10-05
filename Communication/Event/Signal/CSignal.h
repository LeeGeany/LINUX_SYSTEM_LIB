/**
 * @file    CSignal.h
 * @author  jinhee.lee
 * @date    2024.07.09
 * @brief   Class of Signal Header
 * 
 * @copyright jinhee.lee
 */

#ifndef __EWLIB_EVENT_SIGNAL_SIGNAL_H__
#define __EWLIB_EVENT_SIGNAL_SIGNAL_H__

#include "ewlib/stdC++.h"
#include "ewlib/stdLinux.h"

namespace sys
{
    class CSignal
    {
    public:
        CSignal() noexcept;
        virtual ~CSignal() noexcept;


    public:
        void Insert(int SigType, std::function<void(int)>  _callback);
        void Delete(int SigType);
        int Block(int SigType);
        int BlockAll();
        int Release(int SigType);
        int ReleaseAll();

    private:
        static void Callback(int sig);

    private:
        sigset_t m_Set;
        static std::function<void(int)> m_callback;
    }; /* class CSignal */
} /* namespace sys */
#endif /* __EWLIB_EVENT_SIGNAL_SIGNAL_H__ */