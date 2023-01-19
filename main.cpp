//#pragma once
#include <Source/Graphics_Engine/Common/pch.h>
#include <Source/vw_application.h>

int main(void)
{
	auto app = std::make_unique<vw_Application>("JMeshBox");
	app->loop();

	return 0;
}