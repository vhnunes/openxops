//! @file debug.cpp
//! @brief DebugLog�N���X�̒�`

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

#include "debug.h"

#ifdef ENABLE_DEBUGLOG

//! @brief �R���X�g���N�^
DebugLog::DebugLog()
{
	//�t�@�C�����𐶐�
	char time[32];
	GetTimeName(time);
	sprintf(fname, GAMENAME"_log-%s.html", time);

	OutputFlag = false;
	error = false;
}

//! @brief �f�B�X�g���N�^
DebugLog::~DebugLog()
{
	//�o�̓t���O��L���Ȃ�΁A�t�@�C���Ƀt�b�^�[��ǉ�
	if( OutputFlag == true ){
		FILE *fp;

		fp = fopen(fname, "a");
		if( fp == NULL ){ return; }

		fprintf(fp, "</table>\n");
		fprintf(fp, "</body>\n");
		fprintf(fp, "</html>\n");

		fclose(fp);
	}
}

//! @brief ���O�t�@�C���̏o�͂�L����
//! @warning ���̊֐������s���Ȃ��ƁA�N���X���̂����삵�܂���B
void DebugLog::EnableOutputLog()
{
	//�o�̓t���O��L��
	OutputFlag = true;
}

//! @brief ���O�t�@�C���̃t�@�C������ύX
//! @param name �t�@�C����
void DebugLog::SetLogName(const char* name)
{
	strcpy(fname, name);
}

