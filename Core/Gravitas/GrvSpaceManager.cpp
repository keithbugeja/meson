#include "GrvSpaceManager.h"

#include "DebugSupport.h"
#include "HashMap.h"
#include "FileInputStream.h"

#include "GrvLexer.h"
#include "GrvRigidBody.h"
#include "GrvGravitasEngine.h"

using namespace Meson::Common;
using namespace Meson::Common::Collections;
using namespace Meson::Common::IO;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Text;

using namespace Meson::Gravitas::Body;
using namespace Meson::Gravitas::Force;
using namespace Meson::Gravitas::Geometry;
using namespace Meson::Gravitas::Kinetics;
using namespace Meson::Gravitas::Space;

Meson_Gravitas_Space_BEGIN

struct SpaceManagerState
{
	THashMap<String, ISpaceFactory* > m_mapSpaceFactories;

	SpaceManagerState(void)
		: m_mapSpaceFactories() { }

	void LoadKineticProperties(
		LookaheadInputStreamPtr p_pLookaheadInputStream,
		KineticProperties& p_kineticProperties)
	{
		Lexer lexer(p_pLookaheadInputStream);

		lexer.ReadIdentifier("KineticProperties");

		// 'KineticProperties' container opening brace
		lexer.ReadToken(LexerTokenType::OpeningBrace);

		// position
		p_kineticProperties.Position = lexer.ReadPropertyVector("Position");

		// orientation
		lexer.ReadIdentifier("Orientation");
		lexer.ReadToken(LexerTokenType::Assignment);
		p_kineticProperties.Orientation = lexer.ReadQuaternion();

		// linear velocity
		p_kineticProperties.LinearVelocity = lexer.ReadPropertyVector("LinearVelocity");

		// angular velocity
		p_kineticProperties.AngularVelocity = lexer.ReadPropertyVector("AngularVelocity");

		// damping coefficient velocity
		p_kineticProperties.DampingCoefficient = lexer.ReadPropertyReal("DampingCoefficient");

		// CanSleep flag
		p_kineticProperties.CanSleep = lexer.ReadPropertyBoolean("CanSleep");

		// 'KineticProperties' container closing brace
		lexer.ReadToken(LexerTokenType::ClosingBrace);

		// compute derivative quantities
		p_kineticProperties.ComputeDerivativeProperties();

		// ensure active
		p_kineticProperties.Active = true;
	}

	BodyPtr LoadBody(LookaheadInputStreamPtr p_pLookaheadInputStream)
	{
		Lexer lexer(p_pLookaheadInputStream);

		// 'Body' container identifier
		lexer.ReadIdentifier("Body");

		// 'Body' container opening brace
		lexer.ReadToken(LexerTokenType::OpeningBrace);

		// mandatory Type property
		String strBodyType = lexer.ReadPropertyString("Type");
		if (strBodyType != "Rigid")
			throw new MesonException(
				"Only rigid bodies supported to date.",
				__FILE__, __LINE__);

		// mandatory ID property
		String strBodyId = lexer.ReadPropertyString("Id");

		// create rigid body instance with the given ID
		RigidBody* pRigidBody = new RigidBody(strBodyId);
		BodyPtr pBody(pRigidBody);

		// mandatory GeometryId property
		String strGeometryId = lexer.ReadPropertyString("GeometryId");

		// get geometry to bind to body
		GeometryPtr pGeometry
			= GravitasEngine::GetInstance()->GetGeometryManager()->GetGeometry(strGeometryId);

		pRigidBody->SetGeometry(pGeometry);

		// mandatory BoundingVolumeType property
		String strBoundingVolumeType = lexer.ReadPropertyString("BoundingVolumeType");

		if (pGeometry->IsBounded() && strBoundingVolumeType.Length() > 0)
		{
			BoundingVolumePtr pBoundingVolume;
			if (strBoundingVolumeType == "Sphere")
			{
				BoundingSphere* pBoundingSphere = new BoundingSphere();
				pGeometry->ComputeBoundingVolume(*pBoundingSphere);
				pBoundingVolume = pBoundingSphere;
			}
			else if (strBoundingVolumeType == "AxisAlignedBox")
			{
				BoundingAxisAlignedBox* pBoundingAxisAlignedBox = new BoundingAxisAlignedBox();
				pGeometry->ComputeBoundingVolume(*pBoundingAxisAlignedBox);
				pBoundingVolume = pBoundingAxisAlignedBox;
			}
			else if (strBoundingVolumeType == "OrientedBox")
			{
				BoundingOrientedBox* pBoundingOrientedBox = new BoundingOrientedBox();
				pGeometry->ComputeBoundingVolume(*pBoundingOrientedBox);
				pBoundingVolume = pBoundingOrientedBox;
			}
			else
				throw new MesonException(
					"Parsing Error: Invalid bounding volume type. Only 'Sphere', 'AxisAlignedBox' and 'OrientedBox' allowed.",
					__FILE__, __LINE__);

			pRigidBody->SetBoundingVolume(pBoundingVolume);
		}

		// mandatory MassDensity property
		Real rMassDensity = lexer.ReadPropertyReal("MassDensity");
		if (rMassDensity > (Real) 0.0)
			pRigidBody->SetGeometryAndMassProperties(pGeometry, rMassDensity);
		else
			pRigidBody->GetMassProperties().SetInfiniteMass();

		// material properties
		String strMaterialId = lexer.ReadPropertyString("Material");
		pRigidBody->SetMaterial(
			GravitasEngine::GetInstance()->GetMaterialManager()->GetMaterial(strMaterialId));

		// kinetic properties
		LoadKineticProperties(p_pLookaheadInputStream, pRigidBody->GetKineticProperties());

		// attached forces
		lexer.ReadIdentifier("Forces");
		lexer.ReadToken(LexerTokenType::Assignment);
		TArrayList<LexerToken> listTokens;
		lexer.ReadTokenList(LexerTokenType::String, listTokens);
		ForceManager* pForceManager = GravitasEngine::GetInstance()->GetForceManager();
		for (size_t unIndex = 0; unIndex < listTokens.Size(); unIndex++)
			pRigidBody->GetForceAccumulator().AddForce(
				pForceManager->GetForce(listTokens[unIndex].ValueString));

		// 'Body' container closing brace
		lexer.ReadToken(LexerTokenType::ClosingBrace);

		return pBody;
	}
};

