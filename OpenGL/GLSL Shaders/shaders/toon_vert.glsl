#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect scenetex;
uniform sampler2DRect normtex;
uniform sampler2DRect depthtex;

void main(void)
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_TexCoord[1] = gl_MultiTexCoord1;
    gl_TexCoord[2] = gl_MultiTexCoord2;

    //ModelView and Camera projections
    gl_Position = ftransform();
}
