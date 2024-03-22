cmake_minimum_required(VERSION 3.15)

project(portscanl)

set(MAIN_CPP  "./src/portscanl.cpp")
set(CMAKE_EXE_LINKER_FLAGS "-static")
include_directories("./src/include")
add_executable(${PROJECT_NAME} ${MAIN_CPP})

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)
if(WIN32)
    # add_definitions(utf8)
    target_link_libraries(${PROJECT_NAME} ws2_32)
endif(WIN32)
