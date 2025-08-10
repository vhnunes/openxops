//! @file object.h
//! @brief object�N���X�̐錾

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

#ifndef OBJECT_H
#define OBJECT_H

// ���ӁF���x�ύX���́A�����蔻��n�̒萔���v�m�F
#define HUMAN_PROGRESSRUN_ACCELERATION	0.7f			//!< �l�̑O�i�i����j�����x
#define HUMAN_SIDEWAYSRUN_ACCELERATION 0.5f			//!< �l�̉���������x
#define HUMAN_REGRESSRUN_ACCELERATION 0.38f			//!< �l�̌�މ����x
//! �l���΂ߑO�ɐi�މ����x
#define HUMAN_PROGRESSRUN_SIDEWAYSRUN_ACCELERATION ((HUMAN_PROGRESSRUN_ACCELERATION + HUMAN_SIDEWAYSRUN_ACCELERATION) / 2)
//! �l���΂ߌ��ɐi�މ����x
#define HUMAN_REGRESSRUN_SIDEWAYSRUN_ACCELERATION (HUMAN_REGRESSRUN_ACCELERATION)
#define HUMAN_PROGRESSWALK_ACCELERATION	0.35f		//!< �l�����������x
#define HUMAN_ATTENUATION			0.5f		//!< �l��1�t���[��������̌������@�i0.0 < x < 1.0�j

#define HUMAN_JUMP_SPEED 2.2f					//!< �W�����v���鑬�x

#define HUMAN_HEIGHT 20.0f			//!< �l�̍���
#define HUMAN_DAMAGE_HEAD 2.0f		//!< �e�� �� �ɓ��������ꍇ�̃_���[�W�{��
#define HUMAN_DAMAGE_UP 1.0f		//!< �e�� �㔼�g �ɓ��������ꍇ�̃_���[�W�{��
#define HUMAN_DAMAGE_LEG 0.7f		//!< �e�� �����g �ɓ��������ꍇ�̃_���[�W�{��
#define HUMAN_DAMAGE_ZOMBIEU 15		//!< �]���r�̍U�����󂯂��ꍇ�̍Œ�_���[�W
#define HUMAN_DAMAGE_ZOMBIEA 5		//!< �]���r�̍U�����󂯂��ꍇ�̒ǉ��_���[�W��
#define HUMAN_DAMAGE_SPEED 0.066f	//!< ������
#define HUMAN_DAMAGE_MINSPEED -3.8f	//!< �����ɂ��_���[�W���󂯂Ȃ����x
#define HUMAN_DAMAGE_MAXSPEED -7.0f	//!< �ő嗎�����x
#define HUMAN_DAMAGE_MAXFALL 120	//!< �����ɂ��ő�_���[�W
#define MAX_DAMAGE_GRENADE_DISTANCE 80.0f		//!< ��֒e�ɂ��_���[�W���󂯂�ő勗��
#define HUMAN_DAMAGE_GRENADE_HEAD 100			//!< ��֒e�ɂ�� �� �ւ̍ő�_���[�W
#define HUMAN_DAMAGE_GRENADE_LEG 80				//!< ��֒e�ɂ�� �� �ւ̍ő�_���[�W
#define SMALLOBJECT_DAMAGE_GRENADE 80			//!< ��֒e�ɂ�� ���� �ւ̍ő�_���[�W

#define ARMRAD_NOWEAPON DegreeToRadian((-90 + 20))			//!< ��Ԃ玞�̘r�̕`��p�x
#define ARMRAD_RELOADWEAPON DegreeToRadian(-20)				//!< �����[�h���̘r�̕`��p�x

