//! @file datafile.cpp
//! @brief �f�[�^�Ǘ��N���X�̒�`

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

#include "datafile.h"

//! @brief �R���X�g���N�^
DataInterface::DataInterface()
{
	datas = 0;
}

//! @brief �f�B�X�g���N�^
DataInterface::~DataInterface()
{}

//! @brief �f�[�^�t�@�C����ǂ݂���
//! @param fname �t�@�C����
//! @return �����F0�@���s�F1
int DataInterface::LoadFiledata(const char *fname)
{
	//���g�p�����΍�
	UNREFERENCED_PARAMETER(fname);

	if( fname == NULL ){ return 1; }

	return 0;
}

//! @brief ���v�f�[�^���̎擾
//! @return ���v�f�[�^��
int DataInterface::GetTotaldatas()
{
	return datas;
}

//! @brief �f�[�^���擾
//! @param out_data �󂯎��|�C���^
//! @param id �f�[�^�ԍ�
//! @return �����F0�@���s�F0�ȊO
int DataInterface::Getdata(void *out_data, int id)
{
	//���g�p�����΍�
	UNREFERENCED_PARAMETER(out_data);
	UNREFERENCED_PARAMETER(id);

	return 0;
}

//! @brief �R���X�g���N�^
BlockDataInterface::BlockDataInterface()
{
	//blockdata�\���̏�����
	data = new blockdata[MAX_BLOCKS];

	//�e�N�X�`���ݒ菉����
	for(int i=0; i<TOTAL_BLOCKTEXTURE; i++){
		strcpy(texture[i], "");
	}
}

//! @brief �f�B�X�g���N�^
BlockDataInterface::~BlockDataInterface()
{
	//blockdata�\���̉��
	if( data != NULL ){ delete [] data; }

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	//OutputLog.WriteLog(LOG_CLEANUP, "BD1", "");
#endif
}

//! @brief �u���b�N�f�[�^�t�@�C����ǂ݂���
//! @param fname �t�@�C����
//! @return �����F0�@���s�F1
int BlockDataInterface::LoadFiledata(const char *fname)
{
	if( fname == NULL ){ return 1; }

	FILE *fp;
	unsigned char bdata_header[2];
	float bdata_main[80];
	char bdata_mainb[30];

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_LOAD, "BD1", fname);
#endif

#ifdef ENABLE_PATH_DELIMITER_SLASH
	//�p�X��؂蕶����ϊ�
	fname = ChangePathDelimiter(fname);
#endif

	//�t�@�C����ǂݍ���
	fp = fopen(fname, "rb");
	if( fp == NULL ){ return 1; }

	//�e�N�X�`�����擾
	for(int i=0; i<TOTAL_BLOCKTEXTURE; i++){
		fread(texture[i], 1, 31, fp );
	}

	//�f�[�^�����擾
	fread( bdata_header, 1, 2, fp );
	datas = (int)Get2ByteLittleEndian(&(bdata_header[0]));
	if( datas > MAX_BLOCKS ){ datas = MAX_BLOCKS; }

	//�u���b�N�f�[�^�����擾
	for(int i=0; i<datas; i++){
		//�u���b�N�̂𒸓_���Ɩʏ����擾
		fread( bdata_main, 4, 24+48, fp );
		fread( bdata_mainb, 1, 28, fp );

		//�f�[�^�ԍ���ݒ�
		data[i].id = i;

		//�u���b�N�̒��_���ɐݒ�
		for(int j=0; j<8; j++){
			data[i].x[j] = bdata_main[j];
			data[i].y[j] = bdata_main[j+8];
			data[i].z[j] = bdata_main[j+16];
		}

		//�u���b�N�̖ʏ���ݒ�
		for(int j=0; j<6; j++){
			data[i].material[j].textureID = (int)bdata_mainb[j*4];
			data[i].material[j].u[0] = bdata_main[j*4+24];
			data[i].material[j].v[0] = bdata_main[j*4+48];
			data[i].material[j].u[1] = bdata_main[j*4+25];
			data[i].material[j].v[1] = bdata_main[j*4+49];
			data[i].material[j].u[2] = bdata_main[j*4+26];
			data[i].material[j].v[2] = bdata_main[j*4+50];
			data[i].material[j].u[3] = bdata_main[j*4+27];
			data[i].material[j].v[3] = bdata_main[j*4+51];
		}
	}

	//�t�@�C���n���h�������
	fclose( fp );

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_COMPLETE, "", "");
#endif

	return 0;
}

//! @brief �u���b�N�f�[�^�̖@���E�e���Z�o����
//! @param darkflag �u���b�N���Â�����
//! @attention LoadBlockdata()�֐��œǂ݂��񂾌�A��x�������s�B
void BlockDataInterface::CalculationBlockdata(bool darkflag)
{
	int vID[4];
	int uvID[4];
	float g;
	float xs, ys, zs;
	float g1;
	float xs1, ys1, zs1;
	float g2;
	float xs2, ys2, zs2;
	float lx, ly, lz;
	float rx, ry, rz, a;

	//�����̊p�x��ݒ�
	lx = cosf(LIGHT_RX);
	ly = sinf(LIGHT_RY);
	lz = sinf(LIGHT_RX);

	//�e�u���b�N�̖ʏ�񕪏�������
	for(int i=0; i<datas; i++){
		for(int j=0; j<6; j++){

			//�u���b�N���_�f�[�^�̊֘A�t�����擾
			blockdataface(j, vID, uvID);

			//���S�_������
			data[i].material[j].center_x = 0.0f;
			data[i].material[j].center_y = 0.0f;
			data[i].material[j].center_z = 0.0f;

			//4���_�̒��S�_���Z�o
			for(int k=0; k<4; k++){
				data[i].material[j].center_x += data[i].x[ vID[k] ];
				data[i].material[j].center_y += data[i].y[ vID[k] ];
				data[i].material[j].center_z += data[i].z[ vID[k] ];
			}
			data[i].material[j].center_x /= 4;
			data[i].material[j].center_y /= 4;
			data[i].material[j].center_z /= 4;

			//�ʂ̖@���i�x�N�g���j�ƁA���̒��������߂�
			xs1 = ((data[i].y[ vID[3] ] - data[i].y[ vID[2] ]) * (data[i].z[ vID[0] ] - data[i].z[ vID[2] ])) - ((data[i].y[ vID[0] ] - data[i].y[ vID[2] ]) * (data[i].z[ vID[3] ] - data[i].z[ vID[2] ]));
			ys1 = ((data[i].z[ vID[3] ] - data[i].z[ vID[2] ]) * (data[i].x[ vID[0] ] - data[i].x[ vID[2] ])) - ((data[i].z[ vID[0] ] - data[i].z[ vID[2] ]) * (data[i].x[ vID[3] ] - data[i].x[ vID[2] ]));
			zs1 = ((data[i].x[ vID[3] ] - data[i].x[ vID[2] ]) * (data[i].y[ vID[0] ] - data[i].y[ vID[2] ])) - ((data[i].x[ vID[0] ] - data[i].x[ vID[2] ]) * (data[i].y[ vID[3] ] - data[i].y[ vID[2] ]));
			g1 = sqrtf(xs1 * xs1 + ys1 * ys1 + zs1 * zs1);

			//��������̎O�p�`�ł��v�Z����
			xs2 = ((data[i].y[ vID[1] ] - data[i].y[ vID[0] ]) * (data[i].z[ vID[2] ] - data[i].z[ vID[0] ])) - ((data[i].y[ vID[2] ] - data[i].y[ vID[0] ]) * (data[i].z[ vID[1] ] - data[i].z[ vID[0] ]));
			ys2 = ((data[i].z[ vID[1] ] - data[i].z[ vID[0] ]) * (data[i].x[ vID[2] ] - data[i].x[ vID[0] ])) - ((data[i].z[ vID[2] ] - data[i].z[ vID[0] ]) * (data[i].x[ vID[1] ] - data[i].x[ vID[0] ]));
			zs2 = ((data[i].x[ vID[1] ] - data[i].x[ vID[0] ]) * (data[i].y[ vID[2] ] - data[i].y[ vID[0] ])) - ((data[i].x[ vID[2] ] - data[i].x[ vID[0] ]) * (data[i].y[ vID[1] ] - data[i].y[ vID[0] ]));
			g2 = sqrtf(xs2 * xs2 + ys2 * ys2 + zs2 * zs2);

			//�������v�Z���ʂ��̗p����
			if( g1 > g2 ){
				xs = xs1;
				ys = ys1;
				zs = zs1;
				g = g1;
			}
			else{
				xs = xs2;
				ys = ys2;
				zs = zs2;
				g = g2;
			}

			//�@���i�x�N�g���j�𐳋K��
			data[i].material[j].vx = xs / g;
			data[i].material[j].vy = ys / g;
			data[i].material[j].vz = zs / g;

			//�ʂ̖��邳�����߂�
			rx = data[i].material[j].vx + lx;
			ry = data[i].material[j].vy + ly;
			rz = data[i].material[j].vz + lz;
			//a = sqrtf(fabsf(rx*rx + ry*ry + rz*rz)) / 2.0f;
			//data[i].material[j].shadow = a/2 + 0.5f;
			a = sqrtf(rx*rx + ry*ry + rz*rz) / 3.0f;
			data[i].material[j].shadow = a/2;
			if( darkflag == false ){
				data[i].material[j].shadow += 0.5f;
			}
			else{
				data[i].material[j].shadow += 0.3f;
			}
		}
	}
}

