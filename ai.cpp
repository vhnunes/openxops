//! @file ai.cpp
//! @brief AIcontrol�N���X�̒�`

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

#include "ai.h"

//! @brief �R���X�g���N�^
AIcontrol::AIcontrol(class ObjectManager *in_ObjMgr, int in_ctrlid, class BlockDataInterface *in_blocks, class PointDataInterface *in_Points, class ParameterInfo *in_Param, class Collision *in_CollD, class SoundManager *in_GameSound)
{
	ObjMgr = in_ObjMgr;
	ctrlid = in_ctrlid;
	if( in_ObjMgr != NULL ){ ctrlhuman = in_ObjMgr->GetHumanObject(in_ctrlid); }
	else{ ctrlhuman = NULL; }
	enemyhuman = NULL;
	blocks = in_blocks;
	Param = in_Param;
	CollD = in_CollD;
	GameSound = in_GameSound;

	AIlevel = 0;
	battlemode = AI_NORMAL;
	NoFight = false;
	posx = 0.0f;
	posy = 0.0f;
	posz = 0.0f;
	rx = 0.0f;
	ry = 0.0f;
	cautionback_posx = 0.0f;
	cautionback_posz = 0.0f;
	FaceCaution_flag = false;
	FaceCaution_rx = 0.0f;
	total_move = 0.0f;
	waitcnt = 0;
	movejumpcnt = 1*((int)GAMEFPS);
	gotocnt = 0;
	cautioncnt = 0;
	actioncnt = 0;
	longattack = false;
	EventWeaponShot = false;
	LevelParam = NULL;

	MoveNavi = new AIMoveNavi;
	ObjDriver = new AIObjectDriver;
	MoveNavi->SetClass(in_ObjMgr, in_ctrlid, in_Points);
	ObjDriver->SetClass(in_ObjMgr, in_ctrlid);
}

//! @brief �f�B�X�g���N�^
AIcontrol::~AIcontrol()
{
	delete MoveNavi;
	delete ObjDriver;
}

//! @brief �ΏۃN���X��ݒ�
//! @attention ���̊֐��Őݒ���s��Ȃ��ƁA�N���X���̂��������@�\���܂���B
void AIcontrol::SetClass(class ObjectManager *in_ObjMgr, int in_ctrlid, class BlockDataInterface *in_blocks, class PointDataInterface *in_Points, class ParameterInfo *in_Param, class Collision *in_CollD, class SoundManager *in_GameSound)
{
	ObjMgr = in_ObjMgr;
	ctrlid = in_ctrlid;
	ctrlhuman = in_ObjMgr->GetHumanObject(in_ctrlid);
	blocks = in_blocks;
	Param = in_Param;
	CollD = in_CollD;
	GameSound = in_GameSound;

	MoveNavi->SetClass(in_ObjMgr, in_ctrlid, in_Points);
	ObjDriver->SetClass(in_ObjMgr, in_ctrlid);
}

//! @brief �ڕW�n�_�Ɉړ����Ă��邩�m�F
//! @return ���B�Ftrue�@�񓞒B�Ffalse
bool AIcontrol::CheckTargetPos(bool back)
{
	float target_posx, target_posz;
	int movemode;
	if( back == false ){
		MoveNavi->GetTargetPos(&target_posx, &target_posz, NULL, &movemode, NULL);
	}
	else{
		target_posx = cautionback_posx;
		target_posz = cautionback_posz;
		movemode = AI_NAVI_MOVE_RUN;
	}

	//�������Z�o
	float x = posx - target_posx;
	float z = posz - target_posz;
	float r = x * x + z * z;

	if( movemode == AI_NAVI_MOVE_TRACKING ){	//�ǔ����Ȃ�
		if( r < AI_ARRIVALDIST_TRACKING * AI_ARRIVALDIST_TRACKING ){
			return true;
		}
	}
	else{							//����ȊO�Ȃ�
		if( r < (AI_ARRIVALDIST_PATH * AI_ARRIVALDIST_PATH) ){
			return true;
		}
	}

	return false;
}

//! @brief �ڕW�n�_�Ɉړ�
void AIcontrol::MoveTarget(bool back)
{
	float r, atan;
	int paramid;
	HumanParameter Paraminfo;
	bool zombie;

	float target_posx, target_posz;	
	int movemode;
	if( back == false ){
		MoveNavi->GetTargetPos(&target_posx, &target_posz, NULL, &movemode, NULL);
	}
	else{
		target_posx = cautionback_posx;
		target_posz = cautionback_posz;
		movemode = AI_NAVI_MOVE_RUN;
	}

	//�]���r���ǂ�������
	ctrlhuman->GetParamData(&paramid, NULL, NULL, NULL);
	Param->GetHuman(paramid, &Paraminfo);
	if( Paraminfo.type == 2 ){
		zombie = true;
	}
	else{
		zombie = false;
	}

	//��x�S�Ă̓������~�߂�
	ObjDriver->ResetMode();

	//�ڕW�n�_�ւ̊p�x�����߂�
	CheckTargetAngle(posx, 0.0f, posz, rx*-1 + (float)M_PI/2, 0.0f, target_posx, 0.0f, target_posz, 0.0f, &atan, NULL, &r);

	//����
	if( atan > DegreeToRadian(0.5f) ){
		ObjDriver->SetModeFlag(AI_CTRL_TURNLEFT);
	}
	if( atan < DegreeToRadian(-0.5f) ){
		ObjDriver->SetModeFlag(AI_CTRL_TURNRIGHT);
	}

	//�O�i����
	if( zombie == true ){
		if( fabsf(atan) < DegreeToRadian(20) ){
			ObjDriver->SetModeFlag(AI_CTRL_MOVEWALK);
		}
	}
	else if( movemode == AI_NAVI_MOVE_RUN ){
		if( fabsf(atan) < DegreeToRadian(50) ){
			ObjDriver->SetModeFlag(AI_CTRL_MOVEFORWARD);
		}
	}
	else if( movemode == AI_NAVI_MOVE_RUN2 ){
		if( fabsf(atan) < DegreeToRadian(50) ){
			ObjDriver->SetModeFlag(AI_CTRL_MOVEFORWARD);
		}
	}
	else if( movemode == AI_NAVI_MOVE_WALK ){
		if( fabsf(atan) < DegreeToRadian(6) ){
			ObjDriver->SetModeFlag(AI_CTRL_MOVEWALK);
		}
	}
	else if( movemode == AI_NAVI_MOVE_TRACKING ){
		if( fabsf(atan) < DegreeToRadian(20) ){
			if( r < (AI_ARRIVALDIST_WALKTRACKING * AI_ARRIVALDIST_WALKTRACKING) ){
				ObjDriver->SetModeFlag(AI_CTRL_MOVEWALK);
			}
			else{
				ObjDriver->SetModeFlag(AI_CTRL_MOVEFORWARD);
			}
		}
	}

	//�W�����v
	if( GetRand(16) == 0 ){
		MoveJump();
	}

	//�����|�����Ă�����A���E�ւ̉�]�������_���ɍs��
	if( GetRand(28) == 0 ){
		if( ctrlhuman->GetMovemode(true) != 0 ){
			if( ctrlhuman->GetTotalMove() - total_move < 0.1f ){
				if( GetRand(2) == 0 ){ ObjDriver->SetModeFlag(AI_CTRL_TURNRIGHT); }
				else{ ObjDriver->SetModeFlag(AI_CTRL_TURNLEFT); }
			}
		}
	}
	total_move = ctrlhuman->GetTotalMove();
}

//! @brief �ڕW�n�_�Ɉړ��i�D��I�ȑ���p�j
void AIcontrol::MoveTarget2(bool back)
{
	float atan;

	float target_posx, target_posz;	
	if( back == false ){
		MoveNavi->GetTargetPos(&target_posx, &target_posz, NULL, NULL, NULL);
	}
	else{
		target_posx = cautionback_posx;
		target_posz = cautionback_posz;
	}

	//�ڕW�n�_�ւ̊p�x�����߂�
	CheckTargetAngle(posx, 0.0f, posz, rx*-1 + (float)M_PI/2, 0.0f, target_posx, 0.0f, target_posz, 0.0f, &atan, NULL, NULL);

	//�O��ړ��̏���
	if( fabsf(atan) < DegreeToRadian(56) ){
		ObjDriver->SetModeFlag(AI_CTRL_MOVEFORWARD);
	}
	if( fabsf(atan) > DegreeToRadian(123.5f) ){
		ObjDriver->SetModeFlag(AI_CTRL_MOVEBACKWARD);
	}

	//���E�ړ��̏���
	if( (DegreeToRadian(-146) < atan)&&(atan < DegreeToRadian(-33)) ){
		ObjDriver->SetModeFlag(AI_CTRL_MOVERIGHT);
	}
	if( (DegreeToRadian(33) < atan)&&(atan < DegreeToRadian(146)) ){
		ObjDriver->SetModeFlag(AI_CTRL_MOVELEFT);
	}

	//�W�����v
	if( GetRand(16) == 0 ){
		MoveJump();
	}

	//�����|�����Ă�����A���E�ւ̉�]�������_���ɍs��
	if( GetRand(28) == 0 ){
		if( ctrlhuman->GetMovemode(true) != 0 ){
			if( ctrlhuman->GetTotalMove() - total_move < 0.1f ){
				if( GetRand(2) == 0 ){ ObjDriver->SetModeFlag(AI_CTRL_TURNRIGHT); }
				else{ ObjDriver->SetModeFlag(AI_CTRL_TURNLEFT); }
			}
		}
	}
	total_move = ctrlhuman->GetTotalMove();
}

