cmake_minimum_required(VERSION 3.0)

project(tensorrt_zoo)
set(CMAKE_CXX_FLAGS "-std=c++11")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

find_package(CUDA REQUIRED)
include_directories(${CUDA_INCLUDE_DIRS})

find_package(OpenCV REQUIRED)
if (NOT OpenCV_FOUND)
    message(FATAL_ERROR "opencv not found")
endif (NOT OpenCV_FOUND)

include_directories(tiny-tensorrt)
link_directories(/usr/lib/x86_64-linux-gnu)
link_directories(lib/)
# 查询系统名称，jeston和ubuntu下的cuda路径不一样需要分开处理
cmake_host_system_information(RESULT hostname QUERY HOSTNAME)
message("host name is: " ${hostname})
if (${hostname} STREQUAL "nvidia")
  message("use AGX Xavier")
  find_library(CUDART cudart HINTS /usr/local/cuda/targets/aarch64-linux/lib/)
else()
  message("use PC")
  find_library(CUDART cudart HINTS /usr/local/cuda/targets/x86_64-linux/lib/)
endif()

add_subdirectory(tiny-tensorrt)

add_subdirectory(yolov3)

add_subdirectory(openpose)

