#include "Logger.h"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <utility>

namespace netops {
Logger::Logger(std::filesystem::path path) : path_(std::move(path)) {}

void Logger::log(const std::string& level, const std::string& message) const {
    if (!path_.parent_path().empty()) std::filesystem::create_directories(path_.parent_path());
    std::ofstream out(path_, std::ios::app);
    if (!out) throw std::runtime_error("Unable to open log file: " + path_.string());
    const auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    out << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S")
        << " [" << level << "] " << message << '\n';
}
}
