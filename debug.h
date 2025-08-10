//! @file debug.h
//! @brief DebugLog�N���X�̐錾

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

#ifndef DEBUG_H
#define DEBUG_H

#ifndef H_LAYERLEVEL
 #define H_LAYERLEVEL 1		//!< Select include file.
#endif
#include "main.h"

#ifdef ENABLE_DEBUGLOG
//! @brief �f�o�b�N�����o�͂���N���X
//! @details �f�o�b�N�����t�@�C���ɏo�͂��܂��B
class DebugLog
{
	bool OutputFlag;		//!< �o�͗L��
	char fname[MAX_PATH];	//!< �t�@�C����
	bool error;				//!< �G���[�t���O�i����G���[���L�^����j

public:
	DebugLog();
	~DebugLog();
	void EnableOutputLog();
	void SetLogName(const char* name);
	bool MakeLog();
	bool WriteLog(int tag, const char* title, int id);
	bool WriteLog(int tag, const char* title, const char* text);
};

//! ���O�̏o�̓��[�h��\���萔
enum LogTag
{
	LOG_CHECK,
	LOG_INIT,
	LOG_LOAD,
	LOG_CLEANUP,
	LOG_COMPLETE,
	LOG_ERROR
};
#endif	//ENABLE_DEBUGLOG

#endif