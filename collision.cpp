//! @file collision.cpp
//! @brief Collision�N���X�̒�`

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

#include "collision.h"

//! @brief �R���X�g���N�^
Collision::Collision()
{
	BlockDataIF = NULL;
	cbdata = new Coll_Blockdata[MAX_BLOCKS];
	for(int i=0; i<MAX_BLOCKS; i++){
		cbdata[i].min_x = 0.0f;
		cbdata[i].min_y = 0.0f;
		cbdata[i].min_z = 0.0f;
		cbdata[i].max_x = 0.0f;
		cbdata[i].max_y = 0.0f;
		cbdata[i].max_z = 0.0f;
		cbdata[i].BoardBlock = false;
		cbdata[i].worldgroup = 0;
	}
}

//! @brief �f�B�X�g���N�^
Collision::~Collision()
{
	if( cbdata != NULL ){ delete [] cbdata; }
}

//! @brief �u���b�N�f�[�^����荞��
//! @param in_BlockDataIF �u���b�N�f�[�^
int Collision::InitCollision(BlockDataInterface* in_BlockDataIF)
{
	int bs;
	blockdata data;
	int vID[4];

	if( in_BlockDataIF == NULL ){ return 1; }
	BlockDataIF = in_BlockDataIF;

	bs = BlockDataIF->GetTotaldatas();

	for(int i=0; i<bs; i++){
		cbdata[i].BoardBlock = false;
	}

	for(int i=0; i<bs; i++){
		float cx = 0.0f;
		float cy = 0.0f;
		float cz = 0.0f;
		float x, y, z, rx, ry, r;

		BlockDataIF->Getdata(&data, i);

		//���S�����߂�
		for(int j=0; j<8; j++){
			cx += data.x[j];
			cy += data.y[j];
			cz += data.z[j];
		}
		cx /= 8;
		cy /= 8;
		cz /= 8;

		//�e���_�̍��W���Čv�Z
		for(int j=0; j<8; j++){
			//���S����̈ʒu�֌W���Z�o
			x = data.x[j] - cx;
			y = data.y[j] - cy;
			z = data.z[j] - cz;
			rx = atan2f(z, x);
			ry = atan2f(y, sqrtf(x*x + z*z));
			r = sqrtf(x*x + y*y + z*z);

			//�傫������
			r += COLLISION_ADDSIZE;

			//���W���Čv�Z
			cbdata[i].x[j] = cx + cosf(rx) * cosf(ry) * r;
			cbdata[i].y[j] = cy + sinf(ry) * r;
			cbdata[i].z[j] = cz + sinf(rx) * cosf(ry) * r;
		}
	}

	//����p���ʂ̒��S�_�Z�o
	for(int i=0; i<bs; i++){
		for(int j=0; j<6; j++){
			blockdataface(j, &vID[0], NULL);

			cbdata[i].polygon_center_x[j] = 0.0f;
			cbdata[i].polygon_center_y[j] = 0.0f;
			cbdata[i].polygon_center_z[j] = 0.0f;

			//4���_�̒��S�_���Z�o
			for(int k=0; k<4; k++){
				cbdata[i].polygon_center_x[j] += cbdata[i].x[ vID[k] ];
				cbdata[i].polygon_center_y[j] += cbdata[i].y[ vID[k] ];
				cbdata[i].polygon_center_z[j] += cbdata[i].z[ vID[k] ];
			}
			cbdata[i].polygon_center_x[j] /= 4;
			cbdata[i].polygon_center_y[j] /= 4;
			cbdata[i].polygon_center_z[j] /= 4;
		}
	}

	//��̃u���b�N�����o
	for(int i=0; i<bs; i++){
		cbdata[i].BoardBlock = false;

		for(int j=0; j<8; j++){
			for(int k=j+1; k<8; k++){
				//�ꃕ���ł����_�������Ȃ�A��ɂȂ��Ă���Ɣ���B
				if( (cbdata[i].x[j] == cbdata[i].x[k])&&(cbdata[i].y[j] == cbdata[i].y[k])&&(cbdata[i].z[j] == cbdata[i].z[k]) ){
					cbdata[i].BoardBlock = true;
					j = 8; k = 8;	//break
				}
			}
		}

		//�u���b�N�̒��S���Z�o
		float mx = 0.0f;
		float my = 0.0f;
		float mz = 0.0f;
		for(int j=0; j<8; j++){
			mx += cbdata[i].x[j];
			my += cbdata[i].y[j];
			mz += cbdata[i].z[j];
		}
		mx /= 8;
		my /= 8;
		mz /= 8;

		//�u���b�N�̒��S�_�ɑ΂���1�ʂł��\�ʂȂ�΁A��ɂȂ��Ă���Ɣ���B
		for(int j=0; j<6; j++){
			if( CheckPolygonFront(i, j, mx, my, mz) == true ){
				cbdata[i].BoardBlock = true;
				break;
			}
		}
	}


	//�u���b�NAABB�쐬
	for(int i=0; i<bs; i++){
		GetBlockPosMINMAX(i, &cbdata[i].min_x, &cbdata[i].min_y, &cbdata[i].min_z, &cbdata[i].max_x, &cbdata[i].max_y, &cbdata[i].max_z);
	}

	//�u���b�N�̋�ԕ����O���[�v���v�Z
	for(int i=0; i<bs; i++){
		cbdata[i].worldgroup = GetWorldGroup(cbdata[i].min_x, cbdata[i].min_z);
		if( GetWorldGroup(cbdata[i].max_x, cbdata[i].max_z) != cbdata[i].worldgroup ){
			cbdata[i].worldgroup = 0;
		}
	}

	return 0;
}

//! @brief �u���b�N�̍��W�ő�l�E�ŏ��l��Ԃ�
//! @param id ���肷��u���b�N�ԍ�
//! @param *min_x �ŏ� X���W��Ԃ��|�C���^
//! @param *min_y �ŏ� Y���W��Ԃ��|�C���^
//! @param *min_z �ŏ� Z���W��Ԃ��|�C���^
//! @param *max_x �ő� X���W��Ԃ��|�C���^
//! @param *max_y �ő� Y���W��Ԃ��|�C���^
//! @param *max_z �ő� Z���W��Ԃ��|�C���^
void Collision::GetBlockPosMINMAX(int id, float *min_x, float *min_y, float *min_z, float *max_x, float *max_y, float *max_z)
{
	*min_x = cbdata[id].x[0];
	*min_y = cbdata[id].y[0];
	*min_z = cbdata[id].z[0];
	*max_x = cbdata[id].x[0];
	*max_y = cbdata[id].y[0];
	*max_z = cbdata[id].z[0];
	for(int i=1; i<8; i++){
		if( *min_x > cbdata[id].x[i] ){ *min_x = cbdata[id].x[i]; }
		if( *min_y > cbdata[id].y[i] ){ *min_y = cbdata[id].y[i]; }
		if( *min_z > cbdata[id].z[i] ){ *min_z = cbdata[id].z[i]; }

		if( *max_x < cbdata[id].x[i] ){ *max_x = cbdata[id].x[i]; }
		if( *max_y < cbdata[id].y[i] ){ *max_y = cbdata[id].y[i]; }
		if( *max_z < cbdata[id].z[i] ){ *max_z = cbdata[id].z[i]; }
	}

	//�v�Z�덷�΍�̂��߁A�킸���ɑ傫�߂ɂ���B
	*min_x -= COLLISION_ADDSIZE;
	*min_y -= COLLISION_ADDSIZE;
	*min_z -= COLLISION_ADDSIZE;
	*max_x += COLLISION_ADDSIZE;
	*max_y += COLLISION_ADDSIZE;
	*max_z += COLLISION_ADDSIZE;
}

//! @brief ��ԕ����̃O���[�v���Z�o
//! @param x X���W
//! @param z Z���W
//! @return �O���[�v�ԍ�
//! @attention �u�O���[�v�ԍ��v�̊T�O�͕ʓr�h�L�������g���Q��
int Collision::GetWorldGroup(float x, float z)
{
	if( (x > 0)&&(z > 0) ){ return 1; }
	if( (x < 0)&&(z > 0) ){ return 2; }
	if( (x < 0)&&(z < 0) ){ return 3; }
	if( (x > 0)&&(z < 0) ){ return 4; }
	return 0;
}

//! @brief AABB�ɂ��u���b�N�Ƃ̓����蔻��
//! @param id ���肷��u���b�N�ԍ�
//! @param min_x ���̂̍ŏ� X���W
//! @param min_y ���̂̍ŏ� Y���W
//! @param min_z ���̂̍ŏ� Z���W
//! @param max_x ���̂̍ő� X���W
//! @param max_y ���̂̍ő� Y���W
//! @param max_z ���̂̍ő� Z���W
//! @return �������Ă���Ftrue�@�������Ă��Ȃ��Ffalse
//! @attention �G���[������ꍇ�u�������Ă��Ȃ��Ffalse�v���Ԃ���܂��B
bool Collision::CheckBlockAABB(int id, float min_x, float min_y, float min_z, float max_x, float max_y, float max_z)
{
	if( BlockDataIF == NULL ){ return false; }
	if( (id < 0)||(BlockDataIF->GetTotaldatas() <= id) ){ return false; }

	//��̃u���b�N�͌v�Z�O
	if( cbdata[id].BoardBlock == true ){
		return false;
	}

	return CollideBoxAABB(min_x, min_y, min_z, max_x, max_y, max_z, cbdata[id].min_x, cbdata[id].min_y, cbdata[id].min_z, cbdata[id].max_x, cbdata[id].max_y, cbdata[id].max_z);
}

//! @brief AABB�ɂ��S�Ẵu���b�N�Ƃ̓����蔻��
//! @param min_x ���̂̍ŏ� X���W
//! @param min_y ���̂̍ŏ� Y���W
//! @param min_z ���̂̍ŏ� Z���W
//! @param max_x ���̂̍ő� X���W
//! @param max_y ���̂̍ő� Y���W
//! @param max_z ���̂̍ő� Z���W
//! @return �������Ă���Ftrue�@�������Ă��Ȃ��Ffalse
//! @attention �G���[������ꍇ�u�������Ă��Ȃ��Ffalse�v���Ԃ���܂��B
bool Collision::CheckALLBlockAABB(float min_x, float min_y, float min_z, float max_x, float max_y, float max_z)
{
	if( BlockDataIF == NULL ){ return false; }

	int bs = BlockDataIF->GetTotaldatas();

	for(int i=0; i<bs; i++){
		if( CheckBlockAABB(i, min_x, min_y, min_z, max_x, max_y, max_z) == true ){ return true; }
	}

	return false;
}

