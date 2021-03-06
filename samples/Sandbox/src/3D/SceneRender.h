/*
    3D Scene Graphics class
*/

#pragma once

#include <tsgraphics/Graphics.h>
#include <tsgraphics/Buffer.h>

#include "RenderableList.h"
#include "ShaderConstants.h"
#include "Material.h"
#include "MaterialManager.h"

#include "ShadowPass.h"

namespace ts
{
	enum LightSource
	{
		LIGHT0,
		LIGHT1,
		LIGHT2,
		LIGHT3
	};

	class SceneRender
	{
	private:

		GraphicsSystem* m_gfx;
		MaterialManager m_materialManager;

		RenderTargets<> m_targets;

		Buffer m_perMesh;
		Buffer m_perScene;

		ShadowPass m_shadowPass;

		RenderableList m_visibleRenderables;

		/*
			Properties
		*/
		Vector m_ambientColour;
		Matrix m_viewMatrix, m_projMatrix;
		RGBA m_directLightColour;
		Vector m_directLightDir;
		DynamicLight m_dynamicLights[MAX_LIGHTS];
		Matrix m_lightView, m_lightProjection;

	public:

		SceneRender() {}
		SceneRender(const SceneRender&) = delete;

		SceneRender(
			GraphicsSystem* graphics
		);

		/*
			Set global constants
		*/

		void setCameraView(const Matrix& view) { m_viewMatrix = view; }
		void setCameraProjection(const Matrix& proj) { m_projMatrix = proj; }
		void setCamera(const Matrix& view, const Matrix& proj) { setCameraView(view); setCameraProjection(proj); }
		void setAmbientColour(const Vector& ambient) { m_ambientColour = ambient; }

		void setDirectionalLightColour(RGBA colour) { m_directLightColour = colour; }
		void setDirectionalLightDir(const Vector& dir) { m_directLightDir = dir; }

		void enableDynamicLight(LightSource light) { m_dynamicLights[light].enabled = true; }
		void disableDynamicLight(LightSource light) { m_dynamicLights[light].enabled = false; }

		void setLightColour(LightSource light, RGBA colour) { m_dynamicLights[light].colour = Vector(colour); }
		void setLightPosition(LightSource light, const Vector& pos) { m_dynamicLights[light].pos = pos; }

		void setLightAttenuation(LightSource light, float quadratic, float linear, float constant)
		{
			m_dynamicLights[light].attConstant = constant;
			m_dynamicLights[light].attLinear = linear;
			m_dynamicLights[light].attQuadratic = quadratic;
		}

		//Create a renderable
		Renderable createRenderable(const Mesh& mesh, const PhongMaterial&);

		//Draw a renderable
		void draw(const Renderable& item, const Matrix& transform)
		{
			m_visibleRenderables.submit(transform, item);
		}

		void update();

	private:

		void executeColourPass(TargetHandle target, const RenderableList& renderables);
		void executeShadowPass(const RenderableList& renderables);
	};
}
