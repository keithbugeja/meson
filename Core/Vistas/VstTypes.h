#pragma once

#include "Namespaces.h"

#include "VstNamespaces.h"
#include "VstDefinitions.h"
#include "VstVistasObject.h"

#ifdef VISTAS_EXPORTS
	#define VISTAS_API __declspec(dllexport)
#else
	#define VISTAS_API __declspec(dllimport)
#endif

Meson_Common_Config_BEGIN
	class Configuration;
Meson_Common_Config_END

Meson_Vistas_BEGIN
	class ColourARGB;
	class ColourRGBA;
	typedef ColourRGBA Colour;

	class IRenderState;
	class AlphaRenderState;
	class BiasRenderState;
	class CullRenderState;
	class DepthRenderState;
	class LightRenderState;
	class MaterialRenderState;
	class SamplerRenderState;
	class StencilRenderState;
	class TargetRenderState;

	class IRenderDevice;
	class Renderable;
	class RenderContext;
	class RenderTarget;
	class WindowRenderTarget;
	class TextureRenderTarget;
	class WindowMessageManager;
	
	class Resource;
	class HardwareResource;
	class HardwareResourceManager;
	class IHardwareResourceFactory;
	
	class IndexBuffer;
	class VertexBuffer;
	class TextureBuffer;
	class VertexChannel;
	class VertexBufferGroup;
	class VertexDeclaration;

	class Light;
	class Camera;
	class Viewport;
	class SamplerUnit;
	class SamplerState;
	class TextureState;

	class IShaderProgram;
	class IShaderProgramFactory;
	class ShaderProgramManager;

	class Geometry;
	class Attribute;
	class IndexData;
	class VertexData;

	class IEffect;

	class Material;
	class TechniqueGroup;
	class Technique;
	class IPass;
	class AmbientPass;
	class IlluminationPass;
	class EffectPass;

	//class ISceneManager;
	class Scenegraph;
	class ISceneNode;
	class ISceneCompositeNode;
	class SceneGroupNode;
	class SceneLightNode;
	class SceneGeometryNode;
	class Transformation;

	class IBoundingVolume;
	class BoundingSphere;
	class BoundingAxisAlignedBox;

	class ICuller;
	class IRenderer;

	class Texture;
	class TextureManager;

Meson_Vistas_END

// Include basic types
#include "Plane.h"
#include "Quaternion.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Pointer.h"
#include "String8.h"
#include "HashMap.h"
#include "ArrayList.h"

Meson_Vistas_BEGIN
	typedef Meson::Common::Collections::TArrayList<Meson::Common::Maths::Vector2f> Vector2ArrayList;
	typedef Meson::Common::Collections::TEnumerator<Meson::Common::Maths::Vector2f>& Vector2Enumerator;

	typedef Meson::Common::Collections::TArrayList<Meson::Common::Maths::Vector3f> Vector3ArrayList;
	typedef Meson::Common::Collections::TEnumerator<Meson::Common::Maths::Vector3f>& Vector3Enumerator;

	typedef Meson::Common::Collections::TArrayList<Meson::Common::Maths::Vector4f> Vector4ArrayList;
	typedef Meson::Common::Collections::TEnumerator<Meson::Common::Maths::Vector4f>& Vector4Enumerator;

	typedef Meson::Common::Collections::TArrayList<Meson::Common::Maths::Matrix2f> Matrix2ArrayList;
	typedef Meson::Common::Collections::TEnumerator<Meson::Common::Maths::Matrix2f>& Matrix2Enumerator;

	typedef Meson::Common::Collections::TArrayList<Meson::Common::Maths::Matrix3f> Matrix3ArrayList;
	typedef Meson::Common::Collections::TEnumerator<Meson::Common::Maths::Matrix3f>& Matrix3Enumerator;

	typedef Meson::Common::Collections::TArrayList<Meson::Common::Maths::Matrix4f> Matrix4ArrayList;
	typedef Meson::Common::Collections::TEnumerator<Meson::Common::Maths::Matrix4f>& Matrix4Enumerator;
Meson_Vistas_END
