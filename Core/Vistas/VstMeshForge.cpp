//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#include "VstMeshForge.h"
#include "VstBoundingSphere.h"
#include "VstBoundingAxisAlignedBox.h"
#include "VstWavefrontLoader.h"
#include "VstVistasEngine.h"

#include "FileInputStream.h"
#include "Converter.h"
#include "File.h"

using namespace Meson::Common;
using namespace Meson::Common::IO;
using namespace Meson::Common::Maths;
using namespace Meson::Common::Collections;

using namespace Meson::Vistas;

Meson_Vistas_BEGIN

//----------------------------------------------------------------------------------------------
GeometryListPtr MeshForge::CreateMeshListFromFile(const Meson::Common::Text::String p_strFilename, float p_fScale, dword p_dwMeshFlags)
{
	// TODO: A factory should provide file loaders through which the actual loading is done.
	VistasEngine::GetInstance()->Logger().Out << "\nLoading model file '" << p_strFilename << "'...\n";
	WavefrontLoader loader;
	return loader.Load(p_strFilename);
}
//----------------------------------------------------------------------------------------------
GeometryPtr MeshForge::CreateMeshFromFile(const Meson::Common::Text::String p_strFilename, float p_fScale, dword p_dwMeshFlags)
{
	// TODO: A factory should provide file loaders through which the actual loading is done.
	VistasEngine::GetInstance()->Logger().Out << "\nLoading model file '" << p_strFilename << "'...\n";
	WavefrontLoader loader;
	return loader.LoadSingle(p_strFilename);
}
//----------------------------------------------------------------------------------------------
GeometryPtr MeshForge::CreateBoxMesh(float p_fWidth, float p_fHeight, float p_fDepth, bool p_bInvertNormals, dword p_dwMeshFlags)
{
	float fWidth  = p_fWidth * 0.5f,
		  fHeight = p_fHeight * 0.5f,
		  fDepth  = p_fDepth * 0.5f,
		  fRadius = Mathf::Sqrt(fWidth * fWidth + fHeight * fHeight + fDepth * fDepth);

	Vector3f vec3MinExtent(-fWidth, -fHeight, -fDepth),
			 vec3MaxExtent(fWidth, fHeight, fDepth);

	/** Create mesh and respective vertex and index buffers
	 */
	AbstractMeshPtr pAbstractMesh = CreateAbstractMesh(p_dwMeshFlags);
	IndexDataPtr pIndexData = pAbstractMesh->GetIndexData();
	VertexDataPtr pVertexData = pAbstractMesh->GetVertexData();
	VertexDescriptorPtr pVertexDescriptor = pAbstractMesh->GetVertexData()->GetVertexDescriptor();

	Vertex vertex(pVertexDescriptor);

	if (vertex.HasColour())
		vertex.SetColour(Colour::Black);

	/** Create cube vertices
	 */
	vertex.SetPosition(Vector3f(-fWidth, fHeight, fDepth)); pVertexData->Add(vertex);
	vertex.SetPosition(Vector3f( fWidth, fHeight, fDepth)); pVertexData->Add(vertex);
	vertex.SetPosition(Vector3f(-fWidth,-fHeight, fDepth)); pVertexData->Add(vertex);
	vertex.SetPosition(Vector3f( fWidth,-fHeight, fDepth)); pVertexData->Add(vertex);

	vertex.SetPosition(Vector3f( fWidth, fHeight, -fDepth)); pVertexData->Add(vertex);
	vertex.SetPosition(Vector3f(-fWidth, fHeight, -fDepth)); pVertexData->Add(vertex);
	vertex.SetPosition(Vector3f( fWidth,-fHeight, -fDepth)); pVertexData->Add(vertex);
	vertex.SetPosition(Vector3f(-fWidth,-fHeight, -fDepth)); pVertexData->Add(vertex);

	vertex.SetPosition(Vector3f(-fWidth, fHeight, -fDepth)); pVertexData->Add(vertex);
	vertex.SetPosition(Vector3f( fWidth, fHeight, -fDepth)); pVertexData->Add(vertex);
	vertex.SetPosition(Vector3f(-fWidth, fHeight,  fDepth)); pVertexData->Add(vertex);
	vertex.SetPosition(Vector3f( fWidth, fHeight,  fDepth)); pVertexData->Add(vertex);

	vertex.SetPosition(Vector3f( fWidth,-fHeight, -fDepth)); pVertexData->Add(vertex);
	vertex.SetPosition(Vector3f(-fWidth,-fHeight, -fDepth)); pVertexData->Add(vertex);
	vertex.SetPosition(Vector3f( fWidth,-fHeight,  fDepth)); pVertexData->Add(vertex);
	vertex.SetPosition(Vector3f(-fWidth,-fHeight,  fDepth)); pVertexData->Add(vertex);

	vertex.SetPosition(Vector3f(-fWidth, fHeight, -fDepth)); pVertexData->Add(vertex);
	vertex.SetPosition(Vector3f(-fWidth, fHeight,  fDepth)); pVertexData->Add(vertex);
	vertex.SetPosition(Vector3f(-fWidth,-fHeight, -fDepth)); pVertexData->Add(vertex);
	vertex.SetPosition(Vector3f(-fWidth,-fHeight,  fDepth)); pVertexData->Add(vertex);

	vertex.SetPosition(Vector3f( fWidth, fHeight,  fDepth)); pVertexData->Add(vertex);
	vertex.SetPosition(Vector3f( fWidth, fHeight, -fDepth)); pVertexData->Add(vertex);
	vertex.SetPosition(Vector3f( fWidth,-fHeight,  fDepth)); pVertexData->Add(vertex);
	vertex.SetPosition(Vector3f( fWidth,-fHeight, -fDepth)); pVertexData->Add(vertex);
	
	/** Add face indices to index data, flipping normals if required
	 */
	if (p_bInvertNormals)
	{
		for (uint uiQuadGroup=0, uiVertex=0; uiQuadGroup < 23; uiQuadGroup+=4)
		{
			pIndexData->Add(uiQuadGroup+0, uiQuadGroup+1, uiQuadGroup+2);
			pIndexData->Add(uiQuadGroup+1, uiQuadGroup+3, uiQuadGroup+2);

			if (vertex.HasTextureCoordinates2D())
			{
				pVertexData->GetVertex(uiQuadGroup + 0)->SetTextureCoordinates2D(Vector2f(0,0));
				pVertexData->GetVertex(uiQuadGroup + 1)->SetTextureCoordinates2D(Vector2f(1,0));
				pVertexData->GetVertex(uiQuadGroup + 2)->SetTextureCoordinates2D(Vector2f(0,1));
				pVertexData->GetVertex(uiQuadGroup + 3)->SetTextureCoordinates2D(Vector2f(1,1));
			}
		}
	}
	else
	{
		for (uint uiQuadGroup=0, uiVertex=0; uiQuadGroup < 23; uiQuadGroup+=4)
		{
			pIndexData->Add(uiQuadGroup+0, uiQuadGroup+2, uiQuadGroup+1);
			pIndexData->Add(uiQuadGroup+1, uiQuadGroup+2, uiQuadGroup+3);

			if (vertex.HasTextureCoordinates2D())
			{
				pVertexData->GetVertex(uiQuadGroup + 0)->SetTextureCoordinates2D(Vector2f(0,0));
				pVertexData->GetVertex(uiQuadGroup + 1)->SetTextureCoordinates2D(Vector2f(1,0));
				pVertexData->GetVertex(uiQuadGroup + 2)->SetTextureCoordinates2D(Vector2f(0,1));
				pVertexData->GetVertex(uiQuadGroup + 3)->SetTextureCoordinates2D(Vector2f(1,1));
			}
		}
	}

	/** Update mesh normals, tangents and binormals
	 */
	if (vertex.HasNormal())
	{
		pAbstractMesh->UpdateNormals();
	
		if (vertex.HasTangent())
			pAbstractMesh->UpdateTangents(true);
	}

	/** Create a vertex declaration and hardware index and vertex buffers
	 */
	IndexBufferPtr pIndexBuffer = CreateIndexBuffer(pIndexData);
	VertexBufferGroupPtr pVertexBufferGroup = CreateVertexBufferGroup(pVertexData);
	VertexDeclarationPtr pVertexDeclaration = VistasEngine::GetInstance()->GetHardwareResourceManager()->CreateVertexDeclaration(false);
	pVertexDescriptor->ExportDescriptor(pVertexDeclaration);

	/** Create mesh
	 */ 
	GeometryPtr pGeometry(new Geometry());
	pGeometry->SetGeometryParameters(IRenderDevice::PT_TRIANGLE_LIST, pVertexDeclaration, pVertexBufferGroup, pIndexBuffer);

	/** Set mesh bounding volume
	 */
	BoundingVolumePtr pBoundingVolume(new BoundingAxisAlignedBox(vec3MinExtent, vec3MaxExtent));
	pGeometry->SetBoundingVolume(pBoundingVolume);

	return pGeometry;
}
//----------------------------------------------------------------------------------------------
GeometryPtr MeshForge::CreatePlaneMesh(const Vector3f& p_vec3Normal, float p_fDistance, float p_fLength, float p_fBreadth, float p_fUMultiplier, float p_fVMultiplier, bool p_bInvertNormals, dword p_dwMeshFlags)
{
	AbstractMeshPtr pAbstractMesh = CreateAbstractMesh(p_dwMeshFlags);
	IndexDataPtr pIndexData = pAbstractMesh->GetIndexData();
	VertexDataPtr pVertexData = pAbstractMesh->GetVertexData();
	VertexDescriptorPtr pVertexDescriptor = pAbstractMesh->GetVertexData()->GetVertexDescriptor();

	Vertex vertex(pVertexDescriptor);

	if (vertex.HasColour())
		vertex.SetColour(Colour::Black);
	
	/** Determine plane basis
	 */

	Vector3f vec3Normal,
			 vec3AxisA, 
			 vec3AxisB;

	vec3Normal = p_vec3Normal.NormaliseCopy();
	vec3Normal.ComputeOrthonormals(vec3AxisA, vec3AxisB);

	vec3AxisA *= p_fLength;
	vec3AxisB *= p_fBreadth;

	/** Determine quad vertices
	 */
	Vector3f vec3Centre = vec3Normal * p_fDistance;
	Vector3f vec3VertexA = vec3Centre - vec3AxisA + vec3AxisB;
	Vector3f vec3VertexB = vec3Centre + vec3AxisA + vec3AxisB;
	Vector3f vec3VertexC = vec3Centre - vec3AxisA - vec3AxisB;
	Vector3f vec3VertexD = vec3Centre + vec3AxisA - vec3AxisB;

	/** Add vertices to vertex data
	 */
	vertex.SetPosition(vec3VertexA); pVertexData->Add(vertex);
	vertex.SetPosition(vec3VertexB); pVertexData->Add(vertex);
	vertex.SetPosition(vec3VertexC); pVertexData->Add(vertex);
	vertex.SetPosition(vec3VertexD); pVertexData->Add(vertex);

	/** Add face indices to index data, flipping normals if required
	 */
	if (p_bInvertNormals)
	{
		pIndexData->Add(0, 1, 2);
		pIndexData->Add(1, 3, 2);
	}
	else
	{
		pIndexData->Add(0, 2, 1);
		pIndexData->Add(1, 2, 3);
	}

	if (vertex.HasTextureCoordinates2D())
	{
		pVertexData->GetVertex(0)->SetTextureCoordinates2D(Vector2f(0 * p_fUMultiplier,0 * p_fVMultiplier));
		pVertexData->GetVertex(1)->SetTextureCoordinates2D(Vector2f(1 * p_fUMultiplier,0 * p_fVMultiplier));
		pVertexData->GetVertex(2)->SetTextureCoordinates2D(Vector2f(0 * p_fUMultiplier,1 * p_fVMultiplier));
		pVertexData->GetVertex(3)->SetTextureCoordinates2D(Vector2f(1 * p_fUMultiplier,1 * p_fVMultiplier));
	}

	/** Update mesh normals, tangents and binormals
	 */
	if (vertex.HasNormal())
	{
		pAbstractMesh->UpdateNormals();
		
		if (vertex.HasTangent())
			pAbstractMesh->UpdateTangents(true);
	}

	/** Create a vertex declaration and hardware index and vertex buffers
	 */
	IndexBufferPtr pIndexBuffer = CreateIndexBuffer(pIndexData);
	VertexBufferGroupPtr pVertexBufferGroup = CreateVertexBufferGroup(pVertexData);
	VertexDeclarationPtr pVertexDeclaration = VistasEngine::GetInstance()->GetHardwareResourceManager()->CreateVertexDeclaration(false);
	pVertexDescriptor->ExportDescriptor(pVertexDeclaration);

	/** Create mesh
	 */ 
	GeometryPtr pGeometry(new Geometry());
	pGeometry->SetGeometryParameters(IRenderDevice::PT_TRIANGLE_LIST, pVertexDeclaration, pVertexBufferGroup, pIndexBuffer);

	/** Set mesh bounding volume
	 */
	float fRadius = Mathf::Sqrt(p_fLength * p_fLength + p_fBreadth * p_fBreadth);
	BoundingVolumePtr pBoundingVolume(new BoundingAxisAlignedBox(vec3VertexC, vec3VertexB));
	pGeometry->SetBoundingVolume(pBoundingVolume);	
	
	return pGeometry;
}
//----------------------------------------------------------------------------------------------
GeometryPtr MeshForge::CreateSphereMesh(float p_fRadius, uint p_uiSegments, bool p_bInvertNormals, dword p_dwMeshFlags)
{
	AbstractMeshPtr pAbstractMesh = CreateAbstractMesh(p_dwMeshFlags);
	IndexDataPtr pIndexData = pAbstractMesh->GetIndexData();
	VertexDataPtr pVertexData = pAbstractMesh->GetVertexData();
	VertexDescriptorPtr pVertexDescriptor = pAbstractMesh->GetVertexData()->GetVertexDescriptor();

	Vertex vertex(pVertexDescriptor);

	if (vertex.HasColour())
		vertex.SetColour(Colour::Black);

	/** Initialise variable working set
	 */
	Vector3f vec3Position;
	Vector2f vec2TexCoords;

	uint uiRowWidth = p_uiSegments + 1;

	float fRow, fdRow, fColumn, fdColumn,
		  fPhi, fRho, fPsi, fChi,
		  fU, fV, fdU, fdV;

	/** Set the deltas for strip and slice angles,
		and u & v coordinates sets.
	 */ 
	fdU = fdV = 1.0f / ((float)p_uiSegments);
	fdColumn = fdU * Mathf::PiTwo;
	fdRow = fdU * Mathf::Pi;

	fU = fV = fColumn = fRow = 0.0;

	/** Add vertices to vertex data
	 */
	for (uint uiRow = 0; uiRow <= p_uiSegments; uiRow++, fRow += fdRow, fU += fdU)
	{
		fPhi = Mathf::Cos(fRow);
		fChi = Mathf::Sin(fRow);
		fColumn = fV = 0.0f;

		for (uint uiColumn = 0; uiColumn <= p_uiSegments; uiColumn++, fColumn += fdColumn, fV += fdV)
		{
			fRho = Mathf::Sin(fColumn) * fChi;
			fPsi = Mathf::Cos(fColumn) * fChi;

			if (vertex.HasTextureCoordinates2D())
			{
				vec2TexCoords.Set(fU, fV);
				vertex.SetTextureCoordinates2D(vec2TexCoords);
			}

			vec3Position.Set(fRho * p_fRadius, fPhi * p_fRadius, fPsi * p_fRadius); vertex.SetPosition(vec3Position); 
			vec3Position.Normalise(); 

			if (vertex.HasNormal())
			{
				// HINT: Could move normal inversion to an outer loop
				if (p_bInvertNormals) 
					vertex.SetNormal(-vec3Position); 
				else 
					vertex.SetNormal(vec3Position);
			}

			pVertexData->Add(vertex);
		}
	}

	/** Add face indices to index data, flipping normals if required
	 */
	if (p_bInvertNormals)
	{
		for (uint uiRow = 0; uiRow < p_uiSegments; uiRow++)
		{
			for (uint uiColumn = uiRow * uiRowWidth; uiColumn < p_uiSegments + uiRow * uiRowWidth; uiColumn++)
			{
				pIndexData->Add( uiColumn, uiColumn + 1, uiColumn + uiRowWidth );
				pIndexData->Add( uiColumn + 1, uiColumn + uiRowWidth + 1, uiColumn + uiRowWidth);
			}
		}
	}
	else
	{
		for (uint uiRow = 0; uiRow < p_uiSegments; uiRow++)
		{
			for (uint uiColumn = uiRow * uiRowWidth; uiColumn < p_uiSegments + uiRow * uiRowWidth; uiColumn++)
			{
				pIndexData->Add( uiColumn, uiColumn + uiRowWidth, uiColumn + 1 );
				pIndexData->Add( uiColumn + 1, uiColumn + uiRowWidth, uiColumn + uiRowWidth + 1);
			}
		}
	}

	/** Update tangents and binormals
	 */
	if (vertex.HasTangent())
		pAbstractMesh->UpdateTangents();

	/** Create a vertex declaration and hardware index and vertex buffers
	 */
	IndexBufferPtr pIndexBuffer = CreateIndexBuffer(pIndexData);
	VertexBufferGroupPtr pVertexBufferGroup = CreateVertexBufferGroup(pVertexData);
	VertexDeclarationPtr pVertexDeclaration = VistasEngine::GetInstance()->GetHardwareResourceManager()->CreateVertexDeclaration(false);
	pVertexDescriptor->ExportDescriptor(pVertexDeclaration);

	/** Create mesh
	 */ 
	GeometryPtr pGeometry(new Geometry());
	pGeometry->SetGeometryParameters(IRenderDevice::PT_TRIANGLE_LIST, pVertexDeclaration, pVertexBufferGroup, pIndexBuffer);

	/** Set mesh bounding volume
	 */
	BoundingVolumePtr pBoundingVolume(new BoundingSphere(Vector3f::Zero, p_fRadius));
	pGeometry->SetBoundingVolume(pBoundingVolume);
	
	return pGeometry;
}
//----------------------------------------------------------------------------------------------
GeometryPtr MeshForge::CreateCylinderMesh(float p_fRadius, float p_fHeight, uint p_uiSegments, bool p_bCreateCaps, bool p_bInvertNormals, dword p_dwMeshFlags)
{
	AbstractMeshPtr pAbstractMesh = CreateAbstractMesh(p_dwMeshFlags);
	IndexDataPtr pIndexData = pAbstractMesh->GetIndexData();
	VertexDataPtr pVertexData = pAbstractMesh->GetVertexData();
	VertexDescriptorPtr pVertexDescriptor = pAbstractMesh->GetVertexData()->GetVertexDescriptor();

	Vertex vertex(pVertexDescriptor);

	if (vertex.HasColour())
		vertex.SetColour(Colour::Black);

	/** Initialise variable working set
	 */
	Vector3f vec3Position;
	Vector2f vec2TexCoords;

	uint uiRowWidth = p_uiSegments + 1;

	float fColumn, fdColumn,
		  fRho, fPsi,
		  fU, fdU;

	/** Set the deltas for strip and slice angles,
		and u & v coordinates sets.
	 */ 
	fdU = 1.0f / ((float)p_uiSegments);
	fdColumn = fdU * Mathf::PiTwo;
	fU = fColumn = 0.0;
	
	// TODO: Remove slice and slice delta, replace with colum and dcolumn
	// TODO: optimise and implement normal inversion
	float fSlice = fColumn, fSliceDelta = fdColumn;

	/** Add vertices to vertex data, starting from the cylindrical body
	 */
	for (uint uiColumn = 0; uiColumn <= p_uiSegments; uiColumn++, fColumn+=fdColumn, fU += fdU)
	{
		fRho = Mathf::Sin(fColumn);
		fPsi = Mathf::Cos(fColumn);

		if (vertex.HasTextureCoordinates2D())
		{
			vec2TexCoords.Set(fU, 0);
			vertex.SetTextureCoordinates2D(vec2TexCoords);
		}

		vec3Position.Set(fRho * p_fRadius, p_fHeight * 0.5f, fPsi * p_fRadius);
		vertex.SetPosition(vec3Position);

		if (vertex.HasNormal())
		{
			vec3Position.Set(fRho, 0, fPsi); 
			vertex.SetNormal(vec3Position);
		}

		pVertexData->Add(vertex);

		// Bottom row
		if (vertex.HasTextureCoordinates2D())
		{
			vec2TexCoords.Y = 1.0f;
			vertex.SetTextureCoordinates2D(vec2TexCoords);
		}
		
		vec3Position.Set(fRho * p_fRadius, p_fHeight * -0.5f, fPsi * p_fRadius);
		vertex.SetPosition(vec3Position);

		pVertexData->Add(vertex);
	}

	// Set set indices and texture coordinates
	for (uint uiSlice = 0; uiSlice < p_uiSegments * 2; uiSlice += 2)
	{
		pIndexData->Add( uiSlice, uiSlice + 1, uiSlice + 2 );
		pIndexData->Add( uiSlice + 1, uiSlice + 3, uiSlice + 2);
	}

	// Do we need to cap the cylinder?
	if (p_bCreateCaps)
	{
		uint uiTCapStart = pVertexData->GetVertexCount(0),
			 uiBCapStart = uiTCapStart + p_uiSegments + 1;

		fSlice = 0.0f;

		for (uint uiCircum = 0; uiCircum <= p_uiSegments; uiCircum++, fSlice+=fSliceDelta)
		{
				fRho = Mathf::Sin(fSlice);
				fPsi = Mathf::Cos(fSlice);

				if (vertex.HasTextureCoordinates2D())
				{
					vec2TexCoords.Set(0.5f + 0.5f * fRho, 0.5f + 0.5f * fPsi);
					vertex.SetTextureCoordinates2D(vec2TexCoords);
				}

				vec3Position.Set(fRho * p_fRadius, p_fHeight * 0.5f, fPsi * p_fRadius);
				vertex.SetPosition(vec3Position);
				
				if (vertex.HasNormal())
					vertex.SetNormal(Vector3f::Up);
				
				pVertexData->Add(vertex);
		}

		fSlice = 0.0f;

		// Repeated to preserve vertex cache coherency
		for (uint uiCircum = 0; uiCircum <= p_uiSegments; uiCircum++, fSlice+=fSliceDelta)
		{
				fRho = Mathf::Sin(fSlice);
				fPsi = Mathf::Cos(fSlice);

				if (vertex.HasTextureCoordinates2D())
				{
					vec2TexCoords.Set(0.5f + 0.5f * fRho, 0.5f + 0.5f * fPsi);
					vertex.SetTextureCoordinates2D(vec2TexCoords);
				}

				vec3Position.Set(fRho * p_fRadius, p_fHeight * -0.5f, fPsi * p_fRadius);
				vertex.SetPosition(vec3Position);
				
				if (vertex.HasNormal())
					vertex.SetNormal(Vector3f::Down);

				pVertexData->Add(vertex);
		}

		for (uint uiVertex = 1; uiVertex < p_uiSegments; uiVertex++)
		{
			pIndexData->Add(uiTCapStart, uiTCapStart + uiVertex, uiTCapStart + uiVertex + 1);
			pIndexData->Add(uiBCapStart, uiBCapStart + uiVertex + 1, uiBCapStart + uiVertex);
		}
	}

	// Update mesh normals
	if (vertex.HasNormal() && vertex.HasTangent())
		pAbstractMesh->UpdateTangents();

	// Create Hardware Index and Vertex buffers
	IndexBufferPtr pIndexBuffer = CreateIndexBuffer(pIndexData);
	VertexBufferGroupPtr pVertexBufferGroup = CreateVertexBufferGroup(pVertexData);

	// Create Vertex Declaration
	VertexDeclarationPtr pVertexDeclaration = VistasEngine::GetInstance()->GetHardwareResourceManager()->CreateVertexDeclaration(false);
	pVertexDescriptor->ExportDescriptor(pVertexDeclaration);

	// Create a new Mesh
	GeometryPtr pGeometry(new Geometry());
	pGeometry->SetGeometryParameters(IRenderDevice::PT_TRIANGLE_LIST, pVertexDeclaration, pVertexBufferGroup, pIndexBuffer);

	// Create mesh bounding volume
	float fRadius = Mathf::Sqrt(p_fHeight * p_fHeight + p_fRadius * p_fRadius);
	BoundingVolumePtr pBoundingVolume(new BoundingSphere(Vector3f::Zero, fRadius)); 
	pGeometry->SetBoundingVolume(pBoundingVolume);

	return pGeometry;
}
//----------------------------------------------------------------------------------------------
GeometryPtr MeshForge::CreateMesh(AbstractMeshPtr p_pAbstractMesh)
{
	return GeometryPtr(NULL);
}
//----------------------------------------------------------------------------------------------
IndexBufferPtr MeshForge::CreateIndexBuffer(IndexDataPtr p_pIndexData)
{
	uint uiIndexBufferSize = p_pIndexData->GetIndexCount();
	IndexBuffer::IndexType eIndexType = (uiIndexBufferSize < 0x10000) ? IndexBuffer::IT_16BIT : IndexBuffer::IT_32BIT;
	IndexBufferPtr pIndexBuffer = VistasEngine::GetInstance()->GetHardwareResourceManager()->CreateIndexBuffer( eIndexType, uiIndexBufferSize, HardwareResource::MP_DEFAULT, HardwareResource::MU_DEFAULT, false);

	pIndexBuffer->Lock(HardwareResource::ML_NONE);

	// If index buffer contains 16 bit values, we must read each element as a short, not a uint,
	// which prevents us from applying a flat copy from IndexData to IndexBuffer.
	// TODO: Consider basing IndexData on two buffers for contiguous copies.
	if (eIndexType == IndexBuffer::IT_16BIT)
	{
		TArrayList<ushort> contiguousIndexData(uiIndexBufferSize);

		// We can safely cast the uint to ushort
		for (uint uiIdx = 0; uiIdx < uiIndexBufferSize; uiIdx++)
			contiguousIndexData.Add((ushort)p_pIndexData->GetIndex(uiIdx));
		
		pIndexBuffer->Write(0, 0, eIndexType * uiIndexBufferSize, (const byte*)contiguousIndexData.ElementPtr(0));
	}
	else
	{
		pIndexBuffer->Write(0, 0, eIndexType * uiIndexBufferSize, (const byte*)p_pIndexData->GetIndexBuffer(0));
	}

	pIndexBuffer->Unlock();

	return pIndexBuffer;
}
//----------------------------------------------------------------------------------------------
VertexBufferPtr MeshForge::CreateVertexBuffer(VertexDataPtr p_pVertexData, uint p_uiBufferIndex)
{
	if (p_uiBufferIndex >= p_pVertexData->GetBufferCount())
		throw new MesonException("Unabled to create vertex buffer : Invalid buffer index.", __FILE__, __LINE__);

	uint uiVertexCount = p_pVertexData->GetVertexCount(p_uiBufferIndex),
		 uiVertexSize = p_pVertexData->GetPartialVertexSize(p_uiBufferIndex);

	VertexBufferPtr pVertexBuffer = VistasEngine::GetInstance()->GetHardwareResourceManager()->CreateVertexBuffer( uiVertexSize, uiVertexCount, HardwareResource::MP_DEFAULT, HardwareResource::MU_DEFAULT, false);

	pVertexBuffer->Lock(HardwareResource::ML_NONE);
	pVertexBuffer->Write(0, 0, uiVertexSize * uiVertexCount, (const byte*)p_pVertexData->GetBuffer(p_uiBufferIndex, 0));
	pVertexBuffer->Unlock();

	return pVertexBuffer;
}
//----------------------------------------------------------------------------------------------
VertexBufferGroupPtr MeshForge::CreateVertexBufferGroup(VertexDataPtr p_pVertexData)
{
	uint uiVertexCount,
		 uiVertexSize;

	HardwareResourceManager *pHardwareResourceManager = VistasEngine::GetInstance()->GetHardwareResourceManager();
	VertexBufferGroupPtr pVertexBufferGroup(new VertexBufferGroup());
	VertexBufferPtr pVertexBuffer(NULL);

	for (uint uiBufferIndex = 0, uiBufferCount = p_pVertexData->GetBufferCount(); uiBufferIndex < uiBufferCount; uiBufferIndex++)
	{
		uiVertexCount = p_pVertexData->GetVertexCount(uiBufferIndex);
		uiVertexSize = p_pVertexData->GetPartialVertexSize(uiBufferIndex);

		pVertexBuffer = pHardwareResourceManager->CreateVertexBuffer(uiVertexSize, uiVertexCount, HardwareResource::MP_DEFAULT, HardwareResource::MU_DEFAULT, false);
		pVertexBufferGroup->AddVertexBuffer(pVertexBuffer);

		pVertexBuffer->Lock(HardwareResource::ML_NONE);
		pVertexBuffer->Write(0, 0, uiVertexSize * uiVertexCount, (const byte*)p_pVertexData->GetBuffer(uiBufferIndex, 0));
		pVertexBuffer->Unlock();
	}

	return pVertexBufferGroup;
}
//----------------------------------------------------------------------------------------------
AbstractMeshPtr MeshForge::CreateAbstractMesh(dword p_dwMeshFlags)
{
	/** Set mesh vertex format
	 */
	VertexDescriptorPtr pVertexDescriptor(new VertexDescriptor());

	pVertexDescriptor->AddSemantic(VertexDescriptor::VDS_POSITION, 0);

	if (p_dwMeshFlags && MF_COLOUR)
		pVertexDescriptor->AddSemantic(VertexDescriptor::VDS_COLOUR, 0);
	
	if (p_dwMeshFlags && MF_NORMALS)
		pVertexDescriptor->AddSemantic(VertexDescriptor::VDS_NORMAL, 0);

	if (p_dwMeshFlags && MF_BINORMALS)
		pVertexDescriptor->AddSemantic(VertexDescriptor::VDS_BINORMAL, 0);

	if (p_dwMeshFlags && MF_TANGENTS)
		pVertexDescriptor->AddSemantic(VertexDescriptor::VDS_TANGENT, 0);

	if (p_dwMeshFlags && MF_TEXCOORDS)
		pVertexDescriptor->AddSemantic(VertexDescriptor::VDS_TEXCOORD2D, 0);

	///** Bind vertex to descriptor
	// */
	//Vertex vertex(pVertexDescriptor);
	//vertex.SetColour(Colour::Black);

	/** Create a new abstract mesh
	 */
	return AbstractMeshPtr(new AbstractMesh(pVertexDescriptor));
}
//----------------------------------------------------------------------------------------------

Meson_Vistas_END
