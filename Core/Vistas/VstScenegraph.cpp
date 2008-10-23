#include "MesonException.h"
#include "FileInputStream.h"
#include "Matrix4.h"
#include "Vector4.h"

#include "VstSceneGroupNode.h"
#include "VstSceneLightNode.h"
#include "VstSceneGeometryNode.h"

#include "VstScenegraph.h"
#include "VstPropertySet.h"
#include "VstLexer.h"

#include "VstVistasEngine.h"

using namespace Meson::Common;
using namespace	Meson::Common::IO;
using namespace Meson::Common::Text;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
Scenegraph::Scenegraph(void)
	: VistasObject(true)
{
}
//----------------------------------------------------------------------------------------------
Scenegraph::Scenegraph(const Meson::Common::Text::String& p_strId)
	: VistasObject(p_strId)	
{
}
//----------------------------------------------------------------------------------------------
Scenegraph::~Scenegraph(void)
{
	TEnumerator<ISceneNode*>& nodeEnumerator = m_nodeMap.GetValueEnumerator();
	
	while(nodeEnumerator.HasMoreElements())
	{
		ISceneNode* pNode = nodeEnumerator.NextElement();
		
		if (pNode->GetOwner() == this) 
			delete pNode;
	}
	
	m_nodeMap.Clear();
}
//----------------------------------------------------------------------------------------------
bool Scenegraph::ContainsNode(const Meson::Common::Text::String& p_strId)
{
	return m_nodeMap.ContainsKey(p_strId);
}
//----------------------------------------------------------------------------------------------
bool Scenegraph::ContainsNode(const Meson::Common::Text::String& p_strId, ISceneNode*& p_pNode)
{
	return m_nodeMap.ContainsKey(p_strId, p_pNode);
}
//----------------------------------------------------------------------------------------------
ISceneNode* Scenegraph::GetNode(const Meson::Common::Text::String& p_strId)
{
	return m_nodeMap[p_strId];
}
//----------------------------------------------------------------------------------------------
void Scenegraph::RegisterNode(ISceneNode* p_pNode, bool p_bTakeOwnership)
{
	if (ContainsNode(p_pNode->GetId()))
		throw new MesonException("Another node object already exists with the given name", __FILE__, __LINE__);

	m_nodeMap.Insert(p_pNode->GetId(), p_pNode);
	p_pNode->SetOwner(this);
}
//----------------------------------------------------------------------------------------------
void Scenegraph::UnregisterNode(ISceneNode* p_pNode)
{
	if (ContainsNode(p_pNode->GetId()))
	{
		if (p_pNode->GetOwner() == this)
			p_pNode->SetOwner(NULL);

		m_nodeMap.RemoveKey(p_pNode->GetId());
	}
}
//----------------------------------------------------------------------------------------------
void Scenegraph::UnregisterNode(const Meson::Common::Text::String& p_strId)
{
	ISceneNode* pNode;

	if (ContainsNode(p_strId, pNode))
	{
		if (pNode->GetOwner() == this)
			pNode->SetOwner(NULL);

		m_nodeMap.RemoveKey(p_strId);
	}
}
//----------------------------------------------------------------------------------------------
void Scenegraph::UnregisterAllNodes(void)
{
	TEnumerator<ISceneNode*>& nodeEnumerator = m_nodeMap.GetValueEnumerator();
	
	while(nodeEnumerator.HasMoreElements())
	{
		ISceneNode* pNode = nodeEnumerator.NextElement();
		
		if (pNode->GetOwner() == this) 
			pNode->SetOwner(NULL);
	}
	
	m_nodeMap.Clear();
}
//----------------------------------------------------------------------------------------------
void Scenegraph::LoadNode(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream)
{
	Lexer lexer(p_pInputStream);

	// 'Node' container keyword
	lexer.ReadKeyword("Node");

	// 'Node' container left brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	// mandatory ID property
	String strNodeId = lexer.ReadPropertyString("Id");

	// mandatory Type property
	String strNodeType = lexer.ReadPropertyString("Type");

	// ParentID is mandatory if not root node
	ISceneCompositeNode* pParentNode = NULL;
	if (strNodeType.CompareIgnoreCase("root"))
	{
		String strParentId = lexer.ReadPropertyString("ParentId");
		pParentNode = (ISceneCompositeNode*)GetNode(strParentId);
		
		if (!pParentNode->IsDerived(ISceneCompositeNode::GetStaticType()))
		{
			throw new MesonException(
				"Error : nodes must be attached to internal node types.",
				__FILE__, __LINE__ );
		}
	}

	// 'Transformation' container keyword
	lexer.ReadKeyword("Transformation");

	// 'CustomProperties' container left brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	// mandatory orientation, scale and position
	Vector4f vec4Orientation	= lexer.ReadPropertyVector4("Orientation");
	Vector3f vec3Position		= lexer.ReadPropertyVector3("Position");
	Vector3f vec3Scale			= lexer.ReadPropertyVector3("Scale");
	bool bIsAbsoluteTransform	= lexer.ReadPropertyBoolean("Absolute");

	// 'Transformation' container right brace
	lexer.ReadToken(LexerToken::TT_RIGHT_BRACE);

	// attached lights
	lexer.ReadKeyword("Lights");
	lexer.ReadToken(LexerToken::TT_EQUALS);
	TArrayList<LexerToken> lightListTokens;
	lexer.ReadTokenList(LexerToken::TT_LITERAL, lightListTokens);

	// attached effects
	lexer.ReadKeyword("Effects");
	lexer.ReadToken(LexerToken::TT_EQUALS);
	TArrayList<LexerToken> effectListTokens;
	lexer.ReadTokenList(LexerToken::TT_LITERAL, effectListTokens);

	//----------------------------------------------------------------------------------------------
	// NOTE : THIS NEEDS TO BE PROPERLY SORTED OUT, BUT IN THE NAME OF HASTE, I HAVE TO FORSAKE
	// PROPER ABSTRACTION AND MODULARISATION. SORRY.
	//----------------------------------------------------------------------------------------------
	// 'CustomProperties' container keyword
	lexer.ReadKeyword("CustomProperties");

	// 'CustomProperties' container left brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	// create node
	ISceneNode* pNode = NULL;
	
	if (!strNodeType.CompareIgnoreCase("root"))
	{
		pNode = new SceneGroupNode(strNodeId, this);
	}
	else if (!strNodeType.CompareIgnoreCase("group"))
	{
		pNode = new SceneGroupNode(strNodeId, this);
		pParentNode->AttachChild(pNode);
	}
	else if (!strNodeType.CompareIgnoreCase("geometry"))
	{
		SceneGeometryNode* pInnerNode = new SceneGeometryNode(strNodeId, this);

		String strGeometryId = lexer.ReadPropertyString("GeometryId");
		pInnerNode->SetGeometry(VistasEngine::GetInstance()->GetGeometryManager()->GetGeometry(strGeometryId));

		String strMaterialId = lexer.ReadPropertyString("MaterialId");
		pInnerNode->SetMaterial(VistasEngine::GetInstance()->GetMaterialManager()->GetMaterial(strMaterialId));

		pNode = (ISceneNode*)pInnerNode;
		pParentNode->AttachChild(pNode);
	}
	else if (!strNodeType.CompareIgnoreCase("light"))
	{
		SceneLightNode* pInnerNode = new SceneLightNode(strNodeId, this);

		String strLightId = lexer.ReadPropertyString("LightId");
		pInnerNode->SetLight(VistasEngine::GetInstance()->GetLightManager()->GetLight(strLightId));

		pNode = (ISceneNode*)pInnerNode;
		pParentNode->AttachChild(pNode);
	}
	else if (!strNodeType.CompareIgnoreCase("camera"))
	{
		//pNode = new SceneCameraNode(this, strNodeId);
		//pNode->SetParent(GetNode(strParentId));
	}

	// 'CustomProperties' container right brace
	lexer.ReadToken(LexerToken::TT_RIGHT_BRACE);

	//----------------------------------------------------------------------------------------------
	// NOTE : THIS NEEDS TO BE PROPERLY SORTED OUT, BUT IN THE NAME OF HASTE, I HAVE TO FORSAKE
	// PROPER ABSTRACTION AND MODULARISATION. SORRY.
	//----------------------------------------------------------------------------------------------

	// attach lights to node
	LightManager* pLightManager = VistasEngine::GetInstance()->GetLightManager();
	for (uint uiIndex = 0; uiIndex < lightListTokens.Size(); uiIndex++)
		pNode->AttachLight(pLightManager->GetLight(lightListTokens[uiIndex].StringValue));

	// attach effects to node
	EffectManager* pEffectManager = VistasEngine::GetInstance()->GetEffectManager();
	for (uint uiIndex = 0; uiIndex < effectListTokens.Size(); uiIndex++)
	{
		EffectPtr pEffect = pEffectManager->GetEffect(effectListTokens[uiIndex].StringValue);
		pNode->AttachEffect(pEffectManager->GetEffect(effectListTokens[uiIndex].StringValue));
	}

	// Set common properties
	Matrix3f mat3Rotation(*(Vector3f*)(vec4Orientation.Element), vec4Orientation.W);
	pNode->LocalTransformation.SetRotation(mat3Rotation);
	pNode->LocalTransformation.SetScaling(vec3Scale);
	pNode->LocalTransformation.SetTranslation(vec3Position);
	pNode->SetAbsoluteLocalTransform(bIsAbsoluteTransform);

	// 'Node' container right brace
	lexer.ReadToken(LexerToken::TT_RIGHT_BRACE);

	// Node object
	RegisterNode(pNode);
}
//----------------------------------------------------------------------------------------------
void Scenegraph::LoadNodes(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream)
{
	Lexer lexer(p_pInputStream);

	// 'Nodes' container identifier
	lexer.ReadKeyword("Nodes");

	// 'Nodes' container opening brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	while (true)
	{
		LexerToken lexerToken = lexer.ReadToken();

		if (lexerToken.Type == LexerToken::TT_RIGHT_BRACE)
			return;

		if (lexerToken.Type != LexerToken::TT_KEYWORD
			|| lexerToken.StringValue != "Node")
			throw new MesonException(
				"Parsing Error: Expected keyword 'Node'",
				__FILE__, __LINE__);

		p_pInputStream->Pushback("Node", 4);
		LoadNode(p_pInputStream);
	}
}
//----------------------------------------------------------------------------------------------
void Scenegraph::LoadNodes(const Meson::Common::Text::String& p_strResourceName)
{
	String strResourcePath;
	
	if (!VistasEngine::GetInstance()->GetRepositoryManager()->ContainsResource(p_strResourceName, strResourcePath))
		throw new MesonException("Resource not found!", __FILE__, __LINE__);

	InputStreamPtr pInputStream(new FileInputStream(strResourcePath));
	LookaheadInputStreamPtr pLookaheadInputStream(new LookaheadInputStream(pInputStream));
	LoadNodes(pLookaheadInputStream);
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
