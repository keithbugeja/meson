#pragma once

#include "XmlDeclaration.h"
#include "XmlElement.h"

Meson_Common_Xml_BEGIN

struct XmlDocumentState;

class XmlDocument
{
private: // private variables
	XmlDocumentState* m_pXmlDocumentState;

public: // public methods
	XmlDocument(void);
	~XmlDocument(void);

	XmlDeclarationPtr GetDeclaration(void);
	XmlElementPtr GetRootElement(void);

	void Load(Meson::Common::IO::IInputStream& p_inputStream);
	void Store(Meson::Common::IO::IOutputStream& p_outputStream) const;
};

Meson_Common_Xml_END
