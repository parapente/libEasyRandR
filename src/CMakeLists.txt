configure_file (easycfg.h.cmake
                ${CMAKE_CURRENT_SOURCE_DIR}/easycfg.h )

add_library(libeasyrandr output.cpp configuration.cpp screen.cpp)

