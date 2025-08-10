//! @file resource.cpp
//! @brief ResourceManager�N���X�̒�`

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

#include "resource.h"

//! @brief �R���X�g���N�^
ResourceManager::ResourceManager()
{
	ParamInfo = NULL;
	d3dg = NULL;
	SoundCtrl = NULL;

	for(int i=0; i<TOTAL_UPMODE; i++){
		human_upmodel[i] = -1;
	}
	for(int i=0; i<TOTAL_ARMMODE; i++){
		human_armmodel[i] = -1;
	}
	human_legmodel = -1;
	for(int i=0; i<TOTAL_WALKMODE; i++){
		human_walkmodel[i] = -1;
	}
	for(int i=0; i<TOTAL_RUNMODE; i++){
		human_runmodel[i] = -1;
	}
	for(int i=0; i<MAX_LOADHUMANTEXTURE; i++){
		human_texture_Param[i] = -1;
		human_texture_d3dg[i] = -1;
	}
	for(int i=0; i<TOTAL_PARAMETERINFO_WEAPON; i++){
		weapon_model[i] = -1;
		weapon_texture[i] = -1;
		weapon_sound[i] = -1;
	}
	weapon_reloadsound = -1;
	for(int i=0; i<(TOTAL_PARAMETERINFO_SMALLOBJECT + MAX_ADDSMALLOBJECT); i++){
		smallobject_model[i] = -1;
		smallobject_texture[i] = -1;
		smallobject_sound[i] = -1;
	}
	for(int i=0; i<TOTAL_PARAMETERINFO_BULLET; i++){
		bullet_model[i] = -1;
		bullet_texture[i] = -1;
	}

	scopetexture = -1;
	skymodel = -1;
	skytexture = -1;
	bullet_hitsoundA = -1;
	bullet_hitsoundB = -1;
	bullet_humanhitsound = -1;
	bullet_passingsound = -1;
	grenade_bang = -1;
	grenade_cco = -1;
	effecttexture_blood = -1;
	effecttexture_mflash = -1;
	effecttexture_smoke = -1;
	effecttexture_yakkyou = -1;
}

//! @brief �f�B�X�g���N�^
ResourceManager::~ResourceManager()
{
	DestroyResource();
}

//! @brief �Q�Ƃ���ParameterInfo�N���X��ݒ�
//! @param *_ParamInfo ParameterInfo�N���X�̃|�C���^
//! @attention ���̊֐��Őݒ���s��Ȃ��ƁA�N���X���̂��������@�\���܂���B
void ResourceManager::SetParameterInfo(ParameterInfo *_ParamInfo)
{
	ParamInfo = _ParamInfo;
}

//! @brief �Q�Ƃ���D3DGraphics�N���X��ݒ�
//! @param *_d3dg D3DGraphics�N���X�̃|�C���^
//! @attention ���̊֐��Őݒ���s��Ȃ��ƁA�N���X���̂��������@�\���܂���B
void ResourceManager::SetD3DGraphics(D3DGraphics *_d3dg)
{
	d3dg = _d3dg;
}

//! @brief �Q�Ƃ���SoundControl�N���X��ݒ�
//! @param *_SoundCtrl SoundControl�N���X�̃|�C���^
//! @attention ���̊֐��Őݒ���s��Ȃ��ƁA�N���X���̂��������@�\���܂���B
void ResourceManager::SetSoundControl(SoundControl *_SoundCtrl)
{
	SoundCtrl = _SoundCtrl;
}

//! @brief ���
//! @attention �{�֐��͎����I�ɌĂяo����܂����A�����I�ɌĂяo�����Ƃ��\�ł��B
void ResourceManager::DestroyResource()
{
	CleanupHumanModel();
	CleanupHumanTexture();
	CleanupWeaponModelTexture();
	CleanupWeaponSound();
	CleanupSmallObjectModelTexture();
	CleanupSmallObjectSound();

	CleanupScopeTexture();
	CleanupSkyModelTexture();
	CleanupBulletModelTexture();
	CleanupBulletSound();
	CleanupEffectTexture();
}

