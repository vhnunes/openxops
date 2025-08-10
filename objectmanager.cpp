//! @file objectmanager.cpp
//! @brief ObjectManager�N���X�̒�`

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

#include "objectmanager.h"

//! @brief �R���X�g���N�^
ObjectManager::ObjectManager()
{
	HumanIndex = new human[MAX_HUMAN];
	WeaponIndex = new weapon[MAX_WEAPON];
	SmallObjectIndex = new smallobject[MAX_SMALLOBJECT];
	BulletIndex = new bullet[MAX_BULLET];
	GrenadeIndex = new grenade[MAX_GRENADE];
	EffectIndex = new effect[MAX_EFFECT];

	framecnt = 0;
	Human_ontarget = new float[MAX_HUMAN];
	Human_kill = new int[MAX_HUMAN];
	Human_headshot = new int[MAX_HUMAN];
	Human_ShotFlag = new bool[MAX_HUMAN];
	Human_YakkyouCnt = new int[MAX_HUMAN];
	BulletObj_HumanIndex = new BulletObjectHumanIndex[MAX_BULLET];
	AddCollisionFlag = false;
	FriendlyFire = new bool[MAX_HUMAN];
	Player_HumanID = 0;
	Human_FrameTextureRefresh = new bool[MAX_HUMAN];

	GameParamInfo = NULL;
	d3dg = NULL;
	Resource = NULL;
	BlockData = NULL;
	PointData = NULL;
	CollD = NULL;
	GameSound = NULL;
	MIFdata = NULL;

	ObjectLog = new ObjectManagerLog;
}

//! @brief �f�B�X�g���N�^
ObjectManager::~ObjectManager()
{
	Cleanup();

	if( HumanIndex != NULL ){ delete [] HumanIndex; }
	if( WeaponIndex != NULL ){ delete [] WeaponIndex; }
	if( SmallObjectIndex != NULL ){ delete [] SmallObjectIndex; }
	if( BulletIndex != NULL ){ delete [] BulletIndex; }
	if( GrenadeIndex != NULL ){ delete [] GrenadeIndex; }
	if( EffectIndex != NULL ){ delete [] EffectIndex; }

	if( Human_ontarget != NULL ){ delete [] Human_ontarget; }
	if( Human_kill != NULL ){ delete [] Human_kill; }
	if( Human_headshot != NULL ){ delete [] Human_headshot; }
	if( Human_ShotFlag != NULL ){ delete [] Human_ShotFlag; }
	if( Human_YakkyouCnt != NULL ){ delete [] Human_YakkyouCnt; }
	if( BulletObj_HumanIndex != NULL ){ delete [] BulletObj_HumanIndex; }
	if( FriendlyFire != NULL ){ delete [] FriendlyFire; }
	if( Human_FrameTextureRefresh != NULL ){ delete [] Human_FrameTextureRefresh; }

	if( ObjectLog != NULL ){ delete ObjectLog; }
}

//! @brief �Q�Ƃ���N���X��ݒ�
//! @param in_GameParamInfo �Q�[���ݒ�f�[�^�Ǘ��N���X
//! @param in_d3dg �`�揈���N���X
//! @param in_Resource ���\�[�X�Ǘ��N���X
//! @param in_BlockData �u���b�N�f�[�^�Ǘ��N���X
//! @param in_PointData �|�C���g�f�[�^�Ǘ��N���X
//! @param in_CollD �����蔻�菈���N���X
//! @param in_GameSound ���ʉ��Đ��N���X
//! @param in_MIFdata MIF�R���g���[���N���X
//! @attention ���̊֐��Őݒ���s��Ȃ��ƁA�N���X���̂��������@�\���܂���B
void ObjectManager::SetClass(ParameterInfo *in_GameParamInfo, D3DGraphics *in_d3dg, ResourceManager *in_Resource, BlockDataInterface *in_BlockData, PointDataInterface *in_PointData, Collision *in_CollD, SoundManager *in_GameSound, MIFInterface *in_MIFdata)
{
	GameParamInfo = in_GameParamInfo;
	d3dg = in_d3dg;
	Resource = in_Resource;
	BlockData = in_BlockData;
	PointData = in_PointData;
	CollD = in_CollD;
	GameSound = in_GameSound;
	MIFdata = in_MIFdata;

	for(int i=0; i<MAX_HUMAN; i++){
		HumanIndex[i].SetParameterInfoClass(GameParamInfo);
	}
	for(int i=0; i<MAX_WEAPON; i++){
		WeaponIndex[i].SetParameterInfoClass(GameParamInfo);
	}
	for(int i=0; i<MAX_SMALLOBJECT; i++){
		SmallObjectIndex[i].SetParameterInfoClass(GameParamInfo);
		SmallObjectIndex[i].SetMIFInterfaceClass(MIFdata);
	}
	for(int i=0; i<MAX_GRENADE; i++){
		GrenadeIndex[i].SetParameterInfoClass(GameParamInfo);
	}

	int bulletmodel, bullettexture;
	BulletParameter data;

	if( Resource->GetBulletModelTexture(0, &bulletmodel, &bullettexture) == 0 ){
		if( GameParamInfo->GetBullet(0, &data) == 0 ){
			for(int i=0; i<MAX_BULLET; i++){
				BulletIndex[i].SetModel(bulletmodel, data.size);
				BulletIndex[i].SetTexture(bullettexture);
			}
		}
	}

	if( Resource->GetBulletModelTexture(1, &bulletmodel, &bullettexture) == 0 ){
		if( GameParamInfo->GetBullet(1, &data) == 0 ){
			for(int i=0; i<MAX_GRENADE; i++){
				GrenadeIndex[i].SetModel(bulletmodel, data.size);
				GrenadeIndex[i].SetTexture(bullettexture);
			}
		}
	}

	ObjectLog->SetClass(d3dg);
}

//! @brief �l�ǉ�
//! @param data �l�̃|�C���g�f�[�^�@�ipointdata�\���́j
//! @param infodata �Q�Ƃ���l���̃|�C���g�f�[�^�@�i�V�j
//! @param RandomFlag �����_���␳��L���ɂ���
//! @return �����F�f�[�^�ԍ��i0�ȏ�j�@���s�F-1
//! @attention �����Ȑl�̎�ޔԍ����w�肳�ꂽ�ꍇ�� �ʏ́F��l�� ���o�ꂵ�܂��B�e�N�X�`���̓}�b�v�e�N�X�`��0�Ԃ��g�p����AHP�� 0 ���w��i�������j����܂��B
int ObjectManager::AddHumanIndex(pointdata data, pointdata infodata, bool RandomFlag)
{
	int GetHumanFlag;
	HumanParameter HumanParam;
	GetHumanFlag = GameParamInfo->GetHuman(infodata.p2, &HumanParam);
	int Humanindexid = -1;
	int Weaponindexid = -1;
	class weapon *Weapon[TOTAL_HAVEWEAPON];
	int lnbslist[TOTAL_HAVEWEAPON];
	int nbslist[TOTAL_HAVEWEAPON];
	for(int i=0; i<TOTAL_HAVEWEAPON; i++){
		Weapon[i] = NULL;
		lnbslist[i] = 0;
		nbslist[i] = 0;
	}

#ifndef ENABLE_BUG_HUMANWEAPON
	if( GetHumanFlag != 0 ){
		return -1;
	}
#endif

	//�l�̃��f���ԍ����擾
	int upmodel[TOTAL_UPMODE];
	int armmodel[TOTAL_ARMMODE];
	int legmodel;
	int walkmodel[TOTAL_WALKMODE];
	int runmodel[TOTAL_RUNMODE];
	Resource->GetHumanModel(upmodel, armmodel, &legmodel, walkmodel, runmodel);

	float px = data.x;
	float py = data.y;
	float pz = data.z;
	float rx = data.r;
	if( RandomFlag == true ){
		px += 0.001f*GetRand(100*2) - 0.1f;
		pz += 0.001f*GetRand(100*2) - 0.1f;
		rx += DegreeToRadian(0.01f)*GetRand(80*2) - DegreeToRadian(0.8f);
	}

	for(int j=0; j<MAX_HUMAN; j++){
		if( HumanIndex[j].GetEnableFlag() == false ){
			//����������
			HumanIndex[j].SetPosData(px, py, pz, rx);
			HumanIndex[j].SetParamData(infodata.p2, data.id, data.p4, infodata.p3, true);
			if( GetHumanFlag == 0 ){
				int id = Resource->GetHumanTexture(infodata.p2);
				if( id == -1 ){
					//�e�N�X�`�����擾�ł��Ȃ���΁A���t���b�V���t���O��L���ɂ��Ė��t���[���擾
					Human_FrameTextureRefresh[j] = true;
				}

				HumanIndex[j].SetTexture(id);
				HumanIndex[j].SetModel(upmodel[ HumanParam.model ], armmodel, legmodel, walkmodel, runmodel);
			}
			else{
#ifdef ENABLE_BUG_HUMANWEAPON
				HumanIndex[j].SetTexture(d3dg->GetMapTextureID(0));
				HumanIndex[j].SetModel(upmodel[0], armmodel, legmodel, walkmodel, runmodel);
#endif
			}
			HumanIndex[j].SetEnableFlag(true);
			Humanindexid = j;
			break;
		}
	}

	//�������ł��Ȃ�������G���[�Ƃ��ĕԂ�
	if( Humanindexid == -1 ){
		return -1;
	}

	//�l�̐ݒ�f�[�^���������ǂ߂Ă���΁`
	if( GetHumanFlag == 0 ){
		//����B�����z����Ƃ��Ēǉ�
		Weaponindexid = AddVisualWeaponIndex(HumanParam.Weapon[0], true);
		if( Weaponindexid != -1 ){
			//��������Δz��ɋL�^
			Weapon[0] = &WeaponIndex[Weaponindexid];
		}
		if( data.p1 == POINT_P1TYPE_HUMAN ){
			//����A�̉��z����Ƃ��Ēǉ�
			Weaponindexid = AddVisualWeaponIndex(HumanParam.Weapon[1], true);
			if( Weaponindexid != -1 ){
				//��������Δz��ɋL�^
				Weapon[1] = &WeaponIndex[Weaponindexid];
			}
		}

		//�l�Ɏ�������
		HumanIndex[Humanindexid].SetWeapon(Weapon, lnbslist, nbslist);
	}

	//���C�t���O��������
	Human_ShotFlag[Humanindexid] = false;
	Human_YakkyouCnt[Humanindexid] = -1;

	//�v���C���[�Ȃ�΁A�ԍ����L�^
	if( ( (data.p1 == POINT_P1TYPE_HUMAN)||(data.p1 == POINT_P1TYPE_HUMAN2) )&&(data.p4 == 0) ){
		Player_HumanID = Humanindexid;
	}

	return Humanindexid;
}

//! @brief �l�ǉ��i�Q�[�����p�j
//! @param px X���W
//! @param py Y���W
//! @param pz Z���W
//! @param rx X������
//! @param paramID ��ޔԍ�
//! @param TeamID �`�[���ԍ�
//! @param WeaponID �����ޔԍ��̔z��i�v�f���FTOTAL_HAVEWEAPON�j
//! @param param4 ��4�p�����[�^�[
//! @param RandomFlag �����_���␳��L���ɂ���
//! @return �����F�f�[�^�ԍ��i0�ȏ�j�@���s�F-1
//! @attention �����Ȑl�̎�ޔԍ����w�肳�ꂽ�ꍇ�� �ʏ́F��l�� ���o�ꂵ�܂��B�e�N�X�`���̓}�b�v�e�N�X�`��0�Ԃ��g�p����AHP�� 0 ���w��i�������j����܂��B
int ObjectManager::AddHumanIndex(float px, float py, float pz, float rx, int paramID, int TeamID, int WeaponID[], signed short int param4, bool RandomFlag)
{
	int GetHumanFlag;
	HumanParameter HumanParam;
	GetHumanFlag = GameParamInfo->GetHuman(paramID, &HumanParam);
	int Humanindexid = -1;
	int Weaponindexid = -1;
	class weapon *Weapon[TOTAL_HAVEWEAPON];
	int lnbslist[TOTAL_HAVEWEAPON];
	int nbslist[TOTAL_HAVEWEAPON];
	for(int i=0; i<TOTAL_HAVEWEAPON; i++){
		Weapon[i] = NULL;
		lnbslist[i] = 0;
		nbslist[i] = 0;
	}

#ifndef ENABLE_BUG_HUMANWEAPON
	if( GetHumanFlag != 0 ){
		return -1;
	}
#endif

	//�l�̃��f���ԍ����擾
	int upmodel[TOTAL_UPMODE];
	int armmodel[TOTAL_ARMMODE];
	int legmodel;
	int walkmodel[TOTAL_WALKMODE];
	int runmodel[TOTAL_RUNMODE];
	Resource->GetHumanModel(upmodel, armmodel, &legmodel, walkmodel, runmodel);

	if( RandomFlag == true ){
		px += 0.001f*GetRand(100*2) - 0.1f;
		pz += 0.001f*GetRand(100*2) - 0.1f;
		rx += DegreeToRadian(0.01f)*GetRand(80*2) - DegreeToRadian(0.8f);
	}

	for(int i=0; i<MAX_HUMAN; i++){
		if( HumanIndex[i].GetEnableFlag() == false ){
			//����������
			HumanIndex[i].SetPosData(px, py, pz, rx);
			HumanIndex[i].SetParamData(paramID, -1, param4, TeamID, true);
			if( GetHumanFlag == 0 ){
				int id = Resource->GetHumanTexture(paramID);
				if( id == -1 ){
					//�e�N�X�`�����擾�ł��Ȃ���΁A���t���b�V���t���O��L���ɂ��Ė��t���[���擾
					Human_FrameTextureRefresh[i] = true;
				}

				HumanIndex[i].SetTexture(id);
				HumanIndex[i].SetModel(upmodel[ HumanParam.model ], armmodel, legmodel, walkmodel, runmodel);
			}
			else{
#ifdef ENABLE_BUG_HUMANWEAPON
				HumanIndex[i].SetTexture(d3dg->GetMapTextureID(0));
				HumanIndex[i].SetModel(upmodel[0], armmodel, legmodel, walkmodel, runmodel);
#endif
			}
			HumanIndex[i].SetEnableFlag(true);
			Humanindexid = i;
			break;
		}
	}

	//�������ł��Ȃ�������G���[�Ƃ��ĕԂ�
	if( Humanindexid == -1 ){
		return -1;
	}

	if( GetHumanFlag == 0 ){
		//���z�����ǉ�
		for(int i=0; i<TOTAL_HAVEWEAPON; i++){
			Weaponindexid = AddVisualWeaponIndex(WeaponID[i], true);
			if( Weaponindexid != -1 ){
				//��������Δz��ɋL�^
				Weapon[i] = &WeaponIndex[Weaponindexid];
			}
		}

		//�l�Ɏ�������
		HumanIndex[Humanindexid].SetWeapon(Weapon, lnbslist, nbslist);
	}

	//���O�֌W�̏���
	int player_teamid;
	HumanIndex[Player_HumanID].GetParamData(NULL, NULL, NULL, &player_teamid);
	ObjectLog->AddHuman(Humanindexid, TeamID, player_teamid);

	return Humanindexid;
}

//! @brief ����ǉ�
//! @param data ����̃|�C���g�f�[�^�@�ipointdata�\���́j
//! @return �����F�f�[�^�ԍ��i0�ȏ�j�@���s�F-1
int ObjectManager::AddWeaponIndex(pointdata data)
{
	//����ԍ��ƒe�������o�^
	int WeaponID = data.p2;
	int nbs = PointParamToInt(data.p3);

	//�����_���ȕ���Ȃ��
	if( data.p1 == POINT_P1TYPE_RAND_WEAPON ){
		WeaponParameter WeaponParam;

		if( GetRand(2) == 0 ){
			if( GameParamInfo->GetWeapon(data.p2, &WeaponParam) == 1 ){ return -1; }
			nbs = (int)WeaponParam.nbsmax * TOTAL_WEAPON_AUTOBULLET;		//�e���㏑��
		}
		else{
			if( GameParamInfo->GetWeapon(data.p3, &WeaponParam) == 1 ){ return -1; }
			WeaponID = data.p3;						//����ԍ��㏑��
			nbs = (int)WeaponParam.nbsmax * TOTAL_WEAPON_AUTOBULLET;		//�e���㏑��
		}
	}

	//NONE�Ȃ�Ύ��s
	if( WeaponID == ID_WEAPON_NONE ){ return -1; }

	//���f���ƃe�N�X�`�����擾
	int model, texture;
	if( Resource->GetWeaponModelTexture(WeaponID, &model, &texture) == 1 ){
		return -1;
	}

	//�ݒ�l���擾
	WeaponParameter WeaponParam;
	if( GameParamInfo->GetWeapon(WeaponID, &WeaponParam) == 1 ){ return -1; }

	for(int i=0; i<MAX_WEAPON; i++){
		if( WeaponIndex[i].GetEnableFlag() == false ){
			//����������
			WeaponIndex[i].SetPosData(data.x, data.y, data.z, data.r);
			WeaponIndex[i].SetParamData(WeaponID, 0, nbs, true);
			WeaponIndex[i].SetModel(model, WeaponParam.size);
			WeaponIndex[i].SetTexture(texture);
			WeaponIndex[i].SetEnableFlag(true);
			WeaponIndex[i].RunReload();
			return i;
		}
	}
	return -1;
}

//! @brief ���z����ǉ�
//! @param WeaponID ����̎�ޔԍ�
//! @param loadbullet �e�����[�h����
//! @return �����F�f�[�^�ԍ��i0�ȏ�j�@���s�F-1
//! @attention �l���ŏ����珊�����Ă��镐����A����f�[�^�i�I�u�W�F�N�g�j�Ƃ��Ēǉ��������邽�߂̊֐��ł��B
//! @attention ����P�̂Ƃ��Ĕz�u����ꍇ�́AAddWeaponIndex()�֐����g�p���Ă��������B
int ObjectManager::AddVisualWeaponIndex(int WeaponID, bool loadbullet)
{
	//NONE�Ȃ�Ύ��s
	if( WeaponID == ID_WEAPON_NONE ){ return -1; }

	//�����擾
	WeaponParameter WeaponParam;
	if( GameParamInfo->GetWeapon(WeaponID, &WeaponParam) == 1 ){ return -1; }

	//���f���ƃe�N�X�`�����擾
	int model, texture;
	if( Resource->GetWeaponModelTexture(WeaponID, &model, &texture) == 1 ){
		return -1;
	}

	for(int i=0; i<MAX_WEAPON; i++){
		if( WeaponIndex[i].GetEnableFlag() == false ){
			//������
			WeaponIndex[i].SetPosData(0.0f, 0.0f, 0.0f, 0.0f);
			if( loadbullet == false ){
				WeaponIndex[i].SetParamData(WeaponID, 0, 0, true);
			}
			else{
				WeaponIndex[i].SetParamData(WeaponID, WeaponParam.nbsmax, WeaponParam.nbsmax * TOTAL_WEAPON_AUTOBULLET, true);
			}
			WeaponIndex[i].SetModel(model, WeaponParam.size);
			WeaponIndex[i].SetTexture(texture);
			WeaponIndex[i].SetEnableFlag(true);
			return i;
		}
	}
	return -1;
}

