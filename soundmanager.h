//! @file soundmanager.h
//! @brief SoundManager�N���X�̐錾

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

#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#define MAX_SOUNDMGR_LIST 100		//!< �T�E���h���X�g�̍ő吔

#define MAX_SOUNDHITMAP 95			//!< �}�b�v���e���̍ő剹��
#define MAX_SOUNDHITSMALLOBJ 110	//!< �������e���̍ő剹��
#define MAX_SOUNDHITHUMAN 75		//!< �l���e�i��e�j���̍ő剹��
#define MAX_SOUNDPASSING 80			//!< �e�����؂鉹�̍ő剹��
#define MAX_SOUNDCCOGRENADE 100		//!< ��֒e �o�E���h�E���˕Ԃ艹�̍ő剹��
#define MAX_SOUNDHITGRENADE 120		//!< ��֒e �������̍ő剹��

#ifndef H_LAYERLEVEL
 #define H_LAYERLEVEL 2		//!< Select include file.
#endif
#include "main.h"

//! �T�E���h���X�g�uparamid�v�p�萔
enum SoundmgrID
{
	SHOT_WEAPON = 0,		//!< ���C��
	SHOT_WEAPON_PLAYER,		//!< �v���C���[���g�̔��C��
	HIT_MAP,				//!< �}�b�v���e��
	HIT_HUMAN_HEAD,			//!< ��e���i���j
	HIT_HUMAN_UP,			//!< ��e���i�㔼�g�j
	HIT_HUMAN_LEG,			//!< ��e���i�����g�j
	HIT_HUMAN_ZOMBIE,		//!< �]���r�U����
	HIT_SMALLOBJECT,		//!< �����j��
	BULLET,					//!< �e�e�̉��E���؂鉹
	GRE_BOUND,				//!< ��֒e �o�E���h��
	GRE_EXPLOSION,			//!< ��֒e ������
	FOOTSTEPS_WALK,			//!< �����E����
	FOOTSTEPS_FORWARD,		//!< �����E���鉹�i�O�i�j
	FOOTSTEPS_BACK,			//!< �����E���鉹�i��ށj
	FOOTSTEPS_SIDE,			//!< �����E���鉹�i���E�j
	FOOTSTEPS_JUMP,			//!< �����E�W�����v
	FOOTSTEPS_LANDING,		//!< �����E���n��
	WEAPON_RELOAD			//!< �����[�h��
};

//! �T�E���h���X�g�p�\����
typedef struct
{
	int paramid;	//!< �����̎�ޔԍ��ESoundmgrID�萔
	int dataid;		//!< �f�[�^�ԍ�
	float x;		//!< X���W
	float y;		//!< Y���W
	float z;		//!< Z���W
	float move_x;	//!< 1�t���[����� X���W�ړ���
	float move_y;	//!< 1�t���[����� Y���W�ړ���
	float move_z;	//!< 1�t���[����� Z���W�ړ���
	int teamid;		//!< �`�[���ԍ�
	int cnt;		//!< �J�E���g�l
} soundlist;

//! @brief �T�E���h�Ǘ��N���X
//! @details �e�T�E���h�̏������E�v�Z�E�Đ��Ȃǂ��s���Ǘ����܂��B
class SoundManager
{
	class SoundControl *SoundCtrl;		//!< �T�E���h�Đ��N���X
	class ResourceManager *Resource;	//!< ���\�[�X�Ǘ��N���X
	class ParameterInfo *Param;			//!< �ݒ�l���Ǘ�����N���X
	soundlist *soundlistA;		//!< �T�E���h���X�g A
	soundlist *soundlistB;		//!< �T�E���h���X�g B
	bool changeAB;				//!< �T�E���h���X�g�̔��]
	int listAdatas;				//!< �T�E���h���X�g A �̓o�^��
	int listBdatas;				//!< �T�E���h���X�g B �̓o�^��

	bool GetNewList(soundlist **plist);
	int GetTargetList(soundlist **plist);
	bool CheckApproach(soundlist *plist, float camera_x, float camera_y, float camera_z, float *min_x, float *min_y, float *min_z);
	void PlaySound(soundlist *plist, float camera_x, float camera_y, float camera_z, int teamID);

public:
	SoundManager(SoundControl *in_SoundCtrl = NULL, ResourceManager *in_Resource = NULL, ParameterInfo *in_Param = NULL);
	~SoundManager();
	void SetClass(SoundControl *in_SoundCtrl, ResourceManager *in_Resource, ParameterInfo *in_Param);
	void InitWorldSound();
	bool ShotWeapon(float x, float y, float z, int id, int teamID, bool player);
	bool HitMap(float x, float y, float z, int teamID);
	bool HitHuman(float x, float y, float z, int Hit_id, int teamID);
	bool HitSmallObject(float x, float y, float z, int id, int teamID);
	bool PassingBullet(float x, float y, float z, float move_x, float move_y, float move_z, int teamID);
	bool GrenadeBound(float x, float y, float z, int teamID);
	bool GrenadeExplosion(float x, float y, float z, int teamID);
	bool SetFootsteps(float x, float y, float z, int id, int MoveMode, int teamID, int cnt);
	bool ReloadWeapon(float x, float y, float z, int teamID);
	int GetTotalSoundList();
	int GetWorldSound(float pos_x, float pos_y, float pos_z, int teamID, soundlist *psoundlist);
	void PlayWorldSound(float camera_x, float camera_y, float camera_z, float camera_rx, int teamID);
};

#endif