//! @brief ����̍��W���A�u���b�N�̖ʂ̕\�������ׂ�
//! @param id ���肷��u���b�N�ԍ�
//! @param face ���肷��ʔԍ�
//! @param x X���W
//! @param y Y���W
//! @param z Z���W
//! @return �\�����Ftrue�@�������Ffalse
//! @warning ��΍��W���w�肷��K�v������܂��B�x�N�g���ł͂���܂���B
//! @attention �e�����蔻��̊֐����玩���I�ɌĂ΂�܂��B
bool Collision::CheckPolygonFront(int id, int face, float x, float y, float z)
{
	if( BlockDataIF == NULL ){ return false; }
	if( (id < 0)||(BlockDataIF->GetTotaldatas() <= id) ){ return false; }
	if( (face < 0)||(6 < face) ){ return false; }

	blockdata bdata;
	float vx, vy, vz;
	float d;

	BlockDataIF->Getdata(&bdata, id);

	//�ʂ̒��S����_�ɁA���W�̃x�N�g�������߂�
	vx = cbdata[id].polygon_center_x[face] - x;
	vy = cbdata[id].polygon_center_y[face] - y;
	vz = cbdata[id].polygon_center_z[face] - z;

	//����
	d = bdata.material[face].vx*vx + bdata.material[face].vy*vy + bdata.material[face].vz*vz;

	if( d <= 0.0f ){
		return true;
	}
	return false;
}

//! @brief ����̕����ɑ΂��āA�u���b�N�̖ʂ̕\�������ׂ�
//! @param id ���肷��u���b�N�ԍ�
//! @param face ���肷��ʔԍ�
//! @param rx �������̊p�x
//! @return �\�����Ftrue�@�������Ffalse
bool Collision::CheckPolygonFrontRx(int id, int face, float rx)
{
	if( BlockDataIF == NULL ){ return false; }
	if( (id < 0)||(BlockDataIF->GetTotaldatas() <= id) ){ return false; }
	if( (face < 0)||(6 < face) ){ return false; }

	blockdata bdata;
	float vx, vz;
	float d;

	BlockDataIF->Getdata(&bdata, id);

	//�x�N�g���Z�o
	vx = cosf(rx);
	vz = sinf(rx);

	//����
	d = bdata.material[face].vx*vx + bdata.material[face].vz*vz;

	if( d <= 0.0f ){
		return true;
	}
	return false;
}

//! @brief �u���b�N�̖ʂƃ��C�i�����j�̓����蔻��
//! @param blockid ���肷��u���b�N�ԍ�
//! @param face ���肷��ʔԍ�
//! @param RayPos_x ���C�̈ʒu�i�n�_�j���w�肷�� X���W
//! @param RayPos_y ���C�̈ʒu�i�n�_�j���w�肷�� Y���W
//! @param RayPos_z ���C�̈ʒu�i�n�_�j���w�肷�� Z���W
//! @param RayDir_x ���C�̃x�N�g�����w�肷�� X����
//! @param RayDir_y ���C�̃x�N�g�����w�肷�� Y����
//! @param RayDir_z ���C�̃x�N�g�����w�肷�� Z����
//! @param out_Dist ���������u���b�N�Ƃ̋������󂯎��|�C���^
//! @return �������Ă���Ftrue�@�������Ă��Ȃ��Ffalse
//! @warning RayPos�i�n�_�j�� RayDir�i�x�N�g���j���ԈႦ�Ȃ����ƁB
//! @attention �e�����蔻��̊֐����玩���I�ɌĂ΂�܂��B
bool Collision::CheckIntersectTri(int blockid, int face, float RayPos_x, float RayPos_y, float RayPos_z, float RayDir_x, float RayDir_y, float RayDir_z, float *out_Dist)
{
	blockdata data;
	int vID[4];
	float d1, d2;
	float vx1, vy1, vz1;
	float dist;
	float x, y, z;
	float vx2, vy2, vz2;

	BlockDataIF->Getdata(&data, blockid);
	blockdataface(face, vID, NULL);

	//����
	d1 = data.material[face].vx*RayDir_x + data.material[face].vy*RayDir_y + data.material[face].vz*RayDir_z;

	if( d1 >= 0.0f ){
		return false;		//�ʂƃ��C�����s���A�ʂɑ΂��ă��C���t����
	}

	//�ʂ̒��S����_�ɁA���W�̃x�N�g�������߂�
	vx1 = RayPos_x - cbdata[blockid].polygon_center_x[face];
	vy1 = RayPos_y - cbdata[blockid].polygon_center_y[face];
	vz1 = RayPos_z - cbdata[blockid].polygon_center_z[face];

	//����
	d2 = data.material[face].vx*vx1 + data.material[face].vy*vy1 + data.material[face].vz*vz1;		//�ʂ܂ł̍ŒZ���������܂�

	//��_�܂ł̋����ƍ��W
	dist = 1.0f / (d1*-1) * d2;
	x = RayPos_x + RayDir_x * dist;
	y = RayPos_y + RayDir_y * dist;
	z = RayPos_z + RayDir_z * dist;


	//�u���b�N�S�̂�AABB�ɓ���Ȃ���Ώ��O
	if( (x < cbdata[blockid].min_x)||(cbdata[blockid].max_x < x) ){ return false; }
	if( (y < cbdata[blockid].min_y)||(cbdata[blockid].max_y < y) ){ return false; }
	if( (z < cbdata[blockid].min_z)||(cbdata[blockid].max_z < z) ){ return false; }


	//�ȉ��A�u���b�N�ʂ̓����Ɍ�_�����邩�`�F�b�N
	//�@�@�ʂ��`������e4�ӂƂ̈ʒu�֌W���Z�o���A�ʂ̖@���Ɣ�r����B

	//�O��
	vx2 = ((cbdata[blockid].y[ vID[1] ] - cbdata[blockid].y[ vID[0] ]) * (z - cbdata[blockid].z[ vID[0] ])) - ((y - cbdata[blockid].y[ vID[0] ]) * (cbdata[blockid].z[ vID[1] ] - cbdata[blockid].z[ vID[0] ]));
	vy2 = ((cbdata[blockid].z[ vID[1] ] - cbdata[blockid].z[ vID[0] ]) * (x - cbdata[blockid].x[ vID[0] ])) - ((z - cbdata[blockid].z[ vID[0] ]) * (cbdata[blockid].x[ vID[1] ] - cbdata[blockid].x[ vID[0] ]));
	vz2 = ((cbdata[blockid].x[ vID[1] ] - cbdata[blockid].x[ vID[0] ]) * (y - cbdata[blockid].y[ vID[0] ])) - ((x - cbdata[blockid].x[ vID[0] ]) * (cbdata[blockid].y[ vID[1] ] - cbdata[blockid].y[ vID[0] ]));

	//����
	d1 = data.material[face].vx*vx2 + data.material[face].vy*vy2 + data.material[face].vz*vz2;		//�u���b�N�ʂ̖@���Ƃ̊֌W���Z�o

	if( d1 < 0.0f ){	//�O���ɂ���Ώ��O
		return false;
	}


	//�O��
	vx2 = ((cbdata[blockid].y[ vID[2] ] - cbdata[blockid].y[ vID[1] ]) * (z - cbdata[blockid].z[ vID[1] ])) - ((y - cbdata[blockid].y[ vID[1] ]) * (cbdata[blockid].z[ vID[2] ] - cbdata[blockid].z[ vID[1] ]));
	vy2 = ((cbdata[blockid].z[ vID[2] ] - cbdata[blockid].z[ vID[1] ]) * (x - cbdata[blockid].x[ vID[1] ])) - ((z - cbdata[blockid].z[ vID[1] ]) * (cbdata[blockid].x[ vID[2] ] - cbdata[blockid].x[ vID[1] ]));
	vz2 = ((cbdata[blockid].x[ vID[2] ] - cbdata[blockid].x[ vID[1] ]) * (y - cbdata[blockid].y[ vID[1] ])) - ((x - cbdata[blockid].x[ vID[1] ]) * (cbdata[blockid].y[ vID[2] ] - cbdata[blockid].y[ vID[1] ]));

	//����
	d1 = data.material[face].vx*vx2 + data.material[face].vy*vy2 + data.material[face].vz*vz2;		//�u���b�N�ʂ̖@���Ƃ̊֌W���Z�o

	if( d1 < 0.0f ){	//�O���ɂ���Ώ��O
		return false;
	}


	//�O��
	vx2 = ((cbdata[blockid].y[ vID[3] ] - cbdata[blockid].y[ vID[2] ]) * (z - cbdata[blockid].z[ vID[2] ])) - ((y - cbdata[blockid].y[ vID[2] ]) * (cbdata[blockid].z[ vID[3] ] - cbdata[blockid].z[ vID[2] ]));
	vy2 = ((cbdata[blockid].z[ vID[3] ] - cbdata[blockid].z[ vID[2] ]) * (x - cbdata[blockid].x[ vID[2] ])) - ((z - cbdata[blockid].z[ vID[2] ]) * (cbdata[blockid].x[ vID[3] ] - cbdata[blockid].x[ vID[2] ]));
	vz2 = ((cbdata[blockid].x[ vID[3] ] - cbdata[blockid].x[ vID[2] ]) * (y - cbdata[blockid].y[ vID[2] ])) - ((x - cbdata[blockid].x[ vID[2] ]) * (cbdata[blockid].y[ vID[3] ] - cbdata[blockid].y[ vID[2] ]));

	//����
	d1 = data.material[face].vx*vx2 + data.material[face].vy*vy2 + data.material[face].vz*vz2;		//�u���b�N�ʂ̖@���Ƃ̊֌W���Z�o

	if( d1 < 0.0f ){	//�O���ɂ���Ώ��O
		return false;
	}


	//�O��
	vx2 = ((cbdata[blockid].y[ vID[0] ] - cbdata[blockid].y[ vID[3] ]) * (z - cbdata[blockid].z[ vID[3] ])) - ((y - cbdata[blockid].y[ vID[3] ]) * (cbdata[blockid].z[ vID[0] ] - cbdata[blockid].z[ vID[3] ]));
	vy2 = ((cbdata[blockid].z[ vID[0] ] - cbdata[blockid].z[ vID[3] ]) * (x - cbdata[blockid].x[ vID[3] ])) - ((z - cbdata[blockid].z[ vID[3] ]) * (cbdata[blockid].x[ vID[0] ] - cbdata[blockid].x[ vID[3] ]));
	vz2 = ((cbdata[blockid].x[ vID[0] ] - cbdata[blockid].x[ vID[3] ]) * (y - cbdata[blockid].y[ vID[3] ])) - ((x - cbdata[blockid].x[ vID[3] ]) * (cbdata[blockid].y[ vID[0] ] - cbdata[blockid].y[ vID[3] ]));

	//����
	d1 = data.material[face].vx*vx2 + data.material[face].vy*vy2 + data.material[face].vz*vz2;		//�u���b�N�ʂ̖@���Ƃ̊֌W���Z�o

	if( d1 < 0.0f ){	//�O���ɂ���Ώ��O
		return false;
	}


	*out_Dist = dist;

	return true;
}

