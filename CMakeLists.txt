project(WebServer)
cmake_minimum_required(VERSION 3.1)

set(CMAKE_BUILD_TYPE Debug)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O0")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14")

find_library(SEASOCKS_LIB seasocks)

file(GLOB SRC_LIST "./*.cpp" "./controls/*.cpp" "./models/*.cpp")

#set(seasocks_lib ${CMAKE_CURRENT_SOURCE_DIR}/rootfs/lib/libseasocks.a)
set(seasocks_lib /usr/local/lib/libseasocks.a)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost COMPONENTS REQUIRED system filesystem)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    #target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
else()
    message(STATUS "The BOOST not found !!!")
endif()


add_executable(${PROJECT_NAME} ${SRC_LIST})
#add_executable(${PROJECT_NAME} "${PROJECT_SOURCE_DIR}/main.cpp")


#target_link_libraries(${PROJECT_NAME} ${SEA})
#target_link_libraries(${PROJECT_NAME} seasocks)
#target_link_libraries(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/rootfs/lib/libseasocks.a)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${seasocks_lib} pthread z)

