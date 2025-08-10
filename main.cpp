//! @file main.cpp
//! @brief WinMain()�֐��̒�`����уe�X�g�v���O���� 

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

//Doxygen�ݒ�t�@�C��
#include "doxygen.h"

#include "main.h"

#ifdef _DEBUG
 #include <shlwapi.h>
 #pragma comment(lib, "Shlwapi.lib")
#endif

#ifdef ENABLE_DEBUGLOG
 //! ���O�o��
 DebugLog OutputLog;
#endif

//! �X�e�[�g�}�V��
StateMachine GameState;

//! �Q�[���ݒ�f�[�^
Config GameConfig;

//! ���C���E�B���h�E
WindowControl MainWindow;

//! @brief WinMain()�֐�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//���g�p�����΍�
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	char path[MAX_PATH];

#ifdef ENABLE_AUTOLOADMIF
	bool NoBriefingFlag = false;
	int gamemode = 0;
#endif

	//����������
	InitRand();

	//���s�t�@�C���̂���ꏊ���A�J�����g�f�B���N�g���ɂ���B
	GetFileDirectory(__argv[0], path);
	chdir(path);

#ifdef _DEBUG
	//���������[�N�̌��o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	char str[255];
	strcpy(str, "[Information]\nThe compiler is the Debug mode.\nIf release the software, Switch compiler to Release mode.");
	MessageBox(NULL, str, GAMENAME, MB_OK);
#endif

	//��������
	//�����FWindows���Ȃ�AlpCmdLine�����Ƃ�GetCommandLine()�֐��ł����܂����ǂˁE�E�B
	strcpy(path, "");
	if( __argc > 1 ){
		for(int param=1; param<__argc; param++){
#ifdef ENABLE_DEBUGLOG
			//"Log"���^�����Ă�����A���O�o�͂�L����
			if( CheckCommandParameter(__argv[param], "-Log") == true ){
				//���O�o�͂�L����
				OutputLog.EnableOutputLog();

				MainWindow.ErrorInfo("Enable Debug Log...");
				continue;
			}
#endif

#ifdef ENABLE_AUTOLOADMIF
			//"Dir"���^�����Ă�����A�J�����g�f�B���N�g���ړ�
			if( CheckCommandParameter(__argv[param], "-Dir") == true ){
				if( (param+1) < __argc ){
					chdir(__argv[param+1]);
					param += 1;
				}
				continue;
			}

			//"NoBriefing"���^�����Ă�����A�i.mif�w�莞�́j�u���[�t�B���O���ȗ�����
			if( CheckCommandParameter(__argv[param], "-NoBriefing") == true ){
				NoBriefingFlag = true;
				continue;
			}

			//����ȊO�̈����Ȃ�Ίg���q�𔻒肵�Ă݂�
			if( CheckFileExtension(__argv[param], ".mif") == true ){
				//.mif�ł���΃t�@�C���p�X�Ƃ��ċL��
				strcpy(path, __argv[param]);
			}
#endif
		}

#ifdef ENABLE_AUTOLOADMIF
		//���������.mif���w�肳��Ă���Ȃ�΁A�u���[�t�B���O�t���O���f
		if( path[0] != '\0' ){
			if( NoBriefingFlag == false ){ gamemode = 1; }
			else{ gamemode = 2; }
		}
#endif
	}

#ifdef ENABLE_DEBUGLOG
	//�t�@�C���쐬
	OutputLog.MakeLog();

	//���[�U�[�����o��
	GetOperatingEnvironment();

	char infostr[64];

	/*
	//���O�ɏo��
	sprintf(infostr, "Total Parameters : %d", __argc);
	OutputLog.WriteLog(LOG_CHECK, "Parameter", infostr);
	for(int i=0; i<__argc; i++){
		sprintf(infostr, "Parameter[%d]", i);
		OutputLog.WriteLog(LOG_CHECK, infostr, __argv[i]);
	}
	*/

	//���O�ɏo��
	OutputLog.WriteLog(LOG_CHECK, "Startup", "Start program entry point");
#endif

#ifdef _DEBUG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_CHECK, "Startup", "Visual C++ debug mode");
#endif

#ifdef _DEBUG
	char path2[MAX_PATH];
	getcwd(path2, MAX_PATH);
	sprintf(infostr, "EXE directory path length : %d", strlen(path2));

	//���O�ɏo��
	OutputLog.WriteLog(LOG_CHECK, "Environment", infostr);
#endif

	//�f�[�^�t�H���_�`�F�b�N
#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_LOAD, "Directory", "data folder");
	if( CheckDirectory("data") == true ){
		OutputLog.WriteLog(LOG_COMPLETE, "", "");
	}
	else{
		OutputLog.WriteLog(LOG_ERROR, "", "");
	}