//! @brief �O�㍶�E�����_���Ɉړ��i�U�����p�j
void AIcontrol::MoveRandom()
{
	int forwardstart, backstart, sidestart;

	if( longattack == false ){
		forwardstart = 80;
		backstart = 90;
		sidestart = 70;
	}
	else{
		forwardstart = 120;
		backstart = 150;
		sidestart = 130;
	}

	//�����_���Ɉړ����n�߂�
	if( GetRand(forwardstart) == 0 ){
		ObjDriver->SetModeFlag(AI_CTRL_MOVEFORWARD);
	}
	if( GetRand(backstart) == 0 ){
		ObjDriver->SetModeFlag(AI_CTRL_MOVEBACKWARD);
	}
	if( GetRand(sidestart) == 0 ){
		ObjDriver->SetModeFlag(AI_CTRL_MOVELEFT);
	}
	if( GetRand(sidestart) == 0 ){
		ObjDriver->SetModeFlag(AI_CTRL_MOVERIGHT);
	}

	//����������Ă��炸�A��Ԃ�Ȃ��
	if( ctrlhuman->GetMainWeaponTypeNO() == ID_WEAPON_NONE ){
		// 1/80�̊m���ŉ�����n�߂�
		if( GetRand(80) == 0 ){
			ObjDriver->SetModeFlag(AI_CTRL_MOVEBACKWARD);
		}
	}

	// 1/3�̊m�����A�ړ��t���O���ݒ肳��Ă�����
	if( (GetRand(3) == 0)||(ObjDriver->GetModeFlag(AI_CTRL_MOVEFORWARD | AI_CTRL_MOVEBACKWARD | AI_CTRL_MOVELEFT | AI_CTRL_MOVERIGHT)) ){
		float vx, vz;
		float Dist;

		if( GetRand(2) == 0 ){
			//�O�����̃x�N�g�����v�Z
			vx = cosf(rx*-1 + (float)M_PI/2);
			vz = sinf(rx*-1 + (float)M_PI/2);
			if(
				(CollD->CheckALLBlockIntersectDummyRay(posx, posy + HUMAN_MAPCOLLISION_HEIGHT, posz, vx, 0, vz, NULL, NULL, &Dist, HUMAN_MAPCOLLISION_R) == true)||		//���̍����Ƀu���b�N������i�Ԃ���j
				(CollD->CheckALLBlockIntersectDummyRay(posx, posy - 1.0f, posz, vx, 0, vz, NULL, NULL, &Dist, HUMAN_MAPCOLLISION_R) == false)							//�����Ƀu���b�N���Ȃ��i������j
			){
				//�O�i�t���O���폜���A��ރt���O��ݒ�
				ObjDriver->DelModeFlag(AI_CTRL_MOVEFORWARD);
				ObjDriver->SetModeFlag(AI_CTRL_MOVEBACKWARD);
			}

			//������̃x�N�g�����v�Z
			vx = cosf(rx*-1 + (float)M_PI/2 + (float)M_PI);
			vz = sinf(rx*-1 + (float)M_PI/2 + (float)M_PI);
			if(
				(CollD->CheckALLBlockIntersectDummyRay(posx, posy + HUMAN_MAPCOLLISION_HEIGHT, posz, vx, 0, vz, NULL, NULL, &Dist, HUMAN_MAPCOLLISION_R) == true)||		//���̍����Ƀu���b�N������i�Ԃ���j
				(CollD->CheckALLBlockIntersectDummyRay(posx, posy - 1.0f, posz, vx, 0, vz, NULL, NULL, &Dist, HUMAN_MAPCOLLISION_R) == false)							//�����Ƀu���b�N���Ȃ��i������j
			){
				//��ރt���O���폜���A�O�i�t���O��ݒ�
				ObjDriver->DelModeFlag(AI_CTRL_MOVEBACKWARD);
				ObjDriver->SetModeFlag(AI_CTRL_MOVEFORWARD);
			}
		}
		else{
			//
			vx = cosf(rx*-1);
			vz = sinf(rx*-1);
			if(
				(CollD->CheckALLBlockIntersectDummyRay(posx, posy + HUMAN_MAPCOLLISION_HEIGHT, posz, vx, 0, vz, NULL, NULL, &Dist, HUMAN_MAPCOLLISION_R) == true)||		//���̍����Ƀu���b�N������i�Ԃ���j
				(CollD->CheckALLBlockIntersectDummyRay(posx, posy - 1.0f, posz, vx, 0, vz, NULL, NULL, &Dist, HUMAN_MAPCOLLISION_R) == false)							//�����Ƀu���b�N���Ȃ��i������j
			){
				//�E�ړ��t���O���폜���A���ړ��t���O��ݒ�
				ObjDriver->DelModeFlag(AI_CTRL_MOVERIGHT);
				ObjDriver->SetModeFlag(AI_CTRL_MOVELEFT);
			}

			vx = cosf(rx*-1 + (float)M_PI);
			vz = sinf(rx*-1 + (float)M_PI);
			if(
				(CollD->CheckALLBlockIntersectDummyRay(posx, posy + HUMAN_MAPCOLLISION_HEIGHT, posz, vx, 0, vz, NULL, NULL, &Dist, HUMAN_MAPCOLLISION_R) == true)||		//���̍����Ƀu���b�N������i�Ԃ���j
				(CollD->CheckALLBlockIntersectDummyRay(posx, posy - 1.0f, posz, vx, 0, vz, NULL, NULL, &Dist, HUMAN_MAPCOLLISION_R) == false)							//�����Ƀu���b�N���Ȃ��i������j
			){
				//���ړ��t���O���폜���A�E�ړ��t���O��ݒ�
				ObjDriver->DelModeFlag(AI_CTRL_MOVELEFT);
				ObjDriver->SetModeFlag(AI_CTRL_MOVERIGHT);
			}
		}
	}

	//�U���Ώۂ�����Ȃ�
	if( enemyhuman != NULL ){
		float tx, ty, tz;

		enemyhuman->GetPosData(&tx, &ty, &tz, NULL);

		float x = posx - tx;
		float y = posy - ty;
		float z = posz - tz;
		float r = x * x + y * y + z * z;

		//�G�ɋ߂Â��������Ȃ��ނ���
		if( r < 20.0f * 20.0f ){
			ObjDriver->DelModeFlag(AI_CTRL_MOVEFORWARD);
			if( GetRand(70) == 0 ){
				ObjDriver->SetModeFlag(AI_CTRL_MOVEBACKWARD);
			}
		}
	}
}

//! @brief ���̏�����܂킷
void AIcontrol::TurnSeen()
{
	int turnstart, turnstop;

	float target_rx;
	int pointmode;
	MoveNavi->GetTargetPos(NULL, NULL, &target_rx, NULL, &pointmode);

	//������Čx�������Ȃ�A�����ꂽ����������
	if( (battlemode == AI_CAUTION)&&(FaceCaution_flag == true) ){
		float tr;

		//�������v�Z
		tr = FaceCaution_rx - rx;
		tr = AngleNormalization(tr);

		//����
		if( tr > DegreeToRadian(2.5f) ){
			ObjDriver->SetModeFlag(AI_CTRL_TURNRIGHT);
		}
		if( tr < DegreeToRadian(-2.5f) ){
			ObjDriver->SetModeFlag(AI_CTRL_TURNLEFT);
		}

		//��������Ɍ����I���΁A���ʂ̌x���Ɉڍs����
		if( fabsf(tr) <= DegreeToRadian(2.5f) ){
			FaceCaution_flag = false;
		}

		return;
	}

	//��]�̊J�n�E�I���m����ݒ�
	if( battlemode == AI_ACTION ){
		return;
	}
	else if( battlemode == AI_CAUTION ){
		turnstart = 20;
		turnstop = 20;
	}
	else{
		if( pointmode == AI_NAVI_POINT_TRACKING ){ turnstart = 65; }
		else{ turnstart = 85; }
		turnstop = 18;
	}

	//�����_���ɉ�]���n�߂�
	if( GetRand(turnstart) == 0 ){
		ObjDriver->SetModeFlag(AI_CTRL_TURNRIGHT);
	}
	if( GetRand(turnstart) == 0 ){
		ObjDriver->SetModeFlag(AI_CTRL_TURNLEFT);
	}

	if( (battlemode == AI_NORMAL)&&(pointmode == AI_NAVI_POINT_WAIT) ){
		//�����_���Ƀ|�C���g�̕����������Ƃ���
		//�u�|�C���g�̕����������d������v�̍Č� 
		if( GetRand(80) == 0 ){
			float tr;
			tr = target_rx - rx;
			tr = AngleNormalization(tr);

			if( tr > 0.0f ){
				ObjDriver->SetModeFlag(AI_CTRL_TURNRIGHT);
			}
			if( tr < 0.0f ){
				ObjDriver->SetModeFlag(AI_CTRL_TURNLEFT);
			}
		}
	}

	//��]�������_���Ɏ~�߂�
	if( GetRand(turnstop) == 0 ){
		ObjDriver->DelModeFlag(AI_CTRL_TURNRIGHT);
	}
	if( GetRand(turnstop) == 0 ){
		ObjDriver->DelModeFlag(AI_CTRL_TURNLEFT);
	}
}

//! @brief ����̕�������������
bool AIcontrol::StopSeen()
{
	float tr;
	bool returnflag = false;

	float target_rx;
	MoveNavi->GetTargetPos(NULL, NULL, &target_rx, NULL, NULL);

	tr = target_rx - rx;
	tr = AngleNormalization(tr);

	ObjDriver->DelModeFlag(AI_CTRL_TURNRIGHT);
	ObjDriver->DelModeFlag(AI_CTRL_TURNLEFT);

	//����
	if( tr > DegreeToRadian(2.5f) ){
		ObjDriver->SetModeFlag(AI_CTRL_TURNRIGHT);
		returnflag = false;
	}
	if( tr < DegreeToRadian(-2.5f) ){
		ObjDriver->SetModeFlag(AI_CTRL_TURNLEFT);
		returnflag = false;
	}

	//��������Ɍ����Ă��邩����
	if( fabsf(tr) <= DegreeToRadian(2.5f) ){
		returnflag = true;
	}

	return returnflag;
}

//! @brief �i�s�����ɏ�Q��������΃W�����v����
bool AIcontrol::MoveJump()
{
	//�����~�܂��Ă���Ώ������Ȃ�
	//�@��AIObjectDriver(ObjDriver)�̈ړ��t���O�Ń`�F�b�N���Ă��ǂ��B
	if( ctrlhuman->GetMovemode(false) == 0 ){ return false; }

	float dist_dummy;

	float check_rx = 0.0f;
	float new_posx, new_posy, new_posz;

	//�i�s�������Z�o
	if( ObjDriver->GetModeFlag(AI_CTRL_MOVEWALK) == true ){
		check_rx = DegreeToRadian(0.0f);
	}
	if( ObjDriver->GetModeFlag(AI_CTRL_MOVEFORWARD) == true ){
		check_rx = DegreeToRadian(0.0f);
	}
	if( ObjDriver->GetModeFlag(AI_CTRL_MOVEBACKWARD) == true ){
		check_rx = DegreeToRadian(180.0f);
	}
	if( ObjDriver->GetModeFlag(AI_CTRL_MOVELEFT) == true ){
		check_rx = DegreeToRadian(90.0f);
	}
	if( ObjDriver->GetModeFlag(AI_CTRL_MOVERIGHT) == true ){
		check_rx = DegreeToRadian(-90.0f);
	}

	//���t�߂̓����蔻��
	new_posx = posx + cosf(rx*-1 + (float)M_PI/2 + check_rx) * (AI_CHECKJUMP_DIST + HUMAN_MAPCOLLISION_R);
	new_posy = posy + HUMAN_MAPCOLLISION_HEIGHT;
	new_posz = posz + sinf(rx*-1 + (float)M_PI/2 + check_rx) * (AI_CHECKJUMP_DIST + HUMAN_MAPCOLLISION_R);
	if( CollD->CheckALLBlockInside(new_posx, new_posy, new_posz) == true ){
		ObjMgr->MoveJump(ctrlid);
		return true;
	}

	//�̑S�̂̓����蔻��
	new_posx = posx + cosf(rx*-1 + (float)M_PI/2 + check_rx) * AI_CHECKJUMP_DIST;
	new_posy = posy + AI_CHECKJUMP_HEIGHT;
	new_posz = posz + sinf(rx*-1 + (float)M_PI/2 + check_rx) * AI_CHECKJUMP_DIST;
	if( CollD->CheckALLBlockInside(new_posx, new_posy, new_posz) == true ){
		ObjMgr->MoveJump(ctrlid);
		return true;
	}
	else if( CollD->CheckALLBlockIntersectRay(new_posx, new_posy, new_posz, 0.0f, 1.0f, 0.0f, NULL, NULL, &dist_dummy, HUMAN_HEIGHT - AI_CHECKJUMP_HEIGHT) == true ){
		ObjMgr->MoveJump(ctrlid);
		return true;
	}

	return false;
}