//! @brief �l�̃��f����ǂݍ���
//! @return �����F0�@���s�F1
int ResourceManager::LoadHumanModel()
{
	if( d3dg == NULL ){ return 1; }

	human_upmodel[0] = d3dg->LoadModel("data\\model\\up0.x");
	human_upmodel[1] = d3dg->LoadModel("data\\model\\up1.x");
	human_upmodel[2] = d3dg->LoadModel("data\\model\\up2.x");
	human_upmodel[3] = d3dg->LoadModel("data\\model\\up3.x");
	human_upmodel[4] = d3dg->LoadModel("data\\model\\up4.x");
	human_upmodel[5] = d3dg->LoadModel("data\\model\\up5.x");
	human_armmodel[0] = d3dg->LoadModel("data\\model\\arm00.x");
	human_armmodel[1] = d3dg->LoadModel("data\\model\\arm01.x");
	human_armmodel[2] = d3dg->LoadModel("data\\model\\arm02.x");
	human_legmodel = d3dg->LoadModel("data\\model\\leg.x");
	human_walkmodel[0] = d3dg->LoadModel("data\\model\\walk01.x");
	human_walkmodel[2] = d3dg->LoadModel("data\\model\\walk02.x");
	human_walkmodel[4] = d3dg->LoadModel("data\\model\\walk03.x");
	human_walkmodel[6] = d3dg->LoadModel("data\\model\\walk04.x");
	human_runmodel[0] = d3dg->LoadModel("data\\model\\run01.x");
	human_runmodel[2] = d3dg->LoadModel("data\\model\\run02.x");
	human_runmodel[4] = d3dg->LoadModel("data\\model\\run03.x");
	human_runmodel[6] = d3dg->LoadModel("data\\model\\run04.x");
	human_runmodel[8] = d3dg->LoadModel("data\\model\\run05.x");
	human_runmodel[10] = d3dg->LoadModel("data\\model\\run06.x");

#ifdef ENABLE_HUMANMODEL_MORPHING
	//���[�t�B���O���������s����
	human_walkmodel[1] = d3dg->MorphingModel(human_walkmodel[0], human_walkmodel[2]);
	human_walkmodel[3] = d3dg->MorphingModel(human_walkmodel[2], human_walkmodel[4]);
	human_walkmodel[5] = d3dg->MorphingModel(human_walkmodel[4], human_walkmodel[6]);
	human_walkmodel[7] = d3dg->MorphingModel(human_walkmodel[6], human_walkmodel[0]);
	human_runmodel[1] = d3dg->MorphingModel(human_runmodel[0], human_runmodel[2]);
	human_runmodel[3] = d3dg->MorphingModel(human_runmodel[2], human_runmodel[4]);
	human_runmodel[5] = d3dg->MorphingModel(human_runmodel[4], human_runmodel[6]);
	human_runmodel[7] = d3dg->MorphingModel(human_runmodel[6], human_runmodel[8]);
	human_runmodel[9] = d3dg->MorphingModel(human_runmodel[8], human_runmodel[10]);
	human_runmodel[11] = d3dg->MorphingModel(human_runmodel[10], human_runmodel[0]);
#else
	//���[�t�B���O���������s���Ȃ�
	human_walkmodel[1] = human_walkmodel[0];
	human_walkmodel[3] = human_walkmodel[2];
	human_walkmodel[5] = human_walkmodel[4];
	human_walkmodel[7] = human_walkmodel[6];
	human_runmodel[1] = human_runmodel[0];
	human_runmodel[3] = human_runmodel[2];
	human_runmodel[5] = human_runmodel[4];
	human_runmodel[7] = human_runmodel[6];
	human_runmodel[9] = human_runmodel[8];
	human_runmodel[11] = human_runmodel[10];
#endif

	return 0;
}

//! @brief �l�̃��f���̃f�[�^�ԍ����擾
//! @param out_upmodel[] �㔼�g�̃��f���@�i�z�񐔁FTOTAL_UPMODE�j
//! @param out_armmodel[] �r�̃��f���@�i�z�񐔁FTOTAL_ARMMODE�j
//! @param *legmodel ���i�Î~��ԁj�̃��f���̃|�C���^
//! @param out_walkmodel[] ���i�����j�̃��f���@�i�z�񐔁FTOTAL_WALKMODE�j
//! @param out_runmodel[] ���i����j�̃��f���@�i�z�񐔁FTOTAL_RUNMODE�j
void ResourceManager::GetHumanModel(int out_upmodel[], int out_armmodel[], int *legmodel, int out_walkmodel[], int out_runmodel[])
{
	for(int i=0; i<TOTAL_UPMODE; i++){
		out_upmodel[i] = human_upmodel[i];
	}
	for(int i=0; i<TOTAL_ARMMODE; i++){
		out_armmodel[i] = human_armmodel[i];
	}
	*legmodel = human_legmodel;
	for(int i=0; i<TOTAL_WALKMODE; i++){
		out_walkmodel[i] = human_walkmodel[i];
	}
	for(int i=0; i<TOTAL_RUNMODE; i++){
		out_runmodel[i] = human_runmodel[i];
	}
}

