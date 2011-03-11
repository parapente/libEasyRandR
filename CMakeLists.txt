cmake_minimum_required(VERSION 2.8)
project(libeasyrandr)

set(MAJOR_VERSION "0")
set(MINOR_VERSION "0")
set(PATCH_VERSION "1")
set(SUFFIX_VERSION "")

set(VERSION_STRING "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")
if(NOT SUFFIX_VERSION)
	set(VERSION_STRING_FULL "${VERSION_STRING}")
else(NOT SUFFIX_VERSION)
	set(VERSION_STRING_FULL "${VERSION_STRING}-${SUFFIX_VERSION}")
endif(NOT SUFFIX_VERSION)

find_package(X11 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(XRANDR_1_3 xrandr>=1.3)
if (NOT XRANDR_1_3_FOUND)
   pkg_check_modules (XRANDR_1_2 REQUIRED xrandr>=1.2)
endif (NOT XRANDR_1_3_FOUND)
find_package(Qt4 REQUIRED)

include_directories( ${X11_INCLUDE_DIR} ${QT_INCLUDE_DIR}
		     ${QT_QTCORE_INCLUDE_DIR} )

install(DIRECTORY src/ DESTINATION include/easyrandr
          FILES_MATCHING PATTERN "*.h")

add_subdirectory(src)
add_subdirectory(doc)
