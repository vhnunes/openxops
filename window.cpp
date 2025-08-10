//! @file window.cpp
//! @brief ���C���E�B���h�E�̍쐬�E����

//--------------------------------------------------------------------------------
// 
// OpenXOPS
// Copyright (c) 2014-2023, OpenXOPS Project / [-_-;](mikan) All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// * Redistributions of source code must retain the above copyright notice, 
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, 
//   this list of conditions and the following disclaimer in the documentation 
//   and/or other materials provided with the distribution.
// * Neither the name of the OpenXOPS Project nor the names of its contributors 
//   may be used to endorse or promote products derived from this software 
//   without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL OpenXOPS Project BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//--------------------------------------------------------------------------------

#include "window.h"

//! @brief �R���X�g���N�^
WindowControl::WindowControl()
{
	hInstance = 0;
	nCmdShow = 0;
	hWnd = NULL;

	timeBeginPeriod(1);		//timeEndPeriod�Ƒ�
}

//! @brief �f�B�X�g���N�^
WindowControl::~WindowControl()
{
	timeEndPeriod(1);		//timeBeginPeriod�Ƒ�
}

//! @brief �A�v���P�[�V�����̏���ݒ�
//! @param in_hInstance �C���X�^���X �n���h��
//! @param in_nCmdShow �E�B���h�E�̕\�����
void WindowControl::SetParam(HINSTANCE in_hInstance, int in_nCmdShow)
{
	hInstance = in_hInstance;
	nCmdShow = in_nCmdShow;
}

//! @brief ���C���E�B���h�E�쐬
//! @param title �E�B���h�E�^�C�g��
//! @param width �E�B���h�E�̕�
//! @param height �E�B���h�E�̍���
//! @param fullscreen false�F�E�B���h�E�\���@true�F�t���X�N���[���p�\��
//! @return �����Ftrue�@���s�Ffalse
//! @warning ���SetParam()�֐��Őݒ肵�Ă����K�v������܂��B
bool WindowControl::InitWindow(const char* title, int width, int height, bool fullscreen)
{
	if( title == NULL ){ return false; }
	if( (width <= 0)||(height <= 0) ){ return false; }

	WNDCLASS wc;
	int x, y;
	RECT Rect;
	DWORD dwStyle;

	//�E�B���h�E�N���X�̓o�^
	wc.style		= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc	= WindowProc;
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hInstance	= hInstance;
	wc.hIcon		= LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(MAINICON));
	wc.hCursor		= NULL;
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName	= NULL;
	wc.lpszClassName	= "MainWindow";
	if( RegisterClass(&wc) == 0 ){
		return false;
	}

	if( fullscreen == false ){
		dwStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
	}
	else{
		dwStyle = WS_POPUP;
	}

	//�E�B���h�E�T�C�Y���v�Z
	Rect.left = 0;
	Rect.top = 0;
	Rect.right = width;
	Rect.bottom = height;
	AdjustWindowRect(&Rect, dwStyle, FALSE);
	width = Rect.right - Rect.left;
	height = Rect.bottom - Rect.top;

	//�\���ʒu
	x = 0;
	y = 0;

	/*
	//�E�B���h�E���[�h�Ȃ�A�E�B���h�E����ʂ̒��S�ɔz�u����
	if( fullscreen == false ){
		SystemParametersInfo(SPI_GETWORKAREA, 0, &Rect, 0);
		x = (Rect.right - width)/2;
		y = (Rect.bottom - height)/2;
	}
	*/

	//�E�B���h�E�쐬
	hWnd = CreateWindow( "MainWindow", title, dwStyle, x, y, width, height, NULL, NULL, hInstance, NULL );
	if( hWnd == NULL ){
		return false;
	}

	//�\��
	ShowWindow( hWnd, nCmdShow );

	return true;
}

