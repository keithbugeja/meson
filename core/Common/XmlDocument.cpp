#include "XmlDocument.h"

#include "Character8.h"
#include "MesonException.h"

using namespace Meson::Common::Collections;
using namespace Meson::Common::IO;
using namespace Meson::Common::Text;
using namespace Meson::Common::Xml;

Meson_Common_Xml_BEGIN

struct XmlDocumentState
{
	XmlDeclarationPtr m_pXmlDeclaration;
	XmlElementPtr m_pXmlElementRoot;
	String m_strStream;
	size_t m_unStreamIndex;
	size_t m_unCheckpoint;

	XmlDocumentState(void)
		: m_pXmlDeclaration()
		, m_pXmlElementRoot()
		, m_strStream()
		, m_unStreamIndex(0)
		, m_unCheckpoint(0)
	{
	}

	~XmlDocumentState(void)
	{
	}

	void InitialiseStringStream(IInputStream& p_inputStream)
	{
		m_strStream.Clear();
		char ch;
		while(!p_inputStream.IsEndOfStream())
		{
			p_inputStream.Read(&ch, 1);
			m_strStream += ch;
		}

		m_unStreamIndex = 0;
		m_unCheckpoint = 0;

		// strip XML comments
		bool bMoreComments = true;
		while (bMoreComments)
		{
			bMoreComments = false;
			int nCommentStart = m_strStream.IndexOf("<!--");
			int nCommentEnd = m_strStream.IndexOf("-->");
			if (nCommentStart > 0 && nCommentEnd > 0
				&& nCommentEnd - nCommentStart > 3)
			{
				m_strStream.Delete(nCommentStart, nCommentEnd - nCommentStart + 3);
				bMoreComments = true;
			}
		}
	}

	bool IsEndOfStream(void)
	{
		return m_unStreamIndex >= m_strStream.Length();
	}

	char PeekChar(void)
	{
		if (m_unStreamIndex >= m_strStream.Length())
			throw new MesonException("Unexpected end of stream.", __FILE__, __LINE__);
		return m_strStream[m_unStreamIndex];
	}

	char ReadChar(void)
	{
		if (m_unStreamIndex >= m_strStream.Length())
			throw new MesonException("Unexpected end of stream.", __FILE__, __LINE__);
		return m_strStream[m_unStreamIndex++];
	}

	char PeekNonSpaceChar(void)
	{
		char ch = ReadNonSpaceChar();
		RewindStream(1);
		return ch;
	}

	char ReadNonSpaceChar(void)
	{
		while (true)
		{
			char ch = ReadChar();
			if (!Character::IsWhitespace(ch)) return ch;
		}
	}

	void RewindStream(size_t p_unOffset)
	{
		if (p_unOffset > m_unStreamIndex)
			throw new MesonException("Cannot rewind beyond start of stream.", __FILE__, __LINE__);
		m_unStreamIndex -= p_unOffset;
	}

	void SetCheckpoint(void)
	{
		m_unCheckpoint = m_unStreamIndex;
	}

	void RollbackToCheckpoint(void)
	{
		m_unStreamIndex = m_unCheckpoint;
	}

	void ExpectNonSpaceChar(char p_chExpected)
	{
		while (true)
		{
			char ch = ReadChar();
			if (ch == p_chExpected) return;
			if (Character::IsWhitespace(ch)) continue;
			throw new MesonException(
				String("Character '") + p_chExpected + "' expected.",
				__FILE__, __LINE__);
		}
	}

	String ReadIdentifier(void)
	{
		char ch = ReadNonSpaceChar();
		if (!Character::IsLetter(ch) && ch != '_')
			throw new MesonException("Literal must start with a letter or underscore.",
				__FILE__, __LINE__);

		String strIdentifier = ch;
		while (!IsEndOfStream())
		{
			ch = ReadChar();
			if (Character::IsLetterOrDigit(ch) || ch == '_')
				strIdentifier += ch;
			else
			{
				RewindStream(1);
				break;
			}
		}
		return strIdentifier;
	}

	String DecodeText(const String& p_strEncodedText)
	{
		String strPlainText = p_strEncodedText;
		strPlainText.Replace("&quot;", "\"");
		strPlainText.Replace("&apos;", "'");
		strPlainText.Replace("&amp;", "&");
		strPlainText.Replace("&lt;", "<");
		strPlainText.Replace("&gt;", ">");
		return strPlainText;
	}

