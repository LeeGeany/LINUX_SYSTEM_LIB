/**
 * @file CMsgQ.cpp
 * @author Jinhee.Lee (tjrgl@naver.com)
 * @brief 
 * @version 0.1
 * @date 2025-07-13
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #include "CMsgQ.h"

 namespace sys 
 {

    CMsgQ::CMsgQ() noexcept {}
    CMsgQ::CMsgQ(int32_t _MsgKey) noexcept
    : m_MsgID{SubscribeMsgQ(_MsgKey)}, m_SendBuffer{1,}, m_RecvBuffer{1,} {}
    CMsgQ::~CMsgQ() noexcept
    { FlushMsg(); }

    int32_t CMsgQ::SubscribeMsgQ(int32_t _MsgKey)
    {
        return msgget(_MsgKey, IPC_CREAT | 0x666);
    }

    int CMsgQ::SendMsg(char * _pBuffer, size_t _size)
    {
        m_SendBuffer.mtype = 1;
        memcpy(m_SendBuffer.mtext, _pBuffer, _size);

        int ret = msgsnd(m_MsgID, &m_SendBuffer, sizeof(stMsgQ) - sizeof(long), IPC_NOWAIT);
        if(ret == -1) {
            printf("Send Errno = %d\n", errno);
        } else {
            printf("Send Success\n");
        }

        return ret;
    }

    int CMsgQ::RecvMsg(char * _pBuffer, size_t _size)
    {
        int ret =  msgrcv(m_MsgID, &m_RecvBuffer, sizeof(stMsgQ) - sizeof(long), 0, 0);
        if(ret == -1) {
            printf("Receive Errno = %d\n", errno);
        } else {
            memcpy(_pBuffer, &m_RecvBuffer.mtext, _size);
        }

        return ret;
    }

    int CMsgQ::FlushMsg()
    {
        int ret = msgctl(m_MsgID, IPC_RMID, NULL);
        return ret;
    }

    int CMsgQ::InfoMsg(int32_t _MsgID, struct msqid_ds * _pINfo)
    {
        int ret = msgctl(m_MsgID, IPC_STAT|IPC_SET, _pINfo);
        return ret;
    }


    bool CMsgQ::DeleteMsg()
    {
        bool ret = msgctl(m_MsgID, IPC_RMID, NULL);
        
        if(ret == -1)
        {
            std::cout << "errno = " << errno << " errno\n";
        }

        return ret;
    }

    int32_t CMsgQ::native_handle()
    {
        return m_MsgID;
    }

 } /* namespace EWLIB */