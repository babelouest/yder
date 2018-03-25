#.rst:
# FindSystemd
# -----------
#
# Find Systemd
#
# Find Systemd headers and libraries.
#
# ::
#
#   SYSTEMD_FOUND          - True if Systemd found.
#   SYSTEMD_INCLUDE_DIRS   - Where to find orcania.h.
#   SYSTEMD_LIBRARIES      - List of libraries when using Systemd.

#=============================================================================
# Copyright 2018 Silvio Clecio <silvioprog@gmail.com>
# Copyright 2018 Nicolas Mora <mail@babelouest.org>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation;
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU GENERAL PUBLIC LICENSE for more details.
#
# You should have received a copy of the GNU General Public
# License along with this library.	If not, see <http://www.gnu.org/licenses/>.
#=============================================================================

# Sat Jan 27 15:57:36 -03 2018

find_package(PkgConfig QUIET)
pkg_check_modules(PC_SYSTEMD QUIET systemd)

find_path(SYSTEMD_INCLUDE_DIR
        NAMES systemd/_sd-common.h
        HINTS ${PC_SYSTEMD_INCLUDEDIR} ${PC_SYSTEMD_INCLUDE_DIRS})

find_library(SYSTEMD_LIBRARY
        NAMES systemd libsystemd
        HINTS ${PC_SYSTEMD_LIBDIR} ${PC_SYSTEMD_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Systemd
        REQUIRED_VARS SYSTEMD_LIBRARY SYSTEMD_INCLUDE_DIR)

if (SYSTEMD_FOUND)
    set(SYSTEMD_LIBRARIES ${SYSTEMD_LIBRARY})
    set(SYSTEMD_INCLUDE_DIRS ${SYSTEMD_INCLUDE_DIR})
endif ()
mark_as_advanced(SYSTEMD_INCLUDE_DIR SYSTEMD_LIBRARY)
