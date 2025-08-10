//! @file event.cpp
//! @brief EventControl�N���X�̒�`

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

#include "event.h"

//! @brief �R���X�g���N�^
EventControl::EventControl(PointDataInterface *in_Point, ObjectManager *in_ObjMgr)
{
	Point = in_Point;
	ObjMgr = in_ObjMgr;
	nextp4 = 0;
	waitcnt = 0;
}

//! @brief �f�B�X�g���N�^
EventControl::~EventControl()
{}

//! @brief �ΏۃN���X��ݒ�
//! @param in_Point �|�C���g�f�[�^�Ǘ��N���X
//! @param in_ObjMgr �I�u�W�F�N�g�Ǘ��N���X
//! @attention ���̊֐��Őݒ���s��Ȃ��ƁA�N���X���̂��������@�\���܂���B
void EventControl::SetClass(PointDataInterface *in_Point, ObjectManager *in_ObjMgr)
{
	Point = in_Point;
	ObjMgr = in_ObjMgr;
}

//! @brief ���Z�b�g
//! @param EntryP4 �C�x���g�������J�n���鎯�ʔԍ��@�i-100�A-110�A-120�@�Ȃǁj
void EventControl::Reset(signed short int EntryP4)
{
	nextp4 = EntryP4;
	waitcnt = 0;
}

//! @brief ���ɏ������鎯�ʔԍ�
//! @return P4�F���ʔԍ�
signed short int EventControl::GetNextP4()
{
	return nextp4;
}

//! @brief �Ώېl���̎��S����
//! @param targetP4 �^�[�Q�b�g�̎��ʔԍ�
//! @param reverse �������]�i�������艻�j
//! @return true�F���̃C�x���g�֐i�ށ@false�F���֐i�܂Ȃ�
//! @attention �Ώېl�������݂��Ȃ��ꍇ�Afalse�ɂȂ�܂��B
bool EventControl::CheckDead(signed short int targetP4, bool reverse)
{
	human* thuman;

	thuman = ObjMgr->SearchHuman(targetP4);
	if( thuman == NULL ){ return false; }

	if( reverse == false ){
		if( thuman->GetDeadFlag() == false ){ return false; }
	}
	else{
		if( thuman->GetDeadFlag() == true ){ return false; }
	}
	return true;
}

//! @brief �Ώېl���̓�������
//! @param pos_x ���肷��X���W
//! @param pos_y ���肷��Y���W
//! @param pos_z ���肷��Z���W
//! @param dist_r ���肷�鋗���i���a�j
//! @param targetP4 �^�[�Q�b�g�̎��ʔԍ�
//! @param reverse �������]�i�������艻�j
//! @return true�F���̃C�x���g�֐i�ށ@false�F���֐i�܂Ȃ�
//! @attention �Ώېl�������݂��Ȃ��ꍇ�Afalse�ɂȂ�܂��B
bool EventControl::CheckArrival(float pos_x, float pos_y, float pos_z, float dist_r, signed short int targetP4, bool reverse)
{
	human* thuman;
	float hx, hy, hz;
	float x, y, z;

	thuman = ObjMgr->SearchHuman(targetP4);
	if( thuman == NULL ){ return false; }
	thuman->GetPosData(&hx, &hy, &hz, NULL);
	x = hx - pos_x;
	y = hy - pos_y;
	z = hz - pos_z;

	if( reverse == false ){
		if( sqrtf(x*x + y*y+ + z*z) > dist_r ){ return false; }
	}
	else{
		if( sqrtf(x*x + y*y+ + z*z) < dist_r ){ return false; }
	}
	return true;
}

