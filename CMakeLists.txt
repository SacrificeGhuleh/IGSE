cmake_minimum_required(VERSION 3.15)
project(ImGui_Simple_Engine)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(SRC_ROOT src/engine)
set(LIB_ROOT src/libs)
set(EXAMPLE_ROOT src/example)
set(IMGUI_ROOT ${LIB_ROOT}/imgui)
set(GLM_ROOT ${LIB_ROOT}/glm)
set(GL3W_ROOT ${LIB_ROOT}/gl3w)
set(GLFW_ROOT ${LIB_ROOT}/glfw)
set(STB_ROOT ${LIB_ROOT}/stb)

include_directories(${SRC_ROOT})
include_directories(${EXAMPLE_ROOT})
include_directories(${IMGUI_ROOT})
include_directories(${IMGUI_ROOT}/examples)
include_directories(${GLM_ROOT})
include_directories(${GL3W_ROOT}/include)
include_directories(${GLFW_ROOT}/include)
include_directories(${STB_ROOT})

add_subdirectory(${GLM_ROOT})
add_subdirectory(${GL3W_ROOT})
add_subdirectory(${GLFW_ROOT})

set(IMGUI
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

set(GL3W
    ${GL3W_ROOT}/src/gl3w.c
    )

set(SOURCE
    ${SRC_ROOT}/gui.cpp
    ${SRC_ROOT}/gui.h
    ${SRC_ROOT}/pixel.h
    ${SRC_ROOT}/mat.h
    ${SRC_ROOT}/gpuimage.h
    )

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(${PROJECT_NAME}
            ${SOURCE}
            ${IMGUI}
            ${GL3W}
            )

target_link_libraries(${PROJECT_NAME} PUBLIC glfw ${GLFW_LIBRARIES})

set(EXAMPLE
    ${EXAMPLE_ROOT}/main.cpp
    ${EXAMPLE_ROOT}/nxpcarinterface.cpp
    ${EXAMPLE_ROOT}/nxpcarinterface.h
    )

find_package(OpenMP REQUIRED)

add_executable(${PROJECT_NAME}_Example ${EXAMPLE})

if (OpenMP_CXX_FOUND)
  target_link_libraries(${PROJECT_NAME}_Example PUBLIC ImGui_Simple_Engine OpenMP::OpenMP_CXX)
else ()
  target_link_libraries(${PROJECT_NAME}_Example PUBLIC ImGui_Simple_Engine)
endif ()