//! @brief �U��
//! @todo �]���r�̑����߂܂��鏈��
void AIcontrol::Action()
{
	human* EnemyHuman = NULL;
	int paramid;
	HumanParameter Paraminfo;
	bool zombie;
	float posy2;
	float tx, ty, tz;

	EnemyHuman = enemyhuman;

	//���W���擾
	EnemyHuman->GetPosData(&tx, &ty, &tz, NULL);
	posy2 = posy + VIEW_HEIGHT;
	ty += VIEW_HEIGHT;

	//�]���r���ǂ�������
	ctrlhuman->GetParamData(&paramid, NULL, NULL, NULL);
	Param->GetHuman(paramid, &Paraminfo);
	if( Paraminfo.type == 2 ){
		zombie = true;
	}
	else{
		zombie = false;
	}

	//�������Ă��镐��̎�ނ��擾
	int weaponid = ctrlhuman->GetMainWeaponTypeNO();

	float atanx, atany, r;

	//�^�[�Q�b�g�̈ʒu��␳
	if( zombie == true ){		//�]���r�̏ꍇ
		float x = posx - tx;
		float z = posz - tz;
		float r = sqrtf(x * x + z * z);
		float scale;
		float mx, mz;

		if( r > 180.0f ){ r = 180.0f; }		//������������ 1800.0f �������

		//�Ǐ]���̃p�����[�^�[���擾
		switch( Paraminfo.AItrackability ){
			case 0: scale = (0.05f * 2); break;
			case 1: scale = (0.10f * 2); break;
			case 2: scale = (0.13f * 2); break;
			case 3: scale = (0.17f * 2); break;
			default: scale = 0.0f;
		}

		//�G�̈ړ���������
		EnemyHuman->GetMovePos(&mx, NULL, &mz);
		tx += mx * r * scale;
		tz += mz * r * scale;
	}
	else{		//�]���r�łȂ��ꍇ
		//��������֒e�������Ă���΁`
		if( weaponid == ID_WEAPON_GRENADE ){
			if( zombie == false ){
				float x = posx - tx;
				float z = posz - tz;
				float r = sqrtf(x * x + z * z);
				float scale;
				if( longattack == false ){ scale = 0.12f; }
				else{ scale = 0.4f; }

				//�����ɉ����č�����ς���
				ty += (r - AI_CHECKSHORTATTACK_DIST) * scale;
			}
		}
		else{
			float mx, mz;
			float scale;
			EnemyHuman->GetMovePos(&mx, NULL, &mz);
			if( longattack == false ){ scale = 1.5f; }
			else{
				float x = posx - tx;
				float z = posz - tz;
				float r = sqrtf(x * x + z * z);

				scale = r * 0.12f;
			}

			//�G�̈ړ���������
			tx += mx * scale;
			tz += mz * scale;
		}
	}

	//�ڕW�n�_�ւ̊p�x�����߂�
	CheckTargetAngle(posx, posy2, posz, rx*-1 + (float)M_PI/2, ry, tx, ty, tz, 0.0f, &atanx, &atany, &r);

	//������ς���^�C�~���O������
	int randr = LevelParam->aiming;
	if( longattack == false ){ randr += 1; }
	else{ randr += 2; }

	if( randr != 0 ){
		//����
		if( atanx > 0.0f ){
			ObjDriver->SetModeFlag(AI_CTRL_TURNLEFT);
			ObjDriver->DelModeFlag(AI_CTRL_TURNRIGHT);
		}
		if( atanx < 0.0f ){
			ObjDriver->SetModeFlag(AI_CTRL_TURNRIGHT);
			ObjDriver->DelModeFlag(AI_CTRL_TURNLEFT);
		}

		//�r�̊p�x
		if( zombie == true ){
			//ry = AI_ZOMBIEATTACK_ARMRY;

			//����
			if( ry < AI_ZOMBIEATTACK_ARMRY ){
				ObjDriver->SetModeFlag(AI_CTRL_TURNUP);
				ObjDriver->DelModeFlag(AI_CTRL_TURNDOWN);
			}
			if( ry > AI_ZOMBIEATTACK_ARMRY ){
				ObjDriver->SetModeFlag(AI_CTRL_TURNDOWN);
				ObjDriver->DelModeFlag(AI_CTRL_TURNUP);
			}
		}
		else{
			//��������Ԃ�Ȃ�΁`
			if( weaponid == ID_WEAPON_NONE ){
				if( EnemyHuman->GetMainWeaponTypeNO() == ID_WEAPON_NONE ){	//�G����Ԃ�Ȃ�΁`
					//���Ɍ���������
					ObjDriver->SetModeFlag(AI_CTRL_TURNDOWN);
					ObjDriver->DelModeFlag(AI_CTRL_TURNUP);
				}
				else{														//�G������������Ă���΁`
					//��Ɍ���������
					ObjDriver->SetModeFlag(AI_CTRL_TURNUP);
					ObjDriver->DelModeFlag(AI_CTRL_TURNDOWN);
				}
			}
			else{
				//����
				if( atany > 0.0f ){
					ObjDriver->SetModeFlag(AI_CTRL_TURNUP);
					ObjDriver->DelModeFlag(AI_CTRL_TURNDOWN);
				}
				if( atany < 0.0f ){
					ObjDriver->SetModeFlag(AI_CTRL_TURNDOWN);
					ObjDriver->DelModeFlag(AI_CTRL_TURNUP);
				}
			}
		}
	}

	//�]���r�ȊO�Ŏ�Ԃ�Ȃ��
	if( zombie == false ){
		if( weaponid == ID_WEAPON_NONE ){
			//���̊m���Ō�ނ���
			if( GetRand(80) == 0 ){
				ObjDriver->SetModeFlag(AI_CTRL_MOVEBACKWARD);
			}
		}
	}

	
	if( zombie == true ){	//�]���r�̍U��
		float y = posy2 - ty;

		//���������Ă���΁A��x�����ɐ؂�ւ���
		if( ObjDriver->GetModeFlag(AI_CTRL_MOVEFORWARD) ){
			ObjDriver->DelModeFlag(AI_CTRL_MOVEFORWARD);
			ObjDriver->SetModeFlag(AI_CTRL_MOVEWALK);
		}

		//�G�Ɍ������đO�i����
		if( fabsf(atanx) <= DegreeToRadian(25) ){
			if( (fabsf(atanx) <= DegreeToRadian(15)) && (r < 24.0f*24.0f) && (actioncnt%50 > 20) ){
				//�������������A����
				ObjDriver->SetModeFlag(AI_CTRL_MOVEFORWARD);
				ObjDriver->DelModeFlag(AI_CTRL_MOVEWALK);
			}
			else{
				ObjDriver->SetModeFlag(AI_CTRL_MOVEWALK);
			}
		}

		/*
		//�W�����v
		if( GetRand(16) == 0 ){
			MoveJump();
		}
		*/

		if( (r < 9.0f*9.0f)&&( fabsf(y) < 10.0f) ){
			float x = posx - tx;
			float z = posz - tz;

			//�߂܂���@�i�G�������t����j
			EnemyHuman->AddPosOrder(atan2f(z, x), 0.0f, 0.5f);

			//�G�̎��_�������_���ɓ�����
			float erx, ery;
			EnemyHuman->GetRxRy(&erx, &ery);
			switch(GetRand(3)){
				case 0: erx -= DegreeToRadian(2); break;
				case 1: erx += DegreeToRadian(2); break;
				default: break;
			}
			switch(GetRand(3)){
				case 0: ery -= DegreeToRadian(2); break;
				case 1: ery += DegreeToRadian(2); break;
				default: break;
			}
			EnemyHuman->SetRxRy(erx, ery);
		}

		if( actioncnt%50 == 0){
			if( ObjMgr->CheckZombieAttack(ctrlhuman, EnemyHuman) == true ){
				ObjMgr->HitZombieAttack(ctrlhuman, EnemyHuman);
			}
		}
	}
	else{					//���C����
		int scopemode = 0;
		ScopeParameter sparam;
		float ShotAngle;

		//����̃X�R�[�v�ݒ���擾
		if( weaponid != ID_WEAPON_NONE ){
			WeaponParameter wparam;
			Param->GetWeapon(weaponid, &wparam);
			scopemode = wparam.scopemode;
		}

		if( longattack == false ){
			//�G�𑨂����Ɣ��肷��A���e�덷���v�Z����
			Param->GetScopeParam(scopemode, &sparam);
			ShotAngle = sparam.AI_ShotAngle;

			//AI���x�����Ƃɒ���
			ShotAngle += DegreeToRadian(0.5f) * LevelParam->limitserror;

			if( MoveNavi->GetRun2() == true ){
				ShotAngle *= 1.5f;
			}
		}
		else{
			//�G�𑨂����Ɣ��肷��A���e�덷���v�Z����
			Param->GetScopeParam(scopemode, &sparam);
			ShotAngle = sparam.AI_ShotAngleLong;

			//AI���x�����Ƃɒ���
			ShotAngle += DegreeToRadian(0.2f) * LevelParam->limitserror;
		}

		//�G�𑨂��Ă����
		float atanxy = fabsf(atanx) + fabsf(atany);
		if( atanxy < ShotAngle ){
			int rand = LevelParam->attack;
			if( longattack == true ){ rand += 1; }

			//���C
			if( GetRand(rand) == 0 ){
				ObjMgr->ShotWeapon(ctrlid);
			}
		}
	}

	//�����ɉ����ċߋ����E��������؂�ւ���
	//�@�� AI_CHECKSHORTATTACK_DIST �s�b�^���Őݒ�l�ێ�
	if( r < AI_CHECKSHORTATTACK_DIST * AI_CHECKSHORTATTACK_DIST ){
		longattack = false;
	}
	if( (r > AI_CHECKSHORTATTACK_DIST * AI_CHECKSHORTATTACK_DIST)&&(MoveNavi->GetRun2() == false) ){
		longattack = true;
	}

	if( zombie == false ){
		//�����_���Ɉړ�
		MoveRandom();
	}

	actioncnt += 1;
}