//! @brief �u���b�N�f�[�^�ɐݒ肳�ꂽ�e�N�X�`���̃t�@�C�������擾
//! @param fname �t�@�C�������󂯎�镶����^�|�C���^
//! @param id �e�N�X�`���ԍ�
//! @return �����F0�@���s�F0�ȊO
int BlockDataInterface::GetTexture(char *fname, int id)
{
	if( data == NULL ){ return 1; }
	if( fname == NULL ){ return 2; }
	if( (id < 0)||(TOTAL_BLOCKTEXTURE <= id) ){ return 2; }

	//�|�C���^�Ƀe�N�X�`�������R�s�[
	strcpy(fname, texture[id]);

	return 0;
}

//! @brief �u���b�N�f�[�^���擾
//! @param out_data �󂯎��blockdata�^�|�C���^
//! @param id �f�[�^�ԍ�
//! @return �����F0�@���s�F0�ȊO
int BlockDataInterface::Getdata(blockdata *out_data, int id)
{
	if( data == NULL ){ return 1; }
	if( out_data == NULL ){ return 2; }
	if( (id < 0)||(datas <= id) ){ return 2; }

	//�u���b�N�f�[�^���擾
	memcpy(out_data, &(data[id]), sizeof(blockdata));

	return 0;
}

//! @brief �u���b�N���_�f�[�^�̊֘A�t�����擾
//
//	    3 �E----�E2
//	     /|    /|
//	    / |   / |
//	0 �E----�E1 |
//	  |7 �E-| --�E6
//	  |  /  |  /
//	  | /   | /
//	4 �E----�E5
//
//	C-----D
//	|     |
//	|     |
//	B-----A
//
//	��[0](ABCD) =  (1032)
//	��[1]		=  (6745)
//	�O[2]		=  (5401)
//	�E[3]		=  (6512)
//	��[4]		=  (7623)
//	��[5]		=  (4730)
bool blockdataface(int faceID, int* vID, int* uvID)
{
	if( faceID == 0 ){
		if( vID != NULL ){
			vID[0] = 1;		vID[1] = 0;		vID[2] = 3;		vID[3] = 2;
		}
		if( uvID != NULL ){
			uvID[0] = 2;	uvID[1] = 3;	uvID[2] = 0;	uvID[3] = 1;
		}
		return true;
	}
	if( faceID == 1 ){
		if( vID != NULL ){
			vID[0] = 6;		vID[1] = 7;		vID[2] = 4;		vID[3] = 5;
		}
		if( uvID != NULL ){
			uvID[0] = 2;	uvID[1] = 3;	uvID[2] = 0;	uvID[3] = 1;
		}
		return true;
	}
	if( faceID == 2 ){
		if( vID != NULL ){
			vID[0] = 5;		vID[1] = 4;		vID[2] = 0;		vID[3] = 1;
		}
		if( uvID != NULL ){
			uvID[0] = 2;	uvID[1] = 3;	uvID[2] = 0;	uvID[3] = 1;
		}
		return true;
	}
	if( faceID == 3 ){
		if( vID != NULL ){
			vID[0] = 6;		vID[1] = 5;		vID[2] = 1;		vID[3] = 2;
		}
		if( uvID != NULL ){
			uvID[0] = 2;	uvID[1] = 3;	uvID[2] = 0;	uvID[3] = 1;
		}
		return true;
	}
	if( faceID == 4 ){
		if( vID != NULL ){
			vID[0] = 7;		vID[1] = 6;		vID[2] = 2;		vID[3] = 3;
		}
		if( uvID != NULL ){
			uvID[0] = 2;	uvID[1] = 3;	uvID[2] = 0;	uvID[3] = 1;
		}
		return true;
	}
	if( faceID == 5 ){
		if( vID != NULL ){
			vID[0] = 4;		vID[1] = 7;		vID[2] = 3;		vID[3] = 0;
		}
		if( uvID != NULL ){
			uvID[0] = 2;	uvID[1] = 3;	uvID[2] = 0;	uvID[3] = 1;
		}
		return true;
	}
	return false;
}

//! @brief �R���X�g���N�^
PointDataInterface::PointDataInterface()
{
	//pointdata�\���̏�����
	data = new pointdata[MAX_POINTS];

	//�C�x���g���b�Z�[�W������
	for(int i=0; i<MAX_POINTMESSAGES; i++){
		text[i] = new char[MAX_POINTMESSAGEBYTE];
		text[i][0] = '\0';
	}
}

//! @brief �f�B�X�g���N�^
PointDataInterface::~PointDataInterface()
{
	//pointdata�\���̉��
	if( data != NULL ){ delete [] data; }

	//�C�x���g���b�Z�[�W���
	for(int i=0; i<MAX_POINTMESSAGES; i++){
		if( text[i] != NULL ){ delete [] text[i]; }
	}

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	//OutputLog.WriteLog(LOG_CLEANUP, "PD1", "");
#endif
}

//! @brief �|�C���g�f�[�^�t�@�C����ǂ݂���
//! @param fname �t�@�C����
//! @return �����F0�@���s�F1
int PointDataInterface::LoadFiledata(const char *fname)
{
	if( fname == NULL ){ return 1; }

	FILE *fp;
	unsigned char pdata_header[2];
	float pdata_mainf[4];
	signed char pdata_mainc[4];
	char fname2[MAX_PATH];

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_LOAD, "PD1", fname);
#endif

#ifdef ENABLE_PATH_DELIMITER_SLASH
	//�p�X��؂蕶����ϊ�
	fname = ChangePathDelimiter(fname);