//! @brief ���C���E�B���h�E�̃E�B���h�E�v���V�[�W��
LRESULT WINAPI WindowControl::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg ){
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

//! @brief �E�B���h�E�̕\�����[�h�؂�ւ�
//! @param fullscreen false�F�E�B���h�E�\���@true�F�t���X�N���[���p�\��
//! @return �����Ftrue�@���s�Ffalse
bool WindowControl::ChangeWindowMode(bool fullscreen)
{
	if( hWnd == NULL ){ return false; }

	DWORD dwStyle;
	RECT Rect;
	int width, height;

	//�E�B���h�E�T�C�Y���擾
	GetClientRect(hWnd, &Rect);

	if( fullscreen == false ){
		ChangeDisplaySettings(NULL, 0);		//�f�B�X�v���C�𑜓x��߂�

		dwStyle = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
	}
	else{
		dwStyle = WS_POPUP;
	}

	//�E�B���h�E�T�C�Y���v�Z
	AdjustWindowRect(&Rect, dwStyle, FALSE);
	width = Rect.right - Rect.left;
	height = Rect.bottom - Rect.top;

	//���f
	SetWindowLong(hWnd, GWL_STYLE, dwStyle);
	SetWindowPos(hWnd, NULL, 0, 0, width, height, SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);

	return true;
}

//! @brief �E�B���h�E�n���h�����擾
//! @return �E�B���h�E�n���h��
HWND WindowControl::GethWnd()
{
	return hWnd;
}

//! @brief �E�B���h�E���b�Z�[�W�������i���C�����[�v����j
//! @return �A�C�h����ԁF0�@�Q�[���̃��C���������s�F1�@�E�B���h�E�������F-1
//! @attention �{�֐�����u1�v���Ԃ��ꂽ�ꍇ�Ɍ���A�Q�[���̃��C�����������s���Ă��������B
//! @attention �{�֐�����u-1�v���Ԃ��ꂽ�ꍇ�A�e���\�[�X��C���^�[�t�F�[�X��������A�Q�[�����I�����Ă��������B
int WindowControl::CheckMainLoop()
{
	MSG msg = {0};

	if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ){
		//�E�B���h�E���b�Z�[�W�����Ă���

		TranslateMessage( &msg );
		DispatchMessage( &msg );

		if( msg.message == WM_QUIT ){
			return -1;
		}
		return 0;
	}
	else if( GetActiveWindow() == hWnd ){
		//�Q�[���̃t���[������
		return 1;
	}
	//else{
		//�E�B���h�E���b�Z�[�W������܂ő҂�
		WaitMessage();
		return 0;
	//}
}

//! @brief �G���[���b�Z�[�W�\��
//! @param *str ���b�Z�[�W
void WindowControl::ErrorInfo(const char *str)
{
	if( str == NULL ){ return; }

	MessageBox(hWnd, str, "error", MB_OK);
}

//! @brief �E�B���h�E�����悤�ɗv��
void WindowControl::CloseWindow()
{
	PostMessage(hWnd, WM_CLOSE, 0L, 0L);
}

#ifdef ENABLE_DEBUGLOG
//! @brief �f�o�b�N�p���O�Ƀ��[�U�[�����o��
//! @note �����_�ł�OS���ƌ���ݒ�̂�
void GetOperatingEnvironment()
{
	char str[256];
	char OSname[128];
	char OSver[128];
	char OSbit[128];
	int strsize;
	HKEY hKey = NULL;

	//���W�X�g������OS���擾
	strcpy(OSname, "");
	strcpy(OSver, "");
	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_EXECUTE | KEY_WOW64_64KEY, &hKey) == ERROR_SUCCESS ){
		strsize = sizeof(OSname);
		RegQueryValueEx(hKey, "ProductName", 0, NULL, (LPBYTE)OSname, (LPDWORD)&strsize);
		strsize = sizeof(OSver);
		RegQueryValueEx(hKey, "DisplayVersion", 0, NULL, (LPBYTE)OSver, (LPDWORD)&strsize);
		RegCloseKey(hKey);
	}

	//OS�r�b�g���擾
	SYSTEM_INFO SystemInfo = {0};
	GetNativeSystemInfo(&SystemInfo);
	switch(SystemInfo.wProcessorArchitecture){
		case PROCESSOR_ARCHITECTURE_AMD64: strcpy(OSbit, "64bit"); break;
		case PROCESSOR_ARCHITECTURE_IA64:  strcpy(OSbit, "64bit"); break;
		case PROCESSOR_ARCHITECTURE_INTEL: strcpy(OSbit, "32bit"); break;
		default: strcpy(OSbit, "unknown bit");
	}

	//OS�������O�ɏo��
	sprintf(str, "OS Name�F%s %s (%s)", OSname, OSver, OSbit);
	OutputLog.WriteLog(LOG_CHECK, "Environment", str);


	//����������O�ɏo��
	sprintf(str, "OS Language�F0x%04hx / 0x%04hx", GetSystemDefaultLangID(), GetUserDefaultLangID());
	OutputLog.WriteLog(LOG_CHECK, "Environment", str);
}
#endif

