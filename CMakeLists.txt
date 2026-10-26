cmake_minimum_required(VERSION 3.16)
project(NetOps VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(netops_core
    src/Device.cpp
    src/Server.cpp
    src/Router.cpp
    src/Workstation.cpp
    src/Alert.cpp
    src/Incident.cpp
    src/Logger.cpp
    src/MonitoringEngine.cpp
    src/NetworkManager.cpp
    src/FileStore.cpp
)
target_include_directories(netops_core PUBLIC include)
if(MSVC)
    target_compile_options(netops_core PRIVATE /W4)
else()
    target_compile_options(netops_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(netops src/main.cpp)
target_link_libraries(netops PRIVATE netops_core)

enable_testing()
add_executable(netops_tests tests/test_netops.cpp)
target_link_libraries(netops_tests PRIVATE netops_core)
add_test(NAME netops_tests COMMAND netops_tests)
