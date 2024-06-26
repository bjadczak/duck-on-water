cmake_minimum_required(VERSION 3.28)
project(DuckOnWater)

set(CMAKE_CXX_STANDARD 23)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always -Wall -g")

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmodules-ts")
endif()

# Shader failes
file(GLOB SHADER_FILES "${CMAKE_SOURCE_DIR}/shaders/*")

# Dear ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/dep/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)
set(IMGUI_SRC
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
)

set(APP_SRC
        src/Scene.cpp
        src/Scene.h
        src/Shader.cpp
        src/Shader.h
        src/VariantUtil.h
        src/mesh/Mesh.h
        src/mesh/is_vertex.h
        src/mesh/PositionNormalVertex.h
        src/glew_glfw.h
        src/camera/CameraAnchorFree.cpp
        src/camera/CameraAnchorFree.h
        src/input/InputHandler.cpp
        src/input/InputHandler.h
        src/gui/Gui.cpp
        src/gui/Gui.h
        src/app/AppContext.h
        src/framebufferManager/FrameBufferManager.cpp
        src/framebufferManager/FrameBufferManager.h
        src/importer/Importer.cpp
        src/importer/Importer.h
        src/importer/RawModel.h
        src/importer/Model.h
        src/room/Room.cpp
        src/room/Room.h
        src/light/PointLight.h
        src/skybox/Skybox.cpp
        src/skybox/Skybox.h
        src/mesh/PositionVertex.h
        src/mesh/EmptyVertex.h
        src/material/Material.h
        src/point/Point.h
        src/mesh/PosNorTexVertex.h
        dep/inc/stb/stb_image.h
        src/texture/Texture.cpp
        src/texture/Texture.h
        src/camera/baseCamera.cpp
        src/camera/baseCamera.h
        src/camera/CameraGameLike.cpp
        src/camera/CameraGameLike.h
        src/debug/DebugUI.h
        src/debug/DebugUI.cpp
        src/waterSurface/Water.cpp
        src/waterSurface/Water.h
        src/texture/Cubemap.cpp
        src/texture/Cubemap.h
        src/duck/Duck.cpp
        src/duck/Duck.h
        src/texture/common.h
        src/texture/common.cpp
)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/dep/inc)

# Link directories
link_directories(${CMAKE_SOURCE_DIR}/dep/lib)

# Copy shaders
file(COPY ${SHADER_FILES} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shaders)

# Add executables
add_executable(DuckOnWater
        # Dependencies
        ${IMGUI_SRC}
        ${CMAKE_SOURCE_DIR}/src/glad.c
        # Application
        ${CMAKE_SOURCE_DIR}/src/main.cpp
        ${APP_SRC}
)

# Link libraries
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")             # macOS
    target_link_libraries(DuckOnWater
            ${CMAKE_SOURCE_DIR}/dep/lib/libglfw.3.4.dylib
            ${CMAKE_SOURCE_DIR}/dep/lib/libGLEW.2.2.0.dylib
            "-framework OpenGL"
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
            "-framework CoreFoundation"
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")        # Windows
    find_package(OpenGL REQUIRED)
    include_directories( ${OPENGL_INCLUDE_DIRS})
    target_link_libraries(DuckOnWater
            ${CMAKE_SOURCE_DIR}/dep/lib/glfw3.lib
            ${CMAKE_SOURCE_DIR}/dep/lib/glew32.lib
            ${OPENGL_LIBRARIES}
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")          # Linux
    target_link_libraries(DuckOnWater
            -lglfw
            -lGLEW
            -lGL
            -lX11
            -lpthread
            -lXrandr
            -lXi
            -ldl
    )
endif()
