#include "AdditionalFunctions.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ft2build.h"
#include FT_FREETYPE_H

std::vector<char> readFile(const std::string& filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}


unsigned char* readJPG(const std::string& filename, int &width, int &height, int &channels)
{
	stbi_uc* pixels = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if (!pixels) {
		throw std::runtime_error("failed to load texture image!");
	}


	return pixels;
}

void freeImage(unsigned char* pixels)
{
	stbi_image_free(pixels);
}

unsigned char* GenerateFontSet(const int fontsize, int& width, int& height)
{
	FT_Library library;
	FT_Error error;
	FT_Face face;
	FT_UInt glyphIndex;


	error = FT_Init_FreeType(&library);
	if(error)
	{
		throw std::runtime_error("An error occured during library initialization!");
	}

	error = FT_New_Face(library, std::string(INVISION_BASE_DIR).append("/src/Examples/FontDemo/arial.ttf").c_str(), 0, &face);
	if (error == FT_Err_Unknown_File_Format)
	{
		throw std::runtime_error("Font File could not been opened!");
	}
	else if (error)
	{
		throw std::runtime_error("Font File could not been opened!");
	}

	error = FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(fontsize));

	if (error)
	{
		throw std::runtime_error("Font Char Size failed!");
	}



	width = (fontsize + 2) * 16;
	height = (fontsize + 2) * 8;

	unsigned char* buffer = new unsigned char[width * height * 4];
	memset(buffer, 0, width * height * 4);
	int* widths = new int[128];

	int maxUnderBaseline = 0;
	for (int i = 32; i < 127; ++i)
	{
		glyphIndex = FT_Get_Char_Index(face, i);

		error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
		if (error)
		{
			throw std::runtime_error("Failed to load glyph!");
		}

		const FT_Glyph_Metrics& glyphMetrics = face->glyph->metrics;
		int glyphHang = (glyphMetrics.horiBearingY - glyphMetrics.height) / 64;
		if (glyphHang < maxUnderBaseline)
		{
			maxUnderBaseline = glyphHang;
		}

	}

	// draw all characters
	for (int i = 0; i < 128; ++i)
	{
		// get the glyph index from character code
		glyphIndex = FT_Get_Char_Index(face, i);

		// load the glyph image into the slot
		error = FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
		if (error)
		{
			throw std::runtime_error("Failed to load glyph!");
		}

		// convert to an anti-aliased bitmap
		error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
		if (error)
		{
			throw std::runtime_error("Failed to load glyph!");
		}

		// save the character width
		widths[i] = face->glyph->metrics.width / 64;

		// find the tile position where we have to draw the character
		int x = (i % 16) * (fontsize + 2);
		int y = (i / 16) * (fontsize + 2);
		x += 1; // 1 pixel padding from the left side of the tile
		y += (fontsize + 2) - face->glyph->bitmap_top + maxUnderBaseline - 1;

		// draw the character
		const FT_Bitmap& bitmap = face->glyph->bitmap;
		for (int xx = 0; xx < bitmap.width; ++xx)
		{
			for (int yy = 0; yy < bitmap.rows; ++yy)
			{
				unsigned char r = bitmap.buffer[(yy * (bitmap.width) + xx)];
				buffer[(y + yy) * width * 4 + (x + xx) * 4 + 0] = r;
				buffer[(y + yy) * width * 4 + (x + xx) * 4 + 1] = r;
				buffer[(y + yy) * width * 4 + (x + xx) * 4 + 2] = r;
				buffer[(y + yy) * width * 4 + (x + xx) * 4 + 3] = 255;
			}
		}
	}


	return buffer;
	
}