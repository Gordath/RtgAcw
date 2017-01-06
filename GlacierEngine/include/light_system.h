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

		const std::vector<LightDesc>& get_active_light_descriptions() const noexcept
		{
			return m_active_light_descriptions;
		}
	};
}

#endif //GLACIER_LIGHT_SYSTEM_H_
