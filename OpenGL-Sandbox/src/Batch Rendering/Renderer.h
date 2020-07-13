#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLCore/Util/OrthographicCamera.h>

struct RendererProps
{
	size_t batchCapacity;
};

class Renderer {

public:
	static void Init(const RendererProps& props);
	static void Shutdown();

	static void BeginScene(const GLCore::Utils::OrthographicCamera &camera);

	// Rotations are expected in radians.
	static void Draw(
		const glm::vec2 &pos, 
		const glm::vec2 &size, 
		const glm::vec4 &tint, 
		float rotation = 0);
	
	static void EndScene();

private:
	static void Flush();

};