//! @brief �l�̃��f�������
void ResourceManager::CleanupHumanModel()
{
	if( d3dg == NULL ){ return; }

	for(int i=0; i<TOTAL_UPMODE; i++){
		d3dg->CleanupModel(human_upmodel[i]);
		human_upmodel[i] = -1;
	}
	for(int i=0; i<TOTAL_ARMMODE; i++){
		d3dg->CleanupModel(human_armmodel[i]);
		human_armmodel[i] = -1;
	}
	d3dg->CleanupModel(human_legmodel);
	for(int i=0; i<TOTAL_WALKMODE; i++){
		d3dg->CleanupModel(human_walkmodel[i]);
		human_walkmodel[i] = -1;
	}
	for(int i=0; i<TOTAL_RUNMODE; i++){
		d3dg->CleanupModel(human_runmodel[i]);
		human_runmodel[i] = -1;
	}
}

//! @brief �l�̃e�N�X�`����ǂݍ���
//! @param id �l�̎��ID
//! @return �����F0�ȏ�@���s�F-1
int ResourceManager::AddHumanTexture(int id)
{
	if( d3dg == NULL ){ return -1; }
	if( (id < 0)||(TOTAL_PARAMETERINFO_HUMAN <= id ) ){ return -1; }

	HumanParameter data;
	char path[MAX_PATH];
	if( ParamInfo->GetHuman(id, &data) == 1 ){ return -1; }

	//���ɓǂݍ��񂾃e�N�X�`�����ǂ������ׂ�
	for(int i=0; i<MAX_LOADHUMANTEXTURE; i++){
		if( human_texture_Param[i] == data.texture ){
			return -1;
		}
	}

	//�e�N�X�`����ǂݍ���
	for(int i=0; i<MAX_LOADHUMANTEXTURE; i++){
		if( human_texture_Param[i] == -1 ){
			int TexturePathID = data.texture;
			if( ParamInfo->GetHumanTexturePath(TexturePathID, path) == 1 ){ return -1; }
			human_texture_Param[i] = TexturePathID;				//�f�[�^�ԍ����L�^
			human_texture_d3dg[i] = d3dg->LoadTexture(path, false, false);	//�e�N�X�`����ǂݍ���
			return i;
		}
	}

	return -1;
}

//! @brief �ǂݍ���ł���l�̃e�N�X�`�������擾
//! @return �e�N�X�`�����i�ő� MAX_LOADHUMANTEXTURE�j
int ResourceManager::GetHumanTextures()
{
	int Textures = 0;

	for(int i=0; i<MAX_LOADHUMANTEXTURE; i++){
		if( human_texture_Param[i] != -1 ){
			Textures += 1;
		}
	}

	return Textures;
}

//! @brief �l�̃e�N�X�`���ԍ����擾
//! @param id �l�̎��ID
//! @return �����F�e�N�X�`��ID�@���s�F-1
int ResourceManager::GetHumanTexture(int id)
{
	if( (id < 0)||(TOTAL_PARAMETERINFO_HUMAN <= id ) ){ return -1; }

	HumanParameter data;
	if( ParamInfo->GetHuman(id, &data) == 1 ){ return -1; }

	for(int i=0; i<MAX_LOADHUMANTEXTURE; i++){
		if( human_texture_Param[i] == data.texture ){
			return human_texture_d3dg[i];
		}
	}

	return -1;
}

//! @brief �ǂݍ���ł���l�̃e�N�X�`�������擾
//! @param dataid �f�[�^�ԍ�
//! @param ParamID �l�̎��ID���󂯎��|�C���^�iNULL�j
//! @param TextureID �e�N�X�`��ID���󂯎��|�C���^�iNULL�j
//! @return �����Ftrue�@���s�Ffalse
//! @attention �f�o�b�N�p�̊֐��ł��B�ʏ�͎g�p���܂���B
bool ResourceManager::GetHumanTextureInfo(int dataid, int *ParamID, int *TextureID)
{
	if( (dataid < 0)||(MAX_LOADHUMANTEXTURE <= dataid ) ){ return false; }

	if( ParamID != NULL ){ *ParamID = human_texture_Param[dataid]; }
	if( TextureID != NULL ){ *TextureID = human_texture_d3dg[dataid]; }
	return true;
}

//! @brief �l�̃e�N�X�`�����ꊇ���
void ResourceManager::CleanupHumanTexture()
{
	if( d3dg == NULL ){ return; }

	for(int i=0; i<MAX_LOADHUMANTEXTURE; i++){
		if( human_texture_Param[i] != -1 ){
			d3dg->CleanupTexture(human_texture_d3dg[i]);
			human_texture_Param[i] = -1;
			human_texture_d3dg[i] = -1;
		}
	}
}

