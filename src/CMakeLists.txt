cmake_minimum_required(VERSION 3.30)
include(FetchContent)
add_library(mo2_notes SHARED)
set_property(TARGET mo2_notes PROPERTY CXX_STANDARD 20)

set(CMAKE_AUTORCC ON) # Will make the compiler read QRC file and skip the JS stuff.

set(BUILD_QMARKDOWNTEXTEDIT_EXECUTABLES OFF CACHE BOOL "Build executables" FORCE)
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

target_link_libraries(mo2_notes PUBLIC qmarkdowntextedit)

# Link mo2::uibase if found via find_package (CI), otherwise mo2_configure_plugin provides it
if(TARGET mo2::uibase)
    target_link_libraries(mo2_notes PUBLIC mo2::uibase)
    # In CI, add the uibase subdirectory to includes for <iplugin.h> style includes
    get_target_property(_uibase_includes mo2::uibase INTERFACE_INCLUDE_DIRECTORIES)
    foreach(_inc ${_uibase_includes})
        if(EXISTS "${_inc}/uibase")
            target_include_directories(mo2_notes PUBLIC "${_inc}/uibase")
        endif()
    endforeach()
endif()

mo2_configure_plugin(
        mo2_notes
        WARNINGS OFF
        PRIVATE_DEPENDS
        boost
        Qt::WebEngineWidgets
)

# Handle both old (mo2_install_target) and new (mo2_install_plugin) function names
if(COMMAND mo2_install_plugin)
    mo2_install_plugin(mo2_notes)
elseif(COMMAND mo2_install_target)
    mo2_install_target(mo2_notes)
endif()
