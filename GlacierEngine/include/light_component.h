#ifndef GLACIER_LIGHT_COMPONENT_H_
#define GLACIER_LIGHT_COMPONENT_H_
#include "component.h"
#include "internal/types.h"

namespace Glacier
{
	struct LightDesc {
		bool enabled{ true };
		Vec4f ambient_intensity;
		Vec4f diffuse_intensity{1.0f, 1.0f, 1.0f, 1.0f};
		Vec4f specular_intensity{1.0f, 1.0f, 1.0f, 1.0f};
		float spot_cutoff;
		Vec3f spot_direction;
		float spot_exponent;
		Vec3f attenuation{1.0f, 0.0f, 0.0f};
		Vec3f position;
		bool directional;
	};

	class LightComponent : public Component {
	private:
		LightDesc m_light_desc;

	public:
		LightComponent(Object* parent) : Component{ "co_light", parent }
		{
		}

		operator LightDesc() const
		{
			return m_light_desc;
		}

		void setup() noexcept override;
		
		void update(float dt, long time) noexcept override;
		
		void teardown() noexcept override;

		void set_light_description(const LightDesc& light_desc) noexcept
		{
			m_light_desc = light_desc;
		}

		const LightDesc& get_light_description() const noexcept
		{
			return m_light_desc;
		}

		void set_enabled(bool state) noexcept
		{
			m_light_desc.enabled = state;
		}

		bool is_enabled() const noexcept
		{
			return m_light_desc.enabled;
		}

		void set_ambient_intensity(const Vec4f& ambient_intensity) noexcept
		{
			m_light_desc.ambient_intensity = ambient_intensity;
		}

		const Vec4f& get_ambient_intensity() const noexcept
		{
			return m_light_desc.ambient_intensity;
		}

		void set_diffuse_intensity(const Vec4f& diffuse_intensity) noexcept
		{
			m_light_desc.diffuse_intensity = diffuse_intensity;
		}

		const Vec4f& get_diffuse_intensity() const noexcept
		{
			return m_light_desc.diffuse_intensity;
		}

		void set_specular_intensity(const Vec4f& specular_intensity) noexcept
		{
			m_light_desc.specular_intensity = specular_intensity;
		}

		const Vec4f& get_specular_intensity() const noexcept
		{
			return m_light_desc.specular_intensity;
		}

		void set_spotlight_cutoff(float spot_cutoff) noexcept
		{
			m_light_desc.spot_cutoff = spot_cutoff;
		}

		float get_spotlight_cutoff() const noexcept
		{
			return m_light_desc.spot_cutoff;
		}

		void set_spotlight_direction(const Vec3f& spot_direction) noexcept
		{
			m_light_desc.spot_direction = spot_direction;
		}

		const Vec3f& get_spotlight_direction() const noexcept
		{
			return m_light_desc.spot_direction;
		}

		void set_spotlight_exponent(float spot_exponent) noexcept
		{
			m_light_desc.spot_exponent = spot_exponent;
		}

		float get_spot_exponent() const noexcept
		{
			return m_light_desc.spot_exponent;
		}

		void set_directional(bool state) noexcept
		{
			m_light_desc.directional = state;
		}

		bool is_directional() const noexcept
		{
			return m_light_desc.directional;
		}
	};
}

#endif //GLACIER_LIGHT_COMPONENT_H_
