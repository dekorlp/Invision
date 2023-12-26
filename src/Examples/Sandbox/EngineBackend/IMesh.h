#ifndef I_MESH_H
#define I_MESH_H
#include "InCommon.h"
#include "renderer/GraphicsFactory.h"

class IMesh
{
public:
	virtual void Initialize(const std::shared_ptr <Invision::IGraphicsInstance>& graphicsInstance, std::shared_ptr<Invision::IRenderPass>& renderPass) = 0;
	virtual std::shared_ptr <Invision::IVertexBuffer> GetVertexBuffer() = 0;
	virtual std::shared_ptr <Invision::IUniformBuffer> GetUniformBuffer() = 0;
	virtual std::shared_ptr <Invision::IIndexBuffer> GetIndexBuffer() = 0;
	virtual std::shared_ptr <Invision::IPipeline> GetPipeline() = 0;

	virtual bool HasIndexBuffer() = 0;
	virtual size_t GetVertexCount() = 0;

protected:
	
private:
	
};

#endif // I_MESH_H