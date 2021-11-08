cmake_minimum_required(VERSION 3.0.0)
project(songwhip_bot VERSION 0.1.0)
include(FetchContent)

FetchContent_Declare(sleepydiscord GIT_REPOSITORY https://github.com/yourWaifu/sleepy-discord.git GIT_TAG develop)
FetchContent_MakeAvailable(sleepydiscord)

# Fetch nlohmann::json
FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json
  GIT_TAG v3.10.4)

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# Fetch libcurl
set(CURL_LIBRARY "-lcurl") 
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

include(CTest)
enable_testing()

add_executable(songwhip_bot main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(songwhip_bot PRIVATE ${CURL_LIBRARIES})
target_link_libraries(songwhip_bot PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(songwhip_bot PRIVATE sleepy-discord)