//! @brief �����ǉ�
//! @param data �����̃|�C���g�f�[�^�@�ipointdata�\���́j
//! @return �����F�f�[�^�ԍ��i0�ȏ�j�@���s�F-1
int ObjectManager::AddSmallObjectIndex(pointdata data)
{
	//���f���ƃe�N�X�`�����擾
	int model, texture;
	if( Resource->GetSmallObjectModelTexture(data.p2, &model, &texture) == 1 ){
		return -1;
	}

	for(int i=0; i<MAX_SMALLOBJECT; i++){
		if( SmallObjectIndex[i].GetEnableFlag() == false ){
			//������
			SmallObjectIndex[i].SetPosData(data.x, data.y, data.z, data.r);
			SmallObjectIndex[i].SetParamData(data.p2, data.p4, true);
			SmallObjectIndex[i].SetModel(model, SMALLOBJECT_SCALE);
			SmallObjectIndex[i].SetTexture(texture);
			SmallObjectIndex[i].SetEnableFlag(true);

			//�ʒu�C���t���O���L���Ȃ�΁A�}�b�v�Ɣ���
			if( data.p3 !=0 ){
				SmallObjectIndex[i].CollisionMap(CollD);
			}
			return i;
		}
	}
	return -1;
}

//! @brief �����ǉ��i�Q�[�����p�j
//! @param px X���W
//! @param py Y���W
//! @param pz Z���W
//! @param rx X������
//! @param paramID ��ޔԍ�
//! @param MapColl �ʒu�C���t���O
//! @return �����F�f�[�^�ԍ��i0�ȏ�j�@���s�F-1
int ObjectManager::AddSmallObjectIndex(float px, float py, float pz, float rx, int paramID, bool MapColl)
{
	//���f���ƃe�N�X�`�����擾
	int model, texture;
	if( Resource->GetSmallObjectModelTexture(paramID, &model, &texture) == 1 ){
		return -1;
	}

	for(int i=0; i<MAX_SMALLOBJECT; i++){
		if( SmallObjectIndex[i].GetEnableFlag() == false ){
			//������
			SmallObjectIndex[i].SetPosData(px, py, pz, rx);
			SmallObjectIndex[i].SetParamData(paramID, 0, true);
			SmallObjectIndex[i].SetModel(model, SMALLOBJECT_SCALE);
			SmallObjectIndex[i].SetTexture(texture);
			SmallObjectIndex[i].SetEnableFlag(true);

			//�ʒu�C���t���O���L���Ȃ�΁A�}�b�v�Ɣ���
			if( MapColl == true ){
				SmallObjectIndex[i].CollisionMap(CollD);
			}

			//���O�֌W�̏���
			ObjectLog->AddSmallObject(i);

			return i;
		}
	}
	return -1;
}

//! @brief �G�t�F�N�g�ǉ�
//! @param pos_x X���W
//! @param pos_y Y���W
//! @param pos_z Z���W
//! @param move_x X���ړ���
//! @param move_y Y���ړ���
//! @param move_z Z���ړ���
//! @param addmove_y Y���ړ��ʂ̉��Z��
//! @param rotation ��]�p�x
//! @param addrotation ��]�p�x�̉��Z��
//! @param size �`��{��
//! @param addsize �`��{���̉��Z��
//! @param texture �e�N�X�`���̃f�[�^�ԍ�
//! @param brightness ���邳
//! @param addbrightness ���邳�̉��Z��
//! @param alpha �����x
//! @param addalpha �����x�̉��Z��
//! @param settype �G�t�F�N�g�̎�ށ@�iEffect_Type ��g�ݍ�����j
//! @param count �`��t���[����
//! @return �����F�f�[�^�ԍ��i0�ȏ�j�@���s�F-1
int ObjectManager::AddEffect(float pos_x, float pos_y, float pos_z, float move_x, float move_y, float move_z, float addmove_y, float rotation, float addrotation, float size, float addsize, int texture, int brightness, int addbrightness, float alpha, float addalpha, int settype, int count)
{
	for(int i=0; i<MAX_EFFECT; i++){
		if( EffectIndex[i].GetEnableFlag() == false ){
			EffectIndex[i].SetPosData(pos_x, pos_y, pos_z, 0.0f);
			EffectIndex[i].SetParamData(move_x, move_y, move_z, addmove_y, rotation, addrotation, size, addsize, count, settype);
			EffectIndex[i].SetTexture(texture, brightness, addbrightness, alpha, addalpha);
			EffectIndex[i].SetEnableFlag(true);
			return i;
		}
	}

	return -1;
}

//! @brief �i�r���{�[�h�łȂ��j�G�t�F�N�g�ǉ�
//! @param id �u���b�N�ԍ�
//! @param face �ʔԍ�
//! @param pos_x X���W
//! @param pos_y Y���W
//! @param pos_z Z���W
//! @param size �`��{��
//! @param rotation ��]�p�x
//! @param texture �e�N�X�`���̃f�[�^�ԍ�
//! @param brightness ���邳
//! @param addbrightness ���邳�̉��Z��
//! @param alpha �����x
//! @param addalpha �����x�̉��Z��
//! @param count �`��t���[����
//! @return �����F�f�[�^�ԍ��i0�ȏ�j�@���s�F-1
int ObjectManager::AddMapEffect(int id, int face, float pos_x, float pos_y, float pos_z, float size, float rotation, int texture, int brightness, int addbrightness, float alpha, float addalpha, int count)
{
	blockdata data;
	float vx, vy, vz;
	float rx, ry;

	//�u���b�N�̖ʂ̏����擾
	BlockData->Getdata(&data, id);
	vx = data.material[face].vx;
	vy = data.material[face].vy;
	vz = data.material[face].vz;

	//�p�x�����߂�
	rx = atan2f(vz, vx)*-1 + (float)M_PI;
	ry = atan2f(vy, sqrtf(vx*vx + vz*vz))*-1;

	//�G�t�F�N�g�쐬
	for(int i=0; i<MAX_EFFECT; i++){
		if( EffectIndex[i].GetEnableFlag() == false ){
			EffectIndex[i].SetPosData(pos_x, pos_y, pos_z, 0.0f);
			EffectIndex[i].SetParamData(0.0f, 0.0f, 0.0f, 0.0f, rotation, 0.0f, size, 0.0f, count, EFFECT_NOBILLBOARD);
			EffectIndex[i].SetRxRy(rx, ry);
			EffectIndex[i].SetTexture(texture, brightness, addbrightness, alpha, addalpha);
			EffectIndex[i].SetEnableFlag(true);
			return i;
		}
	}

	return -1;
}

//! @brief �o��������
//! @param x X���W
//! @param y Y���W
//! @param z Z���W
//! @param damage �_���[�W
//! @param flowing �����̗L��
void ObjectManager::SetHumanBlood(float x, float y, float z, int damage, bool flowing)
{
	int addtype;

	if( flowing == false ){
		addtype = EFFECT_NORMAL;
	}
	else{
		addtype = EFFECT_COLLIDEMAP;
	}

	if( GameConfig.GetBloodFlag() == true ){
		AddEffect(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, DegreeToRadian(10)*GetRand(36), 0.0f, 10.0f, 0.2f, Resource->GetEffectBloodTexture(), 10, 0, 0.5f, -0.04f, addtype, (int)(GAMEFPS * 0.4f));
		if( flowing == true ){
			for(int i=0; i<(damage/10); i++){
				float rx = DegreeToRadian(10)*GetRand(36);
				AddEffect(x + cosf(rx)*1.0f, y + (float)(GetRand(20)-10)/10, z + sinf(rx)*1.0f, cosf(rx)*0.5f, GetRand(5)*0.1f + 0.5f, sinf(rx)*0.5f, -0.09f, DegreeToRadian(10)*GetRand(36), 0.0f, GetRand(40)*0.1f+0.1f, 0.1f, Resource->GetEffectBloodTexture(), 10, 0, 1.0f, 0.0f, addtype, (int)(GAMEFPS * 0.5f));
			}
		}
	}
}

//! @brief �l���m�̓����蔻��
//! @param in_humanA �Ώۂ̐l�I�u�W�F�N�gA
//! @param in_humanB �Ώۂ̐l�I�u�W�F�N�gB
//! @return �������Ă���Ftrue�@�������Ă��Ȃ��Ffalse
//! @warning in_humanA��in_humanB�ŋ�ʂ͂���܂���B
//! @warning in_humanA��in_humanB�ł̑g�ݍ����́A1�t���[���Ԃ�1�x�������s���Ă��������B
//! @attention ���N���X�͎����I��AddPosOrder()��p���āA���݂������������܂��B
bool ObjectManager::CollideHuman(human *in_humanA, human *in_humanB)
{
	if( in_humanA == NULL ){ return false; }
	if( in_humanB == NULL ){ return false; }

	float h1_x, h1_y, h1_z;
	float h2_x, h2_y, h2_z;
	float angle, length;

	//����������Ă��Ȃ����A���S���ē���Δ��肵�Ȃ�
	if( in_humanA->GetEnableFlag() == false ){ return false; }
	if( in_humanB->GetEnableFlag() == false ){ return false; }
	if( in_humanA->GetHP() <= 0 ){ return false; }
	if( in_humanB->GetHP() <= 0 ){ return false; }

	//���݂��̍��W���擾
	in_humanA->GetPosData(&h1_x, &h1_y, &h1_z, NULL);
	in_humanB->GetPosData(&h2_x, &h2_y, &h2_z, NULL);

	//�~���̓����蔻��
	if( CollideCylinder(h1_x, h1_y, h1_z, HUMAN_HUMANCOLLISION_R, HUMAN_HEIGHT-0.5f, h2_x, h2_y, h2_z, HUMAN_HUMANCOLLISION_R, HUMAN_HEIGHT-0.5f, &angle, &length) == true ){
		//�߂荞�񂾕����������o��
		in_humanA->AddPosOrder(angle, 0.0f, length/2);
		in_humanB->AddPosOrder(angle + (float)M_PI, 0.0f, length/2);
		return true;
	}

	return false;
}

//! @brief �e�̓����蔻��Ə���
//! @param in_bullet �Ώۂ̒e�I�u�W�F�N�g
//! @return ���������Ftrue�@�������Ă��Ȃ��Ffalse
//! @attention ������s���Ώۂ́u�}�b�v�v�u�l�i���E�㔼�g�E�����g�j�v�u�����v�ł��B
//! @attention ����Ɍ��炸�A�_���[�W�v�Z����ʉ��Đ��܂ň�т��čs���܂��B
bool ObjectManager::CollideBullet(bullet *in_bullet)
{
	if( in_bullet == NULL ){ return false; }

	//�g�p����Ă��Ȃ��e�ۂȂ�΁A���������ɕԂ��B
	if( in_bullet->GetEnableFlag() == false ){ return false; }

	int objectID;
	float bx, by, bz;
	float brx, bry;
	int attacks;
	int penetration;
	float speed;
	int teamid;
	int humanid;
	float ontargetcnt;
	float vx, vy, vz;
	bool CollideFlag = false;
	int mapflag = 0;		//0�F�u���b�N�Ȃ��A1�F�u���b�N���薢����A2�F�u���b�N���蔻��ς�

	float Dist;

	//�e�I�u�W�F�N�g�̃f�[�^�ԍ����擾
	objectID = GetBulletObjectID(in_bullet);

	//�e�ۂ̍��W���擾���A�x�N�g�����Z�o�B
	in_bullet->GetPosData(&bx, &by, &bz, &brx, &bry);
	in_bullet->GetParamData(&attacks, &penetration, &speed, &teamid, &humanid, &ontargetcnt);
	vx = cosf(brx)*cosf(bry);
	vy = sinf(bry);
	vz = sinf(brx)*cosf(bry);

	//�}�b�v�Ƃ̓����蔻��i�e����Ƀu���b�N�����邩�j
	if( CollD->CheckALLBlockIntersectRay(bx, by, bz, vx, vy, vz, NULL, NULL, &Dist, speed) == true ){
		mapflag = 1;
	}

	// BULLET_SPEEDSCALE�萔 �̕���\���Z�o
	int maxcnt = (int)roundf(speed / BULLET_SPEEDSCALE);

	// BULLET_SPEEDSCALE�萔 �̕���\�œ����蔻��
	for(int cnt=0; cnt<maxcnt; cnt++){
		//�ђʗ͂��c���Ă��Ȃ���Βe�͖����ɂ���i���� 1��ځj
		if( penetration < 0 ){
			in_bullet->SetEnableFlag(false);
			mapflag = 0;
			break;
		}

		float bvx, bvy, bvz;
		bvx = bx + vx * BULLET_SPEEDSCALE * (cnt+1);
		bvy = by + vy * BULLET_SPEEDSCALE * (cnt+1);
		bvz = bz + vz * BULLET_SPEEDSCALE * (cnt+1);

		//�l�Ƃ̓����蔻��
		for(int i=0; i<MAX_HUMAN; i++){
			//���̐l���g�����C�����e�Ȃ珈�����Ȃ��B
			if( i == humanid ){ continue; }

			//�g�p����Ă��Ȃ����A���̂Ȃ�Ώ������Ȃ��B
			if( HumanIndex[i].GetEnableFlag() == false ){ continue; }
			//if( HumanIndex[i].GetDeadFlag() == true ){ continue; }
			if( HumanIndex[i].GetHP() <= 0 ){ continue; }

			//���ɓ��������l�Ȃ�A�������Ȃ��B
			if( BulletObj_HumanIndex[objectID].GetIndexFlag(i) == true ){ continue; }

			//���W���擾
			float ox, oy, oz;
			int h_teamid;
			HumanIndex[i].GetPosData(&ox, &oy, &oz, NULL);
			HumanIndex[i].GetParamData(NULL, NULL, NULL, &h_teamid);

			if( FriendlyFire[i] == false ){
				//�����`�[���ԍ��i�����j�Ȃ珈�����Ȃ�
				if( h_teamid == teamid ){ continue; }
			}

			//���̓����蔻��
			if( CollideCylinderInside(ox, oy + HUMAN_BULLETCOLLISION_LEG_H + HUMAN_BULLETCOLLISION_UP_H, oz, HUMAN_BULLETCOLLISION_HEAD_R, HUMAN_BULLETCOLLISION_HEAD_H, bvx, bvy, bvz) == true ){
				//�l�ɓ�����������
				HitBulletHuman(i, 0, bvx, bvy, bvz, brx, attacks, humanid, ontargetcnt);

				//�ΐl����p���X�g�ݒ�i��x���������A�Ō�ɓ��������l�����ݒ�j
				BulletObj_HumanIndex[objectID].Init();
				BulletObj_HumanIndex[objectID].SetIndexFlag(i);

				//�U���͂Ɗђʗ͂��v�Z
				attacks = (int)((float)attacks * 0.5f);
				penetration -= 1;

				CollideFlag = true;
			}

			//�㔼�g�Ɠ����蔻��
			if( CollideCylinderInside(ox, oy + HUMAN_BULLETCOLLISION_LEG_H, oz, HUMAN_BULLETCOLLISION_UP_R, HUMAN_BULLETCOLLISION_UP_H, bvx, bvy, bvz) == true ){
				//�l�ɓ�����������
				HitBulletHuman(i, 1, bvx, bvy, bvz, brx, attacks, humanid, ontargetcnt);

				//�ΐl����p���X�g�ݒ�i��x���������A�Ō�ɓ��������l�����ݒ�j
				BulletObj_HumanIndex[objectID].Init();
				BulletObj_HumanIndex[objectID].SetIndexFlag(i);

				//�U���͂Ɗђʗ͂��v�Z
				attacks = (int)((float)attacks * 0.6f);
				penetration -= 1;

				CollideFlag = true;
			}

			//���Ɠ����蔻��
			if( CollideCylinderInside(ox, oy, oz, HUMAN_BULLETCOLLISION_LEG_R, HUMAN_BULLETCOLLISION_LEG_H, bvx, bvy, bvz) == true ){
				//�l�ɓ�����������
				HitBulletHuman(i, 2, bvx, bvy, bvz, brx, attacks, humanid, ontargetcnt);

				//�ΐl����p���X�g�ݒ�i��x���������A�Ō�ɓ��������l�����ݒ�j
				BulletObj_HumanIndex[objectID].Init();
				BulletObj_HumanIndex[objectID].SetIndexFlag(i);

				//�U���͂Ɗђʗ͂��v�Z
				attacks = (int)((float)attacks * 0.7f);
				penetration -= 1;

				CollideFlag = true;
			}
		}

		//�ђʗ͂��c���Ă��Ȃ���Βe�͖����ɂ���i���� 2��ځj
		if( penetration < 0 ){
			in_bullet->SetEnableFlag(false);
			mapflag = 0;
			break;
		}

		//�����Ƃ̓����蔻��
		for(int i=0; i<MAX_SMALLOBJECT; i++){
			//�g�p����Ă��Ȃ���Ώ������Ȃ�
			if( SmallObjectIndex[i].GetEnableFlag() == false ){ continue; }

			//���W���擾
			float ox, oy, oz;
			int id;
			float decide;
			SmallObjectIndex[i].GetPosData(&ox, &oy, &oz, NULL);
			SmallObjectIndex[i].GetParamData(&id, NULL);

			//�����蔻��̑傫�����擾
			if( (TOTAL_PARAMETERINFO_SMALLOBJECT <= id)&&(id <= (TOTAL_PARAMETERINFO_SMALLOBJECT + MAX_ADDSMALLOBJECT -1)) ){
				decide = (float)MIFdata->GetAddSmallobjectDecide(id - TOTAL_PARAMETERINFO_SMALLOBJECT)*SMALLOBJECT_COLLISIONSCALE;
			}
			else{
				SmallObjectParameter Param;
				GameParamInfo->GetSmallObject(id, &Param);
				decide = (float)Param.decide*SMALLOBJECT_COLLISIONSCALE;
			}

			//�����蔻��
			if( CollideSphereInside(ox, oy, oz, decide, bvx, bvy, bvz) == true ){
				//�����ɓ�����������
				int attack2 = (int)(floorf( (float)attacks * 0.25f ));
				HitBulletSmallObject(i, bvx, bvy, bvz, attack2, teamid);

				//�U���͂��v�Z
				attacks = (int)(floorf( (float)attacks * 0.7f ));

				CollideFlag = true;
			}
		}

		if( mapflag > 0 ){
			//�}�b�v�Ƃ̓����蔻��
			if( CollD->CheckALLBlockInside(bvx, bvy, bvz) == true ){
				//�e���}�b�v�ɓ�����������
				HitBulletMap(bx + vx*(Dist-1.0f), by + vy*(Dist-1.0f), bz + vz*(Dist-1.0f), teamid);

				//�U���͂Ɗђʗ͂��v�Z
				penetration -= 1;
				if( penetration >= 0 ){
					attacks = (int)((float)attacks * 0.6f);
				}

				mapflag = 2;
				CollideFlag = true;
			}
		}
	}

	//�e����Ƀu���b�N������ɂ��ւ�炸�A�u���b�N�Ɠ����蔻����s���Ă��Ȃ��Ȃ��
	//�@�i�������u���b�N������j
	if( mapflag == 1 ){
		//�e���}�b�v�ɓ�����������
		HitBulletMap(bx + vx*(Dist-1.0f), by + vy*(Dist-1.0f), bz + vz*(Dist-1.0f), teamid);

		//�U���͂��v�Z
		if( penetration > 0 ){
			attacks = (int)((float)attacks * 0.75f);
		}
		else{
			attacks = (int)((float)attacks * 0.55f);
		}

		CollideFlag = true;
	}

	//�ݒ��K�p�i���ɍU���́E�ђʗ́j
	in_bullet->SetParamData(attacks, penetration, speed, teamid, humanid, ontargetcnt, false);

	return CollideFlag;
}

