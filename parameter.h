//! @file parameter.h
//! @brief ParameterInfo�N���X�̐錾

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

// �l�E����E���� �ݒ�f�[�^�̊m�F���
// �@Manda����@�i2014�N12���j

#ifndef PARAMETER_H
#define PARAMETER_H

#define TOTAL_PARAMETERINFO_HUMAN 43			//!< �l�̐ݒ萔
#define TOTAL_HUMANTEXTURE 30					//!< �l�̃e�N�X�`���̍��v����
#define TOTAL_PARAMETERINFO_WEAPON 23			//!< ����̐ݒ萔
#define TOTAL_PARAMETERINFO_SMALLOBJECT 12	//!< �����̐ݒ萔
#define TOTAL_PARAMETERINFO_BULLET 2		//!< �e�e�I�u�W�F�N�g�̐ݒ萔
#define TOTAL_OFFICIALMISSION 58			//!< �W���~�b�V�����̐�
#define TOTAL_PARAMETERINFO_AILEVEL 6		//!< AI���x���̐ݒ萔
#define TOTAL_PARAMETERINFO_SCOPE 4			//!< �X�R�[�v�̐ݒ萔

#define ID_WEAPON_NONE 0		//!< ����F�Ȃ� �Ƃ��Ď�舵������ԍ�
#define ID_WEAPON_GRENADE 13	//!< ��֒e�Ƃ��Ď�舵������ԍ�
#define ID_WEAPON_CASE 15		//!< �P�[�X�Ƃ��Ď�舵������ԍ�

#define OFFICIALMISSION_BD1 "temp.bd1"		//!< �W���~�b�V������BD1�t�@�C����

#define SMALLOBJECT_SCALE 5.0f				//!< �����̕`��{��

#ifndef H_LAYERLEVEL
 #define H_LAYERLEVEL 1		//!< Select include file.
#endif
#include "main.h"

//! �l�ݒ�p�̍\����
typedef struct
{
	int texture;		//!< �e�N�X�`���ԍ�
	int model;			//!< ���f���ԍ�
	int hp;				//!< �̗�
	int AIlevel;		//!< AI���x��
	int AItrackability;	//!< AI�U�����̒Ǐ]���i�]���r�̂݁j
	int Weapon[TOTAL_HAVEWEAPON];	//!< ����
	int type;			//!< ���
} HumanParameter;

//! ����ݒ�p�̍\����
typedef struct
{
	const char *name;		//!< ���햼
	const char *model;		//!< ���f���t�@�C����
	const char *texture;	//!< �e�N�X�`���t�@�C����
	int attacks;		//!< �U����
	int penetration;	//!< �ђʗ�
	int blazings;		//!< �A�ˊԊu
	int speed;			//!< �e��
	int nbsmax;			//!< ���e��
	int reloads;		//!< �����[�h����
	int reaction;		//!< �����p�x
	int ErrorRangeMIN;	//!< �e���덷 �ŏ�
	int ErrorRangeMAX;	//!< �e���덷 �ő�
	float mx;		//!< �e�̈ʒu�FX
	float my;		//!< �e�̈ʒu�FY
	float mz;		//!< �e�̈ʒu�FZ
	float flashx;	//!< �}�Y���t���b�V���̈ʒu�FX
	float flashy;	//!< �}�Y���t���b�V���̈ʒu�FY
	float flashz;	//!< �}�Y���t���b�V���̈ʒu�FZ
	float yakkyou_px;	//!< ��䰔r�o �ʒu�FX
	float yakkyou_py;	//!< ��䰔r�o �ʒu�FY
	float yakkyou_pz;	//!< ��䰔r�o �ʒu�FZ
	float yakkyou_sx;	//!< ��䰔r�o �����FX
	float yakkyou_sy;	//!< ��䰔r�o �����FY
	float yakkyou_size;	//!< ��� �`��{���i�������ōU���͂���Z�o�j
	int yakkyou_delay;	//!< ��䰔r�o �x���J�E���g
	int burstcnt;		//!< �A�˒e���i0�F�t���I�[�g�@1�F�Z�~�I�[�g�@2�ȏ�F�o�[�X�g���[�h�j
	int scopemode;		//!< �X�R�[�v���[�h
	float size;			//!< �`��{��
	int soundid;		//!< ���C���̔ԍ�
	int soundvolume;	//!< ���C���̉���
	bool silencer;		//!< �T�C�����T�[�̗L��
	int WeaponP;		//!< �������@
	int ChangeWeaponID;		//!< �؂�ւ��镐��Ώ�
	int ChangeWeaponCnt;	//!< ����؂�ւ�����
	int pellet;			//!< ���˒e���i�y���b�g���j
} WeaponParameter;

//! �����ݒ�p�̍\����
typedef struct
{
	const char *model;		//!< ���f����
	const char *texture;	//!< �e�N�X�`����
	int decide;		//!< �����蔻��̑傫��
	int hp;			//!< �ϋv��
	int sound;		//!< �������̌��ʉ��ԍ�
	int jump;		//!< ��ы
} SmallObjectParameter;

