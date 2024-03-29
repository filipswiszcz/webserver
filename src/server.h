#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "worker/factory.h"

class Server {

    public:

        Server(const int port);

        ~Server();

        void init();

        void run();

        void stop();

        void response(const int sock); // TODO it probably shouldn't be here

    private:

        Factory factory;

        int sock_dsc;
        sockaddr_in sock;
        int port;

        bool running = true;

        std::string read(const std::string request);

        // std::thread find_worker();

};

#endif