	String ReadLiteral(void)
	{
		char chQuote = ReadNonSpaceChar();
		if (chQuote != '\"' && chQuote != '\'')
			throw new MesonException("A quote character (') or (\') expected for string literal.",
			__FILE__, __LINE__);

		String strLiteral;
		while (true)
		{
			char ch = ReadChar();
			if (ch == chQuote) break;
			strLiteral += ch;
		}

		return DecodeText(strLiteral);
	}

	XmlAttributePtr ReadAttribute(void)
	{
		String strAttributeName = ReadIdentifier();
		ExpectNonSpaceChar('=');
		String strAttributeValue = ReadLiteral();

		return XmlAttributePtr(
			new XmlAttribute(strAttributeName, strAttributeValue));
	}	

	XmlTextPtr ReadText(void)
	{
		// TODO - decode text
		String strText;
		while (true)
		{
			char ch = ReadChar();
			if (ch == '<')
			{
				RewindStream(1);
				break;
			}
			if (ch == '>')
				throw new MesonException("Invalid character found in text.",
					__FILE__, __LINE__);
			strText += ch;
		}

		return XmlTextPtr(new XmlText(DecodeText(strText)));
	}

	XmlElementPtr ReadElement(void)
	{
		// opening tag
		ExpectNonSpaceChar('<');

		String strElementName = ReadIdentifier();
		XmlElementPtr pXmlElement(new XmlElement(strElementName));

		// attributes within opening tag
		char ch = PeekNonSpaceChar();
		while (ch != '>')
		{
			XmlAttributePtr pXmlAttribute = ReadAttribute();
			pXmlElement->AddAttribute(pXmlAttribute);
			ch = PeekNonSpaceChar(); 
		}

		// end of opening tag
		ExpectNonSpaceChar('>');

		// element content or close tag
		while (true)
		{
			SetCheckpoint();
			ch = ReadNonSpaceChar(); 
			if (ch == '<')
			{
				ch = ReadNonSpaceChar();
				if (ch == '/')
				{
					// closing tag
					String strElementNameClose = ReadIdentifier();
					if (strElementNameClose != strElementName)
						throw new MesonException(
							"Start element '" + strElementName
							+ "' mismatch with end element '" + strElementNameClose + "'.",
							__FILE__, __LINE__);
					ExpectNonSpaceChar('>');
					return pXmlElement;
				}
				else
				{
					// child element
					RollbackToCheckpoint();
					XmlElementPtr pXmlElementChild = ReadElement();
					pXmlElement->AddChildElement(pXmlElementChild);
				}
			}
			else
			{
				// child text
				RollbackToCheckpoint();
				XmlTextPtr pXmlTextChild = ReadText();
				pXmlElement->AddChildText(pXmlTextChild);
			}
		}
	}

	XmlDeclarationPtr ReadDeclaration(void)
	{
		// opening
		ExpectNonSpaceChar('<');
		ExpectNonSpaceChar('?');
		String strDeclarationName = ReadIdentifier();
		if (strDeclarationName != String("xml"))
			throw new MesonException("Invalid XML declaration format.", __FILE__, __LINE__);

		XmlDeclarationPtr pXmlDeclaration(new XmlDeclaration());

		// attributes within declaration
		char ch = PeekNonSpaceChar();
		while (ch != '?')
		{
			XmlAttributePtr pXmlAttribute = ReadAttribute();
			pXmlDeclaration->AddAttribute(pXmlAttribute);
			ch = PeekNonSpaceChar(); 
		}

		// end of opening tag
		ExpectNonSpaceChar('?');
		ExpectNonSpaceChar('>');

		return pXmlDeclaration;
	}

	String EncodeText(const String& p_strPlainText)
	{
		String strEncodedText = p_strPlainText;
		strEncodedText.Replace("\"", "&quot;");
		strEncodedText.Replace("'", "&apos;");
		strEncodedText.Replace("&", "&amp;");
		strEncodedText.Replace("<", "&lt;");
		strEncodedText.Replace(">", "&gt;");
		return strEncodedText;
	}

	void Store(const String& p_strRawText, IOutputStream& p_outputStream)
	{
		p_outputStream.Write((void *) (const char *) p_strRawText, p_strRawText.Size());
	}

	void Store(XmlTextPtr p_pXmlText, IOutputStream& p_outputStream)
	{
		Store(EncodeText(p_pXmlText->GetText()), p_outputStream);
	}

	void Store(XmlAttributePtr p_pXmlAttribute, IOutputStream& p_outputStream)
	{
		Store(" " + p_pXmlAttribute->GetName() + " = \""
			+ EncodeText(p_pXmlAttribute->GetValue()) + "\"", p_outputStream);
	}

