#include "MesonException.h"
#include "FileInputStream.h"

#include "VstVisibleEntitySetManager.h"
#include "VstPropertySet.h"
#include "VstLexer.h"

#include "VstVistasEngine.h"

using namespace Meson::Common;
using namespace	Meson::Common::IO;
using namespace Meson::Common::Text;
using namespace Meson::Common::Collections;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
Operation::Operation(void)
	: VistasObject(true)
	, m_pVisibleEntitySet(NULL)
	, m_pCuller(NULL)
	, m_pRenderer(NULL)
	, m_pProjector(NULL)
	, m_pNode(NULL)
	, m_bAccumulate(false)
	, m_bActive(true)
{
}
//----------------------------------------------------------------------------------------------
Operation::Operation(const Meson::Common::Text::String& p_strId)
	: VistasObject(p_strId)
	, m_pVisibleEntitySet(NULL)
	, m_pCuller(NULL)
	, m_pRenderer(NULL)
	, m_pProjector(NULL)
	, m_pNode(NULL)
	, m_bAccumulate(false)
	, m_bActive(true)
{
}
//----------------------------------------------------------------------------------------------
void Operation::SetRenderOperation(RepeatType p_eRepeatType, VisibleEntitySetPtr p_pVisibleEntitySet, RendererPtr p_pRenderer)
{
	m_eOperationType = OT_RENDER;
	m_eRepeatType = p_eRepeatType;
	m_pVisibleEntitySet = p_pVisibleEntitySet;
	m_pRenderer = p_pRenderer;
}
//----------------------------------------------------------------------------------------------
void Operation::SetCullOperation(RepeatType p_eRepeatType, bool p_bAccumulate, VisibleEntitySetPtr p_pVisibleEntitySet, 
								 ISceneNode* p_pNode, CameraPtr p_pProjector, CullerPtr p_pCuller)
{
	m_eOperationType = OT_CULL;
	m_eRepeatType = p_eRepeatType;
	m_bAccumulate = p_bAccumulate;
	m_pVisibleEntitySet = p_pVisibleEntitySet;
	m_pNode = p_pNode;
	m_pProjector = p_pProjector;
	m_pCuller = p_pCuller;
}
//----------------------------------------------------------------------------------------------
void Operation::SetActive(bool p_bActive)
{
	m_bActive = p_bActive;
}
//----------------------------------------------------------------------------------------------
bool Operation::IsActive(void) const
{
	return m_bActive;
}
//----------------------------------------------------------------------------------------------
void Operation::SetOperationType(Operation::OperationType p_eOperationType)
{
	m_eOperationType = p_eOperationType;
}
//----------------------------------------------------------------------------------------------
Operation::OperationType Operation::GetOperationType(void) const
{
	return m_eOperationType;
}
//----------------------------------------------------------------------------------------------
void Operation::SetRepeatType(Operation::RepeatType p_eRepeatType)
{
	m_eRepeatType = p_eRepeatType;
}
//----------------------------------------------------------------------------------------------
Operation::RepeatType Operation::GetRepeatType(void) const
{
	return m_eRepeatType;
}
//----------------------------------------------------------------------------------------------
void Operation::SetVisibleEntitySet(VisibleEntitySetPtr p_pVisibleEntitySet)
{
	m_pVisibleEntitySet = p_pVisibleEntitySet;
}
//----------------------------------------------------------------------------------------------
VisibleEntitySetPtr Operation::GetVisibleEntitySet(void)
{
	return m_pVisibleEntitySet;
}
//----------------------------------------------------------------------------------------------
void Operation::SetCuller(CullerPtr p_pCuller)
{
	m_pCuller = p_pCuller;
}
//----------------------------------------------------------------------------------------------
void Operation::SetRenderer(RendererPtr p_pRenderer)
{
	m_pRenderer = p_pRenderer;
}
//----------------------------------------------------------------------------------------------
void Operation::SetProjector(CameraPtr p_pProjector)
{
	m_pProjector = p_pProjector;
}
//----------------------------------------------------------------------------------------------
void Operation::SetNode(ISceneNode* p_pNode)
{
	m_pNode = p_pNode;
}
//----------------------------------------------------------------------------------------------
void Operation::Execute(void)
{
	if (m_bActive)
	{
		switch(m_eOperationType)
		{
		case OT_RENDER:
			m_pRenderer->SetVisibleSet(m_pVisibleEntitySet);
			m_pRenderer->Draw();
			break;

		case OT_CULL:
			m_pProjector->Update();

			m_pCuller->SetCamera(m_pProjector);
			m_pCuller->SetRootNode(m_pNode);
			m_pCuller->SetVisibleSet(m_pVisibleEntitySet);
			m_pCuller->ComputeVisibleSet(m_bAccumulate);
			break;
		}

		m_bActive = (m_eRepeatType != RT_ONCE);
	}
}
//----------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------
VisibleEntitySetManager::VisibleEntitySetManager(void)
	: m_operationMap()
	, m_visibleEntitySetMap()
{
}
//----------------------------------------------------------------------------------------------
VisibleEntitySetManager::~VisibleEntitySetManager(void)
{
	VisibleEntitySetEnumerator visibleEntitySetEnumerator = m_visibleEntitySetMap.GetValueEnumerator();

	while(visibleEntitySetEnumerator.HasMoreElements())
		visibleEntitySetEnumerator.NextElement() = NULL;

	m_visibleEntitySetMap.Clear();
}
//----------------------------------------------------------------------------------------------
VisibleEntitySetPtr VisibleEntitySetManager::CreateVisibleEntitySet(const Meson::Common::Text::String& p_strId)
{
	if (ContainsVisibleEntitySet(p_strId))
		throw new MesonException("Another visible entity set already exists with the given name", __FILE__, __LINE__);

	VisibleEntitySetPtr pVisibleEntitySet(new VisibleEntitySet(p_strId));
	m_visibleEntitySetMap.Insert(p_strId, pVisibleEntitySet);

	return pVisibleEntitySet;
}
//----------------------------------------------------------------------------------------------
bool VisibleEntitySetManager::ContainsVisibleEntitySet(const Meson::Common::Text::String& p_strId)
{
	return m_visibleEntitySetMap.ContainsKey(p_strId);
}
//----------------------------------------------------------------------------------------------
bool VisibleEntitySetManager::ContainsVisibleEntitySet(const Meson::Common::Text::String& p_strId, VisibleEntitySetPtr& p_pVisibleEntitySet)
{
	return m_visibleEntitySetMap.ContainsKey(p_strId, p_pVisibleEntitySet);
}
//----------------------------------------------------------------------------------------------
VisibleEntitySetPtr VisibleEntitySetManager::GetVisibleEntitySet(const Meson::Common::Text::String& p_strId)
{
	return m_visibleEntitySetMap[p_strId];
}
//----------------------------------------------------------------------------------------------
void VisibleEntitySetManager::RegisterVisibleEntitySet(VisibleEntitySetPtr p_pVisibleEntitySet, bool p_bReplace)
{
	if (!p_bReplace)
	{
		if (ContainsVisibleEntitySet(p_pVisibleEntitySet->GetId()))
			throw new MesonException("Another visible entity set already exists with the given name", __FILE__, __LINE__);

		m_visibleEntitySetMap.Insert(p_pVisibleEntitySet->GetId(), p_pVisibleEntitySet);
	}
	else
		m_visibleEntitySetMap[p_pVisibleEntitySet->GetId()] = p_pVisibleEntitySet;
}
//----------------------------------------------------------------------------------------------
void VisibleEntitySetManager::UnregisterVisibleEntitySet(VisibleEntitySetPtr p_pVisibleEntitySet)
{
	if (ContainsVisibleEntitySet(p_pVisibleEntitySet->GetId()))
		m_visibleEntitySetMap.RemoveKey(p_pVisibleEntitySet->GetId());
}
//----------------------------------------------------------------------------------------------
void VisibleEntitySetManager::UnregisterVisibleEntitySet(const Meson::Common::Text::String& p_strId)
{
	if (ContainsVisibleEntitySet(p_strId))
		m_visibleEntitySetMap.RemoveKey(p_strId);
}
//----------------------------------------------------------------------------------------------
void VisibleEntitySetManager::UnregisterAllVisibleEntitySets(void)
{
	m_visibleEntitySetMap.Clear();
}
//----------------------------------------------------------------------------------------------
void VisibleEntitySetManager::LoadVisibleEntitySet(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream)
{
	Lexer lexer(p_pInputStream);

	// 'VisibleEntitySet' container identifier
	lexer.ReadKeyword("VisibleEntitySet");

	// 'VisibleEntitySet' container opening brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	// mandatory ID property
	String strVisibleEntitySetId = lexer.ReadPropertyString("Id");

	VisibleEntitySetPtr pVisibleEntitySet = CreateVisibleEntitySet(strVisibleEntitySetId);

	// 'VisibleEntitySet' container closing brace
	lexer.ReadToken(LexerToken::TT_RIGHT_BRACE);

	// Register visibleEntitySet object
	RegisterVisibleEntitySet(pVisibleEntitySet);
}
//----------------------------------------------------------------------------------------------
void VisibleEntitySetManager::LoadVisibleEntitySets(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream)
{
	Lexer lexer(p_pInputStream);

	// 'VisibleEntitySets' container identifier
	lexer.ReadKeyword("VisibleEntitySets");

	// 'VisibleEntitySets' container opening brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	while (true)
	{
		LexerToken lexerToken = lexer.ReadToken();

		if (lexerToken.Type == LexerToken::TT_RIGHT_BRACE)
			return;

		if (lexerToken.Type != LexerToken::TT_KEYWORD
			|| lexerToken.StringValue != "VisibleEntitySet")
			throw new MesonException(
				"Parsing Error: Expected keyword 'VisibleEntitySet'",
				__FILE__, __LINE__);

		p_pInputStream->Pushback("VisibleEntitySet", 16);
		LoadVisibleEntitySet(p_pInputStream);
	}
}
//----------------------------------------------------------------------------------------------
void VisibleEntitySetManager::LoadVisibleEntitySets(const Meson::Common::Text::String& p_strResourceName)
{
	String strResourcePath;
	
	if (!VistasEngine::GetInstance()->GetRepositoryManager()->ContainsResource(p_strResourceName, strResourcePath))
		throw new MesonException("Resource not found!", __FILE__, __LINE__);

	InputStreamPtr pInputStream(new FileInputStream(strResourcePath));
	LookaheadInputStreamPtr pLookaheadInputStream(new LookaheadInputStream(pInputStream));
	LoadVisibleEntitySets(pLookaheadInputStream);
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
OperationPtr VisibleEntitySetManager::CreateOperation(const Meson::Common::Text::String& p_strId)
{
	if (ContainsOperation(p_strId))
		throw new MesonException("Another operation already exists with the given name", __FILE__, __LINE__);

	OperationPtr pOperation(new Operation(p_strId));
	m_operationMap.Insert(p_strId, pOperation);

	return pOperation;
}
//----------------------------------------------------------------------------------------------
bool VisibleEntitySetManager::ContainsOperation(const Meson::Common::Text::String& p_strId)
{
	return m_operationMap.ContainsKey(p_strId);
}
//----------------------------------------------------------------------------------------------
bool VisibleEntitySetManager::ContainsOperation(const Meson::Common::Text::String& p_strId, OperationPtr& p_pOperation)
{
	return m_operationMap.ContainsKey(p_strId, p_pOperation);
}
//----------------------------------------------------------------------------------------------
OperationPtr VisibleEntitySetManager::GetOperation(const Meson::Common::Text::String& p_strId)
{
	return m_operationMap[p_strId];
}
//----------------------------------------------------------------------------------------------
void VisibleEntitySetManager::RegisterOperation(OperationPtr p_pOperation, bool p_bReplace)
{
	if (!p_bReplace)
	{
		if (ContainsOperation(p_pOperation->GetId()))
			throw new MesonException("Another operation already exists with the given name", __FILE__, __LINE__);

		m_operationMap.Insert(p_pOperation->GetId(), p_pOperation);
	}
	else
		m_operationMap[p_pOperation->GetId()] = p_pOperation;
}
//----------------------------------------------------------------------------------------------
void VisibleEntitySetManager::UnregisterOperation(OperationPtr p_pOperation)
{
	if (ContainsOperation(p_pOperation->GetId()))
		m_operationMap.RemoveKey(p_pOperation->GetId());
}
//----------------------------------------------------------------------------------------------
void VisibleEntitySetManager::UnregisterOperation(const Meson::Common::Text::String& p_strId)
{
	if (ContainsOperation(p_strId))
		m_operationMap.RemoveKey(p_strId);
}
//----------------------------------------------------------------------------------------------
void VisibleEntitySetManager::UnregisterAllOperations(void)
{
	m_operationMap.Clear();
}
//----------------------------------------------------------------------------------------------
void VisibleEntitySetManager::LoadOperation(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream)
{
	Lexer lexer(p_pInputStream);

	// 'Operation' container keyword
	lexer.ReadKeyword("Operation");

	// 'Operation' container left brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	// mandatory ID property
	String strOperationId = lexer.ReadPropertyString("Id");
	String strOperationType = lexer.ReadPropertyString("Type");

	// Create operation
	OperationPtr pOperation = CreateOperation(strOperationId);

	lexer.ReadKeyword("CustomProperties");	
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	if (!strOperationType.CompareIgnoreCase("cull"))
	{
		String strRepeat = lexer.ReadPropertyString("Repeat");
		String strCuller = lexer.ReadPropertyString("Culler");
		String strVisibleSet = lexer.ReadPropertyString("VisibleSetId");
		String strProjectorId = lexer.ReadPropertyString("ProjectorId");
		String strScenegraphId = lexer.ReadPropertyString("ScenegraphId"); 
		String strNodeId = lexer.ReadPropertyString("NodeId");
		bool bAccumulate = lexer.ReadPropertyBoolean("Accumulate");

		// Repeat type
		Operation::RepeatType eRepeatType;
		if (!strRepeat.CompareIgnoreCase("once"))
			eRepeatType = Operation::RT_ONCE;
		else if (!strRepeat.CompareIgnoreCase("always"))
			eRepeatType = Operation::RT_ALWAYS;

		// Culler
		CullerPtr pCuller = VistasEngine::GetInstance()->GetCullerManager()->GetCullerFactory(strCuller)->CreateCuller();

		// VisibleSet
		VisibleEntitySetPtr pVisibleSet = GetVisibleEntitySet(strVisibleSet);

		// Projector
		CameraPtr pProjector = VistasEngine::GetInstance()->GetCameraManager()->GetCamera(strProjectorId);

		// Node
		ISceneNode* pNode = VistasEngine::GetInstance()->GetScenegraphManager()->GetScenegraph(strScenegraphId)->GetNode(strNodeId);

		pOperation->SetCullOperation(eRepeatType, bAccumulate, pVisibleSet, pNode, pProjector, pCuller);
	}
	else if (!strOperationType.CompareIgnoreCase("render"))
	{
		String strRepeat = lexer.ReadPropertyString("Repeat");
		String strRenderer = lexer.ReadPropertyString("Renderer");
		String strVisibleSet = lexer.ReadPropertyString("VisibleSetId");

		// Repeat type
		Operation::RepeatType eRepeatType;
		if (!strRepeat.CompareIgnoreCase("once"))
			eRepeatType = Operation::RT_ONCE;
		else if (!strRepeat.CompareIgnoreCase("always"))
			eRepeatType = Operation::RT_ALWAYS;

		// Renderer
		RendererPtr pRenderer = VistasEngine::GetInstance()->GetRendererManager()->GetRendererFactory(strRenderer)->CreateRenderer();

		// VisibleSet
		VisibleEntitySetPtr pVisibleSet = GetVisibleEntitySet(strVisibleSet);

		pOperation->SetRenderOperation(eRepeatType, pVisibleSet, pRenderer);
	}

	lexer.ReadToken(LexerToken::TT_RIGHT_BRACE);
	
	// 'Operation' container right brace
	lexer.ReadToken(LexerToken::TT_RIGHT_BRACE);

	// Register operation object
	RegisterOperation(pOperation);
}
//----------------------------------------------------------------------------------------------
void VisibleEntitySetManager::LoadOperations(Meson::Common::IO::LookaheadInputStreamPtr p_pInputStream)
{
	Lexer lexer(p_pInputStream);

	// 'Operations' container keyword
	lexer.ReadKeyword("Operations");

	// 'Operations' container left brace
	lexer.ReadToken(LexerToken::TT_LEFT_BRACE);

	while (true)
	{
		LexerToken lexerToken = lexer.ReadToken();

		if (lexerToken.Type == LexerToken::TT_RIGHT_BRACE)
			return;

		if (lexerToken.Type != LexerToken::TT_KEYWORD
			|| lexerToken.StringValue != "Operation")
			throw new MesonException(
				"Parsing Error: Expected keyword 'Operation'",
				__FILE__, __LINE__);

		p_pInputStream->Pushback("Operation", 9);
		LoadOperation(p_pInputStream);
	}
}
//----------------------------------------------------------------------------------------------
void VisibleEntitySetManager::LoadOperations(const Meson::Common::Text::String& p_strResourceName)
{
	String strResourcePath;
	
	if (!VistasEngine::GetInstance()->GetRepositoryManager()->ContainsResource(p_strResourceName, strResourcePath))
		throw new MesonException("Resource not found!", __FILE__, __LINE__);

	InputStreamPtr pInputStream(new FileInputStream(strResourcePath));
	LookaheadInputStreamPtr pLookaheadInputStream(new LookaheadInputStream(pInputStream));
	LoadOperations(pLookaheadInputStream);
}
//----------------------------------------------------------------------------------------------
void VisibleEntitySetManager::ExecuteOperations(void)
{
	// Shit, I know I should pre-categorise operation types! 
	// TODO: Sort out this mess
	TEnumerator<OperationPtr>& operationEnumerator = m_operationMap.GetValueEnumerator();
	while (operationEnumerator.HasMoreElements())
	{
		OperationPtr& pOperation = operationEnumerator.NextElement();
		
		if (pOperation->GetOperationType() == Operation::OT_CULL) 
			pOperation->Execute();
	}

	operationEnumerator.Reset();

	while (operationEnumerator.HasMoreElements())
	{
		OperationPtr& pOperation = operationEnumerator.NextElement();
		
		if (pOperation->GetOperationType() == Operation::OT_RENDER) 
			pOperation->Execute();
	}
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
