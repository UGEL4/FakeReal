#pragma once
#include "Application.h"
#include "EngineMain.h"

extern FakeReal::Application* FakeReal::Create();

int main(int argc, char** argv)
{
	FakeReal::Create();
	FakeReal::EngineMain::Initialize();
	FakeReal::Application::ms_pApplication->Main();
	delete FakeReal::Application::ms_pApplication;
	return 0;
}