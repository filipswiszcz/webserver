#include "server.h"

int main() {
    
    Server server(80);
    server.init();
    server.run();

    return 0;
}