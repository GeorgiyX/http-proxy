find_package(Boost REQUIRED COMPONENTS system )

add_executable(${PROJECT_NAME} src/main.cpp src/client.cpp include/client.h include/utils.h src/utils.cpp)
target_link_libraries(${PROJECT_NAME} Boost::system pthread)
include_directories(${Boost_INCLUDE_DIRS} include)
