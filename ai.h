//! @file ai.h
//! @brief AIcontrol�N���X�̐錾

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

#ifndef AI_H
#define AI_H

#define AI_TOTALHUMAN_SCALE (MAX_HUMAN/24)		//!< 24�l������̍ő�l���̔{��

#define AI_ADDTURNRAD DegreeToRadian(0.8f)	//!< AI�̐���i��]�j�\��
#define AI_SEARCH_RX DegreeToRadian(110)	//!< �G�𔭌����鉡���p�x
#define AI_SEARCH_RY DegreeToRadian(60)		//!< �G�𔭌�����c���p�x
#define AI_ZOMBIEATTACK_ARMRY DegreeToRadian(-15)		//!< �]���r�U�����̘r�̊p�x

#define AI_ARRIVALDIST_PATH 5.0f			//!< �p�X�ɓ��B�����Ɣ��肷�鋗��
#define AI_ARRIVALDIST_TRACKING 18.0f		//!< �ǔ��Ώۂɓ��B�����Ɣ��肷�鋗��
#define AI_ARRIVALDIST_WALKTRACKING 24.0f	//!< �ǔ��Ώۂցi���炸�Ɂj�����ċ߂Â�����
#define AI_CHECKJUMP_HEIGHT 0.3f			//!< �O�Ƀu���b�N�����������肷�鍂��
#define AI_CHECKJUMP_DIST 2.0f				//!< �O�Ƀu���b�N�����������肷�鋗��
#define AI_CHECKJUMP2_DIST 10.0f			//!< �O�Ƀu���b�N�����������肷�鋗���i�D��I�ȑ���j
#define AI_CHECKBULLET 20.0f				//!< �e���߂���ʂ����Ɣ��肷�鋗��
#define AI_CHECKSHORTATTACK_DIST 200.0f		//!< �ߋ����U���Ɣ��肷�鋗��

#ifndef H_LAYERLEVEL
 #define H_LAYERLEVEL 3		//!< Select include file.
#endif
#include "main.h"

//! @brief AI������s���N���X
//! @details �p�X�ɂ��ړ��A����ɂ���G�ւ̔F����U���A����̎g�������@�Ȃǂ�AI�̐���S�ʂ��s���܂��B
class AIcontrol
{
	class ObjectManager *ObjMgr;		//!< ObjectManager�N���X�̃|�C���^
	class BlockDataInterface *blocks;	//!< �u���b�N�f�[�^���Ǘ�����N���X�ւ̃|�C���^
	class ParameterInfo *Param;		//!< �ݒ�l���Ǘ�����N���X�ւ̃|�C���^
	class Collision *CollD;				//!< �����蔻����Ǘ�����N���X�ւ̃|�C���^
	class SoundManager *GameSound;		//!< �Q�[�����ʉ��Đ��N���X�ւ̃|�C���^

	class AIMoveNavi *MoveNavi;			//!< �l�ړ��Ǘ��N���X�̃|�C���^
	class AIObjectDriver *ObjDriver;	//!< �l�ړ���]����N���X�̃|�C���^

	int AIlevel;		//!< AI���x��
	int battlemode;		//!< �퓬���[�h
	bool NoFight;		//!< ��퓬���t���O
	float posx;			//!< X���W
	float posy;			//!< Y���W
	float posz;			//!< Z���W
	float rx;			//!< X����]�p�x
	float ry;			//!< Y����]�p�x
	float cautionback_posx;		//!< �x����ɖ߂�X���W
	float cautionback_posz;		//!< �x����ɖ߂�Z���W
	bool FaceCaution_flag;		//!< �x�����ɓ���̕񍐂������t���O
	float FaceCaution_rx;		//!< �x�����Ɍ�������
	float total_move;			//!< ���v�ړ���
	int waitcnt;				//!< ���ԑ҂��J�E���g
	int movejumpcnt;			//!< �W�����v����J�E���g
	int gotocnt;				//!< �ړ��J�E���g
	int cautioncnt;				//!< �x�����J�E���g
	int actioncnt;				//!< �U�����J�E���g
	bool longattack;			//!< �ߋ����E�������t���O
	bool EventWeaponShot;		//!< �C�x���g�ɂ���֒e�����t���O
	AIParameter *LevelParam;	//!< AI�̐��\�l

