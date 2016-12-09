#include "Source\Application.h"
int main()
{
	Application::getInstance().init();
	Application::getInstance().run();
	return 0;
}

