/**
 * @file EntryPoint.h
 * @brief Defines the entry point for the application.
 * 
 * @details This file contains the main entry point for the application, supporting both Windows-specific
 * and standard C++ entry points. The EntryPoint function is shared between main (for non-Windows
 * platforms) and WinMain (for Windows platforms). It is responsible for initializing the logging system,
 * creating the application instance, running the application, and cleaning up afterward.
 * 
 * On Windows platforms, the WinMain function is used as the entry point, while on other platforms,
 * the standard main function is used. Both function delegate to the shared EntryPoint function.
 * 
 * @see @ref usage
 */
#pragma once
#ifdef AR_PLATFORM_WINDOWS
#include <windows.h>
#endif

/**
 * @brief Main entry point for the application.
 * 
 * @details This function initializes the logging system, creates an application instance,
 * runs the application, and finally cleans up by deleting the application instance.
 * It serves as a common entry point for both the Windows platform-specific WinMain
 * and the standard main function.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The list of command-line arguments.
 * @return An integer representing the exit code of the application.
 */
int EntryPoint(int argc, char** argv)
{
	// Initialize the logging system with a trace level.
	Ares::Log::Init(spdlog::level::trace);

	// Create the application instance.
	Ares::Application* app = Ares::CreateApplication();

	// Run the application.
	app->Run();

	// Clean up by deleting the application instance.
	delete app;

	// Return a successful exit code.
	return 0;
}

/**
 * @brief Standard C++ entry point for the application.
 * 
 * @details This function serves as the entry point for non-Windows platforms,
 * calling the shared EntryPoint function with the standard command-line arguments.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The list of command-line arguments.
 * @return An integer representing the exit code of the application.
 */
int main(int argc, char** argv)
{
	return EntryPoint(argc, argv);
}

#ifdef AR_PLATFORM_WINDOWS

/**
 * @brief Windows-specific entry point for the application
 * 
 * @details This function is the entry point for Windows applications, following the
 * WinMain convention. It calls the shared EntryPoint function, passing
 * the command-line arguments specific to Windows applications.
 * 
 * @param hInstance A handle to the current instance of the application.
 * @param hPrevInstance A handle to the previous instance of the application.
 * @param lpCmdLine A pointer to a null-terminated string representing the command-line arguments.
 * @param nShowCmd The show state of the window.
 * @return An integer representing the exit code of the application.
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// Call the EntryPoint function with Windows-specific command-line arguments.
	return EntryPoint(__argc, __argv);
}
#endif