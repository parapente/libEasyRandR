add_executable(testerr main.cpp)

include_directories(..)

link_directories(..)

target_link_libraries(testerr libeasyrandr ${X11_LIBRARIES} ${X11_Xrandr_LIB} ${QT_QT_LIBRARY} ${QT_QTCORE_LIBRARY})
