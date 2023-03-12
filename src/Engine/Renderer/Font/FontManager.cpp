#include "precompiled.h"

#include "FontManager.h"


namespace Invision
{
	void FontManager::RegisterFont(std::string font, std::string path)
	{
		Font *obj = new Font();
		obj->Generatefont(font, path);
		mFonts.insert(std::pair<std::string, Font*>(font, obj));
		Invision::Vector2 vec2;
	}

	Character FontManager::test()
	{
		return mFonts["Arial"]->test();
	}

	void FontManager::UnregisterFont(std::string font)
	{

	}

	void Character::SetCharacters(int textureIndex, Vector2 size, Vector2 bearing, unsigned int advance)
	{
		mTextureIndex = textureIndex;
		mSize = size;
		mBearing = bearing;
		mAdvance = advance;
	}

	INVISION_API Character Font::test()
	{
		return mCharacters['a'];
	}

	Font* Font::Generatefont(std::string font, std::string path)
	{
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

			Character character;
			character.SetCharacters(c, { static_cast<float>(face->glyph->bitmap.width), static_cast<float>(face->glyph->bitmap.rows) },
				{ static_cast<float>(face->glyph->bitmap_left), static_cast<float>(face->glyph->bitmap_top) }, face->glyph->advance.x);
			mCharacters.insert(std::pair<char, Character>(c, character));
			mTextureArray.push_back(face->glyph->bitmap.buffer);
			mFontName = font;
			mFontPath = path;

		}

		return this;
	}
}