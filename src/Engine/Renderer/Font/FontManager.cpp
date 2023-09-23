#include "precompiled.h"

#include "FontManager.h"


namespace Invision
{
	FontManager::FontManager(std::shared_ptr <Invision::IGraphicsInstance> graphicsInstance, std::shared_ptr <Invision::IRenderPass> renderPass)
	{
		mGraphicsInstance = &graphicsInstance;
		mRenderPass = &renderPass;
	}

	void FontManager::RegisterFont(std::string font, std::string path, std::vector<char> vertexShader, std::vector<char> fragmentShader)
	{
		Font *obj = new Font();
		obj->Generatefont(font, path, *mGraphicsInstance, *mRenderPass, vertexShader, fragmentShader);
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

	void Character::SetCharacters(std::shared_ptr <Invision::IUniformBuffer> uniformBuffer, std::shared_ptr <Invision::IPipeline> pipeline, Vector2 size, Vector2 bearing, unsigned int advance, std::shared_ptr <Invision::IRenderPass> renderPass, std::shared_ptr <Invision::ITexture> texture)
	{
		mSize = size;
		mBearing = bearing;
		mAdvance = advance;

		mPipeline = pipeline;
		mUniformBuffer = uniformBuffer;
		mTexture = texture;
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

	std::shared_ptr <Invision::IPipeline> Character::getPipeline()
	{
		return mPipeline;
	}

	std::shared_ptr <Invision::IUniformBuffer> Character::getUniformBuffer()
	{
		return mUniformBuffer;
	}

	Character Font::GetCharacter(char character)
	{
		return mCharacters[character];
	}


	Font* Font::Generatefont(std::string font, std::string path, std::shared_ptr <Invision::IGraphicsInstance> graphicsInstance, std::shared_ptr <Invision::IRenderPass> renderPass, std::vector<char> vertexShader, std::vector<char> fragmentShader)
	{

		const int fontsize = 48;
		int imageWidth = (fontsize + 2);
		int imageHeight = (fontsize + 2);

		FT_Library library;
		FT_Error error;
		FT_Face face;
		FT_UInt glyphIndex;

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

		error = FT_Set_Pixel_Sizes(face, 0, fontsize);
		if (error)
		{
			throw std::runtime_error("Font Char Size failed!");
		}

		int maxUnderBaseline = 0;
		for (int i = 32; i < 127; ++i)
		{
			// get the glyph index from character code
			glyphIndex = FT_Get_Char_Index(face, i);

			// load the glyph image into the slot
			error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
			if (error)
			{
				std::cout << "BitmapFontGenerator > failed to load glyph, error code: " << error << std::endl;
			}

			// get the glyph metrics
			const FT_Glyph_Metrics& glyphMetrics = face->glyph->metrics;

			// find the character that reaches below the baseline by the biggest value
			int glyphHang = (glyphMetrics.horiBearingY - glyphMetrics.height) / 64;
			if (glyphHang < maxUnderBaseline)
			{
				maxUnderBaseline = glyphHang;
			}
		}

		// Create ASCII Set
		for (int c = 0; c < 128; c++)
		{

			auto glyphIndex = FT_Get_Char_Index(face, c);

			error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
			if(error){
				throw std::runtime_error("Failed to load glyph");
			}

			FT_GlyphSlot glyphSlot = face->glyph;
			error = FT_Render_Glyph(glyphSlot, FT_RENDER_MODE_NORMAL);
			if (error)
			{
				throw std::runtime_error("Failed to render glyph");
			}


			std::shared_ptr <Invision::IUniformBuffer> uniformBuffer;
			uniformBuffer = graphicsInstance->CreateUniformBuffer();
		
			std::shared_ptr<Invision::IVertexBindingDescription> bindingDescr = graphicsInstance->CreateVertexBindingDescription();
			bindingDescr->CreateVertexBinding(0, sizeof(Vertex), Invision::VERTEX_INPUT_RATE_VERTEX)
				->CreateAttribute(0, Invision::FORMAT_R32G32_SFLOAT, offsetof(Vertex, position))
				.CreateAttribute(1, Invision::FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color))
				.CreateAttribute(2, Invision::FORMAT_R32G32_SFLOAT, offsetof(Vertex, texCoord));

			if (c == 'A')
			{
				int test = 0;
			}

			unsigned char* buffer = new unsigned char[imageWidth * imageHeight * 4];
			memset(buffer, 0, imageWidth * imageHeight * 4);

			// find the tile position where we have to draw the character
			int x = 0;
			int y = 0;
			x += 1; // 1 pixel padding from the left side of the tile
			y += (fontsize + 2) - face->glyph->bitmap_top + maxUnderBaseline - 1;

			// draw the character
			const FT_Bitmap& bitmap = face->glyph->bitmap;
			for (int xx = 0; xx < bitmap.width; ++xx)
			{
				for (int yy = 0; yy < bitmap.rows; ++yy)
				{
					unsigned char r = bitmap.buffer[(yy * (bitmap.width) + xx)];
					buffer[(y + yy) * imageWidth * 4 + (x + xx) * 4 + 0] = r;
					buffer[(y + yy) * imageWidth * 4 + (x + xx) * 4 + 1] = r;
					buffer[(y + yy) * imageWidth * 4 + (x + xx) * 4 + 2] = r;
					buffer[(y + yy) * imageWidth * 4 + (x + xx) * 4 + 3] = 255;
				}
			}

			std::shared_ptr <Invision::ITexture> texture = graphicsInstance->CreateTexture();
			texture->CreateTexture(buffer, imageWidth, imageHeight, Invision::FORMAT_R8G8B8A8_SRGB, false);
			texture->CreateTextureSampler(Invision::SAMPLER_FILTER_MODE_LINEAR, Invision::SAMPLER_FILTER_MODE_LINEAR, Invision::SAMPLER_ADDRESS_MODE_REPEAT, Invision::SAMPLER_ADDRESS_MODE_REPEAT, Invision::SAMPLER_ADDRESS_MODE_REPEAT);
			uniformBuffer->CreateUniformBinding(0, 0, 1, Invision::SHADER_STAGE_VERTEX_BIT, sizeof(UniformBufferObject)).CreateImageBinding(0, 1, 1, Invision::SHADER_STAGE_FRAGMENT_BIT, texture);
			uniformBuffer->CreateUniformBuffer();

			PipelineProperties *pipProp = new PipelineProperties();
			pipProp->mCullMode = CULL_MODE_NONE;

			std::shared_ptr <Invision::IPipeline> pipeline = graphicsInstance->CreatePipeline(pipProp);
			pipeline->AddUniformBuffer(uniformBuffer);
			pipeline->AddShader(vertexShader, Invision::SHADER_STAGE_VERTEX_BIT);
			pipeline->AddShader(fragmentShader, Invision::SHADER_STAGE_FRAGMENT_BIT);
			pipeline->AddVertexDescription(bindingDescr);
			pipeline->CreatePipeline(renderPass);
		

			Character character;
			character.SetCharacters(uniformBuffer, pipeline, { static_cast<float>(imageWidth), static_cast<float>(imageHeight) },
				{ static_cast<float>(glyphSlot->bitmap_left), static_cast<float>(glyphSlot->bitmap_top) }, glyphSlot->advance.x, renderPass, texture);
			mCharacters.insert(std::pair<char, Character>(c, character));
		
		}

		
		mFontName = font;
		mFontPath = path;

		return this;
	}
}