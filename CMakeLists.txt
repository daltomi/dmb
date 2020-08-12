CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(dmb)

SET(APP dmb)

FILE(GLOB SOURCES "*.cxx" "*.hpp")

set( CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -std=c++11 -Ofast -Wl,-s")

set( CMAKE_CXX_FLAGS_DEBUG "-DDEBUG  -std=c++11 -O0 -ggdb")

set( CMAKE_INSTALL_PREFIX "/usr")

FIND_PACKAGE(FLTK REQUIRED)

ADD_EXECUTABLE(${APP} ${SOURCES})

TARGET_LINK_LIBRARIES(${APP} -lportaudio -lpthread -lrt -lsndfile ${FLTK_LIBRARIES})

INSTALL ( TARGETS "${APP}" 
    DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")

INSTALL ( FILES "${CMAKE_SOURCE_DIR}/thunar-sendto-dmb.desktop" 
    DESTINATION "/usr/share/Thunar/sendto/")

