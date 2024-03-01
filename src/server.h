#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Server {

    public:

        unsigned short getPort() {
            return port;
        }

        sockaddr_in getSocket() {
            return sock;
        }

        char* getBuff() {
            return buff;
        }

        void run();

        void response(const int sock);

    private:

        unsigned short port = 8080;
        sockaddr_in sock;
        char buff[1200];

        std::string read(const std::string request);

};

#endif