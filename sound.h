//! @file sound.h
//! @brief SoundControl�N���X�̐錾

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

#ifndef SOUND2_H
#define SOUND2_H

#define MAX_LOADSOUND 24		//!< �T�E���h��ǂݍ��߂�ő吔
#define MAX_SOUNDLISTS 100		//!< �T�E���h���Đ�����ő吔�iDirectSound�j
#define MAX_SOUNDDIST 335		//!< �T�E���h���Đ�����ő勗��

#ifndef H_LAYERLEVEL
 #define H_LAYERLEVEL 1		//!< Select include file.
#endif
#include "main.h"
#include <windows.h>

#define SOUND_ENGINE 0		//!< @brief �T�E���h�̍Đ����C�u������I�� @details ezds.dll�F0�@DirectSound�i2D�Đ��j�F1�@DirectSound�i3D�Đ��j�F2

#if SOUND_ENGINE == 0
 typedef int (*FARPROCH)(HWND);					//!< DLL Parameter
 typedef int (*FARPROCCI)(const char*, int);	//!< DLL Parameter
 typedef int (*FARPROCIII)(int, int, int);		//!< DLL Parameter
 typedef int (*FARPROCI)(int);					//!< DLL Parameter

 #define SOUND_CORE "ezds"					//!< �o�[�W�����\���p���
#else	//#if SOUND_ENGINE
 #include <dsound.h>
 #include <mmsystem.h>
 #pragma comment(lib, "dsound.lib")
 #pragma comment(lib, "dxguid.lib")
 #pragma comment(lib, "winmm.lib")
 #ifndef WAVE_FORMAT_IEEE_FLOAT
  #define WAVE_FORMAT_IEEE_FLOAT 3
 #endif

 #if SOUND_ENGINE == 1
  #define SOUND_CORE "DirectSound(Monaural)"	//!< �o�[�W�����\���p���
 #else
  #define SOUND_CORE "DirectSound(Stereo)"	//!< �o�[�W�����\���p���
 #endif
#endif	//#if SOUND_ENGINE

//! @brief �T�E���h���Đ�����N���X
//! @details �T�E���h�̓ǂݍ��݂���Đ��܂ł��Ǘ����܂��B
//! @details �Q�l�����Fezds.dll�t�@�C��(ver:0.4)��͏��@http://openxops.net/filesystem-ezds04.php
class SoundControl
{
#if SOUND_ENGINE == 0
	HINSTANCE lib;		//!< DLL�t�@�C���̃C���X�^���X
	FARPROC DSver;		//!< DSver()
	FARPROCH DSinit;	//!< DSinit()
	FARPROC DSend;		//!< DSend()
	FARPROCCI DSload;	//!< DSload()
	FARPROCIII DSplay;	//!< DSplay()
	FARPROCI DSrelease;	//!< DSrelease()
	bool useflag[MAX_LOADSOUND];	//!< �g�p���̃T�E���h�ԍ����Ǘ�����z��

	float camera_x;			//!< �J�������W
	float camera_y;			//!< �J�������W
	float camera_z;			//!< �J�������W
	float camera_rx;		//!< �J����X���p�x
	float mastervolume;		//!< ����(�p�[�Z���g)
	int volume_max;			//!< �ő剹��


	bool CheckSourceDist(float x, float y, float z, bool near, float *out_dist);
	int CalculationVolume(int MaxVolume, float dist, bool near);
#else	//#if SOUND_ENGINE
	LPDIRECTSOUND8 pDSound;					//!< DIRECTSOUND8�̃|�C���^
	LPDIRECTSOUNDBUFFER pDSBuffer[MAX_LOADSOUND][MAX_SOUNDLISTS];	//!< �Z�J���_���[�o�b�t�@�[
	LPDIRECTSOUND3DLISTENER8 p3DListener;	//!< ���X�i�[
	float mastervolume;						//!< ����(�p�[�Z���g)
	float volume_dBper;						//!< �ő剹��

	bool CheckSoundFile(const char* filename, int *filesize, int *fileoffset, WAVEFORMATEX** pwfex);
	int GetDSVolume(int volume);
#if SOUND_ENGINE == 1
	bool CheckSourceDist(float x, float y, float z, bool near, float *out_dist);
	int CalculationVolume(int MaxVolume, float dist, bool near);
#endif
#endif	//#if SOUND_ENGINE

public:
	SoundControl();
	~SoundControl();
	int InitSound(WindowControl *WindowCtrl);
	void DestroySound();
	void SetVolume(float volume);
	void SetCamera(float x, float y, float z, float rx);
	int LoadSound(const char* filename);
	int PlaySound(int id, int volume, int pan);
	int Play3DSound(int id, float x, float y, float z, int volume);
	int GetTotalSounds();
	void CleanupSound(int id);
};

#endif