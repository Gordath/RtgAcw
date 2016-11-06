#define GLACIERENGINE_STATIC
#include <iostream>
#include <glacier_engine.h>
#include "rtg_application.h"


int main(int argc, char* argv[])
{
	RtgApplication app;

	app.initialize(&argc, argv);

	return app.run();
}
