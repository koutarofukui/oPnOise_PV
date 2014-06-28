//#version 130

uniform sampler2DRect colormap;
varying vec2  TexCoord;
varying vec4 color;

uniform sampler2D colormap;
varying vec2  TexCoord;
varying vec4 color;

void main(void) {
    gl_FragColor =  texture2D(colormap,TexCoord.xy);
}