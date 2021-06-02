#
# Yder library
#
# CMake file used to build all programs
#
# Copyright 2018 Silvio Clecio <silvioprog@gmail.com>
# Copyright 2018-2020 Nicolas Mora <mail@babelouest.org>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the MIT License
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#

cmake_minimum_required(VERSION 3.5)

project(yder C)

set(CMAKE_C_STANDARD 99)
if (NOT MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
endif()

# library info

set(PROJECT_DESCRIPTION "Logging library for C applications")
set(PROJECT_HOMEPAGE_URL "https://github.com/babelouest/yder/")
set(PROJECT_BUGREPORT_PATH "https://github.com/babelouest/yder/issues")
set(LIBRARY_VERSION_MAJOR "1")
set(LIBRARY_VERSION_MINOR "4")
set(LIBRARY_VERSION_PATCH "14")

set(PROJECT_VERSION "${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}")
set(PROJECT_VERSION_MAJOR ${LIBRARY_VERSION_MAJOR})
set(PROJECT_VERSION_MINOR ${LIBRARY_VERSION_MINOR})
set(PROJECT_VERSION_PATCH ${LIBRARY_VERSION_PATCH})

if (${LIBRARY_VERSION_MAJOR} VERSION_LESS 10)
    set (LIBRARY_VERSION_MAJOR_PAD "0${LIBRARY_VERSION_MAJOR}")
else ()
    set (LIBRARY_VERSION_MAJOR_PAD "${LIBRARY_VERSION_MAJOR}")
endif ()
if (${LIBRARY_VERSION_MINOR} VERSION_LESS 10)
    set (LIBRARY_VERSION_MINOR_PAD "0${LIBRARY_VERSION_MINOR}")
else ()
    set (LIBRARY_VERSION_MINOR_PAD "${LIBRARY_VERSION_MINOR}")
endif ()
if (${LIBRARY_VERSION_PATCH} VERSION_LESS 10)
    set (LIBRARY_VERSION_PATCH_PAD "0${LIBRARY_VERSION_PATCH}")
else ()
    set (LIBRARY_VERSION_PATCH_PAD "${LIBRARY_VERSION_PATCH}")
endif ()
set(PROJECT_VERSION_NUMBER "${LIBRARY_VERSION_MAJOR_PAD}${LIBRARY_VERSION_MINOR_PAD}${LIBRARY_VERSION_PATCH_PAD}")

set(LIBRARY_VERSION "${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}")
set(LIBRARY_SOVERSION "${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}")
set(ORCANIA_VERSION_REQUIRED "2.2.1")

# cmake modules

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)

include(GNUInstallDirs)
include(CheckSymbolExists)

# check if _GNU_SOURCE is available

if (NOT _GNU_SOURCE)
    check_symbol_exists(__GNU_LIBRARY__ "features.h" _GNU_SOURCE)

    if (NOT _GNU_SOURCE)
        unset(_GNU_SOURCE CACHE)
        check_symbol_exists(_GNU_SOURCE "features.h" _GNU_SOURCE)
    endif ()
endif ()

if (_GNU_SOURCE)
    add_definitions(-D_GNU_SOURCE)
endif ()

# directories and source

set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${INC_DIR})

set(LIB_SRC
        ${INC_DIR}/yder.h # allow many IDEs to find and edit it
        ${SRC_DIR}/yder.c)

# dependencies

option(WITH_JOURNALD "Check journald." ON)

if (WITH_JOURNALD)
  include(FindSystemd)
  find_package(Systemd REQUIRED)
  if (SYSTEMD_FOUND)
      set(SYSTEMD_LIBRARIES systemd)
      include_directories(${SYSTEMD_INCLUDE_DIRS})
      set(Y_DISABLE_JOURNALD OFF)
  endif ()
else()
  set(Y_DISABLE_JOURNALD ON)
endif ()

# shared library

add_library(yder SHARED ${LIB_SRC})
if (NOT MSVC)
    set_target_properties(yder PROPERTIES
            COMPILE_OPTIONS -Wextra
            PUBLIC_HEADER "${INC_DIR}/yder.h;${PROJECT_BINARY_DIR}/yder-cfg.h"
            VERSION "${LIBRARY_VERSION}"
            SOVERSION "${LIBRARY_SOVERSION}")