//! @brief �u���b�N�ɖ��܂��Ă��Ȃ������ׂ�
//! @param blockid ���肷��u���b�N�ԍ�
//! @param x X���W
//! @param y Y���W
//! @param z Z���W
//! @param worldgroup ��Ԃ̃O���[�v�𗘗p���Čv�Z�ȗ������݂�itrue�F�L���E�v�Z�ȗ��@false�F�����E���S�����j
//! @param *planeid �\�ɂ���ʔԍ��iNULL�j
//! @return �����Ă���Ftrue�@�����Ă��Ȃ��Ffalse
//! @warning *planeid ���Ԃ��\�ʁi0�`5�j�́A�����̖ʂ��Y������ꍇ�ł��A�ŏ��Ɍ�����1�ʂ̂ݕԂ��܂��B
bool Collision::CheckBlockInside(int blockid, float x, float y, float z, bool worldgroup, int *planeid)
{
	if( BlockDataIF == NULL ){ return false; }
	if( (blockid < 0)||(BlockDataIF->GetTotaldatas() <= blockid) ){ return false; }

	//��̃u���b�N�͌v�Z�O
	if( cbdata[blockid].BoardBlock == true ){ return false; }


	//����̑e���
	if( worldgroup == true ){

		//��ԕ���
		if( cbdata[blockid].worldgroup != 0 ){
			//�ϑ��_�̋�Ԃ̃O���[�v���擾
			int worldgroup = GetWorldGroup(x, z);

			if( worldgroup != 0 ){
				//��Ԃ̃O���[�v���Ⴆ�Όv�Z�O
				if( cbdata[blockid].worldgroup != worldgroup ){
					return false;
				}
			}
		}

		//�͈͂Ō���
		if( (x < cbdata[blockid].min_x)||(cbdata[blockid].max_x < x) ){ return false; }
		if( (y < cbdata[blockid].min_y)||(cbdata[blockid].max_y < y) ){ return false; }
		if( (z < cbdata[blockid].min_z)||(cbdata[blockid].max_z < z) ){ return false; }
	}


	//6�ʂ��猩�đS�ė��ʂ��ǂ���
	for(int i=0; i<6; i++){
		if( CheckPolygonFront(blockid, i, x, y, z) == true ){
			if( planeid != NULL ){ *planeid = i; }
			return false;	//�\�ʂȂ�ΏI��
		}
	}

	return true;
}

//! @brief �S�Ẵu���b�N�ɖ��܂��Ă��Ȃ������ׂ�
//! @param x X���W
//! @param y Y���W
//! @param z Z���W
//! @return �����Ă���Ftrue�@�����Ă��Ȃ��Ffalse
bool Collision::CheckALLBlockInside(float x, float y, float z)
{
	if( BlockDataIF == NULL ){ return false; }

	int bs = BlockDataIF->GetTotaldatas();

	for(int i=0; i<bs; i++){
		if( CheckBlockInside(i, x, y, z, true, NULL) == true ){ return true; }
	}

	return false;
}

//! @brief �u���b�N�ƃ��C�i�����j�̓����蔻��
//! @param blockid ���肷��u���b�N�ԍ�
//! @param RayPos_x ���C�̈ʒu�i�n�_�j���w�肷�� X���W
//! @param RayPos_y ���C�̈ʒu�i�n�_�j���w�肷�� Y���W
//! @param RayPos_z ���C�̈ʒu�i�n�_�j���w�肷�� Z���W
//! @param RayDir_x ���C�̃x�N�g�����w�肷�� X����
//! @param RayDir_y ���C�̃x�N�g�����w�肷�� Y����
//! @param RayDir_z ���C�̃x�N�g�����w�肷�� Z����
//! @param face ���������u���b�N�̖ʔԍ��i0�`5�j���󂯎��|�C���^�@�iNULL�j
//! @param Dist ���������u���b�N�Ƃ̋������󂯎��|�C���^
//! @param maxDist ������s���ő勗���@�i0.0 �����Ŗ����E�����j
//! @return �������Ă���Ftrue�@�������Ă��Ȃ��Ffalse
//! @warning RayPos�i�n�_�j�� RayDir�i�x�N�g���j���ԈႦ�Ȃ����ƁB
//! @warning ������s���ő勗�����w�肵�Ȃ��ƁA�p�t�H�[�}���X���啝�ɒቺ���܂��B
//! @attention ���C�̎n�_���痠���ɂȂ�u���b�N�̖ʂ͖�������܂��B�������[���ɕό`��������̃u���b�N���������܂��B
//! @attention �܂��A���C�������̃u���b�N�ɓ�����ꍇ�́A���C�̎n�_�����ԋ߂������Ԃ��܂��B
bool Collision::CheckBlockIntersectRay(int blockid, float RayPos_x, float RayPos_y, float RayPos_z, float RayDir_x, float RayDir_y, float RayDir_z, int *face, float *Dist, float maxDist)
{
	if( BlockDataIF == NULL ){ return false; }
	if( (blockid < 0)||(BlockDataIF->GetTotaldatas() <= blockid) ){ return false; }
	if( (RayDir_x == 0.0f)&&(RayDir_y == 0.0f)&&(RayDir_z == 0.0f) ){ return false; }

	float pDist;
	float min_pDist = FLT_MAX;
	int min_blockface = -1;
	float rmin_x = 0.0f;
	float rmin_y = 0.0f;
	float rmin_z = 0.0f;
	float rmax_x = 0.0f;
	float rmax_y = 0.0f;
	float rmax_z = 0.0f;
	int worldgroupA = 0;
	int worldgroupB = 0;

	//��̃u���b�N�͌v�Z�O
	if( cbdata[blockid].BoardBlock == true ){
		if( face != NULL ){ *face = 0; }
		*Dist = 0.0f;
		return false;
	}

	if( maxDist > 0.0f ){
		//���C��AABB�����
		GetAABBRay(RayPos_x, RayPos_y, RayPos_z, RayDir_x, RayDir_y, RayDir_z, maxDist, &rmin_x, &rmin_y, &rmin_z, &rmax_x, &rmax_y, &rmax_z);
	}

	if( maxDist > 0.0f ){
		//�n�_�ƏI�_�̋�ԃO���[�v���擾
		worldgroupA = GetWorldGroup(RayPos_x, RayPos_z);
		worldgroupB = GetWorldGroup(RayPos_x + RayDir_x * maxDist, RayPos_z + RayDir_z * maxDist);
	}

	if( maxDist > 0.0f ){
		if( cbdata[blockid].worldgroup != 0 ){
			//�n�_�ƏI�_���A��Ԃ̃O���[�v����o�Ă��Ȃ����
			if( (worldgroupA != 0)&&(worldgroupA == worldgroupB) ){

				//��Ԃ̃O���[�v���Ⴆ�Όv�Z�O
				if( cbdata[blockid].worldgroup == worldgroupA ){		//worldgroupA == worldgroupB
					//���E�{�b�N�X���m�Ŕ���
					if( CollideBoxAABB(cbdata[blockid].min_x, cbdata[blockid].min_y, cbdata[blockid].min_z, cbdata[blockid].max_x, cbdata[blockid].max_y, cbdata[blockid].max_z, rmin_x, rmin_y, rmin_z, rmax_x, rmax_y, rmax_z) == false ){
						//�������ĂȂ���΁A���̃u���b�N�͒��ׂȂ��B
						if( face != NULL ){ *face = 0; }
						*Dist = 0.0f;
						return false;
					}
				}
			}
		}
	}

	//���E�{�b�N�X�ƃ��C�Ŕ���
	if( CollideAABBRay(cbdata[blockid].min_x, cbdata[blockid].min_y, cbdata[blockid].min_z, cbdata[blockid].max_x, cbdata[blockid].max_y, cbdata[blockid].max_z, RayPos_x, RayPos_y, RayPos_z, RayDir_x, RayDir_y, RayDir_z, NULL, -1.0f) == false ){
		//�������ĂȂ���΁A���̃u���b�N�͒��ׂȂ��B
		if( face != NULL ){ *face = 0; }
		*Dist = 0.0f;
		return false;
	}

	//�e�|���S���P�ʂŔ���
	for(int i=0; i<6; i++){
		if( CheckPolygonFront(blockid, i, RayPos_x, RayPos_y, RayPos_z) == true ){
			if( CheckIntersectTri(blockid, i, RayPos_x, RayPos_y, RayPos_z, RayDir_x, RayDir_y, RayDir_z, &pDist) == true ){
				if( min_pDist > pDist ){
					min_pDist = pDist;
					min_blockface = i;
				}
			}
		}
	}

	//�������������ő勗���𒴂��Ă���΁A����𖳌��ɁB
	if( maxDist >= 0.0f ){
		if( min_pDist > maxDist ){
			min_blockface = -1;
		}
	}

	//������Ȃ���΁A�|�C���^�ɓK���Ȑ��������ĕԂ��B
	if( min_blockface == -1 ){
		if( face != NULL ){ *face = 0; }
		*Dist = 0.0f;
		return false;
	}

	//�v�Z���ʂ����ĕԂ�
	if( face != NULL ){ *face = min_blockface; }
	*Dist = min_pDist;
	return true;
}

