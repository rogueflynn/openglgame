#version 150 core
	uniform vec3 triangleColor;
	in vec3 Color;
	in vec2 Texcoord;
	out vec4 outColor;
	void main()
	{
		outColor = vec4(Color, 1.0);
	}