cmake_minimum_required(VERSION 3.15)
project(IGSE)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
#set(CMAKE_CXX_FLAGS "-Wall -Wextra")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(IGSE_SRC_ROOT src)
set(IGSE_INCLUDES_ROOT include)
set(IGSE_LIBS_ROOT libs)
set(IGSE_EXAMPLES_ROOT examples)

set(IMGUI_ROOT ${IGSE_LIBS_ROOT}/imgui)
set(GLM_ROOT ${IGSE_LIBS_ROOT}/glm)
set(GL3W_ROOT ${IGSE_LIBS_ROOT}/gl3w)
set(GLFW_ROOT ${IGSE_LIBS_ROOT}/glfw)
set(STB_ROOT ${IGSE_LIBS_ROOT}/stb)

include_directories(${IGSE_SRC_ROOT})
include_directories(${IGSE_INCLUDES_ROOT})
include_directories(${IGSE_EXAMPLES_ROOT})
include_directories(${IMGUI_ROOT})
include_directories(${IMGUI_ROOT}/examples)
include_directories(${GLM_ROOT})
include_directories(${GL3W_ROOT}/include)
include_directories(${GLFW_ROOT}/include)
include_directories(${STB_ROOT})

add_subdirectory(${GLM_ROOT})
add_subdirectory(${GL3W_ROOT})
add_subdirectory(${GLFW_ROOT})

set(IMGUI_SRC
    ${IMGUI_ROOT}/imgui.cpp
    ${IMGUI_ROOT}/imgui.h
    ${IMGUI_ROOT}/imgui_demo.cpp
    ${IMGUI_ROOT}/imgui_draw.cpp
    ${IMGUI_ROOT}/imgui_widgets.cpp
    ${IMGUI_ROOT}/imgui_internal.h
    ${IMGUI_ROOT}/imconfig.h
    ${IMGUI_ROOT}/imstb_rectpack.h
    ${IMGUI_ROOT}/imstb_textedit.h
    ${IMGUI_ROOT}/imstb_truetype.h
    ${IMGUI_ROOT}/examples/imgui_impl_opengl3.cpp
    ${IMGUI_ROOT}/examples/imgui_impl_opengl3.h
    ${IMGUI_ROOT}/examples/imgui_impl_glfw.cpp
    ${IMGUI_ROOT}/examples/imgui_impl_glfw.h
    )

set(GL3W_SRC
    ${GL3W_ROOT}/src/gl3w.c
    )

set(IGSE_HEADERS
    ${IGSE_INCLUDES_ROOT}/gui.h
    ${IGSE_INCLUDES_ROOT}/pixel.h
    ${IGSE_INCLUDES_ROOT}/mat.h
    ${IGSE_INCLUDES_ROOT}/gpuimage.h
    )

set(IGSE_SOURCES
    ${IGSE_SRC_ROOT}/gui.cpp
    )

set(IGSE_EXAMPLE_SOURCES
    ${IGSE_EXAMPLES_ROOT}/main.cpp
    ${IGSE_EXAMPLES_ROOT}/example.cpp
    ${IGSE_EXAMPLES_ROOT}/example.h
    )


add_library(${PROJECT_NAME}
            ${IGSE_SOURCES}
            ${IGSE_HEADERS}
            ${IMGUI_SRC}
            ${GL3W_SRC}
            )

add_executable(${PROJECT_NAME}_Example ${IGSE_EXAMPLE_SOURCES})

target_link_libraries(${PROJECT_NAME} PUBLIC glfw ${GLFW_LIBRARIES})

find_package(OpenMP REQUIRED)
if (OpenMP_CXX_FOUND)
  target_link_libraries(${PROJECT_NAME}_Example PUBLIC IGSE OpenMP::OpenMP_CXX)
else ()
  target_link_libraries(${PROJECT_NAME}_Example PUBLIC IGSE)
endif ()