//! @brief �S�Ẵu���b�N�ƃ��C�i�����j�̓����蔻��
//! @param RayPos_x ���C�̈ʒu�i�n�_�j���w�肷�� X���W
//! @param RayPos_y ���C�̈ʒu�i�n�_�j���w�肷�� Y���W
//! @param RayPos_z ���C�̈ʒu�i�n�_�j���w�肷�� Z���W
//! @param RayDir_x ���C�̃x�N�g�����w�肷�� X����
//! @param RayDir_y ���C�̃x�N�g�����w�肷�� Y����
//! @param RayDir_z ���C�̃x�N�g�����w�肷�� Z����
//! @param id ���������u���b�N��ID���󂯎��|�C���^�@�iNULL�j
//! @param face ���������u���b�N�̖ʔԍ��i0�`5�j���󂯎��|�C���^�@�iNULL�j
//! @param Dist ���������u���b�N�Ƃ̋������󂯎��|�C���^
//! @param maxDist ������s���ő勗���@�i0.0 �����Ŗ����E�����j
//! @return �������Ă���Ftrue�@�������Ă��Ȃ��Ffalse
//! @warning RayPos�i�n�_�j�� RayDir�i�x�N�g���j���ԈႦ�Ȃ����ƁB
//! @warning ������s���ő勗�����w�肵�Ȃ��ƁA�p�t�H�[�}���X���啝�ɒቺ���܂��B
//! @warning �g������ CheckALLBlockIntersectDummyRay()�֐� �Ɨގ����Ă��܂����A���֐���荂���x�Œᑬ�ł��B
//! @attention ���C�̎n�_���痠���ɂȂ�u���b�N�̖ʂ͖�������܂��B�������[���ɕό`��������̃u���b�N���������܂��B
//! @attention �܂��A���C�������̃u���b�N�ɓ�����ꍇ�́A<b>���C�̎n�_�����ԋ߂�����</b>��Ԃ��܂��B
bool Collision::CheckALLBlockIntersectRay(float RayPos_x, float RayPos_y, float RayPos_z, float RayDir_x, float RayDir_y, float RayDir_z, int *id, int *face, float *Dist, float maxDist)
{
	if( BlockDataIF == NULL ){ return false; }
	if( (RayDir_x == 0.0f)&&(RayDir_y == 0.0f)&&(RayDir_z == 0.0f) ){ return false; }

	int bs = BlockDataIF->GetTotaldatas();
	float pDist;
	float min_pDist = FLT_MAX;
	int min_blockid = -1;
	int min_blockface = -1;
	float rmin_x = 0.0f;
	float rmin_y = 0.0f;
	float rmin_z = 0.0f;
	float rmax_x = 0.0f;
	float rmax_y = 0.0f;
	float rmax_z = 0.0f;
	int worldgroupA = 0;
	int worldgroupB = 0;

	if( maxDist > 0.0f ){
		//���C��AABB�����
		GetAABBRay(RayPos_x, RayPos_y, RayPos_z, RayDir_x, RayDir_y, RayDir_z, maxDist, &rmin_x, &rmin_y, &rmin_z, &rmax_x, &rmax_y, &rmax_z);
	}

	if( maxDist > 0.0f ){
		//�n�_�ƏI�_�̋�ԃO���[�v���擾
		worldgroupA = GetWorldGroup(RayPos_x, RayPos_z);
		worldgroupB = GetWorldGroup(RayPos_x + RayDir_x * maxDist, RayPos_z + RayDir_z * maxDist);
	}
	
	for(int i=0; i<bs; i++){
		//��̃u���b�N�͌v�Z�O
		if( cbdata[i].BoardBlock == true ){ continue; }

		if( maxDist > 0.0f ){
			if( cbdata[i].worldgroup != 0 ){
				//�n�_�ƏI�_���A��Ԃ̃O���[�v����o�Ă��Ȃ����
				if( (worldgroupA != 0)&&(worldgroupA == worldgroupB) ){

					//��Ԃ̃O���[�v���Ⴆ�Όv�Z�O
					if( cbdata[i].worldgroup == worldgroupA ){		//worldgroupA == worldgroupB

						//���E�{�b�N�X���m�Ŕ���
						if( CollideBoxAABB(cbdata[i].min_x, cbdata[i].min_y, cbdata[i].min_z, cbdata[i].max_x, cbdata[i].max_y, cbdata[i].max_z, rmin_x, rmin_y, rmin_z, rmax_x, rmax_y, rmax_z) == false ){
							continue;	//�������ĂȂ���΁A���̃u���b�N�͒��ׂȂ��B
						}

					}
				}
			}
		}

		//���E�{�b�N�X�ƃ��C�Ŕ���
		if( CollideAABBRay(cbdata[i].min_x, cbdata[i].min_y, cbdata[i].min_z, cbdata[i].max_x, cbdata[i].max_y, cbdata[i].max_z, RayPos_x, RayPos_y, RayPos_z, RayDir_x, RayDir_y, RayDir_z, NULL, -1.0f) == false ){
			continue;		//�������ĂȂ���΁A���̃u���b�N�͒��ׂȂ��B
		}

		//�e�|���S���P�ʂŔ���
		for(int j=0; j<6; j++){
			if( CheckPolygonFront(i, j, RayPos_x, RayPos_y, RayPos_z) == true ){
				if( CheckIntersectTri(i, j, RayPos_x, RayPos_y, RayPos_z, RayDir_x, RayDir_y, RayDir_z, &pDist) == true ){
					if( min_pDist > pDist ){
						min_pDist = pDist;
						min_blockid = i;
						min_blockface = j;
					}
				}
			}
		}
	}

	//�������������ő勗���𒴂��Ă���΁A����𖳌��ɁB
	if( maxDist >= 0.0f ){
		if( min_pDist > maxDist ){
			min_blockid = -1;
		}
	}

	//������Ȃ���΁A�|�C���^�ɓK���Ȑ��������ĕԂ��B
	if( min_blockid == -1 ){
		if( id != NULL ){ *id = 0; }
		if( face != NULL ){ *face = 0; }
		*Dist = 0.0f;
		return false;
	}

	//�v�Z���ʂ����ĕԂ�
	if( id != NULL ){ *id = min_blockid; }
	if( face != NULL ){ *face = min_blockface; }
	*Dist = min_pDist;
	return true;
}

//! @brief �S�Ẵu���b�N�ƏՓ˂��Ă��邩����
//! @param RayPos_x �n�_ X���W
//! @param RayPos_y �n�_ Y���W
//! @param RayPos_z �n�_ Z���W
//! @param RayDir_x �x�N�g�� X����
//! @param RayDir_y �x�N�g�� Y����
//! @param RayDir_z �x�N�g�� Z����
//! @param id ���������u���b�N��ID���󂯎��|�C���^�@�iNULL�j
//! @param face ���������u���b�N�̖ʔԍ��i0�`5�j���󂯎��|�C���^�@�iNULL�j
//! @param Dist �_�~�[�ϐ��̃|�C���^�i��� 0.0f ��Ԃ��j
//! @param maxDist ������s���ő勗���@�i0.0 �����w�� �s�j
//! @return �������Ă���Ftrue�@�������Ă��Ȃ��Ffalse
//! @warning �n�_�� �x�N�g�����ԈႦ�Ȃ����ƁB
//! @warning �g������ CheckALLBlockIntersectRay()�֐� �Ɨގ����Ă��܂����A���֐���荂���Œᐸ�x�ł��B
//! @attention ���C�̎n�_���痠���ɂȂ�u���b�N�̖ʂ͖�������܂��B�������[���ɕό`��������̃u���b�N���������܂��B
//! @attention �܂��A���C�������̃u���b�N�ɓ�����ꍇ�́A<b>��ԍŏ��ɔ�����������</b>��Ԃ��܂��B
bool Collision::CheckALLBlockIntersectDummyRay(float RayPos_x, float RayPos_y, float RayPos_z, float RayDir_x, float RayDir_y, float RayDir_z, int *id, int *face, float *Dist, float maxDist)
{
	if( BlockDataIF == NULL ){ return false; }
	if( maxDist <= 0.0f ){ return false; }

	float RayPos2_x = RayPos_x + RayDir_x * maxDist/2;
	float RayPos2_y = RayPos_y + RayDir_y * maxDist/2;
	float RayPos2_z = RayPos_z + RayDir_z * maxDist/2;
	float RayPos3_x = RayPos_x + RayDir_x * maxDist;
	float RayPos3_y = RayPos_y + RayDir_y * maxDist;
	float RayPos3_z = RayPos_z + RayDir_z * maxDist;
	
	if( (id == NULL)&&(face == NULL) ){
		for(int i=0; i<MAX_BLOCKS; i++){
			//�I�����_
			if( CheckBlockInside(i, RayPos3_x, RayPos3_y, RayPos3_z, true, NULL) == true ){
				*Dist = 0.0f;
				return true;
			}

			//���Ԏ��_
			if( CheckBlockInside(i, RayPos2_x, RayPos2_y, RayPos2_z, true, NULL) == true ){
				*Dist = 0.0f;
				return true;
			}
		}

		*Dist = 0.0f;
		return false;
	}

	for(int i=0; i<MAX_BLOCKS; i++){
		int surface;

		//�J�n�n�_
		CheckBlockInside(i, RayPos_x, RayPos_y, RayPos_z, false, &surface);

		//�I�����_
		if( CheckBlockInside(i, RayPos3_x, RayPos3_y, RayPos3_z, true, &surface) == true ){
			if( id != NULL ){ *id = i; }
			if( face != NULL ){ *face = surface; }
			*Dist = 0.0f;
			return true;
		}

		//���Ԏ��_
		if( CheckBlockInside(i, RayPos2_x, RayPos2_y, RayPos2_z, true, &surface) == true ){
			if( id != NULL ){ *id = i; }
			if( face != NULL ){ *face = surface; }
			*Dist = 0.0f;
			return true;
		}
	}

	if( id != NULL ){ *id = 0; }
	if( face != NULL ){ *face = 0; }
	*Dist = 0.0f;
	return false;
}

