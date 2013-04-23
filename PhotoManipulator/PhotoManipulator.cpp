// PhotoManipulator.cpp : main project file.

#include "stdafx.h"
#include "PhotoManip.h"

using namespace PhotoManipulator;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	Application::Run(gcnew PhotoManip());
	return 0;
}
