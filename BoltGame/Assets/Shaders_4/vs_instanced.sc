$input  a_position, a_texcoord0, i_data0, i_data1
$output v_texcoord0, v_color

#include <bgfx_shader.sh>

#if BGFX_SHADER_LANGUAGE_HLSL
#define floatBitsToUint(x) asuint(x)
float4 unpackUnorm4x8(uint v)
{
    float4 c = float4(
        (v      ) & 0xffu,
        (v >> 8 ) & 0xffu,
        (v >> 16) & 0xffu,
        (v >> 24) & 0xffu
    );
    return c * (1.0/255.0);
}
#endif

void main()
{
    // i_data0 = [rs0.x, rs0.y, rs1.x, rs1.y]
    // i_data1 = [tx, ty, colorPacked, unused]
    vec2 rs0 = i_data0.xy;
    vec2 rs1 = i_data0.zw;
    vec2 t   = i_data1.xy;

    uint bits = floatBitsToUint(i_data1.z);
    v_color   = unpackUnorm4x8(bits);

    vec2 p     = a_position.xy;
    vec2 world = vec2(dot(p, rs0), dot(p, rs1)) + t;

    gl_Position = mul(u_viewProj, vec4(world, 0.0, 1.0));
    v_texcoord0 = a_texcoord0;
}
