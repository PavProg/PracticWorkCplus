#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <typeindex>

#include "Resource.hpp"

class IRenderAdapter;

// Если ресурс жив в кэще - возвращаем его компонентам (cache hit). Если нет - вызываем LoadImpl -> положить в кэш -> вернуть (cache miss)

class ResourceManager {
public:
    explicit ResourceManager(IRenderAdapter& renderer);
    ~ResourceManager();

    // Возвращает shared_ptr или nullptr при ошибке.
    template<typename T>
    std::shared_ptr<Resource<T>> Load(const std::string& path);

    void Clear();

    IRenderAdapter& GetRenderer() { return m_renderer; }

private:
    IRenderAdapter& m_renderer;

    std::unordered_map<
        std::type_index,
        std::unordered_map<std::string, std::weak_ptr<void>>    // weak_ptr<void> - стирание типа = unique_ptr<void, deleter>
    > m_caches;

    template<typename T>
    std::shared_ptr<Resource<T>> LoadImpl(const std::string& path);
};

#include "ResourceManager.tpp"

#endif