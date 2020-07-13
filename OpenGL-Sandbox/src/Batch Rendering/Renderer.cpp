
#define WIREFRAME_MODE 0
#include "Renderer.h"

#include <GLCore/Util/Shader.h>
#include <Glad/glad.h>

#include <vector>

using namespace GLCore;
using namespace GLCore::Utils;

struct Vertex
{
	glm::vec4 dimensions;
	glm::vec4 tint;
	float rotation;
};

struct RendererData
{
	Shader *shader;

	GLuint VAO = 0;
	GLuint VBO = 0;

	std::vector<Vertex> vertices;
	size_t vertexCount = 0;
};

static RendererData *renderer = nullptr;

void SetPipelineState()
{
	// Renderer State
#if WIREFRAME_MODE
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}
void CreateBufferStore()
{
	GLuint &VAO = renderer->VAO;
	GLuint &VBO = renderer->VBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Buffer Data
	glBufferData(GL_ARRAY_BUFFER, renderer->vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	//

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, dimensions));
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, rotation));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, tint));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void Renderer::Init(const RendererProps &props)
{
	renderer = new RendererData;
	renderer->vertices.resize(props.batchCapacity);

	SetPipelineState();
	CreateBufferStore();
	
	renderer->shader = Shader::FromGLSLTextFiles(
		"shaders/renderer-shader-vert.glsl",
		"shaders/renderer-shader-geom.glsl",
		"shaders/renderer-shader-frag.glsl"
	);
}

void Renderer::Shutdown()
{
	glDeleteBuffers(1, &renderer->VBO);
	glDeleteVertexArrays(1, &renderer->VAO);
	delete renderer->shader;

	delete renderer;
	renderer = nullptr;
}

void Renderer::BeginScene(const GLCore::Utils::OrthographicCamera &camera)
{
	Shader &shader = *renderer->shader;
	glUseProgram(shader.GetRendererID());

	// Probably worth storing the the uniform location
	GLint location = glGetUniformLocation(shader.GetRendererID(), "u_ViewProjection");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(camera.GetViewProjectionMatrix()));

	glBindVertexArray(renderer->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);

	SetPipelineState();
}

void Renderer::Draw(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &tint, float rotation)
{
	auto &vertices = renderer->vertices;
	auto &vertexCount = renderer->vertexCount;

	if (vertexCount >= vertices.size())
		Flush();

	Vertex &v = vertices[vertexCount];
	v.dimensions = { pos.x, pos.y, size.x, size.y };
	v.rotation = rotation;
	v.tint = tint;
	++vertexCount;
}

void Renderer::EndScene()
{
	// Flush if there are still some vertices left
	if (renderer->vertexCount > 0)
		Flush();
}

void Renderer::Flush()
{
	auto &vertices = renderer->vertices;
	auto &vertexCount = renderer->vertexCount;

	glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr) (vertexCount * sizeof(Vertex)), vertices.data());
	glDrawArrays(GL_POINTS, 0, (GLsizei) vertexCount);
	vertexCount = 0;
}
