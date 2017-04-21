# IOS - Project 2
# Author: Dominik Harmim <xharmi00@stud.fit.vutbr.cz>

CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
PROJECT(proj2)

SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_C_FLAGS_RELEASE "-std=gnu99 -Wall -Wextra -Werror -pedantic")
SET(CMAKE_C_FLAGS_DEBUG "-std=gnu99 -Werror")
SET(CMAKE_EXE_LINKER_FLAGS "-lpthread")

FILE(GLOB HEADERS ".*.h")
FILE(GLOB SOURCES "*.c")

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES} ${HEADERS})
