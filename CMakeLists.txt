#
# Yder library
#
# CMake file used to build all programs
#
# Copyright 2018 Silvio Clecio <silvioprog@gmail.com>
# Copyright 2018 Nicolas Mora <mail@babelouest.org>
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
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

# library info

set(PROJECT_DESCRIPTION "Logging library for C applications")
set(PROJECT_BUGREPORT_PATH "https://github.com/babelouest/yder/issues")

set(LIBRARY_VERSION_MAJOR "1")
set(LIBRARY_VERSION_MINOR "2")
set(LIBRARY_VERSION_PATCH "0")
set(LIBRARY_VERSION "${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}.${LIBRARY_VERSION_PATCH}")
set(LIBRARY_SOVERSION "${LIBRARY_VERSION_MAJOR}.${LIBRARY_VERSION_MINOR}")
set(ORCANIA_VERSION_DOWNLOAD "1.2.0")

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

# static library

option(BUILD_STATIC "Build static library." OFF)

if (BUILD_STATIC)
    add_library(yder_static STATIC ${LIB_SRC})
    target_compile_definitions(yder_static PUBLIC -DO_STATIC_LIBRARY)
    set_target_properties(yder_static PROPERTIES
            OUTPUT_NAME yder)
endif ()

# shared library

add_library(yder SHARED ${LIB_SRC})
set_target_properties(yder PROPERTIES
        COMPILE_OPTIONS -Wextra
        PUBLIC_HEADER ${INC_DIR}/yder.h
        VERSION "${LIBRARY_VERSION}"
        SOVERSION "${LIBRARY_SOVERSION}")
if (WIN32)
    set_target_properties(yder PROPERTIES SUFFIX "-${LIBRARY_VERSION_MAJOR}.dll")
endif ()

# dependencies

option(CHECK_ORCANIA "Check Orcania library." ON)

if (CHECK_ORCANIA)
	set(Orcania_FIND_QUIETLY ON) # force to find Orcania quietly
	include(FindOrcania)
	find_package(Orcania 1.1 QUIET) # try to find orcania
	if (NOT ORCANIA_FOUND)
			include(DownloadProject)
			download_project(PROJ orcania # ... otherwise, download archive
							URL "https://github.com/babelouest/orcania/archive/v${ORCANIA_VERSION_DOWNLOAD}.tar.gz"
							QUIET)
			add_subdirectory(${orcania_SOURCE_DIR} ${orcania_BINARY_DIR})
			include_directories(${orcania_SOURCE_DIR}/include)
			add_dependencies(yder orcania)
			set(ORCANIA_LIBRARIES orcania)
	endif ()
endif ()

target_link_libraries(yder ${LIBS} ${ORCANIA_LIBRARIES})

# tests

option(BUILD_TESTING "Build the testing tree." OFF) # because we don not use include(CTest)

if (BUILD_TESTING)
    include(FindCheck)
    find_package(Check)
    if (CHECK_FOUND)
        if (NOT WIN32)
            include(FindSubunit)
            find_package(Subunit REQUIRED)
        endif ()

        enable_testing()

        set(CMAKE_CTEST_COMMAND ctest -V)

        set(TST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
        set(LIBS yder ${LIBS} ${CHECK_LIBRARIES})
        if (NOT WIN32)
            find_package(Threads REQUIRED)
            set(LIBS ${LIBS} ${SUBUNIT_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} m rt)
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
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

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

set(CPACK_GENERATOR "TGZ;DEB")
set(CPACK_DEBIAN_PACKAGE_NAME "libyder")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "mail@babelouest.org")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION ${PROJECT_DESCRIPTION})
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/babelouest/yder")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.4), liborcania (>= 1.2.0)")
set(CPACK_PACKAGE_FILE_NAME ${PACKAGE_FILE_NAME})

set(CPACK_SOURCE_GENERATOR "TGZ")
set(CPACK_SOURCE_PACKAGE_FILE_NAME ${PACKAGE_FILE_NAME})
set(CPACK_SOURCE_IGNORE_FILES ${PACKAGE_IGNORED_FILES})

include(CPack)

add_custom_target(dist_y
        COMMAND ${CMAKE_MAKE_PROGRAM} package_source)
