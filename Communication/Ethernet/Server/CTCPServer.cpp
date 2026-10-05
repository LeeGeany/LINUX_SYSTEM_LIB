/**
 * @file CTCPServer.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-11-29
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "CTCPServer.h"

namespace sys
{
    CTCPServer::CTCPServer()
        : m_serverSocket(-1),
          m_clientSocket(-1),
          m_serverPort(-1),
          m_isListening(false),
          m_isConnected(false)
    {
        (void)CreateSocket();
    }

    CTCPServer::~CTCPServer()
    {
        Stop();
    }

    bool CTCPServer::CreateSocket()
    {
        bool bResult = false;

        /*
         * CreateSocket()은 Server Socket을 생성한다.
         *
         * Client Socket은 Accept()에서 생성되므로
         * 여기서는 닫지 않는다.
         */
        if (m_serverSocket >= 0)
        {
            (void)::close(m_serverSocket);
            m_serverSocket = -1;
        }

        m_isListening = false;

        m_serverSocket =
            ::socket(AF_INET, SOCK_STREAM, 0);

        if (m_serverSocket < 0)
        {
            const int32_t err = errno;

            std::cerr
                << "[CTCPServer] Failed to create socket. "
                << "errno: " << err << "\n";

            return false;
        }

        /*
         * Server Socket 재사용.
         *
         * Server 프로그램이 종료되었다가 다시 시작될 때
         * TIME_WAIT 때문에 bind()가 실패하는 것을 방지한다.
         */
        const int optValue = 1;

        const int ret = ::setsockopt(
            m_serverSocket,
            SOL_SOCKET,
            SO_REUSEADDR,
            &optValue,
            sizeof(optValue));

        if (ret != 0)
        {
            const int32_t err = errno;

            std::cerr
                << "[CTCPServer] Failed to set "
                << "SO_REUSEADDR. errno: "
                << err << "\n";

            (void)::close(m_serverSocket);
            m_serverSocket = -1;

            return false;
        }

        bResult = true;

        return bResult;
    }

    bool CTCPServer::Bind(int port)
    {
        if (m_serverSocket < 0)
        {
            if (!CreateSocket())
            {
                return false;
            }
        }

        if ((port <= 0) || (port > 65535))
        {
            std::cerr
                << "[CTCPServer] Invalid port: "
                << port << "\n";

            return false;
        }

        sockaddr_in addr{};

        addr.sin_family =
            static_cast<sa_family_t>(AF_INET);

        addr.sin_addr.s_addr =
            htonl(INADDR_ANY);

        addr.sin_port =
            htons(static_cast<uint16_t>(port));

        const int ret = ::bind(
            m_serverSocket,
            reinterpret_cast<const sockaddr*>(&addr),
            sizeof(addr));

        if (ret != 0)
        {
            const int32_t err = errno;

            std::cerr
                << "[CTCPServer] Failed to bind. "
                << "errno: " << err << "\n";

            return false;
        }

        m_serverPort = port;

        return true;
    }

    bool CTCPServer::Listen(int backlog)
    {
        if (m_serverSocket < 0)
        {
            return false;
        }

        if (backlog <= 0)
        {
            return false;
        }

        const int ret =
            ::listen(m_serverSocket, backlog);

        if (ret != 0)
        {
            const int32_t err = errno;

            std::cerr
                << "[CTCPServer] Failed to listen. "
                << "errno: " << err << "\n";

            return false;
        }

        m_isListening = true;

        std::cout
            << "[CTCPServer] Listening on port "
            << m_serverPort
            << "\n";

        return true;
    }

    bool CTCPServer::Accept()
    {
        if (!m_isListening ||
            (m_serverSocket < 0))
        {
            return false;
        }

        /*
         * 기존 Client가 연결되어 있다면
         * 새로운 Client를 받지 않는다.
         *
         * 즉, 1대1 통신이다.
         */
        if (m_isConnected)
        {
            return false;
        }

        sockaddr_in clientAddr{};
        socklen_t clientAddrLen =
            sizeof(clientAddr);

        const int clientSocket =
            ::accept(
                m_serverSocket,
                reinterpret_cast<sockaddr*>(&clientAddr),
                &clientAddrLen);

        if (clientSocket < 0)
        {
            const int32_t err = errno;

            if (err == EINTR)
            {
                return false;
            }

            std::cerr
                << "[CTCPServer] Failed to accept. "
                << "errno: " << err << "\n";

            return false;
        }

        /*
         * 기존 Client Socket이 혹시 남아 있다면
         * 안전하게 닫는다.
         */
        if (m_clientSocket >= 0)
        {
            (void)::close(m_clientSocket);
        }

        m_clientSocket = clientSocket;
        m_isConnected = true;

        char clientIP[INET_ADDRSTRLEN] = {};

        const char* result =
            ::inet_ntop(
                AF_INET,
                &clientAddr.sin_addr,
                clientIP,
                sizeof(clientIP));

        if (result != nullptr)
        {
            std::cout
                << "[CTCPServer] Client connected: "
                << clientIP
                << ":"
                << ntohs(clientAddr.sin_port)
                << "\n";
        }
        else
        {
            std::cout
                << "[CTCPServer] Client connected.\n";
        }

        return true;
    }

    bool CTCPServer::Start(int port)
    {
        if (!Bind(port))
        {
            return false;
        }

        if (!Listen(1))
        {
            return false;
        }

        return true;
    }

    void CTCPServer::DisconnectClient()
    {
        /*
         * 중요:
         *
         * Server Socket은 절대로 닫지 않는다.
         *
         * Client Socket만 닫고
         * 다시 Accept()할 수 있도록 만든다.
         */
        m_isConnected = false;

        if (m_clientSocket >= 0)
        {
            (void)::shutdown(
                m_clientSocket,
                SHUT_RDWR);

            (void)::close(m_clientSocket);

            m_clientSocket = -1;
        }

        std::cout
            << "[CTCPServer] Client disconnected.\n";
    }

    void CTCPServer::Stop()
    {
        /*
         * Client Socket 종료
         */
        DisconnectClient();

        /*
         * Server Socket 종료
         */
        m_isListening = false;

        if (m_serverSocket >= 0)
        {
            (void)::shutdown(
                m_serverSocket,
                SHUT_RDWR);

            (void)::close(m_serverSocket);

            m_serverSocket = -1;
        }

        m_serverPort = -1;
    }

    bool CTCPServer::IsConnected() const
    {
        return m_isConnected;
    }

    ssize_t CTCPServer::Send(
        const std::byte* data,
        size_t length)
    {
        if (!m_isConnected ||
            (m_clientSocket < 0) ||
            (data == nullptr))
        {
            return -1;
        }

        size_t totalSent = 0U;

        while (totalSent < length)
        {
            const void* pSendBuf =
                static_cast<const void*>(
                    data + totalSent);

            const size_t bytesToSend =
                length - totalSent;

            const ssize_t sent =
                ::send(
                    m_clientSocket,
                    pSendBuf,
                    bytesToSend,
                    MSG_NOSIGNAL);

            if (sent <= 0)
            {
                if (sent < 0)
                {
                    const int32_t err = errno;

                    if (err == EINTR)
                    {
                        continue;
                    }

                    if ((err == EAGAIN) ||
                        (err == EWOULDBLOCK))
                    {
                        continue;
                    }
                }

                /*
                 * Client 연결이 끊어진 경우
                 *
                 * Server Socket은 유지하고
                 * Client Socket만 정리한다.
                 */
                DisconnectClient();

                return -1;
            }

            totalSent +=
                static_cast<size_t>(sent);
        }

        return static_cast<ssize_t>(totalSent);
    }

    ssize_t CTCPServer::Recv(
        std::byte* buffer,
        size_t length)
    {
        if (!m_isConnected ||
            (m_clientSocket < 0) ||
            (buffer == nullptr))
        {
            return -1;
        }

        size_t totalRecv = 0U;

        while (totalRecv < length)
        {
            void* pRecvBuf =
                static_cast<void*>(
                    buffer + totalRecv);

            const size_t bytesToRecv =
                length - totalRecv;

            const ssize_t received =
                ::recv(
                    m_clientSocket,
                    pRecvBuf,
                    bytesToRecv,
                    0);

            if (received == 0)
            {
                /*
                 * Client가 FIN을 보냈다.
                 *
                 * Server Socket은 그대로 유지하고
                 * Client Socket만 제거한다.
                 */
                DisconnectClient();

                return 0;
            }

            if (received < 0)
            {
                const int32_t err = errno;

                if (err == EINTR)
                {
                    continue;
                }

                if ((err == EAGAIN) ||
                    (err == EWOULDBLOCK))
                {
                    continue;
                }

                DisconnectClient();

                return -1;
            }

            totalRecv +=
                static_cast<size_t>(received);
        }

        return static_cast<ssize_t>(totalRecv);
    }

    int CTCPServer::GetServerSocket() const
    {
        return m_serverSocket;
    }

    int CTCPServer::GetClientSocket() const
    {
        return m_clientSocket;
    }

} /* namespace sys */
