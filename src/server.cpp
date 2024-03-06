#include "server.h"
#include "worker/factory.h"
#include "client.h"

#pragma mark - Server
#pragma region Server {

    Server::Server(const int port) : factory(2) {
        this -> port = port;
    }

    Server::~Server() {}

    void Server::init() {
        int serv_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serv_id == -1) {std::cerr << "Error occurred while creating the server socket." << std::endl; exit(0);}

        sockaddr_in sock; memset(&sock, 0, sizeof(sock));
        sock.sin_family = AF_INET;
        sock.sin_addr.s_addr = htonl(INADDR_ANY);
        sock.sin_port = htons(port);

        int b = bind(serv_id, (sockaddr*) &sock, sizeof(sock));
        if (b != 0) {std::cerr << "Error occurred while binding the socket to the address." << std::endl; exit(0);}

        this -> sock_dsc = serv_id;
        this -> sock = sock;
    }

    void Server::run() {

        // init single thread
            // while !stop, wait for connections with selector
                // find worker
                // accept connection
                // let worker handle the rest

        int l = listen(this -> sock_dsc, 5);
        if (l != 0) {std::cerr << "Error occurred while listening for a connection." << std::endl; exit(0);}

        std::cout << "Waiting for connection on port " << 80 << std::endl;

        while (true) {

            sockaddr_in new_sock;
            socklen_t new_sock_addr = sizeof(new_sock);

            int client_id = accept(this -> sock_dsc, (sockaddr*) &new_sock, &new_sock_addr);
            if (client_id < 1) {std::cerr << "Error occurred while accepting the connection." << std::endl; exit(0);} // TODO move to next one
            std::cout << "Connected with a new client: " << client_id << std::endl;

            // create client in worker and add to the connection map

            factory.enqueue([this, client_id] {
                // TODO here should be a conn establishing and send
                Server::response(client_id);
            });



        }

        //Server::response(client_id);

        //close(client_id);
        close(this -> sock_dsc);

        std::cout << "End of session." << std::endl;
    }

    void Server::stop() {}

    void Server::response(const int sock) {

        const std::string request = Server::read("templates/index.html");

        std::stringstream raw_header;
        raw_header << "HTTP/1.0 200 OK\r\n"
               << "Content-Type: text/html\r\n"
               << "Connection: close\r\n"
               << "Content-Length: " << request.size() << "\r\n\r\n";

        const std::string& header = raw_header.str();

        ssize_t res = send(sock, header.data(), header.length(), MSG_NOSIGNAL);
        if (res < 1) {std::cerr << "Failed to send the rheader." << std::endl; return;}

        res = send(sock, request.data(), request.length(), MSG_NOSIGNAL);
        if (res < 1) {std::cerr << "Failed to send the response." << std::endl; return;}

        while(true) {} // TODO i need to hold the connection, to properly send data.
                            // receive a conn in an available thread
                            // move it, to the main thread's connection map, as a new client
                                // somewhere along the lines, think about region distributions, to handle very large amount of connections
                                // dedicated c++ json parser?
                            // release the worker thread
    }

    std::string Server::read(const std::string request) {

        std::ifstream input(request);
        if (!input) {throw std::runtime_error("Could not open " + request);}

        std::string res((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

        return res;
    }

#pragma endregion Server }