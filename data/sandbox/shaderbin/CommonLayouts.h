/*
	Common layouts
*/

#ifdef COMMON_LAYOUT_H
#else
#define COMMON_LAYOUT_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 	Pixel input formats
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct PixelInput_PosColour
{
	float4 pos : SV_POSITION;
	float4 vpos : VIEW_POSITION;
	float4 colour : COLOUR;
};

struct PixelInput_PosTex
{
	float4 pos : SV_POSITION;
	float4 vpos : VIEW_POSITION;
	float2 texcoord : TEXCOORD;
};

struct PixelInput_PosTexNorm
{
    float4 lpos : LIGHT_POSITION;
	float4 pos : SV_POSITION;
	float4 vpos : VIEW_POSITION;
	float3 vnorm : VIEW_NORMAL;
	float2 texcoord : TEXCOORD;
};

struct PixelInput_PosTexNormTangent
{
    float4 lpos : LIGHT_POSITION;
	float4 pos : SV_POSITION;
	float4 vpos : VIEW_POSITION;
	float3 vnorm : VIEW_NORMAL;
	float3 vtangent : VIEW_TANGENT;
	float3 vbitangent : VIEW_BITANGENT;
	float2 texcoord : TEXCOORD;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Vertex input formats
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct VertexInput_PosColour
{
	float4 pos : POSITION;
	float4 colour : COLOUR;
};

struct VertexInput_PosTex
{
	float4 pos : POSITION;
	float2 texcoord : TEXCOORD;
};

struct VertexInput_PosTexNorm
{
	float4 pos : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};

struct VertexInput_PosTexNormTangent
{
	float4 pos : POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
