//! @file resource.h
//! @brief ResourceManager�N���X�̐錾

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

#ifndef RESOURCE_H
#define RESOURCE_H

#define TOTAL_UPMODE 6				//!< �㔼�g�̍��v���f��
#define TOTAL_ARMMODE 3				//!< �r�̍��v���f��
#define TOTAL_WALKMODE 8			//!< ���i�����j�̍��v���f��
#define TOTAL_RUNMODE 12			//!< ���i����j�̍��v���f��

#define MAX_LOADHUMANTEXTURE 10		//!< �l�̃e�N�X�`����ǂݍ��ލő喇��

#define ENABLE_HUMANMODEL_MORPHING	//!< �l���f���̃��[�t�B���O�����L�����i�R�����g���ŋ@�\�����j

#ifndef H_LAYERLEVEL
 #define H_LAYERLEVEL 2		//!< Select include file.
#endif
#include "main.h"

//! @brief ���\�[�X�Ǘ��N���X
//! @details ��Ɏg�p����郂�f����e�N�X�`������уT�E���h���Ǘ����܂��B�@�i�ꕔ�������j
class ResourceManager
{
	class ParameterInfo *ParamInfo;				//!< �Ǘ�����ݒ�l���N���X
	class D3DGraphics *d3dg;					//!< �Ǘ�����`��N���X
	class SoundControl *SoundCtrl;				//!< �Ǘ�����T�E���h�N���X

	int human_upmodel[TOTAL_UPMODE];			//!< �㔼�g�̃��f��
	int human_armmodel[TOTAL_ARMMODE];			//!< �r�̃��f��
	int human_legmodel;							//!< ���i�Î~��ԁj�̃��f��
	int human_walkmodel[TOTAL_WALKMODE];		//!< ���i�����j�̃��f��
	int human_runmodel[TOTAL_RUNMODE];			//!< ���i����j�̃��f��
	int human_texture_Param[MAX_LOADHUMANTEXTURE];		//!< �l�̃e�N�X�`���̑Ή��ԍ�
	int human_texture_d3dg[MAX_LOADHUMANTEXTURE];		//!< �l�̃e�N�X�`���̃f�[�^�ԍ�
	int weapon_model[TOTAL_PARAMETERINFO_WEAPON];		//!< ����̃��f���̃f�[�^�ԍ�
	int weapon_texture[TOTAL_PARAMETERINFO_WEAPON];		//!< ����̃e�N�X�`���̃f�[�^�ԍ�
	int weapon_sound[TOTAL_PARAMETERINFO_WEAPON];		//!< ����̃T�E���h�̃f�[�^�ԍ�
	int weapon_reloadsound;								//!< ����̃����[�h�T�E���h�̃f�[�^�ԍ�
	int smallobject_model[TOTAL_PARAMETERINFO_SMALLOBJECT + MAX_ADDSMALLOBJECT];		//!< �����̃��f���̃f�[�^�ԍ�
	int smallobject_texture[TOTAL_PARAMETERINFO_SMALLOBJECT + MAX_ADDSMALLOBJECT];		//!< �����̃e�N�X�`���̃f�[�^�ԍ�
	int smallobject_sound[TOTAL_PARAMETERINFO_SMALLOBJECT + MAX_ADDSMALLOBJECT];		//!< �����̃T�E���h�̃f�[�^�ԍ�
	int bullet_model[TOTAL_PARAMETERINFO_BULLET];		//!< �e�̃��f��
	int bullet_texture[TOTAL_PARAMETERINFO_BULLET];		//!< �e�̃e�N�X�`��

	int scopetexture;			//!< �X�R�[�v�̃e�N�X�`��
	int skymodel;				//!< ��w�i�̃��f��
	int skytexture;				//!< ��w�i�̃e�N�X�`��
	int bullet_hitsoundA;		//!< �e�̃q�b�g�T�E���h�iA�j
	int bullet_hitsoundB;		//!< �e�̃q�b�g�T�E���h�iB�j
	int bullet_humanhitsound;	//!< �e�̃q�b�g�T�E���h�i�l�j
	int bullet_passingsound;	//!< �e�̒ʉ߃T�E���h
	int grenade_bang;			//!< ��֒e�̔�����
	int grenade_cco;			//!< ��֒e�̃o�E���h��
	int effecttexture_blood;	//!< �G�t�F�N�g�p�E���e�N�X�`��
	int effecttexture_mflash;	//!< �G�t�F�N�g�p�E�t���b�V���e�N�X�`��
	int effecttexture_smoke;	//!< �G�t�F�N�g�p�E���e�N�X�`��
	int effecttexture_yakkyou;	//!< �G�t�F�N�g�p�E��䰃e�N�X�`��

public:
	ResourceManager();
	~ResourceManager();
	void SetParameterInfo(ParameterInfo *_ParamInfo);
	void SetD3DGraphics(D3DGraphics *_d3dg);
	void SetSoundControl(SoundControl *_SoundCtrl);
	void DestroyResource();

	int LoadHumanModel();
	void GetHumanModel(int out_upmodel[], int out_armmodel[], int *legmodel, int out_walkmodel[], int out_runmodel[]);
	void CleanupHumanModel();
	int AddHumanTexture(int id);
	int GetHumanTextures();
	int GetHumanTexture(int id);
	bool GetHumanTextureInfo(int dataid, int *ParamID, int *TextureID);
	void CleanupHumanTexture();
	int LoadWeaponModelTexture();
	int GetWeaponModelTexture(int id, int *model, int *texture);
#ifdef ENABLE_BUG_HUMANWEAPON
	int GetBugWeaponModelTexture(int id, int *model, int *texture);
#endif
	void CleanupWeaponModelTexture();
	int LoadWeaponSound();
	int GetWeaponSound(int id);
	void CleanupWeaponSound();
	int LoadSmallObjectModelTexture();
	int GetSmallObjectModelTexture(int id, int *model, int *texture);
	void CleanupSmallObjectModelTexture();
	int LoadSmallObjectSound();
	int GetSmallObjectSound(int id);
	void CleanupSmallObjectSound();
	int LoadAddSmallObject(int id, char *modelpath, char *texturepath, char *soundpath);
	void CleanupAddSmallObject(int id);
	int LoadBulletModelTexture();
	int GetBulletModelTexture(int id, int *model, int *texture);
	void CleanupBulletModelTexture();

	int LoadScopeTexture();
	int GetScopeTexture();
	void CleanupScopeTexture();
	int LoadSkyModelTexture(int id);
	void GetSkyModelTexture(int *model, int *texture);
	void CleanupSkyModelTexture();
	int LoadBulletSound();
	void GetBulletSound(int *hitsoundA, int *hitsoundB, int *humanhitsound, int *passingsound, int *grenadebang, int *grenadecco);
	void CleanupBulletSound();
	int LoadEffectTexture();
	int GetEffectBloodTexture();
	int GetEffectMflashTexture();
	int GetEffectSmokeTexture();
	int GetEffectYakkyouTexture();
	void CleanupEffectTexture();
};

#endif