#ifndef GLACIER_LIGHT_SYSTEM_H_
#define GLACIER_LIGHT_SYSTEM_H_
#include "system.h"
#include <map>
#include "light_component.h"

namespace Glacier
{
	class LightSystem : public System {
	private:
		mutable std::map<std::string, Object*> m_lights;

		mutable std::vector<LightDesc> m_active_light_descriptions;

		void update_active_light_descriptions() const;

	public:
		bool initialize() noexcept override;

		void process(const std::vector<Object*>& objects, float delta_time) const noexcept override;

		std::vector<LightDesc> get_active_light_descriptions() const noexcept
		{
			std::vector<LightDesc> ldescs;

			for (auto light : m_lights) {
				LightComponent* lc{ static_cast<LightComponent*>(light.second->get_component("co_light")) };
				ldescs.push_back(lc->get_light_description());
			}

			return ldescs;
		}

		Object* get_light_object(const std::string& name) const noexcept
		{
			return m_lights[name];
		}

		void toggle_light(const std::string& light_name) const
		{
			Object* obj{ m_lights[light_name] };

			if (obj) {
				LightComponent* lc{ static_cast<LightComponent*>(obj->get_component("co_light")) };

				lc->set_active(!lc->is_active());
			}
		}
	};
}

#endif //GLACIER_LIGHT_SYSTEM_H_