//! @brief �e���}�b�v�ɓ�����������
//! @param x ���eX���W
//! @param y ���eY���W
//! @param z ���eZ���W
//! @param teamID ���ˌ��̃`�[���ԍ�
void ObjectManager::HitBulletMap(float x, float y, float z, int teamID)
{
	//�G�t�F�N�g�i���j��`��
	AddEffect(x, y, z, 0.0f, 0.05f, 0.0f, 0.0f, DegreeToRadian(10)*GetRand(36), 0.0f, 3.0f, 0.24f, Resource->GetEffectSmokeTexture(), 20, 0, 0.63f, -0.03f, EFFECT_NORMAL, 20);

	//���ʉ����Đ�
	GameSound->HitMap(x, y, z, teamID);
}

//! @brief �e���l�ɓ�����������
//! @param HitHuman_id �l�̔ԍ�
//! @param Hit_id ��e�ӏ��i���F0�@���́F1�@���F2�j
//! @param x ��eX���W
//! @param y ��eY���W
//! @param z ��eZ���W
//! @param brx �����p�x
//! @param attacks �U����
//! @param Shothuman_id ���˂����l�̔ԍ�
//! @param ontargetcnt �������̃J�E���g��
void ObjectManager::HitBulletHuman(int HitHuman_id, int Hit_id, float x, float y, float z, float brx, int attacks, int Shothuman_id, float ontargetcnt)
{
	int Shothuman_TeamID;
	int HitHuman_TeamID;
	int hp_old;
	int damage = 0;
	int paramid;
	HumanParameter Paraminfo;
	bool NotRobot;

	//�g�p����Ă��Ȃ����A���̂Ȃ�Ώ������Ȃ��B
	if( HumanIndex[HitHuman_id].GetEnableFlag() == false ){ return; }
	if( HumanIndex[HitHuman_id].GetDeadFlag() == true ){ return; }

	//���ˌ��ƑΏېl���̃`�[���ԍ��擾
	HumanIndex[Shothuman_id].GetParamData(NULL, NULL, NULL, &Shothuman_TeamID);
	HumanIndex[HitHuman_id].GetParamData(NULL, NULL, NULL, &HitHuman_TeamID);

	//�_���[�W�v�Z�O��HP�擾
	hp_old = HumanIndex[HitHuman_id].GetHP();

	//�l�Ƀ_���[�W�ƏՌ���^����
	if( Hit_id == 0 ){ HumanIndex[HitHuman_id].HitBulletHead(attacks); }
	if( Hit_id == 1 ){ HumanIndex[HitHuman_id].HitBulletUp(attacks); }
	if( Hit_id == 2 ){ HumanIndex[HitHuman_id].HitBulletLeg(attacks); }
	HumanIndex[HitHuman_id].AddPosOrder(brx, 0.0f, 1.0f);

#ifdef ENABLE_BUG_TEAMID
	bool flag = true;

	//�`�[���ԍ��������A���`�[���ԍ����傫���Ȃ�A�t���O����
	if( (HitHuman_TeamID < 0)&&(Shothuman_TeamID < 0) ){
		if( HitHuman_TeamID < Shothuman_TeamID ){
			flag = false;
		}
	}

	if( flag == true ){
		HumanIndex[HitHuman_id].SetHitFlag(brx*-1 - (float)M_PI/2);
	}
#else
	HumanIndex[HitHuman_id].SetHitFlag(brx*-1 - (float)M_PI/2);
#endif

	//���{�b�g���ǂ�������
	HumanIndex[HitHuman_id].GetParamData(&paramid, NULL, NULL, NULL);
	GameParamInfo->GetHuman(paramid, &Paraminfo);
	if( Paraminfo.type == 1 ){
		NotRobot = false;
	}
	else{
		NotRobot = true;
	}

	//�G�t�F�N�g�i���j��`��
	if( Hit_id == 0 ){ damage = (int)((float)attacks * HUMAN_DAMAGE_HEAD); }
	if( Hit_id == 1 ){ damage = (int)((float)attacks * HUMAN_DAMAGE_UP); }
	if( Hit_id == 2 ){ damage = (int)((float)attacks * HUMAN_DAMAGE_LEG); }
	SetHumanBlood(x, y, z, damage, NotRobot);

	//���ʉ����Đ�
	GameSound->HitHuman(x, y, z, Hit_id, Shothuman_TeamID);

	if( (0 <= Shothuman_id)&&(Shothuman_id < MAX_HUMAN) ){
		//�e�𔭎˂����l�̐��ʂɉ��Z
		Human_ontarget[Shothuman_id] += ontargetcnt;
		if( Hit_id == 0 ){ Human_headshot[Shothuman_id] += 1; }
		if( (hp_old > 0)&&(HumanIndex[HitHuman_id].GetHP() <= 0) ){
			//�_���[�W�v�Z�O�� hp>0 �ł��A�v�Z��� hp <= 0 �Ȃ�A���񎀖S�����B
			Human_kill[Shothuman_id] += 1;
		}
	}

	//���O�֌W�̏���
	if( (hp_old > 0)&&(HumanIndex[HitHuman_id].GetHP() <= 0) ){
		//�_���[�W�v�Z�O�� hp>0 �ł��A�v�Z��� hp <= 0 �Ȃ�A���񎀖S�����B

		int player_teamid;
		HumanIndex[Player_HumanID].GetParamData(NULL, NULL, NULL, &player_teamid);

		ObjectLog->DiedHuman(Shothuman_id, HitHuman_id, Shothuman_TeamID, HitHuman_TeamID, player_teamid);
	}
}

//! @brief �e�������ɓ�����������
//! @param HitSmallObject_id �����̔ԍ�
//! @param x ���eX���W
//! @param y ���eY���W
//! @param z ���eZ���W
//! @param attacks �U����
//! @param teamID ���ˌ��̃`�[���ԍ�
void ObjectManager::HitBulletSmallObject(int HitSmallObject_id, float x, float y, float z, int attacks, int teamID)
{
	int hp;

	//�g�p����Ă��Ȃ���Ώ������Ȃ��B
	if( SmallObjectIndex[HitSmallObject_id].GetEnableFlag() == false ){ return; }

	//�̗͂��Ȃ���Ώ������Ȃ�
	hp = SmallObjectIndex[HitSmallObject_id].GetHP();
	if( hp <= 0 ){ return; }

	//�����Ƀ_���[�W��^����
	SmallObjectIndex[HitSmallObject_id].HitBullet(attacks);

	//�G�t�F�N�g�i���j��`��
	//AddEffect(x, y, z, 0.01f*(GetRand(101)-50), 0.01f*(GetRand(101)-50), 0.01f*(GetRand(101)-50), 0.0f, DegreeToRadian(10)*GetRand(36), 0.0f, 1.0f, 0.1f, Resource->GetEffectSmokeTexture(), 10, 0, 0.625f, -0.047f, EFFECT_NORMAL, 255);
	//AddEffect(x, y, z, 0.01f*(GetRand(101)-50), 0.01f*(GetRand(101)-50), 0.01f*(GetRand(101)-50), 0.0f, DegreeToRadian(10)*GetRand(36), 0.0f, 1.0f, 0.1f, Resource->GetEffectSmokeTexture(), 10, 0, 0.625f, -0.047f, EFFECT_NORMAL, 255);
	AddEffect(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, DegreeToRadian(10)*GetRand(36), 0.0f, 1.0f, 0.1f, Resource->GetEffectSmokeTexture(), 10, 0, 0.625f, -0.047f, EFFECT_NORMAL, 255);
	AddEffect(x, y, z, 0.0f, 0.01f*(GetRand(10)-5), 0.0f, 0.0f, DegreeToRadian(10)*GetRand(36), 0.0f, 1.0f, 0.1f, Resource->GetEffectSmokeTexture(), 10, 0, 0.625f, -0.047f, EFFECT_NORMAL, 255);

	//���ʉ����Đ�
	int id;
	SmallObjectIndex[HitSmallObject_id].GetParamData(&id, NULL);
	GameSound->HitSmallObject(x, y, z, id, teamID);

	//���O�֌W�̏���
	hp = SmallObjectIndex[HitSmallObject_id].GetHP();
	if( hp <= 0 ){
		//�_���[�W�v�Z�O�� hp>0 �ł��A�v�Z��� hp <= 0 �Ȃ�A����j�󂳂ꂽ�B
		ObjectLog->BreakSmallObject(HitSmallObject_id);
	}
}

//! @brief ��֒e�̃_���[�W����Ə���
//! @param in_grenade �Ώۂ̎�֒e�I�u�W�F�N�g
//! @return ���������Ftrue�@�������Ă��Ȃ��Ffalse
//! @attention ������s���Ώۂ́u�l�v�Ɓu�����v�ł��B
//! @attention �_���[�W����Ɍ��炸�A�_���[�W�v�Z����ʉ��Đ��܂ň�т��čs���܂��B
bool ObjectManager::GrenadeExplosion(grenade *in_grenade)
{
	if( in_grenade == NULL ){ return false; }

	bool returnflag = false;

	//���W���擾
	float gx, gy, gz;
	int teamid, humanid;
	in_grenade->GetPosData(&gx, &gy, &gz, NULL, NULL);
	in_grenade->GetParamData(NULL, &teamid, &humanid, NULL);

	//�l�ɔ����̓����蔻��
	for(int i=0; i<MAX_HUMAN; i++){
		//����������Ă��Ȃ����A���̂Ȃ�Ώ������Ȃ��B
		if( HumanIndex[i].GetEnableFlag() == false ){ continue; }
		if( HumanIndex[i].GetDeadFlag() == true ){ continue; }

		int HitHuman_TeamID;
		float hx, hy, hz;
		float x, y, z, r;

		//�l�̍��W���擾���A�������v�Z
		HumanIndex[i].GetPosData(&hx, &hy, &hz, NULL);
		x = hx - gx;
		y = hy - gy;
		z = hz - gz;
		r = sqrtf(x*x + y*y + z*z);

		//100.0��艓����Όv�Z���Ȃ�
		if( r > MAX_DAMAGE_GRENADE_DISTANCE + HUMAN_HEIGHT ){ continue; }

		float dummy = 0.0f;
		int total_damage = 0;
		int damage;

		//�����ɓ����蔻��
		y = hy + 2.0f - gy;
		r = sqrtf(x*x + y*y + z*z);
		//�u���b�N���Ղ��Ă��Ȃ���΁@�i���C�œ����蔻����s���A�������Ă��Ȃ���΁j
		if( CollD->CheckALLBlockIntersectRay(gx, gy, gz, x/r, y/r, z/r, NULL, NULL, &dummy, r) == false ){
			//�_���[�W�ʂ��v�Z
			damage = HUMAN_DAMAGE_GRENADE_LEG - (int)((float)HUMAN_DAMAGE_GRENADE_LEG/MAX_DAMAGE_GRENADE_DISTANCE * r);
			if( damage > 0 ){
				total_damage += damage;
			}
		}

		//���ɓ����蔻��
		y = hy + (HUMAN_HEIGHT-2.0f) - gy;
		r = sqrtf(x*x + y*y + z*z);
		//�u���b�N���Ղ��Ă��Ȃ���΁@�i���C�œ����蔻����s���A�������Ă��Ȃ���΁j
		if( CollD->CheckALLBlockIntersectRay(gx, gy, gz, x/r, y/r, z/r, NULL, NULL, &dummy, r) == false ){
			//�_���[�W�ʂ��v�Z
			damage = HUMAN_DAMAGE_GRENADE_HEAD - (int)((float)HUMAN_DAMAGE_GRENADE_HEAD/MAX_DAMAGE_GRENADE_DISTANCE * r);
			if( damage > 0 ){
				total_damage += damage;
			}
		}

		if( total_damage > 0 ){
			int hp_old;

			//�_���[�W�v�Z�O��HP�擾
			hp_old = HumanIndex[i].GetHP();

			//�_���[�W�𔽉f
			HumanIndex[i].HitGrenadeExplosion(total_damage);

			HumanIndex[i].GetParamData(NULL, NULL, NULL, &HitHuman_TeamID);

			float y2;
			float arx, ary;

			if( (0 <= humanid)&&(humanid < MAX_HUMAN) ){
				//�|���Ă���΁A���˂����l�̐��ʂɉ��Z
				if( HumanIndex[i].GetHP() <= 0 ){ Human_kill[humanid] += 1; }
			}

			//�G�t�F�N�g�i���j��`��
			SetHumanBlood(hx, hy+15.0f, hz, total_damage, false);

			//�l�Ǝ�֒e�̋������Z�o
			x = gx - hx;
			y = gy - (hy + 1.0f);
			z = gz - hz;

			//�p�x�����߂�
			arx = atan2f(z, x);

			if( sinf(atan2f(y, sqrtf(x*x + z*z))) < 0.0f ){		//������ɔ�ԂȂ�A�p�x���v�Z
				y2 = gy - (hy + HUMAN_HEIGHT);
				ary = atan2f(y2, sqrtf(x*x + z*z)) + (float)M_PI;
			}
			else{		//�������ɔ�ԂȂ�A�����p�x�͖����B�i�����Œn�ʂɂ߂荞�ނ̂�h�~�j
				ary = 0.0f;
			}

#ifdef ENABLE_BUG_TEAMID
			bool flag = true;

			//�`�[���ԍ��������A���`�[���ԍ����傫���Ȃ�A�t���O����
			if( (HitHuman_TeamID < 0)&&(teamid < 0) ){
				if( HitHuman_TeamID < teamid ){
					flag = false;
				}
			}

			if( flag == true ){
				HumanIndex[i].SetHitFlag(arx*-1 + (float)M_PI/2);
			}
#else
			HumanIndex[i].SetHitFlag(arx*-1 + (float)M_PI/2);
#endif

			//�����ɂ�镗��
			HumanIndex[i].AddPosOrder(arx, ary, 2.2f/MAX_DAMAGE_GRENADE_DISTANCE * (MAX_DAMAGE_GRENADE_DISTANCE - sqrtf(x*x + y*y + z*z)));

			//���O�֌W�̏���
			if( (hp_old > 0)&&(HumanIndex[i].GetHP() <= 0) ){
				//�_���[�W�v�Z�O�� hp>0 �ł��A�v�Z��� hp <= 0 �Ȃ�A���񎀖S�����B

				int player_teamid;
				HumanIndex[Player_HumanID].GetParamData(NULL, NULL, NULL, &player_teamid);

				ObjectLog->DiedHuman(humanid, i, teamid, HitHuman_TeamID, player_teamid);
			}

			returnflag = true;
		}
	}

	//�����ɔ����̓����蔻��
	for(int i=0; i<MAX_SMALLOBJECT; i++){
		//�g�p����Ă���Ώ������Ȃ�
		if( SmallObjectIndex[i].GetEnableFlag() == false ){ continue; }

		float sx, sy, sz;
		float x, y, z, r;

		//�����̍��W���擾���A�������v�Z
		SmallObjectIndex[i].GetPosData(&sx, &sy, &sz, NULL);
		x = sx - gx;
		y = sy - gy;
		z = sz - gz;
		r = sqrtf(x*x + y*y + z*z);

		//100.0��艓����Όv�Z���Ȃ�
		if( r > MAX_DAMAGE_GRENADE_DISTANCE ){ continue; }

		float dummy = 0.0f;

		//�u���b�N���Ղ��Ă��Ȃ���΁@�i���C�œ����蔻����s���A�������Ă��Ȃ���΁j
		if( CollD->CheckALLBlockIntersectRay(gx, gy, gz, x/r, y/r, z/r, NULL, NULL, &dummy, r) == false ){
			int id, damage;
			SmallObjectIndex[i].GetParamData(&id, NULL);

			//�_���[�W�ʂ��v�Z���A���f
			damage = SMALLOBJECT_DAMAGE_GRENADE - (int)((float)SMALLOBJECT_DAMAGE_GRENADE/MAX_DAMAGE_GRENADE_DISTANCE * r);
			SmallObjectIndex[i].HitGrenadeExplosion(damage);

			//����������ʉ��𔭂���
			GameSound->HitSmallObject(sx, sy, sz, id, teamid);

			//���O�֌W�̏���
			if( SmallObjectIndex[i].GetHP() <= 0 ){
				//�_���[�W�v�Z�O�� hp>0 �ł��A�v�Z��� hp <= 0 �Ȃ�A����j�󂳂ꂽ�B
				ObjectLog->BreakSmallObject(i);
			}

			returnflag = true;
		}
	}

	//�G�t�F�N�g�i�t���b�V���j�̕`��
	AddEffect(gx, gy, gz, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 40.0f, 0.0f, Resource->GetEffectMflashTexture(), 0, 0, 0.63f, 0.0f, EFFECT_NORMAL, 2);

	//�G�t�F�N�g�i���j�̕`��
	AddEffect(gx, gy, gz, 0.0f, 0.1f, 0.0f, 0.0f,
					DegreeToRadian(10)*GetRand(36), 0.0f, 36.0f, 0.2f, Resource->GetEffectSmokeTexture(), 220, 0, 0.6f, -0.01f, EFFECT_NORMAL, 50);
	AddEffect(gx + ((float)GetRand(7)/10-0.3f), gy + ((float)GetRand(5)/10-0.2f), gz + ((float)GetRand(7)/10-0.3f), (float)GetRand(5)/10-0.2f, (float)GetRand(3)/10, (float)GetRand(5)/10-0.2f, 0.0f,
					DegreeToRadian(10)*GetRand(36), 0.0f, ((float)GetRand(11)+25.0f)*2, 0.3f, Resource->GetEffectSmokeTexture(), 0, 0, 0.47f, -0.004f, EFFECT_NORMAL, 120);
	AddEffect(gx + ((float)GetRand(13)/10-0.6f), gy + ((float)GetRand(11)/10-0.5f), gz + ((float)GetRand(13)/10-0.6f), (float)GetRand(7)/10-0.3f, (float)GetRand(7)/10-0.3f, (float)GetRand(7)/10-0.3f, 0.0f,
					DegreeToRadian(10)*GetRand(36), DegreeToRadian(1), ((float)GetRand(21)+20.0f)*2, 0.2f, Resource->GetEffectSmokeTexture(), 0, 0, 0.4f, -0.004f, EFFECT_NORMAL, 100);
	AddEffect(gx + ((float)GetRand(13)/10-0.6f), gy + ((float)GetRand(11)/10-0.5f), gz + ((float)GetRand(13)/10-0.6f), (float)GetRand(7)/10-0.3f, (float)GetRand(7)/10-0.3f, (float)GetRand(7)/10-0.3f, 0.0f,
					DegreeToRadian(10)*GetRand(36), DegreeToRadian(-1), ((float)GetRand(21)+20.0f)*2, 0.2f, Resource->GetEffectSmokeTexture(), 0, 0, 0.4f, -0.004f, EFFECT_NORMAL, 100);

	//���ʉ����Đ�
	GameSound->GrenadeExplosion(gx, gy, gz, teamid);

	return returnflag;
}