//! @brief �u���b�N�ɉ����Ĉړ�����x�N�g�������߂�
void Collision::ScratchVector(int id, int face, float in_vx, float in_vy, float in_vz, float *out_vx, float *out_vy, float *out_vz)
{
	if( BlockDataIF == NULL ){ return; }
	if( (id < 0)||(BlockDataIF->GetTotaldatas() <= id) ){ return; }
	if( (face < 0)||(6 < face) ){ return; }

	blockdata bdata;
	BlockDataIF->Getdata(&bdata, id);

	//����
	float Dot = in_vx * bdata.material[face].vx + in_vy * bdata.material[face].vy + in_vz * bdata.material[face].vz;

	*out_vx = in_vx - Dot * bdata.material[face].vx;
	*out_vy = in_vy - Dot * bdata.material[face].vy;
	*out_vz = in_vz - Dot * bdata.material[face].vz;
}

//! @brief �u���b�N�ɔ��˂���x�N�g�������߂�
void Collision::ReflectVector(int id, int face, float in_vx, float in_vy, float in_vz, float *out_vx, float *out_vy, float *out_vz)
{
	if( BlockDataIF == NULL ){ return; }
	if( (id < 0)||(BlockDataIF->GetTotaldatas() <= id) ){ return; }
	if( (face < 0)||(6 < face) ){ return; }

	blockdata bdata;
	BlockDataIF->Getdata(&bdata, id);

	//����
	float Dot = in_vx * bdata.material[face].vx + in_vy * bdata.material[face].vy + in_vz * bdata.material[face].vz;

	*out_vx = in_vx - 2.0f * Dot * bdata.material[face].vx;
	*out_vy = in_vy - 2.0f * Dot * bdata.material[face].vy;
	*out_vz = in_vz - 2.0f * Dot * bdata.material[face].vz;
}

//! @brief �u���b�N�ɑ΂���x�N�g���̐i���p�x�����߂�
//! @return �����Ftrue�@���s�Ffalse
//! @attention �\�����F0.0�`PI/2�i0.0�x�`90.0�x�j�A�������F-0.0�`-PI/2�i-0.0�x�`-90.0�x�j
bool Collision::AngleVector(int id, int face, float in_vx, float in_vy, float in_vz, float *out_angle)
{
	if( BlockDataIF == NULL ){ return false; }
	if( (id < 0)||(BlockDataIF->GetTotaldatas() <= id) ){ return false; }
	if( (face < 0)||(6 < face) ){ return false; }
	if( out_angle == NULL ){ return false; }

	if( (in_vx == 0.0f)&&(in_vy == 0.0f)&&(in_vz == 0.0f) ){ return false; }

	blockdata bdata;
	BlockDataIF->Getdata(&bdata, id);

	VectorNormalization(&in_vx, &in_vy, &in_vz);

	//����
	float Dot = in_vx * bdata.material[face].vx + in_vy * bdata.material[face].vy + in_vz * bdata.material[face].vz;

	//�p�x�����߂�
	*out_angle = asinf(Dot) * -1;

	return true;
}

//! @brief �u���b�N�ɑ΂���x�N�g���̂Ȃ��p�����߂�
//! @return �����Ftrue�@���s�Ffalse
//! @attention �x�N�g���̂Ȃ��p�F0.0�`PI�i0�x�`180�x�j
bool Collision::AngleVector2(int id, int face, float in_vx, float in_vy, float in_vz, float *out_angle)
{
	if( BlockDataIF == NULL ){ return false; }
	if( (id < 0)||(BlockDataIF->GetTotaldatas() <= id) ){ return false; }
	if( (face < 0)||(6 < face) ){ return false; }
	if( out_angle == NULL ){ return false; }

	if( (in_vx == 0.0f)&&(in_vy == 0.0f)&&(in_vz == 0.0f) ){ return false; }

	blockdata bdata;
	BlockDataIF->Getdata(&bdata, id);

	//����
	float Dot = in_vx * bdata.material[face].vx + in_vy * bdata.material[face].vy + in_vz * bdata.material[face].vz;

	//�������Ȃ珜�O
	if( Dot >= 0.0f ){ return false; }

	//�Ȃ��p�����߂�
	*out_angle = acosf(Dot);

	return true;
}

//! @brief ���C��AABB�����߂�
//! @param RayPos_x �n�_ X���W
//! @param RayPos_y �n�_ Y���W
//! @param RayPos_z �n�_ Z���W
//! @param RayDir_x �x�N�g�� X����
//! @param RayDir_y �x�N�g�� Y����
//! @param RayDir_z �x�N�g�� Z����
//! @param maxDist ������s���ő勗��
//! @param *min_x �ŏ� X���W��Ԃ��|�C���^
//! @param *min_y �ŏ� Y���W��Ԃ��|�C���^
//! @param *min_z �ŏ� Z���W��Ԃ��|�C���^
//! @param *max_x �ő� X���W��Ԃ��|�C���^
//! @param *max_y �ő� Y���W��Ԃ��|�C���^
//! @param *max_z �ő� Z���W��Ԃ��|�C���^
void GetAABBRay(float RayPos_x, float RayPos_y, float RayPos_z, float RayDir_x, float RayDir_y, float RayDir_z, float maxDist, float *min_x, float *min_y, float *min_z, float *max_x, float *max_y, float *max_z)
{
	float rmin_x, rmin_y, rmin_z, rmax_x, rmax_y, rmax_z;

	//���C��AABB�����
	rmin_x = RayPos_x + RayDir_x * maxDist;
	rmin_y = RayPos_y + RayDir_y * maxDist;
	rmin_z = RayPos_z + RayDir_z * maxDist;
	rmax_x = rmin_x;
	rmax_y = rmin_y;
	rmax_z = rmin_z;
	if( rmin_x > RayPos_x ){ rmin_x = RayPos_x; }
	if( rmin_y > RayPos_y ){ rmin_y = RayPos_y; }
	if( rmin_z > RayPos_z ){ rmin_z = RayPos_z; }
	if( rmax_x < RayPos_x ){ rmax_x = RayPos_x; }
	if( rmax_y < RayPos_y ){ rmax_y = RayPos_y; }
	if( rmax_z < RayPos_z ){ rmax_z = RayPos_z; }

	//�v�Z�덷�΍�̂��߁A�킸���ɑ傫�߂ɂ���B
	rmin_x -= COLLISION_ADDSIZE;
	rmin_y -= COLLISION_ADDSIZE;
	rmin_z -= COLLISION_ADDSIZE;
	rmax_x += COLLISION_ADDSIZE;
	rmax_y += COLLISION_ADDSIZE;
	rmax_z += COLLISION_ADDSIZE;

	*min_x = rmin_x;
	*min_y = rmin_y;
	*min_z = rmin_z;
	*max_x = rmax_x;
	*max_y = rmax_y;
	*max_z = rmax_z;
}

//! @brief AABB�ɂ�铖���蔻��
//! @param box1_min_x ����A�̍ŏ� X���W
//! @param box1_min_y ����A�̍ŏ� Y���W
//! @param box1_min_z ����A�̍ŏ� Z���W
//! @param box1_max_x ����A�̍ő� X���W
//! @param box1_max_y ����A�̍ő� Y���W
//! @param box1_max_z ����A�̍ő� Z���W
//! @param box2_min_x ����B�̍ŏ� X���W
//! @param box2_min_y ����B�̍ŏ� Y���W
//! @param box2_min_z ����B�̍ŏ� Z���W
//! @param box2_max_x ����B�̍ő� X���W
//! @param box2_max_y ����B�̍ő� Y���W
//! @param box2_max_z ����B�̍ő� Z���W
//! @return �������Ă���Ftrue�@�������Ă��Ȃ��Ffalse
//! @attention �G���[������ꍇ�u�������Ă��Ȃ��Ffalse�v���Ԃ���܂��B
bool CollideBoxAABB(float box1_min_x, float box1_min_y, float box1_min_z, float box1_max_x, float box1_max_y, float box1_max_z, float box2_min_x, float box2_min_y, float box2_min_z, float box2_max_x, float box2_max_y, float box2_max_z)
{
	//�G���[�΍�
	if( box1_min_x > box1_max_x ){ return false; }
	if( box1_min_y > box1_max_y ){ return false; }
	if( box1_min_z > box1_max_z ){ return false; }
	if( box2_min_x > box2_max_x ){ return false; }
	if( box2_min_y > box2_max_y ){ return false; }
	if( box2_min_z > box2_max_z ){ return false; }

	if(
		(box1_min_x < box2_max_x)&&(box1_max_x > box2_min_x)&&
		(box1_min_y < box2_max_y)&&(box1_max_y > box2_min_y)&&
		(box1_min_z < box2_max_z)&&(box1_max_z > box2_min_z)
	){
		return true;
	}

	return false;
}

//! @brief �~�����m�̓����蔻��
//! @param c1_x �~��1 ��ӂ�x���W
//! @param c1_y �~��1 ��ӂ�y���W
//! @param c1_z �~��1 ��ӂ�z���W
//! @param c1_r �~��1 �̔��a
//! @param c1_h �~��1 �̍���
//! @param c2_x �~��2 ��ӂ�x���W
//! @param c2_y �~��2 ��ӂ�y���W
//! @param c2_z �~��2 ��ӂ�z���W
//! @param c2_r �~��2 �̔��a
//! @param c2_h �~��2 �̍���
//! @param *angle �~��1����݂��ڐG�p�x���󂯎��|�C���^�iNULL�j
//! @param *length �~��1����݂��ڐG�������󂯎��|�C���^�iNULL�j
//! @return �������Ă���Ftrue�@�������Ă��Ȃ��Ffalse
bool CollideCylinder(float c1_x, float c1_y, float c1_z, float c1_r, float c1_h, float c2_x, float c2_y, float c2_z, float c2_r, float c2_h, float *angle, float *length)
{
	//��ɂ���y�ȍ����Ŕ���
	if( (c1_y < c2_y + c2_h)&&(c1_y + c1_h > c2_y) ){

		//�����Ŕ���
		float x = c1_x - c2_x;
		float z = c1_z - c2_z;
		float caser = x*x + z*z;
		float minr = (c1_r+c2_r) * (c1_r+c2_r);
		if( caser < minr ){
			if( angle != NULL ){ *angle = atan2f(z, x); }
			if( length != NULL ){ *length = sqrtf(minr) - sqrtf(caser); }
			return true;
		}
	}

	return false;
}

