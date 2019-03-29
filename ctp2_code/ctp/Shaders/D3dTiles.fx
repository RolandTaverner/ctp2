//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)
{
  matrix World;
  matrix View;
  matrix Projection;
}

Texture2D tex2D;
SamplerState linearSampler
{
  Filter = MIN_MAG_MIP_LINEAR;
  AddressU = Wrap;
  AddressV = Wrap;
};

struct VS_INPUT
{
  float4 position : POSITION;
  float2 tex : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
  float4 position : SV_POSITION;
  float2 tex : TEXCOORD0;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
  VS_OUTPUT output;

  input.position.w = 1.0f;
  output.position = mul(input.position, World);
  output.position = mul(output.position, View);
  output.position = mul(output.position, Projection);
  output.tex = input.tex;
  return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_Target
{
  float4 textureColor;

  // Sample the pixel color from the texture using the sampler at this texture coordinate location.
  textureColor = tex2D.Sample(linearSampler, input.tex);
  return textureColor;
}