//! @brief �|�ꂽ�ۂ̃G�t�F�N�g�ݒ�
//! @param in_human �Ώۂ̐l�I�u�W�F�N�g
void ObjectManager::DeadEffect(human *in_human)
{
	if( in_human == NULL ){ return; }

	int paramid;
	HumanParameter data;
	bool robot;

	in_human->GetParamData(&paramid, NULL, NULL, NULL);

	if( GameParamInfo->GetHuman(paramid, &data) != 0 ){ return; }

	if( data.type == 1 ){
		robot = true;
	}
	else{
		robot = false;
	}

	//�������{�b�g�Ȃ��
	if( robot == true ){
		float hx, hy, hz, hrx, hry;

		//�e��p�����[�^�[���擾
		in_human->GetPosData(&hx, &hy, &hz, &hrx);
		hry = in_human->GetDeadRy();

		//���ӂ�̍��W���Z�o
		hx += cosf(hrx*-1 - (float)M_PI/2) * sinf(hry) * HUMAN_HEIGHT/2;
		hz += sinf(hrx*-1 - (float)M_PI/2) * sinf(hry) * HUMAN_HEIGHT/2;

		//�G�t�F�N�g�i���j�̕`��
		float rnd = DegreeToRadian(10)*GetRand(18);
		AddEffect(hx+1.0f, hy+1.0f, hz+1.0f, 0.1f, 0.05f, 0.1f, 0.0f, rnd, 0, GetRand(30)*0.1f+30.0f, 0.1f, Resource->GetEffectSmokeTexture(), 10, 0, 1.0f, -1.0f / (GAMEFPS * 3), EFFECT_NORMAL, (int)GAMEFPS * 3);
		AddEffect(hx+1.0f, hy-1.0f, hz-1.0f, 0.2f, 0.05f, -0.2f, 0.0f, rnd*-1, 0, GetRand(30)*0.1f+30.0f, 0.1f, Resource->GetEffectSmokeTexture(), 10, 0, 1.0f, -1.0f / (GAMEFPS * 3), EFFECT_NORMAL, (int)GAMEFPS * 3);
		AddEffect(hx-1.0f, hy-1.0f, hz-1.0f, -0.3f, 0.05f, -0.3f, 0.0f, rnd, 0, GetRand(30)*0.1f+30.0f, 0.1f, Resource->GetEffectSmokeTexture(), 10, 0, 1.0f, -1.0f / (GAMEFPS * 3), EFFECT_NORMAL, (int)GAMEFPS * 3);
	}
}

//! �}�b�v�Ɍ����t�����邩����
//! @param in_effect �Ώۂ̃G�t�F�N�g�I�u�W�F�N�g
//! @param id �t������u���b�N�ԍ����󂯎��|�C���^
//! @param face �t������ʔԍ����󂯎��|�C���^
//! @param pos_x X���W���󂯎��|�C���^
//! @param pos_y Y���W���󂯎��|�C���^
//! @param pos_z Z���W���󂯎��|�C���^
//! @return �t������Ftrue�@�t�����Ȃ��Ffalse
bool ObjectManager::CollideBlood(effect *in_effect, int *id, int *face, float *pos_x, float *pos_y, float *pos_z)
{
	if( in_effect == NULL ){ return false; }
	if( (id == NULL)||(face == NULL) ){ return false; }
	if( (pos_x == NULL)||(pos_y == NULL)||(pos_z == NULL) ){ return false; }

	//�����ȃG�t�F�N�g�Ȃ�Ώ������Ȃ�
	if( in_effect->GetEnableFlag() == false ){ return false; }
	if( in_effect->GetCollideMapFlag() == false ){ return false; }
	if( in_effect->GetTextureID() != Resource->GetEffectBloodTexture() ){ return false; }

	//�����o�Ȃ��ݒ�Ȃ珈�����Ȃ�
	if( GameConfig.GetBloodFlag() == false ){ return false; }

	float x, y, z;
	float move_x, move_y, move_z;
	float vx, vy, vz, dist, dist2;

	//�G�t�F�N�g�̃p�����[�^�[�擾
	in_effect->GetPosData(&x, &y, &z, NULL);
	in_effect->GetMove(&move_x, &move_y, &move_z);

	//�G�t�F�N�g���ړ����Ȃ��Ȃ珈�����Ȃ�
	if( (move_x == 0.0f)&&(move_y == 0.0f)&&(move_z == 0.0f) ){ return false; }

	//�x�N�g�������߂�
	dist = sqrtf(move_x*move_x + move_y*move_y + move_z*move_z);
	vx = move_x / dist;
	vy = move_y / dist;
	vz = move_z / dist;

	//���C�̓����蔻��
	if( CollD->CheckALLBlockIntersectRay(x, y, z, vx, vy, vz, id, face, &dist2, dist) == false ){
		return false;
	}

	//�}�b�v���킸���ɕ�������
	dist2 -= 1.0f;

	//�t��������W�����߂�
	*pos_x = x + vx*dist2;
	*pos_y = y + vy*dist2;
	*pos_z = z + vz*dist2;

	return true;
}

//! @brief ������E��
//! @param in_human �Ώۂ̐l�I�u�W�F�N�g
//! @param in_weapon �Ώۂ̕���I�u�W�F�N�g
//! @attention �l�̎�ނ� �]���r �̏ꍇ�A���̊֐��͎��s���܂��B
void ObjectManager::PickupWeapon(human *in_human, weapon *in_weapon)
{
	if( in_human == NULL ){ return; }
	if( in_weapon == NULL ){ return; }

	//�����Ȑl�Ȃ�Ώ������Ȃ�
	if( in_human->GetEnableFlag() == false ){ return; }
	if( in_human->GetHP() <= 0 ){ return; }

	//�l�̎�ނ� �]���r �Ȃ�΁A���s
	int paramid;
	HumanParameter Paraminfo;
	in_human->GetParamData(&paramid, NULL, NULL, NULL);
	GameParamInfo->GetHuman(paramid, &Paraminfo);
	if( Paraminfo.type == 2 ){ return; }

	//����������Ă��镐��ŁA���N���g���Ă��Ȃ�����Ȃ�΁`
	if( (in_weapon->GetEnableFlag() == true)&&(in_weapon->GetUsingFlag() == false) ){
		float human_x, human_y, human_z;
		float weapon_x, weapon_y, weapon_z;
		float x, z;
		float r;

		//�l�ƕ���̍��W���擾
		in_human->GetPosData(&human_x, &human_y, &human_z, NULL);
		in_weapon->GetPosData(&weapon_x, &weapon_y, &weapon_z, NULL);

		//�������͈͓��Ȃ��
		if( ((human_y + HUMAN_PICKUPWEAPON_L) <= weapon_y)&&(weapon_y <= (human_y + HUMAN_PICKUPWEAPON_H)) ){
			//�������v�Z
			x = human_x - weapon_x;
			z = human_z - weapon_z;
			r = x*x + z*z;
			//�������͈͓��Ȃ�΁A�E�킹��B
			if( r < (HUMAN_PICKUPWEAPON_R * HUMAN_PICKUPWEAPON_R) ){
				in_human->PickupWeapon(in_weapon);
			}
		}
	}
}

//! @brief �I�u�W�F�N�g�����
void ObjectManager::CleanupPointDataToObject()
{
	for(int i=0; i<MAX_HUMAN; i++){
		HumanIndex[i].SetEnableFlag(false);
	}

	for(int i=0; i<MAX_WEAPON; i++){
		WeaponIndex[i].SetEnableFlag(false);
	}

	for(int i=0; i<MAX_SMALLOBJECT; i++){
		SmallObjectIndex[i].SetEnableFlag(false);
	}

	for(int i=0; i<MAX_BULLET; i++){
		BulletIndex[i].SetEnableFlag(false);
	}

	for(int i=0; i<MAX_GRENADE; i++){
		GrenadeIndex[i].SetEnableFlag(false);
	}

	for(int i=0; i<MAX_EFFECT; i++){
		EffectIndex[i].SetEnableFlag(false);
	}


	if( Resource != NULL ){
		Resource->CleanupHumanTexture();
	}
}

//! @brief �|�C���g�f�[�^�����ɃI�u�W�F�N�g��z�u
void ObjectManager::LoadPointData()
{
	AddCollisionFlag = false;
	Player_HumanID = 0;

	//���C�t���O������
	for(int i=0; i<MAX_HUMAN; i++){
		Human_ShotFlag[i] = false;
		Human_YakkyouCnt[i] = -1;
	}

	//FF�i���m�����j�L�����t���O�ݒ�
	for(int i=0; i<MAX_HUMAN; i++){
		FriendlyFire[i] = false;
	}

	//�l�̃e�N�X�`�����t���b�V���t���O������
	for(int i=0; i<MAX_HUMAN; i++){
		Human_FrameTextureRefresh[i] = false;
	}

	/*
	//�l���|�C���g��T��
	for(int i=0; i<PointData->GetTotaldatas(); i++){
		pointdata data;
		PointData->Getdata(&data, i);

		if( data.p1 == POINT_P1TYPE_HUMANINFO ){
			//�l�̃e�N�X�`����o�^
			Resource->AddHumanTexture(data.p2);
		}
	}
	*/

	//�l�E����E������T��
	for(int i=0; i<PointData->GetTotaldatas(); i++){
		pointdata data;
		PointData->Getdata(&data, i);

		//�l�Ȃ��
		if( (data.p1 == POINT_P1TYPE_HUMAN)||(data.p1 == POINT_P1TYPE_HUMAN2) ){
			pointdata humaninfodata;

			//�l���|�C���g��T��
			if( PointData->SearchPointdata(&humaninfodata, 0x01 + 0x08, POINT_P1TYPE_HUMANINFO, 0, 0, data.p2, 0) == 0 ){
				//continue;

				//�l���|�C���g��������Ȃ�������A�Ƃ肠�����u���� �� A�v�Ƃ��Ēǉ��B�i�o�O�̍Č��j

				HumanParameter HumanParam;
				int Weapon[TOTAL_HAVEWEAPON];
				GameParamInfo->GetHuman(0, &HumanParam);
				for(int j=0; j<TOTAL_HAVEWEAPON; j++){
					Weapon[j] = HumanParam.Weapon[j];
				}

				//�l�̃e�N�X�`����o�^
				Resource->AddHumanTexture(0);

				//�l��ǉ�
				if( data.p1 == POINT_P1TYPE_HUMAN2 ){
					Weapon[1] = ID_WEAPON_NONE;
				}

				//�v���C���[�Ȃ�΁A�ԍ����L�^
				if( (data.p4 == 0)&&(Player_HumanID == 0) ){
					Player_HumanID = AddHumanIndex(data.x, data.y, data.z, data.r, 0, 0, Weapon, data.p4, true);
				}
				else{
					AddHumanIndex(data.x, data.y, data.z, data.r, 0, 0, Weapon, data.p4, true);
				}
			}
			else{
				//�l�̃e�N�X�`����o�^
				Resource->AddHumanTexture(humaninfodata.p2);

				//�l�Ƃ��Ēǉ�
				AddHumanIndex(data, humaninfodata, true);
			}
		}

		//����Ȃ��
		if( (data.p1 == POINT_P1TYPE_WEAPON)||(data.p1 == POINT_P1TYPE_RAND_WEAPON) ){
			AddWeaponIndex(data);
		}

		//�����Ȃ��
		if( data.p1 == POINT_P1TYPE_SMALLOBJ ){
			AddSmallObjectIndex(data);
		}
	}

	//���O�֌W�̏���
	ObjectLog->ClearLog();
	ObjectLog->InfoLog("Set object...");
}

//! @brief ���\�[�X�̉�
//! @todo �S�ẴG�t�F�N�g������������B
void ObjectManager::Recovery()
{
	//�l�̃��f���ԍ����擾
	int upmodel[TOTAL_UPMODE];
	int armmodel[TOTAL_ARMMODE];
	int legmodel;
	int walkmodel[TOTAL_WALKMODE];
	int runmodel[TOTAL_RUNMODE];
	Resource->GetHumanModel(upmodel, armmodel, &legmodel, walkmodel, runmodel);

	//�l
	int HumanID;
	int GetHumanFlag;
	HumanParameter HumanParam;
	for(int i=0; i<MAX_HUMAN; i++){
		if( HumanIndex[i].GetEnableFlag() == true ){
			HumanIndex[i].GetParamData(&HumanID, NULL, NULL, NULL);

			GetHumanFlag = GameParamInfo->GetHuman(HumanID, &HumanParam);

#ifndef ENABLE_BUG_HUMANWEAPON
			if( GetHumanFlag != 0 ){
				HumanIndex[i].SetEnableFlag(false);
				continue;
			}
#endif

			if( GetHumanFlag == 0 ){
				//�l�̃e�N�X�`����o�^
				Resource->AddHumanTexture(HumanID);

				int id = Resource->GetHumanTexture(HumanID);
				if( id == -1 ){
					//�e�N�X�`�����擾�ł��Ȃ���΁A���t���b�V���t���O��L���ɂ��Ė��t���[���擾
					Human_FrameTextureRefresh[i] = true;
				}

				HumanIndex[i].SetTexture(id);
				HumanIndex[i].SetModel(upmodel[ HumanParam.model ], armmodel, legmodel, walkmodel, runmodel);
			}
			else{
#ifdef ENABLE_BUG_HUMANWEAPON
				HumanIndex[i].SetTexture(d3dg->GetMapTextureID(0));
				HumanIndex[i].SetModel(upmodel[0], armmodel, legmodel, walkmodel, runmodel);
#endif
			}
		}
	}

	//����
	int WeaponID;
	int Weaponmodel, Weapontexture;
	for(int i=0; i<MAX_WEAPON; i++){
		if( WeaponIndex[i].GetEnableFlag() == true ){
			//�ݒ�l���擾
			WeaponParameter WeaponParam;
			WeaponIndex[i].GetParamData(&WeaponID, NULL, NULL);
			if( Resource->GetWeaponModelTexture(WeaponID, &Weaponmodel, &Weapontexture) == 1 ){ continue; }
			if( GameParamInfo->GetWeapon(WeaponID, &WeaponParam) == 1 ){ continue; }

			//�K�p
			WeaponIndex[i].SetModel(Weaponmodel, WeaponParam.size);
			WeaponIndex[i].SetTexture(Weapontexture);
		}
	}

	//����
	int SmallObjectID;
	int SmallObjectmodel, SmallObjecttexture;
	for(int i=0; i<MAX_SMALLOBJECT; i++){
		if( SmallObjectIndex[i].GetEnableFlag() == true ){
			//�ݒ�l���擾
			SmallObjectIndex[i].GetParamData(&SmallObjectID, NULL);
			if( Resource->GetSmallObjectModelTexture(SmallObjectID, &SmallObjectmodel, &SmallObjecttexture) == 1 ){ continue; }

			//�K�p
			SmallObjectIndex[i].SetModel(SmallObjectmodel, SMALLOBJECT_SCALE);
			SmallObjectIndex[i].SetTexture(SmallObjecttexture);
		}
	}

	int bulletmodel, bullettexture;
	BulletParameter data;

	//�e�e�K�p
	if( Resource->GetBulletModelTexture(0, &bulletmodel, &bullettexture) == 0 ){
		if( GameParamInfo->GetBullet(0, &data) == 0 ){
			for(int i=0; i<MAX_BULLET; i++){
				BulletIndex[i].SetModel(bulletmodel, data.size);
				BulletIndex[i].SetTexture(bullettexture);
			}
		}
	}

	//��֒e�K�p
	if( Resource->GetBulletModelTexture(1, &bulletmodel, &bullettexture) == 0 ){
		if( GameParamInfo->GetBullet(1, &data) == 0 ){
			for(int i=0; i<MAX_GRENADE; i++){
				GrenadeIndex[i].SetModel(bulletmodel, data.size);
				GrenadeIndex[i].SetTexture(bullettexture);
			}
		}
	}

	//�S�ẴG�t�F�N�g�𖳌���
	for(int i=0; i<MAX_EFFECT; i++){
		if( EffectIndex[i].GetEnableFlag() == true ){
			EffectIndex[i].SetEnableFlag(false);
		}
	}

	//���O�֌W�̏���
	ObjectLog->InfoLog("Recovery object...");
}

//! @brief �ǉ��̓����蔻��t���O��ݒ�
//! @param flag �t���O
void ObjectManager::SetAddCollisionFlag(bool flag)
{
	AddCollisionFlag = flag;
}

//! @brief FF�i���m�����j�L�����t���O���擾
//! @param id �f�[�^�ԍ�
//! @return �t���O
bool ObjectManager::GetFriendlyFireFlag(int id)
{
	if( (id < 0)||(MAX_HUMAN <= id) ){ return false; }
	return FriendlyFire[id];
}

//! @brief FF�i���m�����j�L�����t���O��ݒ�
//! @param id �f�[�^�ԍ�
//! @param flag �t���O
void ObjectManager::SetFriendlyFireFlag(int id, bool flag)
{
	if( (id < 0)||(MAX_HUMAN <= id) ){ return; }
	FriendlyFire[id] = flag;
}

//! @brief �v���C���[�ԍ����擾
//! @return �v���C���[�̃f�[�^�ԍ�
int ObjectManager::GetPlayerID()
{
	return Player_HumanID;
}

