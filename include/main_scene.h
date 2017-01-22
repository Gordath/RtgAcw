#ifndef MAIN_SCENE_H_
#define MAIN_SCENE_H_
#include "scene.h"
#include "../GlacierEngine/rendering/include/d3d/D3D11_render_target.h"
#include "water_jet_sub.h"
#include <array>

class DrebelSubmarine;

class MainScene : public Glacier::Scene
{
private:
	Glacier::D3D11RenderTarget m_color_pass_rt;
	Glacier::ComPtr<ID3D11Buffer> m_color_pass_uniform_buffer;
	Glacier::ComPtr<ID3D11Buffer> m_depth_pass_uniform_buffer;
	Glacier::ComPtr<ID3D11Buffer> m_particle_uniform_buffer;
	Glacier::ComPtr<ID3D11Buffer> m_skybox_uniform_buffer;

	std::array<Glacier::D3D11RenderTarget, 4> m_depth_pass_rts;

	Glacier::ComPtr<ID3D11Buffer> m_light_structured_buffer;
	Glacier::ComPtr<ID3D11ShaderResourceView> m_light_srv;

	Glacier::ComPtr<ID3D11SamplerState> m_sampler_linear_wrap;
	Glacier::ComPtr<ID3D11SamplerState> m_sampler_shadow_comparison;

	Glacier::Object* m_skybox; /* parasoft-suppress  MRM-33 "The memory of all objects added to the Scene is freed by the Scene base class in it's destructor." */
	Glacier::Object* m_globe; /* parasoft-suppress  MRM-33 "The memory of all objects added to the Scene is freed by the Scene base class in it's destructor." */

	DrebelSubmarine* m_drebel; /* parasoft-suppress  MRM-33 "The memory of all objects added to the Scene is freed by the Scene base class in it's destructor." */
	WaterJetSubmarine* m_water_jet_sub; /* parasoft-suppress  MRM-33 "The memory of all objects added to the Scene is freed by the Scene base class in it's destructor." */

	void depth_pass() const noexcept;
	void color_pass() const noexcept;
	void display_to_screen() const noexcept;

	void render_globe() const noexcept;
	void render_skybox() const noexcept;

	void setup_lights() noexcept;
	void setup_cameras() noexcept;
	void setup_d3d() noexcept;

public:
	MainScene() :
		m_skybox{ nullptr },
		m_globe{ nullptr },
		m_drebel{ nullptr },
		m_water_jet_sub{nullptr}
	{
	}

	MainScene(const MainScene& other) = delete;
	explicit MainScene(MainScene&& other) noexcept = delete;
	MainScene& operator=(const MainScene& other) = delete;
	MainScene& operator=(MainScene&& other) noexcept = delete;

	~MainScene();

	void initialize() override;

	void on_key_down(unsigned char key, int x, int y) noexcept override;

	void on_key_up(unsigned char key, int x, int y) noexcept override;

	void on_mouse_motion(int x, int y) noexcept override;

	void on_mouse_click(int button, bool state, int x, int y) override;

	void update(float delta_time, long time) noexcept override;

	void draw() const noexcept override;
};

#endif //MAIN_SCENE_H_
