#include "rtg_application.h"

int main(int argc, char* argv[])
{
	RtgApplication app;

	if (!app.initialize(&argc, argv)) {
		return 1;
	}

	return app.run();
}