//! @brief �v���C���[�ԍ���ݒ�
//! @param id �v���C���[�̃f�[�^�ԍ�
void ObjectManager::SetPlayerID(int id)
{
	Player_HumanID = id;
}

//! @brief �w�肵���f�[�^�ԍ���human�N���X���擾
//! @param id �f�[�^�ԍ�
//! @return �l�I�u�W�F�N�g�̃|�C���^�@�i�����ȃf�[�^�ԍ��� NULL�j
human* ObjectManager::GetHumanObject(int id)
{
	if( (id < 0)||(MAX_HUMAN <= id) ){ return NULL; }
	return &(HumanIndex[id]);
}

//! @brief �v���C���[��human�N���X���擾
//! @return �l�I�u�W�F�N�g�i�v���C���[�j�̃|�C���^
human* ObjectManager::GetPlayerHumanObject()
{
	return GetHumanObject(Player_HumanID);
}

//! @brief �w�肵��human�|�C���^�̃f�[�^�ԍ����擾
//! @param object �l�I�u�W�F�N�g�̃|�C���^
//! @return �f�[�^�ԍ��@�i�G���[�F-1�j
int ObjectManager::GetHumanObjectID(human* object)
{
	if( object == NULL ){ return -1; }

	for(int i=0; i<MAX_HUMAN; i++){
		if( &(HumanIndex[i]) == object ){
			return i;
		}
	}

	return -1;
}

//! @brief �w�肵���f�[�^�ԍ���weapon�N���X���擾
//! @param id �f�[�^�ԍ�
//! @return ����I�u�W�F�N�g�̃|�C���^�@�i�����ȃf�[�^�ԍ��� NULL�j
weapon* ObjectManager::GetWeaponObject(int id)
{
	if( (id < 0)||(MAX_WEAPON <= id) ){ return NULL; }
	return &(WeaponIndex[id]);
}

//! @brief �w�肵���f�[�^�ԍ���smallobject�N���X���擾
//! @param id �f�[�^�ԍ�
//! @return �����I�u�W�F�N�g�̃|�C���^�@�i�����ȃf�[�^�ԍ��� NULL�j
smallobject* ObjectManager::GetSmallObject(int id)
{
	if( (id < 0)||(MAX_SMALLOBJECT <= id) ){ return NULL; }
	return &(SmallObjectIndex[id]);
}

//! @brief �w�肵���f�[�^�ԍ���bullet�N���X���擾
//! @param id �f�[�^�ԍ�
//! @return �e�I�u�W�F�N�g�̃|�C���^�@�i�����ȃf�[�^�ԍ��� NULL�j
bullet* ObjectManager::GetBulletObject(int id)
{
	if( (id < 0)||(MAX_BULLET <= id) ){ return NULL; }
	return &(BulletIndex[id]);
}

//! @brief �w�肵��bullet�|�C���^�̃f�[�^�ԍ����擾
//! @param object �e�I�u�W�F�N�g�̃|�C���^
//! @return �f�[�^�ԍ��@�i�G���[�F-1�j
int ObjectManager::GetBulletObjectID(bullet* object)
{
	if( object == NULL ){ return -1; }

	for(int i=0; i<MAX_BULLET; i++){
		if( &(BulletIndex[i]) == object ){
			return i;
		}
	}

	return -1;
}

//! @brief �g�p����Ă��Ȃ�bullet�N���X���擾
//! @return ���ݖ��g�p�̒e�I�u�W�F�N�g�̃|�C���^�@�i���s����� NULL�j
bullet* ObjectManager::GetNewBulletObject()
{
	for(int i=0; i<MAX_BULLET; i++){
		if( BulletIndex[i].GetEnableFlag() == false ){
			return &(BulletIndex[i]);
		}
	}
	return NULL;
}

//! @brief �g�p����Ă��Ȃ�grenade�N���X���擾
//! @return ���ݖ��g�p�̎�֒e�I�u�W�F�N�g�̃|�C���^�@�i���s����� NULL�j
grenade* ObjectManager::GetNewGrenadeObject()
{
	for(int i=0; i<MAX_GRENADE; i++){
		if( GrenadeIndex[i].GetEnableFlag() == false ){
			return &(GrenadeIndex[i]);
		}
	}
	return NULL;
}

//! @brief �l������
//! @param p4 �����Ώۂ̎��ʔԍ�
//! @return �Y������human�N���X�̃|�C���^�@�i������Ȃ��ꍇ��NULL�j
//! @attention �����Y������ꍇ�A�ŏ��ɊY�������f�[�^��Ԃ��܂��B
human* ObjectManager::SearchHuman(signed short int p4)
{
	signed short int humanp4;

	for(int i=0; i<MAX_HUMAN; i++){
		//�g���Ă��Ȃ��l�Ȃ�Ώ������Ȃ�
		if( HumanIndex[i].GetEnableFlag() == false ){ continue; }

		//��4�p�����[�^���擾
		HumanIndex[i].GetParamData(NULL, NULL, &humanp4, NULL);

		//�w�肳�ꂽp4�ƈ�v����ΕԂ�
		if( humanp4 == p4 ){
			return &(HumanIndex[i]);
		}
	}
	return NULL;
}

//! @brief ����������
//! @param p4 �����Ώۂ̎��ʔԍ�
//! @return �Y������smallobject�N���X�̃|�C���^�@�i������Ȃ��ꍇ��NULL�j
//! @attention �����Y������ꍇ�A�ŏ��ɊY�������f�[�^��Ԃ��܂��B
smallobject* ObjectManager::SearchSmallobject(signed short int p4)
{
	signed short int smallobjectp4;

	for(int i=0; i<MAX_SMALLOBJECT; i++){
		//�g���Ă��Ȃ��l�Ȃ�Ώ������Ȃ�
		//�@�y�j��ς݂̃I�u�W�F�N�g�����肷�邽�߁A�����z
		//if( SmallObjectIndex[i].GetEnableFlag() == false ){ continue; }

		//��4�p�����[�^���擾
		SmallObjectIndex[i].GetParamData(NULL, &smallobjectp4);

		//�w�肳�ꂽp4�ƈ�v����ΕԂ�
		if( smallobjectp4 == p4 ){
			return &(SmallObjectIndex[i]);
		}
	}
	return NULL;
}

//! @brief �L���ȃI�u�W�F�N�g�����擾
//! @param HumanCnt �l�I�u�W�F�N�g�̗L�������󂯎��|�C���^�@�iNULL�j
//! @param WeaponCnt ����I�u�W�F�N�g�̗L�������󂯎��|�C���^�@�iNULL�j
//! @param SmallobjectCnt �����I�u�W�F�N�g�̗L�������󂯎��|�C���^�@�iNULL�j
//! @param BulletCnt �e�I�u�W�F�N�g�̗L�������󂯎��|�C���^�@�iNULL�j
//! @param GrenadeCnt ��֒e�I�u�W�F�N�g�̗L�������󂯎��|�C���^�@�iNULL�j
//! @param EffectCnt �G�t�F�N�g�I�u�W�F�N�g�̗L�������󂯎��|�C���^�@�iNULL�j
void ObjectManager::GetTotalObjects(int *HumanCnt, int *WeaponCnt, int *SmallobjectCnt, int *BulletCnt, int *GrenadeCnt, int *EffectCnt)
{
	if( HumanCnt != NULL ){
		*HumanCnt = 0;
		for(int i=0; i<MAX_HUMAN; i++){
			if( HumanIndex[i].GetEnableFlag() == true ){ *HumanCnt += 1; }
		}
	}

	if( WeaponCnt != NULL ){
		*WeaponCnt = 0;
		for(int i=0; i<MAX_WEAPON; i++){
			if( WeaponIndex[i].GetEnableFlag() == true ){ *WeaponCnt += 1; }
		}
	}

	if( SmallobjectCnt != NULL ){
		*SmallobjectCnt = 0;
		for(int i=0; i<MAX_SMALLOBJECT; i++){
			if( SmallObjectIndex[i].GetEnableFlag() == true ){ *SmallobjectCnt += 1; }
		}
	}

	if( BulletCnt != NULL ){
		*BulletCnt = 0;
		for(int i=0; i<MAX_BULLET; i++){
			if( BulletIndex[i].GetEnableFlag() == true ){ *BulletCnt += 1; }
		}
	}

	if( GrenadeCnt != NULL ){
		*GrenadeCnt = 0;
		for(int i=0; i<MAX_GRENADE; i++){
			if( GrenadeIndex[i].GetEnableFlag() == true ){ *GrenadeCnt += 1; }
		}
	}

	if( EffectCnt != NULL ){
		*EffectCnt = 0;
		for(int i=0; i<MAX_EFFECT; i++){
			if( EffectIndex[i].GetEnableFlag() == true ){ *EffectCnt += 1; }
		}
	}
}

//! @brief �O�i�i����j�����s
//! @param human_id �l�̔ԍ��i0�`MAX_HUMAN-1�j
void ObjectManager::MoveForward(int human_id)
{
	//�l�͈̔͂��`�F�b�N
	if( (human_id < 0)||(MAX_HUMAN <= human_id) ){ return; }

	//�I�u�W�F�N�g�Ƀt���O��ݒ�
	HumanIndex[human_id].SetMoveForward();
}

//! @brief ��ނ����s
//! @param human_id �l�̔ԍ��i0�`MAX_HUMAN-1�j
void ObjectManager::MoveBack(int human_id)
{
	//�l�͈̔͂��`�F�b�N
	if( (human_id < 0)||(MAX_HUMAN <= human_id) ){ return; }

	//�I�u�W�F�N�g�Ƀt���O��ݒ�
	HumanIndex[human_id].SetMoveBack();
}

//! @brief ����������s
//! @param human_id �l�̔ԍ��i0�`MAX_HUMAN-1�j
void ObjectManager::MoveLeft(int human_id)
{
	//�l�͈̔͂��`�F�b�N
	if( (human_id < 0)||(MAX_HUMAN <= human_id) ){ return; }

	//�I�u�W�F�N�g�Ƀt���O��ݒ�
	HumanIndex[human_id].SetMoveLeft();
}

//! @brief �E��������s
//! @param human_id �l�̔ԍ��i0�`MAX_HUMAN-1�j
void ObjectManager::MoveRight(int human_id)
{
	//�l�͈̔͂��`�F�b�N
	if( (human_id < 0)||(MAX_HUMAN <= human_id) ){ return; }

	//�I�u�W�F�N�g�Ƀt���O��ݒ�
	HumanIndex[human_id].SetMoveRight();
}

//! @brief ���������s
//! @param human_id �l�̔ԍ��i0�`MAX_HUMAN-1�j
void ObjectManager::MoveWalk(int human_id)
{
	//�l�͈̔͂��`�F�b�N
	if( (human_id < 0)||(MAX_HUMAN <= human_id) ){ return; }

	//�I�u�W�F�N�g�Ƀt���O��ݒ�
	HumanIndex[human_id].SetMoveWalk();
}

//! @brief �W�����v
//! @param human_id �l�̔ԍ��i0�`MAX_HUMAN-1�j
void ObjectManager::MoveJump(int human_id)
{
	//�l�͈̔͂��`�F�b�N
	if( (human_id < 0)||(MAX_HUMAN <= human_id) ){ return; }

	HumanIndex[human_id].Jump();
}

//! @brief ���C
//! @param human_id ���C����l�ԍ�
//! @return �ʏ�e���ˁF1�@��֒e���ˁF2�@���s�F0
int ObjectManager::ShotWeapon(int human_id)
{
	//�l�͈̔͂��`�F�b�N
	if( (human_id < 0)||(MAX_HUMAN <= human_id) ){ return 0; }

	human *MyHuman = &(HumanIndex[human_id]);

	float pos_x, pos_y, pos_z;
	int teamid;
	float rotation_x, armrotation_y;
	int weapon_paramid;
	int GunsightErrorRange;
	WeaponParameter ParamData;
	bool playerflag;
	bool grenadeflag;

	//�l�̍��W�Ɗp�x���擾
	MyHuman->GetPosData(&pos_x, &pos_y, &pos_z, NULL);
	MyHuman->GetParamData(NULL, NULL, NULL, &teamid);
	MyHuman->GetRxRy(&rotation_x, &armrotation_y);

	//�Ώێ҂��v���C���[���g������
	if( MyHuman == &(HumanIndex[Player_HumanID]) ){
		playerflag = true;
	}
	else{
		playerflag = false;
	}

	//�e�̔��˂�v��
	if( MyHuman->ShotWeapon(&weapon_paramid, &GunsightErrorRange) == false ){ return 0; }

	//����̏����擾
	if( GameParamInfo->GetWeapon(weapon_paramid, &ParamData) != 0 ){ return 0; }

	//��֒e������
	if( weapon_paramid == ID_WEAPON_GRENADE ){ grenadeflag = true; }
	else{ grenadeflag = false; }

	//�덷�͈̔͂��v�Z
	int ErrorRange;
	ErrorRange = GunsightErrorRange;
	if( ErrorRange < ParamData.ErrorRangeMIN ){ ErrorRange = ParamData.ErrorRangeMIN; }

	//���ˊp�x�i�덷�[���̃x�[�X�j������
	float rx, ry;
	rx = rotation_x*-1 + (float)M_PI/2;
	ry = armrotation_y;

	//�덷������
	if( ErrorRange != 0 ){
		int ErrorRange_x, ErrorRange_y;

		ErrorRange_x = GetRand(ErrorRange*2+1) - ErrorRange;
		ErrorRange_y = GetRand(ErrorRange*2+1) - ErrorRange;

		//���ˌ덷���@�p�x�ɉ��Z
		rx += ErrorRange_x * DegreeToRadian(0.15f);
		ry += ErrorRange_y * DegreeToRadian(0.15f);
	}

	//��֒e�łȂ����
	if( grenadeflag == false ){
		class bullet* newbullet;

		//�V���b�g�K���Ȃǔ��˂���e�i�y���b�g�j�̐����J��Ԃ�
		for(int i=0; i<ParamData.pellet; i++){
			int attacks;
			float ontargetcnt;
			float rx2, ry2;

			//�V���b�g�K���Ȃǔ��˂���e�i�y���b�g�j�����������
			if( ParamData.pellet > 1 ){
				//1�̒e������̍U���͂Ɩ������̃J�E���g�����Z�o
				//�@�@�S�e���킹�āA2�{�ɂȂ�悤�ɂ���B
				attacks = (int)( (float)ParamData.attacks / ((float)ParamData.pellet/2) );
				ontargetcnt = 1.0f / ((float)ParamData.pellet/2);

				//����Ɍ덷�𑝂₵�Ċg�U������
				int len;
				float a = DegreeToRadian(10) * GetRand(36);
				len = GetRand(5)*2+5;
				rx2 = rx + cosf(a)*len * DegreeToRadian(0.15f);
				ry2 = ry + sinf(a)*len * DegreeToRadian(0.15f);
			}
			else{
				//���̂܂܍U���͂Ɩ������̃J�E���g���𔽉f
				attacks = ParamData.attacks;
				ontargetcnt = 1.0f;

				//���ˌ덷�͂��̂܂�
				rx2 = rx;
				ry2 = ry;
			}

			//���˂��関�g�p�̃I�u�W�F�N�g���擾
			newbullet = GetNewBulletObject();
			if( newbullet == NULL ){ return 0; }

			//�e�e�𔭎�
			newbullet->SetPosData(pos_x, pos_y + WEAPONSHOT_HEIGHT, pos_z, rx2, ry2);
			newbullet->SetParamData(attacks, ParamData.penetration, (float)ParamData.speed * BULLET_SPEEDSCALE, teamid, human_id, ontargetcnt, true);
			newbullet->SetEnableFlag(true);

			//�ΐl����p���X�g������
			BulletObj_HumanIndex[ GetBulletObjectID(newbullet) ].Init();
		}

		//���C�t���O��ݒ�
		Human_ShotFlag[human_id] = true;
		Human_YakkyouCnt[human_id] = ParamData.yakkyou_delay;
	}
	else{
		class grenade* newgrenade;

		//���˂��関�g�p�̃I�u�W�F�N�g���擾
		newgrenade = GetNewGrenadeObject();
		if( newgrenade == NULL ){ return 0; }

		//��֒e����
		newgrenade->SetPosData(pos_x, pos_y + WEAPONSHOT_HEIGHT, pos_z, rx, ry);
		newgrenade->SetParamData(8.0f, teamid, human_id, 1.0f, true);
		newgrenade->SetEnableFlag(true);
	}

	if( ParamData.soundvolume > 0 ){
		//�e�����Đ�
		GameSound->ShotWeapon(pos_x, pos_y + WEAPONSHOT_HEIGHT, pos_z, weapon_paramid, teamid, playerflag);
	}

	if( grenadeflag == true ){
		return 2;
	}
	return 1;
}

//! @brief �}�Y���t���b�V����ݒ�
//! @param humanid �l�̔ԍ�
//! @attention ���̊֐��̌Ăяo���^�C�~���O�����ƁA�e���ɑ΂��ă}�Y���t���b�V���������܂���B
void ObjectManager::ShotWeaponEffect(int humanid)
{
	//�l�͈̔͂��`�F�b�N
	if( (humanid < 0)||(MAX_HUMAN <= humanid) ){ return; }

	float pos_x, pos_y, pos_z;
	float rotation_x, armrotation_y;
	int weapon_paramid;
	WeaponParameter ParamData;
	float x, y, z;
	float flashsize, smokesize1, smokesize2;
	float rx, emx, emy, emz;

	//�l�̍��W�Ɗp�x���擾
	HumanIndex[humanid].GetPosData(&pos_x, &pos_y, &pos_z, NULL);
	HumanIndex[humanid].GetRxRy(&rotation_x, &armrotation_y);

	//����̏����擾
	weapon_paramid = HumanIndex[humanid].GetMainWeaponTypeNO();
	if( GameParamInfo->GetWeapon(weapon_paramid, &ParamData) != 0 ){ return; }

	//�}�Y���t���b�V���Ɖ��̃T�C�Y������
	if( ParamData.silencer == true ){ flashsize = 1.2f; }
	else{ flashsize = 0.06f * ParamData.attacks; }
	smokesize1 = 0.04f * ParamData.attacks;
	smokesize2 = 0.032f * ParamData.attacks;

	//���̈ړ��x�N�g��������
	rx = rotation_x*-1 + (float)M_PI/2;
	emx = cosf(rx)*cosf(armrotation_y);
	emy = sinf(armrotation_y);
	emz = sinf(rx)*cosf(armrotation_y);

	//�s��ŃG�t�F�N�g���W���v�Z
	d3dg->SetWorldTransformHumanWeapon(pos_x, pos_y + 16.0f, pos_z, ParamData.flashx/10, ParamData.flashy/10, ParamData.flashz/10, rotation_x, armrotation_y*-1, 1.0f);
	d3dg->GetWorldTransformPos(&x, &y, &z);
	d3dg->ResetWorldTransform();

	//�}�Y���t���b�V���`��
	AddEffect(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, DegreeToRadian(10)*GetRand(36), 0.0f, flashsize, 0.0f, Resource->GetEffectMflashTexture(), 0, 0, 0.63f, 0.0f, EFFECT_NORMAL, 1);

	//�s��ŃG�t�F�N�g���W���v�Z
	d3dg->SetWorldTransformHumanWeapon(pos_x, pos_y + 16.0f, pos_z, ParamData.flashx/10, ParamData.flashy/10, ParamData.flashz/10 - 0.1f, rotation_x, armrotation_y*-1, 1.0f);
	d3dg->GetWorldTransformPos(&x, &y, &z);
	d3dg->ResetWorldTransform();

	//�G�t�F�N�g�i���j�̕`��
	AddEffect(x, y, z, emx*0.15f, emy*0.15f + 0.02f, emz*0.15f, 0.0f, DegreeToRadian(10)*GetRand(36), 0.0f, smokesize1, 0.3f, Resource->GetEffectSmokeTexture(), 0, 0, 0.47f, -0.047f, EFFECT_NORMAL, 12);
	AddEffect(x, y, z, emx*0.45f, emy*0.45f, emz*0.45f, 0.0f, DegreeToRadian(10)*GetRand(36), 0.0f, smokesize2, 0.36f, Resource->GetEffectSmokeTexture(), 0, 0, 0.47f, -0.055f, EFFECT_NORMAL, 10);
}

