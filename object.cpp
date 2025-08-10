//! @file object.cpp
//! @brief object�N���X�̒�`

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

#include "object.h"

//! @brief �R���X�g���N�^
object::object(class ParameterInfo *in_Param, float x, float y, float z, float rx, float size, bool flag)
{
	Param = in_Param;
	pos_x = x;
	pos_y = y;
	pos_z = z;
	rotation_x = rx;
	EnableFlag = flag;
	model_size = size;

	id_parameter = 0;
	id_model = -1;
	id_texture = -1;
	DarkModelFlag = false;
}

//! @brief �f�B�X�g���N�^
object::~object()
{}

//! @brief �ݒ�l���Ǘ�����N���X��o�^
//! @attention �e�֐����g�p����O�Ɏ��s���邱�ƁB
void object::SetParameterInfoClass(class ParameterInfo *in_Param)
{
	Param = in_Param;
}

//! @brief ���W�Ɗp�x��ݒ�
//! @param x X���W
//! @param y Y���W
//! @param z Z���W
//! @param rx ������]
void object::SetPosData(float x, float y, float z, float rx)
{
	pos_x = x;
	pos_y = y;
	pos_z = z;
	rotation_x = rx;
}

//! @brief ���W�Ɗp�x���擾
//! @param x X���W���󂯎��|�C���^�iNULL�j
//! @param y Y���W���󂯎��|�C���^�iNULL�j
//! @param z Z���W���󂯎��|�C���^�iNULL�j
//! @param rx ������]���󂯎��|�C���^�iNULL�j
void object::GetPosData(float *x, float *y, float *z, float *rx)
{
	if( x != NULL ){ *x = pos_x; }
	if( y != NULL ){ *y = pos_y; }
	if( z != NULL ){ *z = pos_z; }
	if( rx != NULL ){ *rx = rotation_x; }
}

//! @brief �L�����t���O��ݒ�
//! @param flag �ݒ肷��t���O
void object::SetEnableFlag(bool flag)
{
	EnableFlag = flag;
}

//! @brief �L�����t���O���擾
//! @return ���ݐݒ肳��Ă���t���O
bool object::GetEnableFlag()
{
	return EnableFlag;
}

//! @brief ���f���f�[�^��ݒ�
//! @param id ���f���̃f�[�^�ԍ�
//! @param size �`��{��
void object::SetModel(int id, float size)
{
	id_model = id;
	model_size = size;
}

//! @brief ���f���f�[�^���擾
//! @param id ���f���̃f�[�^�ԍ����󂯎��|�C���^�iNULL�j
//! @param size �`��{�����󂯎��|�C���^�iNULL�j
void object::GetModel(int *id, float *size)
{
	if( id != NULL ){ *id = id_model; }
	if( size != NULL ){ *size = model_size; }
}

//! @brief �e�N�X�`����ݒ�
//! @param id �e�N�X�`���̃f�[�^�ԍ�
void object::SetTexture(int id)
{
	id_texture = id;
}

//! @brief �e�N�X�`�����擾
//! @return �e�N�X�`���̃f�[�^�ԍ�
int object::GetTexture()
{
	return id_texture;
}

//! @brief ���f�����Â�����t���O��ݒ�
//! @param flag �t���O
void object::SetDarkModelFlag(bool flag)
{
	DarkModelFlag = flag;
}

//! @brief �v�Z�����s�i���R�����Ȃǁj
int object::ProcessObject()
{
	return 0;
}

//! @brief �`��
//! @param d3dg D3DGraphics�̃|�C���^
//! @param NoModel ���f���`�斳����
void object::Render(D3DGraphics *d3dg, bool NoModel)
{
	if( d3dg == NULL ){ return; }
	if( EnableFlag == false ){ return; }

	d3dg->SetWorldTransform(pos_x, pos_y, pos_z, rotation_x, 0.0f, model_size);
	d3dg->RenderModel(id_model, id_texture, DarkModelFlag, false, NoModel);
}

//! @brief �R���X�g���N�^
human::human(class ParameterInfo *in_Param, float x, float y, float z, float rx, int id_param, int dataid, signed short int p4, int team, bool flag)
{
	//HumanParameter data;

	Param = in_Param;
	pos_x = x;
	pos_y = y;
	pos_z = z;
	move_x = 0.0f;
	move_y = 0.0f;
	move_z = 0.0f;
	move_y_flag = false;
	move_y_upper = 0;
	move_y_landing = false;
	underblock_id = -1;
	underblock_face = -1;
	rotation_x = rx;
	id_parameter = id_param;
	upmodel_size = 9.4f;
	armmodel_size = 9.0f;
	legmodel_size = 9.0f;
	EnableFlag = flag;
	rotation_y = 0.0f;
	armrotation_y = 0.0f;
	point_dataid = dataid;
	point_p4 = p4;
	teamid = team;

	for(int i=0; i<TOTAL_HAVEWEAPON; i++){
		weapon[i] = NULL;
		noweapon_bullets[i] = 0;
		noweapon_loadbullets[i] = 0;
	}
	selectweapon = 1;
	selectweaponcnt = 0;
	changeweaponidcnt = 0;
	weaponshotcnt = 0;
	weaponreloadcnt = 0;
	weaponshotframe = false;
	weaponburstmodecnt = 0;
	//if( Param->GetHuman(id_param, &data) == 0 ){
	//	hp = data.hp;
	//}
	//else{
		hp = 0;
	//}
	deadstate = 0;
	add_ry = 0.0f;
	id_texture = -1;

	move_rx = 0.0f;
	MoveFlag = 0x00;
	MoveFlag_lt = MoveFlag;
	scopemode = 0;
	HitFlag = false;
	Hit_rx = 0.0f;
	totalmove = 0.0f;
	StateGunsightErrorRange = 0;
	ReactionGunsightErrorRange = 0;
	Invincible = false;

	MotionCtrl = new HumanMotionControl;
}

//! @brief �f�B�X�g���N�^
human::~human()
{
	delete MotionCtrl;
}

//! @brief �ݒ�l���Ǘ�����N���X��o�^
//! @attention �e�֐����g�p����O�Ɏ��s���邱�ƁB
void human::SetParameterInfoClass(class ParameterInfo *in_Param)
{
	Param = in_Param;

	MotionCtrl->SetParameterInfoClass(in_Param);
}

//! @brief �ݒ�l��ݒ�
//! @param id_param �l�̎�ޔԍ�
//! @param dataid �|�C���g�̃f�[�^�ԍ�
//! @param p4 ��4�p�����[�^
//! @param team �`�[���ԍ�
//! @param init �I�u�W�F�N�g��������
//! @warning init������true�ɂ��ď���������ꍇ�A���SetPosData()�֐��Ŋp�x��ݒ肵�Ă������ƁB
void human::SetParamData(int id_param, int dataid, signed short int p4, int team, bool init)
{
	id_parameter = id_param;
	point_dataid = dataid;
	point_p4 = p4;
	teamid = team;

	if( init == true ){
		HumanParameter data;

		move_x = 0.0f;
		move_y = 0.0f;
		move_z = 0.0f;
		move_y_flag = false;
		move_y_upper = 0;
		move_y_landing = false;
		underblock_id = -1;
		underblock_face = -1;
		rotation_y = 0.0f;
		armrotation_y = DegreeToRadian(-30);

		for(int i=0; i<TOTAL_HAVEWEAPON; i++){
			weapon[i] = NULL;
			noweapon_bullets[i] = 0;
			noweapon_loadbullets[i] = 0;
		}
		selectweapon = 1;
		selectweaponcnt = 0;
		changeweaponidcnt = 0;
		weaponshotcnt = 0;
		weaponreloadcnt = 0;
		weaponshotframe = false;
		weaponburstmodecnt = 0;
		if( Param->GetHuman(id_param, &data) == 0 ){
			hp = data.hp;
		}
		else{
			hp = 0;
		}
		deadstate = 0;
		add_ry = 0.0f;
		MoveFlag = 0x00;
		MoveFlag_lt = MoveFlag;
		scopemode = 0;
		HitFlag = false;
		Hit_rx = 0.0f;
		totalmove = 0.0f;
		Invincible = false;

		MotionCtrl->Init(rotation_x);
	}
}

//! @brief �ݒ�l���擾
//! @param id_param �l�̎�ޔԍ����󂯎��|�C���^�iNULL�j
//! @param dataid �|�C���g�̃f�[�^�ԍ����󂯎��|�C���^�iNULL�j
//! @param p4 ��4�p�����[�^���󂯎��|�C���^�iNULL�j
//! @param team �`�[���ԍ����󂯎��|�C���^�iNULL�j
void human::GetParamData(int *id_param, int *dataid, signed short int *p4, int *team)
{
	if( id_param != NULL ){ *id_param = id_parameter; }
	if( dataid != NULL ){ *dataid = point_dataid; }
	if( p4 != NULL ){ *p4 = point_p4; }
	if( team != NULL ){ *team = teamid; }
}

//! @brief HP���擾
//! @return HP
int human::GetHP()
{
	return hp;
}

//! @brief HP��ݒ�
//! @param in_hp �V���ɐݒ肷��HP
//! @return �����Ftrue�@���s�Ffalse
//! @attention HP��1�ȏ�̐l�ɑ΂��Ď��s���Ȃ��Ǝ��s���܂��B
bool human::SetHP(int in_hp)
{
	if( hp > 0 ){
		hp = in_hp;
		return true;
	}
	return false;
}


//! @brief ���̂��ǂ�������
//! @return ���́Ftrue�@���̂łȂ��Ffalse
//! @warning ���S�ɐÎ~������Ԃ��u���́v�Ɣ��肵�܂��B�|��Ă���Œ��̐l�͑ΏۂɊ܂܂�Ȃ����߁Ahp <= 0 ���S�Ď��̂Ɣ��肳���Ƃ͌���܂���B
bool human::GetDeadFlag()
{
	if( deadstate == 5 ){ return true; }
	return false;
}

//! @brief ���S���[�V���������ǂ�������
//! @return ���[�V�������ł���Ftrue�@���[�V�������łȂ��Ffalse
//! @attention �̂��|��Ă���Œ���A���������Ă����Ԃ𔻒肵�܂��B�������͔��肵�܂���B
bool human::GetDeadMotionFlag()
{
	if( deadstate == 1 ){ return true; }
	if( deadstate == 3 ){ return true; }
	return false;
}

//! @brief �`�[���ԍ���ݒ�i�㏑���j
//! @param id �V�����`�[���ԍ�
void human::SetTeamID(int id)
{
	teamid = id;
}

//! @brief ���G�t���O���擾
//! @return ���G�Ftrue�@�ʏ�Ffalse
//! @attention ���G��Ԃ̏ꍇ�A�e�e�E��֒e�̔����E�����@�ɂ��_���[�W����؎󂯂܂���B
bool human::GetInvincibleFlag()
{
	return Invincible;
}

//! @brief ���G�t���O��ݒ�
//! @param flag ���G�Ftrue�@�ʏ�Ffalse
//! @attention ���G��Ԃ̏ꍇ�A�e�e�E��֒e�̔����E�����@�ɂ��_���[�W����؎󂯂܂���B
void human::SetInvincibleFlag(bool flag)
{
	Invincible = flag;
}

//! @brief �O�����̈ړ��ʂ��擾
//! @param *x X���ړ��ʂ��擾����|�C���^�iNULL�j
//! @param *y Y���ړ��ʂ��擾����|�C���^�iNULL�j
//! @param *z Z���ړ��ʂ��擾����|�C���^�iNULL�j
void human::GetMovePos(float *x, float *y, float *z)
{
	if( x != NULL ){ *x = move_x; }
	if( y != NULL ){ *y = move_y; }
	if( z != NULL ){ *z = move_z; }
}

//! @brief ���f���f�[�^��ݒ�
//! @param upmodel �㔼�g�̃��f��
//! @param armmodel[] �r�̃��f���̔z��i�z�񐔁FTOTAL_ARMMODE�j
//! @param legmodel ���i�Î~��ԁj�̃��f��
//! @param walkmodel[] �r�̃��f���̔z��i�z�񐔁FTOTAL_WALKMODE�j
//! @param runmodel[] �r�̃��f���̔z��i�z�񐔁FTOTAL_RUNMODE�j
void human::SetModel(int upmodel, int armmodel[], int legmodel, int walkmodel[], int runmodel[])
{
	MotionCtrl->SetModel(upmodel, armmodel, legmodel, walkmodel, runmodel);
}

//! @brief �����ݒ�
//! @param in_weapon[] �ݒ肷��weapon�N���X�̃|�C���^�z��
//! @param nlnbs[] ���햢�������̑��e����ݒ肷��int�z��
//! @param nnbs[] ���햢�������̍��v�e����ݒ肷��int�z��
//! @warning �ʏ�� PickupWeapon()�֐� ���g�p���邱��
void human::SetWeapon(class weapon *in_weapon[], int nlnbs[], int nnbs[])
{
	for(int i=0; i<TOTAL_HAVEWEAPON; i++){
		if( in_weapon[i] == NULL ){
			weapon[i] = NULL;
		}
		else{
			//����𐳂����E����΁A��������Ƃ��ēo�^
			if( in_weapon[i]->Pickup() == 0 ){
				weapon[i] = in_weapon[i];
			}
		}
		noweapon_loadbullets[i] = nlnbs[i];
		noweapon_bullets[i] = nnbs[i];
	}
}

//! @brief ������E��
//! @param in_weapon[] �ݒ肷��weapon�N���X�̃|�C���^
//! @return �����F1�@���s�F0
int human::PickupWeapon(class weapon *in_weapon)
{
	if( in_weapon == NULL ){ return 0; }

	if( weapon[selectweapon] == NULL ){
		//����𐳂����E����΁A��������Ƃ��ēo�^
		if( in_weapon->Pickup() == 0 ){
			weapon[selectweapon] = in_weapon;

			//�r�̊p�x�i�����j��ݒ�
			class weapon *nowweapon;
			int id_param = 0;
			nowweapon = weapon[selectweapon];
			nowweapon->GetParamData(&id_param, NULL, NULL);
			MotionCtrl->PickupWeapon(id_param);

			//���������̒e�����͏�����
			noweapon_bullets[selectweapon] = 0;
			noweapon_loadbullets[selectweapon] = 0;

			//�؂�ւ������̃J�E���g
			selectweaponcnt = 10;

			return 1;
		}
	}
	return 0;
}

//! @brief �����؂�ւ���i�����ւ���j
//! @param id �����ւ��镐��@�i-1 �Ŏ��̕���j
//! @attention �Q�[���ォ�璼�ڌĂяo�����Ƃ͔����AObjectManager�N���X����Ăяo���Ă��������B
void human::ChangeHaveWeapon(int id)
{
	//�̗͂��Ȃ���Ύ��s
	if( hp <= 0 ){ return; }

	//�����[�h���Ȃ玸�s
	if( weaponreloadcnt > 0 ){ return; }

	//��������ɐ؂�ւ��悤�Ƃ��Ă���Ȃ�A���s
	if( selectweapon == id ){ return; }

	//����؂�ւ����Ȃ玸�s
	if( selectweaponcnt > 0 ){ return; }
	if( changeweaponidcnt > 0 ){ return; }

	if( id == -1 ){
		//���̕���ԍ���I��
		selectweapon += 1;
		if( selectweapon == TOTAL_HAVEWEAPON ){
			selectweapon = 0;
		}
	}
	else{
		//����ԍ����͈͓����m�F
		if( (id < 0)||(TOTAL_HAVEWEAPON <= id ) ){ return; }
		selectweapon = id;
	}

	//�X�R�[�v�\��������
	SetDisableScope();

	//���ݑ������镐��̃N���X���擾
	class weapon *nowweapon;
	nowweapon = weapon[selectweapon];

	//�r�̊p�x�i�����j��ݒ�
	if( nowweapon == NULL ){	//��Ԃ�
		MotionCtrl->ChangeHaveWeapon(ID_WEAPON_NONE);
	}
	else{
		int id_param = 0;
		nowweapon->GetParamData(&id_param, NULL, NULL);
		MotionCtrl->ChangeHaveWeapon(id_param);
	}

	//�؂�ւ������̃J�E���g
	selectweaponcnt = 10;

	//�o�[�X�g���[�h�p�J�E���g
	weaponburstmodecnt = 0;
}

//! @brief ����̐؂�ւ��J�E���g���擾
//! @return �J�E���g���@�i1�ȏ�Ő؂�ւ����j
int human::GetChangeHaveWeaponCnt()
{
	return selectweaponcnt;
}

