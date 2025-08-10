//! @file soundmanager.cpp
//! @brief SoundManager�N���X�̒�`

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

#include "soundmanager.h"

//! @brief �R���X�g���N�^
SoundManager::SoundManager(SoundControl *in_SoundCtrl, ResourceManager *in_Resource, ParameterInfo *in_Param)
{
	SoundCtrl = in_SoundCtrl;
	Resource = in_Resource;
	Param = in_Param;
	soundlistA = new soundlist[MAX_SOUNDMGR_LIST];
	soundlistB = new soundlist[MAX_SOUNDMGR_LIST];
	changeAB = false;
	listAdatas = 0;
	listBdatas = 0;
}

//! @brief �f�B�X�g���N�^
SoundManager::~SoundManager()
{
	if( soundlistA != NULL ){ delete [] soundlistA; }
	if( soundlistB != NULL ){ delete [] soundlistB; }
}

//! @brief �g�p����N���X��ݒ�
//! @param in_SoundCtrl �T�E���h�R���g���[���N���X
//! @param in_Resource ���\�[�X�Ǘ��N���X
//! @param in_Param �ݒ�l�Ǘ��N���X
//! @attention ���̊֐��Őݒ���s��Ȃ��ƁA�N���X���̂��������@�\���܂���B
void SoundManager::SetClass(SoundControl *in_SoundCtrl, ResourceManager *in_Resource, ParameterInfo *in_Param)
{
	SoundCtrl = in_SoundCtrl;
	Resource = in_Resource;
	Param = in_Param;
}

//! @brief ��Ԃ̉�����������
void SoundManager::InitWorldSound()
{
	changeAB = false;
	listAdatas = 0;
	listBdatas = 0;
}

//! @brief ��Ԃɔ��C����ǉ�
//! @param x ������X���W
//! @param y ������Y���W
//! @param z ������Z���W
//! @param id ����̎�ޔԍ�
//! @param teamID �`�[���ԍ�
//! @param player �v���C���[�ɂ�锭�C���ǂ����itrue�Ńv���C���[�j
//! @return �����Ftrue�@���s�Ffalse
bool SoundManager::ShotWeapon(float x, float y, float z, int id, int teamID, bool player)
{
	soundlist *plist = NULL;
	if( GetNewList(&plist) == false ){ return false; }

	if( player == true ){
		plist->paramid = SHOT_WEAPON_PLAYER;
	}
	else{
		plist->paramid = SHOT_WEAPON;
	}
	plist->dataid = id;
	plist->x = x;
	plist->y = y;
	plist->z = z;
	plist->teamid = teamID;

	return true;
}

//! @brief ��ԂɃ}�b�v���e����ǉ�
//! @param x ������X���W
//! @param y ������Y���W
//! @param z ������Z���W
//! @param teamID �`�[���ԍ�
//! @return �����Ftrue�@���s�Ffalse
bool SoundManager::HitMap(float x, float y, float z, int teamID)
{
	soundlist *plist = NULL;
	if( GetNewList(&plist) == false ){ return false; }

	plist->paramid = HIT_MAP;
	plist->x = x;
	plist->y = y;
	plist->z = z;
	plist->teamid = teamID;

	return true;
}

//! @brief ��Ԃɔ�e����ǉ�
//! @param x ������X���W
//! @param y ������Y���W
//! @param z ������Z���W
//! @param Hit_id ��e�ӏ��i���F0�@���́F1�@���F2�@�]���r�U���F3�j
//! @param teamID �`�[���ԍ�
//! @return �����Ftrue�@���s�Ffalse
bool SoundManager::HitHuman(float x, float y, float z, int Hit_id, int teamID)
{
	soundlist *plist = NULL;
	if( GetNewList(&plist) == false ){ return false; }

	if( (Hit_id < 0)||(3 < Hit_id) ){ return false; }

	if( Hit_id == 0 ){ plist->paramid = HIT_HUMAN_HEAD; }
	if( Hit_id == 1 ){ plist->paramid = HIT_HUMAN_UP; }
	if( Hit_id == 2 ){ plist->paramid = HIT_HUMAN_LEG; }
	if( Hit_id == 3 ){ plist->paramid = HIT_HUMAN_ZOMBIE; }
	plist->x = x;
	plist->y = y;
	plist->z = z;
	plist->teamid = teamID;

	return true;
}

