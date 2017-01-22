#ifndef GLACIER_LIGHT_COMPONENT_H_
#define GLACIER_LIGHT_COMPONENT_H_
#include "component.h"
#include "internal/types.h"

namespace Glacier
{
	struct LightDesc {
		Vec4f ambient_intensity;
		Vec4f diffuse_intensity{ 1.0f, 1.0f, 1.0f, 1.0f };
		Vec4f specular_intensity{ 1.0f, 1.0f, 1.0f, 1.0f };
		float spot_cutoff{ 180.0f };
		Vec3f spot_direction{ 0.0f, 0.0f, 1.0f };
		float spot_exponent{ 0.0f }; //[0, 128]
		Vec3f attenuation{ 1.0f, 0.0f, 0.0f };
		Vec4ui flags; //directional, enabled, 0, 0
		Mat4f light_view_matrix;
		Mat4f light_projection_matrix;
		Vec3f position;
		float pad;
	};

	class LightComponent : public Component {
	private:
		LightDesc m_light_desc;

	public:
		explicit LightComponent(Object* parent) : Component{ "co_light", parent }
		{
		}

//		operator LightDesc() const
//		{
//			return m_light_desc;
//		}

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

		LightDesc* get_light_description_ptr() noexcept
		{
			return &m_light_desc;
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

		void set_flags(const Vec4ui& flags) noexcept
		{
			m_light_desc.flags = flags;
		}

		void set_active(bool active) noexcept
		{
			m_light_desc.flags.y = active;
		}

		bool is_active() const noexcept
		{
			return m_light_desc.flags.y;
		}

		const Mat4f& get_light_view_matrix() const noexcept
		{
			return m_light_desc.light_view_matrix;
		}

		const Mat4f& get_light_projection_matrix() const noexcept
		{
			return m_light_desc.light_projection_matrix;
		}
	};
}

#endif //GLACIER_LIGHT_COMPONENT_H_
