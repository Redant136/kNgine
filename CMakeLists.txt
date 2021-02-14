if(APPLE)
  include_directories ( /usr/local/include) 
  link_directories( /usr/local/lib)
  SET(GCC_LINK_FLAGS "-framework Foundation -framework OpenGL -framework AppKit -lglfw -pthread")
else()
  SET(GCC_LINK_FLAGS "-pthread -lGL -lglfw -ldl -lopenal -lalut")
endif()
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_LINK_FLAGS}")


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
  ${OSSpecificFiles}
  src/EngineObjects.cpp 
  src/Physics_2D.cpp
  src/PhysicsB2.cpp
  src/Camera_2D.cpp
  src/TextBox.cpp
  src/SpriteUtils.cpp
)
add_library(kNgine
  ${KNGINE_SOURCE_FILES}
)

add_subdirectory(extern/box2d)
target_link_libraries(kNgine PRIVATE box2d)

add_subdirectory(extern/glad/)
target_link_libraries(kNgine PRIVATE kNgine_glad)

target_include_directories(kNgine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