endif()
if (WIN32)
    set_target_properties(yder PROPERTIES SUFFIX "-${LIBRARY_VERSION_MAJOR}.dll")
endif ()

# static library

option(BUILD_STATIC "Build static library." OFF)

if (BUILD_STATIC)
    add_library(yder_static STATIC ${LIB_SRC})
    target_compile_definitions(yder_static PUBLIC -DO_STATIC_LIBRARY)
    set_target_properties(yder_static PROPERTIES
            OUTPUT_NAME yder)
endif ()

option(DOWNLOAD_DEPENDENCIES "Download required dependencies" ON)

option(SEARCH_ORCANIA "Search for Orcania library" ON)
if (SEARCH_ORCANIA)
    set(Orcania_FIND_QUIETLY ON) # force to find Orcania quietly
    include(FindOrcania)
    find_package(Orcania ${ORCANIA_VERSION_REQUIRED} QUIET) # try to find orcania
    if (NOT ORCANIA_FOUND)
        if (DOWNLOAD_DEPENDENCIES)
            include(DownloadProject)
            download_project(PROJ orcania # ... otherwise, download archive
                URL "https://github.com/babelouest/orcania/archive/v${ORCANIA_VERSION_REQUIRED}.tar.gz"
                QUIET)
            add_subdirectory(${orcania_SOURCE_DIR} ${orcania_BINARY_DIR})
            include_directories(${orcania_SOURCE_DIR}/include)
            include_directories(${orcania_BINARY_DIR})
            add_dependencies(yder orcania)
            set(ORCANIA_LIBRARIES orcania)
            set(LIBS ${LIBS} ${ORCANIA_LIBRARIES})
        else ()
            message( FATAL_ERROR "Orcania not found")
        endif ()
    else()
        set(LIBS ${LIBS} ${ORCANIA_LIBRARIES})
        include_directories(${ORCANIA_INCLUDE_DIRS})
        include_directories(${orcania_BINARY_DIR})
    endif ()
endif ()

target_link_libraries(yder ${LIBS} ${ORCANIA_LIBRARIES} ${SYSTEMD_LIBRARIES})

if (WITH_JOURNALD)
  set(PKGCONF_REQ "")
  set(PKGCONF_REQ_PRIVATE "libsystemd, liborcania")
else ()
  set(PKGCONF_REQ "")
  set(PKGCONF_REQ_PRIVATE "liborcania")
endif ()

# documentation

option(BUILD_YDER_DOCUMENTATION "Build the documentation." OFF)
if (BUILD_YDER_DOCUMENTATION)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doc/doxygen.cfg)
        set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)
        
        configure_file(${doxyfile_in} ${doxyfile} @ONLY)
        
        add_custom_target(doc
                          COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile_in}
                          WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                          COMMENT "Generating documentation with Doxygen"
                          VERBATIM)

    else ()
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif ()
endif ()

# build yder-cfg.h file
configure_file(${INC_DIR}/yder-cfg.h.in ${PROJECT_BINARY_DIR}/yder-cfg.h)
set (CMAKE_EXTRA_INCLUDE_FILES ${PROJECT_BINARY_DIR})
include_directories(${PROJECT_BINARY_DIR})

# tests

option(BUILD_YDER_TESTING "Build the testing tree." OFF) # because we don not use include(CTest)