	void Store(XmlElementPtr p_pXmlElement, IOutputStream& p_outputStream)
	{
		// start open
		Store("<" + p_pXmlElement->GetName(), p_outputStream);

		// attributes
		TEnumerator<XmlAttributePtr>& enumAttributes = p_pXmlElement->GetAttributes();
		while(enumAttributes.HasMoreElements())
			Store(enumAttributes.NextElement(), p_outputStream);

		// start close
		Store(">", p_outputStream);

		// contained elements
		TEnumerator<XmlNodePtr>& enumNodes = p_pXmlElement->GetChildNodes();
		while (enumNodes.HasMoreElements())
		{
			XmlNodePtr pXmlNode = enumNodes.NextElement();
			switch (pXmlNode->GetType())
			{
			case XmlNodeType::Attribute: continue; break;
			case XmlNodeType::Text:
				Store(XmlTextPtr((XmlText*) (XmlNode*) pXmlNode), p_outputStream);
			break;
			case XmlNodeType::Element:
				Store(XmlElementPtr((XmlElement*) (XmlNode*) pXmlNode), p_outputStream);
			break;
			}
		}

		// closing tag
		Store("</" + p_pXmlElement->GetName() + ">", p_outputStream);
	}

	void Store(XmlDeclarationPtr p_pXmlDeclaration, IOutputStream& p_outputStream)
	{
		// open
		Store("<?xml", p_outputStream);

		// attributes
		TEnumerator<XmlAttributePtr>& enumAttributes = p_pXmlDeclaration->GetAttributes();
		while(enumAttributes.HasMoreElements())
			Store(enumAttributes.NextElement(), p_outputStream);

		// close
		Store(" ?>", p_outputStream);
	}
};

Meson_Common_Xml_END

XmlDocument::XmlDocument(void)
	: m_pXmlDocumentState(NULL)
{
	m_pXmlDocumentState = new XmlDocumentState();
}

XmlDocument::~XmlDocument(void)
{
	delete m_pXmlDocumentState;
}

XmlDeclarationPtr XmlDocument::GetDeclaration(void)
{
	return m_pXmlDocumentState->m_pXmlDeclaration;
}

XmlElementPtr XmlDocument::GetRootElement(void)
{
	return m_pXmlDocumentState->m_pXmlElementRoot;
}

void XmlDocument::Load(IInputStream& p_inputStream)
{
	m_pXmlDocumentState->InitialiseStringStream(p_inputStream);

	m_pXmlDocumentState->SetCheckpoint();

	char ch = m_pXmlDocumentState->ReadNonSpaceChar();
	if (ch != '<')
		throw new MesonException(
			"XML declaration or root element start tag expected.",
			__FILE__, __LINE__);

	ch = m_pXmlDocumentState->ReadNonSpaceChar();
	m_pXmlDocumentState->RollbackToCheckpoint();

	// XML declaration (optional)
	if (ch == '?')
	{
		m_pXmlDocumentState->m_pXmlDeclaration
			= m_pXmlDocumentState->ReadDeclaration();
	}
	else
	{
		// construct own declaration
		XmlDeclaration* pXmlDeclaration = new XmlDeclaration();
		pXmlDeclaration->AddAttribute(XmlAttributePtr(new XmlAttribute("version", "1.0")));
		pXmlDeclaration->AddAttribute(XmlAttributePtr(new XmlAttribute("encoding", "UTF-8")));
		pXmlDeclaration->AddAttribute(XmlAttributePtr(new XmlAttribute("standalone", "yes")));
		m_pXmlDocumentState->m_pXmlDeclaration = pXmlDeclaration;
	}

	// root element
	m_pXmlDocumentState->m_pXmlElementRoot
		= m_pXmlDocumentState->ReadElement();

	// ensure no more data
	while (!m_pXmlDocumentState->IsEndOfStream())
	{
		if (!Character::IsWhitespace(m_pXmlDocumentState->ReadChar()))
			throw new MesonException(
				"No further data expected after root element.",
				__FILE__, __LINE__);
	}
}

void XmlDocument::Store(IOutputStream& p_outputStream) const
{
	m_pXmlDocumentState->Store(
		m_pXmlDocumentState->m_pXmlDeclaration, p_outputStream);

	m_pXmlDocumentState->Store(
		m_pXmlDocumentState->m_pXmlElementRoot, p_outputStream);
}
