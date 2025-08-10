//! @file config.cpp
//! @brief config�N���X�̒�` 

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

#include "config.h"

//! @brief �R���X�g���N�^
Config::Config()
{
	//�e�탁���o�[�ϐ�������
	for(int i=0; i<TOTAL_ControlKey; i++){
		Keycode[i] = 0;
	}
	MouseSensitivity = 0;
	FullscreenFlag = false;
	SoundFlag = false;
	BloodFlag = false;
	Brightness = 0;
	InvertMouseFlag = false;
	FrameskipFlag = false;
	AnotherGunsightFlag = false;
	strcpy(PlayerName, "");

	Language = 0;
	ShowArmFlag = false;
	ScreenWidth = 0;
	ScreenHeight = 0;
	MouseLimitFlag = true;
	mastervolume = 0.0f;
}

//! @brief �f�B�X�g���N�^
Config::~Config()
{}

//! @brief �ݒ�t�@�C����ǂݍ���
//! @param fname �t�@�C����
//! @return �����F0�@���s�F1
int Config::LoadFile(const char *fname)
{
	if( fname == NULL ){ return 1; }

	FILE *fp;
	char buf;

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_LOAD, "Config File", fname);
#endif

#ifdef ENABLE_PATH_DELIMITER_SLASH
	//�p�X��؂蕶����ϊ�
	fname = ChangePathDelimiter(fname);
#endif

	//�t�@�C�����J��
	fp = fopen(fname, "rb");
	if( fp == NULL ){ return 1; }

	//�L�[�R�[�h
	for(int i=0; i<TOTAL_ControlKey; i++){
		fread(&buf, 1, 1, fp);
		Keycode[i] = (int)buf;
	}

	//�}�E�X���x
	fread(&buf, 1, 1, fp);
	MouseSensitivity = (int)buf;

	//�t���X�N���[���L��
	fread(&buf, 1, 1, fp);
	if( buf == 0x00 ){ FullscreenFlag = false; }
	else{ FullscreenFlag = true; }

	//���ʉ��L��
	fread(&buf, 1, 1, fp);
	if( buf == 0x00 ){ SoundFlag = false; }
	else{ SoundFlag = true; }

	//�o���L��
	fread(&buf, 1, 1, fp);
	if( buf == 0x00 ){ BloodFlag = false; }
	else{ BloodFlag = true; }

	//��ʂ̖��邳
	fread(&buf, 1, 1, fp);
	Brightness = (int)buf;

	//�}�E�X���]
	fread(&buf, 1, 1, fp);
	if( buf == 0x00 ){ InvertMouseFlag = false; }
	else{ InvertMouseFlag = true; }

	//�t���[���X�L�b�v
	fread(&buf, 1, 1, fp);
	if( buf == 0x00 ){ FrameskipFlag = false; }
	else{ FrameskipFlag = true; }

	//�ʂ̏Ə����g�p
	fread(&buf, 1, 1, fp);
	if( buf == 0x00 ){ AnotherGunsightFlag = false; }
	else{ AnotherGunsightFlag = true; }

	//�v���C���[��
	fread(PlayerName, 1, MAX_PLAYERNAME, fp);

	//�t�@�C���n���h�������
	fclose(fp);

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_COMPLETE, "", "");
#endif
	return 0;
}

//! @brief �ݒ�t�@�C���ɏ�������
//! @param fname �t�@�C����
//! @return �����F0�@���s�F1
int Config::SaveFile(const char *fname)
{
	if( fname == NULL ){ return 1; }

	FILE *fp;
	char buf;

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_LOAD, "Config File (Save)", fname);
#endif

#ifdef ENABLE_PATH_DELIMITER_SLASH
	//�p�X��؂蕶����ϊ�
	fname = ChangePathDelimiter(fname);