//! @brief ��Ԃɏ������e����ǉ�
//! @param x ������X���W
//! @param y ������Y���W
//! @param z ������Z���W
//! @param id �����̎�ޔԍ�
//! @param teamID �`�[���ԍ�
//! @return �����Ftrue�@���s�Ffalse
bool SoundManager::HitSmallObject(float x, float y, float z, int id, int teamID)
{
	soundlist *plist = NULL;
	if( GetNewList(&plist) == false ){ return false; }

	plist->paramid = HIT_SMALLOBJECT;
	plist->dataid = id;
	plist->x = x;
	plist->y = y;
	plist->z = z;
	plist->teamid = teamID;

	return true;
}

//! @brief �e�̒ʉ߁E���؂鉹��ǉ�
//! @param x ������X���W
//! @param y ������Y���W
//! @param z ������Z���W
//! @param move_x ������X���ړ���
//! @param move_y ������Y���ړ���
//! @param move_z ������Z���ړ���
//! @param teamID �`�[���ԍ�
//! @return �����Ftrue�@���s�Ffalse
//! @attention move_x�Emove_y�Emove_z�̈ړ��ʂ́A1�t���[�������w�肵�Ă��������B���ۂɍ��W���ړ�����킯�ł͂���܂���B
bool SoundManager::PassingBullet(float x, float y, float z, float move_x, float move_y, float move_z, int teamID)
{
	soundlist *plist = NULL;
	if( GetNewList(&plist) == false ){ return false; }

	plist->paramid = BULLET;
	plist->x = x;
	plist->y = y;
	plist->z = z;
	plist->move_x = move_x;
	plist->move_y = move_y;
	plist->move_z = move_z;
	plist->teamid = teamID;

	return true;
}

//! @brief ��ԂɎ�֒e�o�E���h�E���˕Ԃ艹��ǉ�
//! @param x ������X���W
//! @param y ������Y���W
//! @param z ������Z���W
//! @param teamID �`�[���ԍ�
//! @return �����Ftrue�@���s�Ffalse
bool SoundManager::GrenadeBound(float x, float y, float z, int teamID)
{
	soundlist *plist = NULL;
	if( GetNewList(&plist) == false ){ return false; }

	plist->paramid = GRE_BOUND;
	plist->x = x;
	plist->y = y;
	plist->z = z;
	plist->teamid = teamID;

	return true;
}

//! @brief ��ԂɎ�֒e��������ǉ�
//! @param x ������X���W
//! @param y ������Y���W
//! @param z ������Z���W
//! @param teamID �`�[���ԍ�
//! @return �����Ftrue�@���s�Ffalse
bool SoundManager::GrenadeExplosion(float x, float y, float z, int teamID)
{
	soundlist *plist = NULL;
	if( GetNewList(&plist) == false ){ return false; }

	plist->paramid = GRE_EXPLOSION;
	plist->x = x;
	plist->y = y;
	plist->z = z;
	plist->teamid = teamID;

	return true;
}

//! @brief ��Ԃɑ�����ǉ�
//! @param x ������X���W
//! @param y ������Y���W
//! @param z ������Z���W
//! @param id �����̎�ޔԍ�
//! @param MoveMode �ړ����[�h�i�����F0�@�O�i�F1�@��ށF2�@���E�F3�@�W�����v�F4�@���n�F5�j
//! @param teamID �`�[���ԍ�
//! @param cnt �J�E���g�l
//! @return �����Ftrue�@���s�Ffalse
bool SoundManager::SetFootsteps(float x, float y, float z, int id, int MoveMode, int teamID, int cnt)
{
	soundlist *plist = NULL;
	if( GetNewList(&plist) == false ){ return false; }

	switch(MoveMode){
		case 0: plist->paramid = FOOTSTEPS_WALK; break;
		case 1: plist->paramid = FOOTSTEPS_FORWARD; break;
		case 2: plist->paramid = FOOTSTEPS_BACK; break;
		case 3: plist->paramid = FOOTSTEPS_SIDE; break;
		case 4: plist->paramid = FOOTSTEPS_JUMP; break;
		case 5: plist->paramid = FOOTSTEPS_LANDING; break;
		default: return false;
	}
	plist->dataid = id;
	plist->x = x;
	plist->y = y;
	plist->z = z;
	plist->teamid = teamID;
	plist->cnt = cnt;

	return true;
}

