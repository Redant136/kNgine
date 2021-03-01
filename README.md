# kNgine
c++ game engine
https://github.com/Redant136/kNgine.git

# Installation

- run git submodule init from kNgine folder, then git submodule update
- install openAl, if using linux install glfw
- add using cmake ``add_subdirectory(kNgine)``, ``target_link_libraries([PROJECT NAME] PRIVATE kNgine)``

# Usage

- Create engine object and add pointer to objects to it
- add a camera
- use engine start
- call cleanup

## object types
- EngineObject: the basis of all objects
- GameObject: object with position
- ComponentGameObject: game object able to hold components or modules
- ObjectComponent: object able to be attached to a CompGameObj
- ParentObject: Object to which you can attach others, this binds many objects to one location
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
- b2PhysicsBodyComponent: when adding a physics body something to an object
- b2PhysicsEngine: has to be included to use physics, handles all physics setup
- b2Rect: rectangular body
### Custom phys engine is deprecated but should still work
## Tilesets
- Tileset: object to hold a tileset/tilemap
- Tileset::TileDef : a definition of a tile. add a tile using the position in the array of it's definition
## Events
- an event contains a name, a pointer to the engine objects(use engine.workingObjects), a function as ``(std::vector<EngineObject*>){}``
- addEvent(Event)
- callEvent(name)