//! @brief �Ώېl���̎w�蕐�폊�L����
//! @param WeaponID ����̎�ޔԍ�
//! @param targetP4 �^�[�Q�b�g�̎��ʔԍ�
//! @param reverse �������]�i�����L���艻�j
//! @return true�F���̃C�x���g�֐i�ށ@false�F���֐i�܂Ȃ�
//! @attention �Ώېl�������݂��Ȃ��ꍇ�Afalse�ɂȂ�܂��B
bool EventControl::CheckHaveWeapon(int WeaponID, signed short int targetP4, bool reverse)
{
	human* thuman;
	int selectweapon;
	weapon *out_weapon[TOTAL_HAVEWEAPON];
	int now_weaponid;
	bool flag = false;			//false:�����L�Atrue:���L

	thuman = ObjMgr->SearchHuman(targetP4);
	if( thuman == NULL ){ return false; }

	//���L���镐��I�u�W�F�N�g��S�Ď擾
	thuman->GetWeapon(&selectweapon, out_weapon, NULL, NULL);

	//�S�Ē��ׂ�
	for(int i=0; i<TOTAL_HAVEWEAPON; i++){
		if( out_weapon[i] != NULL ){

			//����̎�ޔԍ����擾���A���ꂪ�Ώۂ̕��킩�ǂ����B
			out_weapon[i]->GetParamData(&now_weaponid, NULL, NULL);
			if( now_weaponid == WeaponID ){
				flag = true;
			}

		}
	}

	if( reverse == false ){
		if( flag == false ){ return false; }
	}
	else{
		if( flag == true ){ return false; }
	}
	return true;
}

//! @brief �Ώۈړ��p�X�̈ړ����[�h��������
//! @param targetP4 �^�[�Q�b�g�̎��ʔԍ�
//! @param newmode �ړ����[�h
//! @return �����Ftrue�@���s�Ffalse
bool EventControl::SetMovePathMode(signed short int targetP4, int newmode)
{
	pointdata pathdata;
	int pid;

	if( Point->SearchPointdata(&pid, 0x08, 0, 0, 0, targetP4, 0) > 0 ){

		//�Ώۂ�AI�p�X�Ȃ�΁A�����I�Ƀp�����[�^������������
		Point->Getdata(&pathdata, pid);
		if( (pathdata.p1 == POINT_P1TYPE_AIPATH)||(pathdata.p1 == POINT_P1TYPE_RAND_AIPATH) ){
			pathdata.p2 = (signed short int)newmode;
			Point->SetParam(pid, pathdata.p1, pathdata.p2, pathdata.p3, pathdata.p4);
			return true;
		}

	}
	return false;
}

//! @brief �Ώېl���̃`�[���ԍ��ύX
//! @param targetP4 �^�[�Q�b�g�̎��ʔԍ�
//! @param TeamID �`�[���ԍ�
//! @return �����Ftrue�@���s�Ffalse
bool EventControl::SetTeamID(signed short int targetP4, int TeamID)
{
	human* thuman;

	thuman = ObjMgr->SearchHuman(targetP4);
	if( thuman == NULL ){ return false; }

	thuman->SetTeamID(TeamID);
	return true;
}

//! @brief �Ώۏ����̔j�󔻒�
//! @param targetP4 �^�[�Q�b�g�̎��ʔԍ�
//! @param reverse �������]�i���j�󔻒艻�j
//! @return true�F���̃C�x���g�֐i�ށ@false�F���֐i�܂Ȃ�
//! @attention �Ώۏ��������݂��Ȃ��ꍇ�A�j��ς݂Ƃ��ď�������܂��B
bool EventControl::CheckBreakSmallObject(signed short int targetP4, bool reverse)
{
	smallobject* tsmallobject;
	bool flag = true;			//false:���j��Atrue:�j��ς�or���݂��Ȃ�

	tsmallobject = ObjMgr->SearchSmallobject(targetP4);
	if( tsmallobject != NULL ){
		if( tsmallobject->GetEnableFlag() == true ){ flag = false; }
	}

	if( reverse == false ){
		if( flag == false ){ return false; }
	}
	else{
		if( flag == true ){ return false; }
	}
	return true;
}

//! @brief ���ԑ҂��i�b�P�ʁj
//! @param sec �b
//! @return true�F���̃C�x���g�֐i�ށ@false�F���֐i�܂Ȃ�
bool EventControl::WaitSec(int sec)
{
	if( ((int)GAMEFPS * sec) > waitcnt ){
		waitcnt += 1;
		return false;
	}
	waitcnt = 0;
	return true;
}

//! @brief ���b�Z�[�W�\��
//! @param SetID ���b�Z�[�W�ԍ�
//! @param MessageID MessageID�̃|�C���^
//! @param SetMessageID SetMessageID�̃|�C���^
//! @attention �͈͊O�̃��b�Z�[�W�ԍ����w�肳��ďꍇ���A�V���ȃ��b�Z�[�W���w�肳�ꂽ���̂Ƃ��ď������܂��B
void EventControl::SetMessage(int SetID, int *MessageID, bool *SetMessageID)
{
	if( (0 <= SetID)&&(SetID < MAX_POINTMESSAGES) ){
		*MessageID = SetID;
	}
	*SetMessageID = true;
}

