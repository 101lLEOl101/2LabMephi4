cmake_minimum_required(VERSION 3.30)
project(2LabMephi4)

set(CMAKE_CXX_STANDARD 20)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Paths for IMGUI source files
set(IMGUI_SOURCE_DIR ${CMAKE_SOURCE_DIR}/imgui)
set(IMGUI_BACKENDS_DIR ${IMGUI_SOURCE_DIR}/backends)
set(IMGUI_LIBS_DIR ${IMGUI_SOURCE_DIR}/examples/libs)
set(GL3W_SOURCE_DIR ${IMGUI_LIBS_DIR}/gl3w)

# Source files
set(SOURCE_FILES
        ${IMGUI_SOURCE_DIR}/imgui.cpp
        ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
        ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
        ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
        ${IMGUI_SOURCE_DIR}/imgui_demo.cpp
        ${IMGUI_BACKENDS_DIR}/imgui_impl_glfw.cpp
        ${IMGUI_BACKENDS_DIR}/imgui_impl_opengl3.cpp
        ${GL3W_SOURCE_DIR}/GL/gl3w.c
        main.cpp
        visualisation.cpp
        visualisation.h
        Graph.cpp
        Graph.h
        test.cpp
        test.h
)

# Executable
add_executable(2LabMephi4 ${SOURCE_FILES})

# Include directories
target_include_directories(2LabMephi4 PRIVATE
        ${IMGUI_SOURCE_DIR}
        ${IMGUI_BACKENDS_DIR}
        ${GL3W_SOURCE_DIR}
)

# Link GLFW for macOS
if(APPLE)
    find_package(glfw3 REQUIRED) # Use Homebrew-installed GLFW
    target_link_libraries(2LabMephi4 PRIVATE OpenGL::GL glfw "-framework CoreFoundation")
else()
    # Windows/Linux settings
    set(GLFW_INCLUDE_DIR ${IMGUI_LIBS_DIR}/glfw/include)
    set(GLFW_LIB ${IMGUI_LIBS_DIR}/glfw/lib-vc2010-64/glfw3.lib)
    target_include_directories(2LabMephi4 PRIVATE ${GLFW_INCLUDE_DIR})
    target_link_libraries(2LabMephi4 PRIVATE OpenGL::GL ${GLFW_LIB})
endif()