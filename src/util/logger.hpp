#include <iostream>

#include <string>
#include <sstream>
#include <ctime>

#ifndef LOGGER_H
#define LOGGER_H

namespace util {

    class Logger {

        public:

            void info(const std::string& msg);

            // TODO warning, debug
    };

}

#endif