#endif

	//�t�@�C����ǂݍ���
	fp = fopen( fname, "rb" );
	if( fp == NULL ){ return 1; }

	//�f�[�^�����擾
	fread( pdata_header, 1, 2, fp );
	datas = (int)Get2ByteLittleEndian(&(pdata_header[0]));
	if( datas > MAX_POINTS ){ datas = MAX_POINTS; }

	//�|�C���g�f�[�^������������
	for(int i=0; i<datas; i++){
		//�f�[�^�ԍ��ݒ�
		data[i].id = i;

		//���W�f�[�^�擾
		fread( pdata_mainf, 4, 4, fp );
		data[i].x = pdata_mainf[0];
		data[i].y = pdata_mainf[1];
		data[i].z = pdata_mainf[2];
		data[i].r = pdata_mainf[3];

		//�p�����[�^�擾
		fread( pdata_mainc, 1, 4, fp );
		data[i].p1 = (signed short int)pdata_mainc[0];
		data[i].p2 = (signed short int)pdata_mainc[1];
		data[i].p3 = (signed short int)pdata_mainc[2];
		data[i].p4 = (signed short int)pdata_mainc[3];
	}

	//�t�@�C���|�C���^�����
	fclose( fp );

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_COMPLETE, "", "");
#endif

	//�C�x���g���b�Z�[�W������
	for(int i=0; i<MAX_POINTMESSAGES; i++){
		text[i][0] = '\0';
	}

	//�u���t�@�C����.msg�v�𐶐�
	strcpy(fname2, fname);
	//PathRemoveExtension(fname2);
	for(int i=(int)strlen(fname2)-1; i>0; i--){
		if( fname2[i] == '.' ){
			fname2[i] = '\0';
			break;
		}
	}
	strcat(fname2, ".msg");

	LoadMSGFiledata(fname2);

	return 0;
}

//! @brief ���b�Z�[�W�f�[�^�t�@�C����ǂ݂���
//! @param fname �t�@�C����
//! @return �����F0�@���s�F1
int PointDataInterface::LoadMSGFiledata(char *fname)
{
	if( fname == NULL ){ return 1; }

	FILE *fp;

#ifdef ENABLE_PATH_DELIMITER_SLASH
	//�p�X��؂蕶����ϊ�
	fname = ChangePathDelimiter(fname);
#endif

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_LOAD, "MSG", fname);
#endif

	//�t�@�C����ǂݍ���
	fp = fopen( fname, "r" );
	if( fp != NULL ){
		//���b�Z�[�W�f�[�^���擾
		for(int i=0; i<MAX_POINTMESSAGES; i++){
			if( fgets(text[i], MAX_POINTMESSAGEBYTE, fp) == NULL ){ break; }

			//'\r'�������'\n'�ɒu��������
			for(int j=0; j<MAX_POINTMESSAGEBYTE; j++){
				if( text[i][j] == '\r' ){ text[i][j] = '\n'; }
			}
		}

		//�t�@�C���|�C���^�����
		fclose( fp );

#ifdef ENABLE_DEBUGLOG
		//���O�ɏo��
		OutputLog.WriteLog(LOG_COMPLETE, "", "");
#endif
		return 0;
	}

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_ERROR, "", "");
#endif
	return 1;
}

//! @brief �|�C���g�f�[�^���擾
//! @param out_data �󂯎��pointdata�^�|�C���^
//! @param id �f�[�^�ԍ�
//! @return �����F0�@���s�F0�ȊO
int PointDataInterface::Getdata(pointdata *out_data, int id)
{
	if( data == NULL ){ return 1; }
	if( out_data == NULL ){ return 2; }
	if( (id < 0)||(datas <= id) ){ return 2; }

	//�f�[�^���|�C���^�ɃR�s�[
	memcpy(out_data, &(data[id]), sizeof(pointdata));

	return 0;
}

//! @brief �|�C���g�̃p�����[�^�[����������
//! @param id �f�[�^�ԍ�
//! @param p1 ��1�p�����[�^
//! @param p2 ��2�p�����[�^
//! @param p3 ��3�p�����[�^
//! @param p4 ��4�p�����[�^
//! @return �����F0�@���s�F0�ȊO
//! @warning AI���g�p����p�X�ip1:3 or p1:8�j�ȊO�ւ̎g�p�́A�ۏ؂���Ă��炸�������܂���B
int PointDataInterface::SetParam(int id, signed short int p1, signed short int p2, signed short int p3, signed short int p4)
{
	if( data == NULL ){ return 1; }
	if( (id < 0)||(datas <= id) ){ return 2; }

	//�p�����[�^���㏑��
	data[id].p1 = p1;
	data[id].p2 = p2;
	data[id].p3 = p3;
	data[id].p4 = p4;

	return 0;
}

//! @brief ���b�Z�[�W�i1�s�j���擾
//! @param str ��������󂯎��|�C���^
//! @param id �f�[�^�ԍ�
//! @return �����F0�@���s�F1
int PointDataInterface::GetMessageText(char *str, int id)
{
	if( str == NULL ){ return 1; }
	if( (id < 0)||(MAX_POINTMESSAGES <= id) ){ return 1; }

	//�|�C���^�Ƀ��b�Z�[�W���R�s�[
	strcpy(str, text[id]);
	return 0;
}

//! @brief �|�C���g�f�[�^������
//! @param id �ŏ��ɊY�������f�[�^�ԍ����󂯎��|�C���^�@�iNULL �j
//! @param pmask �p�����[�^�̃}�X�N
//! @param p1 ��1�p�����[�^
//! @param p2 ��2�p�����[�^
//! @param p3 ��3�p�����[�^
//! @param p4 ��4�p�����[�^
//! @param offset �������J�n����f�[�^
//! @return �Y���|�C���g��
//! @note �p�����[�^�̃}�X�N�́Ap1�`p4�̌����Ώۂ��w�肵�܂��B
//! @note 1�`4�r�b�g�ڂ܂ł��g�p���Ap1�F1�r�b�g�ځAp2�F2�r�b�g�ځAp3�F3�r�b�g�ځAp4�F4�r�b�g�ځ@�����ꂼ��Ӗ����܂��B
//! @note �����̑Ώۃr�b�g�F1�@�ΏۊO�̃r�b�g�F0�@�ɐݒ肵�Ă��������B�@�Ȃ��A7�`5�r�b�g�ڂ͖�������܂��B
//! @code
//! //16�i���ŋL�q�̏ꍇ�\
//! 0x02	// p2�݂̂������Ώۂɂ���
//! 0x05	// p1��p3�������Ώۂɂ���
//! 0xF1	// p1�݂̂������Ώۂɂ���@�i7�`5�r�b�g�ڂ͖����j
//! @endcode
int PointDataInterface::SearchPointdata(int* id, unsigned char pmask, signed short int p1, signed short int p2, signed short int p3, signed short int p4, int offset)
{
	int cnt = 0;

	if( offset < 0 ){ offset = 0; }
	if( offset >= datas ){ offset = datas; }

	//�I�t�Z�b�g�l����f�[�^��������
	for(int i=offset; i<datas; i++){

		if( ((pmask&0x01) == 0)||(p1 == data[i].p1) ){		//�}�X�N�Ŏw�肳��Ă��Ȃ����Ap1����v
			if( ((pmask&0x02) == 0)||(p2 == data[i].p2) ){		//�}�X�N�Ŏw�肳��Ă��Ȃ����Ap2����v
				if( ((pmask&0x04) == 0)||(p3 == data[i].p3) ){		//�}�X�N�Ŏw�肳��Ă��Ȃ����Ap3����v
					if( ((pmask&0x08) == 0)||(p4 == data[i].p4) ){		//�}�X�N�Ŏw�肳��Ă��Ȃ����Ap4����v

						if( (cnt == 0)&&(id != NULL) ){			//�ŏ��ɊY�����Aid��NULL�łȂ��B
							*id = i;
						}

						//�����Y������+1
						cnt += 1;

					}
				}
			}
		}

	}

	//�����Y������Ԃ�
	return cnt;
}

