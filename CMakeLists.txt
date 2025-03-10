cmake_minimum_required(VERSION 3.15)

if(NOT ESP_PLATFORM)
  project(tcsl)
endif()

# set(CMAKE_C_STANDARD 90)
set(CMAKE_C_STANDARD 99)
# set(CMAKE_C_STANDARD 11)
# set(CMAKE_C_STANDARD 17)
# set(CMAKE_C_STANDARD 23)

set(TCSL_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

if(ESP_PLATFORM)
  include(${CMAKE_CURRENT_LIST_DIR}/env_support/cmake/esp.cmake)
else()
  include(${CMAKE_CURRENT_LIST_DIR}/env_support/cmake/custom.cmake)
endif()

