//----------------------------------------------------------------------------------------------
//  Utility class for the generation of Geometry objects.
//----------------------------------------------------------------------------------------------
//	Author			Date		Version		Description
//----------------------------------------------------------------------------------------------
//	Keith Bugeja	01/07/2007	1.0.0		First version
//----------------------------------------------------------------------------------------------
#pragma once

#include "VstTypes.h"

#include "VstGeometry.h"
#include "VstAbstractMesh.h"

Meson_Vistas_BEGIN
	//----------------------------------------------------------------------------------------------
	//	\class MeshForge
	//	\brief Utility class for generating Geometry objects.
	//	This class is envisaged to evolve into a set of smaller but functionally richer
	//	objects, such as factories for loading and saving geometry objects into a number of 
	//	different formats, factories for the creation and modification of geometry primitives,
	//	and tools for exchanging between concrete and abstract geometry objects.
	//----------------------------------------------------------------------------------------------
	class VISTAS_API MeshForge
	{
	public:
		//----------------------------------------------------------------------------------------------
		/** \enum MeshFlags
			\brief Mesh creation flags.
			This enumeration is used to determine the vertex composition of geometry 
			created through the MeshForge. Note that so far, only MF_ENABLE_ALL is 
			supported.
		 */
		//----------------------------------------------------------------------------------------------
		enum MeshFlags
		{
			MF_NONE			= 0x0000,
			MF_COLOUR		= 0x0001,
			MF_NORMALS		= 0x0002,
			MF_BINORMALS	= 0x0004,
			MF_TANGENTS		= 0x0008,
			MF_TEXCOORDS	= 0x0010,
			MF_ENABLE_ALL	= 0xFFFF
		};

	public:
		//----------------------------------------------------------------------------------------------
		/** \brief Creates an IndexBuffer object from an IndexData object.
		  	\param p_pIndexData IndexData object to convert.
		  	\return An IndexBufferPtr to the newly created IndexBuffer.
		 */
		//----------------------------------------------------------------------------------------------
		static IndexBufferPtr CreateIndexBuffer(IndexDataPtr p_pIndexData);

		//----------------------------------------------------------------------------------------------
		/** \brief Creates a VertexBuffer object from a VertexData object. Note that since
				   VertexData objects, contrary to VertexBuffer objects, can store multiple channels,
				   an index must be specified as to which of the VertexData channels or buffers must
				   be converted to a VertexBuffer.
			\param p_pVertexData VertexData object to convert.
			\param p_uiBufferIndex Index of buffer to convert; default value is set to zero.
			\return A VertexBufferPtr to the newly created VertexBuffer.
		 */
		//----------------------------------------------------------------------------------------------
		static VertexBufferPtr CreateVertexBuffer(VertexDataPtr p_pVertexData, uint p_uiBufferIndex = 0);

		//----------------------------------------------------------------------------------------------
		/** Creates a VertexBufferGroup object from a VertexData object. 
			\param p_pVertexData VertexData object to convert.
			\return A VertexBufferGroupPtr to the newly created VertexBufferGroup.
		 */
		//----------------------------------------------------------------------------------------------
		static VertexBufferGroupPtr CreateVertexBufferGroup(VertexDataPtr p_pVertexData);

		//----------------------------------------------------------------------------------------------
		/** Creates a Geometry object from an AbstractMesh object. 
			\param p_pAbstractMesh AbstractMesh object to create Geometry object from.
			\return A GeometryPtr to the newly created Geometry object.
		 */
		//----------------------------------------------------------------------------------------------
		static GeometryPtr CreateMesh(AbstractMeshPtr p_pAbstractMesh);

		//----------------------------------------------------------------------------------------------
		/** Creates a new AbstractMesh object according to the supplied mesh flags. 
			\param p_dwMeshFlags Flags which determine how the AbstractMesh object is constructed.
			\return An AbstractMeshPtr to a newly created AbstractMesh object.
		 */
		//----------------------------------------------------------------------------------------------
		static AbstractMeshPtr CreateAbstractMesh(dword p_dwMeshFlags);

		//----------------------------------------------------------------------------------------------
		/** Loads a resource and creates a single Geometry object from it. Note that so far, only
			Wavefront (.obj) model files are supported. If the resource contains multiple objects,
			they are merged into a single Geometry object anyway. For multiple objects, see the
			CreateMeshListFromFile method.
			\param p_strFilename Name of resource.
			\param p_fScale Scale multiplier for geometry objects.
			\param p_dwMeshFlags Mesh creation flags; only MF_ENABLE_ALL is supported so far.
			\return A GeometryPtr to the newly created object.
		 */
		//----------------------------------------------------------------------------------------------
		static GeometryPtr CreateMeshFromFile(const Meson::Common::Text::String p_strFilename, float p_fScale = 1.0f, dword p_dwMeshFlags = MF_ENABLE_ALL);

		//----------------------------------------------------------------------------------------------
		/** Loads a resource and creates a list of Geometry objects from it. Note that so far, only
			Wavefront (.obj) model files are supported. 
			\param p_strFilename Name of resource.
			\param p_fScale Scale multiplier for geometry objects.
			\param p_dwMeshFlags Mesh creation flags; only MF_ENABLE_ALL is supported so far.
			\return A GeometryListPtr to a list of Geometry objects.
		 */
		//----------------------------------------------------------------------------------------------
		static GeometryListPtr CreateMeshListFromFile(const Meson::Common::Text::String p_strFilename, float p_fScale = 1.0f, dword p_dwMeshFlags = MF_ENABLE_ALL);

		static GeometryPtr CreatePlaneMesh(const Meson::Common::Maths::Vector3f& p_vec3Normal, float p_fDistance, float p_fLength, float p_fBreadth, float p_fUMultiplier = 1.0f, float p_fVMultiplier = 1.0f, bool p_bInvertNormals = false, dword p_dwMeshFlags = MF_ENABLE_ALL);
		static GeometryPtr CreateBoxMesh(float p_fWidth, float p_fHeight, float p_fDepth, bool p_bInvertNormals = false, dword p_dwMeshFlags = MF_ENABLE_ALL);
		static GeometryPtr CreateSphereMesh(float p_fRadius, uint p_uiSegments, bool p_bInvertNormals = false, dword p_dwMeshFlags = MF_ENABLE_ALL);
		static GeometryPtr CreateCylinderMesh(float p_fRadius, float p_fHeight, uint p_uiSegments, bool p_bCreateCaps, bool p_bInvertNormals = false, dword p_dwMeshFlags = MF_ENABLE_ALL);
	};
	//----------------------------------------------------------------------------------------------

Meson_Vistas_END
