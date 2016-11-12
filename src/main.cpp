#define GLACIERENGINE_STATIC
#include <iostream>
#include <glacier_engine.h>
#include "rtg_application.h"
#include "D3D11_context.h"


int main(int argc, char* argv[])
{
	RtgApplication app;

	app.initialize(&argc, argv);

	int res = app.run();

	Glacier::D3D11Context *ctx = dynamic_cast<Glacier::D3D11Context*>(app.get_context());

	ctx->get_debug_interface()->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

	return res;
}