//! @brief �U�����L�����Z��
bool AIcontrol::ActionCancel()
{
	//��퓬���t���O���L���Ȃ�I��
	if( NoFight == true ){
		return true;
	}

	//�G�����S������I��
	if( enemyhuman->GetDeadFlag() == true ){
		return true;
	}

	//�������擾
	float tx, ty, tz;
	enemyhuman->GetPosData(&tx, &ty, &tz, NULL);
	float x = posx - tx;
	float y = posy - ty;
	float z = posz - tz;
	float r = x*x + y*y + z*z;

	//����������߂��Ă�����I��
	if( r > 620.0f*620.0f ){
		return true;
	}

	if( longattack == false ){
		//�K���ȃ^�C�~���O�œG�������邩�m�F
		if( GetRand(40) == 0 ){
			//�u���b�N���Ղ��Ă����i�������Ȃ��j�Ȃ�ΏI��
			if( CheckLookEnemy(enemyhuman, AI_SEARCH_RX, AI_SEARCH_RY, 620.0f, NULL) == false ){
				return true;
			}
		}

		//�����I�ɏI��
		if( GetRand(550) == 0 ){
			return true;
		}
	}
	else{
		//�K���ȃ^�C�~���O�œG�������邩�m�F
		if( GetRand(30) == 0 ){
			//�u���b�N���Ղ��Ă����i�������Ȃ��j�Ȃ�ΏI��
			if( CheckLookEnemy(enemyhuman, AI_SEARCH_RX, AI_SEARCH_RY, 620.0f, NULL) == false ){
				return true;
			}
		}

		//�����I�ɏI��
		if( GetRand(450) == 0 ){
			return true;
		}
	}

	return false;
}

//! @brief ���������
int AIcontrol::HaveWeapon()
{
	int selectweapon;
	class weapon *weapon[TOTAL_HAVEWEAPON];
	int lnbslist[TOTAL_HAVEWEAPON];
	int nbslist[TOTAL_HAVEWEAPON];
	int nbs = 0;

	for(int i=0; i<TOTAL_HAVEWEAPON; i++){
		weapon[i] = NULL;
		lnbslist[i] = 0;
		nbslist[i] = 0;
	}

	//����̏����擾
	ctrlhuman->GetWeapon(&selectweapon, weapon, lnbslist, nbslist);

	//����������Ă��Ȃ��ɂ��ւ�炸�A���e��������Ȃ�؂�ւ��Ȃ�
	if( (weapon[selectweapon] == NULL)&&(nbslist[selectweapon] > 0) ){
		return 0;
	}

	//����������Ă���΁A���v�e�����擾
	if( weapon[selectweapon] != NULL ){
		weapon[selectweapon]->GetParamData(NULL, NULL, &nbs);
	}

	//����������Ă��Ȃ����A�e��0�Ȃ��
	if( (weapon[selectweapon] == NULL)||(nbs == 0) ){
		//���̕�����w��
		int notselectweapon = selectweapon + 1;
		if( notselectweapon == TOTAL_HAVEWEAPON ){ notselectweapon = 0; }

		//���̕���������Ă����
		if( weapon[notselectweapon] != NULL ){
			//���̕���̒e����0�łȂ����
			weapon[notselectweapon]->GetParamData(NULL, NULL, &nbs);
			if( nbs > 0 ){
				//�����ւ���
				ObjMgr->ChangeHaveWeapon(ctrlid);
				return 1;
			}
		}
	}

	return 0;
}

//! @brief �ړ�������]���������_���ɏI��
void AIcontrol::CancelMoveTurn()
{
	int forward, back, side, updown, rightleft;

	if( battlemode == AI_ACTION ){			//�U����
		if( MoveNavi->GetRun2() == true ){	//�D��I�ȑ���
			ObjDriver->DelModeFlag(AI_CTRL_MOVEFORWARD);
			ObjDriver->DelModeFlag(AI_CTRL_MOVEBACKWARD);
			ObjDriver->DelModeFlag(AI_CTRL_MOVELEFT);
			ObjDriver->DelModeFlag(AI_CTRL_MOVERIGHT);
			ObjDriver->DelModeFlag(AI_CTRL_TURNUP);
			ObjDriver->DelModeFlag(AI_CTRL_TURNDOWN);
			ObjDriver->DelModeFlag(AI_CTRL_TURNLEFT);
			ObjDriver->DelModeFlag(AI_CTRL_TURNRIGHT);
			if( GetRand(3) == 0 ){
				ObjDriver->DelModeFlag(AI_CTRL_MOVEWALK);
			}
			return;
		}
		else{									//�D��I�ȑ��� �ȊO
			if( longattack == false ){
				forward = 6;
				back = 6;
				side = 7;
				updown = 5;
				rightleft = 6;
			}
			else{
				forward = 5;
				back = 4;
				side = 5;
				updown = 3;
				rightleft = 3;
			}
		}
	}
	else if( battlemode == AI_CAUTION ){	//�x����
		forward = 10;
		back = 10;
		side = 10;
		updown = 14;
		rightleft = 20;
	}
	else{
		forward = 12;
		back = 12;
		side = 12;
		updown = 15;
		rightleft = 18;
	}

	//�ړ��������_���Ɏ~�߂�
	if( GetRand(forward) == 0 ){
		ObjDriver->DelModeFlag(AI_CTRL_MOVEFORWARD);
	}
	if( GetRand(back) == 0 ){
		ObjDriver->DelModeFlag(AI_CTRL_MOVEBACKWARD);
	}
	if( GetRand(side) == 0 ){
		ObjDriver->DelModeFlag(AI_CTRL_MOVELEFT);
	}
	if( GetRand(side) == 0 ){
		ObjDriver->DelModeFlag(AI_CTRL_MOVERIGHT);
	}
	if( GetRand(3) == 0 ){
		ObjDriver->DelModeFlag(AI_CTRL_MOVEWALK);
	}

	//��]�������_���Ɏ~�߂�
	if( GetRand(updown) == 0 ){
		ObjDriver->DelModeFlag(AI_CTRL_TURNUP);
	}
	if( GetRand(updown) == 0 ){
		ObjDriver->DelModeFlag(AI_CTRL_TURNDOWN);
	}
	if( GetRand(rightleft) == 0 ){
		ObjDriver->DelModeFlag(AI_CTRL_TURNLEFT);
	}
	if( GetRand(rightleft) == 0 ){
		ObjDriver->DelModeFlag(AI_CTRL_TURNRIGHT);
	}
}

//! @brief ����������[�h�E�̂Ă�
//! @return �̂Ă�F1�@�����[�h�F2�@�����ւ���F3�@FULL/SEMI�؂�ւ��F4�@�������Ȃ��F0
int AIcontrol::ControlWeapon()
{
	int selectweapon;
	class weapon *weapon[TOTAL_HAVEWEAPON];
	int weaponid, lnbs, nbs;
	WeaponParameter paramdata;
	int burstcntS, burstcntN;

	for(int i=0; i<TOTAL_HAVEWEAPON; i++){
		weapon[i] = NULL;
	}

	//����̏����擾
	ctrlhuman->GetWeapon(&selectweapon, weapon, NULL, NULL);
	if( weapon[selectweapon] == NULL ){ return 0; }
	weapon[selectweapon]->GetParamData(&weaponid, &lnbs, &nbs);

	//����̐��\���擾
	if( Param->GetWeapon(weaponid, &paramdata) == 1 ){ return 0; }

	//�X�R�[�v����
	ctrlhuman->SetDisableScope();

	//�u�P�[�X�v�Ȃ牽�����Ȃ�
	if( weaponid == ID_WEAPON_CASE ){ return 0; }

	//�퓬���ɃO���l�[�h�������Ă����
	if( battlemode == AI_ACTION ){
		if( weaponid == ID_WEAPON_GRENADE ){
			int nextselectweapon, nextnds;

			//���̕���̒e�����擾
			nextselectweapon = selectweapon + 1;
			nextnds = 0;
			if( nextselectweapon == TOTAL_HAVEWEAPON ){
				nextselectweapon = 0;
			}
			if( weapon[nextselectweapon] != NULL ){
				weapon[nextselectweapon]->GetParamData(NULL, NULL, &nextnds);
			}

			if( longattack == false ){
				// 1/100�̊m���Ŏ����ւ���
				if( (GetRand(100) == 0)&&(nextnds > 0) ){
					ObjMgr->ChangeHaveWeapon(ctrlid);
					return 3;
				}
			}
			else{
				// 1/66�̊m���Ŏ����ւ���
				if( (GetRand(66) == 0)&&(nextnds > 0) ){
					ObjMgr->ChangeHaveWeapon(ctrlid);
					return 3;
				}
			}
		}
	}

	//�c�e�����������
	if( lnbs == 0 ){
		int ways;

		//�����m������
		if( battlemode == AI_NORMAL ){ ways = 1; }
		else if( battlemode == AI_CAUTION ){ ways = 10; }
		else{ ways = 8; }

		// 1/ways�̊m���ŏ���
		if( GetRand(ways) == 0 ){
			int under;

			//�����[�h�m��
			if( battlemode == AI_NORMAL ){
				ways = 1;
				under = 0;
			}
			else if( battlemode == AI_CAUTION ){
				ways = 5;
				under = 3;
			}
			else{
				if( longattack == false ){
					// �m���́@3/4
					ways = 4;
					under = 2;
				}
				else{
					// �m���́@2/3
					ways = 3;
					under = 1;
				}
			}

			//�e��������Ύ̂Ă�
			if( nbs == 0 ){
				ObjMgr->DumpWeapon(ctrlid);
				return 1;
			}

			//�����_���� �����[�h���s or �������������
			if( GetRand(ways) <= under ){
				ObjMgr->ReloadWeapon(ctrlid);
				return 2;
			}
			//else{
				ObjMgr->ChangeHaveWeapon(ctrlid);
				return 3;
			//}
		}
	}

	//�A�ː؂�ւ����\�ȕ���Ȃ�
	if( paramdata.ChangeWeaponID != -1 ){
		//���݂̕���̘A�ːݒ���擾
		burstcntS = paramdata.burstcnt;

		//�V���ȕ���̘A�ːݒ���擾
		if( Param->GetWeapon(paramdata.ChangeWeaponID, &paramdata) == 1 ){ return 0; }
		burstcntN = paramdata.burstcnt;

		if( longattack == false ){
			//�ߋ����U�����ŁA����SEMI�E�؂�ւ����FULL�ɂȂ�Ȃ�
			if( (burstcntS == 1)&&(burstcntN != 1) ){
				ObjMgr->ChangeWeaponID(ctrlid);	//�؂�ւ���
				return 4;
			}
		}
		else{
			//�������U�����ŁA����FULL�E�؂�ւ����SEMI�ɂȂ�Ȃ�
			if( (burstcntS != 1)&&(burstcntN == 1) ){
				ObjMgr->ChangeWeaponID(ctrlid);	//�؂�ւ���
				return 4;
			}
		}
	}

	return 0;
}

