//! @file datafile.h
//! @brief �f�[�^�Ǘ��N���X�̐錾

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

#ifndef DATAFILE_H
#define DATAFILE_H

#define MAX_BLOCKS 160			//!< �ő�u���b�N��
#define TOTAL_BLOCKTEXTURE 10	//!< ���v�e�N�X�`����

//#define MAX_POINTS 200
#define MAX_POINTS 256			//!< �ő�|�C���g��

//! @brief �ǉ������̐�
//! @attention �ݒ�l�𑝂₷�ꍇ�AMAX_MODEL�EMAX_TEXTURE�EMAX_LOADSOUND�̒l���K�؂Ɍ���������
//! @note �ǉ������ݒ�t�@�C���̉����ɁA�����Ď��̒ǉ������̐ݒ�l���L�ڂ��܂��B�������A�e�ǉ������̐ݒ�l�̊ԂɃ_�~�[�s�i"//"�����j��}��ł��������B
#define MAX_ADDSMALLOBJECT 1

#define MAX_POINTMESSAGES 16			//!< .msg�t�@�C���̍ő僁�b�Z�[�W��
#define MAX_POINTMESSAGEBYTE (71+2)		//!< .msg�t�@�C�� 1�s������̍ő�o�C�g��

#define LIGHT_RX DegreeToRadian(190)	//!< ���C�g�̉����p�x�i�u���b�N�̉A�e�v�Z�p�j
#define LIGHT_RY DegreeToRadian(120)	//!< ���C�g�̏c���p�x�i�u���b�N�̉A�e�v�Z�p�j

#ifndef H_LAYERLEVEL
 #define H_LAYERLEVEL 1		//!< Select include file.
#endif
#include "main.h"

#include <ctype.h>
#include <sys/stat.h>

//! �u���b�N�f�[�^�p�\���́@�iblockdata�\���̂Ŏg�p�j
typedef struct
{
	int textureID;				//!< textureID
	float u[4];					//!< textureUV-U
	float v[4];					//!< textureUV-V
	float center_x;				//!< center
	float center_y;				//!< center
	float center_z;				//!< center
	float vx;					//!< normal vector 
	float vy;					//!< normal vector 
	float vz;					//!< normal vector 
	float shadow;				//!< shadow 
} b_material;
//! �u���b�N�f�[�^�p�\����
typedef struct
{
	int id;						//!< DataID
	float x[8];					//!< Position
	float y[8];					//!< Position
	float z[8];					//!< Position
	b_material material[6];		//!< Material data
} blockdata;

//! �|�C���g�f�[�^�p�\����
typedef struct
{
	int id;						//!< DataID
	float x;					//!< Position
	float y;					//!< Position
	float z;					//!< Position
	float r;					//!< Rotation
	signed short int p1;		//!< Param
	signed short int p2;		//!< Param
	signed short int p3;		//!< Param
	signed short int p4;		//!< Param
} pointdata;

//�|�C���g�f�[�^��P1�̎�ޔԍ�
#define POINT_P1TYPE_HUMAN			1	//!< Param1 type
#define POINT_P1TYPE_WEAPON			2	//!< Param1 type
#define POINT_P1TYPE_AIPATH			3	//!< Param1 type
#define POINT_P1TYPE_HUMANINFO		4	//!< Param1 type
#define POINT_P1TYPE_SMALLOBJ		5	//!< Param1 type
#define POINT_P1TYPE_HUMAN2			6	//!< Param1 type
#define POINT_P1TYPE_RAND_WEAPON	7	//!< Param1 type
#define POINT_P1TYPE_RAND_AIPATH	8	//!< Param1 type

//! �ǉ������p�̍\����
typedef struct
{
	char modelpath[_MAX_PATH];		//!< ���f���f�[�^�p�X
	char texturepath[_MAX_PATH];	//!< �e�N�X�`���p�X
	int decide;						//!< �����蔻��̑傫��
	int hp;							//!< �ϋv��
	char soundpath[_MAX_PATH];		//!< �T�E���h�f�[�^�p�X
	int jump;						//!< ��ы
} addsmallobject;

//! @brief �f�[�^���Ǘ�����N���X�i���N���X�j
//! @details �Q�[���̃f�[�^���A�t�@�C������ǂݍ��ݏ�������N���X�Q�̊��N���X�ł��B
class DataInterface
{
protected:
	int datas;		//!< �f�[�^��

public:
	DataInterface();
	~DataInterface();
	virtual int LoadFiledata(const char *fname);
	virtual int GetTotaldatas();
	virtual int Getdata(void *out_data, int id);
};

//! @brief �u���b�N�f�[�^���Ǘ�����N���X
//! @details �}�b�v�f�[�^�Ƃ��Ďg���A�u���b�N�f�[�^�t�@�C�����Ǘ����܂��B
//! @details �t�@�C���̓ǂݍ��݂̂ق��A�}�b�v�ւ̉e�i�e�ʂ̖��邳�\���j���v�Z���܂��B
class BlockDataInterface : public DataInterface
{
	blockdata *data;								//!< �u���b�N�f�[�^��\���\����
	char texture[TOTAL_BLOCKTEXTURE][_MAX_PATH];	//!< �e�N�X�`����

public:
	BlockDataInterface();
	~BlockDataInterface();
	int LoadFiledata(const char *fname);
	void CalculationBlockdata(bool darkflag);
	int GetTexture(char *fname, int id);
	int Getdata(blockdata *out_data, int id);
};

bool blockdataface(int faceID, int* vID, int* uvID);

