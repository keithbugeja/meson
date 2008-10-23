#pragma once

#include "Namespaces.h"
#include "String8.h"

Meson_Common_IO_BEGIN

	struct FileState;
	class Directory;

	class File
	{
	private:
		FileState* m_pFileState;

	public:
		File(void);
		File(const Meson::Common::Text::String& p_strPath);
		File(const File& p_file);
		~File(void);
		File& operator=(const File& p_file);
		bool operator==(const File& p_file);
		const Meson::Common::Text::String& GetPath() const;
		const Meson::Common::Text::String& GetName() const;
		bool IsDirectory(void) const;
		bool IsReadOnly(void) const;
		bool IsHidden(void) const;
		size_t GetSize(void) const;
		const Directory& GetParent(void) const;
		void MoveTo(const Directory& p_directory);
		void RenameTo(const Meson::Common::Text::String& p_strNewName);
		void Delete(void);

		static bool Exists(const Meson::Common::Text::String& p_strPath);
	};

Meson_Common_IO_END
