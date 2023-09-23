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
#include "common/Types/StopWatch.h"

#include "math\Vector2.h"
#include "math\Vector3.h"
#include "math\Matrix.h"
#include "Renderer/Font/FontManager.h"

struct Vertex {
	Invision::Vector2 position;
	Invision::Vector3 color;
	Invision::Vector2 texCoord;
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
			//if (mIsInit == false)
			//	Init();
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
		
		graphicsInstance = graphicsEngine->CreateInstance(dim, renderPass, framebuffer, commandBuffer);
		vertexBuffer = graphicsInstance->CreateVertexBuffer();

		auto vertShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/Examples/FontDemo/Shader/DrawTexture/vert.spv"));
		auto fragShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/Examples/FontDemo/Shader/DrawTexture/frag.spv"));

		mFontManager = new Invision::FontManager(graphicsInstance, renderPass);
		mFontManager->RegisterFont("Arial", std::string(INVISION_BASE_DIR).append("/src/Examples/FontDemo/arial.ttf"), vertShaderCode, fragShaderCode);
		
		BuildCommandBuffer(this->size().width(), this->size().height());
		renderer = graphicsInstance->CreateRenderer();

		mIsInit = true;

	}


	void RenderWidget::BuildCommandBuffer(float width, float height)
	{
		commandBuffer->BeginCommandBuffer();
		commandBuffer->SetViewport({ 0, 0, (float)width, (float)height, 0.0, 1.0 });
		commandBuffer->SetScissor({ 0, 0, (uint32_t)width, (uint32_t)height });
		commandBuffer->BeginRenderPass(renderPass, framebuffer, 0, 0, width, height);

		DrawText("A", 25.0f, 25.0f, 1.0f);
		commandBuffer->EndRenderPass();
		commandBuffer->EndCommandBuffer();
	}

	void DrawText(std::string text, float x, float y, float scale)
	{
		std::shared_ptr<Invision::IVertexBindingDescription> bindingDescr = graphicsInstance->CreateVertexBindingDescription();
		bindingDescr->CreateVertexBinding(0, sizeof(Vertex), Invision::VERTEX_INPUT_RATE_VERTEX)
			->CreateAttribute(0, Invision::FORMAT_R32G32_SFLOAT, offsetof(Vertex, position))
			.CreateAttribute(1, Invision::FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color))
			.CreateAttribute(2, Invision::FORMAT_R32G32_SFLOAT, offsetof(Vertex, texCoord));

		

		auto vertShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/Examples/FontDemo/Shader/DrawFont/vert.spv"));
		auto fragShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/Examples/FontDemo/Shader/DrawFont/frag.spv"));

		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Invision::Character ch = mFontManager->GetCharacter(*c);
			float xpos = 0;
			float ypos = 0;

			float w = ch.GetSize().getX() * scale;
			float h= ch.GetSize().getY() * scale;

			const std::vector<Vertex> vertices = {
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
			{ {-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}}};

			vertexBuffer->CreateBuffer(vertices.data(), sizeof(vertices[0]) * vertices.size(), 0, bindingDescr);

			commandBuffer->BindPipeline(ch.getPipeline());
			commandBuffer->BindVertexBuffer({ vertexBuffer }, 0, 1);
			commandBuffer->BindDescriptorSets(ch.getUniformBuffer(), ch.getPipeline());

			commandBuffer->Draw(static_cast<uint32_t>(vertices.size()), 1, 0, 0);

			UniformBufferObject ubo = {};
			ubo.model = Invision::Matrix(1.0f) ;
		

			//Orthographic:
			ubo.proj = Invision::Matrix::Orthographic(-5.0f, 5.0f, 5.0f, -5.0f, 0.1f, 10.0f);

			// perspective
			//ubo.view = Invision::Matrix::Camera(Invision::Vector3(2.0f, 2.0f, 2.0f), Invision::Vector3(0.0f, 0.0f, 0.0f), Invision::Vector3(0.0f, 0.0f, 1.0f));
			//ubo.proj = Invision::Matrix::Perspective(45.0, this->size().width() / this->size().height(), 0.1f, 10.0f);

			ch.getUniformBuffer()->UpdateUniform(&ubo, sizeof(ubo), 0, 0);


		}
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
	std::shared_ptr <Invision::IFramebuffer> framebuffer;
	std::shared_ptr <Invision::ICommandBuffer> commandBuffer;
	std::shared_ptr <Invision::IRenderer> renderer;

	// timer for frequency adjusting
	Invision::StopWatch mTimer;
	Invision::FontManager *mFontManager;
};

#endif RENDER_WIDGET_H
