//#version 130
uniform sampler2D colormap;

varying vec2  TexCoord;
varying vec4 color;
float rings(vec2 p)
{
  return sin(length(p)*16.0);
}
 
void main(void) {
   
    TexCoord = gl_MultiTexCoord0.st;
    
    //gl_TexCoord[0] = gl_Vertex;

    float depth = 1.0;
    vec4 pixelDepth = texture2D(colormap,TexCoord);
    gl_FrontColor = texture2D(colormap,TexCoord);
    //gl_Vertex = vec4(gl_Vertex.x, gl_Vertex.y, gl_Vertex.z,1.0);
    //gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.xy, pixelDepth.r * 200.0, 1.0);
    gl_Position = gl_ModelViewProjectionMatrix * vec4(gl_Vertex.x * (pixelDepth.r * depth + 1.0), gl_Vertex.y * (pixelDepth.r * depth + 1.0), gl_Vertex.z * (pixelDepth.r * depth + 1.0), 1.0) ;
    
    
} 