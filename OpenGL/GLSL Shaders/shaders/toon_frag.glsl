#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect scenetex;
uniform sampler2DRect normtex;
uniform sampler2DRect depthtex;


void main(void)
{
    vec4 this_d = texture2DRect(depthtex, gl_TexCoord[0].st);
    vec4 up_d = texture2DRect(depthtex, gl_TexCoord[0].st + vec2(0, -1));
    vec4 down_d = texture2DRect(depthtex, gl_TexCoord[0].st + vec2(0, 1));
    vec4 left_d = texture2DRect(depthtex, gl_TexCoord[0].st + vec2(-1, 0));
    vec4 right_d = texture2DRect(depthtex, gl_TexCoord[0].st + vec2(1, 0));

    vec4 this_n = texture2DRect(normtex, gl_TexCoord[0].st);
    vec4 up_n = texture2DRect(normtex, gl_TexCoord[0].st + vec2(0, -1));
    vec4 down_n = texture2DRect(normtex, gl_TexCoord[0].st + vec2(0, 1));
    vec4 left_n = texture2DRect(normtex, gl_TexCoord[0].st + vec2(-1, 0));
    vec4 right_n = texture2DRect(normtex, gl_TexCoord[0].st + vec2(1, 0));

    float edge =  abs(right_d.x - left_d.x) + abs(down_d.x - up_d.x);

    float ud = acos(dot(up_n, down_n)/(length(up_n) * length(down_n)));
    float lr = acos(dot(left_n, right_n)/(length(left_n) * length(right_n)));

    float lt = acos(dot(left_n, this_n)/(length(left_n) * length(this_n)));
    float rt = acos(dot(right_n, this_n)/(length(right_n) * length(this_n)));
    float ut = acos(dot(up_n, this_n)/(length(up_n) * length(this_n)));
    float dt = acos(dot(down_n, this_n)/(length(down_n) * length(this_n)));
    
    float norm_thresh_ud = 0.19;
    float norm_thresh_lr = 0.0025;
    float dep_thresh = 0.00005;

    if ( (lt < norm_thresh_lr) || (rt < norm_thresh_lr) ) {
        if (edge < dep_thresh) {
            gl_FragColor = texture2DRect(scenetex, gl_TexCoord[0].st);
        }
        else {
            gl_FragColor = vec4(0,0,0,1);
        }
    }
    else {
        gl_FragColor = vec4(0,0,0,1);
    }

    if ( (ut > norm_thresh_ud) || (dt > norm_thresh_ud) ) {
        if (edge < dep_thresh) {
            gl_FragColor = vec4(0,0,0,1);
            
        }
        else {
            gl_FragColor = vec4(0,0,0,1);
        }
    }
    else {
        gl_FragColor = texture2DRect(scenetex, gl_TexCoord[0].st);
    }
}
