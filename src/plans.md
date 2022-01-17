# REplanning doc for Vulkan Engine


## main.cpp structure concepts

These are all the concept code structures that I might consider when rebuilding this engine

### Concept #1 :: Variable approach to literally everything

```cpp

#include "core.cpp"

int main(){

    App app{};

    // functions should probably be global and static when compiling.
    // to to share state they should either use global or some sort of data queue: e.g. to pass data onto next have to do something along the lines of `nextStage.push_back(a bunch of data)`
    //alternatively ensure that nothing is stored in any of the scopes and only use data that is stored in the scene.
    //test if global state in the main file is carried on with function pointers
    app.setup(std::function<void(Scene scene, Camera cam)>& func); //init values such as 

    app.loop(std::function<void(Scene scene, Camera cam)>& func); //changes that happen in the loop

    app.end(std::function<void(Scene scene, Camera cam)>& func); //cleanup if necessary

    app.turn_imgui_off = true || false; //option to disable imgui

    app.ui.add(ImGuiSomethingIdk) //.....????

    //would be considerably easier to intergrate imgui since working from the the beggining and can focus more on tight intergration between the two
    //might need to add it as a seperate render pass or as a render subpass
    //consider using pipeline barriers

    try{
        app.run()
        // app.run would first call setup
        // then it would call the loop function every frame
        // then at window close or whatever the end cleanup would be called
    }
    catch(std::err &e){
        handleError(e)
    }

}

```

Pros:
 - Simple to code with once complete
 - Easy to adapt into a more general-purpose API
 - Very variable and therefore easier to build in the future
 - Hopefully easier to implement imgui since general purpose/variable function should make it easier
 - Cmake is already setup. 

Cons:
 - Harder to implement
 - Variable could come at performance loss
 - Complex structural relationships
 - std::function _bleugghufhh_


 **Note:** I actually really do like the methods of passing functions through for garbage collection and rendering used in vkguide.dev (have a single destructor state which then push/pop/flush the cleanup functions for each).


### Concept 2 :: Similar to existing engine, 

```cpp

int main(){

    App app{};
    
    //essentially the same as what volk/lve has right now
    //might need a new scene heirarchy and whatnot
    //need to intergrate imgui properly however which is hard given the existing systems
    //stealing Sascha Willem's ImGui wrapper is possible bc its very easy to yoink

    try{
        app.run()
    }
    catch(std::err &e){
        // spdlog::critical("{}", e.what());
        throw std::runtime_error("lmao bad");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

}

```

```cpp

app::run(){

    app::init_stuff();

    while(!window.shouldClose()){


        app::gameLoop(a bunch of data);

        ImGui::stuff()
        ImGui::end_stuff()

    }
}

```

Pros
 - Much faster to write because I can just copy and paste from previous work
 - I don't need to use brain as much
 - a lot of the classes need rewriting in my opinon
    - already rewritten gameObject for entt support
    - The whole reason im considering rewriting is because I want to verge away from rewriting the older shit

Cons
 - Very static and needs a lot of refactoring
 - Harder to implement new features
 - Literally the whole reason I wanted to do this was so I don't need to rewrite




### Concept 3 :: Rewrite everything in pure C (worst idea so far)

```CXX

struct failureEnum{
    int PASS;
    int FAIL;
} failureEnum;

int main(){

    someStructRepresentingAnApp app{};

    return handleReturnWithEnum(runApp(app));
    //or something wierd like that
}

```

Pros
 - Fuck OOP
    -  No like seriously I don't like OOP
 - No more inheritence
 - want to properly learn C
 - I really like functional programming

Cons
 - Need to use brain <-- will get headaches
 - WHY THE FUCK ARE THERE NO TRY CATCH BLOCKS LIKE WHAT??????
 - Memory leaks more prevalent
 - Global state is so annoying to deal with
     - big fat global state struct or something?
         - big brother to FrameInfo
 - More memory management
 - No support for glm/imgui/entt
    - There are alternatives (cglm, Nuklear, flecs)
    - Alternatives are generally a bit complicated when it comes to pointers are stuff (**cough _flecs_ cough**)
 - What do you mean Cons C has no negative side at all it is the perfect programming paradigm. It is only the programmer who is at fault the C compiler is perfect.
 - Ah shit nvm classes can be kinda useful
 - still a possibility if I get bored. (e.g. rewrite again in C)




## ImGui Implementation

Ideas
 - Just straight up copy Sascha Willem's wrapper
 - Make my own wrapper
 - Make a more atomized system for vulkan rendering which allows for easier rendering



# System Architecture

- Entity Component system
     - Scene is registry for each object
     - Each object has a reference to a registry and a reference to it's own entity
         - Object can store shared pointer to registry because every entity will contain
     - Implementation Choices:
         - Scene.createGameObject();
             - and then add all the necessary properties
             - Could have wrappers around common renderable (maybe extend some "core" game object class)
         - createGameObject(Scene);
            - basically the same pros and cons its just a stylistic choice
            - could extend functionality with something like
                 - createCubeObject(Scene, detailX, detailY);
                 - createMeshObject(Scene, mesh)
     - Should probably add something like a renderable component
         - This might be good however could just do filters like
             ```cpp
             registry.view<model, transform,>();
             ```
         - Adding renderable component might be good to ensure that only renderable components with a model and a transform can get rendered
             ```cpp
             registry.view<renderable, model, transform>();
             ```

- Asset Manager
     - Asset manager links to all model loaders (tinyobjloader, gltf loader, shader loader, )
     - passes on necessary data to renderer

- ImGUi
     - ImGui wrapper
     - use Sascha Willems's wrapper
     - already meantioned this stuff

- VMA intergration
     - Create a general wrapper around imgui for buffer creation and stuff
     - I really want to use VMA because it is more for future proofing. 