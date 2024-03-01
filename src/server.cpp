#include "server.h"

#pragma mark - Server
#pragma region Server {

    void Server::run() {

        int serv_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serv_id == -1) {std::cerr << "Error occurred while creating the server socket." << std::endl; exit(0);}

        sockaddr_in sock = Server::sock;
        //char* buff = Server::buff;

        sock.sin_family = AF_INET;
        sock.sin_addr.s_addr = INADDR_ANY;
        sock.sin_port = htons(Server::port);

        int b = bind(serv_id, (sockaddr*) &sock, sizeof(sock));
        if (b != 0) {std::cerr << "Error occurred while binding the socket to the address." << std::endl; exit(0);}

        int l = listen(serv_id, 10);
        if (l != 0) {std::cerr << "Error occurred while listening for a connection." << std::endl; exit(0);}

        std::cout << "Waiting for connection on port " << Server::port << std::endl;

        sockaddr_in new_sock;
        socklen_t new_sock_addr = sizeof(new_sock);
        
        int client_id = accept(serv_id, (sockaddr*) &new_sock, &new_sock_addr);
        if (client_id < 1) {std::cerr << "Error occurred while accepting the connection." << std::endl; exit(0);}

        std::cout << "Connected with a new client: " << client_id << std::endl;

        Server::response(client_id);

        /*int read, write = 0;

        while (1) {

            std::cout << "Waiting for client response.." << std::endl;

            memset((char*) buff, 0, sizeof(buff));

            read += recv(new_id, (char*) buff, sizeof(buff), 0);
            if (!strcmp(buff, "close")) {
                std::cout << "Client has closed the connection." << std::endl; break;
            }

            std::cout << "[client]: " << buff << std::endl;
            std::cout << "[server]: ";

            std::string text;
            getline(std::cin, text); memset((char*) buff, 0, sizeof(buff)); strcpy(buff, text.c_str());
            if (text == "close") {
                send(new_id, (char*) buff, strlen(buff), 0); break;
            }

            write += send(new_id, (char*) buff, strlen(buff), 0);
        }*/

        close(client_id);
        close(serv_id);

        std::cout << "End of session." << std::endl;
    }

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

        std::cout << "sended data" << std::endl;
    }

    std::string Server::read(const std::string request) {

        std::ifstream input(request);
        if (!input) {throw std::runtime_error("Could not open " + request);}

        std::string res((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());

        return res;
    }

#pragma endregion Server }