#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <string>
#include <map>
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Matrix.h"

#include "ft2build.h"
#include "Renderer/Common/IPipeline.h"
#include "Renderer/Common/IUniformBuffer.h"
#include "Renderer/Common/IGraphicsInstance.h"
#include "Renderer/Common/IGraphicsEngine.h"
#include "Renderer/Common/ITexture.h"
#include FT_FREETYPE_H

namespace Invision
{
	struct UniformBufferObject {
		Invision::Matrix model;
		Invision::Matrix view;
		Invision::Matrix proj;
	};


	struct Vertex {
		Invision::Vector2 position;
		Invision::Vector3 color;
		Invision::Vector2 texCoord;
	};


	struct Character
	{
		private:
			std::shared_ptr <Invision::IPipeline> mPipeline;
			Vector2 mSize;
			Vector2 mBearing;
			unsigned int mAdvance;
			std::shared_ptr <Invision::IUniformBuffer> mUniformBuffer;
			std::shared_ptr <Invision::ITexture> mTexture;
		public:
			INVISION_API void SetCharacters(std::shared_ptr <Invision::IUniformBuffer> uniformBuffer, std::shared_ptr <Invision::IPipeline> pipeline, Vector2 width, Vector2 bearing, unsigned int advance, std::shared_ptr <Invision::IRenderPass> renderPass, std::shared_ptr <Invision::ITexture>);
			INVISION_API Vector2 GetSize();
			INVISION_API Vector2 GetBearing();
			INVISION_API unsigned int GetAdvance();
			INVISION_API std::shared_ptr <Invision::IPipeline> getPipeline();
			INVISION_API std::shared_ptr <Invision::IUniformBuffer> getUniformBuffer();
	};

	class Font
	{
		private:
			std::string mFontName;
			std::string mFontPath;
			std::map<char, Character> mCharacters;

		public:
			INVISION_API Font* Generatefont(std::string font, std::string path, std::shared_ptr <Invision::IGraphicsInstance> graphicsInstance, std::shared_ptr <Invision::IRenderPass> renderPass, std::vector<char> vertexShader, std::vector<char> fragmentShader);
			INVISION_API Character GetCharacter(char character);
			INVISION_API ~Font();
	};

	class FontManager
	{
	private:
		std::map<std::string, Font*> mFonts;
	public:
		INVISION_API FontManager(){}
		INVISION_API FontManager(std::shared_ptr <Invision::IGraphicsInstance> graphicsInstance, std::shared_ptr <Invision::IRenderPass> renderPass);
		INVISION_API void RegisterFont(std::string font, std::string path, std::vector<char> vertexShader, std::vector<char> fragmentShader);
		INVISION_API Character GetCharacter(char character);
		INVISION_API void UnregisterFont(std::string font);
		std::shared_ptr <Invision::IGraphicsInstance> *mGraphicsInstance;
		std::shared_ptr <Invision::IRenderPass> *mRenderPass;
		INVISION_API ~FontManager();
	};
}
#endif // FONT_MANAGER_H