#endif

	//�t�@�C�����J��
	fp = fopen(fname, "wb");
	if( fp == NULL ){ return 1; }

	//�L�[�R�[�h
	for(int i=0; i<TOTAL_ControlKey; i++){
		buf = (char)Keycode[i];
		fwrite(&buf, 1, 1, fp);
	}

	//�}�E�X���x
	buf = (char)MouseSensitivity;
	fwrite(&buf, 1, 1, fp);

	//�t���X�N���[���L��
	if( FullscreenFlag == false ){ buf = 0x00; }
	else{ buf = 0x01; }
	fwrite(&buf, 1, 1, fp);

	//���ʉ��L��
	if( SoundFlag == false ){ buf = 0x00; }
	else{ buf = 0x01; }
	fwrite(&buf, 1, 1, fp);

	//�o���L��
	if( BloodFlag == false ){ buf = 0x00; }
	else{ buf = 0x01; }
	fwrite(&buf, 1, 1, fp);

	//��ʂ̖��邳
	buf = (char)Brightness;
	fwrite(&buf, 1, 1, fp);

	//�}�E�X���]
	if( InvertMouseFlag == false ){ buf = 0x00; }
	else{ buf = 0x01; }
	fwrite(&buf, 1, 1, fp);

	//�t���[���X�L�b�v
	if( FrameskipFlag == false ){ buf = 0x00; }
	else{ buf = 0x01; }
	fwrite(&buf, 1, 1, fp);

	//�ʂ̏Ə����g�p
	if( AnotherGunsightFlag == false ){ buf = 0x00; }
	else{ buf = 0x01; }
	fwrite(&buf, 1, 1, fp);

	//�v���C���[��
	fwrite(PlayerName, 1, MAX_PLAYERNAME, fp);

	//�t�@�C���n���h�������
	fclose(fp);

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_COMPLETE, "", "");
#endif
	return 0;
}

//! @brief �ݒ�l���f�t�H���g�̒l�ŏ�����
void Config::SetDefaultConfig()
{
	Keycode[KEY_TURNUP]			= 0;
	Keycode[KEY_TURNDOWN]		= 1;
	Keycode[KEY_TURNLEFT]		= 2;
	Keycode[KEY_TURNRIGHT]		= 3;
	Keycode[KEY_MOVEFORWARD]	= 54;
	Keycode[KEY_MOVEBACKWARD]	= 50;
	Keycode[KEY_MOVELEFT]		= 32;
	Keycode[KEY_MOVERIGHT]		= 35;
	Keycode[KEY_WALK]			= 16;
	Keycode[KEY_JUMP]			= 17;
	Keycode[KEY_RELOAD]			= 49;
	Keycode[KEY_DROPWEAPON]		= 38;
	Keycode[KEY_ZOOMSCOPE]		= 20;
	Keycode[KEY_SHOTMODE]		= 55;
	Keycode[KEY_SWITCHWEAPON]	= 57;
	Keycode[KEY_WEAPON1]		= 23;
	Keycode[KEY_WEAPON2]		= 24;
	Keycode[KEY_SHOT]			= 18;
	MouseSensitivity = 25;
	FullscreenFlag = true;
	SoundFlag = true;
	BloodFlag = true;
	Brightness = 4;
	InvertMouseFlag = false;
	FrameskipFlag = false;
	AnotherGunsightFlag = false;
	strcpy(PlayerName, "xopsplayer");
}

//! @brief �I���W�i���L�[�R�[�h���擾
//! @param id �萔
//! @return �I���W�i���L�[�R�[�h
int Config::GetKeycode(int id)
{
	if( (id < 0)||(TOTAL_ControlKey <= id) ){ return 0; }

	return Keycode[id];
}

//! @brief �I���W�i���L�[�R�[�h��ݒ�
//! @param id �萔
//! @param key �I���W�i���L�[�R�[�h
void Config::SetKeycode(int id, int key)
{
	if( (id < 0)||(TOTAL_ControlKey <= id) ){ return; }

	Keycode[id] = key;
}

//! @brief �}�E�X���x�擾
//! @return ���̒l
int Config::GetMouseSensitivity()
{
	return MouseSensitivity;
}

//! @brief �}�E�X���x�ݒ�
//! @param param ���̒l
void Config::SetMouseSensitivity(int param)
{
	MouseSensitivity = param;
}

//! @brief ��ʕ\�����[�h�擾
//! @return �E�B���h�E�Ffalse�@�t���X�N���[���Ftrue
bool Config::GetFullscreenFlag()
{
	return FullscreenFlag;
}

//! @brief ��ʕ\�����[�h�ݒ�
//! @param flag �t���O
void Config::SetFullscreenFlag(bool flag)
{
	FullscreenFlag = flag;
}

//! @brief ���ʉ��ݒ�擾
//! @return �����Ffalse�@�L���Ftrue
bool Config::GetSoundFlag()
{
	return SoundFlag;
}

//! @brief ���ʉ��ݒ�
//! @param flag �t���O
void Config::SetSoundFlag(bool flag)
{
	SoundFlag = flag;
}

//! @brief �o���ݒ�擾
//! @return �����Ffalse�@�L���Ftrue
bool Config::GetBloodFlag()
{
	return BloodFlag;
}

