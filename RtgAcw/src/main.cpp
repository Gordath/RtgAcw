#include "rtg_application.h"

int main(int argc, char* argv[])
{
	RtgApplication app;

	app.initialize(&argc, argv);

	return app.run();
}
