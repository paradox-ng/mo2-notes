cmake_minimum_required(VERSION 3.30)
include(FetchContent)
add_library(mo2_notes SHARED)
set_property(TARGET mo2_notes PROPERTY CXX_STANDARD 20)

set(CMAKE_AUTORCC ON) # Will make the compiler read QRC file and skip the JS stuff.

# QMarkdownTextEdit demo executable - enable if you want to test the editor standalone
set(BUILD_QMARKDOWNTEXTEDIT_EXECUTABLES OFF CACHE BOOL "Build QMarkdownTextEdit demo executable")
FetchContent_Declare(
        QMarkdownTextEdit
        GIT_REPOSITORY https://github.com/pbek/QMarkdownTextEdit.git
        GIT_TAG main
)
FetchContent_MakeAvailable(QMarkdownTextEdit)

set_source_files_properties(
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/marked.min.js.txt
        PROPERTIES HEADER_FILE_ONLY TRUE
)

target_include_directories(
        mo2_notes
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PRIVATE ${qmarkdowntextedit_SOURCE_DIR}
)

if (MSVC)
    target_compile_options(
            mo2_notes
            PRIVATE
            /bigobj # increase number of sections in .obj file
            /W4     # level 4 warnings
            /WX     # treat all compiler warnings as errors
            /wd4201 # nonstandard extension used: nameless struct/union
            /wd4458 # declaration of 'identifier' hides class member
    )
endif ()

# In CI, mo2-uibase is installed separately. Locally it's provided by mo2.cmake
find_package(mo2-uibase CONFIG QUIET)

# Find Qt WebEngine and Boost explicitly
find_package(Qt6 REQUIRED COMPONENTS WebEngineWidgets)
find_package(Boost REQUIRED)

target_link_libraries(mo2_notes 
        PUBLIC 
        qmarkdowntextedit
        Qt6::WebEngineWidgets
        Boost::headers
)

# Link to uibase if found (CI environment)
if(TARGET mo2::uibase)
    target_link_libraries(mo2_notes PUBLIC mo2::uibase)
    message(STATUS "Linked mo2::uibase")
endif()

# In CI, add uibase include directory for <iplugin.h> style includes
# The headers are at CMAKE_INSTALL_PREFIX/include/uibase/
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

foreach(_prefix ${CMAKE_PREFIX_PATH})
    message(STATUS "Checking prefix: ${_prefix}")
    if(EXISTS "${_prefix}/include/uibase")
        target_include_directories(mo2_notes PUBLIC "${_prefix}/include/uibase")
        message(STATUS "Added uibase include: ${_prefix}/include/uibase")
    endif()
    # Also check parent directory (for install/lib/cmake -> install/include/uibase)
    get_filename_component(_parent "${_prefix}" DIRECTORY)
    get_filename_component(_grandparent "${_parent}" DIRECTORY)
    if(EXISTS "${_grandparent}/include/uibase")
        target_include_directories(mo2_notes PUBLIC "${_grandparent}/include/uibase")
        message(STATUS "Added uibase include from grandparent: ${_grandparent}/include/uibase")
    endif()
endforeach()

# Also check CMAKE_INSTALL_PREFIX
if(EXISTS "${CMAKE_INSTALL_PREFIX}/include/uibase")
    target_include_directories(mo2_notes PUBLIC "${CMAKE_INSTALL_PREFIX}/include/uibase")
    message(STATUS "Added uibase include from INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}/include/uibase")
endif()

mo2_configure_plugin(
        mo2_notes
        WARNINGS OFF
)

# Handle both old (mo2_install_target) and new (mo2_install_plugin) function names
if(COMMAND mo2_install_plugin)
    mo2_install_plugin(mo2_notes)
elseif(COMMAND mo2_install_target)
    mo2_install_target(mo2_notes)
endif()

# === Output to artifacts directory ===
# mo2_configure_plugin handles translation compilation automatically
# The .qm file is output to CMAKE_CURRENT_BINARY_DIR
set(QM_FILE "${CMAKE_CURRENT_BINARY_DIR}/mo2_notes_en.qm")

# Copy plugin and translations after build
add_custom_command(TARGET mo2_notes POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${MO2_PLUGIN_TARGET_DIR}/plugins"
    COMMAND ${CMAKE_COMMAND} -E make_directory "${MO2_PLUGIN_TARGET_DIR}/translations"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:mo2_notes>"
        "${MO2_PLUGIN_TARGET_DIR}/plugins/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${QM_FILE}"
        "${MO2_PLUGIN_TARGET_DIR}/translations/"
    COMMENT "Copying plugin and translations to ${MO2_PLUGIN_TARGET_DIR}/"
)

# Install targets (for cmake --install)
install(TARGETS mo2_notes RUNTIME DESTINATION "${MO2_PLUGIN_TARGET_DIR}/plugins")
install(FILES "${QM_FILE}" DESTINATION "${MO2_PLUGIN_TARGET_DIR}/translations")
