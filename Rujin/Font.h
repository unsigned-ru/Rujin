#ifndef FONT_H
#define FONT_H

struct _TTF_Font;
namespace rujin
{
	/**
	 * \brief RAII wrapper for font.
	 */
	class Font
	{
	public:
		explicit Font(const std::string& fullPath, uint32_t size);
		~Font();

		Font(const Font &) = delete;
		Font(Font &&) = delete;
		Font & operator= (const Font &) = delete;
		Font & operator= (const Font &&) = delete;

		const std::string& GetFullPath() const;
		uint32_t GetSize() const;
		_TTF_Font* GetFont() const;

	private:
		std::string m_FullPath;
		_TTF_Font* m_Font;
		uint32_t m_Size;
	};
}
#endif