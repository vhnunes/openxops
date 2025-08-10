//! @file input.h
//! @brief InputControl�N���X�̐錾

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

#ifndef INPUT_H
#define INPUT_H

#ifndef H_LAYERLEVEL
 #define H_LAYERLEVEL 1		//!< Select include file.
#endif
#include "main.h"

#include <windows.h>

#define INPUT_INTERFACE 0	//!< @brief ���͂��擾����C���^�[�t�F�[�X�̑I�� @details WindowsInput�F0�@RawInput�F1�@DirectInput�F2

#if INPUT_INTERFACE == 0
 #define INPUT_CORE "WindowsInput"					//!< �o�[�W�����\���p���
#elif INPUT_INTERFACE == 1
 #include <commctrl.h>
 #pragma comment (lib, "comctl32.lib")

 #define INPUT_CORE "RawInput"						//!< �o�[�W�����\���p���
#elif INPUT_INTERFACE == 2
 #define DIRECTINPUT_VERSION 0x0800	//!< �x���uDIRECTINPUT_VERSION undefined. Defaulting to version 0x0800�v��h�~
 #include <dinput.h>

 #pragma comment(lib, "dinput8.lib")
 #pragma comment(lib, "dxguid.lib")

 #define INPUT_CORE "DirectInput"					//!< �o�[�W�����\���p���
#endif

//! @brief ���̓f�o�C�X���Ǘ�����N���X
//! @details �}�E�X��L�[�{�[�h�Ȃǂ̓��̓f�o�C�X���Ǘ����Ă��܂��B
class InputControl
{
	HWND hWnd;				//!< �E�B���h�E�n���h��
#if INPUT_INTERFACE == 2
	LPDIRECTINPUT8 pDI;					//!< DIRECTINPUT8�̃|�C���^
	LPDIRECTINPUTDEVICE8 pDIDevice;		//!< DIRECTINPUTDEVICE8�̃|�C���^�E�L�[�{�[�h�p
	LPDIRECTINPUTDEVICE8 pMouse;		//!< DIRECTINPUTDEVICE8�̃|�C���^�E�}�E�X�p
#else
	bool InitFlag;			//!< �������t���O
#endif
	bool MouseLimitFlag;	//!< �}�E�X�ړ��ʂ𐧌�����t���O
	char keys[256];			//!< �L�[�����i�[����z��
	char keys_lt[256];		//!< �i�O��́j�L�[�����i�[����z��
	int mx;					//!< �}�E�X��X���W
	int my;					//!< �}�E�X��Y���W
#if INPUT_INTERFACE == 0
	POINT point_lt;			//!< �i�O��́j�}�E�X���W
#endif
	bool mbl;				//!< �}�E�X�̍��{�^��
	bool mbr;				//!< �}�E�X�̉E�{�^��
	bool mbl_lt;			//!< �i�O��́j�}�E�X�̍��{�^��
	bool mbr_lt;			//!< �i�O��́j�}�E�X�̉E�{�^��
#if INPUT_INTERFACE == 1
	char keys_proc[256];	//!< �L�[�����i�[����z��i�v���V�[�W���j
	int mx_proc;			//!< �}�E�X��X���W�i�v���V�[�W���j
	int my_proc;			//!< �}�E�X��Y���W�i�v���V�[�W���j
	bool mbl_proc;			//!< �}�E�X�̍��{�^���i�v���V�[�W���j
	bool mbr_proc;			//!< �}�E�X�̉E�{�^���i�v���V�[�W���j

	static LRESULT CALLBACK SubClassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	void GetRowInput(WPARAM wParam, LPARAM lParam);
#endif

public:
	InputControl();
	~InputControl();
	int InitInput(WindowControl *WindowCtrl);
	void DestroyInput();
	void SetMouseLimitFlag(bool flag);
	void GetInputState(bool mousemode);
	void MoveMouseCenter();
	bool CheckKeyNow(int id);
	bool CheckKeyDown(int id);
	bool CheckKeyUp(int id);
	void GetMouseMovement(int *x, int *y);
	bool CheckMouseButtonNowL();
	bool CheckMouseButtonDownL();
	bool CheckMouseButtonUpL();
	bool CheckMouseButtonNowR();
	bool CheckMouseButtonDownR();
	bool CheckMouseButtonUpR();
	int CheckInputAll();
};

int OriginalkeycodeToDinputdef(int code);
bool GetDoubleKeyCode(int id, int *CodeL, int *CodeR);
int GetEscKeycode();
int GetHomeKeycode();
int GetFunctionKeycode(int key);

#endif