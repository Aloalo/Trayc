#version 330 core

in vec3 color;

uniform vec3 lightDir;

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

    //Calculate lighting
    float diffuse = max(0.0, dot(lightDir, N));

    outColor = diffuse * color;
}
