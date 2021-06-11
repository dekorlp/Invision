#ifndef RENDER_WIDGET_H
#define RENDER_WIDGET_H

#include "qwidget.h"
#include "qapplication.h"
#include "QResizeEvent"

// minimize FPS
#define FIXED_FPS 60
#define LIMIT_FPS

// Vulkan and Engine Header

#include "Engine/InCommon.h"
#include "Engine/renderer/GraphicsFactory.h"
#include "Engine/common/StopWatch.h"
#include "Engine/input/IKeyboard.h"
#include "Engine/input/WindowsKeyboard.h"

#include "Engine\math\Vector2.h"
#include "Engine\math\Vector3.h"
#include "Engine\math\Matrix.h"




/*struct Vertex {
	Invision::Vector3 position;
	Invision::Vector3 color;
	Invision::Vector2 texCoord;
};*/

#include "Vertex.h"

#include "AdditionalFunctions.h"




struct UniformBufferObject {
	Invision::Matrix model;
	Invision::Matrix view;
	Invision::Matrix proj;
};

struct UniformLightBuffer {
	Invision::Vector3 lightPos;
	Invision::Vector3 lightColor;
	Invision::Vector3 viewPos;
};

struct UniformOptionsBuffer {
	int option;
};

struct GBuffer
{
	std::shared_ptr <Invision::IPipeline> gPipeline;
	std::shared_ptr <Invision::IRenderPass> gRenderPass;
	std::shared_ptr <Invision::IFramebuffer> gFramebuffer;
	std::shared_ptr <Invision::ICommandBuffer> gCommandbuffer;
	std::shared_ptr <Invision::ITexture> positionsAttachment;
	std::shared_ptr <Invision::ITexture> albedoAttachment;
	std::shared_ptr <Invision::ITexture> normalAttachment;
	std::shared_ptr <Invision::ITexture> depthAttachment;
};

#define FRAMEBUFFER_SIZE 2048

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
			//if (mIsInit == false)
				//Init();
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
				Run();
				return true;
			default:
				return QWidget::event(event);
			}
		}

private:

	void Run()
	{
		if (isVisible() == false)
			return;
		if (mIsInit == false)
			return;

		mTimer.stop();
		accumulatedTime += mTimer.getElapsedMilliseconds();
		while (accumulatedTime >=  dt)
		{
			DoUpdate(dt);
			accumulatedTime -=  dt;
		}


		mTimer.start();

#ifdef LIMIT_FPS
		// limit to FIXED_FPS
		long long delta_ms = (1000 / FIXED_FPS - accumulatedTime);
		std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms));
