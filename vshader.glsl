attribute highp vec4 a_position;
attribute mediump vec2 a_texcoord;
uniform highp mat4 mvp_matrix;
varying mediump vec2 v_texcoord;

void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * a_position;

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = a_texcoord;
}
