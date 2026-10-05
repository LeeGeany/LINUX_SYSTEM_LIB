/**
 * @file CMsgQ.h
 * @author Jinhee.Lee (tjrgl@naver.com)
 * @brief 
 * @version 0.1
 * @date 2025-07-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __SRCS_COMMUNICATION_IPC_CMSGQ_H__
#define __SRCS_COMMUNICATION_IPC_CMSGQ_H__
 
#include "ewlib/stdC++.h"
#include "ewlib/stdLinux.h"

namespace sys 
{
    inline constexpr size_t MAX_MSGQ_BUFFER_SIZE = 128;

    struct stMsgQ 
    {
        long mtype;
        char mtext[MAX_MSGQ_BUFFER_SIZE];
    };

    class CMsgQ 
    {
    public:
        CMsgQ() noexcept;
        explicit CMsgQ(int32_t _MsgKey) noexcept;
        virtual ~CMsgQ() noexcept;

    public:
        int32_t SubscribeMsgQ(int32_t _MsgKey);


        int SendMsg(char * _pBuffer, size_t _size);
        int RecvMsg(char * _pBuffer, size_t _size);
        
        int FlushMsg();
        int InfoMsg(int32_t _MsgID, struct msqid_ds * _pINfo);
        bool DeleteMsg();

    public:
        int32_t native_handle();

    private:
        int32_t m_MsgID;

        stMsgQ m_SendBuffer;
        stMsgQ m_RecvBuffer;
    }; /* class CMsgQ */
} /* namespace sys */
#endif /* __SRCS_COMMUNICATION_IPC_CMSGQ_H__ */