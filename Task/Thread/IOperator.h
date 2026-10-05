/**
 * @file IOperator.h
 * @author Jinhee.Lee (tjrgl@naver.com)
 * @brief 
 * @version 0.1
 * @date 2025-07-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __EWLIB_TASK_THREAD_IOPERATE_H__
#define __EWLIB_TASK_THREAD_IOPERATE_H__

namespace sys 
{
    class IOperator
    {
    public:
        IOperator(){};
        virtual ~IOperator(){};

    protected:
        virtual void PreOperate() =0;
        virtual void Operate() = 0;
        virtual void PostOperate() = 0;

    protected:
        virtual bool Start()=0;
        virtual bool Stop()=0;
    }; /* IOperator */
} /* namespace EWLIB */
#endif /* __EWLIB_TASK_THREAD_IOPERATE_H__ */
