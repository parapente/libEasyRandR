set(QT_USE_QTCORE TRUE)

include(${QT_USE_FILE})

configure_file (easycfg.h.cmake
                ${CMAKE_CURRENT_SOURCE_DIR}/easycfg.h )

set(libeasyrandr_SRCS outputproperty.cpp crtc.cpp output.cpp configuration.cpp screen.cpp)
set(libeasyrandr_HDRS crtc.h output.h configuration.h screen.h)
set(BUILD_SHARED_LIBS true)

QT4_WRAP_CPP(libeasyrandr_MOC_SRCS ${libeasyrandr_HDRS})

add_library(easyrandr ${libeasyrandr_SRCS} ${libeasyrandr_MOC_SRCS})
install(TARGETS easyrandr EXPORT ERRLibraryDepends DESTINATION lib)
install(EXPORT ERRLibraryDepends DESTINATION lib/easyrandr)

add_subdirectory(test)