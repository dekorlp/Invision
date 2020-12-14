#ifndef IRENDERPASS_H
#define IRENDERPASS_H

namespace Invision
{
	class IGraphicsInstance;

	class IRenderPass
	{
		IGraphicsInstance* instance = nullptr;

	public:
		INVISION_API IRenderPass() = delete;
		//INVISION_API IRenderPass(IGraphicsEngine* engine);
		INVISION_API IRenderPass(IGraphicsInstance* instance);
		INVISION_API virtual ~IRenderPass() = default;
	};
}

#endif // IRENDERPASS_H