//! @brief ����̃��f����e�N�X�`�����ꊇ�ǂݍ���
//! @return �����F0�@���s�F1�ȏ�
int ResourceManager::LoadWeaponModelTexture()
{
	int cnt = 0;

	if( d3dg == NULL ){ return 1; }

	for(int i=0; i<TOTAL_PARAMETERINFO_WEAPON; i++){
		WeaponParameter data;
		if( ParamInfo->GetWeapon(i, &data) == 0 ){
			//���f���ƃe�N�X�`����ǂݍ��݁A�G���[���o��΃J�E���g����B
			weapon_model[i] = d3dg->LoadModel(data.model);
			weapon_texture[i] = d3dg->LoadTexture(data.texture, false, false);
			if( weapon_model[i] == -1 ){ cnt += 1; }
			if( weapon_texture[i] == -1 ){ cnt += 1; }
		}
		else{
			//�ݒ�f�[�^������������΁A���f���ƃe�N�X�`�� 2�Ƃ��G���[�B
			cnt += 2;
		}
	}

	return cnt;
}

//! @brief ����̃��f���ƃe�N�X�`�����擾
//! @return �����F0�@���s�F1
int ResourceManager::GetWeaponModelTexture(int id, int *model, int *texture)
{
	if( model == NULL ){ return 1; }
	if( texture == NULL ){ return 1; }

	if( (id < 0)||(TOTAL_PARAMETERINFO_WEAPON <= id ) ){
#ifdef ENABLE_BUG_HUMANWEAPON
		return GetBugWeaponModelTexture(id, model, texture);
#else
		return 1;
#endif
	}

	*model = weapon_model[id];
	*texture = weapon_texture[id];
	return 0;
}

#ifdef ENABLE_BUG_HUMANWEAPON
//! @brief �o�O����̃��f���ƃe�N�X�`�����擾
//! @return �����F0�@���s�F1
//! @warning ��� GetWeaponModelTexture()�֐� �����s���Ď擾�Ɏ��s�������Ɍ���A���̊֐����g���Ă��������B
//! @attention �o�O�����ǉ�����ꍇ�́AParameterInfo�N���X�� GetBugWeapon() �֐����ҏW���Ă��������B
int ResourceManager::GetBugWeaponModelTexture(int id, int *model, int *texture)
{
	if( d3dg == NULL ){ return 1; }
	if( model == NULL ){ return 1; }
	if( texture == NULL ){ return 1; }

	if( id == 23 ){
		*model = human_upmodel[0];
		*texture = d3dg->GetMapTextureID(0);
		return 0;
	}
	if( id == 24 ){
		*model = human_upmodel[0];
		*texture = d3dg->GetMapTextureID(3);
		return 0;
	}
	if( id == 30 ){
		*model = human_upmodel[0];
		*texture = d3dg->GetMapTextureID(2);
		return 0;
	}
	if( id == 53 ){
		*model = human_upmodel[0];
		*texture = d3dg->GetMapTextureID(8);
		return 0;
	}

	return 1;
}
#endif

//! @brief ����̃��f����e�N�X�`�����ꊇ���
void ResourceManager::CleanupWeaponModelTexture()
{
	if( d3dg == NULL ){ return; }

	for(int i=0; i<TOTAL_PARAMETERINFO_WEAPON; i++){
		d3dg->CleanupModel(weapon_model[i]);
		d3dg->CleanupTexture(weapon_texture[i]);
		weapon_model[i] = -1;
		weapon_texture[i] = -1;
	}
}

//! @brief ����̃T�E���h��ǂݍ���
//! @return �����F0�@���s�F1
int ResourceManager::LoadWeaponSound()
{
	if( SoundCtrl == NULL ){ return 1; }

	int soundid[6];
	int reloadid;

	//�T�E���h�t�@�C�����܂Ƃ߂ēǂݍ���
	soundid[0] = SoundCtrl->LoadSound("data\\sound\\bang1.wav");
	soundid[1] = SoundCtrl->LoadSound("data\\sound\\bang2.wav");
	soundid[2] = SoundCtrl->LoadSound("data\\sound\\bang3.wav");
	soundid[3] = SoundCtrl->LoadSound("data\\sound\\bang4.wav");
	soundid[4] = SoundCtrl->LoadSound("data\\sound\\bang5.wav");
	soundid[5] = SoundCtrl->LoadSound("data\\sound\\ka.wav");
	//reloadid = SoundCtrl->LoadSound("reload.wav");
	reloadid = -1;

	for(int i=0; i<TOTAL_PARAMETERINFO_WEAPON; i++){
		WeaponParameter data;
		if( ParamInfo->GetWeapon(i, &data) == 0 ){
			//�T�E���h�ԍ��ɍ��킹�āA���ʉ������蓖�Ă�B
			if( data.soundid == 0 ){
				weapon_sound[i] = soundid[0];
			}
			else if( data.soundid == 1 ){
				weapon_sound[i] = soundid[1];
			}
			else if( data.soundid == 2 ){
				weapon_sound[i] = soundid[2];
			}
			else if( data.soundid == 3 ){
				weapon_sound[i] = soundid[3];
			}
			else if( data.soundid == 4 ){
				weapon_sound[i] = soundid[4];
			}
			else if( data.soundid == 13 ){
				weapon_sound[i] = soundid[5];
			}
			else{
				weapon_sound[i] = -1;
			}
		}
	}
	weapon_reloadsound = reloadid;

	//�t�@�C���ǂݍ��ݎ��ɃG���[���o�Ȃ����������ׂ�
	for(int i=0; i<6; i++){
		if( soundid[i] == -1 ){ return 1; }
	}
	if( reloadid == -1 ){ return 1; }
	return 0;
}

