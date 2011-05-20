add_executable(testerr main.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(testerr easyrandr ${X11_LIBRARIES} ${X11_Xrandr_LIB} ${QT_QT_LIBRARY} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
