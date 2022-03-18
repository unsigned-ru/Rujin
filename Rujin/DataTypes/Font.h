#pragma once

struct _TTF_Font;
namespace rujin
{
	class Font
	{
	public:
		_TTF_Font* GetFont() const;
		explicit Font(const std::string& fullPath, uint32_t size);
		~Font();

		Font(const Font &) = delete;
		Font(Font &&) = delete;
		Font & operator= (const Font &) = delete;
		Font & operator= (const Font &&) = delete;

		uint32_t GetSize() const;
		
	private:
		_TTF_Font* m_Font;
		uint32_t m_Size;
	};
}
