#ifndef GLACIER_SERVICE_H_
#define GLACIER_SERVICE_H_

namespace Glacier
{
	enum class ServiceType {
		SRV_INPUT,
		SRV_PARTICLE,
		SRV_AUDIO,
		SRV_LOGGER,
		SRV_WINDOWING,
		SRV_NULL
	};

	class Service {
	protected:
		ServiceType _type;

		Service(ServiceType type) : _type(type) {};

	public:
		ServiceType get_type() const { return _type; }
	};
}

#endif //GLACIER_SERVICE_H_