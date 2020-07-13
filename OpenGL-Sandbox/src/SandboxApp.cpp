#include "GLCore.h"
#include "Batch Rendering/BatchRenderingLayer.h"

using namespace GLCore;

class Sandbox : public Application
{
public:
	Sandbox()
	{
		PushLayer(new BatchRenderingLayer());
	}
};

int main()
{
	std::unique_ptr<Sandbox> app = std::make_unique<Sandbox>();
	app->Run();
}