//! @brief �o���ݒ�
//! @param flag �t���O
void Config::SetBloodFlag(bool flag)
{
	BloodFlag = flag;
}

//! @brief ��ʂ̖��邳�ݒ�擾
//! @return ���̒l
int Config::GetBrightness()
{
	return Brightness;
}

//! @brief ��ʂ̖��邳�ݒ�
//! @param param ���̒l
void Config::SetBrightness(int param)
{
	Brightness = param;
}


//! @brief �}�E�X���]�ݒ�擾
//! @return �����Ffalse�@�L���Ftrue
bool Config::GetInvertMouseFlag()
{
	return InvertMouseFlag;
}

//! @brief �}�E�X���]�ݒ�
//! @param flag �t���O
void Config::SetInvertMouseFlag(bool flag)
{
	InvertMouseFlag = flag;
}

//! @brief �t���[���X�L�b�v�ݒ�擾
//! @return �����Ffalse�@�L���Ftrue
bool Config::GetFrameskipFlag()
{
	return FrameskipFlag;
}

//! @brief �t���[���X�L�b�v�ݒ�
//! @param flag �t���O
void Config::SetFrameskipFlag(bool flag)
{
	FrameskipFlag = flag;
}

//! @brief �ʂ̏Ə����g�p�ݒ�擾
//! @return �����Ffalse�@�L���Ftrue
bool Config::GetAnotherGunsightFlag()
{
	return AnotherGunsightFlag;
}

//! @brief �ʂ̏Ə����g�p�ݒ�
//! @param flag �t���O
void Config::SetAnotherGunsightFlag(bool flag)
{
	AnotherGunsightFlag = flag;
}

//! @brief �v���C���[���擾
//! @param out_str �󂯎�镶����^�|�C���^
//! @return �v���C���[��������
int Config::GetPlayerName(char *out_str)
{
	if( out_str == NULL ){ return 0; }

	//�|�C���^�ɃR�s�[����
	strcpy(out_str, PlayerName);

	//��������Ԃ�
	return (int)strlen(PlayerName);
}

//! @brief �v���C���[���ݒ�
//! @param in_str �v���C���[��
void Config::SetPlayerName(char *in_str)
{
	if( in_str == NULL ){ return; }

	if( strlen(in_str) > (MAX_PLAYERNAME - 1) ){
		in_str[MAX_PLAYERNAME - 1] = 0x00;
	}

	//��������R�s�[����
	strcpy(PlayerName, in_str);
}

//! @brief �I���W�i���L�[�R�[�h�̖��O���擾
//! @param code �I���W�i���L�[�R�[�h
//! @return �L�[�̖��O
const char* Config::GetOriginalkeycodeString(int code)
{
	const char *out;

	switch(code){
		case 0x00: out = "UP"; break;
		case 0x01: out = "DOWN"; break;
		case 0x02: out = "LEFT"; break;
		case 0x03: out = "RIGHT"; break;
		case 0x04: out = "NUM0"; break;
		case 0x05: out = "NUM1"; break;
		case 0x06: out = "NUM2"; break;
		case 0x07: out = "NUM3"; break;
		case 0x08: out = "NUM4"; break;
		case 0x09: out = "NUM5"; break;
		case 0x0A: out = "NUM6"; break;
		case 0x0B: out = "NUM7"; break;
		case 0x0C: out = "NUM8"; break;
		case 0x0D: out = "NUM9"; break;
		case 0x0E: out = "BACKSPACE"; break;
		case 0x0F: out = "ENTER"; break;

		case 0x10: out = "TAB"; break;
		case 0x11: out = "SPACE"; break;
		case 0x12: out = "MOUSE L"; break;
		case 0x13: out = "MOUSE R"; break;
		case 0x14: out = "SHIFT"; break;
		case 0x15: out = "CTRL"; break;
		case 0x16: out = "0"; break;
		case 0x17: out = "1"; break;
		case 0x18: out = "2"; break;
		case 0x19: out = "3"; break;
		case 0x1A: out = "4"; break;
		case 0x1B: out = "5"; break;
		case 0x1C: out = "6"; break;
		case 0x1D: out = "7"; break;
		case 0x1E: out = "8"; break;
		case 0x1F: out = "9"; break;

		case 0x20: out = "A"; break;
		case 0x21: out = "B"; break;
		case 0x22: out = "C"; break;
		case 0x23: out = "D"; break;
		case 0x24: out = "E"; break;
		case 0x25: out = "F"; break;
		case 0x26: out = "G"; break;
		case 0x27: out = "H"; break;
		case 0x28: out = "I"; break;
		case 0x29: out = "J"; break;
		case 0x2A: out = "K"; break;
		case 0x2B: out = "L"; break;
		case 0x2C: out = "M"; break;
		case 0x2D: out = "N"; break;
		case 0x2E: out = "O"; break;
		case 0x2F: out = "P"; break;

		case 0x30: out = "Q"; break;
		case 0x31: out = "R"; break;
		case 0x32: out = "S"; break;
		case 0x33: out = "T"; break;
		case 0x34: out = "U"; break;
		case 0x35: out = "V"; break;
		case 0x36: out = "W"; break;
		case 0x37: out = "X"; break;
		case 0x38: out = "Y"; break;
		case 0x39: out = "Z"; break;
		case 0x3A: out = "/"; break;
		case 0x3B: out = ":"; break;
		case 0x3C: out = ";"; break;
		case 0x3D: out = "-"; break;
		case 0x3E: out = "@"; break;
		case 0x3F: out = "["; break;

		case 0x40: out = "]"; break;
		case 0x41: out = "BACKSLASH"; break;
		case 0x42: out = "\\"; break;
		case 0x43: out = ","; break;
		case 0x44: out = "."; break;
		case 0x45: out = "^"; break;
		case 0x46: out = "NUM*" ; break;
		case 0x47: out = "NUM/"; break;
		case 0x48: out = "NUM+"; break;
		case 0x49: out = "NUM-"; break;
		case 0x4A: out = "NUM."; break;

		default : out = "NULL";
	}

	return out;
}


