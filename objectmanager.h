//! @file objectmanager.h
//! @brief ObjectManager�N���X�̐錾

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

#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#define MAX_HUMAN 96			//!< �ő�l���@�y�d�v�z�FAI���x�������̊֌W��e24�̔{���f�ɂ��邱�ƁI
#define MAX_WEAPON 200			//!< ����̍ő吔
#define MAX_SMALLOBJECT 40		//!< �����̍ő吔

#define MAX_BULLET 128			//!< �e�e�̍ő吔
#define MAX_GRENADE 32			//!< ��֒e�̍ő吔
#define MAX_EFFECT 256			//!< �G�t�F�N�g�̍ő吔

#define WEAPONSHOT_HEIGHT (VIEW_HEIGHT)		//!< �e�𔭎˂��鍂��

#define TOTAL_WEAPON_AUTOBULLET 3			//!< ���������Ɏ����I�ɕ�Ă񂳂��e���i���e���̉��{���j

#define HUMAN_PICKUPWEAPON_L -2.0f			//!< ������E�������i���j
#define HUMAN_PICKUPWEAPON_H 16.0f			//!< ������E�������i��j
#define HUMAN_PICKUPWEAPON_R 3.8f			//!< ������E������

#define HUMAN_HUMANCOLLISION_R 2.5f			//!< �l���m�̓����蔻��̔��a

#define HUMAN_BULLETCOLLISION_HEAD_H 2.9f	//!< ���̓����蔻��̍���
#define HUMAN_BULLETCOLLISION_HEAD_R 1.5f	//!< ���̓����蔻��̔��a
#define HUMAN_BULLETCOLLISION_UP_H 7.0f		//!< �㔼�g�̓����蔻��̍���
#define HUMAN_BULLETCOLLISION_UP_R 2.4f		//!< �㔼�g�̓����蔻��̔��a
#define HUMAN_BULLETCOLLISION_LEG_H 10.5f	//!< ���̓����蔻��̍���
#define HUMAN_BULLETCOLLISION_LEG_R 2.6f	//!< ���̓����蔻��̔��a

#define SMALLOBJECT_COLLISIONSCALE 0.13f	//!< ���������蔻��̔{��

#define MAX_OBJECTMANAGER_LOGLEN 3						//!< �I�u�W�F�N�g���O�̍ő�s��
#define MAX_OBJECTMANAGER_LOGLINES 64					//!< �I�u�W�F�N�g���O�̍ő啶����
#define MAX_OBJECTMANAGER_LOGCNT (int)(5.0f*GAMEFPS)	//!< �I�u�W�F�N�g���O�̕\���J�E���g��

#ifndef H_LAYERLEVEL
 #define H_LAYERLEVEL 3		//!< Select include file.
#endif
#include "main.h"

//! �G�t�F�N�g�`��v�Z�p�\����
typedef struct
{
	int id;		//!< �f�[�^�ԍ�
	float dist;	//!< ����
} effectdata;

//! @brief �I�u�W�F�N�g�Ǘ��N���X
//! @details �e�I�u�W�F�N�g�̏������E�v�Z�E�`��Ȃǂ��s���Ǘ����܂��B
class ObjectManager
{
	class human *HumanIndex;					//!< �l�I�u�W�F�N�g
	class weapon *WeaponIndex;					//!< ����I�u�W�F�N�g
	class smallobject *SmallObjectIndex;		//!< �����I�u�W�F�N�g
	class bullet *BulletIndex;					//!< �e�I�u�W�F�N�g
	class grenade *GrenadeIndex;				//!< ��֒e�I�u�W�F�N�g
	class effect *EffectIndex;					//!< �G�t�F�N�g�I�u�W�F�N�g

	unsigned int framecnt;		//!< �t���[�����̃J�E���g

