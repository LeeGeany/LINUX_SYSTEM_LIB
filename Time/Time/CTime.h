/**
 * @file CTime.h
 * @author Jinhee.Lee (tjrgl@naver.com)
 * @brief 
 * @version 0.1
 * @date 2025-10-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef _EWLIB_TIME_TIME_CTIME_H__
#define _EWLIB_TIME_TIME_CTIME_H__

#include "ewlib/stdC++.h"

namespace sys
{
    struct stSystemTime
    {
        std::int64_t seconds;
        std::int64_t nanoseconds;
    }; /* struct stSystemTime */

    class CTime
    {
    public:
        CTime();
        virtual ~CTime();

    public:
        stSystemTime getSystemTime() noexcept;
        std::string getSystemTimeString() noexcept;
        bool set_system_time(const stSystemTime& new_time) noexcept;

    private:
        std::string to_string(const stSystemTime& time) noexcept;
        inline constexpr bool is_leap_year(int year) noexcept;
        
    private:
        stSystemTime m_tm;
    }; /* class CTime */
} /* namespace sys */
#endif /* _EWLIB_TIME_TIME_CTIME_H__ */