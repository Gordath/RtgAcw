#ifndef GLACIER_ENGINE_TEXTURE_2D_H_
#define GLACIER_ENGINE_TEXTURE_2D_H_

namespace Glacier
{
	struct Texture2DDesc {

		unsigned int format;
		int mip_levels;
	};

	class Texture2D {
	private:
		int _id;

	public:
		virtual ~Texture2D() = default;

		virtual bool create() = 0;
		virtual void bind() = 0;
	};
}

#endif //GLACIER_ENGINE_TEXTURE_2D_H_