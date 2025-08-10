//! @file scene.h
//! @brief �e��ʂ��Ǘ�����N���X�̐錾

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

#ifndef SCENE_H
#define SCENE_H

//#include "main.h"
//	�w�b�_�[�t�@�C�����̉��Ő錾����B

//! @brief ��ʊǗ��N���X�i���N���X�j
//! @details �Q�[���̉�ʂ��Ǘ�����ł����̃N���X�ł��B
class scene
{
protected:
	class StateMachine *GameState;		//!< �Q�[���S�̂̏�ԑJ�ڃN���X
	class WindowControl *WindowCtrl;	//!< �E�B���h�E����N���X
	class D3DGraphics *d3dg;			//!< �`��N���X
	class InputControl *inputCtrl;		//!< ���͎擾�N���X
	unsigned int framecnt;				//!< �t���[�����̃J�E���g

public:
	scene();
	~scene();
	virtual void SetClass(StateMachine *in_GameState, WindowControl *in_WindowCtrl, D3DGraphics *in_d3dg, InputControl *in_inputCtrl);
	virtual int Create();
	virtual int Recovery();
	virtual void Input();
	virtual void Process();
	virtual bool RenderMain();
	virtual void Destroy();
	float GetEffectAlpha(unsigned int tframecnt, float MaxAlpha, float timingsec, float offsetsec, bool reversal);
	float GetEffectAlphaLoop(unsigned int tframecnt, float MaxAlpha, float timingsec, bool reversal);
};

//! @brief 2D��ʊǗ��N���X�i���N���X�j
//! @details �Q�[����ʂ̒��ŁA2D�`��݂̂��s���V�[�����Ǘ�������̃N���X�ł��B
class D2Dscene : public scene
{
protected:
	int gametitle;				//!< �Q�[���^�C�g���摜
	virtual void Render2D();

public:
	D2Dscene();
	~D2Dscene();
	virtual int Create();
	virtual int Recovery();
	virtual bool RenderMain();
	virtual void Destroy();
};

//! @brief 3D��ʊǗ��N���X�i���N���X�j
//! @details �Q�[����ʂ̒��ŁA3D�`����s���V�[�����Ǘ�������̃N���X�ł��B
class D3Dscene : public scene
{
protected:
	class SoundManager *GameSound;		//!< �Q�[�����ʉ��Ǘ��N���X
	float camera_x;			//!< �J�������W
	float camera_y;			//!< �J�������W
	float camera_z;			//!< �J�������W
	float camera_rx;		//!< �J������]�p�x
	float camera_ry;		//!< �J������]�p�x
	virtual void Render3D();
	virtual void Render2D();

public:
	D3Dscene();
	~D3Dscene();
	virtual void SetClass(StateMachine *in_GameState, WindowControl *in_WindowCtrl, D3DGraphics *in_d3dg, InputControl *in_inputCtrl, SoundManager *in_GameSound);
	virtual void Process();
	virtual void Sound();
	virtual bool RenderMain();
};

#ifndef H_LAYERLEVEL
 #define H_LAYERLEVEL 3		//!< Select include file.
#endif
#include "main.h"

#endif