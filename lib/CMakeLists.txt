cmake_minimum_required(VERSION 3.0.0)
project("tianmoucv")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2")

set(inc1 ./usb/inc)
set(inc2 ./usb/api)
set(inc3 ./inc)
set(inc4 ./cyusb/inc)
set(inc5 ./)
set(TIANMOUC_LIB_PATH ./lib)

link_directories(${TIANMOUC_LIB_PATH})
include_directories(${inc1})
include_directories(${inc2})
include_directories(${inc3})
include_directories(${inc4})
include_directories(${inc5})

set(file1 tianmoucv.cpp)

add_compile_definitions(USB_SDK)

include_directories("/usr/local/include/opencv4")
find_package(OpenCV 4 REQUIRED)
if(OpenCV_FOUND)
    message(STATUS "Found OpenCV version: ${OpenCV_VERSION}")
else()
    message(FATAL_ERROR "OpenCV version 4.5.0 or higher is required.")
endif()

# Add RPATH for OpenCV libraries
set(CMAKE_BUILD_RPATH "/usr/lib/x86_64-linux-gnu")
set(CMAKE_INSTALL_RPATH "/usr/lib/x86_64-linux-gnu")

# Add the directory for libtianmouc.so explicitly
set(CMAKE_BUILD_RPATH "${CMAKE_BUILD_RPATH};${PROJECT_SOURCE_DIR}/lib")

add_executable(tianmoucv_app ${file1})

# Link the OpenCV libraries and others
target_link_libraries(tianmoucv_app PRIVATE ${OpenCV_LIBS} pthread cyusb -lusb-1.0 -ltianmouc)
