#ifndef MAIN_SCENE_H_
#define MAIN_SCENE_H_
#include "scene.h"

class MainScene : public Glacier::Scene {
public:

	void initialize() override;

	void on_key_down(int key, int x, int y) noexcept override;

	void on_key_up(int key, int x, int y) noexcept override;

	void draw() const noexcept override;
};

#endif //MAIN_SCENE_H_
