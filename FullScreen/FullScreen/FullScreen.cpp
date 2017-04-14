//ʵ��ȫ�����ڣ�F1ȫ����ESCȡ��ȫ��
#include <Windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrecInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WNDCLASSEX wcx;
	HWND hwnd;
	MSG msg;

	wcx.cbSize = sizeof(wcx);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WndProc;
	wcx.cbClsExtra = 0;
	wcx.cbWndExtra = 0;
	wcx.hInstance = hInstance;
	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcx.lpszMenuName = NULL;
	wcx.lpszClassName = TEXT("WndClass");
	wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wcx);

	hwnd = CreateWindow(TEXT("WndClass"), TEXT("���ڳ���"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, nShowCmd);

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static BOOL bFull = FALSE;//�Ƿ�ȫ��
	static WINDOWPLACEMENT wpNormalScreen, wpFullScreen;//��¼������ʾ״̬��Ϣ
	static RECT rectWindow;//��������
	static RECT rectClient;//�ͻ�������
	static RECT rectFullScreen;//��Ļȫ��ʱ����λ��
	static POINT pointUpLeft, pointDownRight;//���ϽǺ����½�����
	static int nScreenWidth, nScreenHeight;//��ʾ���ֱ���
	static MINMAXINFO *pmmi;//��¼������С�����Ϣ

	switch (uMsg)
	{
	case WM_CREATE://���ڴ���
		nScreenWidth = GetSystemMetrics(SM_CXSCREEN);//��Ļ����ֱ���
		nScreenHeight = GetSystemMetrics(SM_CYSCREEN);//��Ļ����ֱ���
		break;
	case WM_KEYDOWN://������Ϣ��F1ȫ����F2ȡ��ȫ��
		switch (wParam)
		{
		case VK_F1://ȫ��
			if (bFull) break;
			bFull = TRUE;
			//����ȫ��ǰ������ʾ״̬��Ϣ
			wpNormalScreen.length = sizeof(WINDOWPLACEMENT);
			GetWindowPlacement(hWnd, &wpNormalScreen);
			//��ȡ���ںͿͻ�����С
			GetWindowRect(hWnd, &rectWindow);
			GetClientRect(hWnd, &rectClient);
			pointUpLeft.x = rectClient.left;
			pointUpLeft.y = rectClient.top;
			pointDownRight.x = rectClient.right;
			pointDownRight.y = rectClient.bottom;
			ClientToScreen(hWnd, &pointUpLeft);
			ClientToScreen(hWnd, &pointDownRight);
			//����ȫ��ʱ����λ��
			rectFullScreen.left = rectWindow.left - pointUpLeft.x;
			rectFullScreen.top = rectWindow.top - pointUpLeft.y;
			rectFullScreen.right = nScreenWidth + rectWindow.right - pointDownRight.x;
			rectFullScreen.bottom = nScreenHeight + rectWindow.bottom - pointDownRight.y;
			//����SetWindowPlacementʵ��ȫ��
			wpFullScreen.length = sizeof(WINDOWPLACEMENT);
			wpFullScreen.flags = 0;
			wpFullScreen.showCmd = SW_SHOWNORMAL;
			wpFullScreen.rcNormalPosition = rectFullScreen;
			SetWindowPlacement(hWnd, &wpFullScreen);
			break;
		case VK_ESCAPE://ȡ��ȫ��
			if (!bFull) break;
			bFull = FALSE;
			//�ָ�ԭ�����ڴ�С
			SetWindowPlacement(hWnd, &wpNormalScreen);
			break;
		}
		break;
	case WM_GETMINMAXINFO://������С�����Ϣ���������������ס������
		if (!bFull) break;
		//����ȫ������С
		pmmi = (MINMAXINFO *)lParam;
		pmmi->ptMaxPosition.x = rectFullScreen.left;
		pmmi->ptMaxPosition.y = rectFullScreen.top;
		pmmi->ptMaxSize.x = rectFullScreen.right - rectFullScreen.left;
		pmmi->ptMaxSize.y = rectFullScreen.bottom - rectFullScreen.top;
		pmmi->ptMaxTrackSize.x = rectFullScreen.right - rectFullScreen.left;
		pmmi->ptMaxTrackSize.y = rectFullScreen.right - rectFullScreen.left;
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;;
	}
	return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}
