#ifndef LOGGER_HPP_
#define LOGGER_HPP_

#include "error_interface.hpp"

#include <string>

class Logger {
  public:
    static Logger& Instanse();

    static struct File {} FileTag;
    static struct Terminal {} TerminalTag;

    void Log(const std::string& message, File);
    void Log(const std::string& message, Terminal);

    Logger(const Logger&) = delete;
    void operator=(const Logger&) = delete;
  private:
    Logger();
  
};

#endif // LOGGER_HPP_

