#include "scene.hpp"

#include <spdlog/spdlog.h>

namespace rush
{

    Scene::Scene(){};

    Scene::~Scene()
    {
        m_Registry.clear();
    }

}