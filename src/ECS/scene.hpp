#pragma once
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

#include "components/transform.hpp"

// #include "group_storage.hpp"

#include <functional>
#include <vector>
#include <deque>
#include <array>
#include <memory>

struct ignore_t{};

namespace rush
{

    class Scene
    {

    public:
        Scene();
        ~Scene();

        template <typename... Components, typename... Exclude>
        void createRuntimeViewIterator(std::function<void(entt::entity)> &&func, entt::exclude_t<Exclude...> = {})
        {
            renderViews.push_back(
                [&]()
                {
                    auto view = entt::runtime_view{};

                    // fancy lambda that took way too long to make.
                    // basically just a for loop over all the components but its funcy because of typename
                    ([&]()
                     { view.iterate(m_Registry.storage<Components>()); }(),
                     ...);
                    ([&]()
                     { view.exclude(m_Registry.storage<Exclude>()); }(),
                     ...);

                    // parrallel mode?
                    //  std::for_each(std::execution::par_unseq, view.begin(), view.end(), [&view](auto entity) {
                    //      func(entity);
                    //  });

                    for (auto ent : view)
                    {
                        func(ent);
                    }
                });
        }

        void flushViewIterators()
        {
            for (auto it = renderViews.rbegin(); it != renderViews.rend(); it++)
            {
                (*it)(); // call functors
            }
        }

        entt::registry m_Registry;

    private:
        std::deque<std::function<void()>> renderViews;
    };
}