#define HUMAN_MAPCOLLISION_R 2.8f							//!< �l�ƃ}�b�v�̓����蔻��@���a
#define HUMAN_MAPCOLLISION_HEIGHT 10.0f						//!< �l�ƃ}�b�v�̓����蔻��@�����i���F�����x�j
#define HUMAN_MAPCOLLISION_ADD_HEIGHT_A 9.0f				//!< �l�ƃ}�b�v�̓����蔻�� �ǉ����@�����i���F�W���}�b�v SCHOOL �ɍ��킹��j
#define HUMAN_MAPCOLLISION_ADD_HEIGHT_B 13.0f				//!< �l�ƃ}�b�v�̓����蔻�� �ǉ����@�����i���F�W���}�b�v SCHOOL �ɍ��킹��j
#define HUMAN_MAPCOLLISION_SLOPEANGLE DegreeToRadian(50)	//!< �l�ƃ}�b�v�̓����蔻��@�o��Ȃ��Ζʂ̊p�x
#define HUMAN_MAPCOLLISION_CLIMBHEIGHT 3.2f					//!< �l�ƃ}�b�v�̓����蔻��@�o��鍂��
#define HUMAN_MAPCOLLISION_CLIMBADDY 0.4f					//!< �l�ƃ}�b�v�̓����蔻��@1�t���[���œo�鍂��
#define HUMAN_MAPCOLLISION_GROUND_HEIGHT -0.5f				//!< �l�ƃ}�b�v�̓����蔻��@�n�ʔ���@����
#define HUMAN_MAPCOLLISION_GROUND_R1 0.15f					//!< �l�ƃ}�b�v�̓����蔻��@�n�ʔ���@���a�i1�j
#define HUMAN_MAPCOLLISION_GROUND_R2 0.50f					//!< �l�ƃ}�b�v�̓����蔻��@�n�ʔ���@���a�i2�j
#define HUMAN_MAPCOLLISION_GROUND_R3 0.30f					//!< �l�ƃ}�b�v�̓����蔻��@�n�ʔ���@���a�i3�j
#define HUMAN_MAPCOLLISION_CHECK_MAXDIST 12.0f				//!< �l�ƃ}�b�v�̓����蔻��@1�t���[���ňړ�����ő勗��
#define HUMAN_MAPCOLLISION_CHECK_HEIGHT (HUMAN_HEIGHT-3.0f)	//!< �l�ƃ}�b�v�̓����蔻��@�ŏI�I�Ȉړ��ۂ𔻒肷�鍂���i���F�����x�j
#define HUMAN_DEADLINE -100.0f						//!< �l�����S����Y���W�i�f�b�h���C���j
#define HUMAN_DEADADDRY DegreeToRadian(0.75f)		//!< ���̂̓|�������x

#define BULLET_SPEEDSCALE 2.5f			//!< �e���̔{��
#define BULLET_DESTROYFRAME 40			//!< �e�̏��Ńt���[����
#define GRENADE_DESTROYFRAME 100		//!< ��֒e�̔����t���[����

#ifndef H_LAYERLEVEL
 #define H_LAYERLEVEL 2		//!< Select include file.
#endif
#include "main.h"

#define HUMAN_DEADBODY_COLLISION	//!< @brief �l���|���ۂɃ}�b�v�Ɠ����蔻������{���邩 @details �萔�錾�L���F�����蔻������{�@�萔�錾�����i�R�����g���j�F���̂܂ܓ|������

//! @brief �I�u�W�F�N�g�Ǘ��N���X�i���N���X�j
//! @details 3D�ō��W�Ǘ���`����s���I�u�W�F�N�g�S�ʂ��Ǘ�����N���X�Q�̊��N���X�ł��B
class object
{
protected:
	class ParameterInfo *Param;	//!< �ݒ�l���Ǘ�����N���X�ւ̃|�C���^
	float pos_x;		//!< X���W
	float pos_y;		//!< Y���W
	float pos_z;		//!< Z���W
	float rotation_x;	//!< ��]�p�x
	float model_size;	//!< �`��T�C�Y
	int id_parameter;	//!< �f�[�^�̎��
	int id_model;		//!< ���f���̃f�[�^�ԍ�
	int id_texture;		//!< �e�N�X�`���̃f�[�^�ԍ�
	bool DarkModelFlag;	//!< ���f�����Â�����
	bool EnableFlag;	//!< �L�����t���O

public:
	object(class ParameterInfo *in_Param = NULL, float x = 0.0f, float y = 0.0f, float z = 0.0f, float rx = 0.0f, float size = 1.0f, bool flag = true);
	~object();
	virtual void SetParameterInfoClass(class ParameterInfo *in_Param);
	virtual void SetPosData(float x, float y, float z, float rx);
	virtual void GetPosData(float *x, float *y, float *z, float *rx);
	virtual void SetEnableFlag(bool flag);
	virtual bool GetEnableFlag();
	virtual void SetModel(int id, float size);
	virtual void GetModel(int *id, float *size);
	virtual void SetTexture(int id);
	virtual int GetTexture();
	virtual void SetDarkModelFlag(bool flag);
	virtual int ProcessObject();
	virtual void Render(class D3DGraphics *d3dg, bool NoModel);
};

