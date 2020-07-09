#ifndef IPIPELINE_H
#define IPIPELINE_H


namespace Invision
{

	class IGraphicsEngine;

	class IPipeline
	{
		IGraphicsEngine* Engine = nullptr;

	public:
		INVISION_API IPipeline() = delete;
		INVISION_API IPipeline(IGraphicsEngine* engine);
		INVISION_API virtual ~IPipeline() = default;

		INVISION_API IGraphicsEngine* engine();

		INVISION_API virtual void render() = 0;
	};
}

#endif // IPIPELINE_H