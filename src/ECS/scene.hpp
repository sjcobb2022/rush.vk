
#include <entt/entity/registry.hpp>

#include <functional>
#include <vector>

namespace rush{

    class Scene{

        public:
            Scene();
            ~Scene();

            template<typename Component , typename... Other, typename... Exclude>
            void createRuntimeView(

                //TODO: create a alias for the runtime view to make things a bit easier for users

                std::function<void(entt::runtime_view)>&&func, 
                entt::exclude_t<Exclude...> = {} /*define exclude as empty since not necesssary*/
                
                );
            
        private:
            entt::registry m_Registry;
            std::vector<std::function<void(entt::runtime_view)>> renderViews;
    };
}