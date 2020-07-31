#ifndef RENDER_WIDGET_H
#define RENDER_WIDGET_H

#include "qwidget.h"
#include "qapplication.h"
#include "QResizeEvent"

// minimize FPS
#define FIXED_FPS 60

// Vulkan and Engine Header
#include "AdditionalFunctions.h"
#include "InCommon.h"
#include "renderer/GraphicsFactory.h"
#include "common/StopWatch.h"

#include "math\Vector2.h"
#include "math\Vector3.h"
#include "math\Matrix.h"

struct Vertex {
	Invision::Vector2 position;
	Invision::Vector3 color;
};

const std::vector<Vertex> vertices = {
	{ { -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f } },
	{ { 0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f } },
	{ { 0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } },
	{ { -0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f } }
};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
};

struct UniformBufferObject {
	Invision::Matrix model;
	Invision::Matrix view;
	Invision::Matrix proj;
};

class RenderWidget : public QWidget
{
	Q_OBJECT;
public:
	explicit RenderWidget(QWidget* parent)
		:QWidget(parent)
	{
		setAttribute(Qt::WA_NativeWindow);
		setAttribute(Qt::WA_PaintOnScreen);
		setAttribute(Qt::WA_NoSystemBackground);
	}

	virtual ~RenderWidget() = default;

	virtual QPaintEngine* paintEngine() const override
	{
		return nullptr;
	}

	void RecreateSwapChain(const int width, const int height);
	void BuildCommandBuffer(float width, float height);
	void UpdateUniformBuffer(float width, float height);

	void Render()
	{
		if (mUpdatePending == false)
		{
			mUpdatePending = true;
			QApplication::postEvent(this, new QEvent{ QEvent::UpdateRequest });
		}
	}

	void SetContinousRender(bool renderContinous)
	{
		this->mContinousRender = renderContinous;

		if (renderContinous == true)
		{
			Render();
		}
	}

	bool GetContinousRender()
	{
		return mContinousRender;
	}

	protected:
		virtual void paintEvent(QPaintEvent* paintEvent) override
		{
			if (mIsInit == false)
				Init();
			Render();
		}

		virtual void showEvent(QShowEvent* showEvent) override
		{
			QWidget::showEvent(showEvent);
			if (mIsInit == false)
				Init();
		}

		virtual void resizeEvent(QResizeEvent* resizeEvent) override
		{
			QWidget::resizeEvent(resizeEvent);
			auto sz = resizeEvent->size();
			if ((sz.width() < 0) || (sz.height() < 0))
				return;

			// my resize Code 
			if(mIsInit == true)
			RecreateSwapChain(sz.width(), sz.height());

			// Has to be send, manually because QT does not send update request by resizing
			Render();
		}

		virtual bool event(QEvent* event) override
		{
			switch (event->type())
			{
			case QEvent::UpdateRequest:
				mUpdatePending = false;
				DoRender();
				return true;
			default:
				return QWidget::event(event);
			}
		}

private:
	void DoRender()
	{
		if (isVisible() == false)
			return;
		if (mIsInit == false)
			return;


		// limit framerate
		mTimer.stop();
		if (mTimer.getElapsedMilliseconds() < 1000 / FIXED_FPS)
		{
			long long delta_ms = (1000 / FIXED_FPS - mTimer.getElapsedMilliseconds());
			std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms));
		}
		mTimer.start();

		// my render code
		bool recreateSwapchainIsNecessary = false;

		recreateSwapchainIsNecessary = renderer->PrepareFrame();

		UpdateUniformBuffer(this->size().width(), this->size().height());

		recreateSwapchainIsNecessary = renderer->SubmitFrame(commandBuffer);


		if (recreateSwapchainIsNecessary) RecreateSwapChain(this->size().width(), this->size().height());


		if (mContinousRender == true)
			Render();
	}

	void Init()
	{
		auto nativeWindowHandler = winId();

		// initialization Code
		std::vector<const char*> requiredExtensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };

		//vulkInstance = vulkanInstance.Init("Hello World", "Invision", VK_MAKE_VERSION(1, 0, 0), VK_MAKE_VERSION(1, 0, 0), requiredExtensions);
		//vulkanInstance.SetDebugMessanger(debugCallback);
		
		Invision::CanvasDimensions dim = { HWND(nativeWindowHandler), this->size().width(), this->size().height() };
		//graphicsEngine = std::make_shared<Invision::VulkanEngine>(dim);
		graphicsEngine = Invision::create_engine(Invision::EngineType::Vulkan);
		graphicsEngine->Init(dim);
		renderPass = graphicsEngine->CreateRenderPass();
		vertexBuffer = graphicsEngine->CreateVertexBuffer();
		uniformBuffer = graphicsEngine->CreateUniformBuffer();
		indexBuffer = graphicsEngine->CreateIndexBuffer();
		pipeline = graphicsEngine->CreatePipeline();


		vertexBuffer->CreateVertexBuffer(sizeof(vertices[0]) * vertices.size(), vertices.data(), 0);
		vertexBuffer->CreateVertexInput(0, sizeof(Vertex), Invision::VERTEX_INPUT_RATE_VERTEX)
			.CreateAttribute(0, Invision::FORMAT_R32G32_SFLOAT, offsetof(Vertex, position))
			.CreateAttribute(1, Invision::FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color));

		indexBuffer->CreateIndexBuffer(sizeof(indices[0]) * indices.size(), indices.data(), 0);

		uniformBuffer->CreateUniformBinding(0, 1, Invision::SHADER_STAGE_VERTEX_BIT, sizeof(UniformBufferObject), 0).CreateUniformBuffer();

		auto vertShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/tools/examples/triangle/Shader/DrawUniformBuffer/vert.spv"));
		auto fragShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/tools/examples/triangle/Shader/DrawUniformBuffer/frag.spv"));
		pipeline->AddUniformBuffer(uniformBuffer);
		pipeline->AddShader(vertShaderCode, Invision::SHADER_STAGE_VERTEX_BIT);
		pipeline->AddShader(fragShaderCode, Invision::SHADER_STAGE_FRAGMENT_BIT);
		pipeline->AddVertexBuffer(vertexBuffer);
		pipeline->CreatePipeline(renderPass);
		framebuffer = graphicsEngine->CreateFramebuffer(renderPass);

		BuildCommandBuffer(this->size().width(), this->size().height());
		renderer = graphicsEngine->CreateRenderer();


		mIsInit = true;

	}

private:
	bool mContinousRender = false;
	bool mUpdatePending = false;
	bool mIsInit = false;

	// Vulkan Variables
	std::shared_ptr <Invision::IGraphicsEngine> graphicsEngine;
	std::shared_ptr <Invision::IRenderPass> renderPass;
	std::shared_ptr <Invision::IVertexBuffer> vertexBuffer;
	std::shared_ptr <Invision::IUniformBuffer> uniformBuffer;
	std::shared_ptr <Invision::IIndexBuffer> indexBuffer;
	std::shared_ptr <Invision::IPipeline> pipeline;
	std::shared_ptr <Invision::IFramebuffer> framebuffer;
	std::shared_ptr <Invision::ICommandBuffer> commandBuffer;
	std::shared_ptr <Invision::IRenderer> renderer;

	// timer for frequency adjusting
	Invision::StopWatch mTimer;
};

#endif RENDER_WIDGET_H