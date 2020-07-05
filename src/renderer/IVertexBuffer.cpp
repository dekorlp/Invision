#include "precompiled.h"

#include "IVertexBuffer.h"

#include "IGraphicsEngine.h"


namespace Invision
{
	IVertexBuffer::IVertexBuffer(IGraphicsEngine* engine) :
		Engine(engine)
	{
	}

	IGraphicsEngine* IVertexBuffer::engine()
	{
		return Engine;
	}

}