//! @brief ��䰔r�o��ݒ�
//! @param humanid �l�̔ԍ�
//! @attention ���̊֐��̌Ăяo���^�C�~���O�����ƁA�e�ɑ΂��Ė�䰔r�o�ʒu�������܂���B
void ObjectManager::ShotWeaponYakkyou(int humanid)
{
	//�l�͈̔͂��`�F�b�N
	if( (humanid < 0)||(MAX_HUMAN <= humanid) ){ return; }

	float pos_x, pos_y, pos_z;
	float rotation_x, armrotation_y;
	int weapon_paramid;
	WeaponParameter ParamData;
	float x, y, z;
	float yakkyousize;
	float rx;
	float mx, my, mz;

	//�l�̍��W�Ɗp�x���擾
	HumanIndex[humanid].GetPosData(&pos_x, &pos_y, &pos_z, NULL);
	HumanIndex[humanid].GetRxRy(&rotation_x, &armrotation_y);

	//����̏����擾
	weapon_paramid = HumanIndex[humanid].GetMainWeaponTypeNO();
	if( GameParamInfo->GetWeapon(weapon_paramid, &ParamData) != 0 ){ return; }

	//�}�Y���t���b�V���Ɖ��̃T�C�Y������
	if( ParamData.yakkyou_size >= 0.0f ){ yakkyousize = ParamData.yakkyou_size/10; }
	else{ yakkyousize = 0.012f * ParamData.attacks; }

	//����̏����擾
	weapon_paramid = HumanIndex[humanid].GetMainWeaponTypeNO();
	if( GameParamInfo->GetWeapon(weapon_paramid, &ParamData) != 0 ){ return; }

	//��]�ʂ�����
	rx = rotation_x*-1 + (float)M_PI/2;

	//��䰂̈ړ��ʂ�����
	mx = cosf(rx - (float)M_PI/2) * ParamData.yakkyou_sx * 0.12f;
	my = (ParamData.yakkyou_sy + (GetRand(3)-1)) * 0.12f;
	mz = sinf(rx - (float)M_PI/2) * ParamData.yakkyou_sx * 0.12f;

	//�s��ŃG�t�F�N�g���W���v�Z
	d3dg->SetWorldTransformHumanWeapon(pos_x, pos_y + 16.0f, pos_z, ParamData.yakkyou_px/10, ParamData.yakkyou_py/10, ParamData.yakkyou_pz/10, rotation_x, armrotation_y*-1, 1.0f);
	d3dg->GetWorldTransformPos(&x, &y, &z);
	d3dg->ResetWorldTransform();

	//��䰕`��
	AddEffect(x, y, z, mx, my, mz, -0.15f, DegreeToRadian(10)*GetRand(36), DegreeToRadian(GetRand(61)-30), yakkyousize, 0.0f, Resource->GetEffectYakkyouTexture(), 0, 0, 1.0f, 0.0f, EFFECT_NORMAL, (int)(GAMEFPS*0.75f));
}

//! @brief ����������[�h
//! @param human_id �Ώۂ̐l�ԍ�
//! @return �����Ftrue�@���s�Ffalse
bool ObjectManager::ReloadWeapon(int human_id)
{
	//�l�͈̔͂��`�F�b�N
	if( (human_id < 0)||(MAX_HUMAN <= human_id) ){ return false; }

	//�����Ȑl�Ȃ�Ώ������Ȃ�
	if( HumanIndex[human_id].GetEnableFlag() == false ){ return false; }
	if( HumanIndex[human_id].GetHP() <= 0 ){ return false; }

	//�����[�h�����s
	if( HumanIndex[human_id].ReloadWeapon() == true ){
		float x, y, z;
		int id;

		//�l�̍��W�ƃ`�[���ԍ����擾
		HumanIndex[human_id].GetPosData(&x, &y, &z, NULL);
		HumanIndex[human_id].GetParamData(NULL, NULL, NULL, &id);
		y += 16.0f;

		//������z�u
		GameSound->ReloadWeapon(x, y, z, id);

		//Human_YakkyouCnt[human_id] = -1;

		return true;
	}

	return false;
}

//! @brief �����؂�ւ���i�����ւ���j
//! @param human_id �l�̔ԍ��i0�`MAX_HUMAN-1�j
//! @param id �����ւ��镐��@�i-1 �Ŏ��̕���j
void ObjectManager::ChangeHaveWeapon(int human_id, int id)
{
	//�l�͈̔͂��`�F�b�N
	if( (human_id < 0)||(MAX_HUMAN <= human_id) ){ return; }

	HumanIndex[human_id].ChangeHaveWeapon(id);
	Human_YakkyouCnt[human_id] = -1;
}

//! @brief ����̎�ސ؂�ւ�
//! @param human_id �l�̔ԍ��i0�`MAX_HUMAN-1�j
//! @return �����F0�@���s�F1
int ObjectManager::ChangeWeaponID(int human_id)
{
	//�l�͈̔͂��`�F�b�N
	if( (human_id < 0)||(MAX_HUMAN <= human_id) ){ return 1; }

	//����̎�ނ�؂�ւ�����
	if( HumanIndex[human_id].ChangeWeaponID() == 0 ){
		int selectweapon;
		weapon *weapon[TOTAL_HAVEWEAPON];
		int WeaponID;
		int model, texture;
		float WeaponSize;

		//���݂̕�����擾
		HumanIndex[human_id].GetWeapon(&selectweapon, weapon, NULL, NULL);
		weapon[selectweapon]->GetParamData(&WeaponID, NULL, NULL);

		//���f���ƃe�N�X�`�����擾
		if( Resource->GetWeaponModelTexture(WeaponID, &model, &texture) == 1 ){
			return 1;
		}

		//����̃T�C�Y���擾
		WeaponParameter ParamData;
		if( GameParamInfo->GetWeapon(WeaponID, &ParamData) != 0 ){ return 1; }
		WeaponSize = ParamData.size;

		//����I�u�W�F�N�g�̃��f���E�e�N�X�`����ύX
		weapon[selectweapon]->SetModel(model, WeaponSize);
		weapon[selectweapon]->SetTexture(texture);

		Human_YakkyouCnt[human_id] = -1;

		return 0;
	}
	return 1;
}

//! @brief ������̂Ă�
//! @param human_id �l�̔ԍ��i0�`MAX_HUMAN-1�j
//! @return �����Ftrue�@���s�Ffalse
bool ObjectManager::DumpWeapon(int human_id)
{
	//�l�͈̔͂��`�F�b�N
	if( (human_id < 0)||(MAX_HUMAN <= human_id) ){ return false; }

	Human_YakkyouCnt[human_id] = -1;

	return HumanIndex[human_id].DumpWeapon();
}

//! @brief �X�R�[�v���[�h��؂�ւ�
//! @param human_id �l�̔ԍ��i0�`MAX_HUMAN-1�j
void ObjectManager::ChangeScopeMode(int human_id)
{
	//�l�͈̔͂��`�F�b�N
	if( (human_id < 0)||(MAX_HUMAN <= human_id) ){ return; }

	if( HumanIndex[human_id].GetScopeMode() == 0 ){	//�X�R�[�v���g�p���Ă��Ȃ���΁A�X�R�[�v��ݒ�
		HumanIndex[human_id].SetEnableScope();
	}
	else{											//�g�p���Ȃ�A����
		HumanIndex[human_id].SetDisableScope();
	}

	//Human_YakkyouCnt[human_id] = -1;
}

//! @brief ���Z�E�������Ă��镐��̒e��ǉ�
//! @param human_id �l�̔ԍ��i0�`MAX_HUMAN-1�j
//! @return �����Ftrue�@���s�Ffalse
bool ObjectManager::CheatAddBullet(int human_id)
{
	//�l�͈̔͂��`�F�b�N
	if( (human_id < 0)||(MAX_HUMAN <= human_id) ){ return false; }

	int selectweapon;
	weapon *weapon[TOTAL_HAVEWEAPON];
	int id_param, lnbs, nbs;
	WeaponParameter ParamData;

	//�������Ă��镐����擾
	for(int i=0; i<TOTAL_HAVEWEAPON; i++){
		weapon[i] = NULL;
	}
	HumanIndex[human_id].GetWeapon(&selectweapon, weapon, NULL, NULL);

	//��������̕���������Ă����
	if( weapon[selectweapon] != NULL ){
		//����̎�ނƒe���A����̐ݒ�l���擾
		weapon[selectweapon]->GetParamData(&id_param, &lnbs, &nbs);
		if( GameParamInfo->GetWeapon(id_param, &ParamData) == 0 ){
			//�ő�e�������Z���A�K�p
			nbs += ParamData.nbsmax;
			weapon[selectweapon]->ResetWeaponParam(Resource, id_param, lnbs, nbs);
			return true;
		}
	}

	return false;
}

//! @brief ���Z�E�������Ă��镐���ύX
//! @param human_id �l�̔ԍ��i0�`MAX_HUMAN-1�j
//! @param new_weaponID �V���ɐݒ肷�镐��̎�ޔԍ�
//! @return �����Ftrue�@���s�Ffalse
bool ObjectManager::CheatNewWeapon(int human_id, int new_weaponID)
{
	//�l�͈̔͂��`�F�b�N
	if( (human_id < 0)||(MAX_HUMAN <= human_id) ){ return false; }
	if( (new_weaponID < 0)||(TOTAL_PARAMETERINFO_WEAPON <= new_weaponID) ){ return false; }

	human *myHuman = &(HumanIndex[human_id]);

	int selectweapon;
	weapon *weapon[TOTAL_HAVEWEAPON];
	int lnbslist[TOTAL_HAVEWEAPON];
	int nbslist[TOTAL_HAVEWEAPON];
	int lnbs, nbs;

	//�������Ă��镐����擾
	for(int i=0; i<TOTAL_HAVEWEAPON; i++){
		weapon[i] = NULL;
		lnbslist[i] = 0;
		nbslist[i] = 0;
	}
	myHuman->GetWeapon(&selectweapon, weapon, lnbslist, nbslist);

	//���ݕ�������L���Ă��炸�A�V���Ɏ������镐����Ȃ��Ȃ�
	if( (weapon[selectweapon] == NULL)&&(new_weaponID == ID_WEAPON_NONE) ){
		return false;		//�������̂����Ӗ�
	}

	if( weapon[selectweapon] == NULL ){		//��������L���Ă��Ȃ����
		int dataid = -1;

		//�V���������z�u
		dataid = AddVisualWeaponIndex(new_weaponID, false);

		//���킪�z�u�ł����
		if( dataid != -1 ){
			//����ݒ��K�p�i��ɒe���j
			weapon[selectweapon] = &(WeaponIndex[dataid]);
			weapon[selectweapon]->ResetWeaponParam(Resource, new_weaponID, lnbslist[selectweapon], nbslist[selectweapon]);

			//���������̒e�����͏�����
			lnbslist[selectweapon] = 0;
			nbslist[selectweapon] = 0;

			//�������������
			myHuman->SetWeapon(weapon, lnbslist, nbslist);

			Human_YakkyouCnt[human_id] = -1;
			return true;
		}
	}
	else{									//��������L���Ă����
		//����ݒ���擾�i�e���ݒ�������p�����߁j
		weapon[selectweapon]->GetParamData(NULL, &lnbs, &nbs);

		if( new_weaponID == ID_WEAPON_NONE ){			//����������Ȃ�
			//���̕���͍폜
			weapon[selectweapon]->SetEnableFlag(false);

			//������폜���A�e���𔽉f
			weapon[selectweapon] = NULL;
			lnbslist[selectweapon] = lnbs;
			nbslist[selectweapon] = nbs;
			myHuman->SetWeapon(weapon, lnbslist, nbslist);

			//�X�R�[�v���[�h������
			myHuman->SetDisableScope();
		}
		else{											//�����ύX����Ȃ�
			//����ݒ��K�p
			weapon[selectweapon]->ResetWeaponParam(Resource, new_weaponID, lnbs, nbs);
		}

		Human_YakkyouCnt[human_id] = -1;
		return true;
	}

	return false;
}

//! @brief �]���r�̍U�����󂯂邩����
//! @param MyHuman �U������l�I�u�W�F�N�g�i�]���r���j�̃|�C���^
//! @param EnemyHuman �U�����󂯂��l�I�u�W�F�N�g�̃|�C���^
//! @return �����Ftrue�@�s�����Ffalse
//! @warning MyHuman �̓]���r�ȊO���w��ł��܂��B<b>�]���r���ǂ����͔��肵�Ȃ�</b>���߁A�]���r�ł��邩�\�ߊm�F���Ă����K�v������܂��B
//! @attention ����݂̂����{���܂��B���̔��肪���������� HitZombieAttack()�֐� ���Ăяo���Ă��������B 
bool ObjectManager::CheckZombieAttack(human* MyHuman, human* EnemyHuman)
{
	if( MyHuman == NULL ){ return false; }
	if( EnemyHuman == NULL ){ return false; }

	//�g�p����Ă��Ȃ����A���S���Ă���Ώ������Ȃ��B
	if( MyHuman->GetEnableFlag() == false ){ return false; }
	if( MyHuman->GetHP() <= 0 ){ return false; }
	if( EnemyHuman->GetEnableFlag() == false ){ return false; }
	if( EnemyHuman->GetDeadFlag() == true ){ return false; }

	float mx, my, mz, mrx, tx, ty, tz;
	int mteam, tteam;
	float AttackPoint_x, AttackPoint_y, AttackPoint_z;
	float ax, az;

	MyHuman->GetPosData(&mx, &my, &mz, &mrx);
	MyHuman->GetParamData(NULL, NULL, NULL, &mteam);
	my += VIEW_HEIGHT;
	EnemyHuman->GetPosData(&tx, &ty, &tz, NULL);
	EnemyHuman->GetParamData(NULL, NULL, NULL, &tteam);
	ty += VIEW_HEIGHT;

	//�����Ȃ�Ώ������Ȃ�
	if( mteam == tteam ){ return false; }

	//�U���|�C���g�i�r�̐�[�j�����߂�
	AttackPoint_x = mx + cosf(mrx*-1 + (float)M_PI/2) * 2.0f;
	AttackPoint_y = my + VIEW_HEIGHT - 0.5f;
	AttackPoint_z = mz + sinf(mrx*-1 + (float)M_PI/2) * 2.0f;
	ax = AttackPoint_x - tx;
	az = AttackPoint_z - tz;

	//�G�i�U���Ώہj���U���|�C���g�ɐG��Ă���΁A�������Ă���
	if( (ax*ax + az*az) < 3.3f*3.3f ){
		if( (AttackPoint_y >= ty)&&(AttackPoint_y <= (ty + HUMAN_HEIGHT)) ){
			return true;
		}
	}

	return false;
}

//! @brief �]���r�̍U�����󂯂�����
//! @param MyHuman �U������l�I�u�W�F�N�g�i�]���r���j�̃|�C���^
//! @param EnemyHuman �U�����󂯂��l�I�u�W�F�N�g�̃|�C���^
void ObjectManager::HitZombieAttack(human* MyHuman, human* EnemyHuman)
{
	if( MyHuman == NULL ){ return; }
	if( EnemyHuman == NULL ){ return; }

	//�g�p����Ă��Ȃ����A���S���Ă���Ώ������Ȃ��B
	if( EnemyHuman->GetEnableFlag() == false ){ return; }
	if( EnemyHuman->GetDeadFlag() == true ){ return; }

	int MyHuman_dataID, MyHuman_TeamID;
	int EnemyHuman_dataID, EnemyHuman_TeamID;
	float mx, my, mz;
	float tx, ty, tz;
	float arx;
	int paramid;
	HumanParameter Paraminfo;
	bool NotRobot;
	int hp_old;

	//�]���r���ƍU�����󂯂鑤�`�[���ԍ��擾
	MyHuman->GetParamData(NULL, &MyHuman_dataID, NULL, &MyHuman_TeamID);
	EnemyHuman->GetParamData(NULL, &EnemyHuman_dataID, NULL, &EnemyHuman_TeamID);

	MyHuman->GetPosData(&mx, &my, &mz, NULL);
	EnemyHuman->GetPosData(&tx, &ty, &tz, NULL);
	ty += VIEW_HEIGHT;

	//�G���U�����󂯂��p�x�����߂�
	arx = atan2f(mz - tz, mx - tx);

	//���{�b�g���ǂ�������
	EnemyHuman->GetParamData(&paramid, NULL, NULL, NULL);
	GameParamInfo->GetHuman(paramid, &Paraminfo);
	if( Paraminfo.type == 1 ){
		NotRobot = false;
	}
	else{
		NotRobot = true;
	}

	//�_���[�W�v�Z�O��HP�擾
	hp_old = EnemyHuman->GetHP();

	//�_���[�W�Ȃǂ��v�Z
	EnemyHuman->HitZombieAttack();

#ifdef ENABLE_BUG_TEAMID
	bool flag = true;

	//�`�[���ԍ��������A���`�[���ԍ����傫���Ȃ�A�t���O����
	if( (EnemyHuman_TeamID < 0)&&(MyHuman_TeamID < 0) ){
		if( EnemyHuman_TeamID < MyHuman_TeamID ){
			flag = false;
		}
	}

	if( flag == true ){
		EnemyHuman->SetHitFlag(arx*-1 + (float)M_PI/2);
	}
#else
	EnemyHuman->SetHitFlag(arx*-1 + (float)M_PI/2);
#endif

	//�G�t�F�N�g�i���j��`��
	SetHumanBlood(tx, ty, tz, HUMAN_DAMAGE_ZOMBIEU, NotRobot);

	//���ʉ����Đ�
	GameSound->HitHuman(tx, ty, tz, 3, MyHuman_TeamID);

	//���O�֌W�̏���
	if( (hp_old > 0)&&(EnemyHuman->GetHP() <= 0) ){
		//�_���[�W�v�Z�O�� hp>0 �ł��A�v�Z��� hp <= 0 �Ȃ�A���񎀖S�����B

		int player_teamid;
		HumanIndex[Player_HumanID].GetParamData(NULL, NULL, NULL, &player_teamid);

		ObjectLog->DiedHuman(MyHuman_dataID, EnemyHuman_dataID, MyHuman_TeamID, EnemyHuman_TeamID, player_teamid);
	}
}

