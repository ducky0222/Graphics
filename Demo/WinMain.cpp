#include "GameProcess.h"

#include "WindowsHeader.h"

// 전역 변수
HWND g_hWnd = nullptr;

/// <summary>
/// 프로그램의 엔트리 포인트
/// </summary>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	/// 0. 프로그램의 엔트리 포인트

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	GameProcess* gameProcess = new GameProcess;
	gameProcess->Initialize(hInstance);
	gameProcess->Loop();
	gameProcess->Finalize();

	_CrtDumpMemoryLeaks();

	delete gameProcess;

	return 0;
}