//! @brief ���̂Ɠ_�̓����蔻��
//! @param s_x ���̂�x���W
//! @param s_y ���̂�y���W
//! @param s_z ���̂�z���W
//! @param s_r ���̂̔��a
//! @param p_x X���W
//! @param p_y Y���W
//! @param p_z Z���W
//! @return �������Ă���Ftrue�@�������Ă��Ȃ��Ffalse
bool CollideSphereInside(float s_x, float s_y, float s_z, float s_r, float p_x, float p_y, float p_z)
{
	float x, y, z, d;

	//�������Z�o
	x = s_x - p_x;
	y = s_y - p_y;
	z = s_z - p_z;
	d = x*x + y*y + z*z;

	//���������a���߂�
	if( d < s_r*s_r ){
		return true;
	}

	return false;
}

//! @brief ���̂ƃ��C�i�����j�̓����蔻��
//! @param s_x ���̂�x���W
//! @param s_y ���̂�y���W
//! @param s_z ���̂�z���W
//! @param s_r ���̂̔��a
//! @param RayPos_x ���C�̈ʒu�i�n�_�j���w�肷�� X���W
//! @param RayPos_y ���C�̈ʒu�i�n�_�j���w�肷�� Y���W
//! @param RayPos_z ���C�̈ʒu�i�n�_�j���w�肷�� Z���W
//! @param RayDir_x ���C�̃x�N�g�����w�肷�� X����
//! @param RayDir_y ���C�̃x�N�g�����w�肷�� Y����
//! @param RayDir_z ���C�̃x�N�g�����w�肷�� Z����
//! @param Dist �����������̂Ƃ̋������󂯎��|�C���^
//! @param maxDist ������s���ő勗���@�i0.0 �����Ŗ����E�����j
//! @return �������Ă���Ftrue�@�������Ă��Ȃ��Ffalse
//! @warning RayPos�i�n�_�j�� RayDir�i�x�N�g���j���ԈႦ�Ȃ����ƁB
//! @warning ������s���ő勗�����w�肵�Ȃ��ƁA�p�t�H�[�}���X���啝�ɒቺ���܂��B
//! @todo ���C�̎n�_�����̂̊O���ł����̂Ƌt�����̏ꍇ�A����������ł��Ȃ��H
bool CollideSphereRay(float s_x, float s_y, float s_z, float s_r, float RayPos_x, float RayPos_y, float RayPos_z, float RayDir_x, float RayDir_y, float RayDir_z, float *Dist, float maxDist)
{
	if( maxDist > 0.0f ){
		float pmin_x, pmin_y, pmin_z, pmax_x, pmax_y, pmax_z;

		//���C��AABB�����
		GetAABBRay(RayPos_x, RayPos_y, RayPos_z, RayDir_x, RayDir_y, RayDir_z, maxDist, &pmin_x, &pmin_y, &pmin_z, &pmax_x, &pmax_y, &pmax_z);

		//���E�{�b�N�X���m�Ŕ���
		if( CollideBoxAABB(s_x - s_r, s_y - s_r, s_z - s_r, s_x + s_r, s_y + s_r, s_z + s_r, pmin_x, pmin_y, pmin_z, pmax_x, pmax_y, pmax_z) == false ){
			return false;
		}
	}

	float x, y, z, d;
	float MinDist, RayDist, RDist;

	//�_�ƃ��C�n�_�̋���
	x = s_x - RayPos_x;
	y = s_y - RayPos_y;
	z = s_z - RayPos_z;
	d = sqrtf(x*x + y*y + z*z);

	//���C�n�_�����a���߂��i���߂荞��ł���j
	if( d < s_r ){
		*Dist = 0.0f;
		return true;
	}

	//�_�i���̂̒��S�j�ƃ��C�̍ŒZ���������߂�
	MinDist = DistancePosRay(s_x, s_y, s_z, RayPos_x, RayPos_y, RayPos_z, RayDir_x, RayDir_y, RayDir_z);

	if( MinDist <= s_r ){
		RayDist = sqrtf(d*d - MinDist*MinDist);		//�i���C�n�_����j�_�ɍł��߂Â�����
		RDist = sqrtf(s_r*s_r - MinDist*MinDist);	//�i�_���a����j�_�ɍł��߂Â�����

		*Dist = RayDist - RDist;	//���C���_�ŒZ - ���a�ŒZ = ���C���_���甼�a�܂ł̍ŒZ

		//������s���ő勗������������΁A�������ĂȂ����Ƃ�
		if( (maxDist > 0.0f)&&(maxDist < *Dist) ){
			return false;
		}
		return true;
	}

	return false;
}

//! @brief AABB�ƃ��C�i�����j�̓����蔻��
//! @param box_min_x ���̂̍ŏ� X���W
//! @param box_min_y ���̂̍ŏ� Y���W
//! @param box_min_z ���̂̍ŏ� Z���W
//! @param box_max_x ���̂̍ő� X���W
//! @param box_max_y ���̂̍ő� Y���W
//! @param box_max_z ���̂̍ő� Z���W
//! @param RayPos_x ���C�̈ʒu�i�n�_�j���w�肷�� X���W
//! @param RayPos_y ���C�̈ʒu�i�n�_�j���w�肷�� Y���W
//! @param RayPos_z ���C�̈ʒu�i�n�_�j���w�肷�� Z���W
//! @param RayDir_x ���C�̃x�N�g�����w�肷�� X����
//! @param RayDir_y ���C�̃x�N�g�����w�肷�� Y����
//! @param RayDir_z ���C�̃x�N�g�����w�肷�� Z����
//! @param Dist ��������AABB�Ƃ̋������󂯎��|�C���^
//! @param maxDist ������s���ő勗���@�i0.0 �����Ŗ����E�����j
//! @return �������Ă���Ftrue�@�������Ă��Ȃ��Ffalse
//! @warning RayPos�i�n�_�j�� RayDir�i�x�N�g���j���ԈႦ�Ȃ����ƁB
//! @warning ������s���ő勗�����w�肵�Ȃ��ƁA�p�t�H�[�}���X���啝�ɒቺ���܂��B
bool CollideAABBRay(float box_min_x, float box_min_y, float box_min_z, float box_max_x, float box_max_y, float box_max_z, float RayPos_x, float RayPos_y, float RayPos_z, float RayDir_x, float RayDir_y, float RayDir_z, float *Dist, float maxDist)
{
	if( box_min_x > box_max_x ){ return false; }
	if( box_min_y > box_max_y ){ return false; }
	if( box_min_z > box_max_z ){ return false; }

	//���C�̎n�_��AABB�̓����ɓ����Ă���΁A���ɓ������Ă���
	if( (box_min_x <= RayPos_x)&&(RayPos_x <= box_max_x)&&(box_min_y <= RayPos_y)&&(RayPos_y <= box_max_y)&&(box_min_z <= RayPos_z)&&(RayPos_z <= box_max_z) ){
		if( Dist != NULL ){ *Dist = 0.0f; }
		return true;
	}

	if( maxDist > 0.0f ){
		float pmin_x, pmin_y, pmin_z, pmax_x, pmax_y, pmax_z;

		//���C��AABB�����
		GetAABBRay(RayPos_x, RayPos_y, RayPos_z, RayDir_x, RayDir_y, RayDir_z, maxDist, &pmin_x, &pmin_y, &pmin_z, &pmax_x, &pmax_y, &pmax_z);

		//���E�{�b�N�X���m�Ŕ���
		if( CollideBoxAABB(box_min_x, box_min_y, box_min_z, box_max_x, box_max_y, box_max_z, pmin_x, pmin_y, pmin_z, pmax_x, pmax_y, pmax_z) == false ){
			return false;
		}
	}

	//�x�N�g���𐳋K��
	VectorNormalization(&RayDir_x, &RayDir_y, &RayDir_z);

	//�e���W��z��Ɋi�[
	float box_min[] = {box_min_x, box_min_y, box_min_z};
	float box_max[] = {box_max_x, box_max_y, box_max_z};
	float RayPos[] = {RayPos_x, RayPos_y, RayPos_z};
	float RayDir[] = {RayDir_x, RayDir_y, RayDir_z};
	float Ray_min[3];
	float Ray_max[3];
	float Ray_tmin, Ray_tmax;

	//X�EY�EZ��3�����̏���
	for(int axis=0; axis<3; axis++){
		if( ( (RayPos[axis] < box_min[axis])||(box_max[axis] < RayPos[axis]) )&&(RayDir[axis] == 0.0f) ){
			//�����ɓ����Ă��Ȃ��̂� �x�N�g���̕����� 0 �Ȃ�A����AABB�̊O
			return false;
		}
		else{
			//�����ɓ����Ă��Ȃ����A�x�N�g�������������Ȃ�`
			float t1, t2;

			//AABB���x�N�g�����˂��������_�܂ł̋������擾
			t1 = (box_min[axis] - RayPos[axis]) / RayDir[axis];
			t2 = (box_max[axis] - RayPos[axis]) / RayDir[axis];

			//��_�܂ł̋������ŏ��E�ő�ŋt�Ȃ�A����ւ���B
			if( t1 > t2 ){
				float temp = t1; t1 = t2; t2 = temp;
			}

			//���̏��Ƃ��ċL�^
			Ray_min[axis] = t1;
			Ray_max[axis] = t2;

			if( (box_min[axis] <= RayPos[axis])&&(RayPos[axis] <= box_max[axis]) ){
				//�����ɓ����Ă���΁A���̂܂܎��̏��Ƃ��ċL�^
				Ray_min[axis] = 0.0f;
			}

			//�������}�C�i�X�i���x�N�g���t�����j�Ȃ�AABB�Ƃ� ������Ȃ�
			if( Ray_min[axis] < 0.0f ){ return false; }
			if( Ray_max[axis] < 0.0f ){ return false; }
		}
	}

	//�e���ŁA�ł������e�ŏ������f�ƍł��߂��e�ő勗���f���Z�o
	Ray_tmin = Ray_min[0];
	Ray_tmax = Ray_max[0];
	if( Ray_tmin < Ray_min[1] ){ Ray_tmin = Ray_min[1]; }
	if( Ray_tmax > Ray_max[1] ){ Ray_tmax = Ray_max[1]; }
	if( Ray_tmin < Ray_min[2] ){ Ray_tmin = Ray_min[2]; }
	if( Ray_tmax > Ray_max[2] ){ Ray_tmax = Ray_max[2]; }

	//�ŏ������ƍő勗���̊֌W����������΁`
	if( (Ray_tmax - Ray_tmin) > 0 ){
		//������s���ő勗����艓����΁A���薳���B
		if( maxDist > 0.0f ){
			if( Ray_tmin > maxDist ){ return false; }
		}

		//�����������Ԃ�
		if( Dist != NULL ){ *Dist = Ray_tmin; }
		return true;
	}

	return false;
}