//! @brief ��֒e�𓊂���
//! @return �������F0�@�����I������F1�@��֒e�������Ă��Ȃ��F2
//! @attention ��֒e�𓊂���p�X�ɉ����������ł��B�G�������̍U����Action()�֐����Q�Ƃ��Ă��������B
int AIcontrol::ThrowGrenade()
{
	int selectweapon;
	class weapon *weapon[TOTAL_HAVEWEAPON];
	int weaponid, nbs, i;
	pointdata pdata;
	float posy2;
	float atan_rx, atan_ry;

	for(int i=0; i<TOTAL_HAVEWEAPON; i++){
		weapon[i] = NULL;
	}

	//��֒e�������Ă��邩�m�F
	ctrlhuman->GetWeapon(&selectweapon, weapon, NULL, NULL);
	for(i=0; i<TOTAL_HAVEWEAPON; i++){
		if( weapon[i] != NULL ){
			weapon[i]->GetParamData(&weaponid, NULL, &nbs);
			if( weaponid == ID_WEAPON_GRENADE ){
				break;
			}
		}
	}

	//��֒e�ɐ؂�ւ�����Ȃ�A�؂�ւ���
	if( (i != TOTAL_HAVEWEAPON)&&(i != selectweapon) ){
		ObjMgr->ChangeHaveWeapon(ctrlid, i);
	}

	//�p�X�Ɛl�̍������擾
	MoveNavi->GetPathPointData(&pdata);
	posy2 = posy + VIEW_HEIGHT;

	//��x�S�Ă̓������~�߂�
	ObjDriver->ResetMode();

	//�ڕW�n�_�ւ̊p�x�����߂�
	CheckTargetAngle(posx, posy2, posz, rx*-1 + (float)M_PI/2, ry, pdata.x, pdata.y, pdata.z, 0.0f, &atan_rx, &atan_ry, NULL);

	//��֒e�������Ă��Ȃ����
	if( i == TOTAL_HAVEWEAPON ){
		//����֒e�������Ă��Ȃ��ꍇ�ł��A���̃t���[���ł͂��̕����֌�������悤��������B
		//�@���̏����ɂ��A��֒e�������Ă��Ȃ���Ԃ� ����֒e�����̖������[�v�Ɋׂ����ꍇ�A��֒e�����p�X�̕��p������������B
		//�@�i�{��XOPS�̎d�l�j

		//����
		if( atan_rx > 0.0f ){
			ObjDriver->SetModeFlag(AI_CTRL_TURNLEFT);
		}
		if( atan_rx < 0.0f ){
			ObjDriver->SetModeFlag(AI_CTRL_TURNRIGHT);
		}
		ArmAngle();

		return 2;
	}

	//����
	if( atan_rx > 0.0f ){
		ObjDriver->SetModeFlag(AI_CTRL_TURNLEFT);
	}
	if( atan_rx < 0.0f ){
		ObjDriver->SetModeFlag(AI_CTRL_TURNRIGHT);
	}
	if( atan_ry > 0.0f ){
		ObjDriver->SetModeFlag(AI_CTRL_TURNUP);
	}
	if( atan_ry < 0.0f ){
		ObjDriver->SetModeFlag(AI_CTRL_TURNDOWN);
	}

	//�Ə��ɒ�܂��Ă��邩�m�F
	if( (fabsf(atan_rx) < DegreeToRadian(1.5f))&&(fabsf(atan_ry) < DegreeToRadian(1.5f)) ){
		//�A�˂����Ȃ�
		if( (ctrlhuman->GetWeaponBlazingmode() == false)&&(EventWeaponShot == true) ){ return 0; }

		//�p�x��ݒ�
		ctrlhuman->SetRxRy(rx, ry);

		//������
		if( ObjMgr->ShotWeapon(ctrlid) != 0 ){
			EventWeaponShot = true;
			return 1;
		}
	}

	return 0;
}

//! @brief �r�̊p�x��ݒ�
void AIcontrol::ArmAngle()
{
	ObjDriver->DelModeFlag(AI_CTRL_TURNUP);
	ObjDriver->DelModeFlag(AI_CTRL_TURNDOWN);

	if( ctrlhuman->GetMainWeaponTypeNO() == ID_WEAPON_NONE ){	//��Ԃ�
		//���Ɍ���������
		ObjDriver->SetModeFlag(AI_CTRL_TURNDOWN);
	}
	else if( (battlemode == AI_CAUTION)&&(cautioncnt > 0) ){	//�x����
		float addry2 = 0.0f - ry;

		//����
		if( addry2 > DegreeToRadian(1.0f) ){
			ObjDriver->SetModeFlag(AI_CTRL_TURNUP);
		}
		if( addry2 < DegreeToRadian(-1.0f) ){
			ObjDriver->SetModeFlag(AI_CTRL_TURNDOWN);
		}
	}
	else{									//���펞�ŕ��폊�L��
		//����
		if( ry < DegreeToRadian(-32) ){
			ObjDriver->SetModeFlag(AI_CTRL_TURNUP);
		}
		if( ry > DegreeToRadian(-28) ){
			ObjDriver->SetModeFlag(AI_CTRL_TURNDOWN);
		}
	}
}

//! @brief �G��T��
int AIcontrol::SearchEnemy()
{
	//��퓬���t���O���L���Ȃ�G�������Ȃ�
	if( NoFight == true ){
		return 0;
	}

	if( battlemode == AI_ACTION ){ return 0; }

	int weaponid = ctrlhuman->GetMainWeaponTypeNO();
	int weaponscope = 0;
	ScopeParameter sparam;
	int searchloops;
	float maxDist;
	float A_rx, A_ry, B_rx, B_ry;

	//����������Ă���΁A�X�R�[�v���[�h���擾
	if( weaponid != ID_WEAPON_NONE ){
		WeaponParameter Wparam;

		Param->GetWeapon(weaponid, &Wparam);
		weaponscope = Wparam.scopemode;
	}

	//�G�̒T���񐔂ƒT���͈́i�����Ɗp�x�j��ݒ�
	if( battlemode == AI_NORMAL ){
		searchloops = (LevelParam->search) * AI_TOTALHUMAN_SCALE;

		Param->GetScopeParam(weaponscope, &sparam);
		maxDist = sparam.AI_AddSearchEnemyMaxDist_Normal;
		maxDist += 12.0f*((LevelParam->search)-2) + 350.0f;
		A_rx = DegreeToRadian(110);
		A_ry = DegreeToRadian(60);
		B_rx = DegreeToRadian(60);
		B_ry = DegreeToRadian(40);
	}
	else {	//battlemode == AI_CAUTION
		searchloops = (LevelParam->search) * AI_TOTALHUMAN_SCALE + 4;

		Param->GetScopeParam(weaponscope, &sparam);
		maxDist = sparam.AI_AddSearchEnemyMaxDist_Caution;
		maxDist += 15.0f*((LevelParam->search)-2) + 420.0f;
		A_rx = DegreeToRadian(130);
		A_ry = DegreeToRadian(80);
		B_rx = DegreeToRadian(80);
		B_ry = DegreeToRadian(50);
	}

	//�w��񐔁A�G��T��
	for(int i=0; i<searchloops; i++){
		int targetid = GetRand(MAX_HUMAN);

		if( CheckLookEnemy(targetid, A_rx, A_ry, AI_CHECKSHORTATTACK_DIST, NULL) == true ){
			longattack = false;
			return 1;
		}

		if( CheckLookEnemy(targetid, B_rx, B_ry, maxDist, NULL) == true ){
			if( GetRand(4) == 0 ){
				if( MoveNavi->GetRun2() == true ){ longattack = false; }
				else{ longattack = true; }
				return 2;
			}
		}
	}
	return 0;
}

//! @brief �G��T���i�������U�����ɋߋ�����T���j
int AIcontrol::SearchShortEnemy()
{
	float A_rx, A_ry;
	A_rx = DegreeToRadian(100);
	A_ry = DegreeToRadian(52);

	for(int i=0; i<3; i++){
		int targetid = GetRand(MAX_HUMAN);

		if( CheckLookEnemy(targetid, A_rx, A_ry, AI_CHECKSHORTATTACK_DIST, NULL) == true ){
			longattack = false;
			return 1;
		}
	}
	return 0;
}

//! @brief �G�������邩����
bool AIcontrol::CheckLookEnemy(int id, float search_rx, float search_ry, float maxDist, float *out_minDist)
{
	if( ObjMgr == NULL ){ return false; }

	//�l�̃I�u�W�F�N�g���擾
	class human* thuman;
	thuman = ObjMgr->GetHumanObject(id);
	if( thuman == NULL ){ return false; }

	//�����֐����I�[�o�[���[�h
	return CheckLookEnemy(thuman, search_rx, search_ry, maxDist, out_minDist);
}

//! @brief �G�������邩����
bool AIcontrol::CheckLookEnemy(class human* thuman, float search_rx, float search_ry, float maxDist, float *out_minDist)
{
	//return false;

	//�����ȃf�[�^�Ȃ�I��
	if( ctrlhuman == NULL ){ return false; }
	if( thuman == NULL ){ return false; }
	if( thuman->GetEnableFlag() == false ){ return false; }
	if( thuman->GetDeadFlag() == true ){ return false; }

	//�����ƓG�̍��W���擾
	int ctrlteam, targetteam;
	float tx, ty, tz;
	ctrlhuman->GetParamData(NULL, NULL, NULL, &ctrlteam);
	thuman->GetParamData(NULL, NULL, NULL, &targetteam);
	thuman->GetPosData(&tx, &ty, &tz, NULL);

	//�����ƓG������l���łȂ����
	if( ctrlteam != targetteam ){
		float mrx, mry;
		float Dist2 , Dist;
		float Dist_dummy; 

		//�����𔻒肵�A�p�x���擾
		if( CheckTargetAngle(posx, posy, posz, rx*-1 + (float)M_PI/2, 0.0f, tx, ty, tz, maxDist, &mrx, &mry, &Dist2) == true ){
			//�p�x��A���E�ɓ����Ă����
			if( (fabsf(mrx) < search_rx/2)&&(fabsf(mry) < search_ry/2) ){
				float vx, vy, vz;

				Dist = sqrtf(Dist2);

				//�x�N�g�����擾
				vx = (tx - posx)/Dist;
				vy = (ty - posy)/Dist;
				vz = (tz - posz)/Dist;

				//�u���b�N���Ղ��Ă��Ȃ���΁@�i���C�œ����蔻����s���A�������Ă��Ȃ���΁j
				if( CollD->CheckALLBlockIntersectRay(posx, posy + VIEW_HEIGHT, posz, vx, vy, vz, NULL, NULL, &Dist_dummy, Dist) == false ){
					if( out_minDist != NULL ){ *out_minDist = Dist; }
					enemyhuman = thuman;
					return true;
				}
			}
		}
	}
	return false;
}