#endif
#ifdef ENABLE_CHECKDATADIR
	if( CheckDirectory("data") == false ){
		MainWindow.ErrorInfo("Data directory not exist.\n\nPlease download and copy \"X operations 0.96\".");
		return 1;
	}
#endif

	//�ݒ�t�@�C���ǂݍ���
	if( GameConfig.LoadFile("config.dat") == 1 ){
#ifndef ENABLE_AUTOCREATECONFIG
		MainWindow.ErrorInfo("config data open failed");
		return 1;
#else
		//�f�t�H���g�l�𔽉f���A�ݒ�t�@�C���ɕۑ�����
		GameConfig.SetDefaultConfig();
		if( GameConfig.SaveFile("config.dat") == 1 ){
			MainWindow.ErrorInfo("config data save failed");
			return 1;
		}
#endif
	}

	//�ǉ��ݒ��ǂݍ���
#ifdef ENABLE_ADDCONFIG
	GameConfig.CreateExtConfig("config-openxops.ini");
	GameConfig.LoadExtFile("config-openxops.ini");
#else
	GameConfig.LoadExtFile(NULL);
#endif

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	sprintf(infostr, "Display resolution : %d x %d", GameConfig.GetScreenWidth(), GameConfig.GetScreenHeight());
	OutputLog.WriteLog(LOG_CHECK, "Environment", infostr);
	if( GameConfig.GetFullscreenFlag() == false ){
		OutputLog.WriteLog(LOG_CHECK, "Environment", "Window mode : Window");
	}
	else{
		OutputLog.WriteLog(LOG_CHECK, "Environment", "Window mode : Fullscreen");
	}
	sprintf(infostr, "Game language mode : %d", GameConfig.GetLanguage());
	OutputLog.WriteLog(LOG_CHECK, "Environment", infostr);
#endif

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_CHECK, "Startup", "Start game initialize");
#endif

	//�E�B���h�E������
	MainWindow.SetParam(hPrevInstance, nCmdShow);
	MainWindow.InitWindow(GAMENAME, GameConfig.GetScreenWidth(), GameConfig.GetScreenHeight(), GameConfig.GetFullscreenFlag());

	//��{�I�ȏ���������
#ifdef ENABLE_AUTOLOADMIF
	if( InitGame(&MainWindow, gamemode, path) ){
#else
	if( InitGame(&MainWindow, 0, "") ){
#endif
		return 1;
	}

	opening Opening;
	mainmenu MainMenu;
	briefing Briefing;
	maingame MainGame;
	result Result;
	InitScreen(&MainWindow, &Opening, &MainMenu, &Briefing, &MainGame, &Result);

#ifdef ENABLE_AUTOLOADMIF
	//���C���Q�[���̂ݕ\�����郂�[�h�Ȃ�΁A�W���Ńf�o�b�N�\����L���ɂ���B
	if( gamemode == 2 ){ MainGame.SetShowInfoFlag(true); }
#endif


#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_CHECK, "Startup", "Start main loop");
#endif

	unsigned int framecnt = 0;

	for(int flag = 0; flag != -1; flag = MainWindow.CheckMainLoop()){
		if( flag == 1 ){
			//���C������
			ProcessScreen(&MainWindow, &Opening, &MainMenu, &Briefing, &MainGame, &Result, framecnt);

			//FPS����
			ControlFps();

			framecnt++;
		}
	}

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_CHECK, "Exit", "Exit main loop");
#endif


	//��{�I�ȉ������
	CleanupGame();

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_CHECK, "Exit", "Exit program entry point");
#endif
	return 0;
}

//! @brief �R�}���h��������
//! @param param ���̕�����
//! @param cmd ���肷�����
//! @return �Y���Ftrue�@��Y���Ffalse
//! @warning cmd�����̍ŏ��̈ꕶ���͖�������܂��B
//! @attention ���̈����𔻒肵�܂��B��/[����]�A-[����]�A/[�����i�������j]�A-[�����i�������j]
bool CheckCommandParameter(const char *param, const char *cmd)
{
	if( param == NULL ){ return false; }
	if( cmd == NULL ){ return false; }
	if( strlen(param) >= 16 ){ return false; }
	if( strlen(cmd) >= 16 ){ return false; }
	if( strlen(param) != strlen(cmd) ){ return false; }

	char str[16];
	strcpy(str, cmd);

	str[0] = '/';
	if( strcmp(param, str) == 0 ){ return true; }
	str[0] = '-';
	if( strcmp(param, str) == 0 ){ return true; }

	for(int i=0; i<(int)strlen(str); i++){ str[i] = (char)tolower((int)(str[i])); }

	str[0] = '/';
	if( strcmp(param, str) == 0 ){ return true; }
	str[0] = '-';
	if( strcmp(param, str) == 0 ){ return true; }

	return false;
}