//! @brief ���҂�h������
//! @param id �l�̔ԍ��i0�`MAX_HUMAN-1�j
//! @return true�F�����@false�F���s
//! @warning ��Ԃ�̂܂ܑh�����܂�
//! @attention �����Ȑl�ԍ����w�肳�ꂽ�ꍇ��A�w�肵���l���������Ă����ꍇ�A���邢�͓�l�Ԃɑ΂��Ď��s�����ꍇ�A���̊֐��͎��s���܂��B
bool ObjectManager::HumanResuscitation(int id)
{
	if( (id < 0)||(MAX_HUMAN <= id) ){ return false; }

	//�g�p����Ă��Ȃ����A�������Ă���Ώ������Ȃ��B
	if( HumanIndex[id].GetEnableFlag() == false ){ return false; }
	if( HumanIndex[id].GetDeadFlag() == false ){ return false; }

	int id_param, dataid, team;
	signed short int p4;
	HumanIndex[id].GetParamData(&id_param, &dataid, &p4, &team);
	if( (id_param < 0)||( TOTAL_PARAMETERINFO_HUMAN <= id_param) ){ return false; }	//��l�ԂȂ珈�����Ȃ�
	HumanIndex[id].SetParamData(id_param, dataid, p4, team, true);

	//���O�֌W�̏���
	int player_teamid;
	HumanIndex[Player_HumanID].GetParamData(NULL, NULL, NULL, &player_teamid);
	ObjectLog->ReviveHuman(id, team, player_teamid);

	return true;
}

//! @brief �Q�[���N���A�[�E�Q�[���I�[�o�[�̔���
//! @return �Q�[���N���A�[�F1�@�Q�[���I�[�o�[�F2�@����Ȃ��F0
//! @attention �Q�[���N���A�\�ƃQ�[���I�[�o�[�������ɐ�����������ł́A�{��XOPS�Ɠ��l�Ɂu�Q�[���N���A�[�v�Ɣ��肳��܂��B
int ObjectManager::CheckGameOverorComplete()
{
	//�����F
	//
	//�@�{���́A�Q�[���I�[�o�[������ɍs���A���ɃQ�[���N���A�[����������������������I�ł��B
	//�@�������A�{��XOPS�̓Q�[���N���A�[�̔����D�悷�邽�߁A��ɃQ�[���N���A�[�̔�����s���Ă��܂��B
	//�@�����Ă���G�̐����i��HP�Ƃ��āj�����A�G�̐��i��HP�j�� 0 �Ȃ�΁A�Q�[���N���A�[�Ɣ��肵�܂��B

	//�Q�[���N���A�[����
	int MyTeamid, teamid;
	int TotalEnemyHP = 0;
	HumanIndex[Player_HumanID].GetParamData(NULL, NULL, NULL, &MyTeamid);	//�v���C���[�̃`�[���ԍ����擾
	for(int i=0; i<MAX_HUMAN; i++){
		//����������Ă��Ȃ���Ώ������Ȃ�
		if( HumanIndex[i].GetEnableFlag() == false ){ continue; }

		//���ׂ�Ώۂ̃`�[���ԍ����擾
		HumanIndex[i].GetParamData(NULL, NULL, NULL, &teamid);

		//�قȂ�`�[���ԍ��i���G�j�Ȃ��
		if( teamid != MyTeamid ){
			//�����Ă���΁A�G��HP�Ƃ��ĉ��Z
			//if( HumanIndex[i].GetDeadFlag() == false ){
			if( HumanIndex[i].GetHP() > 0 ){
				TotalEnemyHP += HumanIndex[i].GetHP();
			}
		}
	}
	if( TotalEnemyHP == 0 ){	//�S�G��HP�� 0 �Ȃ��
		return 1;
	}


	//�Q�[���I�[�o�[����
	if( HumanIndex[Player_HumanID].GetEnableFlag() == true ){	//����Ώۂ��L���Ȃ�΁@�i���F���Z�ɂ��ύX�΍�j
		if( HumanIndex[Player_HumanID].GetDeadFlag() == true ){		//�v���C���[�����S���Ă����
			return 2;
		}
	}

	return 0;
}

//! @brief �I�u�W�F�N�g�̏����擾
//! @param camera_x �J������X���W
//! @param camera_y �J������Y���W
//! @param camera_z �J������Z���W
//! @param camera_rx �J�����̉����p�x
//! @param camera_ry �J�����̏c���p�x
//! @param color �F���擾����|�C���^
//! @param infostr �������擾����|�C���^
//! @return �\����񂠂�Ftrue�@�\�����Ȃ��Ffalse
bool ObjectManager::GetObjectInfoTag(float camera_x, float camera_y, float camera_z, float camera_rx, float camera_ry, int *color, char *infostr)
{
	if( color == NULL ){ return false; }
	if( infostr == NULL ){ return false; }

	float dist = 50.0f;
	float px, py, pz;
	float rx, ry;
	float r;
	int Player_teamID;

	//������������
	infostr[0] = '\0';

	//�v���C���[�̃`�[���ԍ����擾
	HumanIndex[Player_HumanID].GetParamData(NULL, NULL, NULL, &Player_teamID);

	//�l
	for(int i=0; i<MAX_HUMAN; i++){
		//�v���C���[���g�Ȃ珈�����Ȃ�
		if( i == Player_HumanID ){ continue; }

		if( HumanIndex[i].GetEnableFlag() == true ){
			HumanIndex[i].GetPosData(&px, &py, &pz, NULL);

			//���_����ɑΏۂ܂ł̊p�x���Z�o
			if( CheckTargetAngle(camera_x, camera_y, camera_z, camera_rx, 0.0f, px, py, pz, dist, &rx, NULL, &r) == true ){
				int team;

				//�p�x��A���E�ɓ����Ă����
				if( fabsf(rx) < DegreeToRadian(10) ){
					HumanIndex[i].GetParamData(NULL, NULL, NULL, &team);
					if( team == Player_teamID ){
						*color = d3dg->GetColorCode(0.0f,0.0f,1.0f,1.0f);
						sprintf(infostr, "Human[%d]  HP %d : Friend", i, HumanIndex[i].GetHP());
					}
					else{
						*color = d3dg->GetColorCode(1.0f,0.0f,0.0f,1.0f);
						sprintf(infostr, "Human[%d]  HP %d : Enemy", i, HumanIndex[i].GetHP());
					}
					dist = sqrtf(r);
				}
			}
		}
	}

	//����
	for(int i=0; i<MAX_WEAPON; i++){
		if( (WeaponIndex[i].GetEnableFlag() == true)&&(WeaponIndex[i].GetUsingFlag() == false) ){
			int lnbs, nbs;

			WeaponIndex[i].GetPosData(&px, &py, &pz, NULL);
			WeaponIndex[i].GetParamData(NULL, &lnbs, &nbs);

			//���_����ɑΏۂ܂ł̊p�x���Z�o
			if( CheckTargetAngle(camera_x, camera_y, camera_z, camera_rx, camera_ry, px, py, pz, dist, &rx, &ry, &r) == true ){
				//�p�x��A���E�ɓ����Ă����
				if( (fabsf(rx) < DegreeToRadian(10))&&(fabsf(ry) < DegreeToRadian(10)) ){
					*color = d3dg->GetColorCode(0.0f,1.0f,0.0f,1.0f);
					sprintf(infostr, "Weapon[%d]  %d:%d", i, lnbs, (nbs - lnbs));
					dist = sqrtf(r);
				}
			}
		}
	}

	//����
	for(int i=0; i<MAX_SMALLOBJECT; i++){
		if( SmallObjectIndex[i].GetEnableFlag() == true ){
			SmallObjectIndex[i].GetPosData(&px, &py, &pz, NULL);
			
			//���_����ɑΏۂ܂ł̊p�x���Z�o
			if( CheckTargetAngle(camera_x, camera_y, camera_z, camera_rx, camera_ry, px, py, pz, dist, &rx, &ry, &r) == true ){
				//�p�x��A���E�ɓ����Ă����
				if( (fabsf(rx) < DegreeToRadian(10))&&(fabsf(ry) < DegreeToRadian(10)) ){
					*color = d3dg->GetColorCode(1.0f,1.0f,0.0f,1.0f);
					sprintf(infostr, "SmallObject[%d]  HP %d", i, SmallObjectIndex[i].GetHP());
					dist = sqrtf(r);
				}
			}
		}
	}

	//�������ݒ肳��Ă��邩�ǂ����ŁA�L���ȏ��������ꂽ������B
	if( infostr[0] == NULL ){
		return false;
	}
	return true;
}

//! @brief �I�u�W�F�N�g�̎�v�Z����
//! @param cmdF5id �㏸�@�\�iF5���Z�j������l�f�[�^�ԍ��i-1�ŋ@�\�����j
//! @param demomode �f�����[�h
//! @param screen ��ʂ��Â�����
//! @return ��� 0
//! @attention ��ʓI�� cmdF5id �́AF5���Z�g�p���̓v���C���[�ԍ��iGetPlayerID()�֐��Ŏ擾�j�A���g�p���� -1 ���w�肵�܂��B
//! @attention demomode �͎�Ƀ��j���[��ʂŎg�p���܂��B�L���ɂ���ƁA�e�e�E��֒e���������܂���B
int ObjectManager::Process(int cmdF5id, bool demomode, bool screen)
{
	//���̃t���[���̐����������
	for(int i=0; i<MAX_HUMAN; i++){
		Human_ontarget[i] = 0.0f;
		Human_kill[i] = 0;
		Human_headshot[i] = 0;
	}

	//�l�I�u�W�F�N�g�̏���
	for(int i=0; i<MAX_HUMAN; i++){
		bool cmdF5, player;
		int rtn;
		int block_id = -1;
		int block_face = -1;
		int block_textureid = -1;
		int MoveMode;
		int JumpLandingMode;

		//�v���C���[���ǂ�������
		if( Player_HumanID == i ){
			player = true;
		}
		else{
			player = false;
		}

		if( i == cmdF5id ){
			cmdF5 = true;
		}
		else{
			cmdF5 = false;
		}

		rtn = HumanIndex[i].ProcessObject(CollD, BlockData, AddCollisionFlag, player, cmdF5);
		if( rtn == 2 ){
			//���S���̃G�t�F�N�g
			DeadEffect(&(HumanIndex[i]));
		}
		if( rtn == 4 ){
			int teamid, player_teamid;
			HumanIndex[i].GetParamData(NULL, NULL, NULL, &teamid);
			HumanIndex[Player_HumanID].GetParamData(NULL, NULL, NULL, &player_teamid);

			//���O�֌W�̏���
			ObjectLog->DiedHuman(-1, i, -1, teamid, player_teamid);
		}

		HumanIndex[i].SetDarkModelFlag(screen);

		//�����ɂ���u���b�N�̃e�N�X�`���[�ԍ����擾����
		HumanIndex[i].GetUnderBlock(&block_id, &block_face);
		if( block_id != -1 ){
			blockdata bdata;
			BlockData->Getdata(&bdata, block_id);
			block_textureid = bdata.material[block_face].textureID;
		}
		else{
			block_textureid = -1;
		}

		MoveMode = HumanIndex[i].GetMovemode(false);
		JumpLandingMode = HumanIndex[i].GetJumpLanding(false);

		//����
		if( (1 <= MoveMode)&&(MoveMode <= 4) ){
			//�����ǉ�
			//�@�������ɂ���u���b�N�̃e�N�X�`���[�ԍ����A�����̎�ޔԍ��Ƃ���B
			float posx, posy, posz;
			int teamid;
			int cnt;
			HumanIndex[i].GetPosData(&posx, &posy, &posz, NULL);
			HumanIndex[i].GetParamData(NULL, NULL, NULL, &teamid);
			cnt = HumanIndex[i].GetMoveMotionCount();
			GameSound->SetFootsteps(posx, posy, posz, block_textureid, MoveMode - 1, teamid, cnt);
		}

		//�W�����v���鉹
		if( JumpLandingMode == 1 ){
			//�����ǉ�
			float posx, posy, posz;
			int teamid;
			HumanIndex[i].GetPosData(&posx, &posy, &posz, NULL);
			HumanIndex[i].GetParamData(NULL, NULL, NULL, &teamid);
			GameSound->SetFootsteps(posx, posy, posz, block_textureid, 4, teamid, 0);
		}

		//���n���鉹
		if( JumpLandingMode == 2 ){
			//�����ǉ�
			float posx, posy, posz;
			int teamid;
			HumanIndex[i].GetPosData(&posx, &posy, &posz, NULL);
			HumanIndex[i].GetParamData(NULL, NULL, NULL, &teamid);
			GameSound->SetFootsteps(posx, posy, posz, block_textureid, 5, teamid, 0);
		}
	}

	//����I�u�W�F�N�g�̏���
	for(int i=0; i<MAX_WEAPON; i++){
		WeaponIndex[i].ProcessObject(CollD);
		WeaponIndex[i].SetDarkModelFlag(screen);
	}

	//�����I�u�W�F�N�g�̏���
	for(int i=0; i<MAX_SMALLOBJECT; i++){
		SmallObjectIndex[i].ProcessObject();
		SmallObjectIndex[i].SetDarkModelFlag(screen);
	}

	if( demomode == false ){
		//�e�I�u�W�F�N�g�̏���
		for(int i=0; i<MAX_BULLET; i++){
			float bx, by, bz, brx, bry, speed;
			int teamid;
			float mx, my, mz;

			CollideBullet(&BulletIndex[i]);		//�����蔻������s
			BulletIndex[i].ProcessObject();		//��v�Z
			BulletIndex[i].SetDarkModelFlag(screen);

			if( BulletIndex[i].GetEnableFlag() == true ){
				//�e�̍��W�Ɗp�x���擾
				BulletIndex[i].GetParamData(NULL, NULL, &speed, &teamid, NULL, NULL);
				BulletIndex[i].GetPosData(&bx, &by, &bz, &brx, &bry);
				mx = cosf(brx)*cosf(bry)*speed;
				my = sinf(bry)*speed;
				mz = sinf(brx)*cosf(bry)*speed;
				GameSound->PassingBullet(bx, by, bz, mx, my, mz, teamid);
			}
		}
	}
	else{
		//�S�Ă̒e�I�u�W�F�N�g������
		for(int i=0; i<MAX_BULLET; i++){
			if( BulletIndex[i].GetEnableFlag() == true ){
				BulletIndex[i].SetEnableFlag(false);
			}
		}
	}

	//����𔭖C�����ۂ̃G�t�F�N�g���o��
	for(int i=0; i<MAX_HUMAN; i++){
		if( Human_ShotFlag[i] == true ){
			ShotWeaponEffect(i);
			Human_ShotFlag[i] = false;
		}

		if( Human_YakkyouCnt[i] == 0 ){
			ShotWeaponYakkyou(i);
		}
		if( Human_YakkyouCnt[i] > -1 ){ Human_YakkyouCnt[i] -= 1; }
	}

	//�G�t�F�N�g�I�u�W�F�N�g�̏���
	for(int i=0; i<MAX_EFFECT; i++){
		int id, face;
		float pos_x, pos_y, pos_z;
		if( CollideBlood(&(EffectIndex[i]), &id, &face, &pos_x, &pos_y, &pos_z) == true ){
			AddMapEffect(id, face, pos_x, pos_y, pos_z, 10.0f, DegreeToRadian(10)*GetRand(18), Resource->GetEffectBloodTexture(),  0, 0, 1.0f, -0.002f, (int)GAMEFPS * 20);
			EffectIndex[i].SetEnableFlag(false);
		}
		else{
			EffectIndex[i].ProcessObject();
			EffectIndex[i].SetDarkModelFlag(screen);
		}
	}

	if( demomode == false ){
		//��֒e�̏���
		for(int i=0; i<MAX_GRENADE; i++){
			float speed = GrenadeIndex[i].GetSpeed();

			//��v�Z
			int rcr = GrenadeIndex[i].ProcessObject(CollD);
			GrenadeIndex[i].SetDarkModelFlag(screen);

			//�o�E���h�E���˕Ԃ����Ȃ��
			if( rcr == 1 ){
				if( speed > 3.4f ){
					//���W���擾���A���ʉ��Đ�
					float x, y, z;
					int teamid;
					GrenadeIndex[i].GetPosData(&x, &y, &z, NULL, NULL);
					GrenadeIndex[i].GetParamData(NULL, &teamid, NULL, NULL);
					GameSound->GrenadeBound(x, y, z, teamid);
				}
			}

			//���������Ȃ�
			if( rcr == 2 ){
				GrenadeExplosion(&(GrenadeIndex[i]));
			}
		}
	}
	else{
		//�S�Ă̎�֒e������
		for(int i=0; i<MAX_GRENADE; i++){
			if( GrenadeIndex[i].GetEnableFlag() == true ){
				GrenadeIndex[i].SetEnableFlag(false);
			}
		}
	}


	//������E������
	for(int i=0; i<MAX_HUMAN; i++){
		if( HumanIndex[i].GetEnableFlag() == true ){
			//������E�������Ŗ��t���[����������ŏ�������Ӗ��͂Ȃ��̂ŁA�e����2�t���[����1�񏈗��ɃP�`���Ă܂��@�i�O�O�G
			//�t���[�����������ڂ̎��́A�f�[�^�ԍ��������̕���B��̎��́A��̕���������B
			for(int j=(framecnt%2); j<MAX_WEAPON; j+=2){
				PickupWeapon(&HumanIndex[i], &WeaponIndex[j]);
			}
		}
	}

	for(int i=0; i<MAX_HUMAN; i++){
		if( HumanIndex[i].GetEnableFlag() == true ){
			for(int j=i+1; j<MAX_HUMAN; j++){
				if( HumanIndex[j].GetEnableFlag() == true ){

					//�l���m�̓����蔻��i���������j
					CollideHuman(&HumanIndex[i], &HumanIndex[j]);

				}
			}
		}
	}

	//���O�֌W�̏���
	ObjectLog->Process();

	framecnt += 1;

	return 0;
}

