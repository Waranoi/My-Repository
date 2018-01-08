#include "Triangle_app.h"

int main()
{
	Triangle_app app = Triangle_app();
	if (app.Setup() == true)
		app.Application_loop();
	return 0;
}