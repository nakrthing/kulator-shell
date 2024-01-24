cmake_minimum_required(VERSION 3.28.1)
project(kulator VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(kulator ${PROJECT_SOURCE_DIR}/kulator.cpp) #yay


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