//! �e�e�I�u�W�F�N�g�p�̍\����
typedef struct
{
	const char *model;		//!< ���f����
	const char *texture;	//!< �e�N�X�`����
	float size;				//!< �`��{��
} BulletParameter;

//! �W���~�b�V�����ݒ�p�̍\����
typedef struct
{
	const char *name;				//!< �~�b�V�������ʖ�
	const char *fullname;			//!< �~�b�V������������
	const char *directory;			//!< �f�[�^�f�B���N�g��
	const char *txt;				//!< �t�@�C�����i.pd1/.txt�j
	bool collision;			//!< �ǉ��̓����蔻��������t���O
	bool screen;			//!< ��ʂ��Â�����t���O
} OfficialMissionParameter;

//! AI���x���ɂ��ݒ�i���\�l�j�p�\����
typedef struct
{
	int aiming;			//!< �G�C�~���O�\��
	int attack;			//!< �U����
	int search;			//!< �����\��
	int limitserror;	//!< ���e�덷
} AIParameter;

//! �X�R�[�v�ݒ�p�̍\����
typedef struct
{
	float ViewAngle;				//!< ����p
	float MouseRange;				//!< ���슴�x
	int ScopeGunsight;				//!< �X�R�[�v�g�p���̏Ə��\���i0�F�ʏ�A1�F�ȈՁA2�F�����j
	bool NoScopeGunsight;			//!< �ʏ펞�̏Ə��\��
	int WeaponNoScopeReaction;		//!< �ʏ픭�C���̔���
	float WeaponMotionReactionY;	//!< ���C���̔����i���[�V�����j
	bool WeaponRecoil_ScopeOnly;	//!< �X�R�[�v ���C���̔��� �X�R�[�v���̂�
	float WeaponRecoil_ScopeRx;		//!< �X�R�[�v ���C���̔��� �����@���x���@ 0.1f����
	float WeaponRecoil_ScopeRyMin;	//!< �X�R�[�v ���C���̔��� �c���ŏ��@���x���@ 0.1f����
	float WeaponRecoil_ScopeRyMax;	//!< �X�R�[�v ���C���̔��� �c���ő�@���x���@ 0.1f����
	float AI_ShotAngle;							//!< AI ���C���ɓG�𑨂���p�x
	float AI_ShotAngleLong;						//!< AI ���C���ɓG�𑨂���p�x�i�������j
	float AI_AddSearchEnemyMaxDist_Normal;		//!< AI �G�������鋗�� ���Z���i�ʏ펞�j
	float AI_AddSearchEnemyMaxDist_Caution;		//!< AI �G�������鋗�� ���Z���i�x�����j
} ScopeParameter;

//! @brief �ݒ肳�ꂽ�l���Ǘ�����N���X
//! @details �\�ߌŒ肳�ꂽ�ݒ�l���Ǘ����܂��B
//! @details ��̓I�ɂ́A�l�E����E��������A�W���~�b�V�����Ɋ֌W����ݒ�l�Ȃǂł��B
class ParameterInfo
{
	HumanParameter *Human;								//!< �l�̏����i�[����|�C���^
	const char *HumanTexturePath[TOTAL_HUMANTEXTURE];	//!< �l�̃e�N�X�`�����i�[����|�C���^
	WeaponParameter *Weapon;							//!< ����̏����i�[����|�C���^
#ifdef ENABLE_BUG_HUMANWEAPON
	WeaponParameter *BugWeapon;							//!< �o�O����̏����i�[����|�C���^
#endif
	SmallObjectParameter *SmallObject;					//!< �����̏����i�[����|�C���^
	BulletParameter *Bullet;							//!< �e�e�I�u�W�F�N�g�̏����i�[����|�C���^
	OfficialMissionParameter *MissionData;				//!< �W���~�b�V�����̏����i�[����|�C���^
	AIParameter *AIlevel;								//!< AI�̐��\�l
	ScopeParameter *Scope;								//!< �X�R�[�v�ݒ���i�[����|�C���^

public:
	ParameterInfo();
	~ParameterInfo();
	void InitInfo();
	void DestroyInfo();
	int GetHuman(int id, HumanParameter *out_data);
	int GetHumanTexturePath(int id, char *out_str);
	int GetWeapon(int id, WeaponParameter *out_data);
#ifdef ENABLE_BUG_HUMANWEAPON
	int GetBugWeapon(int id, WeaponParameter *out_data);
#endif
	int GetSmallObject(int id, SmallObjectParameter *out_data);
	int GetBullet(int id, BulletParameter *out_data);
	int GetOfficialMission(int id, char *name, char *fullname, char* directory, char *txt, bool *collisionflag, bool *screenflag);
	int GetAIlevel(int level, AIParameter **out_AIlevel);
	int GetScopeParam(int id, ScopeParameter *out_data);
};

#endif