//! @brief �|�C���g�f�[�^������
//! @param out_data �ŏ��ɊY�������f�[�^���󂯎��pointdata�^�|�C���^�@�iNULL �j
//! @param pmask �p�����[�^�̃}�X�N
//! @param p1 ��1�p�����[�^
//! @param p2 ��2�p�����[�^
//! @param p3 ��3�p�����[�^
//! @param p4 ��4�p�����[�^
//! @param offset �������J�n����f�[�^
//! @return �Y���|�C���g��
//! @attention ���̊֐��̏ڂ��������́A���I�[�o�[���[�h�֐����������������B
int PointDataInterface::SearchPointdata(pointdata *out_data, unsigned char pmask, signed short int p1, signed short int p2, signed short int p3, signed short int p4, int offset)
{
	int id, total;

	//�������Ń|�C���g������
	total = SearchPointdata(&id, pmask, p1, p2, p3, p4, offset);
	
	//�Y������΃f�[�^���擾
	if( total > 0 ){
		Getdata(out_data, id);
	}

	//�����Y������Ԃ�
	return total;
}

//! @brief �R���X�g���N�^
MIFInterface::MIFInterface()
{
	datas = 0;

	mif = false;
	strcpy(mission_name, "");
	strcpy(mission_fullname, "");
	strcpy(blockfile_path, "");
	strcpy(pointfile_path, "");
	skynumber = 0;
	strcpy(picturefileA_path, "!");
	strcpy(picturefileB_path, "!");
	strcpy(addsmallobject_path, "");
	strcpy(briefingtext, "");
	collision = false;
	screen = false;
	AddSmallObjectData = new addsmallobject[MAX_ADDSMALLOBJECT];
}

//! @brief �f�B�X�g���N�^
MIFInterface::~MIFInterface()
{
	if( AddSmallObjectData != NULL ){ delete [] AddSmallObjectData; }
}

//! @brief �f�[�^�t�@�C����ǂ݂���
//! @param fname �t�@�C����
//! @param addfile �ǉ��t�@�C�����ǂݍ���
//! @return �����F0�@���s�F1
int MIFInterface::LoadFiledata(const char *fname, bool addfile)
{
	if( fname == NULL ){ return 1; }

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_LOAD, "MIF", fname);
#endif

	mif = true;

	//�g���q��.txt�Ȃ��
	if( CheckFileExtension(fname, ".txt") == true ){
		//MIF�t���O�𖳌���
		mif = false;
	}

#ifdef ENABLE_PATH_DELIMITER_SLASH
	//�p�X��؂蕶����ϊ�
	fname = ChangePathDelimiter(fname);
#endif

	if( mif == false ){
		if( LoadDefaultTextFiledata(fname) != 0 ){
			//briefing data open failed
			return 1;
		}
	}
	else{
		if( LoadMissionInfoFiledata(fname) != 0 ){
			//briefing data open failed
			return 1;
		}
	}

#ifdef ENABLE_AUTOLOADMIF
	//�h���C�u���^�[����w�肳��Ă���i���t���p�X�j�Ȃ��
	if( CheckFullPath(fname) == true ){
		char path[MAX_PATH];

		//mif�̃f�B���N�g���擾
		GetFileDirectory(fname, path);

		//�e�t�@�C���p�X���t���p�X�ɏ���������
		ChangeExePathToFullPath(path, blockfile_path);
		ChangeExePathToFullPath(path, pointfile_path);
		ChangeExePathToFullPath(path, picturefileA_path);
		ChangeExePathToFullPath(path, picturefileB_path);
		ChangeExePathToFullPath(path, addsmallobject_path);
	}
#endif

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_COMPLETE, "", "");
#endif


	//�ǉ��������������l��
	for(int i=0; i<MAX_ADDSMALLOBJECT; i++){
		strcpy(AddSmallObjectData[i].modelpath, "");
		strcpy(AddSmallObjectData[i].texturepath, "");
		AddSmallObjectData[i].decide = 0;
		AddSmallObjectData[i].hp = 0;
		strcpy(AddSmallObjectData[i].soundpath, "");
		AddSmallObjectData[i].jump = 0;
	}

	//��������̒ǉ��������t�@�C�����w�肳��Ă����
	if( (addfile == true)&&(strcmp(addsmallobject_path, "") != 0)&&(strcmp(addsmallobject_path, "!") != 0) ){
		LoadAddSmallObjectFiledata(addsmallobject_path);
	}
#ifdef ENABLE_DEBUGLOG
	else{
		//���O�ɏo��
		OutputLog.WriteLog(LOG_CHECK, "(Add SmallObject)", "File not found");
	}
#endif

#ifdef ENABLE_AUTOLOADMIF
	//�h���C�u���^�[����w�肳��Ă���i���t���p�X�j�Ȃ��
	if( CheckFullPath(fname) == true ){
		char path[MAX_PATH];

		//mif�̃f�B���N�g���擾
		GetFileDirectory(fname, path);

		//�e�t�@�C���p�X���t���p�X�ɏ���������
		for(int i=0; i<MAX_ADDSMALLOBJECT; i++){
			ChangeExePathToFullPath(path, AddSmallObjectData[i].modelpath);
			ChangeExePathToFullPath(path, AddSmallObjectData[i].texturepath);
			ChangeExePathToFullPath(path, AddSmallObjectData[i].soundpath);
		}
	}
#endif

	return 0;
}

//! @brief �~�b�V�����t�@�C���i.txt�j��ǂ݂���
//! @param fname �t�@�C����
//! @return �����F0�@���s�F1
int MIFInterface::LoadDefaultTextFiledata(const char *fname)
{
	if( fname == NULL ){ return 1; }

	FILE *fp;
	char str[64];

	//�t�@�C�����J��
	fp = fopen( fname, "r" );
	if( fp != NULL ){
		//�摜A���擾
		fgets(str, 64, fp);
		DeleteLinefeed(str);
		if( strcmp(str, "!") == 0 ){
			strcpy(picturefileA_path, "!");
		}
		else{
			//�udata\\briefing\\�@�`�@.bmp�v�𐶐�
			strcpy(picturefileA_path, "data\\briefing\\");
			strcat(picturefileA_path, str);
			strcat(picturefileA_path, ".bmp");
		}

		//�摜B���擾
		fgets(str, 64, fp);
		DeleteLinefeed(str);
		if( strcmp(str, "!") == 0 ){
			strcpy(picturefileB_path, "!");
		}
		else{
			//�udata\\briefing\\�@�`�@.bmp�v�𐶐�
			strcpy(picturefileB_path, "data\\briefing\\");
			strcat(picturefileB_path, str);
			strcat(picturefileB_path, ".bmp");
		}

		//�w�i��̔ԍ�
		fgets(str, 16, fp);
		DeleteLinefeed(str);
		skynumber = atoi(str);

		//�u���[�t�B���O�e�L�X�g���擾
		strcpy(briefingtext, "");
		for(int i=0; i<17; i++ ){
			if( fgets(str, 50, fp) == NULL ){ break; }
			strcat(briefingtext, str);
			datas += 1;
		}

		//�t�@�C���n���h�������
		fclose( fp );

		//�擾�ł��Ȃ��l�̏�����
		strcpy(mission_name, "");
		strcpy(mission_fullname, "");
		strcpy(blockfile_path, "");
		strcpy(pointfile_path, "");
		strcpy(addsmallobject_path, "");
		collision = false;
		screen = false;

		return 0;
	}

	return 1;
}