//! @brief �l�Ǘ��N���X
class human : public object
{
protected:
	int point_dataid;			//!< �|�C���g�̃f�[�^�ԍ�
	signed short int point_p4;	//!< �|�C���g�̎��ʔԍ�
	int teamid;					//!< �`�[���ԍ�
	float move_x;				//!< X�����x
	float move_y;				//!< Y���i�����j���x
	float move_z;				//!< Z�����x
	bool move_y_flag;			//!< Y���ړ��t���O
	int move_y_upper;			//!< Y���㏸�֎~�J�E���^�[
	bool move_y_landing;		//!< ���n�t���O
	int underblock_id;			//!< �����̃u���b�NID
	int underblock_face;		//!< �����̃u���b�N�̖ʔԍ�
	float rotation_y;			//!< �S�̂̉�]�p�x
	float armrotation_y;		//!< �r�̉�]�p�x
	float upmodel_size;			//!< �㔼�g�`��T�C�Y
	float armmodel_size;		//!< �r�`��T�C�Y
	float legmodel_size;		//!< ���`��T�C�Y
	class weapon *weapon[TOTAL_HAVEWEAPON];			//!< ����
	int noweapon_bullets[TOTAL_HAVEWEAPON];			//!< ���햢�������̍��v�e��
	int noweapon_loadbullets[TOTAL_HAVEWEAPON];		//!< ���햢�������̑��e��
	int selectweapon;			//!< ����A/B�̑I��
	int selectweaponcnt;		//!< ����̐؂�ւ��i�����ւ��j�J�E���g
	int changeweaponidcnt;		//!< ����̎�ސ؂�ւ��J�E���g
	int weaponshotcnt;			//!< ����̘A�˃J�E���g
	int weaponreloadcnt;		//!< ����̃����[�h�J�E���g
	bool weaponshotframe;		//!< ���C�t���O
	int weaponburstmodecnt;		//!< �o�[�X�g���[�h�J�E���g
	int hp;						//!< �̗�
	int deadstate;				//!< ���̂ɂȂ��Ă��邩
	float add_ry;				//!< ���̂̓|�������x
	float move_rx;				//!< �ړ��p�x
	int MoveFlag;				//!< �ړ�������\���t���O
	int MoveFlag_lt;			//!< �i�O��́j�ړ�������\���t���O
	int scopemode;				//!< �X�R�[�v�g�p���[�h
	bool HitFlag;				//!< ��e��\���t���O
	float Hit_rx;				//!< ��e��������
	float totalmove;			//!< ���v�ړ���
	int StateGunsightErrorRange;		//!< �Ə��̏�Ԍ덷
	int ReactionGunsightErrorRange;		//!< �Ə��̔����덷
	bool Invincible;			//!< ���G�t���O

	class HumanMotionControl *MotionCtrl;	//!< ���[�V�����𐧌䂷��N���X�ւ̃|�C���^

