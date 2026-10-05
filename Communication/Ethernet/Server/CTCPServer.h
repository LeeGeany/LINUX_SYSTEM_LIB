/**
 * @file CTCPServer.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-11-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef __EWLIB_COMMUNICATION_ETHERNET_SERVER_CTCPSERVER_H__
#define __EWLIB_COMMUNICATION_ETHERNET_SERVER_CTCPSERVER_H__

#include "stdC++.h"
#include "stdLinux.h"

namespace sys
{
    class CTCPServer
    {
    public:
        CTCPServer();
        ~CTCPServer();
        CTCPServer(const CTCPServer&) = delete;
        CTCPServer& operator=(const CTCPServer&) = delete;
        bool CreateSocket();
        bool Bind(int port);
        bool Listen(int backlog = 1);
        bool Accept();
        bool Start(int port);
        void DisconnectClient();
        void Stop();
        bool IsConnected() const;

        ssize_t Send(const std::byte* data, size_t length);
        ssize_t Recv(std::byte* buffer, size_t length);
        int GetServerSocket() const;
        int GetClientSocket() const;

    private:
        int m_serverSocket;
        int m_clientSocket;
        int m_serverPort;
        bool m_isListening;
        bool m_isConnected;
    };

} /* namespace sys */
#endif /* __EWLIB_COMMUNICATION_ETHERNET_SERVER_CTCPSERVER_H__ */