//! @brief fps�iFrames Per Second�F�t���[�����[�g�j�v�Z
//! @param getcnt fps���擾��������i�t���[���P�ʁj
//! @return fps��
float GetFps(int getcnt)
{
	if( getcnt <= 0 ){ return 0.0f; }

	static unsigned int ptimeg = 0;
	unsigned int nowtime;
	static float pfps = 0.0f;
	static int cnt = 0;

	if( cnt == 0 ){
		ptimeg = GetTimeMS();
	}
	if( cnt == getcnt ){
		nowtime = GetTimeMS();
		pfps = 1000.0f / ((nowtime - ptimeg)/getcnt);
		cnt = 0;
	}
	else{
		cnt += 1;
	}

	return pfps;
}

//! @brief fps�iFrames Per Second�F�t���[�����[�g�j����
//! @return ���������{�Ftrue�@�@���������{�����Ffalse
//! @attention WindowControl�N���X�����������Ȃ��ƁA�^�C�}�[�̐��x���o�܂���B
bool ControlFps()
{
	static unsigned int ptimec = 0;
	unsigned int nowtime;
	unsigned int waittime;

	nowtime = GetTimeMS();
	waittime = GAMEFRAMEMS - (nowtime - ptimec);
	if( (0 < waittime)&&(waittime <= GAMEFRAMEMS) ){
		//timeBeginPeriod(1);
		Sleep(waittime);
		//timeEndPeriod(1);
		
		ptimec = GetTimeMS();
		return true;
	}

	ptimec = nowtime;
	return false;
}

//! @brief �~���b�P�ʂ�Ԃ�
//! @return �~���b
//! @attention WindowControl�N���X�����������Ȃ��ƁA�^�C�}�[�̐��x���o�܂���B
unsigned int GetTimeMS()
{
	unsigned int time;

	//timeBeginPeriod(1);
	time = timeGetTime();
	//timeEndPeriod(1);

	return time;
}

//! @brief �����ɂ�镶������擾
//! @param str ��������󂯎��|�C���^
void GetTimeName(char *str)
{
	if( str == NULL ){ return; }

	time_t timer;
	struct tm *local;

	timer = time(NULL);
	local = localtime(&timer);

	sprintf(str, "%04d%02d%02d%02d%02d%02d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
}

//! @brief ������������
void InitRand()
{
	srand(GetTimeMS());
}

//! @brief �����_���Ȑ����l��Ԃ�
//! @param num �͈�
//! @return 0�`num-1
int GetRand(int num)
{
	if( num <= 0 ){ return 0; }

	return rand()%num;

	//return rand() / (RAND_MAX/num);

	//�{��XOPS�̃A���S���Y���H
	//static int memory = GetTimeMS();
	//int x;
	//memory = memory * 214013 + 2745024;
	//x = memory >> 16;
	//x = x & 0x00007FFF;
	//return x%num;
}

//! @brief �l�̌ܓ�
//! @param r �l
//! @return �l
float Round(float r)
{
	return (r > 0.0f) ? floorf(r + 0.5f) : ceilf(r - 0.5f);
}

#ifdef ENABLE_PATH_DELIMITER_SLASH
//! @brief �p�X��؂蕶����ϊ�
//! @param *str �t�@�C���p�X�̃|�C���^
//! @return �V�����t�@�C���p�X�̃|�C���^
//! @note '\'��'/'�֒u�������܂��B
char* ChangePathDelimiter(char *str)
{
	if( str == NULL ){ return NULL; }

	static char newstr[MAX_PATH];
	strcpy(newstr, str);

	for(int i=0; i<(int)strlen(newstr); i++){
		if( newstr[i] == '\\' ){
			newstr[i] = '/';
		}
	}

	return newstr;
}
#endif
