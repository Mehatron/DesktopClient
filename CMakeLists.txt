cmake_minimum_required(VERSION 2.8.11)
project(RoboticHand)


find_package(Threads REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(Qt5Widgets)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOMOC ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)

include_directories(include/)
include_directories(
    libs/
    ${Boost_INCLUDE_DIR}
    ${Qt5Widgets_INCLUDE_DIRS}
)

set(SOURCES
    src/wcentral.cpp
    src/wrobotichand.cpp
    src/joypad.cpp

    libs/joystick.cc
    libs/analogclock.cpp
)

set(MOC_HEADERS
    include/wcentral.h
    include/wrobotichand.h
    include/joypad.h

    libs/analogclock.h
)

qt5_wrap_cpp(MOC_OUTFILES ${MOC_HEADERS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_executable(${PROJECT_NAME} ${SOURCES} src/main.cpp ${MOC_OUTFILES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} Qt5::Widgets Threads::Threads)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