//! @brief �ǉ��ݒ�t�@�C���𐶐�����
//! @param fname �t�@�C����
//! @return �����Ftrue�@���s/�s�v�Ffalse
bool Config::CreateExtConfig(const char *fname)
{
#ifdef ENABLE_ADDCONFIG
	if( fname == NULL ){ return false; }

#ifdef ENABLE_PATH_DELIMITER_SLASH
	//�p�X��؂蕶����ϊ�
	fname = ChangePathDelimiter(fname);
#endif

	FILE *fp;

	//�t�@�C�������݂���Ȃ玸�s�i�t�@�C�������s�v�j
	fp = fopen(fname, "r");
	if( fp != NULL ){
		fclose(fp);
		return false;
	}

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_LOAD, "Config File (Create .ini)", fname);
#endif

	fp = fopen(fname, "w");
	if( fp == NULL ){ return false; }

	fprintf(fp, "[Game]\n");
	fprintf(fp, ";Game language.\n");
	fprintf(fp, ";  0: English + Japanese  (default)\n");
	fprintf(fp, ";  1: English + Korean\n");
	fprintf(fp, "Language = 0\n");
	fprintf(fp, "\n");
	fprintf(fp, ";Show player arm and weapon.\n");
	fprintf(fp, ";  0: OFF (default)\n");
	fprintf(fp, ";  1: ON\n");
	fprintf(fp, "ShowArm = 0\n");
	fprintf(fp, "\n");
	fprintf(fp, "\n");

	fprintf(fp, "[Graphics]\n");
	fprintf(fp, ";WindowSize\n");
	fprintf(fp, ";  0: 640x480  [4:3] (default)\n");
	fprintf(fp, ";  1: 800x600  [4:3]\n");
	fprintf(fp, ";  2:1024x768  [4:3]\n");
	fprintf(fp, ";  3:1280x960  [4:3]\n");
	fprintf(fp, ";  4:1600x1200 [4:3]\n");
	fprintf(fp, "; 10:1280x720  [16:9]\n");
	fprintf(fp, "; 11:1600x900  [16:9]\n");
	fprintf(fp, "; 12:1920x1080 [16:9]\n");
	//fprintf(fp, "; 13:2560x1440 [16:9]\n");
	//fprintf(fp, "; 14:3840x2160 [16:9]\n");
	fprintf(fp, "WindowSize = 0\n");
	fprintf(fp, "\n");
	fprintf(fp, "\n");

	fprintf(fp, "[Input]\n");
	fprintf(fp, ";Limit mouse input control.\n");
	fprintf(fp, ";  0: OFF\n");
	fprintf(fp, ";  1: ON (default)\n");
	fprintf(fp, "MouseLimit = 1\n");
	fprintf(fp, "\n");
	fprintf(fp, "\n");

	fprintf(fp, "[Sounds]\n");
	fprintf(fp, ";Game sound volume. (0.0 ~ 1.0)\n");
	fprintf(fp, "MasterVolume = 1.0\n");
	fprintf(fp, "\n");

	fclose(fp);

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_COMPLETE, "", "");
#endif
	return true;
