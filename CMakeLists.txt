cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_LIST_DIR}/install")

foreach(_var IN ITEMS DEPENDENCIES_DIR QT_ROOT PYTHON_ROOT)
	if (NOT DEFINED ${_var} AND DEFINED ENV{${_var}})
		set(${_var} $ENV{${_var}})
	endif ()
endforeach()

if (DEFINED BOOST_ROOT AND NOT EXISTS "${BOOST_ROOT}/include/boost")
	message(WARNING "BOOST_ROOT='${BOOST_ROOT}' does not contain Boost headers; resetting.")
	unset(BOOST_ROOT CACHE)
	unset(BOOST_ROOT)
endif ()

if (NOT DEFINED MO2_PLUGIN_TARGET_DIR AND DEFINED ENV{MO2_PLUGIN_TARGET_DIR})
	set(MO2_PLUGIN_TARGET_DIR $ENV{MO2_PLUGIN_TARGET_DIR})
endif ()

if (DEFINED MO2_PLUGIN_TARGET_DIR)
	message(STATUS "Using MO2_PLUGIN_TARGET_DIR: ${MO2_PLUGIN_TARGET_DIR}")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${MO2_PLUGIN_TARGET_DIR})
else ()
	message(STATUS "Using default install path inside the repository")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_INSTALL_PREFIX}/bin/plugins)
endif ()

if (NOT DEFINED VCPKG_INSTALLED_DIR)
	set(VCPKG_INSTALLED_DIR "${CMAKE_CURRENT_LIST_DIR}/vcpkg_installed")
endif ()

if (NOT DEFINED BOOST_ROOT AND DEFINED ENV{BOOST_ROOT})
	set(BOOST_ROOT $ENV{BOOST_ROOT})
endif ()

if (NOT DEFINED BOOST_ROOT AND DEFINED VCPKG_TARGET_TRIPLET)
	set(BOOST_ROOT "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}")
endif ()

set(BUILD_QMARKDOWNTEXTEDIT_EXECUTABLES OFF CACHE BOOL "Build executables" FORCE)
set(QMARKDOWNTEXTEDIT_INSTALL OFF CACHE BOOL "Install library" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "Build tests" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi /Od")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Zi /O2")

set(_mo2_cmake "")
if (DEFINED DEPENDENCIES_DIR)
	if (EXISTS "${DEPENDENCIES_DIR}/cmake_common/mo2.cmake")
		set(_mo2_cmake "${DEPENDENCIES_DIR}/cmake_common/mo2.cmake")
	elseif (EXISTS "${DEPENDENCIES_DIR}/modorganizer_super/cmake_common/mo2.cmake")
		set(_mo2_cmake "${DEPENDENCIES_DIR}/modorganizer_super/cmake_common/mo2.cmake")
	endif ()
endif ()

# Fallback: check sibling directory (CI environment)
if (NOT _mo2_cmake AND EXISTS "${CMAKE_CURRENT_LIST_DIR}/../cmake_common/mo2.cmake")
	set(_mo2_cmake "${CMAKE_CURRENT_LIST_DIR}/../cmake_common/mo2.cmake")
endif ()

if (NOT _mo2_cmake)
	message(FATAL_ERROR "Set DEPENDENCIES_DIR to your Mod Organizer 2.5.2 source tree (folder containing cmake_common/mo2.cmake).")
endif ()

include(${_mo2_cmake})

project(mo2_notes)
set(project_type plugin)
set(enable_warnings OFF)

add_subdirectory(src)
