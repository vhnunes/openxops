//! @file scene.cpp
//! @brief �e��ʂ��Ǘ�����N���X�̒�`

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

#include "scene.h"

//! @brief �R���X�g���N�^
scene::scene()
{
	GameState = NULL;
	WindowCtrl = NULL; 
	d3dg = NULL;
	inputCtrl = NULL;
	framecnt = 0;
}

//! @brief �f�B�X�g���N�^
scene::~scene()
{}

//! @brief �N���X��ݒ�
void scene::SetClass(StateMachine *in_GameState, WindowControl *in_WindowCtrl, D3DGraphics *in_d3dg, InputControl *in_inputCtrl)
{
	GameState = in_GameState;
	WindowCtrl = in_WindowCtrl; 
	d3dg = in_d3dg;
	inputCtrl = in_inputCtrl;
}

//! @brief �V�[���쐬
int scene::Create()
{
	inputCtrl->MoveMouseCenter();
	framecnt = 0;

	GameState->NextState();
	return 0;
}

//! @brief �V�[���`���
int scene::Recovery()
{
	return 0;
}

//! @brief �V�[�����͏���
void scene::Input()
{
	inputCtrl->GetInputState(false);

	if( inputCtrl->CheckKeyDown(GetEscKeycode()) ){
		GameState->PushBackSpaceKey();
	}
	else if( inputCtrl->CheckMouseButtonUpL() ){
		GameState->PushMouseButton();
	}
}

//! @brief �V�[���v�Z����
void scene::Process()
{
	framecnt += 1;
}

//! @brief �V�[���`�揈��
//! @return �����Ffalse�@���s�Ftrue
//! @attention ���s�����ꍇ�AD3DGraphics�N���X��ResetD3D()�֐����Ăяo���Ă��������B
bool scene::RenderMain()
{
	if( d3dg->StartRender() ){ return true; }

	d3dg->Draw2DBox(0, 0, GameConfig.GetScreenWidth(), GameConfig.GetScreenHeight(), d3dg->GetColorCode(0.0f,0.0f,0.0f,1.0f));
	d3dg->Draw2DTextureFontText(10, 10, "hello world !", d3dg->GetColorCode(1.0f,1.0f,1.0f,1.0f), 20, 32);

	d3dg->ScreenBrightness(GameConfig.GetScreenWidth(), GameConfig.GetScreenHeight(), GameConfig.GetBrightness());

	return d3dg->EndRender();
}

//! @brief �V�[���������
void scene::Destroy()
{
	GameState->NextState();
}

//! @brief �t���[���J�E���g�����ɃA���t�@�l�E�����x��ݒ�i1��j
//! @param tframecnt �ΏۂƂ���J�E���g�l
//! @param MaxAlpha �ő�A���t�@�l
//! @param timingsec �Ԋu�E�b��
//! @param offsetsec �I�t�Z�b�g�E�b��
//! @param reversal ���]�t���O�ifalse�F�t�B�[���h�C���@true�F�t�B�[���h�A�E�g�j
//! @return �A���t�@�l
float scene::GetEffectAlpha(unsigned int tframecnt, float MaxAlpha, float timingsec, float offsetsec, bool reversal)
{
	float alpha;
	unsigned int frametiming;
	unsigned int frameoffset;

	frametiming = (unsigned int)(timingsec*GAMEFPS);
	frameoffset = (unsigned int)(offsetsec*GAMEFPS);
	alpha = MaxAlpha/frametiming * (tframecnt - frameoffset);

	if( alpha < 0.0f ){ alpha = 0.0f; }
	if( alpha > 1.0f ){ alpha = 1.0f; }

	if( reversal == true ){
		return 1.0f - alpha;
	}
	return alpha;
}

//! @brief �t���[���J�E���g�����ɃA���t�@�l�E�����x��ݒ�i���[�v�j
//! @param tframecnt �ΏۂƂ���J�E���g�l
//! @param MaxAlpha �ő�A���t�@�l
//! @param timingsec �Ԋu�E�b��
//! @param reversal ���]�t���O�ifalse�F�t�B�[���h�C���@true�F�t�B�[���h�A�E�g�j
//! @return �A���t�@�l
float scene::GetEffectAlphaLoop(unsigned int tframecnt, float MaxAlpha, float timingsec, bool reversal)
{
	float alpha;
	unsigned int frametiming;

	frametiming = (unsigned int)(timingsec*GAMEFPS);
	alpha = MaxAlpha/frametiming * (tframecnt%frametiming);

	if( reversal == true ){
		return 1.0f - alpha;
	}
	return alpha;
}

