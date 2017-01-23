#ifndef RTGAPPLICATION_H_
#define RTGAPPLICATION_H_
#include "glacier_engine.h"

class RtgApplication : public Glacier::Application {
private:
	static void reshape(int x, int y);
	
	static void key_down(unsigned char key, int x, int y);
	
	static void key_up(unsigned char key, int x, int y);
	
	static void mouse_click(int button, bool state, int x, int y);
	
	static void mouse_motion(int x, int y);

	static void passive_mouse_motion(int x, int y);

public:
	RtgApplication() = default;

	~RtgApplication();

	bool initialize(int* argc, char* argv[]) override;

	void update() noexcept override;

	void draw() const noexcept override;

	int run() noexcept override;
};

#endif //RTGAPPLICATION_H_