	void GunsightErrorRange();
	int CheckAndProcessDead(class Collision *CollD);
	void ControlProcess();
	void CollisionMap(class Collision *CollD, class BlockDataInterface *inblockdata, bool AddCollisionFlag, bool player);
	bool CollisionBlockScratch(class Collision *CollD, class BlockDataInterface *inblockdata, int blockid, float *px, float *py, float *pz, float px_old, float py_old, float pz_old, float in_vx, float in_vy, float in_vz, int mode);

public:
	human(class ParameterInfo *in_Param = NULL, float x = 0.0f, float y = 0.0f, float z = 0.0f, float rx = 0.0f, int id_param = -1, int dataid = 0, signed short int p4 = 0, int team = 0, bool flag = false);
	~human();
	virtual void SetParameterInfoClass(class ParameterInfo *in_Param);
	virtual void SetParamData(int id_param, int dataid, signed short int p4, int team, bool init);
	virtual void GetParamData(int *id_param, int *dataid, signed short int *p4, int *team);
	virtual void GetMovePos(float *x, float *y, float *z);
	virtual void SetModel(int upmodel, int armmodel[], int legmodel, int walkmodel[], int runmodel[]);
	virtual int GetHP();
	virtual bool SetHP(int in_hp);
	virtual bool GetDeadFlag();
	virtual bool GetDeadMotionFlag();
	virtual void SetTeamID(int id);
	virtual bool GetInvincibleFlag();
	virtual void SetInvincibleFlag(bool flag);
	virtual void SetWeapon(class weapon *in_weapon[], int nlnbs[], int nnbs[]);
	virtual int PickupWeapon(class weapon *in_weapon);
	virtual void ChangeHaveWeapon(int id = -1);
	virtual int GetChangeHaveWeaponCnt();
	virtual int ChangeWeaponID();
	virtual int GetChangeWeaponIDCnt();
	virtual int GetWeaponReloadCnt();
	virtual void GetWeapon(int *out_selectweapon, class weapon *out_weapon[], int nlnbs[], int nnbs[]);
	virtual int GetMainWeaponTypeNO();
	virtual bool GetWeaponBlazingmode();
	virtual bool ShotWeapon(int *weapon_paramid, int *GunsightErrorRange);
	virtual bool ReloadWeapon();
	virtual bool DumpWeapon();
	virtual void SetMoveForward();
	virtual void SetMoveBack();
	virtual void SetMoveLeft();
	virtual void SetMoveRight();
	virtual void SetMoveWalk();
	virtual int GetMovemode(bool nowdata);
	virtual int GetMoveFlag(bool nowdata);
	virtual bool SetEnableScope();
	virtual void SetDisableScope();
	virtual int GetScopeMode();
	virtual void GetRxRy(float *rx, float *ry);
	virtual void SetRxRy(float rx, float ry);
	virtual float GetDeadRy();
	virtual void Jump();
	virtual int GetJumpLanding(bool nowdata);
	virtual void GetUnderBlock(int *id, int *face);
	virtual void AddPosOrder(float rx, float ry, float speed);
	virtual void HitBulletHead(int attacks);
	virtual void HitBulletUp(int attacks);
	virtual void HitBulletLeg(int attacks);
	virtual void HitZombieAttack();
	virtual void HitGrenadeExplosion(int attacks);
	virtual void SetHitFlag(float rx);
	virtual bool CheckHit(float *rx);
	virtual float GetTotalMove();
	virtual int GetMoveMotionCount();
	virtual int ProcessObject(class Collision *CollD, class BlockDataInterface *inblockdata, bool AddCollisionFlag, bool player, bool F5mode);
	virtual int GetGunsightErrorRange();
	virtual void Render(class D3DGraphics *d3dg, class ResourceManager *Resource, bool DrawArmOnly, bool player, bool NoModel);
};

//! @brief ����Ǘ��N���X
class weapon : public object
{
protected:
	float move_x;		//!< X���ړ���
	float move_y;		//!< Y���ړ���
	float move_z;		//!< Z���ړ���
	bool usingflag;		//!< �g�p����\���t���O
	int bullets;		//!< ���v�e��
	int Loadbullets;	//!< ���e��
	bool motionflag;	//!< ���W�ړ�����\���t���O

public:
	weapon(class ParameterInfo *in_Param = NULL, float x = 0.0f, float y = 0.0f, float z = 0.0f, float rx = 0.0f, int id_param = 0, int nbs = 0, bool flag = false);
	~weapon();
	virtual void SetPosData(float x, float y, float z, float rx);
	virtual void SetParamData(int id_param, int lnbs, int nbs, bool init);
	virtual void GetParamData(int *id_param, int *lnbs, int *nbs);
	virtual bool GetUsingFlag();
	virtual int Pickup();
	virtual void Dropoff(float x, float y, float z, float rx, float speed);
	virtual int Shot();
	virtual int StartReload();
	virtual int RunReload();
	virtual bool ResetWeaponParam(class ResourceManager *Resource, int id_param, int lnbs, int nbs);
	virtual int ProcessObject(class Collision *CollD);
	virtual void Render(class D3DGraphics *d3dg, bool NoModel);
};