//! @brief ����̃T�E���h���擾
//! @param id 0�ȏ�ŕ���̃f�[�^�ԍ��A-1�Ń����[�h��
//! @return �����F�f�[�^�ԍ��@���s�F-1
int ResourceManager::GetWeaponSound(int id)
{
	if( SoundCtrl == NULL ){ return -1; }
	if( id == -1 ){ return weapon_reloadsound; }

	if( (id < 0)||(TOTAL_PARAMETERINFO_WEAPON <= id ) ){ return -1; }
	return weapon_sound[id];
}

//! @brief ����̃T�E���h���ꊇ���
void ResourceManager::CleanupWeaponSound()
{
	if( SoundCtrl == NULL ){ return; }

	for(int i=0; i<TOTAL_PARAMETERINFO_WEAPON; i++){
		SoundCtrl->CleanupSound(weapon_sound[i]);
		weapon_sound[i] = -1;
	}
	SoundCtrl->CleanupSound(weapon_reloadsound);
	weapon_reloadsound = -1;
}

//! @brief �����̃��f����e�N�X�`�����ꊇ�ǂݍ���
//! @return �����F0�@���s�F1�ȏ�
int ResourceManager::LoadSmallObjectModelTexture()
{
	int cnt = 0;

	if( d3dg == NULL ){ return 1; }

	for(int i=0; i<TOTAL_PARAMETERINFO_SMALLOBJECT; i++){
		SmallObjectParameter data;
		if( ParamInfo->GetSmallObject(i, &data) == 0 ){
			//���f���ƃe�N�X�`����ǂݍ��݁A�G���[���o��΃J�E���g����B
			smallobject_model[i] = d3dg->LoadModel(data.model);
			smallobject_texture[i] = d3dg->LoadTexture(data.texture, false, false);
			if( smallobject_model[i] == -1 ){ cnt += 1; }
			if( smallobject_texture[i] == -1 ){ cnt += 1; }
		}
		else{
			//�ݒ�f�[�^������������΁A���f���ƃe�N�X�`�� 2�Ƃ��G���[�B
			cnt += 2;
		}
	}

	return cnt;
}

//! @brief �����̃��f���ƃe�N�X�`�����擾
//! @return �����F0�@���s�F1
int ResourceManager::GetSmallObjectModelTexture(int id, int *model, int *texture)
{
	if( model == NULL ){ return 1; }
	if( texture == NULL ){ return 1; }
	if( (id < 0)||((TOTAL_PARAMETERINFO_SMALLOBJECT + MAX_ADDSMALLOBJECT) <= id ) ){ return 1; }

	*model = smallobject_model[id];
	*texture = smallobject_texture[id];
	return 0;
}

//! @brief �����̃��f����e�N�X�`�����ꊇ���
void ResourceManager::CleanupSmallObjectModelTexture()
{
	if( d3dg == NULL ){ return; }

	for(int i=0; i<(TOTAL_PARAMETERINFO_SMALLOBJECT + MAX_ADDSMALLOBJECT); i++){
		d3dg->CleanupModel(smallobject_model[i]);
		d3dg->CleanupTexture(smallobject_texture[i]);
		smallobject_model[i] = -1;
		smallobject_texture[i] = -1;
	}
}

//! @brief �����̃T�E���h��ǂݍ���
//! @return �����F0�@���s�F1
int ResourceManager::LoadSmallObjectSound()
{
	if( SoundCtrl == NULL ){ return 1; }

	int soundid[2];

	//�T�E���h�t�@�C�����܂Ƃ߂ēǂݍ���
	soundid[0] = SoundCtrl->LoadSound("data\\sound\\can.wav");
	soundid[1] = SoundCtrl->LoadSound("data\\sound\\dan.wav");

	for(int i=0; i<TOTAL_PARAMETERINFO_SMALLOBJECT; i++){
		SmallObjectParameter data;
		if( ParamInfo->GetSmallObject(i, &data) == 0 ){
			//�T�E���h�ԍ��ɍ��킹�āA���ʉ������蓖�Ă�B
			if( data.sound == 0 ){
				smallobject_sound[i] = soundid[0];
			}
			else if( data.sound == 1 ){
				smallobject_sound[i] = soundid[1];
			}
			else{
				smallobject_sound[i] = -1;
			}
		}
	}

	//�t�@�C���ǂݍ��ݎ��ɃG���[���o�Ȃ����������ׂ�
	for(int i=0; i<2; i++){
		if( soundid[i] == -1 ){ return 1; }
	}
	return 0;
}