//! @brief �~���Ɠ_�̓����蔻��
//! @param c_x �~�� ��ӂ�x���W
//! @param c_y �~�� ��ӂ�y���W
//! @param c_z �~�� ��ӂ�z���W
//! @param c_r �~�� �̔��a
//! @param c_h �~�� �̍���
//! @param p_x X���W
//! @param p_y Y���W
//! @param p_z Z���W
//! @return �������Ă���Ftrue�@�������Ă��Ȃ��Ffalse
bool CollideCylinderInside(float c_x, float c_y, float c_z, float c_r, float c_h, float p_x, float p_y, float p_z)
{
	float x, z, d;


	// X-Z���ʂŉ~�Ƃ��ď�������

	//�������Z�o
	x = c_x - p_x;
	z = c_z - p_z;
	d = x*x + z*z;

	//���������a��藣��Ă��鎞�_�œ�����Ȃ�
	if( d > c_r*c_r ){
		return false;
	}


	//Y���̂�AABB�Ɠ��l�̏���

	if( (c_y <= p_y)&&(p_y <= (c_y+c_h)) ){
		//Y���ł������ɓ����Ă���΁A�������Ă���
		return true;
	}

	return false;
}

//! @brief �~���ƃ��C�i�����j�̓����蔻��
//! @param c_x �~�� ��ӂ�x���W
//! @param c_y �~�� ��ӂ�y���W
//! @param c_z �~�� ��ӂ�z���W
//! @param c_r �~�� �̔��a
//! @param c_h �~�� �̍���
//! @param RayPos_x ���C�̈ʒu�i�n�_�j���w�肷�� X���W
//! @param RayPos_y ���C�̈ʒu�i�n�_�j���w�肷�� Y���W
//! @param RayPos_z ���C�̈ʒu�i�n�_�j���w�肷�� Z���W
//! @param RayDir_x ���C�̃x�N�g�����w�肷�� X����
//! @param RayDir_y ���C�̃x�N�g�����w�肷�� Y����
//! @param RayDir_z ���C�̃x�N�g�����w�肷�� Z����
//! @param Dist ���������~���Ƃ̋������󂯎��|�C���^
//! @param maxDist ������s���ő勗���@�i0.0f�𒴂���l�j
//! @return �������Ă���Ftrue�@�������Ă��Ȃ��Ffalse
//! @warning RayPos�i�n�_�j�� RayDir�i�x�N�g���j���ԈႦ�Ȃ����ƁB
//! @attention Y�������֐����ɗ��~���ł��B�����͕ς����܂���B
bool CollideCylinderRay(float c_x, float c_y, float c_z, float c_r, float c_h, float RayPos_x, float RayPos_y, float RayPos_z, float RayDir_x, float RayDir_y, float RayDir_z, float *Dist, float maxDist)
{
	float x, z, d;
	float cMinDist, cRayDist, cRDist;
	float RayXZ_min, RayXZ_max;
	float Ray_min[2];
	float Ray_max[2];
	float Ray_tmin, Ray_tmax;


	// X-Z���ʂŉ~�Ƃ��ď�������

	//�_�ƃ��C�n�_�̋���
	x = c_x - RayPos_x;
	z = c_z - RayPos_z;
	d = sqrtf(x*x + z*z);

	//�_�i�~���̒��S�j�ƃ��C�̍ŒZ���������߂�
	cMinDist = DistancePosRay(c_x, 0.0f, c_z, RayPos_x, 0.0f, RayPos_z, RayDir_x, 0.0f, RayDir_z);

	//�ŒZ���������a��藣��Ă��鎞�_�œ�����Ȃ�
	if( cMinDist > c_r ){
		return false;
	}

	cRayDist = sqrtf(d*d - cMinDist*cMinDist);		//�i���C�n�_����j�_�ɍł��߂Â�����
	cRDist = sqrtf(c_r*c_r - cMinDist*cMinDist);	//�i�_���a����j�_�ɍł��߂Â�����

	if( d < c_r ){
		RayXZ_min = 0;		//�n�_���~�̒��Ȃ狗���[��
	}
	else{
		RayXZ_min = cRayDist - cRDist;	//���C���_�ŒZ - ���a�ŒZ = ���C���_���甼�a�܂ł̍ŒZ
	}

	//�_�ƃ��C�I�_�̋���
	x = c_x - RayPos_x + RayDir_x*maxDist;
	z = c_z - RayPos_z + RayDir_x*maxDist;
	d = sqrtf(x*x + z*z);

	if( d < c_r ){
		RayXZ_max = maxDist;		//�I�_���~�̒��Ȃ�ő勗��
	}
	else{
		RayXZ_max = cRayDist + cRDist;
	}

	//Y���Ɣ�ׂ�O�ɁAY���Ɣ�r�ł���悤�ϊ��@�i��ӂ̒l���Εӂ֕ϊ��j
	float Ray_ry = atan2f(RayDir_y, sqrtf(RayDir_x*RayDir_x + RayDir_z*RayDir_z));
	Ray_min[0] = RayXZ_min / cosf(Ray_ry);
	Ray_max[0] = RayXZ_max / cosf(Ray_ry);


	//Y���̂�AABB�Ɠ��l�̏���

	if( ( (RayPos_y < c_y)||((c_y+c_h) < RayPos_y) )&&(RayDir_y == 0.0f) ){
		//�����ɓ����Ă��Ȃ��̂� �x�N�g���̕����� 0 �Ȃ�A���ɉ~���̊O
		return false;
	}

	//�����ɓ����Ă��Ȃ����A�x�N�g�������������Ȃ�`
	float t1, t2;

	//�~�����x�N�g�����˂��������_�܂ł̋������擾
	t1 = (c_y - RayPos_y) / RayDir_y;
	t2 = ((c_y+c_h) - RayPos_y) / RayDir_y;

	//��_�܂ł̋������ŏ��E�ő�ŋt�Ȃ�A����ւ���B
	if( t1 > t2 ){
		float temp = t1; t1 = t2; t2 = temp;
	}

	//���̏��Ƃ��ċL�^
	Ray_min[1] = t1;
	Ray_max[1] = t2;

	if( (c_y <= RayPos_y)&&(RayPos_y <= (c_y+c_h)) ){
		//�����ɓ����Ă���΁A���̂܂܎��̏��Ƃ��ċL�^
		Ray_min[1] = 0.0f;
	}

	//�������}�C�i�X�i���x�N�g���t�����j�Ȃ�~���Ƃ� ������Ȃ�
	if( Ray_min[1] < 0.0f ){ return false; }
	if( Ray_max[1] < 0.0f ){ return false; }


	//���v�Z�����킹��

	//�e���ŁA�ł������e�ŏ������f�ƍł��߂��e�ő勗���f���Z�o
	Ray_tmin = Ray_min[0];
	Ray_tmax = Ray_max[0];
	if( Ray_tmin < Ray_min[1] ){ Ray_tmin = Ray_min[1]; }
	if( Ray_tmax > Ray_max[1] ){ Ray_tmax = Ray_max[1]; }

	//�ŏ������ƍő勗���̊֌W����������΁`
	if( (Ray_tmax - Ray_tmin) > 0 ){
		//������s���ő勗����艓����΁A���薳���B
		if( maxDist > 0.0f ){
			if( Ray_tmin > maxDist ){ return false; }
		}

		//�����������Ԃ�
		if( Dist != NULL ){ *Dist = Ray_tmin; }
		return true;
	}

	return false;
}

//! @brief �_�ƃ��C�̍ŒZ���������߂�
//! @param Pos_x �_��X���W
//! @param Pos_y �_��Y���W
//! @param Pos_z �_��Z���W
//! @param RayPos_x ���C�̈ʒu�i�n�_�j���w�肷�� X���W
//! @param RayPos_y ���C�̈ʒu�i�n�_�j���w�肷�� Y���W
//! @param RayPos_z ���C�̈ʒu�i�n�_�j���w�肷�� Z���W
//! @param RayDir_x ���C�̃x�N�g�����w�肷�� X����
//! @param RayDir_y ���C�̃x�N�g�����w�肷�� Y����
//! @param RayDir_z ���C�̃x�N�g�����w�肷�� Z����
//! @return �ŒZ����
//! @warning RayPos�i�n�_�j�� RayDir�i�x�N�g���j���ԈႦ�Ȃ����ƁB
//! @attention ���C�̕����͍l������܂����A���C�̒����͍l������܂���B
float DistancePosRay(float Pos_x, float Pos_y, float Pos_z, float RayPos_x, float RayPos_y, float RayPos_z, float RayDir_x, float RayDir_y, float RayDir_z)
{
	float x1, y1, z1;
	float x2, y2, z2;
	float x3, y3, z3;
	float Dot;

	x1 = Pos_x - RayPos_x;
	y1 = Pos_y - RayPos_y;
	z1 = Pos_z - RayPos_z;
	x2 = RayDir_x;
	y2 = RayDir_y;
	z2 = RayDir_z;

	//����
	Dot = x1 * x2 + y1 * y2 + z1 * z2;

	//���C�̃x�N�g�����t�����Ȃ�
	if( Dot < 0.0f ){
		return sqrtf(x1*x1 + y1*y1 + z1*z1);
	}

	//�O��
	x3 = y1 * z2 - z1 * y2;
	y3 = z1 * x2 - x1 * z2;
	z3 = x1 * y2 - y1 * x2;

	return sqrtf(x3*x3 + y3*y3 + z3*z3) / sqrtf(RayDir_x*RayDir_x + RayDir_y*RayDir_y + RayDir_z*RayDir_z);
}