#endif 

		DoRender(accumulatedTime / dt);
		
	}
	void DoUpdate(double dt)
	{	
		if (keyboard->GetStateOfKey(Invision::INVISION_KEY_A, Invision::INVISION_KEY_PRESSED))
		{
			pos.SetY( pos.getY() + 0.005f * dt);
		}
		if (keyboard->GetStateOfKey(Invision::INVISION_KEY_D, Invision::INVISION_KEY_PRESSED))
		{
			pos.SetY(pos.getY() - 0.005f * dt);
		}
		if (keyboard->GetStateOfKey(Invision::INVISION_KEY_W, Invision::INVISION_KEY_PRESSED))
		{
			pos.SetX(pos.getX() + 0.005f * dt);
		}
		if (keyboard->GetStateOfKey(Invision::INVISION_KEY_S, Invision::INVISION_KEY_PRESSED))
		{
			pos.SetX(pos.getX() - 0.005f * dt);
		}
		if (keyboard->GetStateOfKey(Invision::INVISION_KEY_ADD, Invision::INVISION_KEY_PRESSED))
		{
			scale += 0.005f * dt;
		}
		if (keyboard->GetStateOfKey(Invision::INVISION_KEY_SUBTRACT, Invision::INVISION_KEY_PRESSED))
		{
			scale -= 0.005f * dt;
		}
		if (keyboard->GetStateOfKey(Invision::INVISION_KEY_LEFT_ARROW, Invision::INVISION_KEY_PRESSED))
		{
			phi -= 0.001f * dt;
		}
		if (keyboard->GetStateOfKey(Invision::INVISION_KEY_RIGHT_ARROW, Invision::INVISION_KEY_PRESSED))
		{
			phi += 0.001f * dt;
		}
		if (keyboard->GetStateOfKey(Invision::INVISION_KEY_UP_ARROW, Invision::INVISION_KEY_PRESSED))
		{
			theta -= 0.001f *dt;
		}
		if (keyboard->GetStateOfKey(Invision::INVISION_KEY_DOWN_ARROW, Invision::INVISION_KEY_PRESSED))
		{
			theta += 0.001f *dt;
		}

		// options configuration
		if (keyboard->GetStateOfKey(Invision::INVISION_KEY_NUM_1, Invision::INVISION_KEY_PRESSED))
		{
			// Set Options
			UniformOptionsBuffer uob;
			uob.option = 1;
			DeferredUniformBuffer->UpdateUniform(&uob, sizeof(uob), 0, 3);
		}
		if (keyboard->GetStateOfKey(Invision::INVISION_KEY_NUM_2, Invision::INVISION_KEY_PRESSED))
		{
			// Set Options
			UniformOptionsBuffer uob;
			uob.option = 2;
			DeferredUniformBuffer->UpdateUniform(&uob, sizeof(uob), 0, 3);
		}
		if (keyboard->GetStateOfKey(Invision::INVISION_KEY_NUM_3, Invision::INVISION_KEY_PRESSED))
		{
			// Set Options
			UniformOptionsBuffer uob;
			uob.option = 3;
			DeferredUniformBuffer->UpdateUniform(&uob, sizeof(uob), 0, 3);
		}


		bool spaceCurrentlyPressed = keyboard->GetStateOfKey(Invision::INVISION_KEY_SPACE, Invision::INVISION_KEY_PRESSED);

		if (!spacePressed && spaceCurrentlyPressed) {
			switchFixedCamera = !switchFixedCamera;
		}
		spacePressed = spaceCurrentlyPressed;
		

		angle += 0.05f * dt;
	}

	void DoRender(double dt)
	{

		// my render code
		bool recreateSwapchainIsNecessary = false;
		recreateSwapchainIsNecessary = renderer->PrepareFrame();
		renderer->Draw(mGBuffer.gCommandbuffer);
		renderer->Draw(commandBuffer);

		recreateSwapchainIsNecessary = renderer->SubmitFrame();

		if (recreateSwapchainIsNecessary) RecreateSwapChain(this->size().width(), this->size().height());

		if (mContinousRender == true)
			Render();
		UpdateUniformBuffer(this->size().width(), this->size().height());	
	}

	void Init()
	{


		auto nativeWindowHandler = winId();
		
		int width, height, channels;

		pos = Invision::Vector3(0.0f, 0.0f, 0.0f);
		keyboard = std::make_shared<Invision::WindowsKeyboard>();

		Invision::CanvasDimensions dim = { HWND(nativeWindowHandler), this->size().width(), this->size().height() };
		//graphicsEngine = std::make_shared<Invision::VulkanEngine>(dim);
		
		graphicsInstance = graphicsEngine->CreateInstance(dim, renderPass, framebuffer, commandBuffer, true);

		//renderPass = graphicsInstance->CreateRenderPass(); //graphicsEngine->CreateRenderPass();
		vertexBuffer = graphicsInstance->CreateVertexBuffer();
		uniformBuffer = graphicsInstance->CreateUniformBuffer();
		indexBuffer = graphicsInstance->CreateIndexBuffer();

		uniformBufferGrass = graphicsInstance->CreateUniformBuffer();
		vertexBufferGrass = graphicsInstance->CreateVertexBuffer();
		indexBufferGrass = graphicsInstance->CreateIndexBuffer();
	

		unsigned char* pixelsGrass = readPNG(std::string(INVISION_BASE_DIR).append("/src/Examples/AlphaBlending/Textures/kisspng-texture-mapping-drawing-lawn-tall-grass-texture-alpha-5ab1d0d27d3617.9249521415216027705129.png"), width, height, channels);
		textureGrass = graphicsInstance->CreateTexture(pixelsGrass, width, height, Invision::FORMAT_R8G8B8A8_SRGB, true);
		freeImage(pixelsGrass);
		textureGrass->CreateTextureSampler(Invision::SAMPLER_FILTER_MODE_NEAREST, Invision::SAMPLER_FILTER_MODE_NEAREST, Invision::SAMPLER_ADDRESS_MODE_REPEAT, Invision::SAMPLER_ADDRESS_MODE_REPEAT, Invision::SAMPLER_ADDRESS_MODE_REPEAT);

		unsigned char* pixelsWall = readPNG(std::string(INVISION_BASE_DIR).append("/src/Examples/AlphaBlending/Textures/brick_wall_001_diffuse_1k.png"), width, height, channels);
		textureBrick = graphicsInstance->CreateTexture(pixelsWall, width, height, Invision::FORMAT_R8G8B8A8_SRGB, true);
		freeImage(pixelsWall);
		textureBrick->CreateTextureSampler(Invision::SAMPLER_FILTER_MODE_NEAREST, Invision::SAMPLER_FILTER_MODE_NEAREST, Invision::SAMPLER_ADDRESS_MODE_CLAMP, Invision::SAMPLER_ADDRESS_MODE_CLAMP, Invision::SAMPLER_ADDRESS_MODE_CLAMP);

		// gPass Initialization
		mGBuffer.gPipeline = graphicsInstance->CreatePipeline();
		mGBuffer.gRenderPass = graphicsInstance->CreateRenderPass();
		mGBuffer.positionsAttachment = graphicsInstance->CreateColorAttachment(FRAMEBUFFER_SIZE, FRAMEBUFFER_SIZE, Invision::FORMAT_R16G16B16A16_SFLOAT);
		mGBuffer.albedoAttachment = graphicsInstance->CreateColorAttachment(FRAMEBUFFER_SIZE, FRAMEBUFFER_SIZE, Invision::FORMAT_R16G16B16A16_SFLOAT);
		mGBuffer.normalAttachment = graphicsInstance->CreateColorAttachment(FRAMEBUFFER_SIZE, FRAMEBUFFER_SIZE, Invision::FORMAT_R16G16B16A16_SFLOAT);
		mGBuffer.depthAttachment = graphicsInstance->CreateDepthAttachment(FRAMEBUFFER_SIZE, FRAMEBUFFER_SIZE);

		// gPass Sampler Settings
		mGBuffer.positionsAttachment->CreateTextureSampler(Invision::SAMPLER_FILTER_MODE_NEAREST, Invision::SAMPLER_FILTER_MODE_NEAREST, Invision::SAMPLER_ADDRESS_MODE_CLAMP, Invision::SAMPLER_ADDRESS_MODE_CLAMP, Invision::SAMPLER_ADDRESS_MODE_CLAMP);
		mGBuffer.albedoAttachment->CreateTextureSampler(Invision::SAMPLER_FILTER_MODE_NEAREST, Invision::SAMPLER_FILTER_MODE_NEAREST, Invision::SAMPLER_ADDRESS_MODE_CLAMP, Invision::SAMPLER_ADDRESS_MODE_CLAMP, Invision::SAMPLER_ADDRESS_MODE_CLAMP);
		mGBuffer.normalAttachment->CreateTextureSampler(Invision::SAMPLER_FILTER_MODE_NEAREST, Invision::SAMPLER_FILTER_MODE_NEAREST, Invision::SAMPLER_ADDRESS_MODE_CLAMP, Invision::SAMPLER_ADDRESS_MODE_CLAMP, Invision::SAMPLER_ADDRESS_MODE_CLAMP);
		mGBuffer.depthAttachment->CreateTextureSampler(Invision::SAMPLER_FILTER_MODE_NEAREST, Invision::SAMPLER_FILTER_MODE_NEAREST, Invision::SAMPLER_ADDRESS_MODE_CLAMP, Invision::SAMPLER_ADDRESS_MODE_CLAMP, Invision::SAMPLER_ADDRESS_MODE_CLAMP);

		// gPass to RenderPass
		mGBuffer.gRenderPass->AddAttachment(Invision::ATTACHMENT_TYPE_COLOR, mGBuffer.positionsAttachment); // world Space Positions
		mGBuffer.gRenderPass->AddAttachment(Invision::ATTACHMENT_TYPE_COLOR, mGBuffer.normalAttachment); // Normals
		mGBuffer.gRenderPass->AddAttachment(Invision::ATTACHMENT_TYPE_COLOR, mGBuffer.albedoAttachment); // Albedo
		mGBuffer.gRenderPass->AddAttachment(Invision::ATTACHMENT_TYPE_DEPTH, mGBuffer.depthAttachment); // Depth
		mGBuffer.gRenderPass->CreateRenderPass();

		mGBuffer.gFramebuffer = graphicsInstance->CreateFramebuffer(mGBuffer.gRenderPass, FRAMEBUFFER_SIZE, FRAMEBUFFER_SIZE);
		mGBuffer.gCommandbuffer = graphicsInstance->CreateCommandBuffer(mGBuffer.gFramebuffer);

		std::vector<Invision::Vector3> positions;
		std::vector<Invision::Vector2> texCoords;

		LoadModel(std::string(INVISION_BASE_DIR).append("/src/Examples/AlphaBlending/Models/cube.obj"), vertices, indices);
		LoadModel(std::string(INVISION_BASE_DIR).append("/src/Examples/AlphaBlending/Models/plane.obj"), verticesGrass, indicesGrass);
		
		std::shared_ptr<Invision::IVertexBindingDescription> bindingDescr = graphicsInstance->CreateVertexBindingDescription();
		bindingDescr->CreateVertexBinding(0, sizeof(Vertex), Invision::VERTEX_INPUT_RATE_VERTEX)
			->CreateAttribute(0, Invision::FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position))
			.CreateAttribute(1, Invision::FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color))
			.CreateAttribute(2, Invision::FORMAT_R32G32_SFLOAT, offsetof(Vertex, texCoord))
			.CreateAttribute(3, Invision::FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal));

		vertexBuffer->CreateBuffer(vertices.data(), sizeof(vertices[0]) * vertices.size(), 0, bindingDescr);
		vertexBufferGrass->CreateBuffer(verticesGrass.data(), sizeof(verticesGrass[0]) * verticesGrass.size(), 0, bindingDescr);

		indexBuffer->CreateIndexBuffer(sizeof(indices[0]) * indices.size(), indices.data());
		indexBufferGrass->CreateIndexBuffer(sizeof(indicesGrass[0]) * indicesGrass.size(), indicesGrass.data());

		uniformBuffer->CreateUniformBinding(0, 0, 1, Invision::SHADER_STAGE_VERTEX_BIT, sizeof(UniformBufferObject))
			.CreateImageBinding(0, 1, 1, Invision::SHADER_STAGE_FRAGMENT_BIT, textureBrick).
			CreateUniformBuffer();

		uniformBufferGrass->CreateUniformBinding(0, 0, 1, Invision::SHADER_STAGE_VERTEX_BIT, sizeof(UniformBufferObject))
			.CreateImageBinding(0, 1, 1, Invision::SHADER_STAGE_FRAGMENT_BIT, textureGrass).
			CreateUniformBuffer();

		auto vertShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/Examples/AlphaBlending/Shader/AlphaBlending/gbuffer.vert.spv"));
		auto fragShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/Examples/AlphaBlending/Shader/AlphaBlending/gbuffer.frag.spv"));
		mGBuffer.gPipeline->AddUniformBuffer(uniformBuffer);
		mGBuffer.gPipeline->AddShader(vertShaderCode, Invision::SHADER_STAGE_VERTEX_BIT);
		mGBuffer.gPipeline->AddShader(fragShaderCode, Invision::SHADER_STAGE_FRAGMENT_BIT);
		mGBuffer.gPipeline->AddVertexDescription(bindingDescr);
		mGBuffer.gPipeline->CreatePipeline(mGBuffer.gRenderPass);

		auto vertShaderCodeGrass = readFile(std::string(INVISION_BASE_DIR).append("/src/Examples/AlphaBlending/Shader/AlphaBlending/grass.vert.spv"));
		auto fragShaderCodeGrass = readFile(std::string(INVISION_BASE_DIR).append("/src/Examples/AlphaBlending/Shader/AlphaBlending/grass.frag.spv"));
		pipelineGrass = graphicsInstance->CreatePipeline();
		pipelineGrass->AddUniformBuffer(uniformBufferGrass);
		pipelineGrass->AddShader(vertShaderCodeGrass, Invision::SHADER_STAGE_VERTEX_BIT);
		pipelineGrass->AddShader(fragShaderCodeGrass, Invision::SHADER_STAGE_FRAGMENT_BIT);
		pipelineGrass->SetColorBlendFunction();
		pipelineGrass->SetDepthWrite(false);
		pipelineGrass->AddVertexDescription(bindingDescr);
		pipelineGrass->CreatePipeline(mGBuffer.gRenderPass);


		// Deferred Shading Initialization
		pipeline = graphicsInstance->CreatePipeline(&Invision::PipelineProperties(Invision::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, Invision::POLYGON_MODE_FILL, Invision::CULL_MODE_FRONT_BIT, Invision::FRONT_FACE_COUNTER_CLOCKWISE, 1.0f));
		pipeline->SetDepthWrite(false);

		DeferredUniformBuffer = graphicsInstance->CreateUniformBuffer();
		DeferredUniformBuffer->CreateImageBinding(0, 0, 1, Invision::SHADER_STAGE_FRAGMENT_BIT, mGBuffer.albedoAttachment).CreateImageBinding(0, 1, 1, Invision::SHADER_STAGE_FRAGMENT_BIT, mGBuffer.normalAttachment).CreateImageBinding(0, 2, 1, Invision::SHADER_STAGE_FRAGMENT_BIT, mGBuffer.positionsAttachment).CreateUniformBinding(0, 3, 1, Invision::SHADER_STAGE_FRAGMENT_BIT, sizeof(UniformOptionsBuffer)).CreateUniformBinding(0, 4, 1, Invision::SHADER_STAGE_VERTEX_BIT | Invision::SHADER_STAGE_FRAGMENT_BIT, sizeof(UniformLightBuffer)).CreateUniformBuffer();

		auto deferredVertShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/Examples/AlphaBlending/Shader/AlphaBlending/deferred.vert.spv"));
		auto deferredFragShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/Examples/AlphaBlending/Shader/AlphaBlending/deferred.frag.spv"));
		pipeline->AddShader(deferredVertShaderCode, Invision::SHADER_STAGE_VERTEX_BIT);
		pipeline->AddShader(deferredFragShaderCode, Invision::SHADER_STAGE_FRAGMENT_BIT);
		pipeline->AddUniformBuffer(DeferredUniformBuffer);
		pipeline->CreatePipeline(renderPass);




		BuildCommandBuffer(this->size().width(), this->size().height());
		renderer = graphicsInstance->CreateRenderer();

		// set light
		UniformLightBuffer light;
		light.lightColor = { 1.0, 1.0, 1.0 };
		light.lightPos = { 1.2f, 1.0f, 2.0f };
		light.viewPos = { 0.0f, 0.0f, 0.0f };
		DeferredUniformBuffer->UpdateUniform(&light, sizeof(light), 0, 4);

		// Set Options
		UniformOptionsBuffer uob;
		uob.option = 1;
		DeferredUniformBuffer->UpdateUniform(&uob, sizeof(uob), 0, 3);

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
	
	std::shared_ptr <Invision::IUniformBuffer> DeferredUniformBuffer;
	std::shared_ptr <Invision::IUniformBuffer> uniformBuffer;
	std::shared_ptr <Invision::IUniformBuffer> uniformBufferGrass;
	std::shared_ptr <Invision::IVertexBuffer> vertexBuffer;
	std::shared_ptr <Invision::IIndexBuffer> indexBuffer;
	std::shared_ptr <Invision::IPipeline> pipeline;
	std::shared_ptr <Invision::IFramebuffer> framebuffer;
	std::shared_ptr <Invision::ICommandBuffer> commandBuffer;
	std::shared_ptr <Invision::IRenderer> renderer;
	std::shared_ptr <Invision::ITexture> textureBrick;
	std::shared_ptr <Invision::ITexture> textureGrass;
	std::shared_ptr <Invision::IVertexBuffer> vertexBufferGrass;
	std::shared_ptr <Invision::IIndexBuffer> indexBufferGrass;
	std::shared_ptr <Invision::IPipeline> pipelineGrass;

	std::shared_ptr<Invision::IKeyboard> keyboard;

	std::shared_ptr <Invision::ITexture> debugTexture;
	
	GBuffer mGBuffer;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	std::vector<Vertex> verticesGrass;
	std::vector<uint32_t> indicesGrass;
	// timer for frequency adjusting
	Invision::StopWatch mTimer;
	const double dt = 1000 / FIXED_FPS;
	double accumulatedTime = 0.0;

	bool spacePressed;
	bool switchFixedCamera = false;

	float angle = 0;
	Invision::Vector3 pos;

	float theta = 0.0f;
	float phi = 0.0f;
	float radius = 3.0f;

	float scale = 1;
	bool activateDepthTesting = true;
	bool activateMSAA = true;


};

#endif RENDER_WIDGET_H