//! @brief ����̎�ސ؂�ւ�
//! @return �����F0�@���s�F1
//! @attention �Q�[���ォ�璼�ڌĂяo�����Ƃ͔����AObjectManager�N���X����Ăяo���Ă��������B
int human::ChangeWeaponID()
{
	//����؂�ւ����Ȃ玸�s
	//if( selectweaponcnt > 0 ){ return 1; }
	if( changeweaponidcnt > 0 ){ return 1; }

	//����𑕔����ĂȂ���Ύ��s
	if( weapon[selectweapon] == NULL ){ return 1; }

	//�������Ă��镐��̏����擾
	int param_id, lnbs, nbs;
	weapon[selectweapon]->GetParamData(&param_id, &lnbs, &nbs);

	//����̎�ސ؂�ւ���i�V��������ԍ��j�𒲂ׂ�
	WeaponParameter ParamData;
	int ChangeWeaponID, ChangeCnt;
	if( Param->GetWeapon(param_id, &ParamData) != 0 ){ return 1; }
	ChangeWeaponID = ParamData.ChangeWeaponID;
	ChangeCnt = ParamData.ChangeWeaponCnt;

	//�V��������ԍ������������m�F
	if( ChangeWeaponID == param_id ){ return 1; }
	if( (ChangeWeaponID < 0)||(TOTAL_PARAMETERINFO_WEAPON <= ChangeWeaponID) ){ return 1; }

	//�J�E���g���L���ɂ�������炸�����[�h���Ȃ�؂�ւ��Ȃ�
	if( (ChangeCnt > 0)&&(weaponreloadcnt > 0) ){ return 1; }

	//�ݒ��K�p
	weapon[selectweapon]->SetParamData(ChangeWeaponID, lnbs, nbs, false);

	//�����X�R�[�v�g�p���Ȃ��
	if( scopemode != 0 ){
		if( ChangeCnt == 0 ){
			//�V��������̏����擾
			if( Param->GetWeapon(ChangeWeaponID, &ParamData) != 0 ){ return 1; }

			if( scopemode == ParamData.scopemode ){
				//�����X�R�[�v���g����Ȃ�΁A���������X�R�[�v�ێ�
			}
			else if( ParamData.scopemode == 0 ){
				//�X�R�[�v���g���Ȃ��Ȃ�΁A�X�R�[�v����
				SetDisableScope();
			}
			else{
				//�قȂ�X�R�[�v���g����Ȃ�΁A�X�R�[�v�Đݒ�
				SetEnableScope();
			}
		}
		else{
			//�X�R�[�v����
			SetDisableScope();
		}
	}

	//���[�V�������s
	MotionCtrl->ChangeWeaponID(ChangeWeaponID);

	//�؂�ւ������̃J�E���g
	changeweaponidcnt = ChangeCnt;

	//�o�[�X�g���[�h�p�J�E���g
	weaponburstmodecnt = 0;

	return 0;
}

//! @brief ����̎�ސ؂�ւ��J�E���g���擾
//! @return �J�E���g���@�i1�ȏ�Ő؂�ւ����j
int human::GetChangeWeaponIDCnt()
{
	return changeweaponidcnt;
}

//! @brief ����̃����[�h�J�E���g���擾
//! @return �J�E���g���@�i�����[�h���F1�ȏ�j
int human::GetWeaponReloadCnt()
{
	return weaponreloadcnt;
}

//! @brief ������擾
//! @param out_selectweapon �I������Ă��镐��@�i0 �` [TOTAL_HAVEWEAPON]-1�j
//! @param out_weapon �󂯎��weapon�N���X�̃|�C���^�z��@�i�z�񐔁FTOTAL_HAVEWEAPON�j
//! @param nlnbs[] ���햢�������̑��e�����󂯎��int�z��@�i�z�񐔁FTOTAL_HAVEWEAPON�ANULL�j
//! @param nnbs[] ���햢�������̍��v�e�����󂯎��int�z��@�i�z�񐔁FTOTAL_HAVEWEAPON�ANULL�j
void human::GetWeapon(int *out_selectweapon, class weapon *out_weapon[], int nlnbs[], int nnbs[])
{
	*out_selectweapon = selectweapon;
	for(int i=0; i<TOTAL_HAVEWEAPON; i++){
		out_weapon[i] = weapon[i];
		if( nlnbs != NULL ){ nlnbs[i] = noweapon_loadbullets[i]; }
		if( nnbs != NULL ){ nnbs[i] = noweapon_bullets[i]; }
	}
}

//! @brief ���ݑ������Ă��镐��̎�ޔԍ����擾
//! @return ����̎�ޔԍ��i0 �` TOTAL_PARAMETERINFO_WEAPON -1�j
//! @attention ���ݎ�Ɏ����Ă��镐��̎�ޔԍ��ł��B�@GetWeapon()�֐� ��p���Ē��ׂ�̂Ɠ����ł��B
int human::GetMainWeaponTypeNO()
{
	if( weapon[selectweapon] == NULL ){
		return ID_WEAPON_NONE;
	}

	int id_param;
	weapon[selectweapon]->GetParamData(&id_param, NULL, NULL);
	return id_param;
}

//! @brief �A�ːݒ���擾
//! @return �A�ˉ\�Ftrue�@�A�˕s�Ffalse
bool human::GetWeaponBlazingmode()
{
	int param_id;
	WeaponParameter data;

	//����𑕔����Ă��Ȃ���΁A���s
	if( weapon[selectweapon] == NULL ){ return false; }

	//�������Ă��镐��̎�ޔԍ����擾
	weapon[selectweapon]->GetParamData(&param_id, NULL, NULL);

	//�A�ːݒ��Ԃ�
	Param->GetWeapon(param_id, &data);
	if( data.burstcnt == 1 ){ return false; }
	return true;
}

//! @brief ���C����
//! @param weapon_paramid ���C��������̔ԍ����󂯎��|�C���^
//! @param GunsightErrorRange ���C�����ۂ̏Ə��덷���󂯎��|�C���^
//! @return �����Ftrue�@���s�Ffalse
//! @attention �e�I�u�W�F�N�g�̏�����A���C���̍Đ��͕ʓr�s���K�v������܂��B
//! @attention �Q�[���ォ�璼�ڌĂяo�����Ƃ͔����AObjectManager�N���X����Ăяo���Ă��������B
bool human::ShotWeapon(int *weapon_paramid, int *GunsightErrorRange)
{
	if( weapon_paramid == NULL ){ return false; }
	if( GunsightErrorRange == NULL ){ return false; }

	int param_id;

	//����؂�ւ����Ȃ玸�s
	if( selectweaponcnt > 0 ){ return false; }
	if( changeweaponidcnt > 0 ){ return false; }

	//����𑕔����Ă��Ȃ���΁A���s
	if( weapon[selectweapon] == NULL ){ return false; }

	//���C�v���͎󂯓���A�t���O�L����
	weaponshotframe = true;

	//���ˊԊu�ɖ����Ȃ����A�����[�h���Ȃ�Ύ��s
	if( weaponshotcnt > 0 ){ return false; }
	if( weaponreloadcnt > 0 ){ return false; }

	//����̎�ޔԍ����擾
	weapon[selectweapon]->GetParamData(&param_id, NULL, NULL);

	//����̐ݒ�l���擾
	WeaponParameter ParamData;
	ScopeParameter sparam;
	if( Param->GetWeapon(param_id, &ParamData) != 0 ){ return false; }
	Param->GetScopeParam(ParamData.scopemode, &sparam);

	//�o�[�X�g���[�h���L���ŁA�A�˒e���������Ă���Ύ��s
	if( (ParamData.burstcnt > 0)&&(weaponburstmodecnt >= ParamData.burstcnt) ){ return false; }

	//�e�̔��ˏ������s��
	if( weapon[selectweapon]->Shot() != 0 ){ return false; }

	//���˒e���Ƃ��ăJ�E���g
	weaponburstmodecnt += 1;


	//�A�˃J�E���g��ݒ�
	weaponshotcnt = ParamData.blazings;

	//�X�R�[�v���g�p���̌덷��ݒ�
	if( (scopemode == 0)&&(sparam.WeaponNoScopeReaction > 0) ){
		ReactionGunsightErrorRange = sparam.WeaponNoScopeReaction;
	}


	//����̎�ޔԍ��ƌ덷��Ԃ�
	*weapon_paramid = param_id;
	*GunsightErrorRange = StateGunsightErrorRange + ReactionGunsightErrorRange;


	//�덷�v�Z
	if( ( (scopemode == 0)&&(sparam.NoScopeGunsight == true) )||( (scopemode != 0)&&(sparam.ScopeGunsight == 0) ) ){
		//�Ə����\�������Ȃ�΁A�ݒ肳�ꂽ�덷�����Z�B
		ReactionGunsightErrorRange += ParamData.reaction;
	}

	//�X�R�[�v���g�p���Ă����Ԃ̔��������Z
	if( (sparam.WeaponRecoil_ScopeRx != 0.0f)||(sparam.WeaponRecoil_ScopeRyMin != 0.0f)||(sparam.WeaponRecoil_ScopeRyMax != 0.0f) ){
		if( (scopemode != 0)||(sparam.WeaponRecoil_ScopeOnly == false) ){
			int rx_rnds, ry_rnds;
			float rx_degree, ry_degree;

			//�����_���̐��l�͈͌v�Z
			rx_rnds = (int)(sparam.WeaponRecoil_ScopeRx*10) * 2;
			ry_rnds = (int)(sparam.WeaponRecoil_ScopeRyMax*10) - (int)(sparam.WeaponRecoil_ScopeRyMin*10);

			//�����i�p�x�j����
			rx_degree = sparam.WeaponRecoil_ScopeRx*-1 + ((float)GetRand(rx_rnds))/10;
			ry_degree = sparam.WeaponRecoil_ScopeRyMin + ((float)GetRand(ry_rnds))/10;

			//�������Z
			rotation_x += DegreeToRadian(rx_degree);
			armrotation_y += DegreeToRadian(ry_degree);
		}
	}

	//�r�ɔ�����`����
	MotionCtrl->ShotWeapon(param_id);

	//���킪�����Ȃ��Ă���΁A��������O���������ɁB�@�i��֒e�p�j
	if( weapon[selectweapon]->GetEnableFlag() == false ){
		weapon[selectweapon] = NULL;
	}
	return true;
}

//! @brief �����[�h
//! @return �����Ftrue�@���s�Ffalse
//! @attention �Q�[���ォ�璼�ڌĂяo�����Ƃ͔����AObjectManager�N���X����Ăяo���Ă��������B
bool human::ReloadWeapon()
{
	//����؂�ւ����Ȃ玸�s
	if( selectweaponcnt > 0 ){ return false; }
	if( changeweaponidcnt > 0 ){ return false; }

	//��������̕���𑕔����Ă���΁`
	if( weapon[selectweapon] != NULL ){
		//�����[�h���Ȃ玸�s
		if( weaponreloadcnt > 0 ){ return false; }

		//�����[�h�������J�n
		if( weapon[selectweapon]->StartReload() != 0 ){ return false; }

		//�X�R�[�v���[�h������
		SetDisableScope();

		//���[�V�������s
		class weapon *nowweapon;
		int id_param = 0;
		nowweapon = weapon[selectweapon];
		nowweapon->GetParamData(&id_param, NULL, NULL);
		MotionCtrl->ReloadWeapon(id_param);

		//����̐��\�l���擾
		int param_id;
		WeaponParameter ParamData;
		weapon[selectweapon]->GetParamData(&param_id, NULL, NULL);
		if( Param->GetWeapon(param_id, &ParamData) != 0 ){ return 1; }

		//�����[�h�J�E���g��ݒ�
		weaponreloadcnt = ParamData.reloads + 1;

		//�o�[�X�g���[�h�p�J�E���g��ݒ�
		weaponburstmodecnt = 0;

		return true;
	}
	return false;
}

//! @brief ������̂Ă�
//! @return �����Ftrue�@���s�Ffalse
//! @attention �Q�[���ォ�璼�ڌĂяo�����Ƃ͔����AObjectManager�N���X����Ăяo���Ă��������B
bool human::DumpWeapon()
{
	//����؂�ւ����Ȃ玸�s
	if( selectweaponcnt > 0 ){ return false; }
	if( changeweaponidcnt > 0 ){ return false; }

	//��������̕���𑕔����Ă���΁`
	if( weapon[selectweapon] != NULL ){
		//�����[�h���Ȃ玸�s
		if( weaponreloadcnt > 0 ){ return false; }

		//������̂āA����������
		weapon[selectweapon]->Dropoff(pos_x, pos_y, pos_z, rotation_x, 1.63f);
		weapon[selectweapon] = NULL;

		//���������̒e�����͏�����
		noweapon_bullets[selectweapon] = 0;
		noweapon_loadbullets[selectweapon] = 0;

		//�X�R�[�v���[�h������
		SetDisableScope();

		//���[�V�������s
		MotionCtrl->DumpWeapon();

		return true;
	}

	return false;
}

//! @brief �O�i�i����j��ݒ�
//! @attention �Q�[���ォ�璼�ڌĂяo�����Ƃ͔����AObjectManager�N���X����Ăяo���Ă��������B
void human::SetMoveForward()
{
	SetFlag(MoveFlag, MOVEFLAG_FORWARD);
}

//! @brief ��ނ�ݒ�
//! @attention �Q�[���ォ�璼�ڌĂяo�����Ƃ͔����AObjectManager�N���X����Ăяo���Ă��������B
void human::SetMoveBack()
{
	SetFlag(MoveFlag, MOVEFLAG_BACK);
}

//! @brief �������ݒ�
//! @attention �Q�[���ォ�璼�ڌĂяo�����Ƃ͔����AObjectManager�N���X����Ăяo���Ă��������B
void human::SetMoveLeft()
{
	SetFlag(MoveFlag, MOVEFLAG_LEFT);
}

//! @brief �E�����ݒ�
//! @attention �Q�[���ォ�璼�ڌĂяo�����Ƃ͔����AObjectManager�N���X����Ăяo���Ă��������B
void human::SetMoveRight()
{
	SetFlag(MoveFlag, MOVEFLAG_RIGHT);
}

//! @brief ������ݒ�
//! @attention �Q�[���ォ�璼�ڌĂяo�����Ƃ͔����AObjectManager�N���X����Ăяo���Ă��������B
void human::SetMoveWalk()
{
	SetFlag(MoveFlag, MOVEFLAG_WALK);
}

