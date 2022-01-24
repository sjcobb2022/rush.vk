# Assets

This is the home of the assets library, and asset system for this project.

The asset system should be totally seperate from the main engine to prevent extra linkages in the main engine. This should hopefully decrease compile times and save resources since assets don't need to be compiled 

## Formats

### Textures

All models are loaded with Khronos' KTX binary format. This was chosen because it has built in compression and reliable loading. 

### Models

A custom binary blob format is going to be used for model formatting, so that vertices can simply be streamed to the GPU. This is faster than loading from a .obj or .gltf file in all cases.