//! @brief �p�x�i���W�A���j���K��
//! @param r �p�x
//! @return ���K����̊p�x
//! @note -PI�`PI �̊Ԃɐ��K�����܂��B
float AngleNormalization(float r)
{
	for(; r > (float)M_PI;    r -= (float)M_PI*2){}
	for(; r < (float)M_PI*-1; r += (float)M_PI*2){}
	return r;
}

//! @brief �|�C���g�p�����[�^�[�̒l��unsigned int�^�ɕϊ�
//! @param value �l
//! @return �����l
unsigned int PointParamToInt(signed short int value)
{
	return (unsigned int)((unsigned char)value);
}

//! @brief 2�o�C�g�ɐ��l�ݒ�i���g���G���f�B�A���j
//! @param outdata 2�o�C�g�̃|�C���^
//! @param value ���l
void Set2ByteLittleEndian(unsigned char *outdata, unsigned int value)
{
	if( outdata == NULL ){ return; }

	outdata[0] = (unsigned char)(( value >> 0 ) & 0x00FF);
	outdata[1] = (unsigned char)(( value >> 8 ) & 0x00FF);
}

//! @brief 4�o�C�g�ɐ��l�ݒ�i���g���G���f�B�A���j
//! @param outdata 4�o�C�g�̃|�C���^
//! @param value ���l
void Set4ByteLittleEndian(unsigned char *outdata, unsigned int value)
{
	if( outdata == NULL ){ return; }

	outdata[0] = (unsigned char)(( value >> 0 ) & 0x000000FF);
	outdata[1] = (unsigned char)(( value >> 8 ) & 0x000000FF);
	outdata[2] = (unsigned char)(( value >> 16 ) & 0x000000FF);
	outdata[3] = (unsigned char)(( value >> 24 ) & 0x000000FF);
}

//! @brief 2�o�C�g�ɐ��l�ݒ�i�r�b�O�G���f�B�A���j
//! @param outdata 2�o�C�g�̃|�C���^
//! @param value ���l
void Set2ByteBigEndian(unsigned char *outdata, unsigned int value)
{
	if( outdata == NULL ){ return; }

	outdata[0] = (unsigned char)(( value >> 8 ) & 0x00FF);
	outdata[1] = (unsigned char)(( value >> 0 ) & 0x00FF);
}

//! @brief 4�o�C�g�ɐ��l�ݒ�i�r�b�O�G���f�B�A���j
//! @param outdata 4�o�C�g�̃|�C���^
//! @param value ���l
void Set4ByteBigEndian(unsigned char *outdata, unsigned int value)
{
	if( outdata == NULL ){ return; }

	outdata[0] = (unsigned char)(( value >> 24 ) & 0x000000FF);
	outdata[1] = (unsigned char)(( value >> 16 ) & 0x000000FF);
	outdata[2] = (unsigned char)(( value >> 8 ) & 0x000000FF);
	outdata[3] = (unsigned char)(( value >> 0 ) & 0x000000FF);
}

//! @brief 2�o�C�g�̐��l�擾�i���g���G���f�B�A���j
//! @param indata 2�o�C�g�̃|�C���^
//! @return ���l
unsigned int Get2ByteLittleEndian(unsigned char *indata)
{
	if( indata == NULL ){ return 0; }

	return (unsigned int)( ((indata[1]<<8)&0xFF00) + ((indata[0]<<0)&0x00FF) );
}

//! @brief 4�o�C�g�̐��l�擾�i���g���G���f�B�A���j
//! @param indata 4�o�C�g�̃|�C���^
//! @return ���l
unsigned int Get4ByteLittleEndian(unsigned char *indata)
{
	if( indata == NULL ){ return 0; }

	return (unsigned int)( ((indata[3]<<24)&0xFF000000) + ((indata[2]<<16)&0x00FF0000) + ((indata[1]<<8)&0x0000FF00) + ((indata[0]<<0)&0x000000FF) );
}

//! @brief 2�o�C�g�̐��l�擾�i�r�b�O�G���f�B�A���j
//! @param indata 2�o�C�g�̃|�C���^
//! @return ���l
unsigned int Get2ByteBigEndian(unsigned char *indata)
{
	if( indata == NULL ){ return 0; }

	return (unsigned int)( ((indata[0]<<8)&0xFF00) + ((indata[1]<<0)&0x00FF) );
}

//! @brief 4�o�C�g�̐��l�擾�i�r�b�O�G���f�B�A���j
//! @param indata 4�o�C�g�̃|�C���^
//! @return ���l
unsigned int Get4ByteBigEndian(unsigned char *indata)
{
	if( indata == NULL ){ return 0; }

	return (unsigned int)( ((indata[0]<<24)&0xFF000000) + ((indata[1]<<16)&0x00FF0000) + ((indata[2]<<8)&0x0000FF00) + ((indata[3]<<0)&0x000000FF) );
}
