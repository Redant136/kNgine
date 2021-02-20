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
src/PhysicsB2.cpp
src/Tilemap.cpp 
${OSSpecificFiles}
)

if(BOX2DPHYSICS)
  set(KNGINE_SOURCE_FILES
    ${KNGINE_SOURCE_FILES}
    src/PhysicsB2.cpp
  )
else()
  set(KNGINE_SOURCE_FILES
    ${KNGINE_SOURCE_FILES}
    src/Physics_2D.cpp
  )
endif()
add_library(kNgine
  ${KNGINE_SOURCE_FILES}
)

if(BOX2DPHYSICS)
  add_subdirectory(extern/box2d)
  target_link_libraries(kNgine PRIVATE box2d)
endif()

add_subdirectory(extern/glad/)
target_link_libraries(kNgine PRIVATE kNgine_glad)

if(APPLE)
  target_link_directories(kNgine PUBLIC "/usr/local/lib")
  target_link_libraries(kNgine PRIVATE "-framework Foundation -framework OpenGL -framework AppKit -framework CoreAudio -lglfw -pthread")
else()
  target_link_libraries(kNgine PRIVATE "-pthread -lGL -lglfw -ldl -lopenal -lalut")
endif()

target_include_directories(kNgine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