//! @brief �����Ɛ����̓����蔻��i2D�j
//! @param A1x ����A�̎n�_ X���W
//! @param A1y ����A�̎n�_ Y���W
//! @param A2x ����A�̏I�_ X���W
//! @param A2y ����A�̏I�_ Y���W
//! @param B1x ����B�̎n�_ X���W
//! @param B1y ����B�̎n�_ Y���W
//! @param B2x ����B�̏I�_ X���W
//! @param B2y ����B�̏I�_ Y���W
//! @param out_x ��_�� X���W ���󂯎��|�C���^�iNULL�j
//! @param out_y ��_�� Y���W ���󂯎��|�C���^�iNULL�j
//! @return ��������Ftrue�@�������Ȃ��Ffalse
bool Collide2DLine(int A1x, int A1y, int A2x, int A2y, int B1x, int B1y, int B2x, int B2y, int *out_x, int *out_y)
{
	//�����̃x�N�g�������߂�
	int Avx = A2x - A1x;
	int Avy = A2y - A1y;
	int Bvx = B2x - B1x;
	int Bvy = B2y - B1y;

	float v1_v2 = (float)(Avx * Bvy - Avy * Bvx);	//�O��
	if( v1_v2 == 0.0f ){
		return false;	//���s
	}

	float vx = (float)(B1x - A1x);
	float vy = (float)(B1y - A1y);
	float v_v1 = vx * Avy - vy * Avx;	//�O��
	float v_v2 = vx * Bvy - vy * Bvx;	//�O��
	float t1 = v_v2 / v1_v2;
	float t2 = v_v1 / v1_v2;

	if( (t1 <= 0)||(1 <= t1)||(t2 <= 0)||(1 <= t2) ){
		return false;	//�������ĂȂ�
	}

	if( out_x != NULL ){ *out_x = (int)(A1x + Avx * t1); }
	if( out_y != NULL ){ *out_y = (int)(A1y + Avy * t1); }

	return true;
}

//! @brief �l�p�`�Ɏ��܂���������߂�i2D�j
//! @param line_x1 �����̎n�_ X���W
//! @param line_y1 �����̎n�_ Y���W
//! @param line_x2 �����̏I�_ X���W
//! @param line_y2 �����̏I�_ Y���W
//! @param box_x1 �l�p�`�̍��� X���W
//! @param box_y1 �l�p�`�̍��� Y���W
//! @param box_x2 �l�p�`�̉E�� X���W
//! @param box_y2 �l�p�`�̉E�� Y���W
//! @param out_line_x1 �l�p�`�Ɏ��܂� �����̎n�_ X���W ���󂯎��|�C���^
//! @param out_line_y1 �l�p�`�Ɏ��܂� �����̎n�_ Y���W ���󂯎��|�C���^
//! @param out_line_x2 �l�p�`�Ɏ��܂� �����̏I�_ X���W ���󂯎��|�C���^
//! @param out_line_y2 �l�p�`�Ɏ��܂� �����̏I�_ Y���W ���󂯎��|�C���^
//! @return �L���i�`�悷��j�Ftrue�@�����i�`�悵�Ȃ��j�Ffalse
//! @warning �����͕K���ubox_x1 < box_x2�v���ubox_x1 < box_x2�v�ɂ��邱��
//! @note �ȈՃ��[�_�[�̃}�b�v�`��p
bool Get2DLineInBox(int line_x1, int line_y1, int line_x2, int line_y2, int box_x1, int box_y1, int box_x2, int box_y2, int *out_line_x1, int *out_line_y1, int *out_line_x2, int *out_line_y2)
{
	//�l�p�`�w�肪�ُ�
	if( (box_x1 >= box_x2)||(box_y1 >= box_y2) ){ return false; }

	//�㉺���E�̋�Ԃɂ���Ȃ�A�I�O��
	if( (line_x1 < box_x1)&&(line_x2 < box_x1) ){ return false; }
	if( (line_y1 < box_y1)&&(line_y2 < box_y1) ){ return false; }
	if( (box_x2 < line_x1)&&(box_x2 < line_x2) ){ return false; }
	if( (box_y2 < line_y1)&&(box_y2 < line_y2) ){ return false; }

	//���Ɏl�p�`�Ɏ��܂�
	if( (box_x1 <= line_x1)&&(line_x1 <= box_x2)&&(box_y1 <= line_y1)&&(line_y1 <= box_y2) ){
		if( (box_x1 <= line_x2)&&(line_x2 <= box_x2)&&(box_y1 <= line_y2)&&(line_y2 <= box_y2) ){
			*out_line_x1 = line_x1;
			*out_line_y1 = line_y1;
			*out_line_x2 = line_x2;
			*out_line_y2 = line_y2;
			return true;
		}
	}

	int x, y;

	//���
	if( Collide2DLine(box_x1, box_y1, box_x2, box_y1, line_x1, line_y1, line_x2, line_y2, &x, &y) == true ){
		//�n�_���l�p�`�̓����Ȃ�I�_���A�Ⴆ�΁i���I�_�������j�n�_������������B
		if( (box_x1 <= line_x1)&&(line_x1 <= box_x2)&&(box_y1 <= line_y1)&&(line_y1 <= box_y2) ){
			line_x2 = x;
			line_y2 = y;
		}
		else{
			line_x1 = x;
			line_y1 = y;
		}
	}
	//�E��
	if( Collide2DLine(box_x2, box_y1, box_x2, box_y2, line_x1, line_y1, line_x2, line_y2, &x, &y) == true ){
		//�n�_���l�p�`�̓����Ȃ�I�_���A�Ⴆ�΁i���I�_�������j�n�_������������B
		if( (box_x1 <= line_x1)&&(line_x1 <= box_x2)&&(box_y1 <= line_y1)&&(line_y1 <= box_y2) ){
			line_x2 = x;
			line_y2 = y;
		}
		else{
			line_x1 = x;
			line_y1 = y;
		}
	}
	//����
	if( Collide2DLine(box_x2, box_y2, box_x1, box_y2, line_x1, line_y1, line_x2, line_y2, &x, &y) == true ){
		//�n�_���l�p�`�̓����Ȃ�I�_���A�Ⴆ�΁i���I�_�������j�n�_������������B
		if( (box_x1 <= line_x1)&&(line_x1 <= box_x2)&&(box_y1 <= line_y1)&&(line_y1 <= box_y2) ){
			line_x2 = x;
			line_y2 = y;
		}
		else{
			line_x1 = x;
			line_y1 = y;
		}
	}
	//����
	if( Collide2DLine(box_x1, box_y2, box_x1, box_y1, line_x1, line_y1, line_x2, line_y2, &x, &y) == true ){
		//�n�_���l�p�`�̓����Ȃ�I�_���A�Ⴆ�΁i���I�_�������j�n�_������������B
		if( (box_x1 <= line_x1)&&(line_x1 <= box_x2)&&(box_y1 <= line_y1)&&(line_y1 <= box_y2) ){
			line_x2 = x;
			line_y2 = y;
		}
		else{
			line_x1 = x;
			line_y1 = y;
		}
	}

	//���߂Ďl�p�`�Ɏ��܂邩�m�F
	if( (box_x1 <= line_x1)&&(line_x1 <= box_x2)&&(box_y1 <= line_y1)&&(line_y1 <= box_y2) ){
		if( (box_x1 <= line_x2)&&(line_x2 <= box_x2)&&(box_y1 <= line_y2)&&(line_y2 <= box_y2) ){
			*out_line_x1 = line_x1;
			*out_line_y1 = line_y1;
			*out_line_x2 = line_x2;
			*out_line_y2 = line_y2;
			return true;
		}
	}

	return false;
}

//! @brief �ϑ��_����Ώۓ_�ւ� ��������E�p�x�Z�o
//! @param pos_x �ϑ��_��X���W
//! @param pos_y �ϑ��_��Y���W
//! @param pos_z �ϑ��_��Z���W
//! @param rx �ϑ��_�̐����p�x
//! @param ry �ϑ��_�̐����p�x
//! @param target_x �Ώۓ_��X���W
//! @param target_y �Ώۓ_��Y���W
//! @param target_z �Ώۓ_��Z���W
//! @param checkdist ���苗���i0.0f�ȉ��Ŕ��薳���j
//! @param out_rx �Ώۓ_�ւ̐����p�x�i�΁`-�΁j���󂯎��|�C���^�iNULL�j
//! @param out_ry �Ώۓ_�ւ̐����p�x���󂯎��|�C���^�iNULL�j
//! @param out_dist2 �Ώۓ_�ւ̋���<b>�̓��</b>���󂯎��|�C���^�iNULL�j
//! @return �����Ftrue ���s�Ffalse
//! @warning out_dist2�͋�����<b>���</b>�ł��B�K�v�ɉ����ĉ��߂� sqrtf()�֐� �Ȃǂ�p���Ă��������B
//! @attention ���� checkdist �ɗL���ȋ�����^�����ꍇ�́A�ϑ��_����Ώۓ_�ւ̋���������s���܂��B�w�肳�ꂽ������藣��Ă���ꍇ�A�p�x���v�Z������ false ��Ԃ��܂��B
//! @attention �t�ɁA���� checkdist ��0.0f�ȉ���^�����ꍇ�A�����ɂ�锻����s���܂���B�֐��͏�� true ��Ԃ��܂��B
bool CheckTargetAngle(float pos_x, float pos_y, float pos_z, float rx, float ry, float target_x, float target_y, float target_z, float checkdist, float *out_rx, float *out_ry, float *out_dist2)
{
	float x, y, z;
	float dist2 = 0.0f;
	float angleX, angleY;

	x = target_x - pos_x;
	y = target_y - pos_y;
	z = target_z - pos_z;

	if( (checkdist > 0.0f)||(out_dist2 != NULL) ){
		dist2 = (x*x + y*y + z*z);
	}

	if( checkdist > 0.0f ){
		if( dist2 > checkdist * checkdist ){
			return false;
		}
	}

	if( out_rx != NULL ){
		angleX = atan2f(z, x) - rx;
		angleX = AngleNormalization(angleX);

		*out_rx = angleX;
	}
	if( out_ry != NULL ){
		angleY = atan2f(y, sqrtf(x*x + z*z))  - ry;

		*out_ry = angleY;
	}
	if( out_dist2 != NULL ){ *out_dist2 = dist2; }

	return true;
}

//! @brief �x�N�g�����K��
//! @param vx vx�̃|�C���^
//! @param vy vy�̃|�C���^
//! @param vz vz�̃|�C���^
//! @return ���K���O�̃x�N�g���̒���
float VectorNormalization(float *vx, float *vy, float *vz)
{
	float r = sqrtf((*vx) * (*vx) + (*vy) * (*vy) + (*vz) * (*vz));
	if( r > 0.0f ){
		*vx /= r;
		*vy /= r;
		*vz /= r;
	}
	return r;
}