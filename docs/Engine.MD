# Raindrop game engine architecture
###### Alexis Magnier
---

[TOC]

## Introduction

Raindrop is a game engine of mine. I spent the last few years trying to make a game engine that was easy to use, fast, modular. But it's very hard and this version is the long successor of multiple version and iterations.

To be honest, it's probably not the last version, but I will try to make this good enough for my standards.

## Architecture

The engine works on a layer-system base.
The engine class manages the main loop, system callbacks and layer handling. It's an application of the ECS pattern.

Each layer is linked to a set of components.
The systems can interact with the layers, read and update it's components.

As en example is better that a thousands words, here is an example : 
Let's say we are making a game of sort. We would like a gameplay layer and a HUD layer on top.

Both layers need components such as : 
 - Scene : They need to express what they render
 - Event : They both need to interact with events. As the HUD sits on top the gameplay. It would have a hiher priority level and would be able to stop event propagation. (A mouse click landing on a HUD button)
 - Render : They both need to be rendered on the window.
 - Asset : They both need to load and use assets
 - Audio : They need to output audio

And a lot more, but the point is that the engine is modular and can be extended with new layers and components.

However, these components are quite useless on their own. That's why we need systems.
Systems can interact with layers and apply effects based of it's component.

We could have : 
 - SceneSystem : manages scene updating and loading
 - EventSystem : handles event triggers and redirection
 - RenderSystem : will render the scene
 - AssetSystem : handles asset loading and management
 - ...

### Layers

The layer system works using an ECS registry. Each layer is referenced using a Runtime handle ID.
The layer components are linked to the Runtime handle ID of the layer and can be accesed via said ID.

By itself, the layer system is not very complexe but allows easy extensibility and implementation of complexe systems and managers.

### Systems

Systems are registered in a system manager and can be easily accessed via type. They are internaly stored using a `std::type_index` mapped to a pointer of the implementation.

```cpp
systems.get<RenderSystem>();
```

To ease access and facilitate access, tests and overrides. Systems should have an interface class. Like IRenderSystem or IAssetManager, and then implement them. This allows for dependent systems to still access the base system without having to know the implementation.

For exemple, it would be quite nice to have a Filesystem to handle virtual files, path redirection, etc... But the implementation might change depending on the context. So, a base class that can be used for all file systems would be beneficial.

```cpp
class FilesystemV1 : public IFilesystem{
    ...
};

class FilesystemV2 : public IFilesystem{
    ...
};
```

Another good exemple would be the WindowingSystem with different implementation based of the underlying API.

Extending this idea further. There could be a plugin system that allows for the addition of new systems without modifying the core engine.


### Event system

### Graphics system

The graphics system can be quite complexe, but I'm working so that it would be simple to use, very modular and fast enough to handle complex scenes in real-time.

The graphics system is based of a frame graph. A frame graph is an acyclic tree of passes and effects rendering in the end the final image. However, we need to make the system easy to use and modular. Here is how : 

There are two levels of graphs.
- The render graphs
- The composition graphs

#### Render graphs

The render graph, as said earlier, is a tree of passes. In this context. They will represent how a specific camera will render. The graph as access to the scene data and camera informations.

A simple graph would be
> GBuffer pass -> differred lightning -> output

It will render the scene using the geometry info of the scene and the lights.
Additional effects can be applyed, like bloom, depth of field, etc... This graph is not restraint to only geometry passes.

The output of the camera can serve as texture for materials, but it can also output attachments for further effects to be presented to the output window.

To register as an attachment output. The camera must follow one of the camera archtypes defined by the user. A camera archetype defines just what should the camera output (color, albedo, depth, instance buffer, etc...)

The, if it follows the archetype, it can be registred in one of the predefined camera slots. 
Once registred, itr's output can be used in other graphs.

The passes need to access data from the scene. As multiple passes may need to same data. A centralized data provider need to be defined.

#### Data providers

Data providers are systems that will extract data from scenes and make them availables for render. The type of data can be config structures for passes, position info, etc...

We can for exemple have a LightDataProvider that will extract light data from the scene and store a reflection of it on the GPU. This data can then be used in passes.

#### Data receivers

Data receivers work in the same way, but instead of submiting data to the GPU it will retreive from it. It can be used to access simulation data, screenshot, culling, etc.

However to transmit all data, we need a transmit manager.

#### Transmit manager

#### Composition graphs