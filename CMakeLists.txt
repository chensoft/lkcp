# lua-kcp: Lua bindings for KCP
# Jian Chen <admin@chensoft.com>
# http://chensoft.com
cmake_minimum_required(VERSION 3.1)
project(kcp VERSION 1.7 LANGUAGES C)

add_library(kcp MODULE "")

# third party
find_package(Lua REQUIRED)

set(KCP_INC 3rd/kcp/ikcp.h)
set(KCP_SRC 3rd/kcp/ikcp.c)

# environment
set_target_properties(kcp PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED ON)
set_target_properties(kcp PROPERTIES PREFIX "")

# source codes
file(GLOB PROJ_INC src/*.h)
file(GLOB PROJ_SRC src/*.c)

# generate lib
target_include_directories(kcp PRIVATE ${LUA_INCLUDE_DIR} 3rd/kcp)
target_sources(kcp PRIVATE ${PROJ_INC} ${PROJ_SRC} ${KCP_INC} ${KCP_SRC})
target_link_libraries(kcp ${LUA_LIBRARIES})

# install lib
install(TARGETS kcp DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")