//! @brief �R���X�g���N�^
D2Dscene::D2Dscene()
{
	gametitle = -1;
}

//! @brief �f�B�X�g���N�^
D2Dscene::~D2Dscene()
{}

//! @brief 2D�V�[��������
int D2Dscene::Create()
{
	gametitle = d3dg->LoadTexture("data\\title.dds", false, false);
	GameState->NextState();
	if( gametitle == -1 ){ return 1; }
	return 0;
}

//! @brief �V�[���`���
int D2Dscene::Recovery()
{
	//������
	gametitle = d3dg->LoadTexture("data\\title.dds", false, false);
	if( gametitle == -1 ){ return 1; }
	return 0;
}

//! @brief 2D�V�[���`�揈���i2D�j
void D2Dscene::Render2D()
{
	d3dg->Draw2DTextureFontText(10, 10, "hello world !", d3dg->GetColorCode(1.0f,1.0f,1.0f,1.0f), 20, 32);
}

//! @brief 2D�V�[���`�揈���i���C���j
//! @return �����Ffalse�@���s�Ftrue
//! @attention ���s�����ꍇ�AD3DGraphics�N���X��ResetD3D()�֐����Ăяo���Ă��������B
bool D2Dscene::RenderMain()
{
	if( d3dg->StartRender() ){ return true; }

	d3dg->Draw2DTexture(0, 0, gametitle, GameConfig.GetScreenWidth(), GameConfig.GetScreenHeight(), 0.4f);
	d3dg->Draw2DBox(11, 11, GameConfig.GetScreenWidth() - 10, GameConfig.GetScreenHeight() - 10, d3dg->GetColorCode(0.0f,0.0f,0.0f,0.75f));

	Render2D();

	d3dg->ScreenBrightness(GameConfig.GetScreenWidth(), GameConfig.GetScreenHeight(), GameConfig.GetBrightness());

	return d3dg->EndRender();
}

//! @brief 2D�V�[�����
void D2Dscene::Destroy()
{
	d3dg->CleanupTexture(gametitle);
	GameState->NextState();
}


//! @brief �R���X�g���N�^
D3Dscene::D3Dscene()
{
	GameSound = NULL;
	camera_x = 0.0f;
	camera_y = 0.0f;
	camera_z = 0.0f;
	camera_rx = 0.0f;
	camera_ry = 0.0f;
}

//! @brief �f�B�X�g���N�^
D3Dscene::~D3Dscene()
{}

//! @brief 3D�V�[���N���X��ݒ�
void D3Dscene::SetClass(StateMachine *in_GameState, WindowControl *in_WindowCtrl, D3DGraphics *in_d3dg, InputControl *in_inputCtrl, SoundManager *in_GameSound)
{
	GameState = in_GameState;
	WindowCtrl = in_WindowCtrl; 
	d3dg = in_d3dg;
	inputCtrl = in_inputCtrl;
	GameSound = in_GameSound;
}

//! @brief 3D�V�[����v�Z����
void D3Dscene::Process()
{}

//! @brief 3D�V�[���T�E���h�Đ�����
void D3Dscene::Sound()
{
	//�T�E���h���Đ�
	GameSound->PlayWorldSound(camera_x, camera_y, camera_z, camera_rx, 1);
}

//! @brief 3D�V�[���`�揈���i3D�j
void D3Dscene::Render3D()
{}

//! @brief 3D�V�[���`�揈���i2D�j
void D3Dscene::Render2D()
{}

//! @brief 3D�V�[���`�揈���i���C���j
//! @return �����Ffalse�@���s�Ftrue
//! @attention ���s�����ꍇ�AD3DGraphics�N���X��ResetD3D()�֐����Ăяo���Ă��������B
bool D3Dscene::RenderMain()
{
	if( d3dg->StartRender() ){ return true; }

	Render3D();
	Render2D();

	d3dg->ScreenBrightness(GameConfig.GetScreenWidth(), GameConfig.GetScreenHeight(), GameConfig.GetBrightness());

	return d3dg->EndRender();
}