//! @brief �l�̈ړ����[�h���擾
//! @param nowdata ���݂̒l���擾�Ftrue�@�O�t���[���̒l���g�p�Ffalse
//! @return �����F1�@�O����F2�@��둖��F3�@���E����F4�@�ړ����ĂȂ��F0
int human::GetMovemode(bool nowdata)
{
	//�g�p����Ă��Ȃ����A��������Ă��Ȃ���ΏI��
	if( EnableFlag == false ){ return 0; }
	if( hp <= 0 ){ return 0; }

	if( nowdata == false ){	//�O�̃f�[�^
		//�����Ȃ�� 1
		if( GetFlag(MoveFlag_lt, MOVEFLAG_WALK) ){
			return 1;
		}
		//�O����Ȃ�� 2
		if( GetFlag(MoveFlag_lt, MOVEFLAG_FORWARD) ){
			return 2;
		}
		//��둖��Ȃ�� 3
		if( GetFlag(MoveFlag_lt, MOVEFLAG_BACK) ){
			return 3;
		}
		//���E����Ȃ�� 4
		if( GetFlag(MoveFlag_lt, (MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) ){
			return 4;
		}
	}
	else{					//���݂̃f�[�^
		//�����Ȃ�� 1
		if( GetFlag(MoveFlag, MOVEFLAG_WALK) ){
			return 1;
		}
		//�O����Ȃ�� 2
		if( GetFlag(MoveFlag, MOVEFLAG_FORWARD) ){
			return 2;
		}
		//��둖��Ȃ�� 3
		if( GetFlag(MoveFlag, MOVEFLAG_BACK) ){
			return 3;
		}
		//���E����Ȃ�� 4
		if( GetFlag(MoveFlag, (MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) ){
			return 4;
		}
	}

	//�ړ����ĂȂ�
	return 0;
}

//! @brief �ړ�������\���t���O���擾
//! @return �ݒ�l�iHuman_MoveFlag�񋓌^�j
//! @attention �f�o�b�N�p�֐��ł��B�ʏ��GetMovemode()�֐����g�p���Ă��������B
int human::GetMoveFlag(bool nowdata)
{
	if( nowdata == false ){	//�O�̃f�[�^
		return MoveFlag_lt;
	}
	else{					//���݂̃f�[�^
		return MoveFlag;
	}
}

//! @brief �X�R�[�v��ݒ�
//! @return �����Ftrue�@���s�Ffalse
bool human::SetEnableScope()
{
	int param_id;
	WeaponParameter data;

	//��������𑕔����ĂȂ���Ύ��s
	if( weapon[selectweapon] == NULL ){ return false; }

	//�����[�h���Ȃ玸�s
	if( weaponreloadcnt > 0 ){ return false; }

	//����̎�ޔԍ����擾
	weapon[selectweapon]->GetParamData(&param_id, NULL, NULL);

	//����̐ݒ�l���擾
	Param->GetWeapon(param_id, &data);

	//�X�R�[�v��ݒ�
	scopemode = data.scopemode;
	return true;
}

//! @brief �X�R�[�v������
void human::SetDisableScope()
{
	scopemode = 0;
}

//! @brief �X�R�[�v�ݒ���擾
int human::GetScopeMode()
{
	return scopemode;
}

//! @brief �����Əc���̌������擾
//! @param rx �������擾����|�C���^
//! @param ry �c�����擾����|�C���^
void human::GetRxRy(float *rx, float *ry)
{
	if( (rx == NULL)||(ry == NULL) ){ return; }

	*rx = rotation_x;
	*ry = armrotation_y;
}

//! @brief �����Əc���̌�����ݒ�
//! @param rx �ݒ肷�鉡��
//! @param ry �ݒ肷��c��
void human::SetRxRy(float rx, float ry)
{
	rotation_x = rx;
	armrotation_y = ry;
}

//! @brief �S�̂̉�]�p�x�擾
//! @return �c�����擾����|�C���^
//! @warning ���S���ē|���ۂ̊p�x�ł��BGetRxRy()�֐��Ŏ󂯎��l�Ƃ͈قȂ�܂��B
float human::GetDeadRy()
{
	return rotation_y;
}

//! @brief �W�����v
//! @attention �Q�[���ォ�璼�ڌĂяo�����Ƃ͔����AObjectManager�N���X����Ăяo���Ă��������B
void human::Jump()
{
	SetFlag(MoveFlag, MOVEFLAG_JUMP);
}

//! @brief �W�����v�E���n�̎擾
//! @param nowdata ���݂̒l���擾�Ftrue�@�O�t���[���̒l���g�p�Ffalse
//! @return �W�����v�F1�@���n�F2�@����ȊO�F0
//! @attention ���n�̔���͏�Ɍ��݂̒l���擾���܂��B�O�t���[���̏�Ԃ͎擾���܂���B
//! @note �����Đ�����p
int human::GetJumpLanding(bool nowdata)
{
	//�g�p����Ă��Ȃ����A��������Ă��Ȃ���ΏI��
	if( EnableFlag == false ){ return 0; }
	if( hp <= 0 ){ return 0; }

	if( nowdata == false ){	//�O�̃f�[�^
		if( move_y_landing == true ){ return 2; }
		if( GetFlag(MoveFlag_lt, MOVEFLAG_JUMP) ){ return 1; }
	}
	else{
		if( move_y_landing == true ){ return 2; }
		if( GetFlag(MoveFlag, MOVEFLAG_JUMP) ){ return 1; }
	}

	//�W�����v���Ă��Ȃ�
	return 0;
}

//! @brief �����̃u���b�NID�E�ʔԍ����擾
//! @param id �����̃u���b�NID���󂯎��|�C���^�iNULL�j
//! @param face �����̃u���b�N�̖ʔԍ����󂯎��|�C���^�iNULL�j
//! @attention �󒆂̏ꍇ�ȂǑ����Ƀu���b�N���Ȃ��ꍇ�A�u���b�NID�Ɩʔԍ��� -1 ��Ԃ��܂��B
void human::GetUnderBlock(int *id, int *face)
{
	if( id != NULL ){ *id = underblock_id; }
	if( face != NULL ){ *face = underblock_face; }
}

//! @brief ���������E�͂�������
//! @param rx ����
//! @param ry �c��
//! @param speed ���x
void human::AddPosOrder(float rx, float ry, float speed)
{
	move_x += cosf(rx) * cosf(ry) * speed;
	move_y += sinf(ry) * speed;
	move_z += sinf(rx) * cosf(ry) * speed;
}

//! @brief �e�� �� �Ƀq�b�g
//! @param attacks �e�̍U����
void human::HitBulletHead(int attacks)
{
	if( Invincible == false ){
		hp -= (int)((float)attacks * HUMAN_DAMAGE_HEAD) + GetRand(16);
	}
	ReactionGunsightErrorRange = 15;
}

//! @brief �e�� �㔼�g �Ƀq�b�g
//! @param attacks �e�̍U����
void human::HitBulletUp(int attacks)
{
	if( Invincible == false ){
		hp -= (int)((float)attacks * HUMAN_DAMAGE_UP) + GetRand(8);
	}
	ReactionGunsightErrorRange = 12;
}

//! @brief �e�� �����g �Ƀq�b�g
//! @param attacks �e�̍U����
void human::HitBulletLeg(int attacks)
{
	if( Invincible == false ){
		hp -= (int)((float)attacks * HUMAN_DAMAGE_LEG) + GetRand(6);
	}
	ReactionGunsightErrorRange = 8;
}

//! @brief �]���r�̍U�����q�b�g
void human::HitZombieAttack()
{
	if( Invincible == false ){
		hp -= HUMAN_DAMAGE_ZOMBIEU + GetRand(HUMAN_DAMAGE_ZOMBIEA);
	}
	ReactionGunsightErrorRange = 10;
}

//! @brief ��֒e�̔������q�b�g
//! @param attacks �����̍U����
//! @attention �����ɂ��v�Z�����O�ɍς܂��Ă��������B
void human::HitGrenadeExplosion(int attacks)
{
	if( Invincible == false ){
		hp -= attacks;
	}
	ReactionGunsightErrorRange = 10;
}

//! @brief ��e�t���O���Z�b�g
//! @param rx ��e��������
void human::SetHitFlag(float rx)
{
	HitFlag = true;
	Hit_rx = rx;
}

//! @brief ��e�������`�F�b�N����
//! @param rx ��e�����������󂯎��|�C���^�iNULL�j
//! @return ��e�����Ftrue�@��e���ĂȂ��Ffalse
//! @attention ���s����ƁA�t���O�� false �ɏ���������܂��B
bool human::CheckHit(float *rx)
{
	bool returnflag = HitFlag;
	if( rx != NULL ){ *rx = Hit_rx; }
	HitFlag = false;
	//Hit_rx = 0.0f;
	return returnflag;
}

//! @brief ���v�ړ��ʂ��擾
//! @return ���v�ړ���
float human::GetTotalMove()
{
	return totalmove;
}

//! @brief �����E���胂�[�V�����̃J�E���g�l�擾
//! @return �J�E���g�l
int human::GetMoveMotionCount()
{
	return MotionCtrl->GetMoveMotionCnt();
}

//! @brief �Ə��̏�Ԍ덷�̏���
//!	@attention ControlProcess()���O�ŏ������邱��
void human::GunsightErrorRange()
{
	//������
	StateGunsightErrorRange = 0;

	//�e����ɂ��덷��ݒ�
	if( GetFlag(MoveFlag, MOVEFLAG_WALK) ){
		StateGunsightErrorRange = 4;
	}
	if( GetFlag(MoveFlag, MOVEFLAG_FORWARD) ){
		StateGunsightErrorRange = 8;
	}
	if( GetFlag(MoveFlag, MOVEFLAG_BACK) ){
		StateGunsightErrorRange = 6;
	}
	if( GetFlag(MoveFlag, (MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) ){
		StateGunsightErrorRange = 7;
	}
	if( move_y_flag == true ){
		StateGunsightErrorRange = 22;
	}
	if( hp < 40 ){
		StateGunsightErrorRange += 3;
	}

	//��������𑕔����Ă����
	if( weapon[selectweapon] != NULL ){
		//����̐ݒ�f�[�^���擾
		int param;
		WeaponParameter data;
		weapon[selectweapon]->GetParamData(&param, NULL, NULL);
		Param->GetWeapon(param, &data);

		//�덷�� 1 ���炷
		ReactionGunsightErrorRange -= 1;

		//�덷�͈̔͂�␳
		if( ReactionGunsightErrorRange < 0 ){ ReactionGunsightErrorRange = 0; }
		if( ReactionGunsightErrorRange > data.ErrorRangeMAX ){ ReactionGunsightErrorRange = data.ErrorRangeMAX; }
	}
	else{
		ReactionGunsightErrorRange = 0;
	}
}

//! @brief ���S����Ɠ|��鏈��
//! @return �Î~�������́F4�@�|��I���������F3�@�|��Ă���Œ��F2�@�|��n�߂�F1�@�������Ȃ��F0
int human::CheckAndProcessDead(class Collision *CollD)
{
	if( CollD == NULL ){ return 0; }

	if( hp <= 0 ){
		//�r�̊p�x
		if( armrotation_y < 0.0f ){
			armrotation_y -= DegreeToRadian(6);
			if( armrotation_y < DegreeToRadian(-90) ){ armrotation_y = DegreeToRadian(-90); }
		}
		else{
			armrotation_y += DegreeToRadian(6);
			if( armrotation_y > DegreeToRadian(90) ){ armrotation_y = DegreeToRadian(90); }
		}
	}

#ifdef HUMAN_DEADBODY_COLLISION

	//�����F
	//
	//��ԁF0
	//�m�������Ȃ��n
	//�@HP��0�ȉ��ŁA�����t���ʂȂ�@��ԁF1
	//�@HP��0�ȉ��ŁA�ǂɓ����t���Ȃ�@��ԁF2
	//
	//��ԁF1
	//�m�X���n�߂�n
	//�@135�x�s������@��ԁF2
	//�@�ǂɓ���ł�����@��ԁF3
	//
	//��ԁF2
	//�m�����n
	//�@�����n�ʂɕt������@��ԁF4
	//
	//��ԁF3
	//�m�������点��n
	//�@�@
	//��ԁF4
	//�m�Œ�n

	float check_posx, check_posy, check_posz;

	if( deadstate == 0 ){
		if( hp <= 0 ){		//HP�� 0 �ȉ��ɂȂ����i���S�����j�u�ԂȂ�A�|���n�߂�
			float tr;

			//�Ō�ɍU�����󂯂��������v�Z
			tr = Hit_rx - rotation_x;
			tr = AngleNormalization(tr);

			//�̂̊p�x
			if( ((float)M_PI/2*-1 < tr)&&(tr < (float)M_PI/2) ){
				add_ry = HUMAN_DEADADDRY;
			}
			else{
				add_ry = HUMAN_DEADADDRY * -1;
			}

			//���̂����܂�ʂ悤�A������ +1.0 ����
			pos_y += 1.0f;

			//�������Ă��镐���S�Ď̂Ă�
			for(int i=0; i<TOTAL_HAVEWEAPON; i++){
				if( weapon[i] != NULL ){
					weapon[i]->Dropoff(pos_x, pos_y, pos_z, DegreeToRadian(10)*GetRand(36), 1.5f);
					weapon[i] = NULL;

					//���������̒e�����͏�����
					noweapon_bullets[i] = 0;
					noweapon_loadbullets[i] = 0;
				}
			}

			//�X�R�[�v���[�h������
			SetDisableScope();

			//�e��J�E���g������
			selectweaponcnt = 0;
			changeweaponidcnt = 0;
			weaponshotcnt = 0;
			weaponreloadcnt = 0;

			//���̃t���[���̓��̍��W���擾
			check_posx = pos_x + cosf(rotation_x*-1 - (float)M_PI/2) * sinf(add_ry) * HUMAN_HEIGHT;
			check_posy = pos_y + cosf(add_ry) * HUMAN_HEIGHT;
			check_posz = pos_z + sinf(rotation_x*-1 - (float)M_PI/2) * sinf(add_ry) * HUMAN_HEIGHT;

			if( CollD->CheckALLBlockInside(check_posx, check_posy, check_posz) == true ){
				deadstate = 2;
			}
			else{
				rotation_y += add_ry;
				deadstate = 1;
			}
			return 1;
		}
		return 0;
	}

	if( deadstate == 1 ){
		//135�x�ȏ�|��Ă����
		if( fabsf(rotation_y) >= DegreeToRadian(135) ){
			deadstate = 2;
			move_y = 0.0f;
			return 2;
		}

		if( pos_y <= (HUMAN_DEADLINE + 10.0f) ){
			//90�x�ȏ�|��Ă����
			if( fabsf(rotation_y) >= DegreeToRadian(90) ){
				deadstate = 4;
				return 2;
			}
		}

		//�O��ɓ|��
		if( rotation_y > 0.0f ){		//rotation_y < DegreeToRadian(135)
			add_ry += HUMAN_DEADADDRY;
		}
		else if( rotation_y < 0.0f ){	//rotation_y > DegreeToRadian(-135)
			add_ry -= HUMAN_DEADADDRY;
		}

		if( pos_y <= HUMAN_DEADLINE ){
			rotation_y += add_ry;
		}
		else{
			//���̃t���[���̓��̍��W���擾
			check_posx = pos_x + cosf(rotation_x*-1 - (float)M_PI/2) * sinf(rotation_y + add_ry) * HUMAN_HEIGHT;
			check_posy = pos_y + cosf(rotation_y + add_ry) * HUMAN_HEIGHT;
			check_posz = pos_z + sinf(rotation_x*-1 - (float)M_PI/2) * sinf(rotation_y + add_ry) * HUMAN_HEIGHT;

			if( CollD->CheckALLBlockInside(check_posx, check_posy, check_posz) == true ){
				deadstate = 3;
			}
			else{
				rotation_y += add_ry;
			}
		}

		return 2;
	}

	if( deadstate == 2 ){
		if( pos_y <= HUMAN_DEADLINE ){
			deadstate = 4;
			return 2;
		}

		if( move_y > -4.0f ){
			move_y -= 0.1f;
		}

		//���̃t���[���̑��̍��W
		check_posx = pos_x;
		check_posy = pos_y - move_y - HUMAN_HEIGHT*0.5f;
		check_posz = pos_z;

		if( CollD->CheckALLBlockInside(check_posx, check_posy, check_posz) == true ){
			deadstate = 4;
		}
		else{
			pos_y += move_y;
		}

		return 2;
	}

	if( deadstate == 3 ){
		//deadstate = 4;

		//90�x�ȏ�|��Ă����
		if( fabsf(rotation_y) >= DegreeToRadian(90) ){
			deadstate = 4;
			return 2;
		}

		//�O��ɓ|��
		if( rotation_y > 0.0f ){		//rotation_y < DegreeToRadian(90)
			add_ry += HUMAN_DEADADDRY;
		}
		else if( rotation_y < 0.0f ){	//rotation_y > DegreeToRadian(-90)
			add_ry -= HUMAN_DEADADDRY;
		}

		//���̃t���[���̑��̍��W���擾
		check_posx = pos_x - cosf(rotation_x*-1 - (float)M_PI/2) * sinf(rotation_y + add_ry) * HUMAN_HEIGHT;
		check_posy = pos_y + 0.1f;
		check_posz = pos_z - sinf(rotation_x*-1 - (float)M_PI/2) * sinf(rotation_y + add_ry) * HUMAN_HEIGHT;

		if( CollD->CheckALLBlockInside(check_posx, check_posy, check_posz) == true ){
			deadstate = 4;
			return 2;
		}

		//���̃t���[���̓��̍��W���擾
		check_posx = pos_x - cosf(rotation_x*-1 - (float)M_PI/2) * sinf(rotation_y + add_ry) * HUMAN_HEIGHT;
		check_posy = pos_y + cosf(rotation_y + add_ry) * HUMAN_HEIGHT;
		check_posz = pos_z - sinf(rotation_x*-1 - (float)M_PI/2) * sinf(rotation_y + add_ry) * HUMAN_HEIGHT;

		if( CollD->CheckALLBlockInside(check_posx, check_posy, check_posz) == true ){
			deadstate = 4;
			return 2;
		}

		//���̍��W���ړ�
		pos_x -= cosf(rotation_x*-1 - (float)M_PI/2) * sinf(add_ry) * HUMAN_HEIGHT;
		pos_z -= sinf(rotation_x*-1 - (float)M_PI/2) * sinf(add_ry) * HUMAN_HEIGHT;

		rotation_y += add_ry;
		return 2;
	}

	if( deadstate == 4 ){
		//�������Ȃ��i�Œ�j
		deadstate = 5;
		return 3;
	}

	if( deadstate == 5 ){
		//�������Ȃ��i�Œ�j
		return 4;
	}

	return 0;
#else
	if( fabsf(rotation_y) >= DegreeToRadian(90) ){
		return 4;
	}
	else if( rotation_y > 0.0f ){		//�|��n�߂Ă���΁A���̂܂ܓ|���B
		add_ry += HUMAN_DEADADDRY;
		rotation_y += add_ry;
		if( rotation_y >= DegreeToRadian(90) ){
			rotation_y = DegreeToRadian(90);
			deadstate = 5;
			return 3;
		}
		return 2;
	}
	else if( rotation_y < 0.0f ){	//�|��n�߂Ă���΁A���̂܂ܓ|���B
		add_ry -= HUMAN_DEADADDRY;
		rotation_y += add_ry;
		if( rotation_y <= DegreeToRadian(-90) ){
			rotation_y = DegreeToRadian(-90);
			deadstate = 5;
			return 3;
		}
		return 2;
	}
	else if( hp <= 0 ){		//HP�� 0 �ȉ��ɂȂ����i���S�����j�u�ԂȂ�A�|���n�߂�
		float tr;

		//�Ō�ɍU�����󂯂��������v�Z
		tr = Hit_rx - rotation_x;
		tr = AngleNormalization(tr);

		//�̂̊p�x
		if( ((float)M_PI/2*-1 < tr)&&(tr < (float)M_PI/2) ){
			add_ry = HUMAN_DEADADDRY;
		}
		else{
			add_ry = HUMAN_DEADADDRY * -1;
		}

		rotation_y += add_ry;

		//���̂����܂�ʂ悤�A������ +1.0 ����
		pos_y += 1.0f;

		//�������Ă��镐���S�Ď̂Ă�
		for(int i=0; i<TOTAL_HAVEWEAPON; i++){
			if( weapon[i] != NULL ){
				weapon[i]->Dropoff(pos_x, pos_y, pos_z, DegreeToRadian(10)*GetRand(36), 1.5f);
				weapon[i] = NULL;

				//���������̒e�����͏�����
				noweapon_bullets[i] = 0;
				noweapon_loadbullets[i] = 0;
			}
		}

		//�X�R�[�v���[�h������
		SetDisableScope();

		deadstate = 1;
		return 1;
	}

	return 0;
#endif
}

//! @brief ����ɂ��ړ��v�Z
//! @attention ���s����ƁA�e�L�[�t���O�� false �ɏ���������܂��B
void human::ControlProcess()
{
	//�i�s�����Ƒ��x������
	if( GetFlag(MoveFlag, MOVEFLAG_WALK) ){
		move_rx = DegreeToRadian(0);
		AddPosOrder(rotation_x*-1 + move_rx + (float)M_PI/2, 0.0f, HUMAN_PROGRESSWALK_ACCELERATION);
	}
	else if( GetFlag(MoveFlag, (MOVEFLAG_FORWARD | MOVEFLAG_BACK | MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) == MOVEFLAG_FORWARD ){
		move_rx = DegreeToRadian(0);
		AddPosOrder(rotation_x*-1 + move_rx + (float)M_PI/2, 0.0f, HUMAN_PROGRESSRUN_ACCELERATION);
	}
	else if( GetFlag(MoveFlag, (MOVEFLAG_FORWARD | MOVEFLAG_BACK | MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) == MOVEFLAG_BACK ){
		move_rx = DegreeToRadian(180);
		AddPosOrder(rotation_x*-1 + move_rx + (float)M_PI/2, 0.0f, HUMAN_REGRESSRUN_ACCELERATION);
	}
	else if( GetFlag(MoveFlag, (MOVEFLAG_FORWARD | MOVEFLAG_BACK | MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) == MOVEFLAG_LEFT ){
		move_rx = DegreeToRadian(90);
		AddPosOrder(rotation_x*-1 + move_rx + (float)M_PI/2, 0.0f, HUMAN_SIDEWAYSRUN_ACCELERATION);
	}
	else if( GetFlag(MoveFlag, (MOVEFLAG_FORWARD | MOVEFLAG_BACK | MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) == MOVEFLAG_RIGHT ){
		move_rx = DegreeToRadian(-90);
		AddPosOrder(rotation_x*-1 + move_rx + (float)M_PI/2, 0.0f, HUMAN_SIDEWAYSRUN_ACCELERATION);
	}
	else if( GetFlag(MoveFlag, (MOVEFLAG_FORWARD | MOVEFLAG_BACK | MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) == (MOVEFLAG_FORWARD | MOVEFLAG_LEFT) ){
		move_rx = DegreeToRadian(45);
		AddPosOrder(rotation_x*-1 + move_rx + (float)M_PI/2, 0.0f, HUMAN_PROGRESSRUN_SIDEWAYSRUN_ACCELERATION);
	}
	else if( GetFlag(MoveFlag, (MOVEFLAG_FORWARD | MOVEFLAG_BACK | MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) == (MOVEFLAG_BACK | MOVEFLAG_LEFT) ){
		move_rx = DegreeToRadian(135);
		AddPosOrder(rotation_x*-1 + move_rx + (float)M_PI/2, 0.0f, HUMAN_REGRESSRUN_SIDEWAYSRUN_ACCELERATION);
	}
	else if( GetFlag(MoveFlag, (MOVEFLAG_FORWARD | MOVEFLAG_BACK | MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) == (MOVEFLAG_BACK | MOVEFLAG_RIGHT) ){
		move_rx = DegreeToRadian(-135);
		AddPosOrder(rotation_x*-1 + move_rx + (float)M_PI/2, 0.0f, HUMAN_REGRESSRUN_SIDEWAYSRUN_ACCELERATION);
	}
	else if( GetFlag(MoveFlag, (MOVEFLAG_FORWARD | MOVEFLAG_BACK | MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) == (MOVEFLAG_FORWARD | MOVEFLAG_RIGHT) ){
		move_rx = DegreeToRadian(-45);
		AddPosOrder(rotation_x*-1 + move_rx + (float)M_PI/2, 0.0f, HUMAN_PROGRESSRUN_SIDEWAYSRUN_ACCELERATION);
	}
	else{
		move_rx = 0.0f;
	}

	//�t���O���o�b�N�A�b�v
	MoveFlag_lt = MoveFlag;

	//�L�[�t���O�����ɖ߂�
	MoveFlag = 0x00;
}

//! @brief �}�b�v�Ƃ̓����蔻��
//! @param CollD Collision�N���X�̃|�C���^
//! @param inblockdata BlockDataInterface�N���X�̃|�C���^
//! @param AddCollisionFlag �ǉ��̓����蔻��t���O
//! @param player �Ώۂ̐l�����v���C���[���ǂ���
void human::CollisionMap(class Collision *CollD, class BlockDataInterface *inblockdata, bool AddCollisionFlag, bool player)
{
	if( CollD == NULL ){ return; }
	if( inblockdata == NULL ){ return; }

	float pos_x2, pos_y2, pos_z2;
	float dist_x, dist_y, dist_z;
	float box_min_x, box_min_y, box_min_z, box_max_x, box_max_y, box_max_z;
	bool CheckBlockID[MAX_BLOCKS];
	float speed;
	float Dist;
	bool FallFlag;

	//�u���b�NID�Ɩʔԍ��́A�ЂƂ܂� -1 �ɂ���B
	underblock_id = -1;
	underblock_face = -1;

	//�����_�̍��W���o�b�N�A�b�v
	pos_x2 = pos_x;
	pos_y2 = pos_y;
	pos_z2 = pos_z;

	//�ړ����f
	pos_x += move_x;
	pos_z += move_z;

	//���t���[���̈ړ��ʂ�����
	move_x *= HUMAN_ATTENUATION;
	move_z *= HUMAN_ATTENUATION;

	//���{��XOPS�ł͂����Ől���m�̓����蔻����s���B
	//�@OpenXOPS�ɂ����Ă͕ʃ^�C�~���O�Ōv�Z���邽�߁A�ȉ�dist_y�� 0.0f �m��ł���l�Ƃ��Ă͖��Ӗ��B

	//�ړ��x�N�g���Z�o
	dist_x = pos_x - pos_x2;
	dist_y = pos_y - pos_y2;
	dist_z = pos_z - pos_z2;
	speed = sqrtf(dist_x*dist_x + dist_z*dist_z);
	VectorNormalization(&dist_x, &dist_y, &dist_z);

	int bs = inblockdata->GetTotaldatas();

	//�l��AABB���쐬
	box_min_x = pos_x - HUMAN_MAPCOLLISION_CHECK_MAXDIST - COLLISION_ADDSIZE;
	box_min_y = pos_y - HUMAN_MAPCOLLISION_CHECK_MAXDIST - COLLISION_ADDSIZE;
	box_min_z = pos_z - HUMAN_MAPCOLLISION_CHECK_MAXDIST - COLLISION_ADDSIZE;
	box_max_x = pos_x + HUMAN_MAPCOLLISION_CHECK_MAXDIST + COLLISION_ADDSIZE;
	box_max_y = (pos_y + HUMAN_HEIGHT) + HUMAN_MAPCOLLISION_CHECK_MAXDIST + COLLISION_ADDSIZE;
	box_max_z = pos_z + HUMAN_MAPCOLLISION_CHECK_MAXDIST + COLLISION_ADDSIZE;

	//AABB����őΏۃu���b�N��e��肷��
	for(int i=0; i<bs; i++){
		CheckBlockID[i] = CollD->CheckBlockAABB(i, box_min_x, box_min_y, box_min_z, box_max_x, box_max_y, box_max_z);
	}

	if( (speed != 0.0f)||(move_y != 0.0f) ){

		//�����œ����蔻��
		for(int i=0; i<bs; i++){
			if( CheckBlockID[i] == true ){
				if( CollisionBlockScratch(CollD, inblockdata, i, &pos_x, &pos_y, &pos_z, pos_x2, pos_y2, pos_z2, pos_x, pos_y + HUMAN_HEIGHT-0.22f, pos_z, 0x01) == true ){
					if( move_y > 0.0f ){ move_y = 0.0f; }
				}
			}
		}

		//�����œ����蔻��
		for(int i=0; i<bs; i++){
			if( CheckBlockID[i] == true ){
				CollisionBlockScratch(CollD, inblockdata, i, &pos_x, &pos_y, &pos_z, pos_x2, pos_y2, pos_z2, pos_x, pos_y, pos_z, 0x00);
			}
		}

		//�����炢�œ����蔻��
		for(int i=0; i<bs; i++){
			if( CheckBlockID[i] == true ){
				CollisionBlockScratch(CollD, inblockdata, i, &pos_x, &pos_y, &pos_z, pos_x2, pos_y2, pos_z2, pos_x + dist_x*HUMAN_MAPCOLLISION_R, pos_y + HUMAN_MAPCOLLISION_HEIGHT, pos_z + dist_z*HUMAN_MAPCOLLISION_R, 0x02);
				CollisionBlockScratch(CollD, inblockdata, i, &pos_x, &pos_y, &pos_z, pos_x2, pos_y2, pos_z2, pos_x + dist_z*HUMAN_MAPCOLLISION_R, pos_y + HUMAN_MAPCOLLISION_HEIGHT, pos_z + dist_x*HUMAN_MAPCOLLISION_R, 0x02);
				CollisionBlockScratch(CollD, inblockdata, i, &pos_x, &pos_y, &pos_z, pos_x2, pos_y2, pos_z2, pos_x - dist_z*HUMAN_MAPCOLLISION_R, pos_y + HUMAN_MAPCOLLISION_HEIGHT, pos_z - dist_x*HUMAN_MAPCOLLISION_R, 0x02);
			}
		}

		if( AddCollisionFlag == true ){
			//���t�߂̒ǉ������蔻��
			for(int i=0; i<bs; i++){
				if( CheckBlockID[i] == true ){
					CollisionBlockScratch(CollD, inblockdata, i, &pos_x, &pos_y, &pos_z, pos_x2, pos_y2, pos_z2, pos_x, pos_y + HUMAN_MAPCOLLISION_ADD_HEIGHT_A, pos_z, 0x02);
					CollisionBlockScratch(CollD, inblockdata, i, &pos_x, &pos_y, &pos_z, pos_x2, pos_y2, pos_z2, pos_x, pos_y + HUMAN_MAPCOLLISION_ADD_HEIGHT_B, pos_z, 0x02);
				}
			}
		}

		//�Ζʂ�o��
		if( (fabsf(move_x) > 0.2f)||(fabsf(move_z) > 0.2f) ){
			if( move_y_upper == 0 ){
				bool flag = false;
				for(int i=0; i<bs; i++){
					if( CheckBlockID[i] == true ){
						//���̂ܐ�̂݃u���b�N�ɖ��܂��Ă����
						if(
							(CollD->CheckBlockInside(i, pos_x + dist_x*2.0f, pos_y, pos_z + dist_z*2.0f, true, NULL) == true)&&
							(CollD->CheckBlockInside(i, pos_x + dist_x*2.0f, pos_y + HUMAN_MAPCOLLISION_CLIMBHEIGHT, pos_z + dist_z*2.0f, true, NULL) == false)
						){
							flag = true;

							int face;
							blockdata bdata;

							//�����̖ʂ̊p�x���擾
							//�@�������Ƀu���b�N�����邱�Ƃ͊m��Ȃ̂ŁADummyRay�͕K�v�Ȃ��E�E�E�͂��B
							if( CollD->CheckBlockIntersectRay(i, pos_x, pos_y, pos_z, 0.0f, -1.0f, 0.0f, &face, &Dist, 1.2f) == true ){

								inblockdata->Getdata(&bdata, i);

								//�Ζʂɗ����Ă���Ȃ�΁A�㏸����
								if( acosf(bdata.material[face].vy) > HUMAN_MAPCOLLISION_SLOPEANGLE ){
									flag = false;
									break;
								}
							}
						}
					}
				}

				//�㏸����i�o��j
				if( flag == true ){
					pos_y += HUMAN_MAPCOLLISION_CLIMBADDY;
					move_y *= 0.2f;
				}
			}
		}

		//�ړ��悪�u���b�N�ւ߂荞��ł���Ȃ�ړ��𖳌��ɂ���
		for(int i=0; i<bs; i++){
			if( CheckBlockID[i] == true ){
				//�@�����C�̎n�_���u���b�N�̓������͔��肵�Ȃ��B
				if( CollD->CheckBlockIntersectRay(i, pos_x, pos_y + HUMAN_HEIGHT-2.0f, pos_z, 0.0f, -1.0f, 0.0f, NULL, &Dist, (HUMAN_HEIGHT-2.0f*2)) == true ){
					pos_x = pos_x2;
					pos_z = pos_z2;
				}

				//�ړ��悪�u���b�N�ɂ߂荞�ށ��ړ�����߂荞�ށ@�Ȃ�΁A�ړ��𖳌��ɂ���
				if( CollD->CheckBlockInside(i, pos_x, pos_y + HUMAN_HEIGHT-0.6f, pos_z, true, NULL) == true ){
					//�����F�����̔���������B�\���ɉ����ʒu�ł���Ηǂ��A11�{�ł���K�v�͂Ȃ��H
					//�@��AABB�̔���͈͉z���ĉ����̂ŁA�S���u���b�N���ג����B
					if( CollD->CheckALLBlockInside(pos_x + move_x*11.0f, pos_y + HUMAN_HEIGHT-0.6f, pos_z + move_z*11.0f) == true ){
						pos_x = pos_x2;
						pos_y = pos_y2;
						pos_z = pos_z2;
						if( move_y > 0.0f ){ move_y = 0.0f; }
					}
				}
			}
		}
	}

	if( move_y_upper > 0 ) { move_y_upper -= 1; }

	//��������
	move_y_flag = false;
	FallFlag = false;
	for(int ycnt=0; ycnt<3; ycnt++){
		int cnt;
		float ang = atan2f(move_z, move_x);
		float x, y, z;

		//����
		pos_y += move_y*0.33f;

		if( player == true ){
			y = pos_y + HUMAN_MAPCOLLISION_GROUND_HEIGHT;

			//4�������� No.1
			cnt = 0;
			x = pos_x + cosf(ang)*HUMAN_MAPCOLLISION_GROUND_R1;
			z = pos_z + sinf(ang)*HUMAN_MAPCOLLISION_GROUND_R1;
			for(int i=0; i<bs; i++){
				if( CheckBlockID[i] == true ){
					if( CollD->CheckBlockInside(i, x, y, z, true, NULL) == true ){ cnt += 1; break; }
				}
			}
			x = pos_x - cosf(ang)*HUMAN_MAPCOLLISION_GROUND_R1;
			z = pos_z - sinf(ang)*HUMAN_MAPCOLLISION_GROUND_R1;
			for(int i=0; i<bs; i++){
				if( CheckBlockID[i] == true ){
					if( CollD->CheckBlockInside(i, x, y, z, true, NULL) == true ){ cnt += 1; break; }
				}
			}
			x = pos_x + cosf(ang + (float)M_PI/2)*HUMAN_MAPCOLLISION_GROUND_R1;
			z = pos_z + sinf(ang + (float)M_PI/2)*HUMAN_MAPCOLLISION_GROUND_R1;
			for(int i=0; i<bs; i++){
				if( CheckBlockID[i] == true ){
					if( CollD->CheckBlockInside(i, x, y, z, true, NULL) == true ){ cnt += 1; break; }
				}
			}
			x = pos_x + cosf(ang - (float)M_PI/2)*HUMAN_MAPCOLLISION_GROUND_R1;
			z = pos_z + sinf(ang - (float)M_PI/2)*HUMAN_MAPCOLLISION_GROUND_R1;
			for(int i=0; i<bs; i++){
				if( CheckBlockID[i] == true ){
					if( CollD->CheckBlockInside(i, x, y, z, true, NULL) == true ){ cnt += 1; break; }
				}
			}
			if( cnt == 4 ){ FallFlag = true; break; }

			//4�������� No.2
			cnt = 0;
			x = pos_x + cosf(ang)*HUMAN_MAPCOLLISION_GROUND_R2;
			z = pos_z + sinf(ang)*HUMAN_MAPCOLLISION_GROUND_R2;
			for(int i=0; i<bs; i++){
				if( CheckBlockID[i] == true ){
					if( CollD->CheckBlockInside(i, x, y, z, true, NULL) == true ){ cnt += 1; break; }
				}
			}
			x = pos_x - cosf(ang)*HUMAN_MAPCOLLISION_GROUND_R2;
			z = pos_z - sinf(ang)*HUMAN_MAPCOLLISION_GROUND_R2;
			for(int i=0; i<bs; i++){
				if( CheckBlockID[i] == true ){
					if( CollD->CheckBlockInside(i, x, y, z, true, NULL) == true ){ cnt += 1; break; }
				}
			}
			x = pos_x + cosf(ang + (float)M_PI/2)*HUMAN_MAPCOLLISION_GROUND_R2;
			z = pos_z + sinf(ang + (float)M_PI/2)*HUMAN_MAPCOLLISION_GROUND_R2;
			for(int i=0; i<bs; i++){
				if( CheckBlockID[i] == true ){
					if( CollD->CheckBlockInside(i, x, y, z, true, NULL) == true ){ cnt += 1; break; }
				}
			}
			x = pos_x + cosf(ang - (float)M_PI/2)*HUMAN_MAPCOLLISION_GROUND_R2;
			z = pos_z + sinf(ang - (float)M_PI/2)*HUMAN_MAPCOLLISION_GROUND_R2;
			for(int i=0; i<bs; i++){
				if( CheckBlockID[i] == true ){
					if( CollD->CheckBlockInside(i, x, y, z, true, NULL) == true ){ cnt += 1; break; }
				}
			}
			if( cnt == 4 ){ FallFlag = true; break; }
		}
		else{
			y = pos_y + HUMAN_MAPCOLLISION_GROUND_HEIGHT;

			//�^������
			x = pos_x;
			z = pos_z;
			for(int i=0; i<bs; i++){
				if( CheckBlockID[i] == true ){
					if( CollD->CheckBlockInside(i, x, y, z, true, NULL) == true ){ FallFlag = true; break; }
				}
			}
			if( FallFlag == true ){ break; }

			//�O���֏����O���Ĕ���
			x = pos_x + cosf(ang)*HUMAN_MAPCOLLISION_GROUND_R3;
			z = pos_z + sinf(ang)*HUMAN_MAPCOLLISION_GROUND_R3;
			for(int i=0; i<bs; i++){
				if( CheckBlockID[i] == true ){
					if( CollD->CheckBlockInside(i, x, y, z, true, NULL) == true ){ FallFlag = true; break; }
				}
			}
			if( FallFlag == true ){ break; }
		}

		//�������x���Z
		move_y -= HUMAN_DAMAGE_SPEED;
		if( move_y < HUMAN_DAMAGE_MAXSPEED ){ move_y = HUMAN_DAMAGE_MAXSPEED; }

		move_y_flag = true;
	}

	move_y_landing = false;

	//�n�ʂɐڒn���Ă���ꍇ�̏���
	if( FallFlag == true ){
		int id, face;
		blockdata bdata;
		bool flag = true;

		if( move_y < HUMAN_MAPCOLLISION_GROUND_HEIGHT ){ move_y_landing = true; }

		if( Invincible == false ){
			//�����_���[�W��^����
			if( move_y < HUMAN_DAMAGE_MINSPEED ){
				hp -= GetRand(6) + (int)((float)HUMAN_DAMAGE_MAXFALL / fabsf(HUMAN_DAMAGE_MAXSPEED - (HUMAN_DAMAGE_MINSPEED)) * fabsf(move_y - (HUMAN_DAMAGE_MINSPEED)));
			}
		}

		move_y = 0.0f;

		if( (GetFlag(MoveFlag_lt, MOVEFLAG_JUMP))&&(move_y_upper == 0) ){
			move_y = HUMAN_JUMP_SPEED;

			//���[�V�������s
			MotionCtrl->Jump();
		}

		if( GetRand(4) == 0 ){
			if( (GetFlag(MoveFlag_lt, (MOVEFLAG_FORWARD | MOVEFLAG_BACK | MOVEFLAG_LEFT | MOVEFLAG_RIGHT | MOVEFLAG_WALK)) == 0x00)&&(move_y_upper == 0) ){ flag = false; }
		}

		//�����̃u���b�N�Ɩʔԍ����擾
		id = -1;
		for(int i=0; i<bs; i++){
			if( CheckBlockID[i] == true ){
				if( CollD->CheckBlockIntersectRay(i, pos_x, pos_y + 2.5f, pos_z, 0.0f, -1.0f, 0.0f, &face, &Dist, (2.5f + 1.0f)) == true ){
					id = i;
					break;
				}
			}
		}

		if( id != -1 ){
			//�u���b�NID�Ɩʔԍ����L��
			underblock_id = id;
			underblock_face = face;

			if( flag == true ){
				inblockdata->Getdata(&bdata, id);

				//�Ζʂɗ����Ă���Ȃ�΁A�Ζʂ����点�鏈��
				if( acosf(bdata.material[face].vy) > HUMAN_MAPCOLLISION_SLOPEANGLE ){
					move_x = bdata.material[face].vx * 1.2f;
					move_y = bdata.material[face].vy * -0.5f;
					move_z = bdata.material[face].vz * 1.2f;

					for(int i=0; i<bs; i++){
						if( CheckBlockID[i] == true ){
							if( CollD->CheckBlockInside(i, pos_x + move_x*3.0f, pos_y + move_y*3.0f, pos_z + move_z*3.0f, true, NULL) == true ){
								move_y = 0.0f;
								if( CollD->CheckBlockInside(i, pos_x + move_x*3.0f, pos_y, pos_z + move_z*3.0f, true, NULL) == true ){
									move_x = 0.0f;
									move_z = 0.0f;
									break;
								}
							}
						}
					}

					move_y_upper = 8;
				}
			}
		}
	}

	//�ړ��x�N�g���ĎZ�o
	dist_x = pos_x - pos_x2;
	dist_y = pos_y - pos_y2;
	dist_z = pos_z - pos_z2;

	Dist = sqrtf(dist_x*dist_x + dist_z*dist_z);
	speed = sqrtf((move_x*2)*(move_x*2) + (move_z*2)*(move_z*2));

	//���炩�Ɉړ��ʂ����������Ȃ�Ό��ɖ߂�
	if( (sqrtf(dist_x*dist_x + dist_y*dist_y + dist_z*dist_z) > HUMAN_MAPCOLLISION_CHECK_MAXDIST)||(Dist - speed > 1.0f) ){
		pos_x = pos_x2;
		pos_y = pos_y2;
		pos_z = pos_z2;
	}

	if( player == true ){
		speed = VectorNormalization(&dist_x, &dist_y, &dist_z);

		//�ړ���Ƃ̊ԂɃu���b�N������Ȃ�Ό��ɖ߂�
		//�@�����C�̎n�_���u���b�N�̓������͔��肵�Ȃ��B
		if( CollD->CheckALLBlockIntersectRay(pos_x2, pos_y2 + HUMAN_MAPCOLLISION_CHECK_HEIGHT, pos_z2, dist_x, dist_y, dist_z, NULL, NULL, &Dist, speed) == true ){
			pos_x = pos_x2;
			pos_y = pos_y2;
			pos_z = pos_z2;
		}
	}
}

//! @brief �u���b�N�ƏՓˎ��̃x�N�g���v�Z
//! @param CollD Collision�N���X�̃|�C���^
//! @param inblockdata BlockDataInterface�N���X�̃|�C���^
//! @param blockid �u���b�NID
//! @param px X���W
//! @param py Y���W
//! @param pz Z���W
//! @param px_old �O�t���[����X���W
//! @param py_old �O�t���[����Y���W
//! @param pz_old �O�t���[����Z���W
//! @param in_vx ���肷��X���W
//! @param in_vy ���肷��Y���W
//! @param in_vz ���肷��Z���W
//! @param mode ���W�␳���[�h�i0x00�F�ʏ�A0x01�FY���W�㏸�֎~�A0x02�FY���W�Œ�j
//! @return �����Ftrue�@���s�Ffalse
bool human::CollisionBlockScratch(class Collision *CollD, class BlockDataInterface *inblockdata, int blockid, float *px, float *py, float *pz, float px_old, float py_old, float pz_old, float in_vx, float in_vy, float in_vz, int mode)
{
	if( CollD == NULL ){ return false; }
	if( inblockdata == NULL ){ return false; }
	if( (blockid < 0)||(inblockdata->GetTotaldatas() <= blockid) ){ return false; }
	//if( (*px == px_old)&&(*py == py_old)&&(*pz == pz_old) ){ return false; }

	float px2, py2, pz2;
	float vx, vy, vz;
	float dist;
	int face;
	float face_vx, face_vy, face_vz, face_angle, face_angle_per;
	float temp;

	//�����F
	//OpenXOPS�̓����蔻��́A�v�Z�덷���z�����邽�ߋ͂��ɑ傫�߂Ƀu���b�N�𔻒肵�Ă���B
	//���̂��߁A�����̃u���b�N�̂Ȃ��ڂň����|����ꍇ��������B
	//�΍�Ƃ��āA�����̔���imode 0x00 �ʏ�j�̏ꍇ�́A���肷��Y���W��␳����B
	if( mode == 0 ){
		in_vy += COLLISION_ADDSIZE;
	}

	px2 = *px;
	py2 = *py;
	pz2 = *pz;

	//���݂̈ړ��x�N�g���Ƌ����Z�o
	vx = *px - px_old;
	vy = *py - py_old;
	vz = *pz - pz_old;
	dist = VectorNormalization(&vx, &vy, &vz);

	//AABB�����
	float rmin_x, rmin_y, rmin_z, rmax_x, rmax_y, rmax_z;
	GetAABBRay(in_vx - vx*dist, in_vy - vy*dist, in_vz - vz*dist, vx, vy, vz, dist, &rmin_x, &rmin_y, &rmin_z, &rmax_x, &rmax_y, &rmax_z);

	//�Ǔ˂����u���b�N�ʎ擾
	face = -1;
	if( CollD->CheckBlockAABB(blockid, rmin_x, rmin_y, rmin_z, rmax_x, rmax_y, rmax_z) == false ){ return false; } 
	if( CollD->CheckBlockIntersectRay(blockid, in_vx - vx*dist, in_vy - vy*dist, in_vz - vz*dist, vx, vy, vz, &face, &temp, dist) == false ){ return false; }

	//�u���b�N�̖ʂɑ΂���x�N�g���̂Ȃ��p�����߂�
	if( CollD->AngleVector2(blockid, face, vx, vy, vz, &face_angle) == false ){ return false; }

	//�p�x�̊����Z�o
	//if( face_angle != 0.0f ){ face_angle_per = 1.0f - face_angle / ((float)M_PI); }
	if( face_angle != 0.0f ){ face_angle_per = ((float)M_PI) / face_angle - 1.0f; }
	else{ face_angle_per = 0.0f; }

	//�u���b�N�ʂ̖@���x�N�g���擾
	blockdata bdata;
	inblockdata->Getdata(&bdata, blockid);
	face_vx = bdata.material[face].vx;
	face_vy = bdata.material[face].vy;
	face_vz = bdata.material[face].vz;

	//�ړ��x�N�g���{�@���x�N�g�����Z�o
	float vx2, vy2, vz2;
	vx2 = vx + face_vx;
	vy2 = vy + face_vy;
	vz2 = vz + face_vz;
	VectorNormalization(&vx2, &vy2, &vz2);

	//�Ȃ��p�ɉ����Ĉړ��x�N�g�����Z�o
	vx = vx2 * (1.0f-face_angle_per) + vx * face_angle_per;
	vy = vy2 * (1.0f-face_angle_per) + vy * face_angle_per;
	vz = vz2 * (1.0f-face_angle_per) + vz * face_angle_per;
	VectorNormalization(&vx, &vy, &vz);

	//���������Ɉړ����Ȃ��Ȃ�΁A�@���x�N�g����p����
	if( (vx == 0.0f)&&(vz == 0.0f) ){
		vx = bdata.material[face].vx;
		vy = bdata.material[face].vy;
		vz = bdata.material[face].vz;
	}

	//�ŏI�I�Ȉʒu���v�Z
	temp = face_angle_per * dist;
	*px = vx*temp + px_old;
	*py = vy*temp + py_old;
	*pz = vz*temp + pz_old;

	//�ŏI�I�Ȉʒu���u���b�N�̓����Ȃ�΁A�ړ�����
	//if( CollD->CheckALLBlockInside(*px, *py, *pz) == true ){
	if( CollD->CheckBlockInside(blockid, *px, *py, *pz, true, NULL) == true ){
		*px = px_old;
		*py = py_old;
		*pz = pz_old;
	}

	//���W�␳
	if( mode == 0x01 ){
		if( *py > py2 ){ *py = py2; }	//������ɂ͈ړ������Ȃ�
	}
	if( mode == 0x02 ){
		*py = py2;						//Y���W�͕ς��Ȃ�
	}

	return true;
}

//! @brief �v�Z�����s�i�����蔻��j
//! @param CollD Collision�̃|�C���^
//! @param inblockdata BlockDataInterface�̃|�C���^
//! @param AddCollisionFlag �ǉ��̓����蔻��t���O
//! @param player �Ώۂ̐l�����v���C���[���ǂ���
//! @param F5mode �㏸�@�\�iF5���Z�j�̃t���O�@�i�L���Ftrue�@�����Ffalse�j
//! @return �����Ȃ��F0�@�ʏ폈���F1�@���S���ē|��I���������F2�@�Î~�������́F3�@�n�`�ɂ�莀�S��������F4
int human::ProcessObject(class Collision *CollD, class BlockDataInterface *inblockdata, bool AddCollisionFlag, bool player, bool F5mode)
{
	if( CollD == NULL ){ return 0; }
	if( inblockdata == NULL ){ return 0; }
	if( EnableFlag == false ){ return 0; }

	//F5���g�p���Ă���΁A�����I�ɏ㏸
	if( F5mode == true ){
		move_y = 0.0f;
		pos_y += 5.0f;
	}

	if( deadstate == 5 ){ return 3; }

	int WeaponReloadMotionCnt;
	int WeaponChangeWeaponIDCnt;
	int CheckDead;

	//����؂�ւ��J�E���g
	if( selectweaponcnt > 0 ){
		selectweaponcnt -= 1;
	}
	if( changeweaponidcnt > 0 ){
		changeweaponidcnt -= 1;
	}

	//�A�˃J�E���g���c���Ă���΁A1 ���炷
	if( weaponshotcnt > 0 ){
		weaponshotcnt -= 1;
	}

	//����̃t���[���Ŕ��C���Ȃ������Ȃ�΁A�o�[�X�g���[�h�p�J�E���g�� 0 �ɂ���B
	if( weaponshotframe == false ){ weaponburstmodecnt = 0; }
	weaponshotframe = false;

	//�����[�h�J�E���g���c���Ă���� 1 ���炵�A�����[�h�J�E���g�� 0 �Ȃ�΃����[�h���������s
	if( weaponreloadcnt > 0 ){
		weaponreloadcnt -= 1;
		if( weaponreloadcnt == 0 ){
			if( weapon[selectweapon] != NULL ){ weapon[selectweapon]->RunReload(); }
		}
	}

	//�����[�h�E����ID�ؑփJ�E���g�擾�i���[�V�����p�j
	if( weapon[selectweapon] != NULL ){
		WeaponReloadMotionCnt = weaponreloadcnt;
		WeaponChangeWeaponIDCnt = changeweaponidcnt;
	}
	else{
		WeaponReloadMotionCnt = 0;
		WeaponChangeWeaponIDCnt = 0;
	}

	//�Ə��̏�Ԍ덷�̏���
	GunsightErrorRange();

	//���S����Ɠ|��鏈��
	CheckDead = CheckAndProcessDead(CollD);
	if( CheckDead == 3 ){ return 2; }

	//if( CheckDead == 0 ){
	if( (CheckDead == 0)||(deadstate == 1) ){
		int hp_old = hp;

		//�i�s�����Ƒ��x������
		ControlProcess();

		//�}�b�v�Ƃ̓����蔻��
		CollisionMap(CollD, inblockdata, AddCollisionFlag, player);

		//����̃}�b�v�Ƃ̓����蔻���HP���[���ɂȂ����Ȃ�A�n�`�ɂ�鎀�S
		if( (hp_old > 0)&&(hp <= 0) ){
			return 4;
		}

		//����̓����蔻���-100.0��艺�ɗ�������A���S
		if( CheckDead == 0 ){
			if( pos_y < HUMAN_DEADLINE ){
				//pos_y = HUMAN_DEADLINE;
				hp = 0;
				return 4;
			}
		}
	}


	//���ݑ������镐��̃N���X���擾
	class weapon *nowweapon;
	int weapon_paramid;
	nowweapon = weapon[selectweapon];

	if( nowweapon == NULL ){		//��Ԃ�
		weapon_paramid = ID_WEAPON_NONE;
	}
	else{							//��������������Ă���
		nowweapon->GetParamData(&weapon_paramid, NULL, NULL);
	}

	//���[�V�����v�Z
	MotionCtrl->ProcessObject(rotation_x, armrotation_y, weapon_paramid, WeaponReloadMotionCnt, WeaponChangeWeaponIDCnt, MoveFlag_lt, hp, player);

	if( CheckDead != 0 ){ return 3; }
	return 1;
}

//! @brief �W���덷���擾
int human::GetGunsightErrorRange()
{
	return StateGunsightErrorRange + ReactionGunsightErrorRange;
}

//! @brief �`��
//! @param d3dg D3DGraphics�̃|�C���^
//! @param Resource ResourceManager�̃|�C���^
//! @param DrawArmOnly �r�ƕ���̂ݕ`�悷��
//! @param player �Ώۂ̐l�����v���C���[���ǂ���
//! @param NoModel ���f���`�斳����
//! @todo �r�̈ʒu���s��ŋ��߂�
void human::Render(class D3DGraphics *d3dg, class ResourceManager *Resource, bool DrawArmOnly, bool player, bool NoModel)
{
	//���g�p�����΍�
	UNREFERENCED_PARAMETER(player);

	//����������������Ă��Ȃ���΁A�������Ȃ�
	if( d3dg == NULL ){ return; }
	if( Resource == NULL ){ return; }
	if( EnableFlag == false ){ return; }

	//���[�V�����擾
	int upmodel, armmodel, legmodel;
	float armry, legrx;
	MotionCtrl->GetRenderMotion(&armry, &legrx, &upmodel, &armmodel, &legmodel);

	//���ݑ������镐��̃N���X���擾
	class weapon *nowweapon;
	nowweapon = weapon[selectweapon];

	if( DrawArmOnly == false ){
		float add_x, add_y, add_z;

		//�㔼�g��`��
		if( rotation_y != 0.0f ){		//���S���ē|��Ă��� or �|��n�߂�
			add_x = cosf(rotation_x*-1 - (float)M_PI/2)*sinf(rotation_y)*-1.0f;
			add_y = cosf(rotation_y)*-1.0f;
			add_z = sinf(rotation_x*-1 - (float)M_PI/2)*sinf(rotation_y)*-1.0f;
		}
		else{
			add_x = 0.0f;
			add_y = -1.0f;
			add_z = 0.0f;
		}
		d3dg->SetWorldTransform(pos_x + add_x, pos_y + add_y, pos_z + add_z, rotation_x + (float)M_PI, rotation_y, upmodel_size);
		d3dg->RenderModel(upmodel, id_texture, DarkModelFlag, false, NoModel);

		//����`��
		if( rotation_y != 0.0f ){		//���S���ē|��Ă��� or �|��n�߂�
			add_x = cosf(rotation_x*-1 - (float)M_PI/2)*sinf(rotation_y)*-0.2f;
			add_y = cosf(rotation_y)*-0.2f;
			add_z = sinf(rotation_x*-1 - (float)M_PI/2)*sinf(rotation_y)*-0.2f;
		}
		else{
			add_x = 0.0f;
			add_y = -0.2f;
			add_z = 0.0f;
		}
		d3dg->SetWorldTransform(pos_x + add_x, pos_y + add_y, pos_z + add_z, legrx + (float)M_PI, rotation_y, legmodel_size);
		d3dg->RenderModel(legmodel, id_texture, DarkModelFlag, false, NoModel);
	}

	//�r��`��
	if( rotation_y != 0.0f ){		//���S���ē|��Ă��� or �|��n�߂�
		float add_x = cosf(rotation_x*-1 - (float)M_PI/2)*sinf(rotation_y)*16.0f;
		float add_y = cosf(rotation_y)*16.0f;
		float add_z = sinf(rotation_x*-1 - (float)M_PI/2)*sinf(rotation_y)*16.0f;
		d3dg->SetWorldTransform(pos_x + add_x, pos_y + add_y, pos_z + add_z, rotation_x + (float)M_PI, armrotation_y + rotation_y, armmodel_size);
		d3dg->RenderModel(armmodel, id_texture, DarkModelFlag, false, NoModel);
	}
	else if( nowweapon == NULL ){	//��Ԃ�
		d3dg->SetWorldTransform(pos_x, pos_y + 16.0f, pos_z, rotation_x + (float)M_PI, armry, armmodel_size);
		d3dg->RenderModel(armmodel, id_texture, DarkModelFlag, false, NoModel);
	}
	else{							//��������������Ă���
		//����̃��f���ƃe�N�X�`�����擾
		int id_param;
		WeaponParameter paramdata;
		int model, texture;
		nowweapon->GetParamData(&id_param, NULL, NULL);
		Param->GetWeapon(id_param, &paramdata);
		Resource->GetWeaponModelTexture(id_param, &model, &texture);

		//�r��`��
		d3dg->SetWorldTransform(pos_x, pos_y + 16.0f, pos_z, rotation_x + (float)M_PI, armry, armmodel_size);
		d3dg->RenderModel(armmodel, id_texture, DarkModelFlag, false, NoModel);

		//�����`��
		d3dg->SetWorldTransformHumanWeapon(pos_x, pos_y + 16.0f, pos_z, paramdata.mx/10*-1, paramdata.my/10, paramdata.mz/10*-1, rotation_x + (float)M_PI, armry, paramdata.size);
		d3dg->RenderModel(model, texture, DarkModelFlag, false, NoModel);
	}
}

//! @brief �R���X�g���N�^
weapon::weapon(class ParameterInfo *in_Param, float x, float y, float z, float rx, int id_param, int nbs, bool flag)
{
	Param = in_Param;
	pos_x = x;
	pos_y = y;
	pos_z = z;
	move_x = 0.0f;
	move_y = 0.0f;
	move_z = 0.0f;
	rotation_x = rx;
	EnableFlag = flag;

	id_parameter = id_param;
	usingflag = false;
	bullets = nbs;
	Loadbullets = 0;
	motionflag = true;

	if( Param != NULL ){
		WeaponParameter ParamData;
		if( Param->GetWeapon(id_param, &ParamData) == 0 ){
			model_size = ParamData.size;
			//id_model = ParamData.id_model;
			//id_texture = ParamData.id_texture;
		}
	}
	id_model = -1;
	id_texture = -1;
}

//! @brief �f�B�X�g���N�^
weapon::~weapon()
{}

//! @brief ���W�Ɗp�x��ݒ�
//! @param x X���W
//! @param y Y���W
//! @param z Z���W
//! @param rx ������]
void weapon::SetPosData(float x, float y, float z, float rx)
{
	pos_x = x;
	pos_y = y;
	pos_z = z;
	move_x = 0.0f;
	move_y = 0.0f;
	move_z = 0.0f;
	rotation_x = rx;
}

//! @brief �ݒ�l��ݒ�
//! @param id_param ����̎�ޔԍ�
//! @param lnbs ���e��
//! @param nbs ���v�e��
//! @param init �I�u�W�F�N�g��������
void weapon::SetParamData(int id_param, int lnbs, int nbs, bool init)
{
	id_parameter = id_param;
	bullets = nbs;
	Loadbullets = lnbs;

	if( init == true ){
		usingflag = false;
		motionflag = true;
	}
}

//! @brief �ݒ�l���擾
//! @param id_param ����̎�ޔԍ����󂯎��|�C���^�iNULL�j
//! @param lnbs ���e�����󂯎��|�C���^�iNULL�j
//! @param nbs ���v�e�����󂯎��|�C���^�iNULL�j
void weapon::GetParamData(int *id_param, int *lnbs, int *nbs)
{
	if( id_param != NULL ){ *id_param = id_parameter; }
	if( lnbs != NULL ){ *lnbs = Loadbullets; }
	if( nbs != NULL ){ *nbs = bullets; }
}

//! @brief ����̎g�p�󋵂̎擾
//! @return �g�p���Ftrue�@���g�p�Ffalse
bool weapon::GetUsingFlag()
{
	return usingflag;
}

//! @brief ������E��
//! @return �����F0�@���s�F1
int weapon::Pickup()
{
	if( usingflag == true ){ return 1; }
	usingflag = true;
	return 0;
}

//! @brief ������̂Ă�
//! @param x X���W
//! @param y Y���W
//! @param z Z���W
//! @param rx ������]
//! @param speed �̂Ă鏉��
void weapon::Dropoff(float x, float y, float z, float rx, float speed)
{
	//�`�悷����W�Ɗp�x��ݒ�
	move_x = cosf(rx*-1 + (float)M_PI/2) * speed;
	move_y = 0.0f;
	move_z = sinf(rx*-1 + (float)M_PI/2) * speed;
	pos_x = x + cosf(rx*-1 + (float)M_PI/2) * 5.0f;
	pos_y = y + 16.0f + move_y;
	pos_z = z + sinf(rx*-1 + (float)M_PI/2) * 5.0f;
	rotation_x = rx + (float)M_PI;

	//���g�p�i�������j�ɐݒ肵�A���W�ړ���L����
	usingflag = false;
	motionflag = true;
}

//! @brief ���C
//! @return �����F0�@���s�F1
//! @attention �A�ˊԊu���l������܂��B
//! @attention �֐������s����̂́A�����ꂩ�̏����ł��B�@�u�A�ˊԊu�ɖ����Ȃ��v�u�����[�h���s���v�u�e���Ȃ��v�u���˒e�����Ȃ��v�u�����ȕ���̎�ނ��ݒ肳��Ă���v
int weapon::Shot()
{
	//�N���X���ݒ肳��Ă��Ȃ���Ύ��s
	if( Param == NULL ){ return 1; }

	//�e��������Ύ��s
	if( Loadbullets == 0 ){ return 1; }

	//�ݒ�l���擾
	WeaponParameter ParamData;
	int param_pellet;
	if( Param->GetWeapon(id_parameter, &ParamData) == 1 ){ return 1; }
	param_pellet = ParamData.pellet;

	//���˂���e�̐��i�y���b�g���j��0�����Ȃ玸�s
	if( param_pellet <= 0 ){ return 1; }

	//���킪��֒e�Ȃ�΁`
	if( id_parameter == ID_WEAPON_GRENADE ){
		//�e�����炷
		bullets -= 1;
		Loadbullets -= 1;

		if( (bullets - Loadbullets) <= 0 ){		//�i�����[�h���Ă��Ȃ��j�e�������Ȃ�΁A���킲�Ə��ł�����B
			EnableFlag = false;
			usingflag = false;
		}
		else if( Loadbullets <= 0 ){			//���������[�h
			Loadbullets = 1;
		}
		return 0;
	}

	//�e�����炷
	Loadbullets -= 1;
	bullets -= 1;
	return 0;
}

//! @brief �����[�h���J�n
//! @return �����F0�@���s�F1
//! @attention �����[�h���Ԃ��l������܂��B
//! @attention �֐������s����̂́A�����ꂩ�̏����ł��B�@�u�e���Ȃ��v�u�����ȕ���̎�ނ��ݒ肳��Ă���v
int weapon::StartReload()
{
	//�N���X���ݒ肳��Ă��Ȃ���Ύ��s
	if( Param == NULL ){ return 1; }

	//�e��������Ύ��s
	if( (bullets - Loadbullets) == 0 ){ return 1; }
	return 0;
}

//! @brief �����[�h�����s
//! @attention StartReload()�֐��ƈقȂ�A�u���ɒe���[���܂��B�����[�h���Ԃ͍l������܂���B
int weapon::RunReload()
{
	//�N���X���ݒ肳��Ă��Ȃ���Ύ��s
	if( Param == NULL ){ return 1; }

	//�e��������Ύ��s
	if( (bullets - Loadbullets) == 0 ){ return 1; }

	//����̐��\�l����A���U����e�����擾
	WeaponParameter ParamData;
	int nbsmax = 0;
	if( Param->GetWeapon(id_parameter, &ParamData) == 0 ){
		nbsmax = ParamData.nbsmax;
	}

	if( (bullets - Loadbullets) < nbsmax ){	//�c��̒e����葕�U����e�����������
		bullets = (bullets - Loadbullets);
		Loadbullets = bullets;
	}
	else{									//�c��̒e���̕����������
		bullets -= Loadbullets;
		Loadbullets = nbsmax;
	}

	return 0;
}

//! @brief ����̎�ށE���e���̕ύX
//! @param Resource ResourceManager�̃|�C���^
//! @param id_param ��ޔԍ�
//! @param lnbs ���e��
//! @param nbs ���v�e��
//! @return �����Ftrue�@���s�Ffalse
//! @attention �v���C���[�ɂ�闠�Z�iF6�EF7�j�p�ɗp�ӂ��ꂽ�֐��ł��B��֒e���I�����ꂽ�ꍇ�A�����I�ɒe���[���܂��B
//! @attention �g�p����Ă��Ȃ�����I�u�W�F�N�g�ɑ΂��Ď��s����ƁA���̊֐��͎��s���܂��B
bool weapon::ResetWeaponParam(class ResourceManager *Resource, int id_param, int lnbs, int nbs)
{
	//����������Ă��Ȃ���΁A���s
	if( EnableFlag == false ){ return false; }
	if( Resource == NULL ){ return false; }

	//�w�肳�ꂽ�ݒ�l�֏㏑��
	id_parameter = id_param;
	bullets = nbs;
	Loadbullets = lnbs;

	//������֒e�Ȃ�΁A���������[�h
	if( id_param == ID_WEAPON_GRENADE ){
		if( (bullets > 0)&&(Loadbullets == 0) ){
			Loadbullets = 1;
		}
	}

	//���f���ƃe�N�X�`����ݒ�
	Resource->GetWeaponModelTexture(id_param, &id_model, &id_texture);
	WeaponParameter param;
	if( Param->GetWeapon(id_param, &param) == 0 ){
		model_size = param.size;
	}

	return true;
}

//! @brief �v�Z�����s�i���R�����j
//! @param CollD Collision�̃|�C���^
int weapon::ProcessObject(class Collision *CollD)
{
	//�N���X���ݒ肳��Ă��Ȃ���Ύ��s
	if( CollD == NULL ){ return 0; }

	//����������Ă��Ȃ���΁A���s
	if( EnableFlag == false ){ return 0; }

	//�N�ɂ��g���Ă��炸�A�ړ��t���O���L���Ȃ�΁`
	if( (usingflag == false)&&(motionflag == true) ){
		float Dist;
		float maxDist;

		//�ړ����x���X�V
		move_x *= 0.96f;
		move_z *= 0.96f;
		if( move_y > -1.8f ){
			move_y -= 0.3f;
		}

		//�u���b�N�ɖ��܂��Ă���Ώ������Ȃ�
		if( CollD->CheckALLBlockInside(pos_x, pos_y, pos_z) == true ){
			motionflag = false;
			return 0;
		}

		//�����̈ړ����x�����߂�
		maxDist = sqrtf(move_x*move_x + move_z*move_z);
		if( maxDist < 0.1f ){
			maxDist = 0.0f;
			move_x = 0.0f;
			move_z = 0.0f;
		}

		//�u���b�N�ɐڂ��Ă���΁A����ȏ�͐����ړ����Ȃ��B
		if( maxDist > 0.0f ){
			if( CollD->CheckALLBlockIntersectDummyRay(pos_x, pos_y, pos_z, move_x/maxDist, 0, move_z/maxDist, NULL, NULL, &Dist, maxDist) == true ){
				//�u���b�N�ɖ��܂�Ȃ��悤�Ɏ�O��
				Dist -= 0.1f;

				//�ڂ��Ă�����W�܂ňړ�
				pos_x += move_x/maxDist * Dist;
				pos_z += move_z/maxDist * Dist;

				//�ړ��ʂ� 0 ��
				move_x = 0.0f;
				move_z = 0.0f;
			}
		}

		//��������
		for(int i=0; i<2; i++){
			//�u���b�N�ɖ��܂��Ă���΁A�����ŏ����I��
			if( CollD->CheckALLBlockInside(pos_x, pos_y + move_y*0.5f, pos_z) == true ){
				motionflag = false;
				return 0;
			}

			//����������
			pos_y += move_y*0.5f;
		}

		//���������̍��W�𔽉f
		pos_x += move_x;
		pos_z += move_z;
	}

	return 0;
}

//! @brief �`��
//! @param d3dg D3DGraphics�̃|�C���^
//! @param NoModel ���f���`�斳����
void weapon::Render(class D3DGraphics *d3dg, bool NoModel)
{
	//�N���X���ݒ肳��Ă��Ȃ���Ύ��s
	if( d3dg == NULL ){ return; }

	//����������ĂȂ����A�N���Ɏg���Ă���Ώ������Ȃ�
	if( EnableFlag == false ){ return; }
	if( usingflag == true ){ return; }

	//�����`��
	d3dg->SetWorldTransform(pos_x, pos_y, pos_z, rotation_x, 0.0f, (float)M_PI/2, model_size);
	d3dg->RenderModel(id_model, id_texture, DarkModelFlag, false, NoModel);
}

//! @brief �R���X�g���N�^
smallobject::smallobject(class ParameterInfo *in_Param, class MIFInterface *in_MIFdata, float x, float y, float z, float rx, int id_param, signed short int p4, bool flag)
{
	Param = in_Param;
	MIFdata = in_MIFdata;
	pos_x = x;
	pos_y = y;
	pos_z = z;
	rotation_x = rx;
	rotation_y = 0.0f;
	EnableFlag = flag;
	model_size = 5.0f;

	id_parameter = id_param;
	point_p4 = p4;

	hp = 0;		//�b��
	jump_rx = 0.0f;
	move_rx = 0.0f;
	add_rx = 0.0f;
	add_ry = 0.0f;
	jump_cnt = 0;

	if( Param != NULL ){
		SmallObjectParameter ParamData;
		if( Param->GetSmallObject(id_param, &ParamData) == 0 ){
			hp = ParamData.hp;
		}
	}
	id_model = -1;
	id_texture = -1;
}

//! @brief �f�B�X�g���N�^
smallobject::~smallobject()
{}

//! @brief MIF�f�[�^���Ǘ�����N���X��ݒ�
//! @param in_MIFdata MIFInterface�N���X�̃|�C���^
void smallobject::SetMIFInterfaceClass(class MIFInterface *in_MIFdata)
{
	MIFdata = in_MIFdata;
}

//! @brief �ݒ�l��ݒ�
//! @param id_param �����̎�ޔԍ�
//! @param p4 ��4�p�����[�^
//! @param init �I�u�W�F�N�g��������
void smallobject::SetParamData(int id_param, signed short int p4, bool init)
{
	rotation_y = 0.0f;
	id_parameter = id_param;
	point_p4 = p4;

	if( init == true ){
		hp = 0;		//�b��
		jump_rx = 0.0f;
		move_rx = 0.0f;
		add_rx = 0.0f;
		add_ry = 0.0f;
		jump_cnt = 0;

		if( (TOTAL_PARAMETERINFO_SMALLOBJECT <= id_param)&&(id_param <= (TOTAL_PARAMETERINFO_SMALLOBJECT + MAX_ADDSMALLOBJECT -1)) ){
			if( MIFdata != NULL ){
				hp = MIFdata->GetAddSmallobjectHP(id_param - TOTAL_PARAMETERINFO_SMALLOBJECT);
			}
		}
		else{
			if( Param != NULL ){
				SmallObjectParameter ParamData;
				if( Param->GetSmallObject(id_param, &ParamData) == 0 ){
					hp = ParamData.hp;
				}
			}
		}
	}
}

//! @brief �ݒ�l���擾
//! @param id_param �����̎�ޔԍ����󂯎��|�C���^�iNULL�j
//! @param p4 ��4�p�����[�^���󂯎��|�C���^�iNULL�j
void smallobject::GetParamData(int *id_param, signed short int *p4)
{
	if( id_param != NULL ){ *id_param = id_parameter; }
	if( p4 != NULL ){ *p4 = point_p4; }
}

//! @brief �̗͂��擾
//! @return �̗͒l
int smallobject::GetHP()
{
	return hp;
}

//! @brief �u���b�N�̏�Ɉړ�
//! @param CollD Collision�̃|�C���^
//! @return ���̍��W����̈ړ��ʁi0�ňړ��Ȃ��j
//! @attention �u���b�N�ɖ��܂��Ă���ꍇ�ł��A���̃u���b�N�܂� ���蔲���܂��B
float smallobject::CollisionMap(class Collision *CollD)
{
	//�N���X���ݒ肳��Ă��Ȃ���Ύ��s
	if( CollD == NULL ){ return 0.0f; }

	float Dist;
	SmallObjectParameter ParamData;

	//�������ɓ����蔻��
	if( CollD->CheckALLBlockIntersectRay(pos_x, pos_y + COLLISION_ADDSIZE, pos_z, 0, -1, 0, NULL, NULL, &Dist, 1000.0f) == true ){
		//�����蔻��̑傫�����擾
		if( (TOTAL_PARAMETERINFO_SMALLOBJECT <= id_parameter)&&(id_parameter <= (TOTAL_PARAMETERINFO_SMALLOBJECT + MAX_ADDSMALLOBJECT -1)) ){
			Dist -= (float)MIFdata->GetAddSmallobjectDecide(id_parameter - TOTAL_PARAMETERINFO_SMALLOBJECT)/10.0f;
		}
		else{
			if( Param->GetSmallObject(id_parameter, &ParamData) == 0 ){
				Dist -= (float)ParamData.decide/10.0f;
			}
		}

		//���W���ړ�����
		pos_y -= Dist;
		return Dist;
	}

	return 0.0f;
}

//! @brief �e���q�b�g����
//! @param attacks �e�̍U����
void smallobject::HitBullet(int attacks)
{
	hp -= attacks;
	if( hp <= 0 ){
		Destruction();
	}
}

//! @brief ��֒e�̔������q�b�g����
//! @param attacks �����̍U����
//! @attention �����ɂ��v�Z�����O�ɍς܂��Ă��������B
void smallobject::HitGrenadeExplosion(int attacks)
{
	hp -= attacks;
	if( hp <= 0 ){
		Destruction();
	}
}

//! @brief ������j�󂷂�
//! @attention �ʏ�� HitBullet()�֐� ����� GrenadeExplosion()�֐� ���玩���I�Ɏ��s����邽�߁A���ڌĂяo���K�v�͂���܂���B
void smallobject::Destruction()
{
	//EnableFlag = false;
	//return;

	int jump;

	//��ы���擾
	if( (TOTAL_PARAMETERINFO_SMALLOBJECT <= id_parameter)&&(id_parameter <= (TOTAL_PARAMETERINFO_SMALLOBJECT + MAX_ADDSMALLOBJECT -1)) ){
		jump = MIFdata->GetAddSmallobjectJump(id_parameter - TOTAL_PARAMETERINFO_SMALLOBJECT);
	}
	else{
		SmallObjectParameter paramdata;
		Param->GetSmallObject(id_parameter, &paramdata);
		jump = paramdata.jump;
	}

	//��΂�
	hp = 0;
	jump_cnt = jump;

	//�p���ݒ�
	jump_rx = DegreeToRadian(10) * GetRand(36);
	move_rx = (float)jump * 0.04243f;
	add_rx = DegreeToRadian(1) * GetRand(20);
	add_ry = DegreeToRadian(1) * GetRand(20);
}

//! @brief �v�Z�����s�i�j�󎞂̈ړ��Ȃǁj
int smallobject::ProcessObject()
{
	//�`�悳��Ă��Ȃ����A�̗͂��c���Ă��Ȃ���Ώ������Ȃ��B
	if( EnableFlag == false ){ return 0; }
	if( hp > 0 ){ return 0; }

	int cnt;
	int jump;

	//��ы���擾
	if( (TOTAL_PARAMETERINFO_SMALLOBJECT <= id_parameter)&&(id_parameter <= (TOTAL_PARAMETERINFO_SMALLOBJECT + MAX_ADDSMALLOBJECT -1)) ){
		jump = MIFdata->GetAddSmallobjectJump(id_parameter - TOTAL_PARAMETERINFO_SMALLOBJECT);
	}
	else{
		SmallObjectParameter paramdata;
		Param->GetSmallObject(id_parameter, &paramdata);
		jump = paramdata.jump;
	}
	
	//�������ł���J�E���g�����v�Z
	cnt = jump - jump_cnt;

	//�p��������W�E�p�x���v�Z
	pos_x += cosf(jump_rx) * move_rx;
	pos_y += jump_cnt * 0.1f;
	pos_z += sinf(jump_rx) * move_rx;
	rotation_x += add_rx;
	rotation_y += add_ry;

	jump_cnt -= 1;

	//1�b���ł�����`��I��
	if( cnt > (int)GAMEFPS ){
		EnableFlag = false;
		return 2;
	}

	return 1;
}

//! @brief �`��
//! @param d3dg D3DGraphics�̃|�C���^
//! @param NoModel ���f���`�斳����
void smallobject::Render(D3DGraphics *d3dg, bool NoModel)
{
	//�N���X���ݒ肳��Ă��Ȃ���Ύ��s
	if( d3dg == NULL ){ return; }

	//����������Ă��Ȃ���Ώ������Ȃ��B
	if( EnableFlag == false ){ return; }

	//�`��
	d3dg->SetWorldTransform(pos_x, pos_y, pos_z, rotation_x, rotation_y, model_size);
	d3dg->RenderModel(id_model, id_texture, DarkModelFlag, false, NoModel);
}

//! @brief �R���X�g���N�^
bullet::bullet(int modelid, int textureid)
{
	model_size = 1.0f;
	id_model = modelid;
	id_texture = textureid;
	EnableFlag = false;

	rotation_y = 0.0f;
	attacks = 0;
	penetration = 0;
	speed = 0.0f;
	teamid = 0;
	humanid = 0;
	ontargetcnt = 0.0f;
	cnt = 0;
}

//! @brief �f�B�X�g���N�^
bullet::~bullet()
{}

//! @brief ���W�Ɗp�x��ݒ�
//! @param x X���W
//! @param y Y���W
//! @param z Z���W
//! @param rx ������]
//! @param ry �c����]
void bullet::SetPosData(float x, float y, float z, float rx, float ry)
{
	pos_x = x;
	pos_y = y;
	pos_z = z;
	rotation_x = rx;
	rotation_y = ry;
}

//! @brief �ݒ�l��ݒ�
//! @param _attacks �U����
//! @param _penetration �ђʗ�
//! @param _speed �e��
//! @param _teamid �`�[���ԍ�
//! @param _humanid �l�̃f�[�^�ԍ�
//! @param _ontargetcnt �������̃J�E���g��
//! @param init �I�u�W�F�N�g��������
void bullet::SetParamData(int _attacks, int _penetration, float _speed, int _teamid, int _humanid, float _ontargetcnt, bool init)
{
	attacks = _attacks;
	penetration = _penetration;
	speed = _speed;
	teamid = _teamid;
	humanid = _humanid;
	ontargetcnt = _ontargetcnt;

	if( init == true ){
		cnt = 0;
	}
}

//! @brief ���W�Ɗp�x���擾
//! @param x X���W���󂯎��|�C���^�iNULL�j
//! @param y Y���W���󂯎��|�C���^�iNULL�j
//! @param z Z���W���󂯎��|�C���^�iNULL�j
//! @param rx ������]���󂯎��|�C���^�iNULL�j
//! @param ry �c����]���󂯎��|�C���^�iNULL�j
void bullet::GetPosData(float *x, float *y, float *z, float *rx, float *ry)
{
	if( x != NULL ){ *x = pos_x; }
	if( y != NULL ){ *y = pos_y; }
	if( z != NULL ){ *z = pos_z; }
	if( rx != NULL ){ *rx = rotation_x; }
	if( ry != NULL ){ *ry = rotation_y; }
}

//! @brief �ݒ�l���擾
//! @param _attacks �U���͂��󂯎��|�C���^�iNULL�j
//! @param _penetration �ђʗ͂��󂯎��|�C���^�iNULL�j
//! @param _speed �e�����󂯎��|�C���^�iNULL�j
//! @param _teamid �`�[���ԍ����󂯎��|�C���^�iNULL�j
//! @param _humanid �l�̃f�[�^�ԍ����󂯎��|�C���^�iNULL�j
//! @param _ontargetcnt �������̃J�E���g�����󂯎��|�C���^�iNULL�j
void bullet::GetParamData(int *_attacks, int *_penetration, float *_speed, int *_teamid, int *_humanid, float *_ontargetcnt)
{
	if( _attacks != NULL ){ *_attacks = attacks; }
	if( _penetration != NULL ){ *_penetration = penetration; }
	if( _speed != NULL ){ *_speed = speed; }
	if( _teamid != NULL ){ *_teamid = teamid; }
	if( _humanid != NULL ){ *_humanid = humanid; }
	if( _ontargetcnt != NULL ){ *_ontargetcnt = ontargetcnt; }
}

//! @brief �v�Z�����s�i�e�̐i�s�E���ԏ��Łj
int bullet::ProcessObject()
{
	//����������Ă��Ȃ���Ώ������Ȃ�
	if( EnableFlag == false ){ return 0; }

	//���Ŏ��Ԃ��߂��Ă���΁A�I�u�W�F�N�g�𖳌���
	if( cnt > BULLET_DESTROYFRAME ){
		EnableFlag = false;
		return 0;
	}

	//�ړ�����
	pos_x += cosf(rotation_x)*cosf(rotation_y)*speed;
	pos_y += sinf(rotation_y)*speed;
	pos_z += sinf(rotation_x)*cosf(rotation_y)*speed;
	cnt += 1;

	return 0;
}

//! @brief �`��
//! @param d3dg D3DGraphics�̃|�C���^
//! @param NoModel ���f���`�斳����
void bullet::Render(class D3DGraphics *d3dg, bool NoModel)
{
	//�N���X���ݒ肳��Ă��Ȃ���Ύ��s
	if( d3dg == NULL ){ return; }

	//����������Ă��Ȃ���Ώ������Ȃ��B
	if( EnableFlag == false ){ return; }

	//�e���ړ��O��������`�悵�Ȃ�
	//�@�e��������˂������Č�����΍�
	if( cnt == 0 ){ return; }

	//�`��
	d3dg->SetWorldTransform(pos_x, pos_y, pos_z, (rotation_x * -1 - (float)M_PI/2), rotation_y, model_size);
	d3dg->RenderModel(id_model, id_texture, false, false, NoModel);
}

//! @brief �R���X�g���N�^
grenade::grenade(int modelid, int textureid) : bullet(modelid, textureid)
{
	if( Param != NULL ){
		WeaponParameter ParamData;
		if( Param->GetWeapon(ID_WEAPON_GRENADE, &ParamData) == 0 ){
			model_size = ParamData.size;
		}
	}

	move_x = 0.0f;
	move_y = 0.0f;
	move_z = 0.0f;
}

//! @brief �f�B�X�g���N�^
grenade::~grenade()
{}

//! @brief ���W�Ə���ݒ�
//! @param speed ����
//! @param _teamid �`�[���ԍ�
//! @param _humanid �l�̃f�[�^�ԍ�
//! @param _ontargetcnt �������̃J�E���g��
//! @param init �I�u�W�F�N�g��������
//! @attention ��� SetPosData() �����s���Ă��������B
void grenade::SetParamData(float speed, int _teamid, int _humanid, float _ontargetcnt, bool init)
{
	move_x = cosf(rotation_x) * cosf(rotation_y) * speed;
	move_y = sinf(rotation_y) * speed;
	move_z = sinf(rotation_x) * cosf(rotation_y) * speed;
	teamid = _teamid;
	humanid = _humanid;
	ontargetcnt = _ontargetcnt;

	if( init == true ){
		cnt = 0;
	}
}

//! @brief �ݒ�l���擾
//! @param _speed ���x���󂯎��|�C���^�iNULL�j
//! @param _teamid �`�[���ԍ����󂯎��|�C���^�iNULL�j
//! @param _humanid �l�̃f�[�^�ԍ����󂯎��|�C���^�iNULL�j
//! @param _ontargetcnt �������̃J�E���g�����󂯎��|�C���^�iNULL�j
void grenade::GetParamData(float *_speed, int *_teamid, int *_humanid, float *_ontargetcnt)
{
	if( _speed != NULL ){ *_speed = GetSpeed(); }
	if( _teamid != NULL ){ *_teamid = teamid; }
	if( _humanid != NULL ){ *_humanid = humanid; }
	if( _ontargetcnt != NULL ){ *_ontargetcnt = ontargetcnt; }
}

//! @brief ���x���擾
//! @return ���x
float grenade::GetSpeed()
{
	return sqrtf(move_x*move_x + move_y*move_y + move_z*move_z);
}

//! @brief �v�Z�����s�i��֒e�̐i�s�E�����j
//! @return �����F2�@�o�E���h�E���˕Ԃ�F1�@����ȊO�F0
int grenade::ProcessObject(class Collision *CollD)
{
	if( CollD == NULL ){ return 0; }

	//����������Ă��Ȃ���Ώ������Ȃ�
	if( EnableFlag == false ){ return 0; }

	//���Ԃ��߂��Ă���΁A�I�u�W�F�N�g�𖳌������A�u�����v�Ƃ��ĕԂ��B
	if( cnt > GRENADE_DESTROYFRAME ){
		EnableFlag = false;
		return 2;
	}

	//�Î~���Ă���Ώ������Ȃ�
	if( (move_x == 0.0f)&&(move_y == 0.0f)&&(move_z == 0.0f) ){
		cnt += 1;
		return 0;
	}

	int id, face;
	float Dist;
	float maxDist = sqrtf(move_x*move_x + move_y*move_y + move_z*move_z);

	//�}�b�v�ɑ΂��ē����蔻������s
	if( CollD->CheckALLBlockIntersectRay(pos_x, pos_y, pos_z, move_x/maxDist, move_y/maxDist, move_z/maxDist, &id, &face, &Dist, maxDist) == true ){
		//�Փ˂��Ă���ꍇ�`

		float Angle, Acceleration;
		float vx, vy, vz;

		//�p�x�����߂�
		CollD->AngleVector(id, face, move_x, move_y, move_z, &Angle);

		//���˂���x�N�g�������߂�
		CollD->ReflectVector(id, face, move_x, move_y, move_z, &vx, &vy, &vz);

		//�����������߂�
		Acceleration = Angle*-1 * 0.2546f + 0.7f;

		//���� and ����
		move_x = vx * Acceleration;
		move_y = vy * Acceleration;
		move_z = vz * Acceleration;

		//�@���Փ˂����u�ԁi�t���[���j�́A�ړ����Ȃ��B

		//�ړ����x���v�Z
		move_x *= 0.98f;
		move_y = (move_y - 0.17f) * 0.98f;
		move_z *= 0.98f;

		cnt += 1;
		return 1;
	}

	//���W���ړ�
	pos_x += move_x;
	pos_y += move_y;
	pos_z += move_z;

	//�ړ����x���v�Z
	move_x *= 0.98f;
	move_y = (move_y - 0.17f) * 0.98f;
	move_z *= 0.98f;

	cnt += 1;
	return 0;
}

//! @brief �`��
//! @param d3dg D3DGraphics�̃|�C���^
//! @param NoModel ���f���`�斳����
void grenade::Render(class D3DGraphics *d3dg, bool NoModel)
{
	//�N���X���ݒ肳��Ă��Ȃ���Ύ��s
	if( d3dg == NULL ){ return; }

	//����������Ă��Ȃ���Ώ������Ȃ��B
	if( EnableFlag == false ){ return; }

	//�`��
	d3dg->SetWorldTransform(pos_x, pos_y, pos_z, (rotation_x * -1 - (float)M_PI/2), 0.0f, (float)M_PI/2, model_size);
	d3dg->RenderModel(id_model, id_texture, DarkModelFlag, false, NoModel);
}

//! @brief �R���X�g���N�^
effect::effect(float x, float y, float z, float size, float rotation, int texture, int count)
{
	pos_x = x;
	pos_y = y;
	pos_z = z;
	move_x = 0.0f;
	move_y = 0.0f;
	move_z = 0.0f;
	addmove_y = 0.0f;
	rotation_x = 0.0f;
	rotation_y = 0.0f;
	rotation_texture = rotation;
	addrotation_texture = 0.0f;
	model_size = size;
	addmodel_size = 0.0f;
	id_texture = texture;
	brightness = 0;
	addbrightness = 0;
	alpha = 1.0f;
	add_alpha = 0.0f;
	cnt = count;
	type = EFFECT_NORMAL;
	if( cnt > 0 ){
		EnableFlag = true;
	}
	else{
		EnableFlag = false;
	}
}

//! @brief �f�B�X�g���N�^
effect::~effect()
{}

//! @brief �ݒ�l��ݒ�
//! @param in_move_x X���ړ���
//! @param in_move_y Y���ړ���
//! @param in_move_z Z���ړ���
//! @param in_addmove_y Y���ړ��ʂ̉��Z��
//! @param rotation ��]�p�x
//! @param addrotation ��]�p�x�̉��Z��
//! @param size �`��{��
//! @param addsize �`��{��
//! @param count �J�E���g
//! @param settype �G�t�F�N�g�̎�ށ@�iEffect_Type ��g�ݍ�����j
void effect::SetParamData(float in_move_x, float in_move_y, float in_move_z, float in_addmove_y, float rotation, float addrotation, float size, float addsize, int count, int settype)
{
	move_x = in_move_x;
	move_y = in_move_y;
	move_z = in_move_z;
	addmove_y = in_addmove_y;
	rotation_x = addrotation;
	rotation_y = 0.0f;
	rotation_texture = rotation;
	addrotation_texture = addrotation;
	model_size = size;
	addmodel_size = addsize;
	cnt = count;
	type = settype;
}

//! @brief �e�N�X�`����ݒ�
//! @param id �e�N�X�`���̃f�[�^�ԍ�
//! @param in_brightness ���邳
//! @param in_addbrightness ���邳�̉��Z��
//! @param in_alpha �����x
//! @param in_addalpha �����x�̉��Z��
void effect::SetTexture(int id, int in_brightness, int in_addbrightness, float in_alpha, float in_addalpha)
{
	id_texture = id;
	brightness = in_brightness;
	addbrightness = in_addbrightness;
	alpha = in_alpha;
	add_alpha = in_addalpha;
}

//! @brief �����Əc���̌�����ݒ�
//! @param rx �ݒ肷�鉡��
//! @param ry �ݒ肷��c��
void effect::SetRxRy(float rx, float ry)
{
	rotation_x = rx;
	rotation_y = ry;
}

//! �e�N�X�`���̃f�[�^�ԍ����擾
//! @return �e�N�X�`���̃f�[�^�ԍ�
int effect::GetTextureID()
{
	return id_texture;
}

//! �G�t�F�N�g�̈ړ��ʂ��擾
//! @param mx X���ړ��ʂ��󂯎��|�C���^
//! @param my Y���ړ��ʂ��󂯎��|�C���^
//! @param mz Z���ړ��ʂ��󂯎��|�C���^
void effect::GetMove(float *mx, float *my, float *mz)
{
	if( (mx == NULL)||(my == NULL)||(mz == NULL) ){ return; }

	*mx = move_x;
	*my = move_y;
	*mz = move_z;
}

//! �G�t�F�N�g�̃}�b�v�����蔻��t���O�擾
//! @return �L���Ftrue�@�����Ffalse
bool effect::GetCollideMapFlag()
{
	if( type&EFFECT_COLLIDEMAP ){
		return true;
	}
	//else{
		return false;
	//}
}

//! @brief �v�Z�����s�i�G�t�F�N�g�ړ��E���ԏ��Łj
//! @return �������s�F1�@�`��ŏI�t���[���F2�@�����Ȃ��F0
int effect::ProcessObject()
{
	//����������Ă��Ȃ���Ώ������Ȃ�
	if( EnableFlag == false ){ return 0; }

	//�J�E���g���I�����Ă���΁A�������Ȃ��悤�ɐݒ�
	if( cnt <= 0 ){
		EnableFlag = false;
		return 2;
	}

	//���W�ړ�
	pos_x += move_x;
	pos_y += move_y;
	pos_z += move_z;
	move_y += addmove_y;

	//��]
	rotation_texture += addrotation_texture;

	//�g��
	model_size += addmodel_size;

	//���邳�Ɠ����x
	brightness += addbrightness;
	alpha += add_alpha;

	//�����x���}�C�i�X�ɂȂ�����A�������Ȃ��悤�ɐݒ�
	if( alpha <= 0.0f ){
		EnableFlag = false;
		cnt = 0;
		return 2;
	}

	//�J�E���g�� 255 �����Ȃ�΁A�J�E���g�� 1 ����
	if( cnt < 255 ){
		cnt -= 1;
	}
	return 1;
}

//! @brief �`��
//! @param d3dg D3DGraphics�̃|�C���^
//! @param camera_rx �J�����̉����p�x
//! @param camera_ry �J�����̏c���p�x
//! @param NoModel ���f���`�斳����
void effect::Render(class D3DGraphics *d3dg, float camera_rx, float camera_ry, bool NoModel)
{
	//�N���X���ݒ肳��Ă��Ȃ���Ύ��s
	if( d3dg == NULL ){ return; }

	//����������Ă��Ȃ���Ώ������Ȃ��B
	if( EnableFlag == false ){ return; }

	if( (type & EFFECT_NOBILLBOARD) == 0 ){
		//�J�������W��K�p���A�r���{�[�h������
		rotation_x = camera_rx*-1;
		rotation_y = camera_ry;
	}

	//�`��
	d3dg->SetWorldTransformEffect(pos_x, pos_y, pos_z, rotation_x, rotation_y, rotation_texture, model_size);
	d3dg->RenderBoard(id_texture, alpha, NoModel);
}

//! @brief �R���X�g���N�^
HumanMotionControl::HumanMotionControl(class ParameterInfo *in_Param)
{
	Param = in_Param;

	id_upmodel = -1;
	for(int i=0; i<TOTAL_ARMMODE; i++){
		id_armmodel[i] = -1;
	}
	id_legmodel = -1;
	for(int i=0; i<TOTAL_WALKMODE; i++){
		id_walkmodel[i] = -1;
	}
	for(int i=0; i<TOTAL_RUNMODE; i++){
		id_runmodel[i] = -1;
	}

	reaction_y = 0.0f;
	slowarm = false;
	armmodel_rotation_y = 0.0f;
	legrotation_x = 0.0f;
	walkcnt = 0;
	runcnt = 0;
	armmodelid = -1;
	legmodelid = -1;
}

//! @brief �f�B�X�g���N�^
HumanMotionControl::~HumanMotionControl()
{}

//! @brief �ݒ�l���Ǘ�����N���X��o�^
//! @attention �e�֐����g�p����O�Ɏ��s���邱�ƁB
void HumanMotionControl::SetParameterInfoClass(class ParameterInfo *in_Param)
{
	Param = in_Param;
}

//! @brief ���f���f�[�^��ݒ�
//! @param upmodel �㔼�g�̃��f��
//! @param armmodel[] �r�̃��f���̔z��i�z�񐔁FTOTAL_ARMMODE�j
//! @param legmodel ���i�Î~��ԁj�̃��f��
//! @param walkmodel[] �r�̃��f���̔z��i�z�񐔁FTOTAL_WALKMODE�j
//! @param runmodel[] �r�̃��f���̔z��i�z�񐔁FTOTAL_RUNMODE�j
void HumanMotionControl::SetModel(int upmodel, int armmodel[], int legmodel, int walkmodel[], int runmodel[])
{
	id_upmodel = upmodel;
	for(int i=0; i<TOTAL_ARMMODE; i++){
		id_armmodel[i] = armmodel[i];
	}
	id_legmodel = legmodel;
	for(int i=0; i<TOTAL_WALKMODE; i++){
		id_walkmodel[i] = walkmodel[i];
	}
	for(int i=0; i<TOTAL_RUNMODE; i++){
		id_runmodel[i] = runmodel[i];
	}
}

//! @brief �������n�֐�
//! @param rx ��]�p�x
void HumanMotionControl::Init(float rx)
{
	reaction_y = 0.0f;
	slowarm = false;
	armmodel_rotation_y = 0.0f;
	legrotation_x = rx;
	walkcnt = 0;
	runcnt = 0;
	armmodelid = -1;
	legmodelid = -1;
}

//! @brief ������E��
//! @param weapon_paramid ����̎�ޔԍ�
void HumanMotionControl::PickupWeapon(int weapon_paramid)
{
	//���g�p�����΍�
	UNREFERENCED_PARAMETER(weapon_paramid);

	//�r�̊p�x�i�����j��ݒ�
	reaction_y = DegreeToRadian(-20);
	slowarm = true;
}

//! @brief �����؂�ւ���i�����ւ���j
//! @param weapon_paramid ����̎�ޔԍ�
void HumanMotionControl::ChangeHaveWeapon(int weapon_paramid)
{
	//���g�p�����΍�
	UNREFERENCED_PARAMETER(weapon_paramid);

	//�r�̊p�x�i�����j��ݒ�
	reaction_y = DegreeToRadian(-20);
	slowarm = true;
}

//! @brief ����̎�ސ؂�ւ�
//! @param weapon_paramid ����̎�ޔԍ�
//! @attention ���̊֐��͊g���p�̃_�~�[�֐��ł��B
void HumanMotionControl::ChangeWeaponID(int weapon_paramid)
{
	//���g�p�����΍�
	UNREFERENCED_PARAMETER(weapon_paramid);
}

//! @brief ���C����
//! @param weapon_paramid ����̎�ޔԍ�
void HumanMotionControl::ShotWeapon(int weapon_paramid)
{
	//�r�ɔ�����`����
	if( weapon_paramid == ID_WEAPON_GRENADE ){
		reaction_y = DegreeToRadian(20);
	}
	else{
		//����̐ݒ�l���擾
		WeaponParameter ParamData;
		ScopeParameter sparam;
		if( Param->GetWeapon(weapon_paramid, &ParamData) != 0 ){ return; }
		Param->GetScopeParam(ParamData.scopemode, &sparam);

		if( sparam.WeaponMotionReactionY >= 0.0f ){
			reaction_y = sparam.WeaponMotionReactionY;
		}
		else{
			reaction_y = DegreeToRadian(0.5f) * ParamData.reaction;
		}
	}
	slowarm = false;
}

//! @brief �����[�h
//! @param weapon_paramid ����̎�ޔԍ�
//! @attention ���̊֐��͊g���p�̃_�~�[�֐��ł��B
void HumanMotionControl::ReloadWeapon(int weapon_paramid)
{
	//���g�p�����΍�
	UNREFERENCED_PARAMETER(weapon_paramid);
}

//! @brief ������̂Ă�
//! @attention ���̊֐��͊g���p�̃_�~�[�֐��ł��B
void HumanMotionControl::DumpWeapon()
{
	//
}

//! @brief �W�����v
//! @attention ���̊֐��͊g���p�̃_�~�[�֐��ł��B
void HumanMotionControl::Jump()
{
	//
}

//! @brief ���[�V�����v�Z�����s
//! @attention rotation_x �̑S�̂̉�]�p�x
//! @attention armrotation_y �r�̉�]�p�x
//! @attention weapon_paramid ����̎�ޔԍ�
//! @attention ReloadCnt ����̃����[�h�J�E���g
//! @attention ChangeWeaponIDCnt ����ID�̐ؑփJ�E���g
//! @attention MoveFlag �ړ�������\���t���O
//! @attention hp �̗�
//! @attention PlayerFlag �v���C���[���ǂ���
void HumanMotionControl::ProcessObject(float rotation_x, float armrotation_y, int weapon_paramid, int ReloadCnt, int ChangeWeaponIDCnt, int MoveFlag, int hp, bool PlayerFlag)
{
	int ArmModelID;
	float move_rx;

	if( slowarm == false ){
		//���C�Ȃǂɂ�锽��
		if( reaction_y > 0.0f ){
			if( reaction_y > DegreeToRadian(0.01f) ){ reaction_y *= 0.5f; }
			else{ reaction_y = 0.0f; }
		}
		if( reaction_y < 0.0f ){
			if( reaction_y < DegreeToRadian(-0.01f) ){ reaction_y *= 0.5f; }
			else{ reaction_y = 0.0f; }
		}
	}
	else{
		if( (DegreeToRadian(-2.0f) < reaction_y)&&(reaction_y < DegreeToRadian(2.0f)) ){
			reaction_y = 0.0f;
			slowarm = false;
		}
		if( reaction_y > 0.0f ){ reaction_y += DegreeToRadian(-2.0f); }
		if( reaction_y < 0.0f ){ reaction_y += DegreeToRadian(2.0f); }
	}

	//�����[�h���Ȃ�r�̊p�x���Đݒ�
	if( ReloadCnt > 0 ){
		reaction_y = ARMRAD_RELOADWEAPON;
	}

	//����ID�ؑ֒��Ȃ�r�̊p�x���Đݒ�
	if( ChangeWeaponIDCnt > 0 ){
		reaction_y = ARMRAD_RELOADWEAPON;
	}

	if( weapon_paramid == ID_WEAPON_NONE ){	//��Ԃ�
		if( PlayerFlag == true ){
			armmodel_rotation_y = ARMRAD_NOWEAPON;
		}
		else{
			armmodel_rotation_y = armrotation_y;
		}
		ArmModelID = 0;
	}
	else{							//��������������Ă���
		//����̏����擾
		WeaponParameter paramdata;
		Param->GetWeapon(weapon_paramid, &paramdata);

		armmodel_rotation_y = 0.0f;
		ArmModelID = 0;

		//�r�̌`�Ɗp�x������
		if( paramdata.WeaponP == 0 ){
			armmodel_rotation_y = armrotation_y + reaction_y;
			ArmModelID = 1;
		}
		if( paramdata.WeaponP == 1 ){
			armmodel_rotation_y = armrotation_y + reaction_y;
			ArmModelID = 2;
		}
		if( paramdata.WeaponP == 2 ){
			armmodel_rotation_y = ARMRAD_NOWEAPON;
			ArmModelID = 0;
		}
	}

	//�i�s����������
	if( GetFlag(MoveFlag, MOVEFLAG_WALK) ){
		move_rx = DegreeToRadian(0);
	}
	else if( GetFlag(MoveFlag, (MOVEFLAG_FORWARD | MOVEFLAG_BACK | MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) == MOVEFLAG_FORWARD ){
		move_rx = DegreeToRadian(0);
	}
	else if( GetFlag(MoveFlag, (MOVEFLAG_FORWARD | MOVEFLAG_BACK | MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) == MOVEFLAG_BACK ){
		move_rx = DegreeToRadian(180);
	}
	else if( GetFlag(MoveFlag, (MOVEFLAG_FORWARD | MOVEFLAG_BACK | MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) == MOVEFLAG_LEFT ){
		move_rx = DegreeToRadian(90);
	}
	else if( GetFlag(MoveFlag, (MOVEFLAG_FORWARD | MOVEFLAG_BACK | MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) == MOVEFLAG_RIGHT ){
		move_rx = DegreeToRadian(-90);
	}
	else if( GetFlag(MoveFlag, (MOVEFLAG_FORWARD | MOVEFLAG_BACK | MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) == (MOVEFLAG_FORWARD | MOVEFLAG_LEFT) ){
		move_rx = DegreeToRadian(45);
	}
	else if( GetFlag(MoveFlag, (MOVEFLAG_FORWARD | MOVEFLAG_BACK | MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) == (MOVEFLAG_BACK | MOVEFLAG_LEFT) ){
		move_rx = DegreeToRadian(135);
	}
	else if( GetFlag(MoveFlag, (MOVEFLAG_FORWARD | MOVEFLAG_BACK | MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) == (MOVEFLAG_BACK | MOVEFLAG_RIGHT) ){
		move_rx = DegreeToRadian(-135);
	}
	else if( GetFlag(MoveFlag, (MOVEFLAG_FORWARD | MOVEFLAG_BACK | MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) == (MOVEFLAG_FORWARD | MOVEFLAG_RIGHT) ){
		move_rx = DegreeToRadian(-45);
	}
	else{
		move_rx = 0.0f;
	}

	//���̊p�x���Z�o
	if( hp <= 0 ){
		legrotation_x = rotation_x;
	}
	else{
		float move_rx2;

		//���̌��������߂�
		if( fabsf(move_rx) > DegreeToRadian(90)){
			move_rx2 = move_rx + (float)M_PI;
		}
		else{
			move_rx2 = move_rx;
		}
		move_rx2 = AngleNormalization(move_rx2);

		//���X�ɂ��̌�����
		legrotation_x = legrotation_x*0.85f + (rotation_x + move_rx2*-1)*0.15f;		// 3/4 + 1/4
	}

	//�r�̃��f���ݒ�
	armmodelid = id_armmodel[ArmModelID];

	//���̃��f���ݒ�
	legmodelid = id_legmodel;	//�����~�܂�
	if( hp > 0 ){
		if( GetFlag(MoveFlag, MOVEFLAG_WALK) ){
			legmodelid = id_walkmodel[ (walkcnt/3 % TOTAL_WALKMODE) ];	//����
			walkcnt += 1;
			runcnt = 0;
		}
		else{
			if( GetFlag(MoveFlag, (MOVEFLAG_LEFT | MOVEFLAG_RIGHT)) ){
				legmodelid = id_runmodel[ (runcnt/3 % TOTAL_RUNMODE) ];		//���E����
			}
			if( GetFlag(MoveFlag, MOVEFLAG_FORWARD) ){
				legmodelid = id_runmodel[ (runcnt/2 % TOTAL_RUNMODE) ];		//�O����
			}
			if( GetFlag(MoveFlag, MOVEFLAG_BACK) ){
				legmodelid = id_runmodel[ (runcnt/4 % TOTAL_RUNMODE) ];		//��둖��
			}
			walkcnt = 0;
			runcnt += 1;
		}
	}
}

//! @brief ���[�V�����擾
//! @param arm_rotation_y �r�̊p�x���擾����|�C���^
//! @param leg_rotation_x ���̊p�x���擾����|�C���^
//! @param upmodel �㔼�g�̃��f���̃f�[�^�ԍ����擾����|�C���^
//! @param armmodel �r�̃��f���̃f�[�^�ԍ����擾����|�C���^
//! @param legmodel ���̃��f���̃f�[�^�ԍ����擾����|�C���^
void HumanMotionControl::GetRenderMotion(float *arm_rotation_y, float *leg_rotation_x, int *upmodel, int *armmodel, int *legmodel)
{
	if( arm_rotation_y != NULL ){ *arm_rotation_y = armmodel_rotation_y; }
	if( leg_rotation_x != NULL ){ *leg_rotation_x = legrotation_x; }

	if( upmodel != NULL ){ *upmodel = id_upmodel; }
	if( armmodel != NULL ){ *armmodel = armmodelid; }
	if( legmodel != NULL ){ *legmodel = legmodelid; }
}

//! @brief �����E���胂�[�V�����̃J�E���g�l�擾
//! @return �J�E���g�l
int HumanMotionControl::GetMoveMotionCnt()
{
	if( walkcnt > 0 ){ return walkcnt; }
	return runcnt;
}