#include <stdio.h>
#include "./inc/TcpServer.hpp"

int main(int, char**){
    Http::TcpServer server = Http::TcpServer("127.0.0.1",8080);
    server.startListen();
}
