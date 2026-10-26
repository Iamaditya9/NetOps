#pragma once
#include "NetworkManager.h"
#include <filesystem>

namespace netops {
class FileStore {
public:
    static void saveDevices(const NetworkManager& manager, const std::filesystem::path& path);
    static void loadDevices(NetworkManager& manager, const std::filesystem::path& path);
};
}
