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

//const std::vector<Vertex> vertices = {
//	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
//	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
//	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
//	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
//};

//const std::vector<uint32_t> indices = {
//	0, 1, 2, 2, 3, 0
//};

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
		UpdateUniformBuffer(this->size().width(), this->size().height());

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
		
		graphicsInstance = graphicsEngine->CreateInstance(dim, renderPass, framebuffer, commandBuffer);

		//renderPass = graphicsInstance->CreateRenderPass(); //graphicsEngine->CreateRenderPass();
		vertexBuffer = graphicsInstance->CreateVertexBuffer();
		uniformBuffer = graphicsInstance->CreateUniformBuffer();
		indexBuffer = graphicsInstance->CreateIndexBuffer();
		pipeline = graphicsInstance->CreatePipeline();
		//texture = graphicsInstance->CreateTexture();

		
		mFontManager.RegisterFont("Arial", std::string(INVISION_BASE_DIR).append("/src/Examples/FontDemo/arial.ttf"));

	/*	unsigned char* font = GenerateFontSet(12, width, height);
		std::vector<unsigned char*> texArray;
		texArray.push_back(font);*/

		

		//BuildCommandBuffer(this->size().width(), this->size().height());

		//commandBuffer->BeginCommandBuffer().
		//	SetViewport({ 0, 0, (float)width, (float)height, 0.0, 1.0 }).
		//	SetScissor({ 0, 0, (uint32_t)width, (uint32_t)height }).
		//	BeginRenderPass(renderPass, framebuffer, 0, 0, width, height).
		//	BindPipeline(pipeline).
		//	BindVertexBuffer({ vertexBuffer }, 0, 1).
		//	BindDescriptorSets(uniformBuffer, pipeline).
		//	BindIndexBuffer(indexBuffer, Invision::INDEX_TYPE_UINT32).
		//	//Draw(static_cast<uint32_t>(vertices.size()), 1, 0, 0).
		//	DrawIndexed(static_cast<uint32_t>(indices.size()), 1, 0, 0, 0).
		//	EndRenderPass().
		//	EndCommandBuffer();
		
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

		DrawText("H", 25.0f, 25.0f, 1.0f);
		commandBuffer->EndRenderPass();
		commandBuffer->EndCommandBuffer();
	}

	void DrawText(std::string text, float x, float y, float scale)
	{
		Invision::Vector2 dimensions(24, 30); //mFontManager.GetCharacter('a').GetSize();

		
		/*std::vector<unsigned char*> textureAtlas = mFontManager.GetTextureArray();*/
		

		std::shared_ptr<Invision::IVertexBindingDescription> bindingDescr = graphicsInstance->CreateVertexBindingDescription();
		bindingDescr->CreateVertexBinding(0, sizeof(Vertex), Invision::VERTEX_INPUT_RATE_VERTEX)
			->CreateAttribute(0, Invision::FORMAT_R32G32_SFLOAT, offsetof(Vertex, position))
			.CreateAttribute(1, Invision::FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color))
			.CreateAttribute(2, Invision::FORMAT_R32G32_SFLOAT, offsetof(Vertex, texCoord));

		

		auto vertShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/Examples/FontDemo/Shader/DrawTexture/vert.spv"));
		auto fragShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/Examples/FontDemo/Shader/DrawTexture/frag.spv"));

		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Invision::Character ch = mFontManager.GetCharacter(*c);

			texture = graphicsInstance->CreateTexture();
			//texture->CreateTexture(textureAtlas[0], ch.GetSize().getX(), ch.GetSize().getY(), Invision::FORMAT_R8G8B8A8_SRGB, false);
			texture->CreateTextureSampler(Invision::SAMPLER_FILTER_MODE_LINEAR, Invision::SAMPLER_FILTER_MODE_LINEAR, Invision::SAMPLER_ADDRESS_MODE_REPEAT, Invision::SAMPLER_ADDRESS_MODE_REPEAT, Invision::SAMPLER_ADDRESS_MODE_REPEAT);

			uniformBuffer->CreateUniformBinding(0, 0, 1, Invision::SHADER_STAGE_VERTEX_BIT, sizeof(UniformBufferObject)).CreateImageBinding(0, 1, 1, Invision::SHADER_STAGE_FRAGMENT_BIT, texture).CreateUniformBuffer();

			float xpos = x + ch.GetBearing().getX() * scale;
			float ypos = y - (ch.GetSize().getY() - ch.GetBearing().getY()) * scale;

			float w = ch.GetSize().getX() * scale;
			float h= ch.GetSize().getY() * scale;

			const std::vector<Vertex> vertices = {
			{ { xpos, ypos + h }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
			{ {xpos, ypos}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} },
			{ {xpos + w, ypos}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },

			{ {xpos, ypos + h}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
			{ {xpos + w, ypos}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} },
			{ {xpos + w, ypos + h}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} }
			};

			vertexBuffer->CreateBuffer(vertices.data(), sizeof(vertices[0]) * vertices.size(), 0, bindingDescr);

			pipeline->AddUniformBuffer(uniformBuffer);
			pipeline->AddShader(vertShaderCode, Invision::SHADER_STAGE_VERTEX_BIT);
			pipeline->AddShader(fragShaderCode, Invision::SHADER_STAGE_FRAGMENT_BIT);
			pipeline->AddVertexDescription(bindingDescr);
			pipeline->CreatePipeline(renderPass);

		
			commandBuffer->BindPipeline(pipeline);
			commandBuffer->BindVertexBuffer({ vertexBuffer }, 0, 1);
			commandBuffer->BindDescriptorSets(uniformBuffer, pipeline);
			commandBuffer->Draw(static_cast<uint32_t>(vertices.size()), 1, 0, 0);
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
	std::shared_ptr <Invision::IUniformBuffer> uniformBuffer;
	std::shared_ptr <Invision::IIndexBuffer> indexBuffer;
	std::shared_ptr <Invision::IPipeline> pipeline;
	std::shared_ptr <Invision::IFramebuffer> framebuffer;
	std::shared_ptr <Invision::ICommandBuffer> commandBuffer;
	std::shared_ptr <Invision::IRenderer> renderer;
	std::shared_ptr <Invision::ITexture> texture;

	// timer for frequency adjusting
	Invision::StopWatch mTimer;
	Invision::FontManager mFontManager;
};

#endif RENDER_WIDGET_H
