#extension GL_ARB_texture_rectangle : enable

varying vec3 normal;

void main(void)
{
    gl_FragColor = vec4(normalize(normal), 1.0);
}