//! @brief �����Ǘ��N���X
class smallobject : public object
{
protected:
	class MIFInterface *MIFdata;		//!< MIF���Ǘ�����N���X�ւ̃|�C���^
	float rotation_y;			//!< ��]�p�x
	signed short int point_p4;	//!< �|�C���g�̎��ʔԍ�
	int hp;						//!< �̗�
	float jump_rx;				//!< ��΂������p�x
	float move_rx;				//!< ��΂������ړ���
	float add_rx;				//!< ��΂�������]��
	float add_ry;				//!< ��΂��c����]��
	int jump_cnt;				//!< ��΂��㏸�J�E���g

public:
	smallobject(class ParameterInfo *in_Param = NULL, class MIFInterface *in_MIFdata = NULL, float x = 0.0f, float y = 0.0f, float z = 0.0f, float rx = 0.0f, int id_param = 0, signed short int p4 = 0, bool flag = false);
	~smallobject();
	virtual void SetMIFInterfaceClass(class MIFInterface *in_MIFdata);
	virtual void SetParamData(int id_param, signed short int p4, bool init);
	virtual void GetParamData(int *id_param, signed short int *p4);
	virtual int GetHP();
	virtual float CollisionMap(class Collision *CollD);
	virtual void HitBullet(int attacks);
	virtual void HitGrenadeExplosion(int attacks);
	virtual void Destruction();
	virtual int ProcessObject();
	virtual void Render(D3DGraphics *d3dg, bool NoModel);
};

//! @brief �e�ۊǗ��N���X
class bullet : public object
{
protected:
	float rotation_y;	//!< ��]�p�x
	int attacks;		//!< �U����
	int penetration;	//!< �ђʗ�
	float speed;		//!< �e��
	int teamid;			//!< �`�[���ԍ�
	int humanid;		//!< �l�̃f�[�^�ԍ�
	float ontargetcnt;	//!< �������̃J�E���g��
	int cnt;			//!< �J�E���g

public:
	bullet(int modelid = -1, int textureid = -1);
	~bullet();
	virtual void SetPosData(float x, float y, float z, float rx, float ry);
	virtual void SetParamData(int _attacks, int _penetration, float _speed, int _teamid, int _humanid, float _ontargetcnt, bool init);
	virtual void GetPosData(float *x, float *y, float *z, float *rx, float *ry);
	virtual void GetParamData(int *_attacks, int *_penetration, float *_speed, int *_teamid, int *_humanid, float *_ontargetcnt);
	virtual int ProcessObject();
	virtual void Render(class D3DGraphics *d3dg, bool NoModel);
};

//! @brief ��֒e�Ǘ��N���X
class grenade : public bullet
{
	float move_x;		//!< X���ړ���
	float move_y;		//!< Y���ړ���
	float move_z;		//!< Y���ړ���

public:
	grenade(int modelid = -1, int textureid = -1);
	~grenade();
	void SetParamData(float speed, int _teamid, int _humanid, float _ontargetcnt, bool init);
	void GetParamData(float *_speed, int *_teamid, int *_humanid, float *_ontargetcnt);
	float GetSpeed();
	int ProcessObject(class Collision *CollD);
	virtual void Render(D3DGraphics *d3dg, bool NoModel);
};

