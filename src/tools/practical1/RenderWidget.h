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
	{{-0.8, -0.7}, {1.0, 0.0, 0.0}}, // 0
{{-0.2, -0.4}, {1.0, 0.0, 0.0 } }, // 1
{ { -0.6, -0.3}, {1.0, 0.0, 0.0 }}, // 2
{ { -0.8,  0.0}, {1.0, 0.0, 0.0 }}, // 3
{ { -0.6,  0.7}, {1.0, 0.0, 0.0 }}, // 4
{ { -0.8,  1.0}, {1.0, 0.0, 0.0 }}, // 5
{ { -0.5,  0.9}, {1.0, 0.0, 0.0 }}, // 6
{ { -0.4,  0.5}, {1.0, 0.0, 0.0 }}, // 7
{ { -0.2,  0.8}, {1.0, 0.0, 0.0 }}, // 8
{ { -0.2,  0.4}, {1.0, 0.0, 0.0 }}, // 9
{ { -0.4, -0.1}, {1.0, 0.0, 0.0 }}, // 10


{ { 0.2, -0.8}, {1.0, 0.0, 0.0 }}, // 11
{ { 0.35,-0.8}, {1.0, 0.0, 0.0 }}, // 12
{ { 0.2,  0.0}, {1.0, 0.0, 0.0 }}, // 13
{ { 0.35, 1.0}, {1.0, 0.0, 0.0 }}, // 14
{ { 0.2,  1.0}, {1.0, 0.0, 0.0 }}, // 15
{ { 0.35, 0.3}, {1.0, 0.0, 0.0 }}, // 16
{ { 0.45, 0.1}, {1.0, 0.0, 0.0 }}, // 17
{ { 0.35,-0.1}, {1.0, 0.0, 0.0 }}, // 18
{ { 0.8,  0.87}, {1.0, 0.0, 0.0 }}, //19
{ { 1.0,  0.8}, {1.0, 0.0, 0.0 }}, // 20
{ { 0.9,  1.0}, {1.0, 0.0, 0.0 }}, // 21
{ { 1.0, -0.5}, {1.0, 0.0, 0.0 }}, // 22
{ { 0.9, -0.8}, {1.0, 0.0, 0.0 }}, // 23
};

const std::vector<uint32_t> indices = { 0, 1, 2,
				  0, 2, 3,
				  3, 2, 4,
				  3, 4, 5,
				  4, 6, 5,
				  4, 7, 6,
				  7, 8, 6,
				  7, 9, 8,
				  7, 10, 9,
				  10, 1, 9,
				  2, 1, 10,
				  1, 2, 0,


				  11, 12, 13,
				  13, 12, 14,
				  13, 14, 15,
				  16, 18, 17,
				  17, 19, 16,
				  17, 20, 19,
				  20, 21, 19,
				  18, 22, 17,
				  18, 23, 22,
};

/*const std::vector<Vertex> vertices = {
	{ { -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f } },
	{ { 0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f } },
	{ { 0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } },
	{ { -0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f } }
};

const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
};*/

struct UniformBufferObject {
	Invision::Matrix model;
	Invision::Matrix view;
	Invision::Matrix proj;
};

class RenderWidget : public QWidget
{
	Q_OBJECT;
public:
	explicit RenderWidget(QWidget* parent, std::shared_ptr <Invision::IGraphicsEngine> engine)
		:QWidget(parent)
	{
		setAttribute(Qt::WA_NativeWindow);
		setAttribute(Qt::WA_PaintOnScreen);
		setAttribute(Qt::WA_NoSystemBackground);

		graphicsEngine = engine;
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
		// my render code
		bool recreateSwapchainIsNecessary = false;
		recreateSwapchainIsNecessary = renderer->PrepareFrame();

		renderer->Draw(commandBuffer);

		recreateSwapchainIsNecessary = renderer->SubmitFrame();

		if (recreateSwapchainIsNecessary) RecreateSwapChain(this->size().width(), this->size().height());

		if (mContinousRender == true)
			Render();
	}

	void Init()
	{
		auto nativeWindowHandler = winId();
		
		Invision::CanvasDimensions dim = { HWND(nativeWindowHandler), this->size().width(), this->size().height() };
		//graphicsEngine = std::make_shared<Invision::VulkanEngine>(dim);
		
		graphicsInstance = graphicsEngine->CreateInstance(dim);

		renderPass = graphicsInstance->CreateRenderPass(); //graphicsEngine->CreateRenderPass();
		vertexBuffer = graphicsInstance->CreateVertexBuffer();
		uniformBuffer = graphicsInstance->CreateUniformBuffer();
		indexBuffer = graphicsInstance->CreateIndexBuffer();
		pipeline = graphicsInstance->CreatePipeline();


		vertexBuffer->CreateVertexBinding(sizeof(vertices[0]) * vertices.size(), vertices.data(), 0, sizeof(Vertex), Invision::VERTEX_INPUT_RATE_VERTEX)
			->CreateAttribute(0, Invision::FORMAT_R32G32_SFLOAT, offsetof(Vertex, position))
			.CreateAttribute(1, Invision::FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color));

		indexBuffer->CreateIndexBuffer(sizeof(indices[0]) * indices.size(), indices.data(), 0);

		uniformBuffer->CreateUniformBinding(0, 1, Invision::SHADER_STAGE_VERTEX_BIT, sizeof(UniformBufferObject), 0).CreateUniformBuffer();

		auto vertShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/tools/practical1/Shader/DrawUniformBuffer/vert.spv"));
		auto fragShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/tools/practical1/Shader/DrawUniformBuffer/frag.spv"));
		pipeline->AddUniformBuffer(uniformBuffer);
		pipeline->AddShader(vertShaderCode, Invision::SHADER_STAGE_VERTEX_BIT);
		pipeline->AddShader(fragShaderCode, Invision::SHADER_STAGE_FRAGMENT_BIT);
		pipeline->AddVertexBuffer(vertexBuffer);
		pipeline->CreatePipeline(renderPass);
		framebuffer = graphicsInstance->CreateFramebuffer(renderPass, graphicsInstance->GetSizeSwapchainImages());

		BuildCommandBuffer(this->size().width(), this->size().height());
		renderer = graphicsInstance->CreateRenderer();


		mIsInit = true;

	}

private:
	bool mContinousRender = false;
	bool mUpdatePending = false;
	bool mIsInit = false;

	// Vulkan Variables
	std::shared_ptr <Invision::IGraphicsEngine> graphicsEngine;
	std::shared_ptr <Invision::IGraphicsInstance> graphicsInstance;
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
