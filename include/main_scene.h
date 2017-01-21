#ifndef MAIN_SCENE_H_
#define MAIN_SCENE_H_
#include "scene.h"
#include "../GlacierEngine/rendering/include/d3d/D3D11_render_target.h"
#include "water_jet_sub.h"

class DrebelSubmarine;

class MainScene : public Glacier::Scene {
private:
	Glacier::D3D11RenderTarget m_color_pass_rt;
	Glacier::ComPtr<ID3D11Buffer> m_color_pass_uniform_buffer;
	Glacier::ComPtr<ID3D11Buffer> m_depth_pass_uniform_buffer;
	Glacier::ComPtr<ID3D11Buffer> m_particle_uniform_buffer;
	Glacier::ComPtr<ID3D11Buffer> m_skybox_uniform_buffer;

	Glacier::D3D11RenderTarget m_depth_pass_rts[4];

	Glacier::ComPtr<ID3D11Buffer> m_light_structured_buffer;
	Glacier::ComPtr<ID3D11ShaderResourceView> m_light_srv;

	Glacier::ComPtr<ID3D11SamplerState> m_sampler_linear_wrap;
	Glacier::ComPtr<ID3D11SamplerState> m_sampler_shadow_comparison;

	Glacier::Object* m_skybox{ nullptr };
	Glacier::Object* m_globe{ nullptr };

	DrebelSubmarine* m_drebel{ nullptr };
	WaterJetSubmarine* m_water_jet_sub{ nullptr };

	void depth_pass() const noexcept;
	void color_pass() const noexcept;
	void display_to_screen() const noexcept;

	void render_globe() const noexcept;
	void render_skybox() const noexcept;

	void setup_lights() noexcept;
	void setup_cameras() noexcept;
	void setup_d3d() noexcept;

public:
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
