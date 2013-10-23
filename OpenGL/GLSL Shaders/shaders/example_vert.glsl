#extension GL_ARB_texture_rectangle : enable

varying vec3 normal;

void main(void)
{
    normal = gl_NormalMatrix * gl_Normal;

    //ModelView and Camera projections
    gl_Position = ftransform();
}
