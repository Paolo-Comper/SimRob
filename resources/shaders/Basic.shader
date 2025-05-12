#shader vertex
#version 330 core

layout(location = 0) in vec4 position;     
                                           
void main()                                
{                                          
   gl_Position = position;                 
}





#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 u_color;

void main() {
    float y = gl_FragCoord.y / 600.0; // Normalizza tra 0 e 1 (cambia 600 con la risoluzione verticale)
    vec3 col = mix(vec3(1.0, 0.0, 0.0), u_color, y);
    color = vec4(col, 1.0);
}