#else
	return false;
#endif
}

//! @brief �ǉ��ݒ�t�@�C����ǂݍ���
//! @param fname �t�@�C����
//! @return �����F0�@���s�F1
int Config::LoadExtFile(const char *fname)
{
#ifdef ENABLE_ADDCONFIG
	if( fname == NULL ){ return 1; }

	INIFileInterface INIConfig;
	int mode;
	
#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_LOAD, "Config File (.ini)", fname);
#endif

#ifdef ENABLE_PATH_DELIMITER_SLASH
	//�p�X��؂蕶����ϊ�
	fname = ChangePathDelimiter(fname);
#endif

	INIConfig.LoadINIFile(fname);

	//�\������擾
	Language = INIConfig.GetINIFileInt("Game", "Language", 0, NULL);

	//�r�\���t���O�擾
	mode = INIConfig.GetINIFileInt("Game", "ShowArm", 0, NULL);
	if( mode == 0 ){ ShowArmFlag = false; }
	else{ ShowArmFlag = true; }

	//�𑜓x�擾
	mode = INIConfig.GetINIFileInt("Graphics", "WindowSize", 0, NULL);
	switch(mode){
		case  0: ScreenWidth =  640; ScreenHeight =  480; break;
		case  1: ScreenWidth =  800; ScreenHeight =  600; break;
		case  2: ScreenWidth = 1024; ScreenHeight =  768; break;
		case  3: ScreenWidth = 1280; ScreenHeight =  960; break;
		case  4: ScreenWidth = 1600; ScreenHeight = 1200; break;
		case 10: ScreenWidth = 1280; ScreenHeight =  720; break;
		case 11: ScreenWidth = 1600; ScreenHeight =  900; break;
		case 12: ScreenWidth = 1920; ScreenHeight = 1080; break;
		case 13: ScreenWidth = 2560; ScreenHeight = 1440; break;
		case 14: ScreenWidth = 3840; ScreenHeight = 2160; break;
		default: ScreenWidth = DEFAULT_SCREEN_WIDTH; ScreenHeight = DEFAULT_SCREEN_HEIGHT; break;
	}

	//�}�E�X�ړ��ʐ����t���O
	mode = INIConfig.GetINIFileInt("Input", "MouseLimit", 1, NULL);
	if( mode == 0 ){ MouseLimitFlag = false; }
	else{ MouseLimitFlag = true; }

	//���ʎ擾
	mastervolume = INIConfig.GetINIFileFloat("Sounds", "MasterVolume", 1.0f, NULL);
	if( mastervolume < 0.0f ){ mastervolume = 0.0f; }
	if( mastervolume > 1.0f ){ mastervolume = 1.0f; }

	INIConfig.ReleaseINIFile();

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_COMPLETE, "", "");
#endif

#else
	ShowArmFlag = false;
	ScreenWidth = DEFAULT_SCREEN_WIDTH;
	ScreenHeight = DEFAULT_SCREEN_HEIGHT;
	mastervolume = 1.0f;
#endif
	return 0;
}

//! @brief �ǉ��ݒ�l���f�t�H���g�̒l�ŏ�����
void Config::SetDefaultExtConfig()
{
	Language = 0;
	ShowArmFlag = false;
	ScreenWidth = DEFAULT_SCREEN_WIDTH;
	ScreenHeight = DEFAULT_SCREEN_HEIGHT;
	MouseLimitFlag = true;
	mastervolume = 1.0f;
}

//! @brief ����ݒ���擾
//! @return �l
int Config::GetLanguage()
{
	return Language;
}

//! @brief �r�\���t���O���擾
//! @return �t���O
bool Config::GetShowArmFlag()
{
	return ShowArmFlag;
}

//! @brief ��ʉ𑜓x�i���j���擾
//! @return �l
int Config::GetScreenWidth()
{
	return ScreenWidth;
}

//! @brief ��ʉ𑜓x�i�����j���擾
//! @return �l
int Config::GetScreenHeight()
{
	return ScreenHeight;
}

//! @brief �}�E�X�ړ��ʐ����t���O
//! @return �l
bool Config::GetMouseLimitFlag()
{
	return MouseLimitFlag;
}

//! @brief ���ʐݒ���擾
//! @return �l
float Config::GetVolume()
{
	return mastervolume;
}
