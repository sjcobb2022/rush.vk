#pragma once

#include "lve_model.hpp"
#include "components/transform.hpp"

// libs
#include <spdlog/spdlog.h>

#include <entt/entity/registry.hpp>

// std
#include <cxxabi.h>

#include <cstring>
#include <iostream>
#include <memory>
#include <unordered_map>

namespace volk {

class LveGameObject {
 public:
  using id_t = unsigned int;

  static LveGameObject createGameObject(std::shared_ptr<entt::registry> registry) {
    static id_t currentId = 0;

    //create ent
    auto ent = registry->create();

    //add id_t to reg
    registry->emplace<id_t>(ent, currentId++);

    return LveGameObject{ent, registry};
  }

  LveGameObject(const LveGameObject &) = delete;
  LveGameObject &operator=(const LveGameObject &) = delete;
  LveGameObject(LveGameObject &&) = default;
  LveGameObject &operator=(LveGameObject &&) = default;

  id_t getId() {
    // spdlog::debug("Created gameObject: {}", );
    return m_Registry->get<LveGameObject::id_t>(m_Entity);
  }

  template <typename Component>
  Component addComponent(Component value) {
    if (typeid(Component).hash_code() == typeid(LveGameObject::id_t).hash_code()) {
      throw std::runtime_error("Cannot change id_t of gameObject, this is private");
    }

    int status;
    spdlog::debug(
        "Creating {} component for entity {}",
        abi::__cxa_demangle(typeid(Component).name(), 0, 0, &status),
        getId());

    return m_Registry->emplace<Component>(m_Entity, value);
  }

  template <typename Component>
  Component emplaceComponent(Component value) {
    return m_Registry->emplace<Component>(m_Entity, value);
  }

  template <typename Component>
  Component replaceComponent(Component value) {
    return m_Registry->replace<Component>(m_Entity, value);
  }

  template <typename Component>
  Component emplaceOrReplaceComponent(Component value) {
    return m_Registry->emplace_or_replace<Component>(m_Entity, value);
  }

  template <typename Component>
  void deleteComponent() {
    m_Registry->remove<Component>(m_Entity);
  }

  template <typename Component>
  Component getComponent() {
    return m_Registry->get<Component>(m_Entity);
  }

  template <typename Component>
  Component modifyComponent(Component new_val) {
    if (typeid(Component).hash_code() == typeid(LveGameObject::id_t).hash_code()) {
      throw std::runtime_error("Cannot modify id_t of Entity, this is private");
    }

    return m_Registry->replace<Component>(m_Entity, new_val);
  }

 private:
  LveGameObject(entt::entity ent, std::shared_ptr<entt::registry> reg)
      : m_Entity{ent}, m_Registry{reg} {
    // spdlog::info("Created gameObject with id: {}", m_Registry->get<id_t>(m_Entity));
  }

  entt::entity m_Entity;

  std::shared_ptr<entt::registry> m_Registry;
};

}  // namespace volk
