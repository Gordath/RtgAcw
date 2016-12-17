#ifndef RTGAPPLICATION_H_
#define RTGAPPLICATION_H_
#include "glacier_engine.h"

class RtgApplication : public Glacier::Application {
public:
	~RtgApplication();

	bool initialize(int *argc, char *argv[]) override;

	void update() noexcept override;

	void draw() noexcept override;

	int run() noexcept override;
};

#endif //RTGAPPLICATION_H_