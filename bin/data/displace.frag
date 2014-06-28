uniform sampler2D colormap;
uniform float peak;
varying vec2  TexCoord;
varying vec4 color;

void main(void) {
    gl_FragColor =  texture2D(colormap,TexCoord.xy);
}