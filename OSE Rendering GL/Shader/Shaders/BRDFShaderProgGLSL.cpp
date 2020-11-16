#include "pch.h"
#include "BRDFShaderProgGLSL.h"

namespace ose::shader
{
	BRDFShaderProgGLSL::BRDFShaderProgGLSL() : ShaderProgGLSL(nullptr)
	{

	}

	BRDFShaderProgGLSL::~BRDFShaderProgGLSL()
	{

	}

	// Build an OpenGL shader object from a shader graph
	void BRDFShaderProgGLSL::CreateShaderProg()
	{
		if(shader_prog_)
			return;

		// Builds the deferred rendering shader
		GLuint vert = glCreateShader(GL_VERTEX_SHADER);
		char const * vert_source =
			"#version 330\n"
			"layout(location = 0) in vec3 position;\n"
			"layout(location = 1) in vec3 normal;\n"
			"layout(location = 2) in vec2 uv;\n"
			"layout(location = 3) in vec3 tangent;\n"
			"out vec2 vertexUV;\n"
			"out vec3 vertexNormal;\n"
			"out vec3 vertexWorldPos;\n"
			"out mat3 vertexTBN;\n"
			"uniform mat4 viewProjMatrix;\n"
			"uniform mat4 worldTransform;\n"
			"uniform sampler2D texSampler;\n"
			"void main() {\n"
			"	vertexUV = uv;\n"
			"	vertexWorldPos = vec3(worldTransform * vec4(position, 1.0));\n"

				// TODO - Confirm that this is the correct transform (possibly should be model transform)
			"	vertexNormal = mat3(transpose(inverse(worldTransform))) * normal;\n"

			"	vec3 T = normalize(vec3(worldTransform * vec4(tangent, 0.0)));\n"
			"	vec3 N = normalize(vec3(worldTransform * vec4(normal, 0.0)));\n"
				// Orthogonalise T wrt. N then calculate the bi-tangent vector B and the TBN matrix
			"	T = normalize(T - dot(T, N) * N);\n"
			"	vec3 B = cross(N, T);\n"
			"	vertexTBN = mat3(T, B, N);\n"

			"	gl_Position = (viewProjMatrix * worldTransform) * vec4(position, 1.0);\n"
			"}\n"
			;
			/*"#version 330\n"
			"layout (location = 0) in vec2 position;\n"
			"layout (location = 1) in vec2 texCoords;\n"
			"out vec2 TexCoords;\n"
			"void main() {\n"
			"	gl_Position = vec4(position.x, position.y, 0.0, 1.0);\n"
			"	TexCoords = texCoords;\n"
			"}\n"
			;*/
		glShaderSource(vert, 1, &vert_source, NULL);
		glCompileShader(vert);

		GLint isCompiled = 0;
		glGetShaderiv(vert, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vert, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(vert, maxLength, &maxLength, &errorLog[0]);
			std::string msg(errorLog.begin(), errorLog.end());
			LOG_ERROR(msg);

			// Provide the infolog in whatever manor you deem best.
			// Exit with failure.
			glDeleteShader(vert); // Don't leak the shader.
			return;
		}

		GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
		char const * frag_source =
			"#version 330\n"
			"in vec2 vertexUV;\n"
			"in vec3 vertexNormal;\n"
			"in vec3 vertexWorldPos;\n"
			"in mat3 vertexTBN;\n"
			"out vec4 fragColor;\n"

			"uniform sampler2D albedoMap;\n"
			"uniform sampler2D normalMap;\n"
			"uniform sampler2D metallicMap;\n"
			"uniform sampler2D roughnessMap;\n"
			"uniform sampler2D aoMap;\n"

			"struct PointLight {\n"
			"	vec3 position;\n"
			"	vec3 color;\n"
			"};\n"
			"uniform PointLight pointLights[16];\n"
			"uniform int numPointLights;\n"

			"struct DirLight {\n"
			"	vec3 direction;\n"
			"	vec3 color;\n"
			"};\n"
			"uniform DirLight dirLights[16];\n"
			"uniform int numDirLights;\n"

			"uniform vec3 cameraPos;\n"

			"float pi = 3.14159265359;\n"

			// Approximates the ratio of reflection (specular) to refraction (diffuse)
			"vec3 fresnelSchlick(float cosTheta, vec3 F0) {\n"
			"	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);\n"
			"}\n"

			"float distributionGGX(vec3 N, vec3 H, float roughness) {\n"
			"	float a = roughness * roughness;\n"
			"	float a2 = a * a;\n"
			"	float NdotH = max(dot(N, H), 0.0);\n"
			"	float NdotH2 = NdotH * NdotH;\n"
			"	float num = a2;\n"
			"	float denom = (NdotH2 * (a2 - 1.0) + 1.0);\n"
			"	denom = pi * denom * denom;\n"
			"	return num / denom;\n"
			"}\n"

			"float geometrySchlickGGX(float NdotV, float roughness) {\n"
			"	float r = roughness + 1.0;\n"
			"	float k = (r * r) / 8.0;\n"
			"	float num = NdotV;\n"
			"	float denom = NdotV * (1.0 - k) + k;\n"
			"	return num / denom;\n"
			"}\n"

			"float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {\n"
			"	float NdotV = max(dot(N, V), 0.0);\n"
			"	float NdotL = max(dot(N, L), 0.0);\n"
			"	float ggx2 = geometrySchlickGGX(NdotV, roughness);\n"
			"	float ggx1 = geometrySchlickGGX(NdotL, roughness);\n"
			"	return ggx1 * ggx2;\n"
			"}\n"

			"vec3 getNormalFromNormalMap() {\n"
			"	vec3 normal = texture(normalMap, vertexUV).xyz;\n"
			"	normal = normal * 2.0 - 1.0;\n"
			"	normal = vertexTBN * normal;\n"
			"	return normal;\n"
			"}\n"

			"void main() {\n"
				// Get the fragment properties from the map textures
			"	vec3 albedo = texture(albedoMap, vertexUV).rgb;\n"
			"	vec3 normal = getNormalFromNormalMap();\n"
			"	float metallic = texture(metallicMap, vertexUV).r;\n"
			"	float roughness = texture(roughnessMap, vertexUV).r;\n"
			"	float ao = texture(aoMap, vertexUV).r;\n"
				// Normalise the vertex direction and normal
			"	vec3 V = normalize(cameraPos - vertexWorldPos);\n"
			"	vec3 N = normalize(normal);\n"
				// For non-metallic surfaces, F0 is always 0.04
			"	vec3 F0 = vec3(0.04);\n"
			"	F0 = mix(F0, albedo, metallic);\n"
				// Calculate the illumination from each point light
			"	vec3 Lo = vec3(0.0);\n"
			"	for(int i = 0; i < numPointLights && i < 16; ++i) {\n"
					// Calculate the radiance at the fragment due to the light source
			"		vec3 L = normalize(pointLights[i].position - vertexWorldPos);\n"
			"		vec3 H = normalize(V + L);\n"
			"		float distance = length(pointLights[i].position - vertexWorldPos);\n"
			"		float attenuation = 1.0 / (distance * distance);\n"
			"		vec3 radiance = pointLights[i].color.rgb * attenuation;\n"
					// Calculate the Cook-Torrance BRDF
			"		float NDF = distributionGGX(N, H, roughness);\n"
			"		float G = geometrySmith(N, V, L, roughness);\n"
			"		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);\n"
					// Calculate the ratio of specular and diffuse lighting
			"		vec3 kS = F;\n"
			"		vec3 kD = vec3(1.0) - kS;\n"
			"		kD *= 1.0 - metallic;\n"
					// Calculate the specular highlight
			"		vec3 num = NDF * G * F;\n"
			"		float denom = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);\n"
			"		vec3 specular = num / max(denom, 0.001);\n"
					// Add to the total radiance at the frament
			"		float NdotL = max(dot(N, L), 0.0);\n"
			"		Lo += (kD * albedo / pi + specular) * radiance * NdotL;\n"
			"	}\n"
				// Calculate the illumination from each direction light
			"	for(int i = 0; i < numDirLights && i < 16; ++i) {\n"
					// Calculate the radiance at the fragment due to the light source
			"		vec3 L = normalize(dirLights[i].direction);\n"
			"		vec3 H = normalize(V + L);\n"
			"		vec3 radiance = dirLights[i].color.rgb;\n"
					// Calculate the Cook-Torrance BRDF
			"		float NDF = distributionGGX(N, H, roughness);\n"
			"		float G = geometrySmith(N, V, L, roughness);\n"
			"		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);\n"
					// Calculate the ratio of specular and diffuse lighting
			"		vec3 kS = F;\n"
			"		vec3 kD = vec3(1.0) - kS;\n"
			"		kD *= 1.0 - metallic;\n"
					// Calculate the specular highlight
			"		vec3 num = NDF * G * F;\n"
			"		float denom = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);\n"
			"		vec3 specular = num / max(denom, 0.001);\n"
					// Add to the total radiance at the frament
			"		float NdotL = max(dot(N, L), 0.0);\n"
			"		Lo += (kD * albedo / pi + specular) * radiance * NdotL;\n"
			"	}\n"
				// Add ambient light to the fragment
			"	vec3 ambient = vec3(0.03) * albedo * ao;\n"
			"	vec3 color = ambient + Lo;\n"
				// Apply gamma correction
			"	color /= (color + vec3(1.0));\n"
			"	color = pow(color, vec3(1.0 / 2.2));\n"
				// Set the output color
			"	fragColor = vec4(color, 1.0);\n"
		//	"   fragColor = vec4(1, 1, 1, 1);\n"
		//	"	fragColor = vec4(N, 1);\n"
			"}\n"
			;
		/*	"#version 330\n"
			"in vec2 TexCoords;\n"
			"out vec4 outColor;\n"
			"uniform sampler2D gPos;\n"
			"uniform sampler2D gNormal;\n"
			"uniform sampler2D gColourSpec;\n"
			"struct PointLight {\n"
			"	vec4 position;\n"
			"	vec4 color;\n"
			"	float intensity;\n"
			"};\n"
			"uniform PointLight pointLights[16];\n"
			"uniform int numPointLights;\n"
			"void main() {\n"
			"	vec3 fragPos	   = texture(gPos, TexCoords).rgb;\n"
			"	vec3 surfaceNormal = texture(gNormal, TexCoords).rgb;\n"
			"	vec4 texColor	   = vec4(texture(gColourSpec, TexCoords).rgb, 1.0);\n"
			"	float shininess	   = texture(gColourSpec, TexCoords).a;\n"
			"	vec4 color = vec4(0, 0, 0, 1);\n"
			"	for(int i = 0; i < 16 && i < numPointLights; ++i) {\n"
			"		color += pointLights[i].color;\n"
			"	}\n"
			"	color *= texColor;\n"
			//"	float gamma = 2.2;\n"
			//"	colour.rgb = pow(colour.rgb, vec3(1.0/gamma));\n"
			//"	colour = vec4(1, 1, 0, 1);\n"
			"	outColor = color;\n"
			"}\n"
			;
			*/

		glShaderSource(frag, 1, &frag_source, NULL);
		glCompileShader(frag);

		isCompiled = 0;
		glGetShaderiv(frag, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(frag, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(frag, maxLength, &maxLength, &errorLog[0]);
			std::string msg(errorLog.begin(), errorLog.end());
			LOG_ERROR(msg);

			// Exit with failure.
			glDeleteShader(frag); // Don't leak the shader.
			return;
		}

		GLuint prog = glCreateProgram();
		glAttachShader(prog, vert);
		glAttachShader(prog, frag);
		glLinkProgram(prog);

		GLint isLinked = 0;
		glGetProgramiv(prog, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetProgramInfoLog(prog, maxLength, &maxLength, &errorLog[0]);
			std::string msg(errorLog.begin(), errorLog.end());
			LOG_ERROR(msg);

			// We don't need the program anymore.
			glDeleteProgram(prog);

			// Don't leak shaders either.
			glDeleteShader(vert);
			glDeleteShader(frag);
			return;
		}

		glDetachShader(prog, vert);
		glDetachShader(prog, frag);
		glDeleteShader(vert);
		glDeleteShader(frag);

		glUseProgram(prog);
		glUniform1i(glGetUniformLocation(prog, "albedoMap"), 0);
		glUniform1i(glGetUniformLocation(prog, "normalMap"), 1);
		glUniform1i(glGetUniformLocation(prog, "metallicMap"), 2);
		glUniform1i(glGetUniformLocation(prog, "roughnessMap"), 3);
		glUniform1i(glGetUniformLocation(prog, "aoMap"), 4);

		shader_prog_ = prog;
	}

	// Destroy the OpenGL shader object
	void BRDFShaderProgGLSL::DestroyShaderProg()
	{
		if(shader_prog_)
			glDeleteProgram(shader_prog_);
	}
}
