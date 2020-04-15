# lkcp: cmake support for lkcp
# Jian Chen <admin@chensoft.com>
# http://chensoft.com
cmake_minimum_required(VERSION 3.1)
project(lkcp VERSION 1.0.0 LANGUAGES C)

add_library(lkcp MODULE "")

# environment
set_target_properties(lkcp PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
set_target_properties(lkcp PROPERTIES PREFIX "")

# source codes
file(GLOB PROJ_INC src/*.h)
file(GLOB PROJ_SRC src/*.c)

# third party
find_package(Lua REQUIRED)

set(KCP_INC 3rd/kcp/ikcp.h)
set(KCP_SRC 3rd/kcp/ikcp.c)

# generate lib
target_include_directories(lkcp PRIVATE ${LUA_INCLUDE_DIR} 3rd/kcp)
target_sources(lkcp PRIVATE ${PROJ_INC} ${PROJ_SRC} ${KCP_INC} ${KCP_SRC})
target_link_libraries(lkcp ${LUA_LIBRARIES})

# install lib
install(TARGETS lkcp DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")