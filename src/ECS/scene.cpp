#include "scene.hpp"

namespace rush
{

    Scene::Scene() {}

    Scene::~Scene()
    {
        m_Registry.clear();
    }

    template <typename Component, typename... Other, typename... Exclude>
    void Scene::createRuntimeView(std::function<void(entt::runtime_view)> &&func, entt::exclude_t<Exclude...>)
    {
        renderViews.push_back(
            //NOTE: We are pushing this back to the renderViews vector that can be iterated every frame

            //TODO: does the capture method effect anything here?

            //TODO: See if initialising outside of view has any benefits
            [=]()
            {
                //following the entt docu and only creating when needed.
                entt::runtime_view view{};

                // add initial component, following format of entt.
                view.iterate(m_Registry.storage<Component>());

                // spread variadic template with fold for all other components
                (view.iterate(m_Registry.storage<Other>()...));

                // spread variadic template with fold for exclusion parameters
                (view.exclude(m_Registry.storage<Exclude>()...));

                func(view);
            }
            );
    }


}