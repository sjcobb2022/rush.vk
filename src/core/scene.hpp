#pragma once
#include <entt/entity/registry.hpp>
#include <spdlog/spdlog.h>

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
#include <type_traits>

// template <typename T>
// class has_valid_operator
// {
//     typedef char valid;
//     typedef long invalid;

//     // checks if operator
//     template <typename C>
//     static valid check(decltype(&C::operator()));
//     template <typename C>
//     static invalid check(...);

// public:
//     enum
//     {
//         value = sizeof(check<T>(0)) == sizeof(char)
//     };
// };

// //
// template <typename T>
// inline constexpr bool has_valid_operator_v = has_valid_operator<T>::value;

// struct ignore_t
// {
// };

namespace rush
{
    // class Scene
    // {

    // public:
    //     Scene();
    //     ~Scene();

    //     entt::registry &registry()
    //     {
    //         return m_Registry;
    //     }

    //     entt::entity create()
    //     {
    //         return m_Registry.create();
    //     }

    //     template <typename Component>
    //     Component &emplace(entt::entity ent, Component Val)
    //     {
    //         return m_Registry.emplace<Component>(ent, Val);
    //     }

    //     template <typename... Components>
    //     void emplace(entt::entity ent, Components &&...Args)
    //     {
    //         (m_Registry.emplace<Components>(ent, std::forward<Components>(Args)), ...);
    //     }

    //     template <typename Component>
    //     Component &get(entt::entity ent)
    //     {
    //         return m_Registry.get<Component>(ent);
    //     }

    //     void destroy(entt::entity ent)
    //     {
    //         m_Registry.destroy(ent);
    //         return;
    //     }

    //     template <typename iterator_t>
    //     void destroy(iterator_t begin, iterator_t end)
    //     {
    //         m_Registry.destroy(begin, end);
    //     }

    //     template <typename F>
    //     void createRuntimeViewIterator(F &&f)
    //     {
    //         if constexpr (has_valid_operator<F>::value)
    //         {
    //             runtimeViewFunctors.push_back(
    //                 [&f, this]()
    //                 {
    //                     Each<decltype(&F::operator())>::each(std::forward<F>(f), m_Registry);
    //                 });
    //         }
    //         else
    //         {
    //             throw std::runtime_error("Cannot create lambda-in-lamda view, this causes a segfault for some reason");
    //             // runtimeViewFunctors.push_back([&f, this]()
    //             //                               { each(std::forward<F>(f)); });
    //         }
    //     }

    //     void flushViewIterators()
    //     {
    //         for (auto it = runtimeViewFunctors.rbegin(); it != runtimeViewFunctors.rend(); it++)
    //         {
    //             (*it)(); // call functors
    //         }
    //     }

    //     template <typename Func>
    //     inline void each(Func &&func)
    //     {
    //         if constexpr (has_valid_operator_v<Func>)
    //         {
    //             Each<decltype(&Func::operator())>::each(std::forward<Func>(func), m_Registry);
    //         }
    //         else
    //         {
    //             // spdlog::debug("Internal :: fails has_parenthesis");
    //             each(std::forward<Func>(func));
    //             throw std::runtime_error("Cannot create lambda-in-lamda view, this causes a segfault");
    //         }
    //     }

    // private:
    //     entt::registry m_Registry;
    //     std::deque<std::function<void()>> runtimeViewFunctors;

    //     //<-------- DO NOT TOUCH :: WILL CAUSE SEGFAULTS -------->
    //     // Credits to Nikki93 for helping me implement this template fuckery
    //     // https://gist.github.com/nikki93/3cee41b34af3cefe5733d9a5fc502876#file-entity-hh-L72

    //     template <typename T>
    //     struct Each
    //     {
    //     };

    //     template <typename R, typename C>
    //     struct Each<R (C::*)(entt::entity ent) const>
    //     {
    //         template <typename F>
    //         static void each(F &&f, entt::registry &reg)
    //         {
    //             reg.each(std::forward<F>(f));
    //         }
    //     };

    //     template <typename R, typename C, typename T, typename... Ts>
    //     struct Each<R (C::*)(entt::entity ent, T &, Ts &...) const>
    //     {
    //         template <typename F>
    //         static void each(F &&f, entt::registry &reg)
    //         {
    //             reg.view<T, Ts...>().each([&](entt::entity ent, T &t, Ts &...ts)
    //                                       { f(ent, t, ts...); });
    //         }
    //     };
    //     template <typename R, typename C, typename T, typename... Ts>
    //     struct Each<R (C::*)(entt::entity ent, T *, Ts *...) const>
    //     {
    //         template <typename F>
    //         static void each(F &&f, entt::registry &reg)
    //         {
    //             reg.view<T, Ts...>().each([&](entt::entity ent, T &t, Ts &...ts)
    //                                       { f(ent, &t, &ts...); });
    //         }
    //     };

    //     template <typename R, typename... Args>
    //     inline void each(R (&f)(Args...))
    //     {
    //         each([&](Args... args)
    //              { f(std::forward<Args>(args)...); });
    //     }
    //     //<-------- DO NOT TOUCH :: WILL CAUSE SEGFAULTS -------->
    // };
}