//! @brief ��Ԃɕ��탊���[�h����ǉ�
//! @param x ������X���W
//! @param y ������Y���W
//! @param z ������Z���W
//! @param teamID �`�[���ԍ�
//! @return �����Ftrue�@���s�Ffalse
bool SoundManager::ReloadWeapon(float x, float y, float z, int teamID)
{
	soundlist *plist = NULL;
	if( GetNewList(&plist) == false ){ return false; }

	plist->paramid = WEAPON_RELOAD;
	plist->x = x;
	plist->y = y;
	plist->z = z;
	plist->teamid = teamID;

	return true;
}

//! @brief ��ԁi�T�E���h���X�g�j��ŗL���ȉ��������擾
//! @return �L���ȉ�����
int SoundManager::GetTotalSoundList()
{
	if( changeAB == false ){
		return listBdatas;
	}
	//else{
		return listAdatas;
	//}
}

//! @brief �w�肵���ʒu�̎��ӂɂ��鉹�����擾
//! @param pos_x ������X���W
//! @param pos_y ������Y���W
//! @param pos_z ������Z���W
//! @param teamID �`�[���ԍ�
//! @param psoundlist �T�E���h���X�g���󂯎��|�C���^
//! @return �Ԃ��i�T�E���h���X�g�́j�����̐�
int SoundManager::GetWorldSound(float pos_x, float pos_y, float pos_z, int teamID, soundlist *psoundlist)
{
	if( psoundlist == NULL ){ return 0; }

	int lists;
	soundlist *getlist = NULL;
	int newlists = 0;
	WeaponParameter WParam;

	//�������郊�X�g������
	lists = GetTargetList(&getlist);

	for(int i=0; i<lists; i++){
		float x, y, z;
		float maxdist = 20.0f;	//���苗���̕W���l

		//�����̎�ނɂ�蔻�苗����ς���
		if( (getlist[i].paramid == SHOT_WEAPON)||(getlist[i].paramid == SHOT_WEAPON_PLAYER) ){
			Param->GetWeapon(getlist[i].dataid, &WParam);

			if( getlist[i].teamid == teamID ){ maxdist = 20.0f; }
			else if( WParam.silencer == true ){ maxdist = 30.0f; }
			else{ maxdist = 120.0f; }
		}
		if( getlist[i].paramid == HIT_MAP ){
			maxdist = 30.0f;
		}
		if( getlist[i].paramid == HIT_HUMAN_HEAD ){
			maxdist = 60.0f;
		}
		if( getlist[i].paramid == HIT_HUMAN_UP ){
			maxdist = 50.0f;
		}
		if( getlist[i].paramid == HIT_HUMAN_LEG ){
			maxdist = 40.0f;
		}
		if( getlist[i].paramid == HIT_HUMAN_ZOMBIE ){
			maxdist = 40.0f;
		}
		if( getlist[i].paramid == FOOTSTEPS_WALK ){
			continue;
		}
		if( getlist[i].paramid == FOOTSTEPS_FORWARD ){
			if( getlist[i].teamid == teamID ){ continue; }
			else{ maxdist = 40.0f; }
		}
		if( getlist[i].paramid == FOOTSTEPS_BACK ){
			if( getlist[i].teamid == teamID ){ continue; }
			else{ maxdist = 30.0f; }
		}
		if( getlist[i].paramid == FOOTSTEPS_SIDE ){
			if( getlist[i].teamid == teamID ){ continue; }
			else{ maxdist = 35.0f; }
		}
		if( (getlist[i].paramid == FOOTSTEPS_JUMP)||(getlist[i].paramid == FOOTSTEPS_LANDING) ){
			continue;
		}
		if( getlist[i].paramid == GRE_BOUND ){
			continue;
		}
		if( getlist[i].paramid == GRE_EXPLOSION ){
			maxdist = 80.0f;
		}
		if( getlist[i].paramid == WEAPON_RELOAD ){
			continue;
		}

#ifdef ENABLE_BUG_TEAMID
		//�`�[���ԍ��������A�������̃`�[���ԍ����傫���Ȃ�A�����𖳎�
		if( (teamID < 0)&&(getlist[i].teamid < 0) ){
			if( teamID < getlist[i].teamid ){
				continue;
			}
		}
#endif

		//�e�e�Ȃ��
		if( getlist[i].paramid == BULLET ){
			float min_x, min_y, min_z;

			//�e�e���ł��߂Â����W���Z�o
			if( CheckApproach(&(getlist[i]), pos_x, pos_y, pos_z, &min_x, &min_y, &min_z) == false ){ continue; }

			//�����Ƃ̋������͈͓��Ȃ�΁`
			x = min_x - pos_x;
			y = min_y - pos_y;
			z = min_z - pos_z;
			if( x*x + y*y + z*z < maxdist*maxdist ){
				//�o�͐�̃��X�g�ɒǉ�
				psoundlist[newlists].paramid = getlist[i].paramid;
				psoundlist[newlists].dataid = getlist[i].dataid;
				psoundlist[newlists].x = min_x;
				psoundlist[newlists].y = min_y;
				psoundlist[newlists].z = min_z;
				newlists += 1;
			}
		}
		else{
			//�����Ƃ̋������͈͓��Ȃ�΁`
			x = getlist[i].x - pos_x;
			y = getlist[i].y - pos_y;
			z = getlist[i].z - pos_z;
			if( x*x + y*y + z*z < maxdist*maxdist ){
				//�o�͐�̃��X�g�ɒǉ�
				psoundlist[newlists].paramid = getlist[i].paramid;
				psoundlist[newlists].dataid = getlist[i].dataid;
				psoundlist[newlists].x = getlist[i].x;
				psoundlist[newlists].y = getlist[i].y;
				psoundlist[newlists].z = getlist[i].z;
				newlists += 1;
			}
		}
	}

	return newlists;
}

