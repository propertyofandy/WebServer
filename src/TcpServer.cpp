
#include "../inc/TcpServer.hpp" 
#include <iostream>
#include <sstream>
//#include <unistd.h>

const int BUFFER_SIZE = 30720;

void log(const std::string &message){
    std::cout << message << std::endl; 
}

void exitWithError(const std::string &error_message){
    #if _WIN32
        std::cout << WSAGetLastError() << std::endl;
    #endif 
    log("error: "+ error_message);
    exit(1);
}


namespace Http {

     TcpServer::~TcpServer()
    {
        closeServer();
    }

    
    TcpServer::TcpServer(std::string ip_addr, int port)
    : ip_address(ip_addr), server_port(port), socket_address(),
        server_socket(), new_socket(), incoming_msg(), 
        socket_address_len(sizeof(socket_address)),
        server_msg(buildResponse()) { 
            socket_address.sin_family = AF_INET;
            socket_address.sin_port = htons(server_port); 
            socket_address.sin_addr.s_addr = inet_addr(ip_address.c_str());
            startServer(); 
            }

    int TcpServer::startServer(){
        
        #if _WIN32
            if(WSAStartup(MAKEWORD(2,0), &wsa_data) != 0)
                exitWithError("WSA start up failed");
        #endif
        
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if(server_socket < 0)
            exitWithError("Socket creation failed");
        
        if(bind(server_socket, (sockaddr *)&socket_address, socket_address_len) < 0){
            exitWithError("Cannot connect socket to address.");
            return 1;
        }
        return 0; 
    }

    void TcpServer::closeServer(){
        
        #if __linux__
            close(server_socket);
            close(new_socket);
        #elif _WIN32
            closesocket(server_socket);
            closesocket(new_socket);
            WSACleanup();
        #endif
        
        exit(EXIT_SUCCESS);
    }


    void TcpServer::startListen() {
        if(listen(server_socket, 20) < 0)
            exitWithError("Socket failed to listen.");

        std::ostringstream ss;
        ss << "\n*** Listening on address:"
           << inet_ntoa(socket_address.sin_addr)
           << ntohs(socket_address.sin_port)
           <<"***\n\n";
        log(ss.str());

        int bytes_received; 

        while(true){
            log("=== Waiting for a new connection === \n\n");
            acceptConnection(new_socket);

            char buffer[BUFFER_SIZE] = {0};

            #if _WIN32
                bytes_received = recv(new_socket, buffer, BUFFER_SIZE, 0);
            #elif __linux__
                bytes_received = read(new_socket, buffer, BUFFER_SIZE);
            #endif
            if(bytes_received < 0){
                exitWithError("Failed to receive bytes from client");
            }

            std::ostringstream ss;
            ss << "=== Received request from client === \n\n";
            log(ss.str() + buffer);

            sendResponse();

            #if _WIN32
                closesocket(new_socket);
            #elif __linux__
                close(new_socket);
            #endif
        }
    }

    void TcpServer::acceptConnection(SOCKET &new_socket){
        new_socket = accept(server_socket, (sockaddr *)&socket_address,
            &socket_address_len
        );

        if(new_socket < 0){
            std::ostringstream ss;
            ss << "Server failed to accept connection to address: "
               << inet_ntoa(socket_address.sin_addr)
               << ". Port: "
               << ntohs(socket_address.sin_port);
               exitWithError(ss.str());
        }
    }

     std::string TcpServer::buildResponse()
    {
        std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";
        std::ostringstream ss;
        ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
           << htmlFile;

        return ss.str();
    }

    void TcpServer::sendResponse()
    {
        int bytesSent;

        #if _WIN32
        int totalBytesSent = 0;

        while (totalBytesSent < server_msg.size())
        {
            bytesSent = send(new_socket, server_msg.c_str(), server_msg.size(), 0);
            if (bytesSent < 0)
            {
                break;
            }
            totalBytesSent += bytesSent;
        }

        if (totalBytesSent == server_msg.size())
        {
            log("------ Server Response sent to client ------\n\n");
        }
        else
        {
            log("Error sending response to client.");
        }
        #elif __linux__
            bytesSent = write(m_new_socket, m_serverMessage.c_str(), m_serverMessage.size());

            if (bytesSent == m_serverMessage.size())
            {
                log("------ Server Response sent to client ------\n\n");
            }
            else
            {
                log("Error sending response to client");
            }
        #endif
    }

}