#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <typeindex>

#include "Resource.hpp"
#include "LoadQueue.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"

class IRenderAdapter;

// Если ресурс жив в кэше - возвращаем его компонентам (cache hit). Если нет - вызываем LoadImpl -> положить в кэш -> вернуть (cache miss)

class ResourceManager {
public:
    explicit ResourceManager(IRenderAdapter& renderer);
    ~ResourceManager();

    // Возвращает shared_ptr или nullptr при ошибке.
    template<typename T>
    std::shared_ptr<Resource<T>> Load(const std::string& path);

    void Clear();

    IRenderAdapter& GetRenderer() { return m_renderer; }

    void ReloadModifiedShaders();
    void ReloadModifiedTextures();

    template<typename T>
    std::shared_ptr<Resource<T>> LoadAsync(const std::string& path);

    // async архитектура
    void InitPlaceholders();
    void ReleasePlaceholders();
    void StartAsync(int workerks = 2) { m_queue.Start(workerks); }
    void StopAsync() { m_queue.Stop(); }
    void PumpUploads() { m_queue.Pump(); }
private:
    IRenderAdapter& m_renderer;

    std::unordered_map<
        std::type_index,
        std::unordered_map<std::string, std::weak_ptr<void>>    // weak_ptr<void> - стирание типа = unique_ptr<void, deleter>
    > m_caches;

    LoadQueue m_queue;
    GPUMesh m_placeholderMesh{};
    GPUTexture m_placeholderTex{};

    template<typename T>
    std::shared_ptr<Resource<T>> LoadImpl(const std::string& path);
};

#include "ResourceManager.tpp"

#endif