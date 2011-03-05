configure_file (easycfg.h.cmake
                ${CMAKE_CURRENT_BINARY_DIR}/easycfg.h )

set(libeasyrandr_SRCS main.cpp)

add_library(libeasyrandr ${libeasyrandr_SRCS})

