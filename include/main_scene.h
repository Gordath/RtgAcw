#ifndef MAIN_SCENE_H_
#define MAIN_SCENE_H_
#include "scene.h"

class MainScene : public Glacier::Scene {
public:
	void initialize() override;

	void on_key_down(unsigned char key, int x, int y) noexcept override;

	void on_key_up(unsigned char key, int x, int y) noexcept override;

	void on_mouse_motion(int x, int y) noexcept override;

	void on_mouse_click(int button, bool state, int x, int y) override;

	void update(float delta_time, long time) noexcept override;

	void draw() const noexcept override;
};

#endif //MAIN_SCENE_H_
