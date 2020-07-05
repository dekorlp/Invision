#include "precompiled.h"

#include "IIndexBuffer.h"

#include "IGraphicsEngine.h"


namespace Invision
{
	IIndexBuffer::IIndexBuffer(IGraphicsEngine* engine) :
		Engine(engine)
	{
	}

	IGraphicsEngine* IIndexBuffer::engine()
	{
		return Engine;
	}

}