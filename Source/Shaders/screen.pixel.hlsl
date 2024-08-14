struct PixelIN
{
    float2 TexCoord : TexCoord;
};

Texture2D screenTexture : register(t0);
SamplerState LinearSampler : register(s0);

float3 Palette(float t)
{
    float3 a = float3(0.5, 0.5, 0.5);
    float3 b = float3(0.5, 0.5, 0.5);
    float3 c = float3(1.0, 1.0, 1.0);
    float3 d = float3(0.263, 0.416, 0.557);
 
    return a + b * cos(6.28318 * (c * t + d));
}

float4 main(PixelIN IN) : SV_TARGET
{
    float4 color = screenTexture.Sample(LinearSampler, IN.TexCoord);
    
    return float4(color.rgb, 1.0f);
}