	float *Human_ontarget;	//!< ������
	int *Human_kill;		//!< �|�����G�̐�
	int *Human_headshot;	//!< �G�̓����ɖ���������
	bool *Human_ShotFlag;	//!< ���C�t���O�i�}�Y���t���b�V���p�j
	int *Human_YakkyouCnt;	//!< ���C���̖�䰃J�E���g
	class BulletObjectHumanIndex *BulletObj_HumanIndex;		//!< �e�ΐl����p�I�u�W�F�N�g

	bool AddCollisionFlag;	//!< �ǉ��̓����蔻��
	bool *FriendlyFire;		//!< FF�i���m�����j�L����

	int Player_HumanID;		//!< �v���C���[�����삷��l�I�u�W�F�N�g��ID

	bool *Human_FrameTextureRefresh;	//!< �l�̃e�N�X�`�����t���b�V���t���O�i�e�N�X�`���U���l�ԁi��l�ԁj�p�j

	ParameterInfo *GameParamInfo;		//!< �Q�[���̐ݒ�l
	D3DGraphics *d3dg;					//!< �`��N���X
	ResourceManager *Resource;			//!< ���\�[�X�Ǘ�
	BlockDataInterface *BlockData;		//!< �u���b�N�f�[�^�Ǘ��N���X
	PointDataInterface *PointData;		//!< �|�C���g�f�[�^�Ǘ��N���X
	Collision *CollD;					//!< �����蔻��Ǘ��N���X
	SoundManager *GameSound;			//!< �Q�[�����ʉ��Ǘ��N���X
	MIFInterface *MIFdata;				//!< MIF�R���g���[���N���X

	class ObjectManagerLog *ObjectLog;	//!< �I�u�W�F�N�g���O�N���X

	int AddHumanIndex(pointdata data, pointdata infodata, bool RandomFlag);
	int AddWeaponIndex(pointdata data);
	int AddSmallObjectIndex(pointdata data);
	void SetHumanBlood(float x, float y, float z, int damage, bool flowing);
	bool CollideHuman(human *in_humanA, human *in_humanB);
	bool CollideBullet(bullet *in_bullet);
	void HitBulletMap(float x, float y, float z, int teamID);
	void HitBulletHuman(int HitHuman_id, int Hit_id, float x, float y, float z, float brx, int attacks, int humanid, float ontargetcnt);
	void HitBulletSmallObject(int HitSmallObject_id, float x, float y, float z, int attacks, int teamID);
	bool GrenadeExplosion(grenade *in_grenade);
	void DeadEffect(human *in_human);
	bool CollideBlood(effect *in_effect, int *id, int *face, float *pos_x, float *pos_y, float *pos_z);
	void PickupWeapon(human *in_human, weapon *in_weapon);
	void CleanupPointDataToObject();
	int SortEffect(float camera_x, float camera_y, float camera_z, effectdata data[]);
	void ShotWeaponEffect(int humanid);
	void ShotWeaponYakkyou(int humanid);

public:
	ObjectManager();
	~ObjectManager();
	void SetClass(ParameterInfo *in_GameParamInfo, D3DGraphics *in_d3dg, ResourceManager *in_Resource, BlockDataInterface *in_BlockData, PointDataInterface *in_PointData, Collision *in_CollD, SoundManager *in_GameSound, MIFInterface *in_MIFdata);
	int AddHumanIndex(float px, float py, float pz, float rx, int CharacterID, int TeamID, int WeaponID[], signed short int param4, bool RandomFlag);
	int AddVisualWeaponIndex(int WeaponID, bool loadbullet);
	int AddSmallObjectIndex(float px, float py, float pz, float rx, int paramID, bool MapColl);
	int AddEffect(float pos_x, float pos_y, float pos_z, float move_x, float move_y, float move_z, float addmove_y, float rotation, float addrotation, float size, float addsize, int texture, int brightness, int addbrightness, float alpha, float addalpha, int settype, int count);
	int AddMapEffect(int id, int face, float pos_x, float pos_y, float pos_z, float size, float rotation, int texture, int brightness, int addbrightness, float alpha, float addalpha, int count);
	void LoadPointData();
	void Recovery();
	void SetAddCollisionFlag(bool flag);
	bool GetFriendlyFireFlag(int id);
	void SetFriendlyFireFlag(int id, bool flag);
	int GetPlayerID();
	void SetPlayerID(int id);
	human* GetHumanObject(int id);
	human* GetPlayerHumanObject();
	int GetHumanObjectID(human* object);
	weapon* GetWeaponObject(int id);
	smallobject* GetSmallObject(int id);
	bullet* GetBulletObject(int id);
	int GetBulletObjectID(bullet* object);
	bullet* GetNewBulletObject();
	grenade* GetNewGrenadeObject();
	human* SearchHuman(signed short int p4);
	smallobject* SearchSmallobject(signed short int p4);
	void GetTotalObjects(int *HumanCnt, int *WeaponCnt, int *SmallobjectCnt, int *BulletCnt, int *GrenadeCnt, int *EffectCnt);
	void MoveForward(int human_id);
	void MoveBack(int human_id);
	void MoveLeft(int human_id);
	void MoveRight(int human_id);
	void MoveWalk(int human_id);
	void MoveJump(int human_id);
	int ShotWeapon(int human_id);
	bool ReloadWeapon(int human_id);
	void ChangeHaveWeapon(int human_id, int id = -1);
	int ChangeWeaponID(int human_id);
	bool DumpWeapon(int human_id);
	void ChangeScopeMode(int human_id);
	bool CheatAddBullet(int human_id);
	bool CheatNewWeapon(int human_id, int new_weaponID);
	bool CheckZombieAttack(human* MyHuman, human* EnemyHuman);
	void HitZombieAttack(human* MyHuman, human* EnemyHuman);
	bool HumanResuscitation(int id);
	int CheckGameOverorComplete();
	bool GetObjectInfoTag(float camera_x, float camera_y, float camera_z, float camera_rx, float camera_ry, int *color, char *infostr);
	int Process(int cmdF5id, bool demomode, bool screen);
	bool GetHumanShotInfo(int id, float *ontarget, int *kill, int *headshot);
	void Render(float camera_x, float camera_y, float camera_z, float camera_rx, float camera_ry, int HidePlayer, bool NoModel);
	void RenderLog(int x, int y);
	void Cleanup();
};

