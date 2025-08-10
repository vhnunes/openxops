//! @file statemachine.h
//! @brief StateMachine�N���X�̐錾

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

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#ifndef H_LAYERLEVEL
 #define H_LAYERLEVEL 3		//!< Select include file.
#endif
#include "main.h"

//! ��Ԃ�\���萔
enum State
{
	STATE_NULL = 0,			//!< �m�G���[�n�����ȏ��
	STATE_CREATE_OPENING,	//!< �I�[�v�j���O��� �������E����
	STATE_NOW_OPENING,		//!< �I�[�v�j���O��� ���s��
	STATE_DESTROY_OPENING,	//!< �I�[�v�j���O��� �I���E�p��
	STATE_CREATE_MENU,		//!< ���j���[��� �������E����
	STATE_NOW_MENU,			//!< ���j���[��� ���s��
	STATE_DESTROY_MENU,		//!< ���j���[��� �I���E�p��
	STATE_CREATE_BRIEFING,	//!< �u���[�t�B���O��� �������E����
	STATE_NOW_BRIEFING,		//!< �u���[�t�B���O��� ���s��
	STATE_DESTROY_BRIEFING,	//!< �u���[�t�B���O��� �I���E�p��
	STATE_CREATE_MAINGAME,	//!< ���C����� �������E����
	STATE_NOW_MAINGAME,		//!< ���C����� ���s��
	STATE_DESTROY_MAINGAME,	//!< ���C����� �I���E�p��
	STATE_CREATE_RESULT,	//!< ���ʕ\����� �������E����
	STATE_NOW_RESULT,		//!< ���ʕ\����� ���s��
	STATE_DESTROY_RESULT,	//!< ���ʕ\����� �I���E�p��
	STATE_EXIT				//!< �Q�[���I��
};

//! @brief ��ԑJ�ڃN���X
//! @details �Q�[���S�̂̉�ʑJ�ڂɊւ��Ǘ����s���܂��B
class StateMachine
{
	int StateMode;	//!< ��ԑJ�ڃ��[�h
	int NowState;	//!< ���݂̃X�e�[�g
	bool back;		//!< ESC�L�[�������ꂽ
	bool f12;		//!< F12�L�[�������ꂽ

public:
	StateMachine();
	~StateMachine();
	void SetStateMode(int mode);
	void NextState();
	void PushMouseButton();
	void PushBackSpaceKey();
	void PushF12Key();
	int GetState();
};

#endif