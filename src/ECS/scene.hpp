#pragma once
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

#include "components/transform.hpp"

// #include "group_storage.hpp"

#include <functional> //std::function
#include <vector>     //std::vector
#include <deque>      //std::deque
#include <memory>

#ifndef __clang__
#include <execution>
#include <algorithm>
#endif

#include <cstdlib>

struct ignore_t
{
};

namespace rush
{

    class Scene
    {

    public:
        Scene();
        ~Scene();

        template <typename... Components, typename... Exclude>
        void createRuntimeViewIterator(std::function<void(entt::entity)> &&func, entt::exclude_t<Exclude...> = {}, bool experimentalUseParallel = false)
        {
            runtimeViewFunctors.push_back(
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

                    // auto view = m_Registry.view<Components...>(entt::exclude<Exclude...>);

                    if (experimentalUseParallel)
                    {

#ifndef __clang__
                        std::for_each(std::execution::par_unseq, view.begin(), view.end(),
                                      [&view](auto entity)
                                      { func(entity); });
#else
                        spdlog::critical("Clang does not support parallel implementation yet");
                        throw std::runtime_error("Clang does not support parallel implementation yet");
#endif
                    }
                    else
                    {
                        for (auto ent : view)
                        {
                            func(ent);
                        }
                    }
                });
        }

        void flushViewIterators()
        {
            for (auto it = runtimeViewFunctors.rbegin(); it != runtimeViewFunctors.rend(); it++)
            {
                (*it)(); // call functors
            }
        }

        entt::registry m_Registry;

    private:
        std::deque<std::function<void()>> runtimeViewFunctors;
    };
}