cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 17)
set(PROJECT_NAME http_proxy)
project(${PROJECT_NAME})

set(PROJECT_HEADERS include/utils.h include/flow.h include/http.h)
set(PROJECT_SRC src/utils.cpp src/main.cpp include/proxy.h src/proxy.cpp src/flow.cpp src/http.cpp)

find_package(Boost REQUIRED COMPONENTS system )
add_executable(${PROJECT_NAME} ${PROJECT_HEADERS} ${PROJECT_SRC})
target_link_libraries(${PROJECT_NAME} Boost::system pthread)
include_directories(${Boost_INCLUDE_DIRS} include)