if (BUILD_YDER_TESTING)
    include(FindCheck)
    find_package(Check)
    if (CHECK_FOUND)
        if (NOT WIN32 AND NOT APPLE)
            include(FindSubunit)
            find_package(Subunit REQUIRED)
        endif ()

        enable_testing()

        set(CMAKE_CTEST_COMMAND ctest -V)

        set(TST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
        set(LIBS yder ${LIBS} ${CHECK_LIBRARIES} ${ORCANIA_LIBRARIES})
        if (NOT WIN32)
            find_package(Threads REQUIRED)
            set(LIBS ${LIBS} ${CMAKE_THREAD_LIBS_INIT} m)
        endif ()
        if (NOT APPLE)
            set(LIBS ${LIBS} rt)
        endif ()
        if (NOT WIN32 AND NOT APPLE)
            set(LIBS ${LIBS} ${SUBUNIT_LIBRARIES})
        endif ()

        set(TESTS yder_test)

        configure_file(
                "${CMAKE_MODULE_PATH}/CTestCustom.cmake.in"
                "${CMAKE_CURRENT_BINARY_DIR}/CTestCustom.cmake"
                @ONLY)

        foreach (t ${TESTS})
            add_executable(${t} EXCLUDE_FROM_ALL ${TST_DIR}/${t}.c)
            target_include_directories(${t} PUBLIC ${TST_DIR})
            target_link_libraries(${t} PUBLIC ${LIBS})
            add_test(NAME ${t}
                    WORKING_DIRECTORY ${TST_DIR}
                    COMMAND ${t})
        endforeach ()
    endif ()
endif ()

# install target

option(INSTALL_HEADER "Install the header files" ON) # Install yder.h or not

configure_file(libyder.pc.in libyder.pc @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libyder.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

set(TARGETS yder)
if (BUILD_STATIC)
    set(TARGETS ${TARGETS} yder_static)
endif ()

if (INSTALL_HEADER)
    install(TARGETS ${TARGETS}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(DIRECTORY examples/
            DESTINATION ${CMAKE_INSTALL_DOCDIR}/examples/ COMPONENT runtime)
    install(FILES README.md
            DESTINATION ${CMAKE_INSTALL_DOCDIR} COMPONENT runtime)
else ()
    install(TARGETS ${TARGETS}
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif ()

# uninstall target

if (NOT TARGET uninstall)
    configure_file(
            "${CMAKE_MODULE_PATH}/CMakeUninstall.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
            IMMEDIATE @ONLY)
    add_custom_target(uninstall
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif ()

# packaging

set(CPACK_PACKAGE_VERSION_MAJOR ${LIBRARY_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${LIBRARY_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${LIBRARY_VERSION_PATCH})

if (INSTALL_HEADER)
    set(PACKAGE_FILE_NAME
            "lib${CMAKE_PROJECT_NAME}-dev_${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
else ()
    set(PACKAGE_FILE_NAME
            "lib${CMAKE_PROJECT_NAME}_${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")
endif ()
set(PACKAGE_IGNORED_FILES
        "${CMAKE_CURRENT_BINARY_DIR}/;/.git/;.gitignore;~$;${CPACK_SOURCE_IGNORE_FILES}")

set(CPACK_PACKAGE_NAME "libyder")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Logging library for C applications")
set(CPACK_GENERATOR "TGZ;DEB")
set(CPACK_PACKAGE_VERSION_MAJOR ${LIBRARY_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${LIBRARY_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${LIBRARY_VERSION_PATCH})

option(BUILD_RPM "Build a RPM for your system" OFF)
if (BUILD_RPM)
  set(CPACK_GENERATOR "TGZ;DEB;RPM")
  set(CPACK_RPM_PACKAGE_LICENSE "LGPL")
  set(CPACK_RPM_PACKAGE_URL "http://babelouest.github.io/yder/")
endif ()

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "mail@babelouest.org")

set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.4), liborcania (>= ${ORCANIA_VERSION_REQUIRED})")
if (INSTALL_HEADER)
  if (WITH_SYSTEMD)
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}, libsystemd-dev")
  endif ()
else ()
  if (WITH_SYSTEMD)
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "${CPACK_DEBIAN_PACKAGE_DEPENDS}, libsystemd0")
  endif ()
endif ()

set(CPACK_PACKAGE_FILE_NAME ${PACKAGE_FILE_NAME})

set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${PACKAGE_FILE_NAME})
set(CPACK_SOURCE_IGNORE_FILES ${PACKAGE_IGNORED_FILES})

include(CPack)

add_custom_target(dist_y
        COMMAND ${CMAKE_MAKE_PROGRAM} package_source)

message(STATUS "Journald support: ${WITH_JOURNALD}")
message(STATUS "Build static library: ${BUILD_STATIC}")
message(STATUS "Build testing tree: ${BUILD_YDER_TESTING}")
message(STATUS "Install the header files: ${INSTALL_HEADER}")
message(STATUS "Build RPM package: ${BUILD_RPM}") 
message(STATUS "Build documentation: ${BUILD_YDER_DOCUMENTATION}")
