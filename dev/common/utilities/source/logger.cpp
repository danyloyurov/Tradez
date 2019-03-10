#include "logger.hpp"

#include <iostream>
#include <fstream>
#include <ctime>

Logger::Logger() {

}

Logger& Logger::Instanse() {
    static Logger logger;
    return logger;
}

void Logger::Log(const std::string& message, File) {
    time_t raw_time = 0;
    tm* timestamp = nullptr;
    std::ofstream log_file("tradez_log_file.txt", std::ios::app);

    time(&raw_time);
    timestamp = localtime(&raw_time);

    if(true == log_file.is_open()) {
        log_file << "[" << timestamp->tm_hour << ":"
                  << timestamp->tm_min << ":"
                  << timestamp->tm_sec << "] "
                  <<  message << std::endl;
        log_file.close();
    }
}

void Logger::Log(const std::string& message, Terminal) {
    time_t raw_time = 0;
    tm* timestamp = nullptr;

    time(&raw_time);
    timestamp = localtime(&raw_time);

    std::cout << "[" << timestamp->tm_hour << ":"
              << timestamp->tm_min << ":"
              << timestamp->tm_sec << "] "
              <<  message << std::endl;
}