//! @brief �G�t�F�N�g�Ǘ��N���X
class effect : public object
{
protected:
	float move_x;		//!< X���ړ���
	float move_y;		//!< Y���ړ���
	float move_z;		//!< Z���ړ���
	float addmove_y;	//!< Y���ړ��ʂ̉��Z��
	float rotation_y;	//!< ��]�p�x
	float rotation_texture;		//!< ��]�p�x
	float addrotation_texture;	//!< ��]�p�x�̉��Z��
	float addmodel_size;		//!< �`��T�C�Y
	int brightness;				//!< ���邳
	int addbrightness;			//!< ���邳�̉��Z��
	float alpha;				//!< �����x
	float add_alpha;			//!< �����x�̉��Z��
	int cnt;			//!< �J�E���g
	int type;			//!< ���

public:
	effect(float x = 0.0f, float y = 0.0f, float z = 0.0f, float size = 1.0f, float rotation = 0.0f, int texture = -1, int count = 0);
	~effect();
	virtual void SetParamData(float in_move_x, float in_move_y, float in_move_z, float in_addmove_y, float rotation, float addrotation, float size, float addsize, int count, int settype);
	virtual void SetTexture(int id, int in_brightness, int in_addbrightness, float in_alpha, float in_addalpha);
	virtual void SetRxRy(float rx, float ry);
	virtual int GetTextureID();
	virtual void GetMove(float *mx, float *my, float *mz);
	virtual bool GetCollideMapFlag();
	virtual int ProcessObject();
	virtual void Render(class D3DGraphics *d3dg, float camera_rx, float camera_ry, bool NoModel);
};

//! @brief �l���[�V��������N���X
//! @details �l�̃��[�V�����i�A�j���[�V�����j������s���N���X�ł��Bhuman�N���X���Ŏg�p���܂��B
//! @attention �{�N���X�ɐl�����S�����ۂ̓|��郂�[�V�����͎�������Ă��܂���B
class HumanMotionControl
{
	class ParameterInfo *Param;				//!< �ݒ�l���Ǘ�����N���X�ւ̃|�C���^
	int id_upmodel;							//!< �㔼�g
	int id_armmodel[TOTAL_ARMMODE];			//!< �r
	int id_legmodel;						//!< ���i�Î~�j
	int id_walkmodel[TOTAL_WALKMODE];		//!< ���i�����j
	int id_runmodel[TOTAL_RUNMODE];			//!< ���i����j
	float reaction_y;				//!< �r���グ�������鑊�Ίp�x
	bool slowarm;					//!< �r�̏グ������������肷��
	float armmodel_rotation_y;		//!< �r�̉�]�p�x
	float legrotation_x;			//!< ���̉�]�p�x
	int walkcnt;					//!< �������[�V�����̃J�E���g
	int runcnt;						//!< ���郂�[�V�����̃J�E���g
	int armmodelid;					//!< �`�悷��r�̃��f��
	int legmodelid;					//!< �`�悷�鑫�̃��f��

public:
	HumanMotionControl(class ParameterInfo *in_Param = NULL);
	~HumanMotionControl();
	void SetParameterInfoClass(class ParameterInfo *in_Param);
	void SetModel(int upmodel, int armmodel[], int legmodel, int walkmodel[], int runmodel[]);
	void Init(float rx);
	void PickupWeapon(int weapon_paramid);
	void ChangeHaveWeapon(int weapon_paramid);
	void ChangeWeaponID(int weapon_paramid);
	void ShotWeapon(int weapon_paramid);
	void ReloadWeapon(int weapon_paramid);
	void DumpWeapon();
	void Jump();
	void ProcessObject(float rotation_x, float armrotation_y, int weapon_paramid, int ReloadCnt, int ChangeWeaponIDCnt, int MoveFlag, int hp, bool PlayerFlag);
	void GetRenderMotion(float *arm_rotation_y, float *leg_rotation_x, int *upmodel, int *armmodel, int *legmodel);
	int GetMoveMotionCnt();
};

//! �l�̑��̏�Ԃ������萔
enum Human_LegState
{
	LEG_STOP = 0,
	LEG_WALK,
	LEG_RUN
};

//! �l�̈ړ������\���t���O
enum Human_MoveFlag
{
	MOVEFLAG_FORWARD = 0x01,
	MOVEFLAG_BACK = 0x02,
	MOVEFLAG_LEFT = 0x04,
	MOVEFLAG_RIGHT = 0x08,
	MOVEFLAG_WALK = 0x10,
	MOVEFLAG_JUMP = 0x20,
};

//! �G�t�F�N�g�̎�ނ�\���萔
enum Effect_Type
{
	EFFECT_NORMAL = 0x00,			//!< �m�[�}��
	EFFECT_NOBILLBOARD = 0x01,		//!< �r���{�[�h�����Ȃ�
	EFFECT_COLLIDEMAP = 0x02		//!< �}�b�v�Ƃ̓����蔻��
};

#endif