//! @brief ��ԏ�̃T�E���h���Đ�
//! @param camera_x �J������X���W
//! @param camera_y �J������Y���W
//! @param camera_z �J������Z���W
//! @param camera_rx �J������X���p�x�@�i�\��j
//! @param teamID �`�[���ԍ�
//! @warning ���t���[���Ăяo���Ă��������B
void SoundManager::PlayWorldSound(float camera_x, float camera_y, float camera_z, float camera_rx, int teamID)
{
	int lists;
	soundlist *getlist = NULL;

	//�t���O��؂�ւ���
	if( changeAB == false ){
		listBdatas = 0;
		changeAB = true;
	}
	else{
		listAdatas = 0;
		changeAB = false;
	}

	//�J�������W��ݒ�
	SoundCtrl->SetCamera(camera_x, camera_y, camera_z, camera_rx);

	//�������郊�X�g������
	lists = GetTargetList(&getlist);

	for(int i=0; i<lists; i++){
		float x = getlist[i].x - camera_x;
		float y = getlist[i].y - camera_y;
		float z = getlist[i].z - camera_z;

		//�͈͓��̉����Ȃ�΍Đ������݂�
		if( x*x + y*y + z*z < MAX_SOUNDDIST*MAX_SOUNDDIST ){
			PlaySound( &(getlist[i]), camera_x, camera_y, camera_z, teamID );
		}
	}
}

