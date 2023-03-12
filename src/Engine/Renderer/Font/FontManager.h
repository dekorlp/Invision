#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <string>
#include <map>
#include "Math/Vector2.h"

#include "ft2build.h"
#include FT_FREETYPE_H

namespace Invision
{
	struct Character
	{
		private:
			unsigned int mTextureIndex;
			Vector2 mSize;
			Vector2 mBearing;
			unsigned int mAdvance;
		public:
			INVISION_API void SetCharacters(int textureIndex, Vector2 width, Vector2 bearing, unsigned int advance);
	};

	class Font
	{
		private:
			std::string mFontName;
			std::string mFontPath;
			std::map<char, Character> mCharacters;
			std::vector<unsigned char*> mTextureArray;
		public:
			INVISION_API Font* Generatefont(std::string font, std::string path);
			INVISION_API Character test();
	};

	class FontManager
	{
	private:
		std::map<std::string, Font*> mFonts;
	public:
		INVISION_API void RegisterFont(std::string font, std::string path);
		INVISION_API Character test();
		INVISION_API void UnregisterFont(std::string font);
	};
}
#endif // FONT_MANAGER_H
