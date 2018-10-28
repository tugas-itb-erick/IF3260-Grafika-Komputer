#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 inc_rain;
uniform int is_rain;

void main()
{
	vec3 real_position = position;
	if (is_rain == 1) {
		real_position = vec3(position.x + inc_rain.x, position.y + inc_rain.y, position.z + inc_rain.z);
	} else {
		real_position = position;
	}

    gl_Position = projection * view *  model * vec4(position, 1.0f);
    FragPos = vec3(model * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal;
    TexCoords = texCoords;
}
