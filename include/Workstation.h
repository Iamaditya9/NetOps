#pragma once
#include "Device.h"
namespace netops {
class Workstation final : public Device {
public:
    using Device::Device;
    std::string type() const override;
};
}
