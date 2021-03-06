set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(APPLE)
  include_directories ( /usr/local/include) 
  link_directories( /usr/local/lib)
  set(OSSpecificFiles
    src/AudioEngineNSSound.mm
    src/RendererGLFW.cpp
  )
else()
  if(WIN32)
    add_compile_definitions(_USE_MATH_DEFINES)
  endif()
  set(OSSpecificFiles
    src/AudioEngineOpenAL.cpp
    src/RendererGLFW.cpp
  )
endif()

set(KNGINE_SOURCE_FILES
src/EngineObjects.cpp 
src/Camera_2D.cpp
src/TextBox.cpp
src/SpriteUtils.cpp
src/Tilemap.cpp 
${OSSpecificFiles}
)

set(KNGINE_HEADER_FILES
src/utils.h
src/kThread.h
src/EngineObjects.hpp
src/Camera.hpp
src/TextBox.hpp
src/SpriteUtils.hpp
src/Tilemap.hpp 
src/AudioEngine.hpp
src/Renderer.hpp
)

set(KNGINE_SOURCE_FILES
  ${KNGINE_SOURCE_FILES}
  src/PhysicsB2.cpp
)
set(KNGINE_HEADER_FILES
  ${KNGINE_HEADER_FILES}
  src/PhysicsB2.hpp
)

add_library(kNgine
  ${KNGINE_SOURCE_FILES}
  ${KNGINE_HEADER_FILES}
)

set(BOX2D_BUILD_UNIT_TESTS OFF)
set(BOX2D_BUILD_TESTBED OFF)
add_subdirectory(extern/box2d)
target_link_libraries(kNgine PRIVATE box2d)

add_subdirectory(extern/glfw)
target_link_libraries(kNgine PRIVATE glfw)

add_subdirectory(extern/glad/)
target_link_libraries(kNgine PRIVATE glad)

add_subdirectory(extern/glm)
target_link_libraries(kNgine PRIVATE glm)


if(APPLE)
  target_link_directories(kNgine PUBLIC "/usr/local/lib")
  target_link_libraries(kNgine PRIVATE "-framework Foundation -framework AppKit -framework CoreAudio")
elseif(WIN32)
  set(wave_enable_tests OFF)
  add_subdirectory(extern/wave)
  target_link_libraries(kNgine PRIVATE wave)

  add_subdirectory(extern/openal-soft)
  target_link_libraries(kNgine PRIVATE OpenAL)
   
  target_link_libraries(kNgine PRIVATE "-lGL -ldl")
elseif(UNIX)
  set(wave_enable_tests OFF)
  add_subdirectory(extern/wave)
  target_link_libraries(kNgine PRIVATE wave)

  add_subdirectory(extern/openal-soft)
  target_link_libraries(kNgine PRIVATE OpenAL)

  target_link_libraries(kNgine PRIVATE "-lGL -ldl -lopenal")
endif()

target_include_directories(kNgine PUBLIC include/)
target_include_directories(kNgine PUBLIC src/)


