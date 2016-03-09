 #version 150 core
	
	uniform vec3 s;
	uniform vec3 t;
	uniform vec3 r;
	uniform vec3 ucolor;
    in vec2 position;
	in vec3 color;
	out vec3 Color;
	
    void main()
    {
												   
		Color = ucolor;										   
		 mat4 RotZMatrix = mat4(cos(r.z), -sin(r.z), 0.0, 0.0, 
					             sin(r.z), cos(r.z), 0.0, 0.0, 
					             0.0,      0.0,      1.0, 0.0, 
					             0.0,      0.0,      0.0, 1.0 );
															
		 mat4 RotYMatrix = mat4(cos(r.y), 0.0, sin(r.y), 0.0, 
					             0.0, 1.0, 0.0, 0.0, 
					             -sin(r.y), 0.0, cos(r.y), 0.0, 
					             0.0,       0.0, 0.0,      1.0 );
																
		 mat4 RotXMatrix = mat4(1.0,  0.0,       0.0,      0.0, 
					             0.0, cos(r.x), -sin(r.x), 0.0, 
					             0.0, sin(r.x),  cos(r.x), 0.0, 
					             0.0, 0.0,       0.0,      1.0 );
																
		 mat4 ScaleMatrix = mat4(s.x, 0.0, 0.0, 0.0, 
					             0.0, s.y, 0.0, 0.0, 
					             0.0, 0.0, s.z, 0.0, 
					             0.0, 0.0, 0.0, 1.0 );
																
		 mat4 TransMatrix = mat4(1.0, 0.0, 0.0, t.x, 
					             0.0, 1.0, 0.0, t.y, 
					             0.0, 0.0, 1.0, t.z, 
					             0.0, 0.0, 0.0, 1.0 );
																
	    gl_Position = ScaleMatrix  * vec4(position, 0.0, 1.0) * TransMatrix * RotZMatrix * RotXMatrix * RotYMatrix;
    }