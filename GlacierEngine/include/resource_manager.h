#ifndef GLACIER_RESOURCE_MANAGER_H_
#define GLACIER_RESOURCE_MANAGER_H_

#include <map>
#include <iostream>
#include "resource.h"

namespace Glacier
{
	static const std::wstring MODELS_PATH{ L"data\\models\\" };
	static const std::wstring TEXTURE_PATH{ L"data\\textures\\" };

	static int id = 0;

	class ResourceManager {
	private:
		static std::map<std::wstring, Resource*> m_resources_by_name;

		static std::map<unsigned int, Resource*> m_resources_by_id;

	public:
		ResourceManager() = default;

		ResourceManager(const ResourceManager&) = delete;

		ResourceManager& operator=(const ResourceManager&) = delete;

		~ResourceManager()
		{
			for (auto resource : m_resources_by_name) {
				delete resource.second;
			}

			m_resources_by_name.clear();

			m_resources_by_id.clear();
		}

		template<typename T>
		static bool load(const std::wstring& fileName)
		{
			T* resource{ new T };

			if (resource->load(fileName)) {
				resource->set_id(id);
				register_resource(resource, fileName);

				return true;
			}

			return false;
		}

		template<typename T>
		static T* get(const std::wstring& fileName)
		{
			Resource* resource{ m_resources_by_name[fileName] };

			if (!resource) {
				load<T>(fileName);
				resource = m_resources_by_name[fileName];
			}

			T* res{ dynamic_cast<T*>(resource) };
			if (!res) {
				std::cerr << "The file you asked for does not represent the type you are requesting!" << std::endl;
				return nullptr;
			}

			return res;
		}

		static void register_resource(Resource* resource, const std::wstring& name)
		{
			if (!resource) {
				std::cerr << "Resource registration failed: Cannot register a null resource." << std::endl;
				return;
			}

			if (name.empty()) {
				std::cerr << "Resource registration failed: Cannot register a resource without providing a name." << std::endl;
				return;
			}

			m_resources_by_name[name] = resource;
			m_resources_by_id[id++] = resource;
		}
	};
}

#endif //GLACIER_RESOURCE_MANAGER_H_
