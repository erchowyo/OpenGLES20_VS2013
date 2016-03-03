#include "CELLWinApp.hpp"
int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpcmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(lpcmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	CELL::CELLWinApp app(hInstance);
	app.main(100,100);
	return 0;
}