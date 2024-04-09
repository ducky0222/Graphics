#include "GameProcess.h"

#include "WindowsHeader.h"

// ���� ����
HWND g_hWnd = nullptr;

/// <summary>
/// ���α׷��� ��Ʈ�� ����Ʈ
/// </summary>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	/// 0. ���α׷��� ��Ʈ�� ����Ʈ

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