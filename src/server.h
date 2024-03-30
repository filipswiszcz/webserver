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

#include <random>

#include "worker/factory.h"
#include "client.h"

class Server {

    public:

        Server(const int port);

        ~Server();

        void init();

        void run();

        void stop();

        void tick();

        void debug();

        void add_conn(const Client& client);

        void del_conn(const int sock_dsc);

        void response(const int sock); // TODO it probably shouldn't be here

    private:

        Factory factory;

        int sock_dsc;
        sockaddr_in sock;
        int port;

        bool running = true;
        bool debugging = true;

        std::vector<Client*> conns;

        std::string read(const std::string request);

        // std::thread find_worker();

};

#endif