//! @brief �e�ΐl����p�N���X
//! @details �e�Ɛl�̓����蔻��̗L�����Ǘ�����N���X�ł��BObjectManager�N���X���Ŏg�p���܂��B
class BulletObjectHumanIndex
{
	bool *HumanIndex;		//!< ���X�g

public:
	BulletObjectHumanIndex();
	~BulletObjectHumanIndex();
	void Init();
	bool GetIndexFlag(int id);
	void SetIndexFlag(int id);
};

//! @brief �I�u�W�F�N�g���O�N���X
//! @details �I�u�W�F�N�g�Ǘ��N���X ObjectManager �̃��O���L�^�E�\�����܂��B
class ObjectManagerLog
{
	D3DGraphics *d3dg;							//!< �`��N���X

	int TextCnt[MAX_OBJECTMANAGER_LOGLEN];		//!< �t���[����
	char *TextStr[MAX_OBJECTMANAGER_LOGLEN];	//!< ������
	int TextColor[MAX_OBJECTMANAGER_LOGLEN];	//!< ������̐F

	bool AddTextLog(int cnt, const char *str, int color);

public:
	ObjectManagerLog();
	~ObjectManagerLog();
	void SetClass(D3DGraphics *in_d3dg);
	void ClearLog();
	void InfoLog(const char *str);
	void AddHuman(int humanID, int TeamID, int PlayerTeamID);
	void DiedHuman(int ShothumanID, int HitHumanID, int ShothumanTeamID, int HitHumanTeamID, int PlayerTeamID);
	void ReviveHuman(int humanID, int TeamID, int PlayerTeamID);
	void AddSmallObject(int objID);
	void BreakSmallObject(int objID);
	void Process();
	void Render(int x, int y);
};

#endif