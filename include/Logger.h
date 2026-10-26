#pragma once
#include <filesystem>
#include <string>

namespace netops {
class Logger {
public:
    explicit Logger(std::filesystem::path path);
    void log(const std::string& level, const std::string& message) const;
private:
    std::filesystem::path path_;
};
}