//! @brief �|�C���g�f�[�^���Ǘ�����N���X
//! @details �~�b�V�����f�[�^�Ƃ��Ďg���A�|�C���g�f�[�^�t�@�C�����Ǘ����܂��B
//! @details �t�@�C���̓ǂݍ��݂̂ق��A�|�C���g�̌����@�\������܂��B
class PointDataInterface : public DataInterface
{
	pointdata *data;							//!< �u���b�N�f�[�^��\���\����
	char *text[MAX_POINTMESSAGES];				//!< �C�x���g���b�Z�[�W

	int LoadMSGFiledata(char *fname);

public:
	PointDataInterface();
	~PointDataInterface();
	int LoadFiledata(const char *fname);
	int Getdata(pointdata *out_data, int id);
	int SetParam(int id, signed short int p1, signed short int p2, signed short int p3, signed short int p4);
	int GetMessageText(char *str, int id);
	int SearchPointdata(int* id, unsigned char pmask, signed short int p1, signed short int p2, signed short int p3, signed short int p4, int offset = 0);
	int SearchPointdata(pointdata *out_data, unsigned char pmask, signed short int p1, signed short int p2, signed short int p3, signed short int p4, int offset = 0);
};

//! @brief MIF���Ǘ�����N���X
//! @details MIF�iMISSION INFORMATION FILE�j���Ǘ����܂��B
//! @details �W���~�b�V�����̃u���[�t�B���O�t�@�C���i.txt�j�ɂ��Ή����Ă��܂��B
class MIFInterface : public DataInterface
{
	bool mif;								//!< �t�@�C���`���� .mif
	char mission_name[24];					//!< �~�b�V�������ʖ�
	char mission_fullname[64];				//!< �~�b�V������������
	char blockfile_path[_MAX_PATH];			//!< �u���b�N�f�[�^�t�@�C��
	char pointfile_path[_MAX_PATH];			//!< �|�C���g�f�[�^�t�@�C��
	int skynumber;							//!< �w�i���ID�@�i�Ȃ��F0�j
	char picturefileA_path[_MAX_PATH];		//!< �u���[�t�B���O�摜A
	char picturefileB_path[_MAX_PATH];		//!< �u���[�t�B���O�摜B�i�ǉ����j
	char addsmallobject_path[_MAX_PATH];	//!< �ǉ��������t�@�C���ւ̃p�X
	char briefingtext[816];					//!< �u���[�t�B���O���́E�{��
	bool collision;					//!< �����蔻��𑽂߂ɍs��
	bool screen;					//!< ��ʂ��Â߂ɂ���
	addsmallobject *AddSmallObjectData;		//!< �ǉ������̏��

	int LoadDefaultTextFiledata(const char *fname);
	int LoadMissionInfoFiledata(const char *fname);
	int LoadAddSmallObjectFiledata(const char *fname);
	bool ChangeExePathToFullPath(char *dir, char *fname);

public:
	MIFInterface();
	~MIFInterface();
	int LoadFiledata(const char *fname, bool addfile);
	bool GetFiletype();
	char* GetMissionName();
	char* GetMissionFullname();
	void GetDatafilePath(char *blockfile, char *pointfile);
	int GetSkynumber();
	void GetPicturefilePath(char *picturefileA, char *picturefileB);
	char* GetBriefingText();
	bool GetCollisionFlag();
	bool GetScreenFlag();
	char* GetAddSmallobjectFile();
	char* GetAddSmallobjectModelPath(int id);
	char* GetAddSmallobjectTexturePath(int id);
	int GetAddSmallobjectDecide(int id);
	int GetAddSmallobjectHP(int id);
	char* GetAddSmallobjectSoundPath(int id);
	int GetAddSmallobjectJump(int id);
};

//! @brief ADDON���X�g���Ǘ�����N���X
//! @details ����̃f�B���N�g���ɓ�����.mif��ADDON���X�g�Ƃ��ĊǗ����܂��B
class AddonList : public DataInterface
{
	char filename[MAX_ADDONLIST][_MAX_PATH];	//!< .mif�t�@�C����
	char mission_name[MAX_ADDONLIST][24];		//!< �~�b�V�������ʖ�

	void GetMIFlist(const char *dir);
	void GetMissionName(const char *dir);
	void Sort();

public:
	AddonList();
	~AddonList();
	int LoadFiledata(const char *dir);
	char *GetMissionName(int id);
	char *GetFileName(int id);
};

//! @brief INI�t�@�C�����Ǘ�����N���X
//! @details INI�t�@�C�����Ǘ����܂��B
class INIFileInterface
{
	FILE *inifp;	//!< �t�@�C���|�C���^

public:
	INIFileInterface();
	~INIFileInterface();
	bool LoadINIFile(const char *fname);
	int GetINIFileString(const char *sectionname, const char *keyname, const char *defaultvalue, char *valuestr, int strbuflen);
	int GetINIFileInt(const char *sectionname, const char *keyname, int defaultvalue, int *errorcode);
	float GetINIFileFloat(const char *sectionname, const char *keyname, float defaultvalue, int *errorcode);
	bool GetINIFileBool(const char *sectionname, const char *keyname, bool defaultvalue, int *errorcode);
	void ReleaseINIFile();
};

int DeleteLinefeed(char *str);
bool CheckFullPath(const char *path);
void GetFileDirectory(const char *path, char *dir);
bool CheckFileExtension(const char *filepath, const char *checkstr);
bool CheckDirectory(const char *dir);

#endif