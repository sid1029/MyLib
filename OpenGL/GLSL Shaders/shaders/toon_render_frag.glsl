#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect toontex;

void main(void)
{
    gl_FragColor = texture2DRect(toontex, gl_TexCoord[0].st);
}
