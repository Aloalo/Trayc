#version 330 core

in vec3 color;
in float center_z;

uniform vec3 lightDir;
uniform mat4 P;

out vec3 outColor;
 
void main()
{
    //Calculate normal from texture coordinates
    vec3 N;
    N.xy = gl_PointCoord  * vec2(2.0, -2.0) + vec2(-1.0, 1.0);
    float mag = dot(N.xy, N.xy);

    if(mag > 1.0) //Kill pixels outside circle
        discard;

    N.z = sqrt(1.0 - mag);
    
    vec2 clipZW = center_z * P[2].zw + P[3].zw;
 
    float depth = 0.5 + 0.5 * clipZW.x / clipZW.y;
 
    //This is a workaround necessary for Mac
    //Otherwise the modified fragment wont clip properly
    if (depth <= 0.0)
      discard;
 
    if (depth >= 1.0)
      discard;
 
    gl_FragDepth = depth;
    
    //Calculate lighting
    float diffuse = max(0.0, dot(lightDir, N));

    outColor = diffuse * color;
}
