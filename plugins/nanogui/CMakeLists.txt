cmake_minimum_required(VERSION 3.14)
# apt-get install cmake xorg-dev libglu1-mesa-dev
# mesa-libGLU-devel libXi-devel libXcursor-devel libXinerama-devel
# libXrandr-devel xorg-x11-server-devel

project(sq_nano LANGUAGES CXX C)

set(SOURCES_WITH 1)
include_directories(../../simplesquirrel/include
                    ../../simplesquirrel/source/
                    ../../simplesquirrel/libs/squirrel/include
                    ./nanogui/include
                    ./nanogui/build
                    ./nanogui/ext/eigen
                    ./nanogui/ext/nanovg/src
                    ./nanogui/include/nanogui
                    ./widgets)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BIN_NAME sq_nano)
SET(CMAKE_EXE_LINKER_FLAGS -rdynamic)

if(SOURCES_WITH)
FILE(GLOB NANO_SRCS
    ../nanogui/nanogui/src/button.cpp
    ../nanogui/nanogui/src/checkbox.cpp
    ../nanogui/nanogui/src/colorpicker.cpp
    ../nanogui/nanogui/src/colorwheel.cpp
    ../nanogui/nanogui/src/combobox.cpp
    ../nanogui/nanogui/src/common.cpp
    ../nanogui/nanogui/src/glcanvas.cpp
    ../nanogui/nanogui/src/glutil.cpp
    ../nanogui/nanogui/src/graph.cpp
    ../nanogui/nanogui/src/imagepanel.cpp
    ../nanogui/nanogui/src/imageview.cpp
    ../nanogui/nanogui/src/label.cpp
    ../nanogui/nanogui/src/layout.cpp
    ../nanogui/nanogui/src/messagedialog.cpp
    ../nanogui/nanogui/src/popupbutton.cpp
    ../nanogui/nanogui/src/popup.cpp
    ../nanogui/nanogui/src/progressbar.cpp
    ../nanogui/nanogui/src/screen.cpp
    ../nanogui/nanogui/src/serializer.cpp
    ../nanogui/nanogui/src/slider.cpp
    ../nanogui/nanogui/src/stackedwidget.cpp
    ../nanogui/nanogui/src/tabheader.cpp
    ../nanogui/nanogui/src/tabwidget.cpp
    ../nanogui/nanogui/src/textbox.cpp
    ../nanogui/nanogui/src/theme.cpp
    ../nanogui/nanogui/src/vscrollpanel.cpp
    ../nanogui/nanogui/src/widget.cpp
    ../nanogui/nanogui/src/window.cpp
    ../nanogui/nanogui/build/nanogui_resources.cpp
    ../nanogui/nanogui/ext/nanovg/src/stb_image.h
    ../nanogui/nanogui/ext/nanovg/src/nanovg.c
)
else(SOURCES_WITH)
    FILE(GLOB NANO_SRCS)
endif(SOURCES_WITH)


add_library(sq_nano SHARED
  sq_nano_global.h
  sq_nano.cpp
  sq_nano.h
  ${NANO_SRCS}
  ../../bin/nano.js
  ../../bin/nano_screen.js
 )

if(SOURCES_WITH)
    list(APPEND NANOGUI_EXTRA_LIBS GL Xxf86vm Xrandr Xinerama Xcursor Xi X11 pthread )
    if (NOT CMAKE_SYSTEM_NAME MATCHES "OpenBSD")
        list(APPEND NANOGUI_EXTRA_LIBS rt)
    endif()
    if(CMAKE_SYSTEM MATCHES "Linux")
        list(APPEND NANOGUI_EXTRA_LIBS dl)
    endif()
    TARGET_LINK_LIBRARIES(${BIN_NAME}  ${NANOGUI_EXTRA_LIBS}  -lglfw)
else()
    TARGET_LINK_LIBRARIES(${BIN_NAME} ${PROJECT_SOURCE_DIR}/lib/libnanogui.so)
endif(SOURCES_WITH)




set(LIB_FILENAME "lib${BIN_NAME}.so" )


## copy to bin
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${PROJECT_BINARY_DIR}/${LIB_FILENAME}
    ${PROJECT_SOURCE_DIR}/../../bin/${LIB_FILENAME}
)

