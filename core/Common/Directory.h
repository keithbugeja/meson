#pragma once

#include "Namespaces.h"
#include "String8.h"
#include "List.h"
#include "File.h"

Meson_Common_IO_BEGIN

	struct DirectoryState;

	class Directory
	{
	private:
		DirectoryState* m_pDirectoryState;

	public:
		static const Meson::Common::Text::String Delimeter;

	public:
		static Directory GetRoot(void);
		static Directory GetCurrent(void);

	public: // public methods
		Directory(void);
		Directory(const Meson::Common::Text::String& p_strPath);
		~Directory(void);
		Directory& operator=(const Directory& p_directory);
		const Meson::Common::Text::String& GetPath(void) const;
		const Meson::Common::Text::String& GetName(void) const;
		Directory GetParent(void) const;
		void EnumerateFiles(Meson::Common::Collections::TList<File>& p_listFiles);
		Directory CreateSubDirectory(const Meson::Common::Text::String& p_strName);
	};

Meson_Common_IO_END
