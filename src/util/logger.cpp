#include "logger.hpp"

#pragma mark - Logger
#pragma region Logger {

namespace util {

    void Logger::info(const std::string& msg) {

        time_t curr_t = time(0);
        tm* local_tm = localtime(&curr_t);

        char ts[20];
        strftime(ts, sizeof(ts), "%Y/%m/%d %H:%M:%S", local_tm);

        std::ostringstream ln;
        ln << "[webserver][" << ts << "][INFO]: " << msg;

        std::cout << ln.str() << std::endl; // TODO save it in a log file 
    }

    // TODO warning

} // namespace util

#pragma endregion Logger }