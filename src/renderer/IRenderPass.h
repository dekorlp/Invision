#ifndef IRENDERPASS_H
#define IRENDERPASS_H

namespace Invision
{

	class IGraphicsEngine;
	class IGraphicsInstance;

	class IRenderPass
	{
		IGraphicsEngine* Engine = nullptr;
		IGraphicsInstance* instance = nullptr;

	public:
		INVISION_API IRenderPass() = delete;
		//INVISION_API IRenderPass(IGraphicsEngine* engine);
		INVISION_API IRenderPass(IGraphicsInstance* instance);
		INVISION_API virtual ~IRenderPass() = default;

		INVISION_API IGraphicsEngine* engine();
	};
}

#endif // IRENDERPASS_H
