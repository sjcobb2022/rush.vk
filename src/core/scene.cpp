#include "scene.hpp"

#include "rush_pch.hpp"

namespace rush
{

    Scene::Scene(){};

    Scene::~Scene()
    {
        m_Registry.clear();
    }
    
}