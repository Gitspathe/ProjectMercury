#ifndef LOG_H
#define LOG_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <mutex>
#include <memory>
#include <string>
//#include <bits/fs_path.h>

namespace GameLogging
{
    namespace log_targets
    {
        struct serverCmd {};
        struct general {};
    }

    struct endl_struct {};

    class log {
    private:
        std::ostream* currentStream;
        std::ostream* defaultStream;
        std::ostream* serverCmdStream;
        std::ofstream logFile;
        std::mutex mutex;

    public:
        static log write;
        static log_targets::serverCmd general;
        static log_targets::serverCmd serverCmd;
        static endl_struct endl;

        log() : currentStream(nullptr), defaultStream(nullptr), serverCmdStream(nullptr) {}

        static log& init(std::ostream* defaultStream, std::ostream* serverCmdStream)
        {
            write.currentStream = defaultStream;
            write.defaultStream = defaultStream;
            write.serverCmdStream = serverCmdStream;
            return write;
        }

        log& operator()(const log_targets::serverCmd&)
        {
            std::lock_guard lock(mutex);
            currentStream = serverCmdStream ? serverCmdStream : defaultStream;
            return *this;
        }

        log& operator()(const log_targets::general&)
        {
            std::lock_guard lock(mutex);
            currentStream = defaultStream;
            return *this;
        }

        template <typename T>
        log& operator<<(const T& value)
        {
            std::lock_guard lock(mutex);
            if(currentStream != nullptr) {
                *currentStream << value;
            }
            if (logFile.is_open()) {
                logFile << value;
            }
            return *this;
        }

        log& operator<<(std::ostream& (*manip)(std::ostream&))
        {
            std::lock_guard lock(mutex);
            if(currentStream != nullptr) {
                *currentStream << manip;
            }
            if (logFile.is_open()) {
                logFile << manip;
            }
            return *this;
        }

        log& operator<<(const endl_struct&)
        {
            std::lock_guard lock(mutex);
            if(currentStream != nullptr) {
                *currentStream << std::endl;
            }
            if (logFile.is_open()) {
                logFile << std::endl;
            }
            currentStream = defaultStream;
            return *this;
        }

        static void setLogFile(const std::string& filename)
        {
            std::lock_guard lock(write.mutex);
            const std::filesystem::path logPath(filename);
            create_directories(logPath.parent_path());
            if (write.logFile.is_open()) {
                write.logFile.flush();
                write.logFile.close();
            }
            write.logFile.open(filename, std::ios::out | std::ofstream::trunc);
        }

        static void flush()
        {
            if (write.logFile.is_open()) {
                write.logFile.flush();
            }
        }

        void resetToDefault()
        {
            std::lock_guard lock(mutex);
            currentStream = defaultStream;
        }
    };

    // Static definitions.
    log log::write;
    log_targets::serverCmd log::serverCmd;
    log_targets::serverCmd log::general;
    endl_struct log::endl;
}
#endif //LOG_H
