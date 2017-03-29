#version 440
 
// Captured render
uniform sampler2D tex;

// screen width
uniform float screen_width;
// screen height
uniform float screen_height;

uniform float elapsedTime; // seconds for noise effect
 
// Incoming texture coordinate
layout(location = 0) in vec2 tex_coord;

// Outgoing colour
layout(location = 0) out vec4 colour;

void main()
{
	vec4 color;

	float dxtex = screen_width /*image width*/;
	float dytex = screen_height /*image height*/;
    
	vec2 uv = tex_coord.xy / vec2(dxtex, dytex).xy;
    
    float distanceFromCenter = length( uv - vec2(0.5,0.5) );
    
    float vignetteAmount;
    
    float lum;
    
    vignetteAmount = 1.0 - distanceFromCenter;
    vignetteAmount = smoothstep(0.1, 1.0, vignetteAmount);
    
    color = texture( tex, uv);
    
    // luminance hack, responses to red channel most
    lum = dot(color.rgb, vec3( 0.85, 0.30, 0.10) );
    
    color.rgb = vec3(0.0, lum, 0.0);
    
    // scanlines
    color += 0.1*sin(uv.y*screen_height*2.0);
    
    // screen flicker
    color += 0.005 * sin(elapsedTime*16.0);
    
    // vignetting
    color *=  vignetteAmount*1.0;
    
	colour = color;
	colour.a = 1.0;
 
}