//! @brief �~�b�V�����t�@�C���i.mif�j��ǂ݂���
//! @param fname �t�@�C����
//! @return �����F0�@���s�F1
int MIFInterface::LoadMissionInfoFiledata(const char *fname)
{
	if( fname == NULL ){ return 1; }

	FILE *fp;
	char str[64];

	//�t�@�C�����J��
	fp = fopen( fname, "r" );
	if( fp != NULL ){
		//�~�b�V�������ʖ�
		fgets(mission_name, 24, fp);
		DeleteLinefeed(mission_name);

		//�~�b�V������������
		fgets(mission_fullname, 64, fp);
		DeleteLinefeed(mission_fullname);

		//�u���b�N�f�[�^�t�@�C��
		fgets(blockfile_path, _MAX_PATH, fp);
		DeleteLinefeed(blockfile_path);

		//�|�C���g�f�[�^�t�@�C��
		fgets(pointfile_path, _MAX_PATH, fp);
		DeleteLinefeed(pointfile_path);

		//�w�i��̔ԍ�
		fgets(str, 16, fp);
		DeleteLinefeed(str);
		skynumber = atoi(str);

		//�����蔻��E��ʐݒ�̎擾
		fgets(str, 16, fp);
		DeleteLinefeed(str);
		if( strcmp(str, "1") == 0 ){
			collision = true;
			screen = false;
		}
		else if( strcmp(str, "2") == 0 ){
			collision = false;
			screen = true;
		}
		else if( strcmp(str, "3") == 0 ){
			collision = true;
			screen = true;
		}
		else{	// == "0"
			collision = false;
			screen = false;
		}

		//�ǉ��������t�@�C���擾
		fgets(addsmallobject_path, _MAX_PATH, fp);
		DeleteLinefeed(addsmallobject_path);

		//�摜A���擾
		fgets(picturefileA_path, _MAX_PATH, fp);
		DeleteLinefeed(picturefileA_path);

		//�摜B���擾
		fgets(picturefileB_path, _MAX_PATH, fp);
		DeleteLinefeed(picturefileB_path);

		//�u���[�t�B���O�e�L�X�g���擾
		strcpy(briefingtext, "");
		for(int i=0; i<17; i++ ){
			if( fgets(str, 50, fp) == NULL ){ break; }
			strcat(briefingtext, str);
			datas += 1;
		}

		//�t�@�C���n���h�������
		fclose( fp );

		return 0;
	}

	return 1;
}

//! @brief �ǉ��������t�@�C����ǂ݂���
//! @param fname �t�@�C����
//! @return �����F0�@���s�F1
int MIFInterface::LoadAddSmallObjectFiledata(const char *fname)
{
	if( fname == NULL ){ return 1; }

	FILE *fp;
	char str1[_MAX_PATH];
	char str2[_MAX_PATH];
	char str3[32];
	char str4[32];
	char str5[_MAX_PATH];
	char str6[32];

	char str[64];

#ifdef ENABLE_PATH_DELIMITER_SLASH
	//�p�X��؂蕶����ϊ�
	fname = ChangePathDelimiter(fname);
#endif

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_LOAD, "(Add SmallObject)", fname);
#endif

	//�t�@�C�����J��
	fp = fopen( fname, "r" );
	if( fp != NULL ){
		for(int i=0; i<MAX_ADDSMALLOBJECT; i++){
			//6�s�擾
			//6�s���ׂĎ擾�ł��Ȃ���Ύ��s
			if( fgets(str1, _MAX_PATH, fp) == NULL ){ break; }
			if( fgets(str2, _MAX_PATH, fp) == NULL ){ break; }
			if( fgets(str3, 16, fp) == NULL ){ break; }
			if( fgets(str4, 16, fp) == NULL ){ break; }
			if( fgets(str5, _MAX_PATH, fp) == NULL ){ break; }
			if( fgets(str6, 16, fp) == NULL ){ break; }


			//���f���f�[�^�p�X
			DeleteLinefeed(str1);
			strcpy(AddSmallObjectData[i].modelpath, str1);

			//�e�N�X�`���p�X
			DeleteLinefeed(str2);
			strcpy(AddSmallObjectData[i].texturepath, str2);

			//�����蔻��̑傫��
			DeleteLinefeed(str3);
			AddSmallObjectData[i].decide = atoi(str3);
#ifdef ENABLE_ADDOBJ_PARAM8BIT
			AddSmallObjectData[i].decide = AddSmallObjectData[i].decide & 0x0000007F;
#endif

			//�ϋv��
			DeleteLinefeed(str4);
			AddSmallObjectData[i].hp = atoi(str4);
#ifdef ENABLE_ADDOBJ_PARAM8BIT
			AddSmallObjectData[i].hp = AddSmallObjectData[i].hp & 0x0000007F;
#endif

			//�T�E���h�f�[�^�p�X
			DeleteLinefeed(str5);
			strcpy(AddSmallObjectData[i].soundpath, str5);

			//��ы
			DeleteLinefeed(str6);
			AddSmallObjectData[i].jump = atoi(str6);
#ifdef ENABLE_ADDOBJ_PARAM8BIT
			AddSmallObjectData[i].jump = AddSmallObjectData[i].jump & 0x000000FF;
#endif

			//���̒ǉ��������̑O�ɂ���A�_�~�[ 1�s��ǂݏo��
			if( i+1 < MAX_ADDSMALLOBJECT ){
				if( fgets(str, 32, fp) == NULL ){ break; }
			}
		}

		//�t�@�C���n���h�������
		fclose( fp );

#ifdef ENABLE_DEBUGLOG
		//���O�ɏo��
		OutputLog.WriteLog(LOG_COMPLETE, "", "");
#endif
		return 0;
	}

#ifdef ENABLE_DEBUGLOG
	//���O�ɏo��
	OutputLog.WriteLog(LOG_ERROR, "", "");
#endif
	return 1;
}

//! @brief addon�t�H���_���Q�Ƃ��鑊�΃p�X��.exe���.mif��֕ύX
//! @param dir .mif�t�@�C���܂ł̃t���p�X
//! @param fname �����Ώۂ̃t�@�C���p�X
//! @return �ϊ��ς݁Ftrue�@���ϊ��Ffalse
//! @attention fname�̃|�C���^�𒼐ڏ��������܂��B
bool MIFInterface::ChangeExePathToFullPath(char *dir, char *fname)
{
	if( dir == NULL ){ return false; }
	if( fname == NULL ){ return false; }

	char str[MAX_PATH];
	int index = 0;

	//�����ȃt�@�C���p�X�Ȃ�Ώ������Ȃ�
	if( strcmp(fname, "") == 0 ){ return false; }
	if( strcmp(fname, "!") == 0 ){ return false; }

	//addon�t�H���_�����Q�Ƃ��Ă��邩����
	sprintf(str, "%s", ADDONDIR);
	if( memcmp(fname, str, strlen(str)) == 0 ){ index = (int)strlen(str); }
	sprintf(str, "\\%s", ADDONDIR);
	if( memcmp(fname, str, strlen(str)) == 0 ){ index = (int)strlen(str); }
	sprintf(str, "/%s", ADDONDIR);
	if( memcmp(fname, str, strlen(str)) == 0 ){ index = (int)strlen(str); }
	sprintf(str, ".\\%s", ADDONDIR);
	if( memcmp(fname, str, strlen(str)) == 0 ){ index = (int)strlen(str); }
	sprintf(str, "./%s", ADDONDIR);
	if( memcmp(fname, str, strlen(str)) == 0 ){ index = (int)strlen(str); }

	//addon�t�H���_�����Q�Ƃ��Ă���Ȃ��
	if( index != 0 ){
		//������̕�������MAX_PATH�𒴂���悤�Ȃ珈�����f
		if( strlen(dir)+strlen(fname)-index+1 >= MAX_PATH -1 ){ return false; }

		//addon�̕���������
		strcpy(str, fname);
		for(int i=0; i<(int)strlen(str)-index+1; i++){ str[i] = str[i+index]; }

		//.mif�܂ł̃p�X�{�����Ώۂ̃t�@�C���p�X
		strcpy(fname, dir);
		strcat(fname, str);
		return true;
	}
	return false;
}

//! @brief �ǂݍ��񂾃f�[�^�t�@�C���̌`�����擾
//! @return �W���`���Ffalse�@MIF�`���Ftrue
//! @attention �t�@�C���𐳏�ɓǂݍ��񂾌�Ɏ��s���Ă��������B
bool MIFInterface::GetFiletype()
{
	return mif;
}

