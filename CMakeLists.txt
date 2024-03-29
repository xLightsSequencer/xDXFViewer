cmake_minimum_required(VERSION 3.5)

project(xDXFViewer VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set path to our custom CMAKE scripts
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

# Include Qt deployment helper function
include(windeployqt)

include(FetchContent)

FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG        v1.x
)

FetchContent_GetProperties(spdlog)
if (NOT spdlog_POPULATED)
    FetchContent_Populate(spdlog)
    add_subdirectory(${spdlog_SOURCE_DIR} ${spdlog_BINARY_DIR})
endif()

#set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport)

configure_file(src/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h)
configure_file(res/installer/xDXFViewer.iss.in ${CMAKE_CURRENT_SOURCE_DIR}/res/installer/xDXFViewer.iss)

file( GLOB_RECURSE BASE_SRC src/*cpp src/*h)
file( GLOB_RECURSE BASE_RES res/*ui res/*qrc)

file( GLOB_RECURSE DXFLIB_SRC dxflib/src/*cpp dxflib/src/*h)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
qt6_wrap_ui(BASE_SRC res/mainwindow.ui)
else()
qt5_wrap_ui(BASE_SRC res/mainwindow.ui)
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${BASE_SRC}
        ${BASE_RES}
        ${DXFLIB_SRC}
        res/xDXFViewer.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${BASE_SRC}
            ${BASE_RES}
            ${DXFLIB_SRC}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
            ${BASE_SRC}
            ${BASE_RES}
            ${DXFLIB_SRC}
            res/xDXFViewer.qrc
        )
    endif()
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER xdxf_viewer.xlights.org
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::PrintSupport spdlog::spdlog)

target_sources(${PROJECT_NAME} PRIVATE res/resource.rc)

if(WIN32)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

source_group(src FILES ${BASE_SRC})
source_group(res FILES ${BASE_RES})
source_group(dxflib FILES ${DXFLIB_SRC})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
 else()   
    if(WIN32)
        windeployqt(${PROJECT_NAME})
    endif()
    if(APPLE)
    message("macdeployqt")
            execute_process(
            COMMAND ${qt_root}/bin/macdeployqt
            ${CMAKE_INSTALL_PREFIX}/build/${PROJECT_NAME}.app
            -dmg
        )
    endif()
endif()

install(CODE "include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"${QT_PLUGINS}\" \"${DIRS}\")")

