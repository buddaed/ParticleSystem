uniform sampler2D tex;

in vec4 v_color;
in vec2 v_uv1;

out vec4 PixelColor;


void main()
{
    PixelColor = texture(tex, v_uv1);
    PixelColor *= v_color;
}