//! @brief �����̃T�E���h���擾
//! @param id �����̃f�[�^�ԍ�
//! @return �����F�f�[�^�ԍ��@���s�F-1
int ResourceManager::GetSmallObjectSound(int id)
{
	if( SoundCtrl == NULL ){ return -1; }
	if( (id < 0)||((TOTAL_PARAMETERINFO_SMALLOBJECT + MAX_ADDSMALLOBJECT) <= id ) ){ return -1; }
	return smallobject_sound[id];
}

//! @brief �����̃T�E���h���ꊇ���
void ResourceManager::CleanupSmallObjectSound()
{
	if( SoundCtrl == NULL ){ return; }

	for(int i=0; i<(TOTAL_PARAMETERINFO_SMALLOBJECT + MAX_ADDSMALLOBJECT); i++){
		SoundCtrl->CleanupSound(smallobject_sound[i]);
		smallobject_sound[i] = -1;
	}
}

//! @brief �ǉ������̃��f���E�e�N�X�`���E�T�E���h���擾
//! @param id �f�[�^�ԍ��i�ʏ�� 0 �Œ�j
//! @param modelpath ���f���f�[�^�̃p�X
//! @param texturepath �e�N�X�`���f�[�^�̃p�X
//! @param soundpath �T�E���h�f�[�^�̃p�X
//! @return �����F0�@���s�F1�ȏ�
int ResourceManager::LoadAddSmallObject(int id, char *modelpath, char *texturepath, char *soundpath)
{
	if( d3dg == NULL ){ return 1; }
	if( SoundCtrl == NULL ){ return 1; }
	if( modelpath == NULL ){ return 1; }
	if( texturepath == NULL ){ return 1; }
	if( soundpath == NULL ){ return 1; }
	if( (id < 0)||(MAX_ADDSMALLOBJECT <= id) ){ return 1; }

	int dataid = TOTAL_PARAMETERINFO_SMALLOBJECT + id;
	int cnt = 0;

	smallobject_model[dataid] = d3dg->LoadModel(modelpath);
	smallobject_texture[dataid] = d3dg->LoadTexture(texturepath, false, false);
	if( smallobject_model[dataid] == -1 ){ cnt += 1; }
	if( smallobject_texture[dataid] == -1 ){ cnt += 1; }
	smallobject_sound[dataid] = SoundCtrl->LoadSound(soundpath);
	if( smallobject_sound[dataid] == -1 ){ cnt += 1; }

	return cnt;
}

//! @brief �ǉ������̃��f���E�e�N�X�`���E�T�E���h�����
//! @param id �f�[�^�ԍ��i�ʏ�� 0 �Œ�j
void ResourceManager::CleanupAddSmallObject(int id)
{
	if( d3dg == NULL ){ return; }
	if( SoundCtrl == NULL ){ return; }
	if( (id < 0)||(MAX_ADDSMALLOBJECT <= id) ){ return; }

	int dataid = TOTAL_PARAMETERINFO_SMALLOBJECT + id;

	d3dg->CleanupModel(smallobject_model[dataid]);
	d3dg->CleanupTexture(smallobject_texture[dataid]);
	SoundCtrl->CleanupSound(smallobject_sound[dataid]);
}

//! @brief �e�̃��f���ƃe�N�X�`����ǂݍ���
//! @return �����F0�@���s�F1�ȏ�
int ResourceManager::LoadBulletModelTexture()
{
	int cnt = 0;

	if( d3dg == NULL ){ return 1; }

	for(int i=0; i<TOTAL_PARAMETERINFO_BULLET; i++){
		BulletParameter data;
		if( ParamInfo->GetBullet(i, &data) == 0 ){
			//���f���ƃe�N�X�`����ǂݍ��݁A�G���[���o��΃J�E���g����B
			bullet_model[i] = d3dg->LoadModel(data.model);
			bullet_texture[i] = d3dg->LoadTexture(data.texture, false, false);
			if( bullet_model[i] == -1 ){ cnt += 1; }
			if( bullet_texture[i] == -1 ){ cnt += 1; }
		}
		else{
			//�ݒ�f�[�^������������΁A���f���ƃe�N�X�`�� 2�Ƃ��G���[�B
			cnt += 2;
		}
	}

	return cnt;
}