//! @brief �~�b�V�������ʖ����擾
//! @return ���ʖ��̃|�C���^�i�ő�F24�j
char* MIFInterface::GetMissionName()
{
	return mission_name;
}

//! @brief �~�b�V�����������̂��擾
//! @return �������̂̃|�C���^�i�ő�F64�j
char* MIFInterface::GetMissionFullname()
{
	return mission_fullname;
}

//! @brief �u���b�N�f�[�^�ƃ|�C���g�f�[�^�̃p�X���擾
//! @param *blockfile �u���b�N�f�[�^���󂯎��|�C���^
//! @param *pointfile �|�C���g�f�[�^���󂯎��|�C���^
void MIFInterface::GetDatafilePath(char *blockfile, char *pointfile)
{
	if( blockfile == NULL ){ return; }
	if( pointfile == NULL ){ return; }

	strcpy(blockfile, blockfile_path);
	strcpy(pointfile, pointfile_path);
}

//! @brief �w�i����擾
//! @return ��̔ԍ��i0�`5�j
//! @attention �ԍ� 0 �́u�w�i�Ȃ��v���Ӗ����܂��B
int MIFInterface::GetSkynumber()
{
	return skynumber;
}

//! @brief �u���[�t�B���O�摜�t�@�C���̃p�X���擾
//! @param *picturefileA �摜�t�@�C��A�̃p�X���󂯎��|�C���^
//! @param *picturefileB �摜�t�@�C��B�̃p�X���󂯎��|�C���^
//! @attention �ݒ肳��Ă��Ȃ��ꍇ�́u!�v���Ԃ���܂��B
//! @attention �摜��1�������g�p���Ȃ��ꍇ�A�摜�t�@�C��B�́u!�v��Ԃ��܂��B
void MIFInterface::GetPicturefilePath(char *picturefileA, char *picturefileB)
{
	if( picturefileA == NULL ){ return; }
	if( picturefileB == NULL ){ return; }

	strcpy(picturefileA, picturefileA_path);
	strcpy(picturefileB, picturefileB_path);
}

//! @brief �u���[�t�B���O���́i�{���j���擾
//! @return ���͂̃|�C���^�i�ő�F816�j
//! @attention ���s�R�[�h���܂߂āA�ő�17�s������x�ɕԂ���܂��B
//! @attention �s���� GetTotaldatas() �Ŏ擾�ł��܂��B
char* MIFInterface::GetBriefingText()
{
	return briefingtext;
}

//! @brief �ǉ��̓����蔻��������t���O���擾
//! @return �L���Ftrue�@�����Ffalse
bool MIFInterface::GetCollisionFlag()
{
	return collision;
}

//! @brief ��ʂ��Â��������t���O���擾
//! @return �L���Ftrue�@�����Ffalse
bool MIFInterface::GetScreenFlag()
{
	return screen;
}

//! @brief �ǉ��������t�@�C���p�X���擾
//! @return �ǉ��������t�@�C���p�X�̃|�C���^�i�ő�F_MAX_PATH�j
//! @attention �{�֐��͒ʏ�K�v����܂���B���f���f�[�^�p�X�Ȃǂ̃t�@�C�����̏��́A���̊֐��Ŏ擾�ł��܂��B
char* MIFInterface::GetAddSmallobjectFile()
{
	return addsmallobject_path;
}

//! @brief �ǉ������̃��f���f�[�^�p�X���擾
//! @param id �f�[�^�ԍ��i�ʏ�� 0 �Œ�j
//! @return ���f���f�[�^�p�X�̃|�C���^�i�ő�F_MAX_PATH�j
char* MIFInterface::GetAddSmallobjectModelPath(int id)
{
	if( (id < 0)||(MAX_ADDSMALLOBJECT <= id) ){ return NULL; }
	return AddSmallObjectData[id].modelpath;
}

//! @brief �ǉ������̃e�N�X�`���p�X���擾
//! @param id �f�[�^�ԍ��i�ʏ�� 0 �Œ�j
//! @return �e�N�X�`���p�X�̃|�C���^�i�ő�F_MAX_PATH�j
char* MIFInterface::GetAddSmallobjectTexturePath(int id)
{
	if( (id < 0)||(MAX_ADDSMALLOBJECT <= id) ){ return NULL; }
	return AddSmallObjectData[id].texturepath;
}

//! @brief �ǉ������̓����蔻��̑傫�����擾
//! @param id �f�[�^�ԍ��i�ʏ�� 0 �Œ�j
//! @return �����蔻��̑傫��
int MIFInterface::GetAddSmallobjectDecide(int id)
{
	if( (id < 0)||(MAX_ADDSMALLOBJECT <= id) ){ return 0; }
	return AddSmallObjectData[id].decide;
}

//! @brief �ǉ������̑ϋv�͂��擾
//! @param id �f�[�^�ԍ��i�ʏ�� 0 �Œ�j
//! @return �ϋv��
int MIFInterface::GetAddSmallobjectHP(int id)
{
	if( (id < 0)||(MAX_ADDSMALLOBJECT <= id) ){ return 0; }
	return AddSmallObjectData[id].hp;
}

//! @brief �ǉ������̃T�E���h�f�[�^�p�X���擾
//! @param id �f�[�^�ԍ��i�ʏ�� 0 �Œ�j
//! @return �T�E���h�f�[�^�p�X�̃|�C���^�i�ő�F_MAX_PATH�j
char* MIFInterface::GetAddSmallobjectSoundPath(int id)
{
	if( (id < 0)||(MAX_ADDSMALLOBJECT <= id) ){ return NULL; }
	return AddSmallObjectData[id].soundpath;
}

//! @brief �ǉ������̔�ы���擾
//! @param id �f�[�^�ԍ��i�ʏ�� 0 �Œ�j
//! @return ��ы
int MIFInterface::GetAddSmallobjectJump(int id)
{
	if( (id < 0)||(MAX_ADDSMALLOBJECT <= id) ){ return 0; }
	return AddSmallObjectData[id].jump;
}

//! @brief �R���X�g���N�^
AddonList::AddonList()
{
	datas = 0;

	for(int i=0; i<MAX_ADDONLIST; i++){
		strcpy(filename[i], "");
		strcpy(mission_name[i], "");
	}
}

//! @brief �f�B�X�g���N�^
AddonList::~AddonList()
{}

//! @brief .mif�t�@�C�����擾
void AddonList::GetMIFlist(const char *dir)
{
	if( dir == NULL ){ return; }

	char SearchDIR[_MAX_PATH];
	HANDLE hFind;
	WIN32_FIND_DATA FindFileData;

	//.mif�̌��������𐶐�
	strcpy(SearchDIR, dir);
	strcat(SearchDIR, "\\*.mif");

	//����
	hFind = FindFirstFile(SearchDIR, &FindFileData);
	if( hFind != INVALID_HANDLE_VALUE ){
		strcpy(filename[0], FindFileData.cFileName);
		datas += 1;

		while( FindNextFile(hFind, &FindFileData) == TRUE ){
			strcpy(filename[datas], FindFileData.cFileName);
			datas += 1;
		}
	}
	FindClose(hFind);
}

//! @brief �~�b�V���������擾
void AddonList::GetMissionName(const char *dir)
{
	if( dir == NULL ){ return; }

	for(int i=0; i<datas; i++){
		char str[_MAX_PATH];
		MIFInterface mifdata;

		//�t�@�C�����𐶐�
		strcpy(str, dir);
		strcat(str, "\\");
		strcat(str, filename[i]);

		//MIFInterface�œǂݍ���
		mifdata.LoadFiledata(str, false);

		//�~�b�V���������擾
		strcpy(mission_name[i], mifdata.GetMissionName());
	}
}

