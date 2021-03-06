struct Vertex
{ 
    float2 Position : POSITION;
	float2 Size : TEXCOORD0;
	float2 Param : TEXCOORD1;	// Energy, ShapeInterpol
	int3 Shape : TEXCOORD2;		// ShapeIdx1 and 2, Materialindex
};

struct PSInput
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float2 Param : TEXCOORD1;
	int3 Shape : TEXCOORD2;
};

struct Material {
	float3 Color;
	float Refraction;
};

cbuffer Constants : register(b0)
{
	float c_time;
	float2 c_mapSize;
	float c_lightScale;
	float c_playerEnergy;
	float2 c_deltaCoord;
	float _1;
	Material c_materials[8];
};

struct Photon {
	float2 Position : POSITION;
	float2 Direction : TEXCOORD0;
	float3 Energy : TEXCOORD1;		// Colored photons
};

struct PhotonOut {
	float4 Position : SV_POSITION;
	float2 Direction : TEXCOORD0;
	float3 Energy : TEXCOORD1;		// remaining energy
	float3 EnergyOut : TEXCOORD2;	// part to be saved to the map
};



int HashInt( int _a )
{
	_a -= _a << 6;
	_a ^= _a >> 17;
	_a -= _a << 9;
	_a ^= _a << 4;
	_a -= _a << 3;
	_a ^= _a << 10;
	_a ^= _a >> 15;
	return _a;
}

// Standard random variable [0,1]
float RandomSample( float2 _val, int _seed )
{
	return HashInt(int(_val.x*211.364 + _val.y*450101.21 + _seed + c_time * 123.456)) * 4.656612873e-10;
}