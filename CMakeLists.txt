cmake_minimum_required(VERSION 2.8)
project(game_sandbox)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${game_sandbox_SOURCE_DIR}/cmake")

set(BIN_DIR ${game_sandbox}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++17")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")

include_directories(/usr/include/SDL2 /usr/include/boost ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB ROOT_SRC "src/*.cpp")
file(GLOB INPUT_SRC "src/input/*.cpp")

add_executable(game_sandbox ${ROOT_SRC} ${INPUT_SRC})

if(MINGW)
	target_link_libraries(game_sandbox /usr/local/lib/libSDL2.dll.a )
else()
	target_link_libraries(game_sandbox /usr/lib/libSDL2.so /usr/lib/libSDL2_image.so)
endif()

install(TARGETS game_sandbox RUNTIME DESTINATION ${BIN_DIR})
