//! @file statemachine.cpp
//! @brief StateMachine�N���X�̒�`

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

#include "statemachine.h"

//! @brief �R���X�g���N�^
StateMachine::StateMachine()
{
	StateMode = 0;
	NowState = STATE_CREATE_OPENING;
	back = false;
	f12 = false;
}

//! @brief �f�B�X�g���N�^
StateMachine::~StateMachine()
{}

//! @brief ��ԑJ�ڃ��[�h��ݒ�
//! @param mode �ʏ�F0�A�u���[�t�B���O/���C�����/���ʕ\���̂݁F1�A���C����ʂ̂݁F2
//! @warning �v���O�������C�����[�v���s�O�ɐݒ肵�Ă��������B
//! @warning ���C�����[�v���Ɏ��s����ƁA��ԑJ�ڂ����������삹���N���b�V������ꍇ������܂��B
void StateMachine::SetStateMode(int mode)
{
	if( mode == 1 ){
		StateMode = mode;
		NowState = STATE_CREATE_BRIEFING;
	}
	else if( mode == 2 ){
		StateMode = mode;
		NowState = STATE_CREATE_MAINGAME;
	}
	else{
		StateMode = 0;
		NowState = STATE_CREATE_OPENING;
	}
}

//! @brief ���̏�Ԃֈڍs
void StateMachine::NextState()
{
	if( StateMode == 0 ){
		//�ʏ�
		switch(NowState){
			case STATE_CREATE_OPENING:
				NowState = STATE_NOW_OPENING;
				break;
			case STATE_NOW_OPENING:
				NowState = STATE_DESTROY_OPENING;
				break;
			case STATE_DESTROY_OPENING:
				if( f12 == true ){
					NowState = STATE_CREATE_OPENING;
				}
				else{
					NowState = STATE_CREATE_MENU;
				}
				break;
			case STATE_CREATE_MENU:
				NowState = STATE_NOW_MENU;
				break;
			case STATE_NOW_MENU:
				NowState = STATE_DESTROY_MENU;
				break;
			case STATE_DESTROY_MENU:
				if( f12 == true ){
					NowState = STATE_CREATE_MENU;
				}
				else if( back == false ){
					NowState = STATE_CREATE_BRIEFING;
				}
				else{
					NowState = STATE_EXIT;
				}
				break;
			case STATE_CREATE_BRIEFING:
				NowState = STATE_NOW_BRIEFING;
				break;
			case STATE_NOW_BRIEFING:
				NowState = STATE_DESTROY_BRIEFING;
				break;
			case STATE_DESTROY_BRIEFING:
				if( f12 == true ){
					NowState = STATE_CREATE_BRIEFING;
				}
				else if( back == false ){
					NowState = STATE_CREATE_MAINGAME;
				}
				else{
					NowState = STATE_CREATE_MENU;
				}
				break;
			case STATE_CREATE_MAINGAME:
				NowState = STATE_NOW_MAINGAME;
				break;
			case STATE_NOW_MAINGAME:
				NowState = STATE_DESTROY_MAINGAME;
				break;
			case STATE_DESTROY_MAINGAME:
				if( f12 == true ){
					NowState = STATE_CREATE_MAINGAME;
				}
				else if( back == false ){
					NowState = STATE_CREATE_RESULT;
				}
				else{
					NowState = STATE_CREATE_MENU;
				}
				break;
			case STATE_CREATE_RESULT:
				NowState = STATE_NOW_RESULT;
				break;
			case STATE_NOW_RESULT:
				NowState = STATE_DESTROY_RESULT;
				break;
			case STATE_DESTROY_RESULT:
				if( f12 == true ){
					NowState = STATE_CREATE_RESULT;
				}
				else{
					NowState = STATE_CREATE_MENU;
				}
				break;
			case STATE_EXIT:
				NowState = STATE_EXIT;
				break;
			default:
				NowState = STATE_NULL;
		}
	}
	else if( StateMode == 1 ){
		//�u���[�t�B���O/���C�����/���ʕ\���̂�
		switch(NowState){
			case STATE_CREATE_BRIEFING:
				NowState = STATE_NOW_BRIEFING;
				break;
			case STATE_NOW_BRIEFING:
				NowState = STATE_DESTROY_BRIEFING;
				break;
			case STATE_DESTROY_BRIEFING:
				if( f12 == true ){
					NowState = STATE_CREATE_BRIEFING;
				}
				else if( back == false ){
					NowState = STATE_CREATE_MAINGAME;
				}
				else{
					NowState = STATE_EXIT;
				}
				break;
			case STATE_CREATE_MAINGAME:
				NowState = STATE_NOW_MAINGAME;
				break;
			case STATE_NOW_MAINGAME:
				NowState = STATE_DESTROY_MAINGAME;
				break;
			case STATE_DESTROY_MAINGAME:
				if( f12 == true ){
					NowState = STATE_CREATE_MAINGAME;
				}
				else if( back == false ){
					NowState = STATE_CREATE_RESULT;
				}
				else{
					NowState = STATE_CREATE_BRIEFING;
				}
				break;
			case STATE_CREATE_RESULT:
				NowState = STATE_NOW_RESULT;
				break;
			case STATE_NOW_RESULT:
				NowState = STATE_DESTROY_RESULT;
				break;
			case STATE_DESTROY_RESULT:
				if( f12 == true ){
					NowState = STATE_CREATE_RESULT;
				}
				else{
					NowState = STATE_CREATE_BRIEFING;
				}
				break;
			case STATE_EXIT:
				NowState = STATE_EXIT;
				break;
			default:
				NowState = STATE_NULL;
		}
	}
	else if( StateMode == 2 ){
		//���C����ʂ̂�
		switch(NowState){
			case STATE_CREATE_MAINGAME:
				NowState = STATE_NOW_MAINGAME;
				break;
			case STATE_NOW_MAINGAME:
				NowState = STATE_DESTROY_MAINGAME;
				break;
			case STATE_DESTROY_MAINGAME:
				if( f12 == true ){
					NowState = STATE_CREATE_MAINGAME;
				}
				else if( back == false ){
					NowState = STATE_CREATE_MAINGAME;
				}
				else{
					NowState = STATE_EXIT;
				}
				break;
			case STATE_EXIT:
				NowState = STATE_EXIT;
				break;
			default:
				NowState = STATE_NULL;
		}
	}
	else{
		NowState = STATE_NULL;
	}

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	switch(NowState){
		case STATE_CREATE_OPENING:
			OutputLog.WriteLog(LOG_CHECK, "State machine", "Opening");
			break;
		case STATE_CREATE_MENU:
			OutputLog.WriteLog(LOG_CHECK, "State machine", "Main menu");
			break;
		case STATE_CREATE_BRIEFING:
			OutputLog.WriteLog(LOG_CHECK, "State machine", "Briefing");
			break;
		case STATE_CREATE_MAINGAME:
			OutputLog.WriteLog(LOG_CHECK, "State machine", "Play mission");
			break;
		case STATE_CREATE_RESULT:
			OutputLog.WriteLog(LOG_CHECK, "State machine", "Result");
			break;
		default:
			;//
	}
#endif
}

//! @brief �}�E�X�N���b�N ���󂯂�
void StateMachine::PushMouseButton()
{
	back = false;
	f12 = false;
	NextState();
}

//! @brief BackSpace �L�[���󂯂�
void StateMachine::PushBackSpaceKey()
{
	back = true;
	f12 = false;
	NextState();
}

//! @brief F12 �L�[���󂯂�
void StateMachine::PushF12Key()
{
	back = false;
	f12 = true;
	NextState();
}

//! @brief ���݂̏�Ԃ�Ԃ�
int StateMachine::GetState()
{
	return NowState;
}