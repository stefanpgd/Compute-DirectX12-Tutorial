RWTexture2D<float4> buffer : register(u0);

struct Particle
{
    float2 position;
    float color;
    float2 velocity;
};
RWStructuredBuffer<Particle> particles : register(u1);

float3 Palette(float t)
{
    float3 a = float3(0.5, 0.5, 0.5);
    float3 b = float3(0.5, 0.5, 0.5);
    float3 c = float3(1.0, 1.0, 1.0);
    float3 d = float3(0.263, 0.416, 0.557);
 
    return a + b * cos(6.28318 * (c * t + d));
}

[numthreads(64, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    int width;
    int height;
    buffer.GetDimensions(width, height);
    
    if(dispatchThreadID.x > 500)
    {
        return;
    }
    
    float2 position = particles[dispatchThreadID.x].position;
    float2 v = particles[dispatchThreadID.x].velocity;
    
    position += v * 0.1f;
    
    if(position.x > width)
    {
        position.x = 0.1f;
    }
    
    if(position.x < 0.0f)
    {
        position.x = width;
    }
    
    if(position.y > height)
    {
        position.y = 0.1f;
    }
    
    if(position.y < 0.0f)
    {
        position.y = height - 1.0f;
    }
    
    particles[dispatchThreadID.x].position = position;
    Particle p = particles[dispatchThreadID.x];
    
    uint2 p_2d = uint2(p.position.xy);
    
    float3 outputcolor = Palette(p.color);
    buffer[p_2d] = float4(outputcolor, 1.0f);
}