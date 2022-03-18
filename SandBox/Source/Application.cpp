#include "Application.h"
#include "EntryPoint.h"
#include "Editor.h"

class SandBoxApp : public FakeReal::Application
{
public:
	SandBoxApp()
	{
		PushLayer(new FakeReal::GameEditor);
	}
	~SandBoxApp() {}
};

FakeReal::Application* FakeReal::Create()
{
	return new SandBoxApp;
}