//! @brief �e�̃��f���ƃe�N�X�`�����擾
//! @return �����F0�@���s�F1
int ResourceManager::GetBulletModelTexture(int id, int *model, int *texture)
{
	if( model == NULL ){ return 1; }
	if( texture == NULL ){ return 1; }
	if( (id < 0)||(TOTAL_PARAMETERINFO_BULLET <= id ) ){ return 1; }

	*model = bullet_model[id];
	*texture = bullet_texture[id];

	return 0;
}

//! @brief �e�̃��f���ƃe�N�X�`�������
void ResourceManager::CleanupBulletModelTexture()
{
	if( d3dg == NULL ){ return; }

	for(int i=0; i<TOTAL_PARAMETERINFO_BULLET; i++){
		d3dg->CleanupModel(bullet_model[i]);
		bullet_model[i] = -1;

		d3dg->CleanupTexture(bullet_texture[i]);
		bullet_texture[i] = -1;
	}
}

//! @brief �X�R�[�v�e�N�X�`����ǂݍ���
//! @return �����F0�@���s�F1
int ResourceManager::LoadScopeTexture()
{
	if( d3dg == NULL ){ return 1; }

	scopetexture = d3dg->LoadTexture("data\\scope.dds", false, false);
	if( scopetexture == -1 ){ return 1; }
	return 0;
}

//! @brief �X�R�[�v�e�N�X�`�����擾
//! @return �e�N�X�`���̃f�[�^�ԍ�
int ResourceManager::GetScopeTexture()
{
	return scopetexture;
}

//! @brief �X�R�[�v�e�N�X�`�������
void ResourceManager::CleanupScopeTexture()
{
	if( d3dg == NULL ){ return; }
	if( scopetexture == -1 ){ return; }

	d3dg->CleanupTexture(scopetexture);
	scopetexture = -1;
}

//! @brief �w�i��̃e�N�X�`����ǂݍ���
//! @param id ��̔ԍ��i1�`5�j
//! @return �����F0�@���s�F1
int ResourceManager::LoadSkyModelTexture(int id)
{
	if( d3dg == NULL ){ return 1; }
	if( (id < 1)||(5 < id) ){ return 1; }

	//���f����ǂݍ��ށi�Œ�j
	skymodel = d3dg->LoadModel("data\\sky\\sky.x");

	//�e�N�X�`�����𐶐����ēǂݍ���
	char path[MAX_PATH];
	sprintf(path, "data\\sky\\sky%d.bmp", id);
	skytexture = d3dg->LoadTexture(path, false, false);

	if( skymodel == -1 ){ return 1; }
	if( skytexture == -1 ){ return 1; }
	return 0;
}

//! @brief �w�i��̃��f���ƃe�N�X�`�����擾
void ResourceManager::GetSkyModelTexture(int *model, int *texture)
{
	if( model == NULL ){ return; }
	if( texture == NULL ){ return; }

	*model = skymodel;
	*texture = skytexture;
}

//! @brief �w�i��̃��f���ƃe�N�X�`�������
void ResourceManager::CleanupSkyModelTexture()
{
	if( d3dg == NULL ){ return; }

	if( skymodel != -1 ){
		d3dg->CleanupModel(skymodel);
		skymodel = -1;
	}
	if( skytexture != -1 ){
		d3dg->CleanupTexture(skytexture);
		skytexture = -1;
	}
}

//! @brief �e�E��֒e�̃T�E���h��ǂݍ���
//! @return �����F0�@���s�F1
int ResourceManager::LoadBulletSound()
{
	if( SoundCtrl == NULL ){ return 1; }

	bullet_hitsoundA = SoundCtrl->LoadSound("data\\sound\\hit1.wav");
	bullet_hitsoundB = SoundCtrl->LoadSound("data\\sound\\hit3.wav");
	bullet_humanhitsound = SoundCtrl->LoadSound("data\\sound\\hit2.wav");
	bullet_passingsound = SoundCtrl->LoadSound("data\\sound\\hyu.wav");
	grenade_bang = SoundCtrl->LoadSound("data\\sound\\bang.wav");
	grenade_cco = SoundCtrl->LoadSound("data\\sound\\cco.wav");

	if( bullet_hitsoundA == -1 ){ return 1; }
	if( bullet_hitsoundB == -1 ){ return 1; }
	if( bullet_humanhitsound == -1 ){ return 1; }
	if( bullet_passingsound == -1 ){ return 1; }
	if( grenade_bang == -1 ){ return 1; }
	if( grenade_cco == -1 ){ return 1; }
	return 0;
}

