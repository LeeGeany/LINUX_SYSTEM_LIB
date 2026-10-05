/**
 * @file CSharedMutex.h
 * @author Jinhee.Lee (tjrgl@naver.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __EWLIB_SYNC_MUTEX_CSHAREDMUTEX_H__
#define __EWLIB_SYNC_MUTEX_CSHAREDMUTEX_H__

#include "ewlib/stdLinux.h"
#include "ewlib/stdC++.h"

namespace sys
{
    class CSharedMutex : public std::shared_mutex
    {
    public:
        CSharedMutex() noexcept;
        virtual ~CSharedMutex() noexcept;
    
    public:
        void lock();
        void try_lock();
        void unlock();
        
    public:
        void lock_shared();
        void try_lock_shared();
        void unlock_shared();
    }; /* class CSharedMutex */
} /* namespace sys */
#endif /* __EWLIB_SYNC_MUTEX_CSHAREDMUTEX_H__ */