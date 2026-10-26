#pragma once
#include "Device.h"
namespace netops {
class Server final : public Device {
public:
    using Device::Device;
    std::string type() const override;
};
}