//! @brief ���O�t�@�C����V�K�쐬
//! @return �����Ffalse�@���s:true
//! @warning EnableOutputLog()�֐����s��ɁA���̊֐������s����K�v������܂��B
//! @attention ���Ƀt�@�C�������݂���ꍇ�A�㏑���ۑ�����܂��B�����̃t�@�C���͎����܂��B
bool DebugLog::MakeLog()
{
	//�o�̓t���O���L���łȂ���Ώ������Ȃ�
	if( OutputFlag == false ){ return true; }

	FILE *fp;

	time_t timer;
	struct tm *local;

	//�������擾
	timer = time(NULL);
	local = localtime(&timer);


	fp = fopen(fname, "w");
	if( fp == NULL ){ return true; }

	//�w�b�_�[
	fprintf(fp, "<html>\n");
	fprintf(fp, "<head>\n");
	fprintf(fp, "<title>%s %s Debug Log</title>\n", GAMENAME, GAMEVERSION);
	fprintf(fp, "<style type=text/css>\n");
	fprintf(fp, "<!--//\n");
	fprintf(fp, "body{ font-size : 12px; }\n");
	fprintf(fp, "table { font-size : 12px; }\n");
	fprintf(fp, "//-->\n");
	fprintf(fp, "</style>\n");
	fprintf(fp, "</head>\n");

	fprintf(fp, "<body>\n");

	//�y�[�W�㕔
	fprintf(fp, "<p>\n");
	fprintf(fp, "<b>%s %s Debug Log</b><br>\n", GAMENAME, GAMEVERSION);
	fprintf(fp, "Date and time : %04d-%02d-%02d %02d:%02d:%02d<br>\n", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
	fprintf(fp, "</p>\n");

	fprintf(fp, "<table>\n");

	fprintf(fp, "<tr><td><b><font size=-2>System time (ms)</font></b></td><td><b>Title</b></td><td colspan=3><b>Message</b></td></tr>\n");

	fclose(fp);

	//�o�̓t���O��L��
	//OutputFlag = true;

	return false;
}

//! @brief ���O�t�@�C���ɒǋL
//! @param tag �o�̓��[�h��\���萔
//! @param title �^�C�g��
//! @param id ID
//! @return �����Ffalse�@���s:true
//! @attention WriteLog(int tag, const char* title, const char* text)�֐����I�[�o�[���[�h���܂��B
bool DebugLog::WriteLog(int tag, const char* title, int id)
{
	if( title == NULL ){ return true; }

	//�o�̓t���O��L���Ȃ���Ώ������Ȃ�
	if( OutputFlag == false ){ return true; }

	char str[16];
	sprintf(str, "ID�F%d", id);

	//�I�[�o�[���[�h
	return WriteLog(tag, title, str);
}

//! @brief ���O�t�@�C���ɒǋL
//! @param tag �o�̓��[�h��\���萔
//! @param title �^�C�g��
//! @param text �{��
//! @return �����Ffalse�@���s:true
//! @attention �O��A�������iLOG_INIT�j���ǂݍ��݁iLOG_LOAD�j���w�肳�ꂽ��ԂŁA�����iLOG_COMPLETE�j���w�肳��Ȃ������ꍇ�A��ɃG���[�iLOG_ERROR�j�����L�����܂��B
//! @attention ���Ȃ킿�A���������ǂݍ��݂��L�^�����ꍇ�A���Ɋ������L�^����Ȃ���΁A�����I�ɃG���[�Ƃ��ċL�^���܂��B
bool DebugLog::WriteLog(int tag, const char* title, const char* text)
{
	if( title == NULL ){ return true; }
	if( text == NULL ){ return true; }

	//�o�̓t���O��L���Ȃ���Ώ������Ȃ�
	if( OutputFlag == false ){ return true; }

	FILE *fp;

	char tagcolor[32];
	char tagstr[32];

	//������񂩑f���ɃG���[��`���Ă����Ȃ�A�G���[�t���O������
	if( (tag == LOG_COMPLETE)||(tag == LOG_ERROR) ){
		error = false;
	}

	//�G���[�t���O���L���Ȃ�΁A�G���[�������������̂Ƃ��ċL�^�B
	if( error == true ){
		error = false;
		WriteLog(LOG_ERROR, "", "");
	}

	//�萔���當���ƐF�����߂�
	switch(tag){
		case LOG_CHECK:
			strcpy(tagcolor, "008000");
			strcpy(tagstr, "Information");
			break;
		case LOG_INIT:
			strcpy(tagcolor, "FF0080");
			strcpy(tagstr, "Initialize");
			break;
		case LOG_LOAD:
			strcpy(tagcolor, "0000FF");
			strcpy(tagstr, "Open-Load");
			break;
		case LOG_CLEANUP:
			strcpy(tagcolor, "FF8000");
			strcpy(tagstr, "Close-Release");
			break;
		case LOG_COMPLETE:
			strcpy(tagcolor, "008000");
			strcpy(tagstr, "Success");
			break;
		case LOG_ERROR:
			strcpy(tagcolor, "FF0000");
			strcpy(tagstr, "Failed");
			break;
		default:
			strcpy(tagcolor, "000000");
			strcpy(tagstr, "");
	}

	fp = fopen(fname, "a");
	if( fp == NULL ){ return true; }

	//���݂̃~���b
	fprintf(fp, "<tr><td>�@%u�@</td>", GetTimeMS());

	if( (tag != LOG_COMPLETE)&&(tag != LOG_ERROR) ){
		//�ʏ�̏��
		fprintf(fp, "<td width=100><b><font color=#%s>%s</font></b></td>", tagcolor, tagstr);
		fprintf(fp, "<td width=100>%s</td><td colspan=3>%s</td>", title, text);
	}
	else{
		//�����E�G���[���
		fprintf(fp, "<td colspan=2></td><td width=100><b><font color=#%s>%s</font></b></td>", tagcolor, tagstr);
		fprintf(fp, "<td>%s</td><td>%s</td>", title, text);
	}

	fprintf(fp, "</tr>\n");

	fclose(fp);

	//���������ǂݍ��݂Ȃ�A�G���[�t���O�𗧂Ă�
	if( (tag == LOG_INIT)||(tag == LOG_LOAD) ){
		error = true;
	}

	return false;
}

#endif	//ENABLE_DEBUGLOG