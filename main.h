//! @file main.h
//! @brief �ŏ�ʂ̃w�b�_�[�t�@�C��

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

#ifndef MAIN_H
#define MAIN_H

#define MAINICON 101		//!< Icon

#ifdef _DEBUG
 #define _CRTDBG_MAP_ALLOC
 #include <crtdbg.h>
#endif

#ifndef H_LAYERLEVEL
 #define H_LAYERLEVEL 3		//!< Select include file.
#endif

#pragma warning(disable:4996)		//VC++�x���h�~
//#pragma warning(disable:4244)

//�萔
#define GAMENAME "OpenXOPS"				//!< �Q�[����
#define GAMEVERSION "1.080"			//!< �Q�[���̃o�[�W����
#define GAMEFRAMEMS 30					//!< �t���[��������̏������ԁims�j
#define GAMEFPS (1000.0f/GAMEFRAMEMS)	//!< FPS�i�t���[�����[�g�j�@1000 / 30 = 33.333[FPS]
#define DEFAULT_SCREEN_WIDTH 640		//!< �X�N���[���̕�
#define DEFAULT_SCREEN_HEIGHT 480		//!< �X�N���[���̍���

#define ADDONDIR "addon"		//!< ADDON������f�B���N�g����
#define MAX_ADDONLIST 128		//!< ADDON��ǂݍ��ލő吔

#define TOTAL_HAVEWEAPON 2		//!< ���Ă镐��̐�

//�V�X�e���̃C���N���[�h�t�@�C��
#define _USE_MATH_DEFINES	 //!< math.h 'M_PI' enable
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <direct.h>

#define ENABLE_BUG_HUMANWEAPON			//!< �͈͊O�̐l�E�����ޔԍ���p�����o�O���Č�����i�R�����g���Ŗ����j
#define ENABLE_BUG_TEAMID				//!< �`�[���ԍ��𕉐��ɐݒ肵���ۂ̃o�O���Č�����i�R�����g���Ŗ����j
#define ENABLE_WEAPONNAME_SCALING		//!< ���햼�������ꍇ�ɕ\�����k������i�R�����g���Ŗ����j
#define ENABLE_ADDOBJ_PARAM8BIT			//!< �ǉ������̐ݒ�l��char�^ 8bit�ɂ���i�R�����g���Ŗ����j
#define ENABLE_DEBUGCONSOLE				//!< �f�o�b�N�p�R���\�[���̗L�����i�R�����g���ŋ@�\�����j
#define ENABLE_DEBUGLOG					//!< �f�o�b�N�p���O�o�̗͂L�����i�R�����g���ŋ@�\�����j
#define ENABLE_CHECKOPENXOPSEVENT		//!< OpenXOPS���씻��p�C�x���g�̗L�����i�R�����g���ŋ@�\�����j
#define ENABLE_MENUOPTIONS				//!< ���j���[�ɃI�v�V������ʒǉ��i�R�����g���ŋ@�\�����j
//#define ENABLE_AUTOCREATECONFIG		//!< config.dat��������Ȃ��ꍇ�A���t�@�C����������������B
//#define ENABLE_PATH_DELIMITER_SLASH	//!< �p�X��؂蕶�����A'\'����e/�f�֕ϊ�����B
#define ENABLE_AUTOLOADMIF				//!< .mif�������Ɏw�肳�ꂽ�ꍇ�A�����I�Ƀ��[�h����i�R�����g���ŋ@�\�����j
#define ENABLE_ADDCONFIG				//!< ini�t�@�C���ɂ��ǉ��ݒ���擾����i�R�����g���ŋ@�\�����j
#define ENABLE_CHECKDATADIR				//!< �N������data�t�H���_�[�̗L�����`�F�b�N����i�R�����g���ŋ@�\�����j

//windows.h���g�p���Ȃ��Ȃ��
#ifndef _MAX_PATH
 #define _MAX_PATH 260		//!< _MAX_PATH is 260
#endif
#ifndef MAX_PATH
 #define MAX_PATH 260		//!< MAX_PATH is 260
#endif

//BorlandC++ Compiler�p�̏���
#ifdef __BORLANDC__
 //#define sqrtf(df) sqrt(df)
 int _matherr(struct _exception  *__e) { return 1; }
#endif

//���g�p�����̑΍��p�}�N��
#ifndef MAIN_H
 #define UNREFERENCED_PARAMETER(P) ((P)=(P))
#endif

//float�^�̎Z�p�֐��p��
#ifndef _MSC_VER
 #define sinf(x) (float)sin(x)				//!< sinf(x)
 #define cosf(x) (float)cos(x)				//!< cosf(x)
 #define asinf(x) (float)asin(x)			//!< asinf(x)
 #define acosf(x) (float)acos(x)			//!< acosf(x)
 #define atan2f(y,x) (float)atan2(y,x)		//!< atan2f(y,x)
 #define sqrtf(x) (float)sqrt(x)			//!< sqrtf(x)
 #define floorf(x) (float)floor(x)			//!< floorf(x)
 #define ceilf(x) (float)ceil(x)			//!< ceilf(x)
 #define fabsf(x) (float)fabs(x)			//!< fabsf(x)
#endif

#define SetFlag(value, bit) value = value | bit		//!< �r�b�g�ɂ��t���O �ݒ�
#define DelFlag(value, bit) value = value & (~bit)	//!< �r�b�g�ɂ��t���O ����
#define GetFlag(value, bit) (value & bit)			//!< �r�b�g�ɂ��t���O �擾

#define DegreeToRadian(value) ((float)M_PI/180*value)	//!< �x���烉�W�A���ւ̕ϊ�

//�჌�C���[
#if H_LAYERLEVEL >= 1
 #include "debug.h"
 #include "window.h"
 #include "config.h"
 #include "datafile.h"
 #include "d3dgraphics.h"
 #include "input.h"
 #include "sound.h"
 #include "parameter.h"
#endif

//���ԃ��C���[
#if H_LAYERLEVEL >= 2
 #include "resource.h"
 #include "collision.h"
 #include "object.h"
 #include "soundmanager.h"
 #include "gui-object.h"
#endif

//�����C���[
#if H_LAYERLEVEL >= 3
 #include "objectmanager.h"
 #include "scene.h"
 #include "gamemain.h"
 #include "ai.h"
 #include "statemachine.h"
 #include "event.h"
#endif

//�ǉ����C�u����
// not .lib

#ifdef ENABLE_DEBUGLOG
 extern class DebugLog OutputLog;
#endif
extern class StateMachine GameState;
extern class Config GameConfig;

bool CheckCommandParameter(const char *param, const char *cmd);
float AngleNormalization(float r);
unsigned int PointParamToInt(signed short int value);
void Set2ByteLittleEndian(unsigned char *outdata, unsigned int value);
void Set4ByteLittleEndian(unsigned char *outdata, unsigned int value);
void Set2ByteBigEndian(unsigned char *outdata, unsigned int value);
void Set4ByteBigEndian(unsigned char *outdata, unsigned int value);
unsigned int Get2ByteLittleEndian(unsigned char *indata);
unsigned int Get4ByteLittleEndian(unsigned char *indata);
unsigned int Get2ByteBigEndian(unsigned char *indata);
unsigned int Get4ByteBigEndian(unsigned char *indata);

#endif