//! @brief ���̂����邩�m�F
bool AIcontrol::CheckCorpse(int id)
{
	//�N���X�ݒ肪����������Ώ������Ȃ�
	if( ObjMgr == NULL ){ return false; }
	if( ctrlhuman == NULL ){ return false; }

	if( (id < 0)||(MAX_HUMAN <= id) ){ return false; }

	//�^�[�Q�b�g�̃N���X���擾
	class human* thuman;
	thuman = ObjMgr->GetHumanObject(id);
	if( thuman == NULL ){ return false; }
	if( thuman->GetEnableFlag() == false ){ return false; }

	//���S���Ă����
	if( thuman->GetDeadFlag() == true ){

		//�`�[���ԍ����擾
		int ctrlteam, targetteam;
		ctrlhuman->GetParamData(NULL, NULL, NULL, &ctrlteam);
		thuman->GetParamData(NULL, NULL, NULL, &targetteam);

		//�����Ȃ��
		if( ctrlteam == targetteam ){
			float tposx, tposy, tposz;
			float atan;

			//���W���狗�����擾
			thuman->GetPosData(&tposx, &tposy, &tposz, NULL);

			//�����Ɗp�x���v�Z
			if( CheckTargetAngle(posx, posy, posz, rx*-1 + (float)M_PI/2, 0.0f, tposx, tposy, tposz, 22.0f, &atan, NULL, NULL) == true ){
				if( fabsf(atan) < DegreeToRadian(40) ){
					return true;
				}
			}
		}
	}
	return false;
}

//! @brief �p�X�ɂ��ړ�
void AIcontrol::MovePath()
{
	int movemode, pointmode;
	MoveNavi->GetTargetPos(NULL, NULL, NULL, &movemode, &pointmode);

	if( movemode == AI_NAVI_MOVE_NULL ){			//�ُ�ȃp�X
		return;
	}

	if( CheckTargetPos(false) == false ){
		MoveTarget(false);
	}
	else{
		if( (pointmode == AI_NAVI_POINT_WAIT)||(pointmode == AI_NAVI_POINT_TRACKING) ){
			TurnSeen();
		}
		else if( pointmode == AI_NAVI_POINT_STOP_5SEC ){
			if( waitcnt < ((int)GAMEFPS)*5 ){
				if( StopSeen() == true ){
					waitcnt += 1;
				}
			}
			else{
				waitcnt = 0;
				MoveNavi->MovePathNextState();
				MoveNavi->MovePathNowState();
			}
		}
		else{	//pointmode == AI_NAVI_POINT_NULL
			waitcnt = 0;
			MoveNavi->MovePathNextState();
			MoveNavi->MovePathNowState();
		}
	}
}

//! @brief �U�����C������
//! @return �s�ρFfalse�@�ύX�Ftrue
bool AIcontrol::ActionMain()
{
	int newbattlemode = AI_ACTION;

	//�U������
	Action();

	//�����_���p�X�Ȃ珈�����s�@���u�D��I�ȑ���v��z��
	if( MoveNavi->GetMoveMode() == AI_RANDOM ){
		MoveNavi->MovePathNextState();
		MoveNavi->MovePathNowState();
	}

	if( MoveNavi->GetRun2() == true ){				//�D��I�ȑ���
		//�ڕW�n�_�ֈړ�
		if( CheckTargetPos(false) == true ){
			//newbattlemode = AI_NORMAL;

			//���̖ڕW�n�_���A�x����ԉ������̖߂�ʒu�ɐݒ肷��B
			MoveNavi->GetTargetPos(&cautionback_posx, &cautionback_posz, NULL, NULL, NULL);

			MoveNavi->MovePathNextState();
			MoveNavi->MovePathNowState();
		}
		else{
			MoveTarget2(false);
		}
	}
	else{									//�D��I�ȑ��� �ȊO
		//�������U�����Ȃ�A�߂��̓G��T��
		if( longattack == true ){
			SearchShortEnemy();
		}
	}

	//�U���I������
	if( ActionCancel() == true ){
		enemyhuman = NULL;

		if( MoveNavi->GetRun2() == true ){
			newbattlemode = AI_NORMAL;
		}
		else{
			newbattlemode = AI_CAUTION;
			cautioncnt = 160;
			FaceCaution_flag = false;
			FaceCaution_rx = 0.0f;
		}
	}

	//�ݒ�𔻒�
	if( battlemode != newbattlemode ){
		battlemode = newbattlemode;
		return true;
	}
	return false;
}

//! @brief �x�����C������
//! @return �s�ρFfalse�@�ύX�Ftrue
bool AIcontrol::CautionMain()
{
	int newbattlemode = AI_CAUTION;
	float caution_rx;

	//���W�ƃ`�[���ԍ����擾
	int teamid;
	ctrlhuman->GetParamData(NULL, NULL, NULL, &teamid);

	//��e�Ɖ��̏󋵂��擾
	bool HitFlag = ctrlhuman->CheckHit(&caution_rx);
	soundlist soundlist[MAX_SOUNDMGR_LIST];
	int soundlists = GameSound->GetWorldSound(posx, posy + VIEW_HEIGHT, posz, teamid, soundlist);

	//��퓬���t���O���L���Ȃ�A���͕������Ȃ����Ƃɂ���
	if( NoFight == true ){
		soundlists = 0;
	}

	//���C������
	if( enemyhuman != NULL ){		//���ɓG�������Ă����
		newbattlemode = AI_ACTION;
		actioncnt = 0;
	}
	else if( SearchEnemy() != 0 ){	//�G���������
		newbattlemode = AI_ACTION;
		actioncnt = 0;
	}
	else if( HitFlag == true ){		//��e����
		cautioncnt = 160;					//�x�����ĊJ
		FaceCaution_flag = true;
		FaceCaution_rx = caution_rx;
	}
	else if( soundlists > 0 ){		//������������
		cautioncnt = 160;					//�x�����ĊJ
	}
	else if( cautioncnt == 0 ){		//�x�����I������Ȃ�
		if( CheckTargetPos(true) == false ){				//�x���J�n�n�_��藣��Ă��邩
			MoveTarget(true);				//�x���J�n�n�_�ɋ߂Â�
		}
		else{
			newbattlemode = AI_NORMAL;
			FaceCaution_flag = false;
			FaceCaution_rx = 0.0f;

			//�x���҂��p�X�Ȃ玟�֐i�߂�
			pointdata pdata;
			MoveNavi->GetPathPointData(&pdata);
			if( (pdata.p1 == POINT_P1TYPE_AIPATH)&&(pdata.p2 == 4) ){
				MoveNavi->MovePathNextState();
				MoveNavi->MovePathNowState();
			}
		}
	}
	else if( (cautioncnt < 100)&&(FaceCaution_flag == false) ){		//100�t���[����؂�����A�����_���Ɍx���I���i�J�E���g�F0�Ɂj
		if( GetRand(50) == 0 ){ cautioncnt = 0; }
	}
	else{ cautioncnt -= 1; }

	//�ǔ����őΏۂ��痣�ꂷ������A�����_���Ɍx���I��
	if( (MoveNavi->GetMoveMode() == AI_TRACKING)&&(GetRand(3) == 0) ){
		float x, z;
		float tx, tz;
		class human *targethuman = ObjMgr->GetHumanObject(MoveNavi->GetTargetHumanID());
		targethuman->GetPosData(&tx, NULL, &tz, NULL);
		x = posx - tx;
		z = posz - tz;
		if( (x*x + z*z) > 25.0f*25.0f ){
			cautioncnt = 0;
		}
	}

	//��]�Ƙr�̊p�x
	TurnSeen();
	ArmAngle();

	//�ݒ�𔻒�
	if( battlemode != newbattlemode ){
		battlemode = newbattlemode;
		return true;
	}
	return false;
}

//! @brief �ʏ탁�C������
//! @return �s�ρFfalse�@�ύX�Ftrue
bool AIcontrol::NormalMain()
{
	int newbattlemode = AI_NORMAL;
	float caution_rx;
	int EventThrowGrenade = 0;

	MoveNavi->MovePathNowState();
	enemyhuman = NULL;

	//���W�ƃ`�[���ԍ����擾
	int teamid;
	ctrlhuman->GetParamData(NULL, NULL, NULL, &teamid);

	//��e�Ɖ��̏󋵂��擾
	bool HitFlag = ctrlhuman->CheckHit(&caution_rx);
	soundlist soundlist[MAX_SOUNDMGR_LIST];
	int soundlists = GameSound->GetWorldSound(posx, posy + VIEW_HEIGHT, posz, teamid, soundlist);

	//��퓬���t���O���L���Ȃ�A���͕������Ȃ����Ƃɂ���
	if( NoFight == true ){
		soundlists = 0;
	}

	//�����_���p�X�Ȃ珈�����s
	if( MoveNavi->GetMoveMode() == AI_RANDOM ){
		MoveNavi->MovePathNextState();
		MoveNavi->MovePathNowState();
	}

	//��֒e�p�X
	if( MoveNavi->GetMoveMode() == AI_GRENADE ){
		EventThrowGrenade = ThrowGrenade();
		if( EventThrowGrenade != 0 ){
			MoveNavi->MovePathNextState();
			MoveNavi->MovePathNowState();
		}
	}
	if( EventThrowGrenade != 1 ){
		EventWeaponShot = false;
	}

	if( MoveNavi->GetMoveMode() == AI_RUN2 ){		//�D��I�ȑ���̏���
		//�G����������U���ɓ���
		if( SearchEnemy() != 0 ){
			newbattlemode = AI_ACTION;
			//cautionback_posx = posx;
			//cautionback_posz = posz;
		}
		else{
			MovePath();		//�ړ����s
		}
	}
	else{							//�D��I�ȑ���ȊO�̏���
		//�x������ɓ��鏈��
		if( HitFlag == true ){							//��e����
			newbattlemode = AI_CAUTION;
			cautioncnt = 160;
			cautionback_posx = posx;
			cautionback_posz = posz;
			FaceCaution_flag = true;
			FaceCaution_rx = caution_rx;
		}
		else if(
			(SearchEnemy() != 0)||						//�G��������
			(soundlists > 0)||							//������������
			(CheckCorpse( GetRand(MAX_HUMAN) ) == true)	//���̂�������
		){
			newbattlemode = AI_CAUTION;
			cautioncnt = 160;
			cautionback_posx = posx;
			cautionback_posz = posz;
			FaceCaution_flag = false;
			FaceCaution_rx = 0.0f;
		}
		else{
			MovePath();		//�ړ����s
		}
	}

	//�r�̊p�x��ݒ�
	if( MoveNavi->GetMoveMode() != AI_GRENADE ){
		ArmAngle();
	}

	//�ݒ�𔻒�
	if( battlemode != newbattlemode ){
		battlemode = newbattlemode;
		return true;
	}
	return false;
}

