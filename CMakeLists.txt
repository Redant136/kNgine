cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)


if(APPLE)
  include_directories ( /usr/local/include) 
  link_directories( /usr/local/lib)
  set(OSSpecificFiles
    src/kNgine/AudioEngineNSSound.mm
  )
else()
  if(WIN32)
    add_compile_definitions(_USE_MATH_DEFINES)
  endif()
  set(OSSpecificFiles
    src/kNgine/AudioEngineOpenAL.cpp
  )
endif()

option(kRenderer_GLFW "sets if you link kRenderer GLFW" ON)
option(kRenderer_GLUT "sets if you link kRenderer GLUT (glut has problems on macos)" OFF)

option(kNgine_includeOpenAL "if openal is to be comiled and linked or not" ON)

if(kRenderer_GLFW)
set(kRendererSource "src/kFramework/kRendererGLFW.c")
elseif(kRenderer_GLUT)
set(kRendererSource "src/kFramework/kRendererGLUT.cpp")
else()
set(kRendererSource "src/kFramework/kRendererSoftwareRenderer.c")
endif()

set(KNGINEFRAMEWORK_HEADER_FILES 
src/kFramework/utils.h
src/kFramework/kThread.h
src/kFramework/kRenderer.h
)

set(KNGINEFRAMEWORK_SOURCE_FILES
${kRendererSource}
)

set(KNGINE_HEADER_FILES
${KNGINEFRAMEWORK_HEADER_FILES}

src/kNgine/EngineObjects.hpp
src/kNgine/Camera.hpp
src/kNgine/SpriteUtils.hpp
src/kNgine/PhysicsB2.hpp
src/kNgine/AudioEngine.hpp

src/kNgine/Engine.hpp
)

set(KNGINE_SOURCE_FILES
${KNGINEFRAMEWORK_SOURCE_FILES}

src/kNgine/EngineObjects.cpp 
src/kNgine/Camera_2D.cpp
src/kNgine/SpriteUtils.cpp
src/kNgine/PhysicsB2.cpp
src/kNgine/Tilemap.cpp
src/kNgine/Engine.cpp
${OSSpecificFiles}
)

add_library(kNgineFramework
  ${KNGINEFRAMEWORK_SOURCE_FILES}
  ${KNGINEFRAMEWORK_HEADER_FILES}
)

add_library(kNgine
  ${KNGINE_SOURCE_FILES}
  ${KNGINE_HEADER_FILES}
)


if(kRenderer_GLFW)
  add_subdirectory(extern/glad/)
  target_link_libraries(kNgineFramework PRIVATE glad)
  target_link_libraries(kNgine PRIVATE glad)

  add_subdirectory(extern/glfw)
  target_link_libraries(kNgineFramework PRIVATE glfw)
  target_link_libraries(kNgine PRIVATE glfw)

elseif(kRenderer_GLUT)
  add_subdirectory(extern/glad/)
  target_link_libraries(kNgineFramework PRIVATE glad)
  target_link_libraries(kNgine PRIVATE glad)

  if(APPLE)
    target_link_libraries(kNgine PRIVATE "-framework OpenGL -framework GLUT")

  else()

  add_subdirectory(extern/FreeGlut/freeglut/freeglut)
  target_include_directories(kNgineFramework PRIVATE extern/FreeGlut/freeglut/freeglut/include)
  target_include_directories(kNgine PRIVATE extern/FreeGlut/freeglut/freeglut/include)

  target_link_libraries(kNgineFramework PRIVATE ${OPENGL_LIBRARIES} freeglut})
  # target_link_libraries(kNgine PRIVATE freeglut)
  target_link_libraries(kNgine PRIVATE ${OPENGL_LIBRARIES} freeglut)
endif()

else()
endif()

add_subdirectory(extern/freetype/)
target_link_libraries(kNgineFramework PRIVATE freetype)
target_link_libraries(kNgine PRIVATE freetype)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/src/ressources/ DESTINATION ${CMAKE_BINARY_DIR}/ressources/)

set(BOX2D_BUILD_UNIT_TESTS OFF)
set(BOX2D_BUILD_TESTBED OFF)
add_subdirectory(extern/box2d)
target_link_libraries(kNgine PRIVATE box2d)

set(BUILD_DEMOS OFF)
set(INSTALL_DEMOS OFF)
set(BUILD_SHARED OFF)
set(BUILD_STATIC ON)
set(INSTALL_STATIC OFF)
add_subdirectory(extern/chipmunk2D)
target_include_directories(kNgine PRIVATE extern/Chipmunk2D/include)
target_link_libraries(kNgine PRIVATE chipmunk_static)

add_subdirectory(extern/AudioFile)
target_link_libraries(kNgine PRIVATE AudioFile)

if(APPLE)
  target_link_directories(kNgine PUBLIC "/usr/local/lib")
  target_link_libraries(kNgine PRIVATE "-framework Foundation -framework AppKit -framework CoreAudio -pthread")
elseif(WIN32)
  if(kNgine_includeOpenAL)
    set(LIBTYPE STATIC)
    add_subdirectory(extern/openal-soft)
    target_link_libraries(kNgine PRIVATE OpenAL)
  endif()
elseif(UNIX)
  if(kNgine_includeOpenAL)
    set(LIBTYPE STATIC)
    add_subdirectory(extern/openal-soft)
    target_link_libraries(kNgine PRIVATE OpenAL)
  endif()

  target_link_libraries(kNgine PRIVATE "-lGL -ldl -lpthread")
endif()

target_include_directories(kNgineFramework PUBLIC include/)
target_include_directories(kNgineFramework PUBLIC src/kFramework)

target_include_directories(kNgine PUBLIC include/)
target_include_directories(kNgine PUBLIC src/kFramework)
target_include_directories(kNgine PUBLIC src/kNgine)


