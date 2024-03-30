#include <netdb.h>

#ifndef CONN_STATE_H
#define CONN_STATE_H

enum ConnState {
    HANDSHAKE, ESTABLISHED, CLOSED
};

#endif

#ifndef CLIENT_H
#define CLIENT_H

class Client {

    public:

        Client(const int sock_dsc, const sockaddr_in sock);

        ~Client();

        int getSockDescriptor() const {
            return sock_dsc;
        }

        ConnState getConnectionState() {
            return conn_state;
        }

        void setConnectionState(const ConnState state);

    private:

        int sock_dsc;
        sockaddr_in sock;
        ConnState conn_state; // TODO might be deleted in the future

};

#endif