//! @brief �������n�֐�
void AIcontrol::Init()
{
	//�N���X�ݒ肪����������Ώ������Ȃ�
	if( ctrlhuman == NULL ){ return; }
	if( blocks == NULL ){ return; }
	if( CollD == NULL ){ return; }

	//�g�p����Ă��Ȃ��l�Ȃ珈�����Ȃ�
	if( ctrlhuman->GetEnableFlag() == false ){ return; }

	//�X�e�[�g��������
	NoFight = false;
	battlemode = AI_NORMAL;
	enemyhuman = NULL;
	FaceCaution_flag = false;
	FaceCaution_rx = 0.0f;
	total_move = 0.0f;
	waitcnt = 0;
	gotocnt = 0;
	cautioncnt = 0;
	actioncnt = 0;
	longattack = false;
	EventWeaponShot = false;

	MoveNavi->Init();
	ObjDriver->Init();

	//AI���x���Ɛݒ�l���擾
	int paramid;
	HumanParameter paramdata;
	ctrlhuman->GetParamData(&paramid, NULL, NULL, NULL);
	Param->GetHuman(paramid, &paramdata);
	AIlevel = paramdata.AIlevel;
	Param->GetAIlevel(AIlevel, &LevelParam);

	//���̃|�C���g������
	//MoveNavi->MovePathNextState();
	MoveNavi->MovePathNowState();
}

//! @brief AI���x����ݒ�
//! @param level AI���x��
void AIcontrol::SetAIlevel(int level)
{
	if( (level < -1)||(TOTAL_PARAMETERINFO_AILEVEL <= level) ){ return; }

	if( level == -1 ){
		//AI���x�����擾
		int paramid;
		HumanParameter paramdata;
		ctrlhuman->GetParamData(&paramid, NULL, NULL, NULL);
		Param->GetHuman(paramid, &paramdata);
		AIlevel = paramdata.AIlevel;
	}
	else{
		AIlevel = level;
	}

	//�ݒ�l���擾
	Param->GetAIlevel(AIlevel, &LevelParam);
}

//! @brief AI���x�����擾
//! @return AI���x��
int AIcontrol::GetAIlevel()
{
	return AIlevel;
}

//! @brief �w�肵���ꏊ�֑ҋ@������
//! @param px X���W
//! @param pz Z���W
//! @param rx �d���������
//! @attention �ړ��p�X�Ɋւ�炸�A�w�肵�����W�ւ̑ҋ@���������܂��BInit()�֐����ēx���s����܂Ō��ɖ߂��܂���B
void AIcontrol::SetHoldWait(float px, float pz, float rx)
{
	MoveNavi->SetHoldWait(px, pz, rx);
}

//! @brief �w�肵���l��ǔ�������
//! @param id �l�̃f�[�^�ԍ�
//! @attention �ړ��p�X�Ɋւ�炸�A�w�肵���l�ւ̒ǔ����������܂��BInit()�֐����ēx���s����܂Ō��ɖ߂��܂���B
void AIcontrol::SetHoldTracking(int id)
{
	if( (0 <= id)&&(id < MAX_HUMAN) ){ MoveNavi->SetHoldTracking(id); }
}

//! @brief �����I�Ɍx��������
//! @warning �D��I�ȑ��� �����s���̏ꍇ�A���̊֐��͉������܂���B
void AIcontrol::SetCautionMode()
{
	//�D��I�ȑ���Ȃ�Ή������Ȃ�
	if( MoveNavi->GetRun2() == true ){ return; }

	if( battlemode == AI_NORMAL ){
		cautionback_posx = posx;
		cautionback_posz = posz;
	}
	battlemode = AI_CAUTION;
	cautioncnt = 160;
	FaceCaution_flag = false;
	FaceCaution_rx = 0.0f;
}

//! @brief ��퓬���t���O��ݒ�
//! @param flag true�F�퓬���s��Ȃ��i��퓬���j�@false�F�퓬���s���i�ʏ�j
//! @attention �t���O��L���ɂ���ƓG��F�����Ȃ��Ȃ�܂��B
void AIcontrol::SetNoFightFlag(bool flag)
{
	NoFight = flag;
}

//! @brief �퓬���[�h�擾
//! @param *mode �퓬���[�h�i�����j���󂯎��|�C���^
//! @param *modestr �퓬���[�h���̕�������󂯎��|�C���^
void AIcontrol::GetBattleMode(int *mode, char *modestr)
{
	if( mode != NULL ){
		*mode = battlemode;
	}
	if( modestr != NULL ){
		switch(battlemode){
			case 0: strcpy(modestr, "AI_DEAD"); break;
			case 1: strcpy(modestr, "AI_ACTION"); break;
			case 2: strcpy(modestr, "AI_CAUTION"); break;
			case 3: strcpy(modestr, "AI_NORMAL"); break;
			default: strcpy(modestr, "");
		}
	}
}

//! @brief �U���Ώۂ̐l�f�[�^�ԍ�
//! @return �l�f�[�^�ԍ��i�^�[�Q�b�g�����Ȃ��ꍇ�� -1�j
int AIcontrol::GetEnemyHumanID()
{
	if( enemyhuman == NULL ){
		return -1;
	}
	//else{
		return ObjMgr->GetHumanObjectID(enemyhuman);
	//}
}

//! @brief �ړ�����ڕW�n�_�i�^�[�Q�b�g�j���擾
//! @param posx �ڕW�n�_��X���W���󂯎��|�C���^
//! @param posz �ڕW�n�_��Z���W���󂯎��|�C���^
//! @param movemode �ڕW�n�_�ւ̈ړ����[�h���󂯎��|�C���^
void AIcontrol::GetMoveTargetPos(float *posx, float *posz, int *movemode)
{
	MoveNavi->GetTargetPos(posx, posz, NULL, movemode, NULL);

	//�u�D��I�ȑ���v�ł͂Ȃ��{�U�����E�x�����Ȃ�΁A�x����ɖ߂�ʒu��Ԃ��B
	if( MoveNavi->GetRun2() == false ){
		if( (battlemode == AI_ACTION)||(battlemode == AI_CAUTION) ){
			*posx = cautionback_posx;
			*posz = cautionback_posz;
		}
	}
}

//! @brief ���ݓǂݍ���ł���|�C���g�f�[�^�p�X
//! @param out_data �|�C���g�f�[�^���󂯎��|�C���^
void AIcontrol::GetPathPointData(pointdata *out_data)
{
	MoveNavi->GetPathPointData(out_data);
}

//! @brief �����n�֐�
void AIcontrol::Process()
{
	//�N���X�ݒ肪����������Ώ������Ȃ�
	if( ctrlhuman == NULL ){ return; }
	if( blocks == NULL ){ return; }
	if( CollD == NULL ){ return; }

	//�����Ȑl�N���X�Ȃ珈�����Ȃ�
	if( ctrlhuman->GetEnableFlag() == false ){ return; }

	//���S������
	if( ctrlhuman->GetHP() <= 0 ){
		battlemode = AI_DEAD;
		return;
	}

	//HP��0�łȂ��̂� ���S�������ƂɂȂ��Ă�@�E�E�E�����Ԃ����H
	if( battlemode == AI_DEAD ){
		battlemode = AI_NORMAL;
		MoveNavi->MovePathNowState();
	}

	//���W�Ɗp�x���擾
	ctrlhuman->GetPosData(&posx, &posy, &posz, NULL);
	ctrlhuman->GetRxRy(&rx, &ry);

	//�����_���ɓ�����~�߂�
	CancelMoveTurn();

	//�U�������x�����Ȃ��
	if( (battlemode == AI_ACTION)||(battlemode == AI_CAUTION) ){
		//���������
		HaveWeapon();
	}

	//��v�Z���s
	if( battlemode == AI_ACTION ){			//�U����
		ActionMain();
	}
	else if( battlemode == AI_CAUTION ){	//�x����
		CautionMain();
	}
	else{									//���펞
		NormalMain();
	}

	//�ړ��E�����]������
	ObjDriver->ControlObject();

	//�������舵��
	ControlWeapon();
}

//! @brief �R���X�g���N�^
AIMoveNavi::AIMoveNavi(class ObjectManager *in_ObjMgr, int in_ctrlid, class PointDataInterface *in_Points)
{
	ObjMgr = in_ObjMgr;
	ctrlid = in_ctrlid;
	Points = in_Points;

	movemode = AI_WAIT;
	hold = false;
	path_pointid = 0;
	target_humanid = -1;
	target_posx = 0.0f;
	target_posz = 0.0f;
	target_rx = 0.0f;
}

//! @brief �f�B�X�g���N�^
AIMoveNavi::~AIMoveNavi()
{}

//! @brief �ΏۃN���X��ݒ�
//! @attention ���̊֐��Őݒ���s��Ȃ��ƁA�N���X���̂��������@�\���܂���B
void AIMoveNavi::SetClass(class ObjectManager *in_ObjMgr, int in_ctrlid, class PointDataInterface *in_Points)
{
	ObjMgr = in_ObjMgr;
	ctrlid = in_ctrlid;
	Points = in_Points;
}

//! @brief �������n�֐�
void AIMoveNavi::Init()
{
	movemode = AI_NULL;
	hold = false;
	path_pointid = 0;
	target_humanid = -1;

	ObjMgr->GetHumanObject(ctrlid)->GetParamData(NULL, &path_pointid, NULL, NULL);
	MovePathNextState();
}

//! @brief �ړ�����ڕW�n�_��K�p
//! @return �����Ftrue�@���s�Ffalse
//! @attention �ǔ����ɑΏېl���̍��W�𔽉f�����邽�߁A�{�֐��𖈃t���[���Ăяo���āA�ŐV�̈ʒu�����v�Z����K�v������܂��B
bool AIMoveNavi::MovePathNowState()
{
	if( hold == true ){
		if( movemode == AI_TRACKING ){
			class human *targethuman;
			targethuman = ObjMgr->GetHumanObject(target_humanid);
			targethuman->GetPosData(&target_posx, NULL, &target_posz, &target_rx);
		}
		return true;
	}


	//�|�C���g�̏����擾
	pointdata pdata;
	if( Points->Getdata(&pdata, path_pointid) != 0 ){
		movemode = AI_NULL;
		return false;
	}

	//�ړ��p�X�Ȃ�`
	if( pdata.p1 == POINT_P1TYPE_AIPATH ){
		//�ړ��X�e�[�g�ݒ�
		switch(pdata.p2){
			case 0: movemode = AI_WALK; break;
			case 1: movemode = AI_RUN; break;
			case 2: movemode = AI_WAIT; break;
			case 3: movemode = AI_TRACKING; break;
			case 4: movemode = AI_WAIT; break;
			case 5: movemode = AI_STOP_5SEC; break;
			case 6: movemode = AI_GRENADE; break;
			case 7: movemode = AI_RUN2; break;
			default: break;
		}

		if( movemode == AI_TRACKING ){
			class human *targethuman;

			if( target_humanid == -1 ){
				signed short int nextpointp4 = pdata.p3;

				//�|�C���g�i�l�j�̏����擾
				if( Points->SearchPointdata(&pdata, 0x08, 0, 0, 0, nextpointp4, 0) == 0 ){
					return false;
				}

				//�l���������ăN���X���擾
				targethuman = ObjMgr->SearchHuman(pdata.p4);
				if( targethuman == NULL ){ return false; }

				//�l�̃f�[�^�ԍ����擾
				target_humanid = ObjMgr->GetHumanObjectID(targethuman);
			}

			targethuman = ObjMgr->GetHumanObject(target_humanid);
			targethuman->GetPosData(&target_posx, NULL, &target_posz, &target_rx);
		}
		else{
			//���K�p
			target_posx = pdata.x;
			target_posz = pdata.z;
			target_rx = pdata.r;
		}

		return true;
	}

	//�����_���p�X�Ȃ�
	if( pdata.p1 == POINT_P1TYPE_RAND_AIPATH ){
		movemode = AI_RANDOM;
		return false;
	}

	movemode = AI_NULL;
	return false;
}