//! @brief �~�b�V���������\�[�g����
void AddonList::Sort()
{
	if( datas <= 1 ){ return; }

	char mission_name_c[MAX_ADDONLIST][24];
	char temp[_MAX_PATH];
	int cmp;

	//�~�b�V����������x�������ɕϊ�����
	for(int i=0; i<datas; i++){
		for(int j=0; j<(int)strlen(mission_name[i]); j++){
			mission_name_c[i][j] = (char)tolower(mission_name[i][j]);
		}
		mission_name_c[i][strlen(mission_name[i])] = '\0';
	}

	//�ᑬ�ȃo�u���\�[�g�@�i�O�O�G
	for(int i=0; i<datas-1; i++){
		for(int j=i+1; j<datas; j++){
			cmp = strcmp(mission_name_c[i], mission_name_c[j]);
			if(cmp > 0){
				strcpy(temp, mission_name_c[i]);
				strcpy(mission_name_c[i], mission_name_c[j]);
				strcpy(mission_name_c[j], temp);

				strcpy(temp, mission_name[i]);
				strcpy(mission_name[i], mission_name[j]);
				strcpy(mission_name[j], temp);

				strcpy(temp, filename[i]);
				strcpy(filename[i], filename[j]);
				strcpy(filename[j], temp);
			}
		}
	}

}

//! @brief ADDON���X�g���擾����
//! @param dir ADDON�i.mif�j���������f�B���N�g���@�i�W���F"addon\\"�j
//! @return addon�̑���
int AddonList::LoadFiledata(const char *dir)
{
	if( dir == NULL ){ return 0; }

	datas = 0;

	//.mif�t�@�C�����擾
	GetMIFlist(dir);

	//�~�b�V���������擾
	GetMissionName(dir);

	//�~�b�V���������\�[�g����
	Sort();

#ifdef ENABLE_DEBUGLOG
	char str[128];
	sprintf(str, "Research addon directory (Directory name : [%s]  Total : %d )", dir, datas);

	//���O�ɏo��
	OutputLog.WriteLog(LOG_CHECK, "Directory", str);
#endif

	return datas;
}

//! @brief �~�b�V���������擾
//! @param id �f�[�^�ԍ�
//! @return �~�b�V������
char* AddonList::GetMissionName(int id)
{
	if( (id < 0)||(MAX_ADDONLIST <= id ) ){ return NULL; }
	return mission_name[id];
}

//! @brief �t�@�C�������擾
//! @param id �f�[�^�ԍ�
//! @return �t�@�C����
char* AddonList::GetFileName(int id)
{
	if( (id < 0)||(MAX_ADDONLIST <= id ) ){ return NULL; }
	return filename[id];
}

//! @brief �R���X�g���N�^
INIFileInterface::INIFileInterface()
{
	inifp = NULL;
}

//! @brief �f�B�X�g���N�^
INIFileInterface::~INIFileInterface()
{
	ReleaseINIFile();
}

//! @brief INI�t�@�C����ǂݍ���
//! @param fname �t�@�C����
//! @return �����Ffalse�@���s�Ftrue
//! @attention ����INI�t�@�C����ǂݍ���ł���ꍇ�AReleaseINIFile()�֐����g���܂Ŗ{�֐��͎��s���܂��B
bool INIFileInterface::LoadINIFile(const char *fname)
{
	if( fname == NULL ){ return true; }
	if( inifp != NULL ){ return true; }

#ifdef ENABLE_PATH_DELIMITER_SLASH
	//�p�X��؂蕶����ϊ�
	fname = ChangePathDelimiter(fname);
#endif

	inifp = fopen(fname, "rb");
	if( inifp == NULL ){ return true; }

	return false;
}

//! @brief ��������擾
//! @param sectionname �Z�N�V�������iNULL�j
//! @param keyname �L�[��
//! @param defaultvalue �f�t�H���g�l
//! @param valuestr �l
//! @param strbuflen �ő啶�����i-1�Ŗ����j
//! @return �G���[�R�[�h
//! @attention �t�@�C�����J����Ă��Ȃ��� �L�[��������Ȃ���΁A�f�t�H���g�l��Ԃ��܂��B
//! @attention �擾���������񂪍ő啶�����𒴂��Ă���ꍇ�A�ő啶�����܂Ő؂���܂��B
//! @note �G���[�R�[�h�@1�F�����ُ�A2�F�t�@�C�����J����Ă��Ȃ��A3�F�l��������Ȃ�
int INIFileInterface::GetINIFileString(const char *sectionname, const char *keyname, const char *defaultvalue, char *valuestr, int strbuflen)
{
	if( valuestr == NULL ){ return 1; }
	if( defaultvalue == NULL ){ strcpy(valuestr, ""); return 1; }
	if( keyname == NULL ){ strcpy(valuestr, defaultvalue); return 1; }

	int state = 0;
	char readline[512];
	char buf[512];

	//�t�@�C�����ǂݍ��܂�Ă��Ȃ���Ύ��s
	if( inifp == NULL ){
		strcpy(valuestr, defaultvalue);
		return 2;
	}

	fseek(inifp, 0, SEEK_SET);
	strcpy(readline, "");

	//�Z�N�V�����������w��Ȃ�΁A�Z�N�V����������𖳌���
	if( sectionname == NULL ){
		state = 1;
	}
	else if( sectionname[0] == '\0' ){
		state = 1;
	}

	//�f�[�^�T�C�Y�ϐ��`�F�b�N
	if( strbuflen <= -2 ){ strbuflen = 0; }
	if( strbuflen == -1 ){ strbuflen = 512; }
	if( strbuflen > 512 ){ strbuflen = 512; }

	while( fgets(readline, 512, inifp) != NULL ){
		//�R�����g�s�Ȃ疳��
		if( readline[0] == ';' ){ continue; }

		//�Z�N�V����������
		if( state == 0 ){//if( sectionname != NULL ){
			char str[64];
			sprintf(str, "[%s]", sectionname);
			strcpy(buf, readline);
			buf[ strlen(str) ] = '\0';
			if( strcmp(buf, str) == 0 ){
				state = 1;
				continue;
			}
		}


		//�L�[������
		if( state == 1 ){
			//�ꕶ���ڂ�"["�Ȃ疳��
			if( readline[0] == '[' ){ break; }

			char readline2[512];
			int strcnt = 0;
			int quotationmode = 0;
			char str[64];

			//�X�y�[�X/�R�����g/"" �Ȃǂ��Ȃ������𐶐�
			strcpy(readline2, "");
			for(int i=0; i<(int)strlen(readline); i++){
				if( (readline[i] == '\r')||(readline[i] == '\n') ){ break; }

				if( quotationmode == 0 ){
					if( (readline[i] == ' ')||(readline[i] == '\t') ){ continue; }
					if( readline[i] == ';' ){ break; }

					if( readline[i] == '\'' ){
						quotationmode = 1;
						continue;
					}
					if( readline[i] == '"' ){
						quotationmode = 2;
						continue;
					}
				}
				if( (quotationmode == 1)&&(readline[i] == '\'') ){
					quotationmode = 0;
					continue;
				}
				if( (quotationmode == 2)&&(readline[i] == '"') ){
					quotationmode = 0;
					continue;
				}

				readline2[strcnt] = readline[i];
				readline2[strcnt+1] = '\0';
				strcnt += 1;
			}

			//"="�̌��̕������擾������
			sprintf(str, "%s=", keyname);
			strcpy(buf, readline2);
			buf[ strlen(str) ] = '\0';
			if( strcmp(buf, str) == 0 ){
				state = 1;

				//�f�[�^�T�C�Y����
				strcpy(buf, &(readline2[strlen(str)]));
				buf[strbuflen-1] = '\0';

				//�l��Ԃ�
				strcpy(valuestr, buf);
				return 0;
			}
		}
	}

	//�l��������Ȃ���΃G���[��Ԃ�
	strcpy(valuestr, defaultvalue);
	return 3;
}