//! @brief �e�E��֒e�̃T�E���h���擾
//! @param hitsoundA �e�q�b�g�T�E���h�FA�@�iNULL�j
//! @param hitsoundB �e�q�b�g�T�E���h�FB�@�iNULL�j
//! @param humanhitsound �e�q�b�g�T�E���h�F�l�@�iNULL�j
//! @param passingsound �e�ʉ߃T�E���h�@�iNULL�j
//! @param grenadebang ��֒e �����T�E���h�@�iNULL�j
//! @param grenadecco ��֒e�o�E���h �T�E���h�@�iNULL�j
void ResourceManager::GetBulletSound(int *hitsoundA, int *hitsoundB, int *humanhitsound, int *passingsound, int *grenadebang, int *grenadecco)
{
	if( SoundCtrl == NULL ){ return; }

	if( hitsoundA != NULL ){ *hitsoundA = bullet_hitsoundA; }
	if( hitsoundB != NULL ){ *hitsoundB = bullet_hitsoundB; }
	if( humanhitsound != NULL ){ *humanhitsound = bullet_humanhitsound; }
	if( passingsound != NULL ){ *passingsound = bullet_passingsound; }
	if( grenadebang != NULL ){ *grenadebang = grenade_bang; }
	if( grenadecco != NULL ){ *grenadecco = grenade_cco; }
}

//! @brief �e�E��֒e�̃T�E���h���ꊇ���
void ResourceManager::CleanupBulletSound()
{
	if( SoundCtrl == NULL ){ return; }

	SoundCtrl->CleanupSound(bullet_hitsoundA);
	SoundCtrl->CleanupSound(bullet_hitsoundB);
	SoundCtrl->CleanupSound(bullet_humanhitsound);
	SoundCtrl->CleanupSound(bullet_passingsound);
	SoundCtrl->CleanupSound(grenade_bang);
	SoundCtrl->CleanupSound(grenade_cco);

	bullet_hitsoundA = -1;
	bullet_hitsoundB = -1;
	bullet_humanhitsound = -1;
	bullet_passingsound = -1;
	grenade_bang = -1;
	grenade_cco = -1;
}

//! @brief �G�t�F�N�g�̃e�N�X�`����ǂݍ���
//! @return �����F0�@���s�F1
int ResourceManager::LoadEffectTexture()
{
	if( d3dg == NULL ){ return 1; }

	effecttexture_blood = d3dg->LoadTexture("data\\blood.dds", false, false);
	effecttexture_mflash = d3dg->LoadTexture("data\\mflash.dds", false, false);
	effecttexture_smoke = d3dg->LoadTexture("data\\smoke.dds", false, false);
	effecttexture_yakkyou = d3dg->LoadTexture("data\\yakkyou.dds", false, false);

	if( effecttexture_blood == -1 ){ return 1; }
	if( effecttexture_mflash == -1 ){ return 1; }
	if( effecttexture_smoke == -1 ){ return 1; }
	if( effecttexture_yakkyou == -1 ){ return 1; }
	return 0;
}

//! @brief ���e�N�X�`���擾
//! @return �e�N�X�`���̃f�[�^�ԍ�
int ResourceManager::GetEffectBloodTexture()
{
	return effecttexture_blood;
}

//! @brief �t���b�V���e�N�X�`���擾
//! @return �e�N�X�`���̃f�[�^�ԍ�
int ResourceManager::GetEffectMflashTexture()
{
	return effecttexture_mflash;
}

//! @brief ���e�N�X�`���擾
//! @return �e�N�X�`���̃f�[�^�ԍ�
int ResourceManager::GetEffectSmokeTexture()
{
	return effecttexture_smoke;
}

//! @brief ��䰃e�N�X�`���擾
//! @return �e�N�X�`���̃f�[�^�ԍ�
int ResourceManager::GetEffectYakkyouTexture()
{
	return effecttexture_yakkyou;
}

//! @brief �G�t�F�N�g�̃e�N�X�`�������
void ResourceManager::CleanupEffectTexture()
{
	if( d3dg == NULL ){ return; }

	if( effecttexture_blood != -1 ){
		d3dg->CleanupTexture(effecttexture_blood);
		effecttexture_blood = -1;
	}
	if( effecttexture_mflash != -1 ){
		d3dg->CleanupTexture(effecttexture_mflash);
		effecttexture_mflash = -1;
	}
	if( effecttexture_smoke != -1 ){
		d3dg->CleanupTexture(effecttexture_smoke);
		effecttexture_smoke = -1;
	}
	if( effecttexture_yakkyou != -1 ){
		d3dg->CleanupTexture(effecttexture_yakkyou);
		effecttexture_yakkyou = -1;
	}
}