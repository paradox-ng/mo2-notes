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

find_package(mo2-uibase CONFIG REQUIRED)

target_link_libraries(mo2_notes PUBLIC qmarkdowntextedit mo2::uibase)
mo2_configure_plugin(
        mo2_notes
        WARNINGS OFF
        PRIVATE_DEPENDS
        boost
        Qt::WebEngineWidgets
)
mo2_install_plugin(mo2_notes)