	bool CheckTargetPos(bool back);
	void MoveTarget(bool back);
	void MoveTarget2(bool back);
	void MoveRandom();
	void TurnSeen();
	bool StopSeen();
	bool MoveJump();
	void Action();
	bool ActionCancel();
	int HaveWeapon();
	void CancelMoveTurn();
	int ControlWeapon();
	int ThrowGrenade();
	void ArmAngle();
	int SearchEnemy();
	int SearchShortEnemy();
	bool CheckLookEnemy(int id, float search_rx, float search_ry, float maxDist, float *out_minDist);
	bool CheckLookEnemy(class human* thuman, float search_rx, float search_ry, float maxDist, float *out_minDist);
	bool CheckCorpse(int id);
	void MovePath();
	bool ActionMain();
	bool CautionMain();
	bool NormalMain();

	int ctrlid;					//!< �������g�i����Ώہj�̐l�ԍ�
	class human *ctrlhuman;		//!< �������g�i����Ώہj��human�N���X
	class human *enemyhuman;	//!< �U���Ώۂ�human�N���X

public:
	AIcontrol(class ObjectManager *in_ObjMgr = NULL, int in_ctrlid = -1, class BlockDataInterface *in_blocks = NULL, class PointDataInterface *in_Points = NULL, class ParameterInfo *in_Param = NULL, class Collision *in_CollD = NULL, class SoundManager *in_GameSound = NULL);
	~AIcontrol();
	void SetClass(class ObjectManager *in_ObjMgr, int in_ctrlid, class BlockDataInterface *in_blocks, class PointDataInterface *in_Points, class ParameterInfo *in_Param, class Collision *in_CollD, class SoundManager *in_GameSound);
	void Init();
	void SetAIlevel(int level);
	int GetAIlevel();
	void SetHoldWait(float px, float pz, float rx);
	void SetHoldTracking(int id);
	void SetCautionMode();
	void SetNoFightFlag(bool flag);
	void GetBattleMode(int *mode, char *modestr);
	int GetEnemyHumanID();
	void GetMoveTargetPos(float *posx, float *posz, int *movemode);
	void GetPathPointData(pointdata *out_data);
	void Process();
};

//! @brief �l�ړ��Ǘ��N���X
//! @details �ʏ탂�[�h�ɂ����āA�l�̈ړ�������肷��N���X�ł��BAIcontrol�N���X���Ŏg�p���܂��B
class AIMoveNavi
{
	class ObjectManager *ObjMgr;		//!< ObjectManager�N���X�̃|�C���^
	int ctrlid;							//!< �������g�i����Ώہj�̐l�ԍ�
	class PointDataInterface *Points;	//!< �|�C���g�f�[�^���Ǘ�����N���X�ւ̃|�C���^

	int movemode;			//!< �ړ����[�h
	bool hold;				//!< �ړ��p�X��ǂ܂Ȃ�
	int path_pointid;		//!< �p�X�̃|�C���g�f�[�^�ԍ�
	int target_humanid;		//!< �^�[�Q�b�g�ɂ���l�̃f�[�^�ԍ�
	float target_posx;		//!< �^�[�Q�b�g��X���W
	float target_posz;		//!< �^�[�Q�b�g��Z���W
	float target_rx;		//!< �^�[�Q�b�g�̐����p�x

public:
	AIMoveNavi(class ObjectManager *in_ObjMgr = NULL, int in_ctrlid = -1, class PointDataInterface *in_Points = NULL);
	~AIMoveNavi();
	void SetClass(class ObjectManager *in_ObjMgr, int in_ctrlid, class PointDataInterface *in_Points);
	void Init();
	bool MovePathNowState();
	bool MovePathNextState();
	void SetHoldWait(float px, float pz, float rx);
	void SetHoldTracking(int id);
	int GetMoveMode();
	bool GetRun2();
	int GetTargetHumanID();
	void GetPathPointData(pointdata *out_data);
	void GetTargetPos(float *posx, float *posz, float *rx, int *out_movemode, int *out_pointmode);
};

