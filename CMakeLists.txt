set(BOX2DPHYSICS ON)

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
src/EngineObjects.hpp
src/Camera.hpp
src/TextBox.hpp
src/SpriteUtils.hpp
src/Tilemap.hpp 
src/AudioEngine.hpp
src/Renderer.hpp
)

if(BOX2DPHYSICS)
  set(KNGINE_SOURCE_FILES
    ${KNGINE_SOURCE_FILES}
    src/PhysicsB2.cpp
  )
  set(KNGINE_HEADER_FILES
    ${KNGINE_HEADER_FILES}
    src/PhysicsB2.hpp
  )

else()
  set(KNGINE_SOURCE_FILES
    ${KNGINE_SOURCE_FILES}
    src/Physics_2D.cpp
  )
  set(KNGINE_HEADER_FILES
    ${KNGINE_HEADER_FILES}
    src/Physics.hpp
  )
endif()

add_library(kNgine
  ${KNGINE_SOURCE_FILES}
  ${KNGINE_HEADER_FILES}
)

if(BOX2DPHYSICS)
  set(BOX2D_BUILD_UNIT_TESTS OFF)
  set(BOX2D_BUILD_TESTBED OFF)
  add_subdirectory(extern/box2d)
  target_link_libraries(kNgine PRIVATE box2d)
endif()

add_subdirectory(extern/glfw)
target_link_libraries(kNgine PRIVATE glfw)

add_subdirectory(extern/glad/)
target_link_libraries(kNgine PRIVATE glad)

add_subdirectory(extern/glm)
target_link_libraries(kNgine PRIVATE glm)


if(APPLE)
  target_link_directories(kNgine PUBLIC "/usr/local/lib")
  target_link_libraries(kNgine PRIVATE "-framework Foundation -framework AppKit -framework CoreAudio -pthread")
else()
  add_subdirectory(extern/openal-soft)
  target_link_libraries(kNgine PRIVATE openal)

  add_subdirectory(extern/freealut)
  target_link_libraries(kNgine PRIVATE alut)

  target_link_libraries(kNgine PRIVATE "-pthread -lGL -ldl")
endif()

target_include_directories(kNgine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

