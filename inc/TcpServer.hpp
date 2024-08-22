#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string>

#if __linux__
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
#elif _WIN32
    #pragma comment(lib, "Ws2_32.lib")
    #pragma comment(lib, "windowscodecs.lib")
    #include <winsock.h>

#endif

namespace Http{


    class TcpServer
    {
    public:
        TcpServer(std::string ip_addr, int port);
        ~TcpServer();
        void startListen(); 
    private:
        int server_port, incoming_msg, socket_address_len;
        std::string ip_address, server_msg; 
        struct sockaddr_in socket_address; 
        
        #if _WIN32
            SOCKET server_socket, new_socket; 
            WSADATA wsa_data; 
        #elif __linux__
            int server_socket, new_socket; 
        #endif

        int startServer();
        void closeServer(); 
        
        #if _WIN32
            void acceptConnection(SOCKET &new_socket);
        #elif __linux__
            void acceptConnection(int &new_socket);
        #endif

        void sendResponse();
        std::string buildResponse();
    };

}




#endif