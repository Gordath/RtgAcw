#include "resource_manager.h"

namespace Glacier
{
	std::map<std::wstring, Resource*> ResourceManager::m_resources_by_name;

	std::map<unsigned int, Resource*> ResourceManager::m_resources_by_id;
}