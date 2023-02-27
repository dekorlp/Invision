#ifndef IRENDERPASS_H
#define IRENDERPASS_H

namespace Invision
{
	class IGraphicsInstance;
	class ITexture;
	enum AttachmentType;
	enum GfxFormat;

	class IRenderPass
	{
		IGraphicsInstance* mInstance = nullptr;

	public:
		INVISION_API IRenderPass() = delete;
		//INVISION_API IRenderPass(IGraphicsEngine* engine);
		INVISION_API IRenderPass(IGraphicsInstance* instance);
		INVISION_API virtual void AddAttachment(AttachmentType attachmentType, std::shared_ptr<ITexture>& attachment) = 0;
		INVISION_API virtual void CreateRenderPass() = 0;
		INVISION_API virtual bool IsMainRenderPass() = 0;
		INVISION_API virtual unsigned int GetCountOfColorAttachments() = 0;
		INVISION_API virtual ~IRenderPass() = default;
	};
}

#endif // IRENDERPASS_H
