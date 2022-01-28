
#include <entt/entity/registry.hpp>

#include <cstddef>

namespace rush{

    struct c_Relationship{
        std::size_t children{};
        entt::entity first{entt::null};
        entt::entity prev{entt::null};
        entt::entity next{entt::null};
        entt::entity parent{entt::null};
    };
}