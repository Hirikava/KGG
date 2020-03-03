#version 440
#extension GL_EXT_geometry_shader4 : enable

layout(lines) in;
layout(points, max_vertices = 256) out;

uniform float stridePerPixelX;
uniform float stridePerPixelY;

void main()
{
	vec2 cord1 = gl_in[0].gl_Position.xy;
	vec2 cord2 = gl_in[1].gl_Position.xy;
	//Brezenhem algo
	float tg = (cord2.y - cord1.y) / (cord2.x - cord1.x);
	float error = 0;

	float strideX = stridePerPixelX;
	float strideY = stridePerPixelY;

	if(cord2.x < cord1.x)
		strideX *= -1.0;
	
	if(cord2.y < cord1.y)
		strideY *= -1.0;


	vec2 currentCord = cord1;
	if(abs(tg) <= 0.5)
	{
		for(int i = 0; i < (cord2.x - cord1.x)/strideX;i++)
		{
			if(error >= 0.5)
			{
				error -= 1 - abs(tg);
				gl_Position = vec4(currentCord.xy,0.0f,1.0f);
				EmitVertex();
				currentCord += vec2(strideX,stridePerPixelY);
			}
			else
			{
				error += abs(tg);
				gl_Position = vec4(currentCord.xy,0.0f,1.0f);
				EmitVertex();
				currentCord += vec2(strideX,0);
			}
		}
	}
	else
	{
		for(int i = 0; i < (cord2.y - cord1.y)/strideY;i++)
		{
			if(error >= 0.5)
			{
				error -= 1 - abs(tg);
				gl_Position = vec4(currentCord.xy,0.0f,1.0f);
				EmitVertex();
				currentCord += vec2(strideX,strideY);
			}
			else
			{
				error += abs(tg);
				gl_Position = vec4(currentCord.xy,0.0f,1.0f);
				EmitVertex();
				currentCord += vec2(0,strideY);
			}
		}
	}
	EndPrimitive();
}
