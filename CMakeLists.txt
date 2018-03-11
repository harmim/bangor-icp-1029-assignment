# Author: Dominik Harmim <harmim6@gmail.com>

CMAKE_MINIMUM_REQUIRED(VERSION 3.7)

SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_C_FLAGS "-std=c99 -Wall -Wextra -Werror -g")

PROJECT(word_search)

FILE(GLOB SOURCES "*.c")
FILE(GLOB HEADERS "*.h")

ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCES} ${HEADERS})
