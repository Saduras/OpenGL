#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Model;

out vec2 v_TexCoord;
out vec3 v_Normal;
out vec3 v_FragPos;

void main()
{
	gl_Position = u_Proj * u_View * u_Model * position;
	v_TexCoord = texCoord;
	v_Normal = normal;
	v_FragPos = vec3(u_Model * position);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;
uniform vec3 u_LightPos;

in vec2 v_TexCoord;
in vec3 v_Normal;
in vec3 v_FragPos;

void main()
{
	vec3 norm = normalize(v_Normal);
	vec3 lightDir = normalize(u_LightPos - v_FragPos);
	float diff = max(dot(norm, lightDir), 0.0);

	vec4 texColor = texture(u_Texture, v_TexCoord);
	color = vec4(texColor.rgb * (0.1 + diff), 1.0f);
};