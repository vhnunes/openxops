//! @file gui-object.h
//! @brief gui-object�֘A�N���X�̐錾

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

#ifndef OBJECT2D_H
#define OBJECT2D_H

#ifndef H_LAYERLEVEL
 #define H_LAYERLEVEL 2		//!< Select include file.
#endif
#include "main.h"

#define MAX_INPUTBOXLEN 127				//!< ���̓{�b�N�X�����񏉊������̍ő啶����

//! @brief �X�N���[���o�[�̊Ǘ��`��N���X
//! @details �X�N���[���o�[�̏�����`����s���܂��B
//! @attention �c/Y�������̃X�N���[�������z�肵�Ă��܂���B
class ScrollbarObject
{
	int pos_x;				//!< x���W
	int pos_y;				//!< y���W
	int width;				//!< ��
	int height;				//!< ����
	int totalitems;			//!< ���v�A�C�e����
	int showtems;			//!< �\������A�C�e����
	int scrollitem;			//!< �X�N���[�������A�C�e����
	float knob_height;		//!< �m�u�̍���
	float knob_scale;		//!< �m�u�̖ڐ�
	int knob_y;				//!< �m�u��Y���W
	int mouse_mode;			//!< �}�E�X�ɂ��I����
	int color_body;			//!< �{�̂̐F
	int color1_default;		//!< �ʏ펞�̐F�i�O���j
	int color2_default;		//!< �ʏ펞�̐F�i�����j
	int color1_cursor;		//!< �J�[�\���I�����̐F�i�O���j
	int color2_cursor;		//!< �J�[�\���I�����̐F�i�����j
	int color1_select;		//!< �N���b�N���̐F�i�O���j
	int color2_select;		//!< �N���b�N���̐F�i�����j

public:
	ScrollbarObject();
	~ScrollbarObject();
	void Create(int x, int y, int w, int h, int in_totalitems, int in_showtems, int in_scrollitem);
	void SetColor(int body, int default1, int default2, int cursor1, int cursor2, int select1, int select2);
	void SetScrollItem(int id);
	int GetScrollItem();
	void ScrollUP();
	void ScrollDOWN();
	bool CheckScrolling();
	void Input(int mouse_x, int mouse_y, bool mouse_click, int scroll_y);
	void Draw(class D3DGraphics *d3dg);
};

//! @brief ���̓{�b�N�X�̕����Ǘ��N���X
//! @details ���̓{�b�N�X�̕������͂��Ǘ����܂��B
//! @attention �`��@�\�͊܂܂�Ă��܂���B
class TextBoxObject
{
	int mode;							//!< ���̓��[�h
	char inputstr[MAX_INPUTBOXLEN+1];	//!< ���̓{�b�N�X�̕�����
	int maxlen;							//!< ���̓{�b�N�X�̍ő啶����

	void InputChar(char inchar);
	void DeleteChar();

public:
	TextBoxObject();
	~TextBoxObject();
	void InitTextBox(const char *str, int in_maxlen, int in_mode);
	void DestroyTextBox();
	void SetTextBoxStr(const char *str);
	const char* GetTextBoxStr();
	bool ProcessTextBox(InputControl *inputCtrl);
};

#endif