//! @brief int�l���擾
//! @param sectionname �Z�N�V�������iNULL�j
//! @param keyname �L�[��
//! @param defaultvalue �f�t�H���g�l
//! @param errorcode �G���[�R�[�h���擾����|�C���^�iNULL�j
//! @return �l
//! @attention �t�@�C�����J����Ă��Ȃ��� �L�[��������Ȃ���΁A�f�t�H���g�l��Ԃ��܂��B
//! @note �G���[�R�[�h�@1�F�����ُ�A2�F�t�@�C�����J����Ă��Ȃ��A3�F�l��������Ȃ�
int INIFileInterface::GetINIFileInt(const char *sectionname, const char *keyname, int defaultvalue, int *errorcode)
{
	if( keyname == NULL ){ return defaultvalue; }

	char defaultint[64];
	char buf[64];
	int error;

	sprintf(defaultint, "%d", defaultvalue);
	error = GetINIFileString(sectionname, keyname, defaultint, buf, 64);
	if( errorcode != NULL ){ *errorcode = error; }
	return atoi(buf);
}

//! @brief float�l���擾
//! @param sectionname �Z�N�V�������iNULL�j
//! @param keyname �L�[��
//! @param defaultvalue �f�t�H���g�l
//! @param errorcode �G���[�R�[�h���擾����|�C���^�iNULL�j
//! @return �l
//! @attention �t�@�C�����J����Ă��Ȃ��� �L�[��������Ȃ���΁A�f�t�H���g�l��Ԃ��܂��B
//! @note �G���[�R�[�h�@1�F�����ُ�A2�F�t�@�C�����J����Ă��Ȃ��A3�F�l��������Ȃ�
float INIFileInterface::GetINIFileFloat(const char *sectionname, const char *keyname, float defaultvalue, int *errorcode)
{
	if( keyname == NULL ){ return defaultvalue; }

	char defaultfloat[64];
	char buf[64];
	int error;

	sprintf(defaultfloat, "%f", defaultvalue);
	error = GetINIFileString(sectionname, keyname, defaultfloat, buf, 64);
	if( errorcode != NULL ){ *errorcode = error; }
	for(int i=(int)strlen(buf)-1; i>0; i--){
		if( buf[i] == 'f' ){ buf[i] = '\0'; }
		if( buf[i] == 'F' ){ buf[i] = '\0'; }
	}
	return (float)atof(buf);
}

//! @brief bool�l���擾
//! @param sectionname �Z�N�V�������iNULL�j
//! @param keyname �L�[��
//! @param defaultvalue �f�t�H���g�l
//! @param errorcode �G���[�R�[�h���擾����|�C���^�iNULL�j
//! @return �l
//! @attention �t�@�C�����J����Ă��Ȃ��� �L�[��������Ȃ���΁A�f�t�H���g�l��Ԃ��܂��B
//! @note �G���[�R�[�h�@1�F�����ُ�A2�F�t�@�C�����J����Ă��Ȃ��A3�F�l��������Ȃ�
bool INIFileInterface::GetINIFileBool(const char *sectionname, const char *keyname, bool defaultvalue, int *errorcode)
{
	if( keyname == NULL ){ return defaultvalue; }

	char defaultbool[64];
	char buf[64];
	int error;

	if( defaultvalue == false ){
		strcpy(defaultbool, "false");
	}
	else{
		strcpy(defaultbool, "true");
	}
	error = GetINIFileString(sectionname, keyname, defaultbool, buf, 64);
	if( errorcode != NULL ){ *errorcode = error; }
	for(int j=0; j<=(int)strlen(buf); j++){ if(('A'<=buf[j])&&(buf[j]<='Z')){ buf[j] += 32; } }
	if( strcmp(buf, "false") == 0 ){
		return false;
	}
	else if( strcmp(buf, "0") == 0 ){
		return false;
	}
	//else{
		return true;
	//}
}

//! @brief INI�t�@�C�������
void INIFileInterface::ReleaseINIFile()
{
	if( inifp == NULL ){ return; }

	fclose(inifp);
	inifp = NULL;
}


//! @brief fgets()�p�@���s�R�[�h����菜��
//! @param str ������
//! @return �u����������F0�@�u�������Ȃ��F1
int DeleteLinefeed(char *str)
{
	if( str == NULL ){ return 1; }

	char *pstr;
	bool flag = false;

	pstr = strchr(str, '\n');
	if( pstr ){
		*pstr = '\0';
		flag = true;
	}

	pstr = strchr(str, '\r');
	if( pstr ){
		*pstr = '\0';
		flag = true;
	}

	if( flag == true ){
		return 0;
	}
	return 1;
}

//! @brief �t�@�C���p�X���t���p�X�����肷��
//! @param path �t�@�C���p�X
//! @return �t���p�X�ł���i��΃p�X�j�Ftrue�@�t���p�X�łȂ��i���΃p�X�j�Ffalse
//! @warning Windows�������z�肳��Ă��܂���B
bool CheckFullPath(const char *path)
{
	if( path == NULL ){ return false; }
	if( strlen(path) < 2 ){ return false; }

	if( path[1] == ':' ){ return true; }
	return false;
}

//! @brief �t�@�C���p�X����t�@�C��������菜��
//! @param path �t�@�C���p�X�i�|�C���^�j
//! @param dir �f�B���N�g�������󂯎��|�C���^
void GetFileDirectory(const char *path, char *dir)
{
	if( path == NULL ){ return; }
	if( dir == NULL ){ return; }

	strcpy(dir, path);

	//�I�[����'\\'��'/'��T���A'\0'�ɒu��������
	for(int i=(int)strlen(dir)-1; i>0; i--){
		if( dir[i] == '\\' ){ dir[i+1] = '\0'; break; }
		if( dir[i] == '/' ){ dir[i+1] = '\0'; break; }
	}
}

//! @brief �t�@�C���g���q�𔻒肷��
//! @param filepath �t�@�C����
//! @param checkstr �g���q��
//! @return �Y���Ftrue�@��Y���Ffalse
//! @attention �g���q��"."���݂Ŏw�肵�Ă��������B�啶���E�������͋�ʂ��܂���B
bool CheckFileExtension(const char *filepath, const char *checkstr)
{
	if( filepath == NULL ){ return false; }
	if( checkstr == NULL ){ return false; }
	if( strlen(filepath) >= MAX_PATH ){ return false; }
	if( strlen(checkstr) >= 16 ){ return false; }
	if( strlen(filepath) < strlen(checkstr) ){ return false; }

	char filepath2[MAX_PATH];
	char checkstr2[16];

	//�t�@�C�������������֕ϊ�
	for(int i=0; i<(int)strlen(filepath); i++){
		filepath2[i] = (char)tolower((int)filepath[i]);
	}
	filepath2[ strlen(filepath) ] = '\0';

	//�g���q�����������֕ϊ�
	for(int i=0; i<(int)strlen(checkstr); i++){
		checkstr2[i] = (char)tolower((int)checkstr[i]);
	}
	checkstr2[ strlen(checkstr) ] = '\0';

	//�g���q����
	for(int i=(int)strlen(filepath2)-1; i>0; i--){
		if( filepath2[i] == '.' ){
			if( strcmp(&(filepath2[i]), checkstr2) == 0 ){ return true; }
			return false;
		}
	}
	return false;
}

//! @brief �f�B���N�g���̑��݂𔻒�
//! @param dir �f�B���N�g����
//! @return ���݂���Ftrue�@���݂��Ȃ��Ffalse
bool CheckDirectory(const char *dir)
{
	struct stat statBuf;
	if( stat(dir, &statBuf) == 0 ){ return true; } 
	return false;
}
