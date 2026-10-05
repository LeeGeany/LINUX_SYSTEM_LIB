/**
 * @file CTCPClient.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-11-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "CTCPClient.h"

namespace sys
{
    CTCPClient::CTCPClient()
    : m_socket(-1),
      m_serverIP(""),
      m_serverPort(-1),
      m_isConnected(false)
    {
        CreateSocket();
    }

    CTCPClient::~CTCPClient()
    {
        Disconnect();
    }

    bool CTCPClient::CreateSocket()
    {
        bool bResult = false;

        if (m_socket >= 0)
        {
            (void)::close(m_socket);
            m_socket = -1;
        }

        m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
        if (m_socket < 0)
        {
            const int32_t err = errno;
            std::cerr << "[CTCPClient] Failed to create socket. errno: " << err << "\n";
            bResult = false;
        }
        else
        {
            bResult = true;
        }

        return bResult;
    }

    bool CTCPClient::Connect(const std::string ip, int port)
    {
        bool bResult = false;

        m_serverIP = ip;
        m_serverPort = port;

        if (m_socket < 0)
        {
            if (!CreateSocket())
            {
                return false;
            }
        }

        sockaddr_in addr{};
        addr.sin_family = static_cast<sa_family_t>(AF_INET);
        addr.sin_port = htons(static_cast<uint16_t>(port));

        if (::inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0)
        {
            std::cerr << "[CTCPClient] Invalid IP Address: " << ip << "\n";
            bResult = false;
        }
        else
        {
            const int32_t ret = ::connect(m_socket, reinterpret_cast<const sockaddr*>(&addr), sizeof(addr));
            if (ret == 0)
            {
                m_isConnected = true;
                bResult = true;
            }
            else
            {
                m_isConnected = false;
                bResult = false;
            }
        }

        return bResult;
    }

    bool CTCPClient::ReconnectWithRetry(int maxRetries, int retryIntervalMs)
    {
        bool bConnected = false;

        Disconnect();

        for (int32_t retry = 1; retry <= maxRetries; ++retry)
        {
            std::cout << "[CTCPClient] Reconnecting to " << m_serverIP << ":" << m_serverPort 
                      << " (Attempt " << retry << "/" << maxRetries << ")...\n";

            if (CreateSocket())
            {
                if (Connect(m_serverIP, m_serverPort))
                {
                    std::cout << "[CTCPClient] Reconnected Successfully!\n";
                    bConnected = true;
                    break; // 성공 시 반복문 탈출
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(retryIntervalMs));
        }

        if (!bConnected)
        {
            std::cerr << "[CTCPClient] Failed to Reconnect after " << maxRetries << " attempts.\n";
        }

        return bConnected;
    }

    void CTCPClient::Disconnect()
    {
        m_isConnected = false;
        if (m_socket >= 0)
        {
            (void)::close(m_socket);
            m_socket = -1;
        }
    }

    ssize_t CTCPClient::Send(const std::byte* data, size_t length)
    {
        if (!m_isConnected || (m_socket < 0) || (data == nullptr))
        {
            return -1;
        }

        size_t totalSent = 0U;
        ssize_t finalResult = -1;

        while (totalSent < length)
        {
            // std::byte* -> const void* 안전 캐스팅 (POSIX send API 대응)
            const void* pSendBuf = static_cast<const void*>(data + totalSent);
            const size_t bytesToSent = length - totalSent;

            const ssize_t sent = ::send(m_socket, pSendBuf, bytesToSent, MSG_NOSIGNAL);

            if (sent <= 0)
            {
                const int32_t err = errno;
                if ((sent < 0) && ((err == EAGAIN) || (err == EWOULDBLOCK)))
                {
                    continue;
                }

                m_isConnected = false;
                finalResult = -1;
                break;
            }

            totalSent += static_cast<size_t>(sent);
        }

        if (totalSent == length)
        {
            finalResult = static_cast<ssize_t>(totalSent);
        }

        return finalResult;
    }

    ssize_t CTCPClient::Recv(std::byte* buffer, size_t length)
    {
        if (!m_isConnected || (m_socket < 0) || (buffer == nullptr))
        {
            return -1;
        }

        size_t totalRecv = 0U;
        ssize_t finalResult = -1;

        while (totalRecv < length)
        {
            void* pRecvBuf = static_cast<void*>(buffer + totalRecv);
            const size_t bytesToRecv = length - totalRecv;

            const ssize_t received = ::recv(m_socket, pRecvBuf, bytesToRecv, 0);

            if (received == 0)
            {
                // FIN 수신 (서버 연결 정상 종료)
                m_isConnected = false;
                finalResult = 0;
                break;
            }
            else if (received < 0)
            {
                const int32_t err = errno;
                if ((err == EAGAIN) || (err == EWOULDBLOCK))
                {
                    continue;
                }

                m_isConnected = false;
                finalResult = -1;
                break;
            }
            else
            {
                totalRecv += static_cast<size_t>(received);
            }
        }

        if (totalRecv == length)
        {
            finalResult = static_cast<ssize_t>(totalRecv);
        }
        return finalResult;
    }
} /* namespace sys */