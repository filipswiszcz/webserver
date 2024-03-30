#include "server.h"

#pragma mark - Server
#pragma region Server {

    Server::Server(const int port) : factory(2) {
        this -> port = port;
    }

    Server::~Server() {}

    void Server::init() {
        int serv_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serv_id == -1) {std::cerr << "Error occurred while creating the server socket." << std::endl; exit(1);}

        sockaddr_in sock; memset(&sock, 0, sizeof(sock));
        sock.sin_family = AF_INET;
        sock.sin_addr.s_addr = htonl(INADDR_ANY);
        sock.sin_port = htons(port);

        int b = bind(serv_id, (sockaddr*) &sock, sizeof(sock));
        if (b != 0) {std::cerr << "Error occurred while binding the socket to the address." << std::endl; exit(1);}

        this -> sock_dsc = serv_id;
        this -> sock = sock;
    }

    void Server::run() {

        // init single thread
            // while !stop, wait for connections with poll (epoll is not working on macos because it is called kqueue here)
                // find worker
                // accept connection
                // let worker handle the rest

        int l = listen(this -> sock_dsc, 5);
        if (l != 0) {std::cerr << "Error occurred while listening for a connection." << std::endl; exit(1);}
        std::cout << "Waiting for connection on port " << 80 << std::endl;

        this -> running = true;

        std::thread t([this]() {
            while (running) {

                sockaddr_in client_sock;
                socklen_t client_sock_addr = sizeof(client_sock);

                int client_id = accept(this -> sock_dsc, (sockaddr*) &client_sock, &client_sock_addr);
                if (client_id < 1) {std::cerr << "Error occurred while accepting the connection." << std::endl; exit(1);} // TODO move to next one
                std::cout << "New connection from id=" << client_id << std::endl;
                
                this -> factory.enqueue([this, client_id, client_sock] {
                    Client client(client_id, client_sock); // TODO add client to connection map
                    this -> add_conn(client);
                    // Server::response(client_id);
                });
            }
        });

        this -> tick();

        t.join();

        //Server::response(client_id);

        //close(client_id);
        close(this -> sock_dsc);

        std::cout << "End of session." << std::endl;
    }

    void Server::stop() {}

    void Server::tick() {
        std::thread t([this]() {
            while (running) {
                std::cout << "[alive]" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(5));

                for (auto client : this -> conns) {
                    std::cout << client -> getSockDescriptor() << std::endl;
                    Server::response(client -> getSockDescriptor()); std::cout << "[sent]" << std::endl;
                }
            }
        });

        t.join();
    }

    void Server::debug() { // i have no idea if this is going to work
        this -> debugging = !(this -> debugging);
    }

    void Server::add_conn(const Client& client) { // TODO check if client is already there
        this -> conns.push_back(new Client(client));         // also resolve opening same www in second tab
        if (this -> debugging) {std::cout << "[debug] added a new client " << client.getSockDescriptor() << " to conn list." << std::endl;}
    }

    void Server::del_conn(const int sock_dsc) {
        for (int i = 0; i < this -> conns.size(); i++) {
            if (this -> conns[i] -> getSockDescriptor() != sock_dsc) continue;
            delete this -> conns[i]; this -> conns.erase(this -> conns.begin() + i); break;
        }
        if (this -> debugging) {std::cout << "[debug] deleted client " << sock_dsc << " from conn list." << std::endl;}
    }

    void Server::response(const int sock) {

        const std::string request = Server::read("templates/index.html"); // TODO templates should have an absolute path

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

        /*while(true) {
            std::this_thread::sleep_for(std::chrono::seconds(5));
            
            std::cout << "pulse" << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(5));
            
            std::cout << "pulse" << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(5));

            break;
        }*/ // TODO i need to hold the connection, to properly send data.
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