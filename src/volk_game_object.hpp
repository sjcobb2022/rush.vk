#pragma once

#include "lve_model.hpp"

// libs
#include <entt/entity/registry.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <spdlog/spdlog.h>

// std
#include <memory>
#include <unordered_map>
#include <cxxabi.h>
#include <cstring>
#include <iostream>

namespace lve {

struct TransformComponent {
  glm::vec3 translation{};
  glm::vec3 scale{1.f, 1.f, 1.f};
  glm::vec3 rotation{};

  // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
  // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
  // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
  glm::mat4 mat4();

  glm::mat3 normalMatrix();
};

class LveGameObject {
 public:
  using id_t = unsigned int;

  static LveGameObject createGameObject(std::shared_ptr<entt::registry> registry) {
    static id_t currentId = 0;

    auto ent = registry->create();

    registry->emplace<id_t>(ent, currentId++);

    return LveGameObject{ent, registry};
  }

  LveGameObject(const LveGameObject &) = delete;
  LveGameObject &operator=(const LveGameObject &) = delete;
  LveGameObject(LveGameObject &&) = default;
  LveGameObject &operator=(LveGameObject &&) = default;

  id_t getId() {
    // spdlog::debug("Created id: {}", id);
    return m_Registry->get<LveGameObject::id_t>(m_Entity);
  }

  entt::entity getEntity(){
    return m_Entity;
  }

  template<typename Component>
  Component addComponent(Component value){
    
    // #if DEBUG
    // if(Component == LveGameObject::id_t){
    // if(typeid(Component).hash_code)
    // assert( && "Cannot change LveGameObject::id_t of LveGameObject");

    if(typeid(Component).hash_code() == typeid(LveGameObject::id_t).hash_code()){
      throw std::runtime_error("Cannot set id_t of Entity, this is private");
    }

    int status;
    spdlog::debug("Creating {} component for entity {}", abi::__cxa_demangle(typeid(Component).name(), 0, 0, &status), getId());
    return m_Registry->emplace<Component>(m_Entity, value);
  }

  template<typename Component>
  void deleteComponent(){
    m_Registry->remove<Component>(m_Entity);
  }

  template<typename Component>
  Component getComponent(){
    return m_Registry->get<Component>(m_Entity);
  }

  template<typename Component>
  Component modifyComponent(Component new_val){

    if(typeid(Component).hash_code() == typeid(LveGameObject::id_t).hash_code()){
      throw std::runtime_error("Cannot modify id_t of Entity, this is private");
    }

    return m_Registry->replace<Component>(m_Entity, new_val);
  }

 private:
  LveGameObject(entt::entity ent, std::shared_ptr<entt::registry> reg) : m_Entity{ent}, m_Registry{reg} {
    // spdlog::info("Created gameObject with id: {}", m_Registry->get<id_t>(m_Entity));
  }

  entt::entity m_Entity;

  std::shared_ptr<entt::registry> m_Registry;

};

}  // namespace lve
