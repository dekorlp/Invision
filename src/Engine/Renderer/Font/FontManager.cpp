#include "precompiled.h"

#include "FontManager.h"


namespace Invision
{
	FontManager::FontManager(std::shared_ptr <Invision::IGraphicsInstance> graphicsInstance, std::shared_ptr <Invision::IRenderPass> renderPass)
	{
		mGraphicsInstance = &graphicsInstance;
		mRenderPass = &renderPass;
	}

	void FontManager::RegisterFont(std::string font, std::string path)
	{
		Font *obj = new Font();
		obj->Generatefont(font, path, *mGraphicsInstance, *mRenderPass);
		mFonts.insert(std::pair<std::string, Font*>(font, obj));
		Invision::Vector2 vec2;
	}

	Character FontManager::GetCharacter(char character)
	{
		return mFonts["Arial"]->GetCharacter(character);
	}

	void FontManager::UnregisterFont(std::string font)
	{

	}

	void Character::SetCharacters(std::shared_ptr <Invision::IUniformBuffer> uniformBuffer, Vector2 size, Vector2 bearing, unsigned int advance, std::shared_ptr <Invision::IRenderPass> renderPass)
	{
	/*	mSize = size;
		mBearing = bearing;
		mAdvance = advance;

		auto vertShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/Examples/FontDemo/Shader/DrawTexture/vert.spv"));
		auto fragShaderCode = readFile(std::string(INVISION_BASE_DIR).append("/src/Examples/FontDemo/Shader/DrawTexture/frag.spv"));


		mPipeline->AddUniformBuffer(uniformBuffer);
		mPipeline->AddShader(vertShaderCode, Invision::SHADER_STAGE_VERTEX_BIT);
		mPipeline->AddShader(fragShaderCode, Invision::SHADER_STAGE_FRAGMENT_BIT);
		mPipeline->AddVertexDescription(bindingDescr);
		mPipeline->CreatePipeline(renderPass);*/

	}

	Vector2 Character::GetSize()
	{
		return mSize;
	}

	Vector2 Character::GetBearing()
	{
		return mBearing;
	}

	unsigned int Character::GetAdvance()
	{
		return mAdvance;
	}


	Character Font::GetCharacter(char character)
	{
		return mCharacters[character];
	}


	Font* Font::Generatefont(std::string font, std::string path, std::shared_ptr <Invision::IGraphicsInstance> graphicsInstance, std::shared_ptr <Invision::IRenderPass> renderPass)
	{
		FT_Library library;
		FT_Error error;
		FT_Face face;
		FT_UInt glyphIndex;
		std::shared_ptr <Invision::IUniformBuffer> uniformBuffer;
		uniformBuffer = graphicsInstance->CreateUniformBuffer();
		

		error = FT_Init_FreeType(&library);
		if (error)
		{
			throw std::runtime_error("An error occured during library initialization!");
		}
		
		error = FT_New_Face(library, path.c_str(), 0, &face);
		if (error == FT_Err_Unknown_File_Format)
		{
			throw std::runtime_error("Font File could not been opened!");
		}
		else if (error)
		{
			throw std::runtime_error("Font File could not been opened!");
		}

		error = FT_Set_Pixel_Sizes(face, 0, 48);
		if (error)
		{
			throw std::runtime_error("Font Char Size failed!");
		}

		// Create ASCII Set
		for (unsigned char c = 0; c < 128; c++)
		{
			// load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				throw std::runtime_error("Failed to load Glyph!");
				continue;
			}

		

			std::shared_ptr <Invision::ITexture> texture = graphicsInstance->CreateTexture();
			texture->CreateTexture((unsigned char*)face->glyph->bitmap_top, face->glyph->bitmap.width, face->glyph->bitmap.rows, Invision::FORMAT_R8G8B8A8_SRGB, false);
			texture->CreateTextureSampler(Invision::SAMPLER_FILTER_MODE_LINEAR, Invision::SAMPLER_FILTER_MODE_LINEAR, Invision::SAMPLER_ADDRESS_MODE_REPEAT, Invision::SAMPLER_ADDRESS_MODE_REPEAT, Invision::SAMPLER_ADDRESS_MODE_REPEAT);
			uniformBuffer->CreateUniformBinding(0, 0, 1, Invision::SHADER_STAGE_VERTEX_BIT, sizeof(UniformBufferObject)).CreateImageBinding(0, 1, 1, Invision::SHADER_STAGE_FRAGMENT_BIT, texture).CreateUniformBuffer();

			Character character;
			character.SetCharacters(uniformBuffer, { static_cast<float>(face->glyph->bitmap.width), static_cast<float>(face->glyph->bitmap.rows) },
				{ static_cast<float>(face->glyph->bitmap_left), static_cast<float>(face->glyph->bitmap_top) }, face->glyph->advance.x,renderPass);
			mCharacters.insert(std::pair<char, Character>(c, character));
		
		
		}

		mFontName = font;
		mFontPath = path;

		return this;
	}
}