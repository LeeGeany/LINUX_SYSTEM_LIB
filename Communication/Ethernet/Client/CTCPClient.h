/**
 * @file CTCPClient.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-11-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef __EWLIB_COMMUNICATION_ETHERNET_CLIENT_CTCPCLIENT_H__
#define __EWLIB_COMMUNICATION_ETHERNET_CLIENT_CTCPCLIENT_H__

#include "ewlib/stdC++.h"
#include "ewlib/stdLinux.h"

namespace sys
{
    class CTCPClient
    {
    public:
        CTCPClient();
        ~CTCPClient();

    public:
        bool Connect(const std::string ip, int port);
        bool ReconnectWithRetry(int maxRetries = 5, int retryIntervalMs = 1000);
        void Disconnect();
        bool IsConnected() const { return m_isConnected; }

        ssize_t Send(const std::byte* data, size_t length);
        ssize_t Recv(std::byte* buffer, size_t length);

    private:
        bool CreateSocket();

    private:
        int32_t m_socket{-1};
        std::string m_serverIP;
        int m_serverPort{-1};
        bool m_isConnected{false};
    }; /* class CTCPClient */
}/* namespace sys */
#endif /* __EWLIB_COMMUNICATION_ETHERNET_CLIENT_CTCPCLIENT_H__ */