//! @brief ���̖ڕW�n�_������
//! @return �����Ftrue�@���s�Ffalse
bool AIMoveNavi::MovePathNextState()
{
	//�|�C���g�̏����擾
	pointdata pdata;
	if( Points->Getdata(&pdata, path_pointid) != 0 ){
		movemode = AI_NULL;
		return false;
	}

	signed short int nextpointp4 = pdata.p3;

	//�����_���p�X����
	if( pdata.p1 == POINT_P1TYPE_RAND_AIPATH ){
		if( GetRand(2) == 0 ){
			nextpointp4 = pdata.p2;
		}
		else{
			nextpointp4 = pdata.p3;
		}
		movemode = AI_RANDOM;
	}

	//�|�C���g������
	if( Points->SearchPointdata(&pdata, 0x08, 0, 0, 0, nextpointp4, 0) == 0 ){
		return false;
	}

	path_pointid = pdata.id;

	return true;
}

//! @brief �w�肵���ꏊ�֑ҋ@������
//! @param px X���W
//! @param pz Z���W
//! @param rx �d���������
//! @attention �ړ��p�X�Ɋւ�炸�A�w�肵�����W�ւ̑ҋ@���������܂��BInit()�֐����ēx���s����܂Ō��ɖ߂��܂���B
void AIMoveNavi::SetHoldWait(float px, float pz, float rx)
{
	movemode = AI_WAIT;
	hold = true;
	target_posx = px;
	target_posz = pz;
	target_rx = rx;
}

//! @brief �w�肵���l��ǔ�������
//! @param id �l�̃f�[�^�ԍ�
//! @attention �ړ��p�X�Ɋւ�炸�A�w�肵���l�ւ̒ǔ����������܂��BInit()�֐����ēx���s����܂Ō��ɖ߂��܂���B
void AIMoveNavi::SetHoldTracking(int id)
{
	if( (id < 0)||(MAX_HUMAN <= id) ){ return; }

	movemode = AI_TRACKING;
	hold = true;
	target_humanid = id;
}

//! @brief �ړ����[�h���擾
//! @return �ړ����[�h
int AIMoveNavi::GetMoveMode()
{
	return movemode;
}

//! @brief �D��I�ȑ���t���O���擾
//! @return �D��I�ȑ���ł���Ftrue�@�D��I�ȑ���łȂ��Ffalse
bool AIMoveNavi::GetRun2()
{
	if( movemode == AI_RUN2 ){
		return true;
	}
	//else{
		return false;
	//}
}

//! @brief �^�[�Q�b�g�i�l�j�̃f�[�^�ԍ����擾
//! @return �l�̃f�[�^�ԍ�
int AIMoveNavi::GetTargetHumanID()
{
	return target_humanid;
}

//! @brief ���ݓǂݍ���ł���|�C���g�f�[�^�p�X
//! @param out_data �|�C���g�f�[�^���󂯎��|�C���^
void AIMoveNavi::GetPathPointData(pointdata *out_data)
{
	Points->Getdata(out_data, path_pointid);
}

//! @brief �ړ�����ڕW�n�_�i�^�[�Q�b�g�j���擾
//! @param posx �ڕW�n�_��X���W���󂯎��|�C���^
//! @param posz �ڕW�n�_��Z���W���󂯎��|�C���^
//! @param rx �ڕW�n�_�̐����p�x���󂯎��|�C���^
//! @param out_movemode �ڕW�n�_�ւ̈ړ����[�h���󂯎��|�C���^
//! @param out_pointmode �ڕW�n�_�̃|�C���g���[�h���󂯎��|�C���^
//! @warning �{�֐��ł͍��W�͍X�V����܂���B��� MovePathNowState() �֐��ō��W���X�V������A�{�֐����Ăяo���Ă��������B
void AIMoveNavi::GetTargetPos(float *posx, float *posz, float *rx, int *out_movemode, int *out_pointmode)
{
	if( posx != NULL ){ *posx = target_posx; }
	if( posz != NULL ){ *posz = target_posz; }
	if( rx != NULL ){ *rx = target_rx; }

	if( out_movemode != NULL ){
		if( (movemode == AI_WALK)||(movemode == AI_WAIT)||(movemode == AI_STOP_5SEC) ){
			*out_movemode = AI_NAVI_MOVE_WALK;
		}
		else if( movemode == AI_RUN ){
			*out_movemode = AI_NAVI_MOVE_RUN;
		}
		else if( movemode == AI_RUN2 ){
			*out_movemode = AI_NAVI_MOVE_RUN2;
		}
		else if( movemode == AI_TRACKING ){
			*out_movemode = AI_NAVI_MOVE_TRACKING;
		}
		else{
			*out_movemode = AI_NAVI_MOVE_NULL;
		}
	}

	if( out_pointmode != NULL ){
		if( movemode == AI_WAIT ){
			*out_pointmode = AI_NAVI_POINT_WAIT;
		}
		else if( movemode == AI_STOP_5SEC ){
			*out_pointmode = AI_NAVI_POINT_STOP_5SEC;
		}
		else if( movemode == AI_TRACKING ){
			*out_pointmode = AI_NAVI_POINT_TRACKING;
		}
		else if( movemode == AI_GRENADE ){	//��֒e�p�X
			*out_pointmode = AI_NAVI_POINT_GRENADE;
		}
		else{								//�ړ��p�X�E�ُ�ȃp�X
			*out_pointmode = AI_NAVI_POINT_NULL;
		}
	}
}

//! @brief �R���X�g���N�^
AIObjectDriver::AIObjectDriver(class ObjectManager *in_ObjMgr, int in_ctrlid)
{
	ObjMgr = in_ObjMgr;
	ctrlid = in_ctrlid;

	moveturn_mode = 0;
	addrx = 0.0f;
	addry = 0.0f;
}

//! @brief �f�B�X�g���N�^
AIObjectDriver::~AIObjectDriver()
{}

//! @brief �ΏۃN���X��ݒ�
//! @attention ���̊֐��Őݒ���s��Ȃ��ƁA�N���X���̂��������@�\���܂���B
void AIObjectDriver::SetClass(class ObjectManager *in_ObjMgr, int in_ctrlid)
{
	ObjMgr = in_ObjMgr;
	ctrlid = in_ctrlid;
}

//! @brief �������n�֐�
void AIObjectDriver::Init()
{
	addrx = 0.0f;
	addry = 0.0f;

	ResetMode();
}

//! @brief �ړ���]����t���O���N���A�i��x�S�Ă̓������~�߂�j
void AIObjectDriver::ResetMode()
{
	moveturn_mode = 0;
}

//! @brief �ړ���]����t���O �ݒ�
//! @param flag ���샂�[�h��\���萔�iAIcontrolFlag�񋓌^�j
void AIObjectDriver::SetModeFlag(int flag)
{
	SetFlag(moveturn_mode, flag);
}

//! @brief �ړ���]����t���O ����
//! @param flag ���샂�[�h��\���萔�iAIcontrolFlag�񋓌^�j
void AIObjectDriver::DelModeFlag(int flag)
{
	DelFlag(moveturn_mode, flag);
}

//! @brief �ړ���]����t���O �擾
//! @param flag ���샂�[�h��\���萔�iAIcontrolFlag�񋓌^�j
bool AIObjectDriver::GetModeFlag(int flag)
{
	if( GetFlag(moveturn_mode, flag) == 0 ){
		return false;
	}
	//else{
		return true;
	//}
}

//! @brief �ړ���]��������s
//! @attention ���t���[��1�񂾂��Ăяo���Ă��������B
void AIObjectDriver::ControlObject()
{
	float rx, ry;

	//�ړ��̎��s
	if( GetFlag(moveturn_mode, AI_CTRL_MOVEFORWARD) ){
		ObjMgr->MoveForward(ctrlid);
	}
	if( GetFlag(moveturn_mode, AI_CTRL_MOVEBACKWARD) ){
		ObjMgr->MoveBack(ctrlid);
	}
	if( GetFlag(moveturn_mode, AI_CTRL_MOVELEFT) ){
		ObjMgr->MoveLeft(ctrlid);
	}
	if( GetFlag(moveturn_mode, AI_CTRL_MOVERIGHT) ){
		ObjMgr->MoveRight(ctrlid);
	}
	if( GetFlag(moveturn_mode, AI_CTRL_MOVEWALK) ){
		ObjMgr->MoveWalk(ctrlid);
	}

	//�����]���̎��s�i��]���x�̉��Z�j
	if( GetFlag(moveturn_mode, AI_CTRL_TURNUP) ){
		addry += AI_ADDTURNRAD;
	}
	if( GetFlag(moveturn_mode, AI_CTRL_TURNDOWN) ){
		addry -= AI_ADDTURNRAD;
	}
	if( GetFlag(moveturn_mode, AI_CTRL_TURNLEFT) ){
		addrx -= AI_ADDTURNRAD;
	}
	if( GetFlag(moveturn_mode, AI_CTRL_TURNRIGHT) ){
		addrx += AI_ADDTURNRAD;
	}

	//�p�x���擾
	ObjMgr->GetHumanObject(ctrlid)->GetRxRy(&rx, &ry);

	//�p�x�ɉ��Z
	rx += addrx;
	ry += addry;

	//�c�̉�]�͈͂����߂�
	if( ry > DegreeToRadian(70) ){ ry = DegreeToRadian(70); }
	if( ry < DegreeToRadian(-70) ){ ry = DegreeToRadian(-70); }

	//�p�x��ݒ�
	ObjMgr->GetHumanObject(ctrlid)->SetRxRy(rx, ry);

	//��]���x�̌���
	addrx *= 0.8f;
	addry *= 0.8f;

	//0.0f�֕␳
	if( fabsf(addrx) < DegreeToRadian(0.2f) ){ addrx = 0.0f; }
	if( fabsf(addry) < DegreeToRadian(0.2f) ){ addry = 0.0f; }
}