//! @brief ���������s
//! @param SkipFlag �C�x���g�^�X�N�̃X�L�b�v�t���O
//! @param Param �C�x���g�̃p�����[�^�[���󂯓n������\���̂̃|�C���^
//! @return ���s�����X�e�b�v��
//! @attention �n���ꂽ�p�����[�^�[�i�\���́j�ɕύX���Ȃ��ꍇ�A�n���ꂽ�p�����[�^�[�͑��삳��܂���B
//! @attention �\���̂�SetMessageID �́A���b�Z�[�W�C�x���g�����s���ꂽ�ꍇ�� true �ɂȂ�܂��B<b>�O�񂩂�ύX���ꂽ�Ƃ͌���܂���B</b>
int EventControl::ProcessEventPoint(bool SkipFlag, EventParameter *Param)
{
	if( Param == NULL ){ return 0; }

	pointdata data;
	int cnt = 0;

	for(int i=0; i<TOTAL_EVENTFRAMESTEP; i++){
		cnt = i;

		//���̃|�C���g��T��
		if( Point->SearchPointdata(&data, 0x08, 0, 0, 0, nextp4, 0) == 0 ){ return cnt; }

		//�����Ȕԍ��Ȃ珈�����Ȃ�
		if( (data.p1 < 10)||(19 < data.p1) ){
#ifdef ENABLE_CHECKOPENXOPSEVENT
			if( data.p1 != 29 ){ return cnt; }
#else
			return cnt;
#endif
		}

		//���[�v1��ڂŃX�L�b�v�t���O���L���Ȃ�΁A���C�x���g�֐i�ށB
		if( (cnt == 0)&&(SkipFlag == true) ){
			nextp4 = data.p3;
			continue;
		}

		switch(data.p1){
			case 10:	//�C���B��
				Param->endcnt += 1;
				Param->complete = true;
				return cnt;

			case 11:	//�C�����s
				Param->endcnt += 1;
				Param->complete = false;
				return cnt;

			case 12:	//���S�҂�
				if( CheckDead(data.p2, false) == false ){ return cnt; }
				nextp4 = data.p3;
				break;

			case 13:	//�����҂�
				if( CheckArrival(data.x, data.y, data.z, DISTANCE_CHECKPOINT, data.p2, false) == false ){ return cnt; }
				nextp4 = data.p3;
				break;

			case 14:	//�����ɕύX
				SetMovePathMode(data.p2, 0);
				nextp4 = data.p3;
				break;

			case 15:	//�����j��҂�
				if( CheckBreakSmallObject(data.p2, false) == false ){ return cnt; }
				nextp4 = data.p3;
				break;

			case 16:	//�P�[�X�҂�
				if( CheckArrival(data.x, data.y, data.z, DISTANCE_CHECKPOINT, data.p2, false) == false ){ return cnt; }
				if( CheckHaveWeapon(ID_WEAPON_CASE, data.p2, false) == false ){ return cnt; }
				nextp4 = data.p3;
				break;

			case 17:	//���ԑ҂�
				if( WaitSec(PointParamToInt(data.p2)) == false ){ return cnt; }
				nextp4 = data.p3;
				break;

			case 18:	//���b�Z�[�W
				SetMessage(PointParamToInt(data.p2), &(Param->MessageID), &(Param->SetMessageID));
				nextp4 = data.p3;
				break;

			case 19:	//�`�[���ύX
				SetTeamID(data.p2, 0);
				nextp4 = data.p3;
				break;

#ifdef ENABLE_CHECKOPENXOPSEVENT
			case 29:	//OpenXOPS����
				if( data.p2 != 0 ){ return cnt; }
				nextp4 = data.p3;
				break;
#endif

			//�V���ȃC�x���g�|�C���g��ǉ�����ꍇ�A�����ɏ����B
			//�@�� ��ޔԍ��̋��� ����
			//�@�� �{switch�����O�ɂ���A��������i�͈̓`�F�b�N�j�����������邱�ƁB
		}
	}
	return cnt;
}