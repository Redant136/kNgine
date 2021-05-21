# kNgine
c++ game engine
https://github.com/Redant136/kNgine.git

# Installation

- run ``git submodule update --init --recursive --remote``
- if using linux install glfw
- add using cmake ``add_subdirectory(kNgine)``, ``target_link_libraries([PROJECT NAME] PRIVATE kNgine)``

# Usage

- add objects using kNgine::addObject()
- add a camera
- call kNgine::start()
- call cleanup

## object types
- EngineObject: the basis of all objects
- GameObject: object with position
- ComponentGameObject: game object able to hold components or modules
- ObjectComponent: object able to be attached to a CompGameObject
- NodeComponentObject: Component to add children to object
## Rendering
- Camera: renders sprites
- SpriteComponent: to bind image to object
- SpriteMap: an object to preload all sprites before rendering, speeds up rendering
- SpriteMapAccessor: NOT REAL COMPOENT, is extended by any component accessing spriteMap
- SpriteReferenceComponent: an component to access sprite map
- SpriteAnimation: an object that references multiple sprites, the sprite changes after determined amount of time
- SpriteAnimationSystem: contains multiple spriteMapAccessors and can switch active spriteAccessor when setActive is called
### Renderer
- can be used to draw objects on window, like rect or triangle with pixel location
## Audio
- SoundListenerComponent: Should only be added once, attach to main character
- SoundEmiterComponent: Component to be added to object emiting sound
- AudioEngine: object to play global sound or music
## Physics
- cpPhysicsBodyComponent: when adding a physics body something to an object
- cpPhysicsEngine: has to be included to use physics, handles all physics setup
- cpRect: rectangular body
## Tilesets
- Tileset: object to hold a tileset/tilemap
- Tileset::TileDef : a definition of a tile. add a tile using the position in the array of it's definition
## Events
- an event contains a name and a function as ``void*(void*arg){}``
- addEvent(Event)
- callEvent(name)
