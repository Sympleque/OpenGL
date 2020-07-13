
#include "BatchRenderingLayer.h"

#include "Renderer.h"

using namespace GLCore;
using namespace GLCore::Utils;

BatchRenderingLayer::BatchRenderingLayer()
	: m_Camera(16.0f / 9.0f)
{
}

BatchRenderingLayer::~BatchRenderingLayer()
{
}

void BatchRenderingLayer::OnAttach()
{
	EnableGLDebugging();

	// Determine how big you want the batches
	Renderer::Init({ 10000 });
}

void BatchRenderingLayer::OnDetach()
{
	Renderer::Shutdown();
}

void BatchRenderingLayer::OnEvent(Event& event)
{
	m_Camera.OnEvent(event);
}

void BatchRenderingLayer::OnUpdate(Timestep ts)
{
	static float rotation = 0.0f;
	rotation += glm::radians(90.0f) * ts;

	m_Camera.OnUpdate(ts);
	glClear(GL_COLOR_BUFFER_BIT);

	Renderer::BeginScene(m_Camera.GetCamera());

	// Drawing 10,000? :P
	for (int i = 0; i < 100; ++i)
		for (int j = 0; j < 100; ++j)
			Renderer::Draw({ i * 0.2f, j * 0.2f }, glm::vec2{ 0.1f }, { 0.2f, 0.4f, 1.0f, 1.0f }, rotation);
	
	Renderer::EndScene();
}

void BatchRenderingLayer::OnImGuiRender()
{
}
