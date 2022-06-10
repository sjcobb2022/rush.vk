# Assets

This is the home of the assets library, and asset system for this project.

The asset system should be totally seperate from the main engine to prevent extra linkages in the main engine. This should hopefully decrease compile times and save resources since assets don't need to be compiled every time the engine is compiled

## Formats

KTX -

PNG - 

### Textures

All textures will be loaded with Khronos' KTX binary format. This was chosen because it has built in compression and reliable loading. 

### Models

The project will initially start with a quick and dirty binary blob format taken from .obj files. This ensures that vertices can be streamed to the GPU. When more features are avaialabe, gltf will also be an option.