//! @brief �V�����T�E���h���X�g�i�P��j���擾
//! @param plist �V�����T�E���h���X�g�i�P��j�̓�d�|�C���^
//! @return �����Ftrue�@���s�Ffalse
bool SoundManager::GetNewList(soundlist **plist)
{
	if( changeAB == false ){
		if( (listAdatas + 1) >= MAX_SOUNDMGR_LIST ){
			return false;
		}
		*plist = &(soundlistA[listAdatas]);
		listAdatas += 1;
	}
	else{
		if( (listBdatas + 1) >= MAX_SOUNDMGR_LIST ){
			return false;
		}
		*plist = &(soundlistB[listBdatas]);
		listBdatas += 1;
	}

	return true;
}

//! @brief �����Ώۂ̃T�E���h���X�g���擾
//! @param plist �����Ώۂ̃T�E���h���X�g�̓�d�|�C���^
//! @return �擾�����T�E���h���X�g�Ɋ܂܂�鉹���̐�
int SoundManager::GetTargetList(soundlist **plist)
{
	if( changeAB == false ){
		*plist = soundlistB;
		return listBdatas;
	}
	//else{
		*plist = soundlistA;
		return listAdatas;
	//}
}

//! @brief �������J�����i���_�j�ɍł��߂Â����m�F
//! @param plist �����Ώۂ̃T�E���h���X�g�i�P��j�̃|�C���^
//! @param camera_x �J������X���W
//! @param camera_y �J������Y���W
//! @param camera_z �J������Z���W
//! @param min_x �ŒZ������X���W���󂯎��|�C���^
//! @param min_y �ŒZ������Y���W���󂯎��|�C���^
//! @param min_z �ŒZ������Z���W���󂯎��|�C���^
//! @return �ʂ�߂����Ftrue�@�ʂ�߂��Ă��Ȃ��Ffalse
bool SoundManager::CheckApproach(soundlist *plist, float camera_x, float camera_y, float camera_z, float *min_x, float *min_y, float *min_z)
{
	float x, y, z;
	float dist1, dist2, dist3;

	//1�t���[���O�̋���
	x = camera_x - (plist->x - plist->move_x);
	y = camera_y - (plist->y - plist->move_y);
	z = camera_z - (plist->z - plist->move_z);
	dist1 = x*x + y*y + z*z;

	//���݈ʒu�̋���
	x = camera_x - plist->x;
	y = camera_y - plist->y;
	z = camera_z - plist->z;
	dist2 = x*x + y*y + z*z;

	//1�t���[����̋���
	x = camera_x - (plist->x + plist->move_x);
	y = camera_y - (plist->y + plist->move_y);
	z = camera_z - (plist->z + plist->move_z);
	dist3 = x*x + y*y + z*z;

	//���݈ʒu�̋������ł��߂���΁`
	if( (dist1 > dist2)&&(dist2 < dist3) ){
		float speed;
		float min_dist, dist;

		//�ړ����x�����߂�
		speed = sqrtf(plist->move_x*plist->move_x + plist->move_y*plist->move_y + plist->move_z*plist->move_z);

		//�ŒZ�����̍��W�����߂�
		min_dist = DistancePosRay(camera_x, camera_y, camera_z, plist->x, plist->y, plist->z, plist->move_x/speed, plist->move_y/speed, plist->move_z/speed);

		//�ŒZ�������̍��W�����߂�
		dist = sqrtf(dist2 - min_dist*min_dist);
		*min_x = plist->x + plist->move_x/speed * dist;
		*min_y = plist->y + plist->move_y/speed * dist;
		*min_z = plist->z + plist->move_z/speed * dist;

		return true;
	}

	return false;
}