Meson_Gravitas_Space_END

SpaceManager::SpaceManager()
	: m_pSpaceManagerState(NULL)
{
	m_pSpaceManagerState = new SpaceManagerState();
}

SpaceManager::~SpaceManager()
{
	delete m_pSpaceManagerState;
}

TEnumerator<ISpaceFactory*>& SpaceManager::GetSpaceFactories(void)
{
	return m_pSpaceManagerState->m_mapSpaceFactories.GetValueEnumerator();
}

ISpaceFactory* SpaceManager::GetSpaceFactory(const Meson::Common::Text::String& p_strName)
{
	ISpaceFactory* pSpaceFactory;
	if (!m_pSpaceManagerState->m_mapSpaceFactories.ContainsKey(p_strName, pSpaceFactory))
		throw new MesonException(
			"Space factory '" + p_strName + "' not registered.",
			__FILE__, __LINE__);

	return pSpaceFactory;
}

void SpaceManager::RegisterSpaceFactory(ISpaceFactory* p_pSpaceFactory)
{
	THashMap<String, ISpaceFactory *> &mapSpaceFactories
		= m_pSpaceManagerState->m_mapSpaceFactories;

	const String &strName = p_pSpaceFactory->GetName();

	if (mapSpaceFactories.ContainsKey(strName))
		throw new MesonException(
			"Space Factory '" + strName + "' already registered.",
			__FILE__, __LINE__);

	mapSpaceFactories[strName] = p_pSpaceFactory;
}

void SpaceManager::UnregisterSpaceFactory(ISpaceFactory* p_pSpaceFactory)
{
	THashMap<String, ISpaceFactory* > &mapSpaceFactories
		= m_pSpaceManagerState->m_mapSpaceFactories;

	const String& strName = p_pSpaceFactory->GetName();

	if (!mapSpaceFactories.ContainsKey(strName))
		throw new MesonException(
			"Space Factory '" + strName + "' is not registered and cannot be removed.",
			__FILE__, __LINE__);

	mapSpaceFactories.RemoveKey(strName);
}

void SpaceManager::UnregisterAllSpaceFactories(void)
{
	m_pSpaceManagerState->m_mapSpaceFactories.Clear();
}

SpacePtr SpaceManager::LoadSpace(LookaheadInputStreamPtr p_pLookaheadInputStream)
{
	Lexer lexer(p_pLookaheadInputStream);

	// 'Space' container identifier
	lexer.ReadIdentifier("Space");

	// 'Space' container opening brace
	lexer.ReadToken(LexerTokenType::OpeningBrace);

	// mandatory Type property
	String strSpaceType = lexer.ReadPropertyString("Type");

	// mandatory ID property
	String strSpaceId = lexer.ReadPropertyString("Id");

	// get the space factory for the required type
	ISpaceFactory* pSpaceFactory
		= GetSpaceFactory(strSpaceType);

	// create space with given id from the specified factory
	SpacePtr pSpace = pSpaceFactory->CreateSpace(strSpaceId);

	// get custom properties
	lexer.ReadCustomProperties(*pSpace);

	// bodies contained in space
	lexer.ReadIdentifier("Bodies");
	lexer.ReadToken(LexerTokenType::OpeningBrace);
	while (true)
	{
		LexerToken lexerToken = lexer.ReadToken();
		if (lexerToken.Type == LexerTokenType::ClosingBrace)
			break;

		if (lexerToken.Type != LexerTokenType::Identifier
			|| lexerToken.ValueString != "Body")
			throw new MesonException(
				"Parsing Error: Expected identifier 'Body'",
				__FILE__, __LINE__);

		p_pLookaheadInputStream->Pushback("Body", 4);
		BodyPtr pBody = m_pSpaceManagerState->LoadBody(p_pLookaheadInputStream);
		pSpace->AddBody(pBody);
	}

	// 'Space' container closing brace
	lexer.ReadToken(LexerTokenType::ClosingBrace);

	// rebuild space
	pSpace->Rebuild();

	return pSpace;
}