//! @brief ���t���[���� �������E�|�����G�̐��E�w�b�g�V���b�g�� �̎擾
//! @param id �擾����l�̃f�[�^�ԍ�
//! @param ontarget ���������󂯎��|�C���^
//! @param kill �|�����G�̐����󂯎��|�C���^
//! @param headshot �G�̓����ɖ������������󂯎��|�C���^
//! @return �����Ftrue�@���s�Ffalse
bool ObjectManager::GetHumanShotInfo(int id, float *ontarget, int *kill, int *headshot)
{
	if( (id < 0)||(MAX_HUMAN <= id) ){ return false; }
	if( (ontarget == NULL)||(kill == NULL)||(headshot == NULL) ){ return false; }

	*ontarget = Human_ontarget[id];
	*kill = Human_kill[id];
	*headshot = Human_headshot[id];
	return true;
}

//! @brief �G�t�F�N�g���\�[�g����
//! @param camera_x �J������X���W
//! @param camera_y �J������Y���W
//! @param camera_z �J������Z���W
//! @param data ���ʂ��󂯎�� effectdata�\���́@�i�v�f���FMAX_EFFECT�j
//! @return �L���ȃG�t�F�N�g�̐�
int ObjectManager::SortEffect(float camera_x, float camera_y, float camera_z, effectdata data[])
{
	int cnt = 0;

	//�J��������̋��������߂A�����𐔂���B
	for(int i=0; i<MAX_EFFECT; i++){
		if( EffectIndex[i].GetEnableFlag() == true ){
			float ex, ey, ez;
			float x, y, z;

			//���W���擾���A�������v�Z�A�f�[�^�ɓo�^�B
			EffectIndex[i].GetPosData(&ex, &ey, &ez, NULL);
			x = ex - camera_x;
			y = ey - camera_y;
			z = ez - camera_z;
			data[cnt].id = i;
			data[cnt].dist = x*x + y*y + z*z;

			//�J�E���g�����Z
			cnt += 1;
		}
	}

	//�P���}���\�[�g
	effectdata temp;
	for(int i=1; i<cnt; i++) {
		int j;
		temp = data[i];
		for(j=i; j>0 && data[j-1].dist<temp.dist; j--){
			data[j] = data[j-1];
		}
		data[j] = temp;
	}

	return cnt;
}

//! @brief �I�u�W�F�N�g�̕`�揈��
//! @param camera_x �J������X���W
//! @param camera_y �J������Y���W
//! @param camera_z �J������Z���W
//! @param camera_rx �J�����̉����p�x
//! @param camera_ry �J�����̏c���p�x
//! @param HidePlayer �v���C���[�̕`��ݒ�@�i�`�悷��F0�@�`�悵�Ȃ��F1�@�r�ƕ���̂ݕ`�悷��F2�j
//! @param NoModel ���f���`�斳����
void ObjectManager::Render(float camera_x, float camera_y, float camera_z, float camera_rx, float camera_ry, int HidePlayer, bool NoModel)
{
	//���[���h���W�����_��
	d3dg->ResetWorldTransform();

	//�l�`��
	for(int i=0; i<MAX_HUMAN; i++){
		bool DrawArm, player;

		//�l�̃e�N�X�`�����t���b�V���t���O���L���Ȃ�
		if( Human_FrameTextureRefresh[i] == true ){
			if( i > 0 ){
				//�O�̐l�������Ă��镐����擾
				int WeaponType = HumanIndex[i-1].GetMainWeaponTypeNO();

				if( WeaponType != ID_WEAPON_NONE ){
					int model, texture;

					//����������Ă���΁A���̕���̃e�N�X�`����K�p
					Resource->GetWeaponModelTexture(WeaponType, &model, &texture);
					HumanIndex[i].SetTexture(texture);
				}
				else{
					//��Ԃ�Ȃ�A�l�̃e�N�X�`�������̂܂ܔ��f
					HumanIndex[i].SetTexture( HumanIndex[i-1].GetTexture() );
				}
			}
		}

		//�r�̕`��
		if( HidePlayer == 0 ){
			DrawArm = false;
		}
		else if( Player_HumanID != i ){	// HidePlayer != 0
			DrawArm = false;
		}
		else if( HidePlayer == 2 ){		// Player_HumanID == i
			DrawArm = true;
		}
		else{							// Player_HumanID == i && HidePlayer == 1
			continue;	//�v���C���[���g��`�悵�Ȃ��Ȃ�X�L�b�v
		}

		//�v���C���[���ǂ�������
		if( Player_HumanID == i ){
			player = true;
		}
		else{
			player = false;
		}

		HumanIndex[i].Render(d3dg, Resource, DrawArm, player, NoModel);

		/*
		//�����蔻��̊ȈՕ\��
		float x, y, z;
		HumanIndex[i].GetPosData(&x, &y, &z, NULL);
		if( HumanIndex[i].GetHP() > 0 ){
			d3dg->SetWorldTransform(x, y, z, 0.0f, 0.0f, 1.0f);
			d3dg->Renderline(HUMAN_BULLETCOLLISION_HEAD_R, HUMAN_BULLETCOLLISION_LEG_H + HUMAN_BULLETCOLLISION_UP_H + HUMAN_BULLETCOLLISION_HEAD_H, 0.0f, HUMAN_BULLETCOLLISION_HEAD_R * -1, HUMAN_BULLETCOLLISION_LEG_H + HUMAN_BULLETCOLLISION_UP_H + HUMAN_BULLETCOLLISION_HEAD_H, 0.0f);
			d3dg->Renderline(0.0f, HUMAN_BULLETCOLLISION_LEG_H + HUMAN_BULLETCOLLISION_UP_H + HUMAN_BULLETCOLLISION_HEAD_H, HUMAN_BULLETCOLLISION_HEAD_R, 0.0f, HUMAN_BULLETCOLLISION_LEG_H + HUMAN_BULLETCOLLISION_UP_H + HUMAN_BULLETCOLLISION_HEAD_H, HUMAN_BULLETCOLLISION_HEAD_R * -1);
			d3dg->Renderline(HUMAN_BULLETCOLLISION_UP_R, HUMAN_BULLETCOLLISION_LEG_H + HUMAN_BULLETCOLLISION_UP_H, 0.0f, HUMAN_BULLETCOLLISION_UP_R * -1, HUMAN_BULLETCOLLISION_LEG_H + HUMAN_BULLETCOLLISION_UP_H, 0.0f);
			d3dg->Renderline(0.0f, HUMAN_BULLETCOLLISION_LEG_H + HUMAN_BULLETCOLLISION_UP_H, HUMAN_BULLETCOLLISION_UP_R, 0.0f, HUMAN_BULLETCOLLISION_LEG_H + HUMAN_BULLETCOLLISION_UP_H, HUMAN_BULLETCOLLISION_UP_R * -1);
			d3dg->Renderline(HUMAN_BULLETCOLLISION_LEG_R, HUMAN_BULLETCOLLISION_LEG_H, 0.0f, HUMAN_BULLETCOLLISION_LEG_R * -1, HUMAN_BULLETCOLLISION_LEG_H, 0.0f);
			d3dg->Renderline(0.0f, HUMAN_BULLETCOLLISION_LEG_H, HUMAN_BULLETCOLLISION_LEG_R, 0.0f, HUMAN_BULLETCOLLISION_LEG_H, HUMAN_BULLETCOLLISION_LEG_R * -1);
		}
		*/
	}

	//����`��
	for(int i=0; i<MAX_WEAPON; i++){
		WeaponIndex[i].Render(d3dg, NoModel);
	}

	//�����`��
	for(int i=0; i<MAX_SMALLOBJECT; i++){
		SmallObjectIndex[i].Render(d3dg, NoModel);
	}

	//�e�`��
	for(int i=0; i<MAX_BULLET; i++){
		BulletIndex[i].Render(d3dg, NoModel);
	}

	//��֒e�`��
	for(int i=0; i<MAX_GRENADE; i++){
		GrenadeIndex[i].Render(d3dg, NoModel);
	}

	//�G�t�F�N�g�`��
	d3dg->StartEffectRender();
	/*
	for(int i=0; i<MAX_EFFECT; i++){
		EffectIndex[i].Render(d3dg, camera_rx, camera_ry, NoModel);
	}
	*/
	effectdata data[MAX_EFFECT];
	int cnt = SortEffect(camera_x, camera_y, camera_z, data);
	for(int i=0; i<cnt; i++) {
		EffectIndex[ data[i].id ].Render(d3dg, camera_rx, camera_ry, NoModel);
	}
	d3dg->EndEffectRender();
}

//! @brief �I�u�W�F�N�g���O�̕`�揈��
//! @param x ����� x���W
//! @param y ����� y���W
void ObjectManager::RenderLog(int x, int y)
{
	ObjectLog->Render(x, y);
}

//! @brief �f�[�^�̉��
void ObjectManager::Cleanup()
{
	//�|�C���g�f�[�^���
	CleanupPointDataToObject();
}

//! @brief �R���X�g���N�^
BulletObjectHumanIndex::BulletObjectHumanIndex()
{
	HumanIndex = new bool[MAX_HUMAN];

	Init();
}

//! @brief �f�B�X�g���N�^
BulletObjectHumanIndex::~BulletObjectHumanIndex()
{
	if( HumanIndex != NULL ){ delete [] HumanIndex; }
}

//! @brief ������
void BulletObjectHumanIndex::Init()
{
	for(int i=0; i<MAX_HUMAN; i++){
		HumanIndex[i] = false;
	}
}

//! @brief �t���O���擾
//! @param id �l�̃f�[�^�ԍ�
//! @return �t���O�l
bool BulletObjectHumanIndex::GetIndexFlag(int id)
{
	if( (id < 0)||(MAX_HUMAN <= id) ){ return false; }
	return HumanIndex[id];
}

//! @brief �t���O��L����
//! @param id �l�̃f�[�^�ԍ�
void BulletObjectHumanIndex::SetIndexFlag(int id)
{
	if( (id < 0)||(MAX_HUMAN <= id) ){ return; }
	HumanIndex[id] = true;
}

//! @brief �R���X�g���N�^
ObjectManagerLog::ObjectManagerLog()
{
	d3dg = NULL;
	for(int i=0; i<MAX_OBJECTMANAGER_LOGLEN; i++){
		TextStr[i] = new char [MAX_OBJECTMANAGER_LOGLINES];
	}
	ClearLog();
}

//! @brief �f�B�X�g���N�^
ObjectManagerLog::~ObjectManagerLog()
{
	for(int i=0; i<MAX_OBJECTMANAGER_LOGLEN; i++){
		delete [] TextStr[i];
	}
}

//! @brief �Q�Ƃ���N���X��ݒ�
//! @param in_d3dg �`�揈���N���X
//! @attention ���̊֐��Őݒ���s��Ȃ��ƁA�N���X���̂��������@�\���܂���B
void ObjectManagerLog::SetClass(D3DGraphics *in_d3dg)
{
	d3dg = in_d3dg;
}

//! @brief �I�u�W�F�N�g���O��������
void ObjectManagerLog::ClearLog()
{
	for(int i=0; i<MAX_OBJECTMANAGER_LOGLEN; i++){
		TextCnt[i] = -1;
		TextStr[i][0] = '\0';
		TextColor[i] = 0;
	}
}

//! @brief ����ǋL
//! @param str ������@�i���s�R�[�h�F<b>�s��</b>�j
void ObjectManagerLog::InfoLog(const char *str)
{
	if( str == NULL ){ return; }
	AddTextLog(MAX_OBJECTMANAGER_LOGCNT, str, d3dg->GetColorCode(0.0f,1.0f,0.0f,1.0f));
}

//! @brief �l��ǉ�����
//! @param humanID �l��ID
//! @param TeamID �`�[���ԍ�
//! @param PlayerTeamID �v���C���[�̃`�[���ԍ�
void ObjectManagerLog::AddHuman(int humanID, int TeamID, int PlayerTeamID)
{
	char str[64];
	int str_color;

	//�����̐F������
	if( PlayerTeamID == TeamID ){ str_color = d3dg->GetColorCode(0.0f,1.0f,1.0f,1.0f); }
	else{ str_color = d3dg->GetColorCode(1.0f,0.5f,0.0f,1.0f); }

	sprintf(str, "Added human[%d]", humanID);

	AddTextLog(MAX_OBJECTMANAGER_LOGCNT, str, str_color);
}

//! @brief �l�����S����
//! @param ShothumanID  �U���������l��ID
//! @param HitHumanID �U�����󂯂��l��ID
//! @param ShothumanTeamID �U���������l�̃`�[���ԍ�
//! @param HitHumanTeamID �U�����󂯂��l�̃`�[���ԍ�
//! @param PlayerTeamID �v���C���[�̃`�[���ԍ�
void ObjectManagerLog::DiedHuman(int ShothumanID, int HitHumanID, int ShothumanTeamID, int HitHumanTeamID, int PlayerTeamID)
{
	char str[64];
	int str_color;

	//�����̐F������
	if( PlayerTeamID == HitHumanTeamID ){ str_color = d3dg->GetColorCode(1.0f,0.5f,0.0f,1.0f); }
	else{ str_color = d3dg->GetColorCode(0.0f,1.0f,1.0f,1.0f); }

	if( (ShothumanID == -1)||(ShothumanID == HitHumanID) ){
		sprintf(str, "human[%d] died", HitHumanID);
	}
	else{
		sprintf(str, "human[%d] killed human[%d]", ShothumanID, HitHumanID);

		//TK����
		if( ShothumanTeamID == HitHumanTeamID ){
			strcat(str, " (TK)");
		}
	}

	AddTextLog(MAX_OBJECTMANAGER_LOGCNT, str, str_color);
}

//! @brief �l���h������
//! @param humanID �l��ID
//! @param TeamID �`�[���ԍ�
//! @param PlayerTeamID �v���C���[�̃`�[���ԍ�
void ObjectManagerLog::ReviveHuman(int humanID, int TeamID, int PlayerTeamID)
{
	char str[64];
	int str_color;

	//�����̐F������
	if( PlayerTeamID == TeamID ){ str_color = d3dg->GetColorCode(0.0f,1.0f,1.0f,1.0f); }
	else{ str_color = d3dg->GetColorCode(1.0f,0.5f,0.0f,1.0f); }

	sprintf(str, "Revived human[%d]", humanID);

	AddTextLog(MAX_OBJECTMANAGER_LOGCNT, str, str_color);
}

//! @brief ������ǉ�����
//! @param objID ������ID
void ObjectManagerLog::AddSmallObject(int objID)
{
	char str[64];
	sprintf(str, "Added SmallObject[%d]", objID);
	AddTextLog(MAX_OBJECTMANAGER_LOGCNT, str, d3dg->GetColorCode(1.0f,1.0f,0.0f,1.0f));
}

//! @brief ������j�󂵂�
//! @param objID ������ID
void ObjectManagerLog::BreakSmallObject(int objID)
{
	char str[64];
	sprintf(str, "Breaked SmallObject[%d]", objID);
	AddTextLog(MAX_OBJECTMANAGER_LOGCNT, str, d3dg->GetColorCode(1.0f,1.0f,0.0f,1.0f));
}

//! @brief �I�u�W�F�N�g���O�ǉ�
//! @param cnt �`��t���[����
//! @param str ������@�i���s�R�[�h�F<b>�s��</b>�j
//! @param color �F
//! @return 1�s�㏑���Ftrue�@�ǉ��̂݁Ffalse
bool ObjectManagerLog::AddTextLog(int cnt, const char *str, int color)
{
	if( str == NULL ){ return false; }

	//�󂢂Ă���s������Ȃ�A���̍s�ɏ����ďI��
	for(int i=0; i<MAX_OBJECTMANAGER_LOGLEN; i++){
		if( TextCnt[i] == -1 ){
			TextCnt[i] = cnt;
			strcpy(TextStr[i], str);
			TextColor[i] = color;
			return false;
		}
	}

	//�󂢂Ă���s���Ȃ���΁A1�s�Âl�߂�
	for(int i=0; i<MAX_OBJECTMANAGER_LOGLEN-1; i++){
		TextCnt[i] = TextCnt[i+1];
		strcpy(TextStr[i], TextStr[i+1]);
		TextColor[i] = TextColor[i+1];
	}

	//�Ō�̍s�ɏ���
	TextCnt[MAX_OBJECTMANAGER_LOGLEN-1] = cnt;
	strcpy(TextStr[MAX_OBJECTMANAGER_LOGLEN-1], str);
	TextColor[MAX_OBJECTMANAGER_LOGLEN-1] = color;
	return true;
}

//! @brief �I�u�W�F�N�g���O�̎�v�Z����
void ObjectManagerLog::Process()
{
	//�e�s���ƂɃJ�E���g����
	for(int i=0; i<MAX_OBJECTMANAGER_LOGLEN; i++){
		if( TextCnt[i] == 0 ){
			TextCnt[i] = -1;
			TextStr[i][0] = '\0';
			TextColor[i] = 0;
		}
		else if( TextCnt[i] > 0 ){
			TextCnt[i] -= 1;
		}
	}

	//�[���J�E���g�̍s���l�߂�
	for(int i=0; i<MAX_OBJECTMANAGER_LOGLEN-1; i++){
		if( TextCnt[i] == 0 ){
			for(int j=i; j<MAX_OBJECTMANAGER_LOGLEN-1; j++){
				TextCnt[j] = TextCnt[j+1];
				strcpy(TextStr[j], TextStr[j+1]);
				TextColor[j] = TextColor[j+1];
			}

			//�Ō�̍s�͋�s��
			TextCnt[MAX_OBJECTMANAGER_LOGLEN-1] = -1;
			TextStr[MAX_OBJECTMANAGER_LOGLEN-1][0] = '\0';
			TextColor[MAX_OBJECTMANAGER_LOGLEN-1] = 0;
		}
	}
}

//! @brief �I�u�W�F�N�g���O�̕`�揈��
//! @param x ����� x���W
//! @param y ����� y���W
//! @attention �ォ��t���ŕ`�悳��܂��B
void ObjectManagerLog::Render(int x, int y)
{
#ifdef ENABLE_DEBUGCONSOLE
	if( d3dg == NULL ){ return; }

	for(int i=MAX_OBJECTMANAGER_LOGLEN-1; i>=0; i--){
		if( TextCnt[i] >= 0 ){
			d3dg->Draw2DTextureDebugFontText(x+2+1, y+2+1, TextStr[i], d3dg->GetColorCode(0.1f,0.1f,0.1f,1.0f));
			d3dg->Draw2DTextureDebugFontText(x+2, y+2, TextStr[i], TextColor[i]);
		}
		y += 20;
	}
#endif
}