//! @brief �w�肵���T�E���h���X�g�i1�����j���Đ�
//! @param plist �Đ�����T�E���h���X�g�i�P��j�̃|�C���^
//! @param camera_x �J������X���W
//! @param camera_y �J������Y���W
//! @param camera_z �J������Z���W
//! @param teamID �`�[���ԍ�
void SoundManager::PlaySound(soundlist *plist, float camera_x, float camera_y, float camera_z, int teamID)
{
	WeaponParameter WParam;
	int hitsoundA, hitsoundB;
	int hitsound;
	int ccosound;
	int bangsound;

	int id = -1;
	int volume = 0;

	//�Đ�������ݒ�
	switch(plist->paramid){
		case SHOT_WEAPON:		//���C��
			Param->GetWeapon(plist->dataid, &WParam);
			if( WParam.soundvolume == 0 ){ return; }

			id = Resource->GetWeaponSound(plist->dataid);
			volume = WParam.soundvolume;
			break;

		case SHOT_WEAPON_PLAYER:	//�v���C���[���g�̔��C��
			Param->GetWeapon(plist->dataid, &WParam);
			if( WParam.soundvolume == 0 ){ return; }

			//3D�����Ƃ����ɂ��̂܂܍Đ����ďI��
			SoundCtrl->PlaySound(Resource->GetWeaponSound(plist->dataid), WParam.soundvolume, 0);
			return;

		case HIT_MAP:			//�}�b�v���e��
			Resource->GetBulletSound(&hitsoundA, &hitsoundB, NULL, NULL, NULL, NULL);
			if( GetRand(3) == 0 ){
				id = hitsoundB;
			}
			else{
				id = hitsoundA;
			}
			volume = MAX_SOUNDHITMAP;
			break;

		case HIT_HUMAN_HEAD:			//��e��
		case HIT_HUMAN_UP:
		case HIT_HUMAN_LEG:
		case HIT_HUMAN_ZOMBIE:
			Resource->GetBulletSound(NULL, NULL, &hitsound, NULL, NULL, NULL);
			id = hitsound;
			volume = MAX_SOUNDHITHUMAN;
			break;

		case HIT_SMALLOBJECT:	//�����j��
			id = Resource->GetSmallObjectSound(plist->dataid);
			volume = MAX_SOUNDHITSMALLOBJ;
			break;

		case BULLET:			//�e�e�̉��E���؂鉹
			float new_x, new_y, new_z;
			int passingsound;

			if( CheckApproach(plist, camera_x, camera_y, camera_z, &new_x, &new_y, &new_z) == false ){ return; }

			//�����̒e�Ȃ牽�������I��
			if( plist->teamid == teamID ){ return; }

			//���̂܂܍Đ����ďI��
			Resource->GetBulletSound(NULL, NULL, NULL, &passingsound, NULL, NULL);
			SoundCtrl->Play3DSound(passingsound, new_x, new_y, new_z, MAX_SOUNDPASSING);
			return;

		case GRE_BOUND:			//��֒e �o�E���h��
			Resource->GetBulletSound(NULL, NULL, NULL, NULL, NULL, &ccosound);
			id = ccosound;
			volume = MAX_SOUNDCCOGRENADE;
			break;

		case GRE_EXPLOSION:		//��֒e ������
			Resource->GetBulletSound(NULL, NULL, NULL, NULL, &bangsound, NULL);
			id = bangsound;
			volume = MAX_SOUNDHITGRENADE;
			break;

		case FOOTSTEPS_WALK:			//����
		case FOOTSTEPS_FORWARD:
		case FOOTSTEPS_BACK:
		case FOOTSTEPS_SIDE:
			//�������Đ����鏈��������
			//�@�ʓr�����̃T�E���h�f�[�^��ǂݍ��񂾏�ŁA
			//�@plist->cnt�i�ړ����[�V�����̃J�E���g�l�j�����Ɉ��Ԋu�ŁA
			//�@plist->dataid�i�����̎�ޔԍ��j�ɍ��킹�������Đ�����΂悢�B
			//break;
			return;	//�����Đ������ɕԂ�

		case FOOTSTEPS_JUMP:			//�W�����v���̉�
		case FOOTSTEPS_LANDING:			//���n���̉�
			//�W�����v���E���n�����Đ����鏈��������
			//�@plist->dataid�i�����̎�ޔԍ��j���Q�Ƃł���B
			//break;
			return;	//�����Đ������ɕԂ�

		case WEAPON_RELOAD:			//�����[�h��
			id = Resource->GetWeaponSound(-1);
			volume = 100;
			break;

		default:
			return;
	}

	//�Đ�
	SoundCtrl->Play3DSound(id, plist->x, plist->y, plist->z, volume);
}