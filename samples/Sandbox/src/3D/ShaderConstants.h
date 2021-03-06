/*
    Constant buffer parameters for the rendering pipeline
*/

#pragma once

#include <tscore/maths.h>

namespace ts
{
	enum ImageBindings
	{
		BIND_DIFFUSE_MAP      = 0,
		BIND_NORMAL_MAP       = 1,
		BIND_SPECULAR_MAP     = 2,
		BIND_DISPLACEMENT_MAP = 3,

		BIND_SHADOW_MAP       = 7,
	};

	enum BufferBindings
	{
		BIND_SCENE_CONSTANTS = 0,
		BIND_MESH_CONSTANTS  = 1,
		BIND_MAT_CONSTANTS   = 2
	};

	enum Ranges
	{
		MAX_LIGHTS = 4
	};

    struct DirectionalLight
    {
        Vector colour;
        Vector dir;
    };

    struct DynamicLight
    {
        Vector colour;
        Vector pos;

        int enabled = 0;

        //Attenuation factors
        float attConstant;
        float attLinear;
        float attQuadratic;
    };

    struct SceneConstants
    {
        Matrix view;
        Matrix projection;
		Matrix lightView;
        //Vector viewPos;
        Vector ambient;
        //Directional light
        DirectionalLight direct;
        DynamicLight dynamic[MAX_LIGHTS];
    };

    struct MeshConstants
    {
        Matrix world;
    };

	struct MaterialConstants
	{
		Vector diffuseColour;
		Vector ambientColour;
		Vector specularColour;
		Vector emissiveColour;
		float specularPower = 0.0f;
	};
}
