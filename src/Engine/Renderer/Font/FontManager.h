#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <string>
#include <map>
#include "Math/Vector2.h"
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

	struct Character
	{
		private:
			std::shared_ptr <Invision::IPipeline> mPipeline;
			Vector2 mSize;
			Vector2 mBearing;
			unsigned int mAdvance;
			std::shared_ptr <Invision::IUniformBuffer> uniformBuffer;
		public:
			INVISION_API void SetCharacters(std::shared_ptr <Invision::IUniformBuffer> uniformBuffer, Vector2 width, Vector2 bearing, unsigned int advance, std::shared_ptr <Invision::IRenderPass> renderPass);
			INVISION_API Vector2 GetSize();
			INVISION_API Vector2 GetBearing();
			INVISION_API unsigned int GetAdvance();
	};

	class Font
	{
		private:
			std::string mFontName;
			std::string mFontPath;
			std::map<char, Character> mCharacters;
			
		public:
			INVISION_API Font* Generatefont(std::string font, std::string path, std::shared_ptr <Invision::IGraphicsInstance> graphicsInstance, std::shared_ptr <Invision::IRenderPass> renderPass);
			INVISION_API Character GetCharacter(char character);
	};

	class FontManager
	{
	private:
		std::map<std::string, Font*> mFonts;
	public:
		FontManager(){}
		FontManager(std::shared_ptr <Invision::IGraphicsInstance> graphicsInstance, std::shared_ptr <Invision::IRenderPass> renderPass);
		INVISION_API void RegisterFont(std::string font, std::string path);
		INVISION_API Character GetCharacter(char character);
		INVISION_API void UnregisterFont(std::string font);
		std::shared_ptr <Invision::IGraphicsInstance> *mGraphicsInstance;
		std::shared_ptr <Invision::IRenderPass> *mRenderPass;
	};
}
#endif // FONT_MANAGER_H
