// conn state
// data
// process packets
    // read data
    // compress data
    // encrypt data?
    // write data

#include "client.h"

#pragma mark - Client
#pragma region Client {

    Client::Client(const int sock_dsc, const sockaddr_in sock) {
        this -> sock_dsc = sock_dsc;
        this -> sock = sock;
    }

    Client::~Client() {}

    void Client::setConnectionState(const ConnState state) {}

#pragma endregion Client }