//! @brief �l�ړ���]����N���X
//! @details �l�̈ړ�����Ɖ�]������t���O�Ǘ�����N���X�ł��BAIcontrol�N���X���Ŏg�p���܂��B
class AIObjectDriver
{
	class ObjectManager *ObjMgr;		//!< ObjectManager�N���X�̃|�C���^
	int ctrlid;				//!< �������g�i����Ώہj�̐l�ԍ�

	int moveturn_mode;		//!< �ړ��������]�����̃t���O
	float addrx;			//!< X����]�p�����x
	float addry;			//!< Y����]�p�����x

public:
	AIObjectDriver(class ObjectManager *in_ObjMgr = NULL, int in_ctrlid = -1);
	~AIObjectDriver();
	void SetClass(class ObjectManager *in_ObjMgr, int in_ctrlid);
	void Init();
	void ResetMode();
	void SetModeFlag(int flag);
	void DelModeFlag(int flag);
	bool GetModeFlag(int flag);
	void ControlObject();
};

//! AI�̐��䃂�[�h��\���萔
//! @warning �萔��ύX����ꍇ�A�K�v�ɉ�����GetBattleMode()�֐��̕�����o�͏������ύX���邱�ƁB
enum AImode
{
	AI_DEAD = 0,	//!< ���S���Ă���l
	AI_ACTION,		//!< �퓬���̐l
	AI_CAUTION,		//!< �x�����̐l
	AI_NORMAL,		//!< �ʏ�̃��[�h

	AI_WALK,		//!< �ړ��p�X�ɂ���ĕ����Ă���l
	AI_RUN,			//!< �ړ��p�X�ɂ���đ����Ă���l
	AI_WAIT,		//!< �p�X�ɂ���đҋ@���Ă���l
	AI_STOP_5SEC,	//!< �p�X�ɂ���Ď��ԑ҂������Ă���l�i5�b�j
	AI_TRACKING,	//!< ����̐l��ǔ�����
	AI_GRENADE,		//!< ��֒e�𓊂���
	AI_RUN2,		//!< �D��I�ȑ���
	AI_RANDOM,		//!< �����_���p�X������
	AI_NULL			//!< �p�X�Ȃ�
};

//! AI�̈ړ����[�h��\���萔
enum AIMoveNaviFlag
{
	AI_NAVI_MOVE_NULL,
	AI_NAVI_MOVE_WALK,
	AI_NAVI_MOVE_RUN,
	AI_NAVI_MOVE_RUN2,
	AI_NAVI_MOVE_TRACKING,

	AI_NAVI_POINT_NULL,
	AI_NAVI_POINT_WAIT,
	AI_NAVI_POINT_STOP_5SEC,
	AI_NAVI_POINT_TRACKING,
	AI_NAVI_POINT_GRENADE
};

//! AI�̑��샂�[�h��\���萔
enum AIcontrolFlag
{
	AI_CTRL_MOVEFORWARD = 0x0001,
	AI_CTRL_MOVEBACKWARD = 0x0002,
	AI_CTRL_MOVELEFT = 0x0004,
	AI_CTRL_MOVERIGHT = 0x0008,
	AI_CTRL_MOVEWALK = 0x0010,
	AI_CTRL_TURNUP = 0x0100,
	AI_CTRL_TURNDOWN = 0x0200,
	AI_CTRL_TURNLEFT = 0x0400,
	AI_CTRL_TURNRIGHT = 0x0800
};

#endif