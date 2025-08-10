//! @file d3dgraphics.h
//! @brief D3DGraphics�N���X�̐錾

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

// ***** OpenGL core only *****
//
// libjpeg
//    Copyright (C) 1991-2014, Thomas G. Lane, Guido Vollbeding.
//    this software is based in part on the work of the Independent JPEG Group
//
// zlib
//    Copyright (C) 1995-2013 Jean-loup Gailly and Mark Adler
//
// libpng
//    Copyright (c) 1998-2014 Glenn Randers-Pehrson
//
// ****************************

#ifndef D3DGRAPHICS_H
#define D3DGRAPHICS_H

#define MAX_MODEL 72		//!< �ő僂�f����
#define MAX_TEXTURE 72		//!< �ő�e�N�X�`����

#define CLIPPINGPLANE_NEAR 1.0f		//!< �߃N���b�v�ʁi���`�悷��ŏ������j
#define CLIPPINGPLANE_FAR 800.0f	//!< ���N���b�v�ʁi���`�悷��ő勗���j

#ifndef H_LAYERLEVEL
 #define H_LAYERLEVEL 1		//!< Select include file.
#endif
#include "main.h"

//! @brief �g�p����O���t�B�b�N�X�R�A�̑I��
//! @details DirectX 9.0c�i�Œ�p�C�v���C���j�F0�@DirectX 9.0c�i�V�F�[�_�[�j�F1�@OpenGL 1.1�F8
//! @attention DirectX 9.0c�̃V�F�[�_�[���g���ꍇ�A"resource.rc"���ҏW���Ă��������B
#define GRAPHIC_ENGINE 0

#if (GRAPHIC_ENGINE == 0)||(GRAPHIC_ENGINE == 1)

#define BLOCKDATA_GPUMEMORY	//!< @brief �u���b�N�f�[�^���i�[���郁�����[��I�� @details �萔�錾�L���FGPU�������[�@�萔�錾�����i�R�����g���j�F���C���������[

#pragma warning(disable:4819)		//VC++�x���h�~
#include <d3dx9.h>
#pragma warning(default:4819)

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#if GRAPHIC_ENGINE == 0
 #define GRAPHICS_CORE "DirectX 9.0"				//!< �o�[�W�����\���p���
#else
 #define GRAPHICS_CORE "DirectX 9.0 (Shader)"		//!< �o�[�W�����\���p���
 #define HLSLFILE 201								//!< ���\�[�X����HLSL�t�@�C��
#endif

#elif GRAPHIC_ENGINE == 8

//#define ENABLE_OPENGL_LIBJPEGPNG		//!< OpenGL�ɂ�����libjpeg�Elibpng���g�p����

#include <ctype.h>
#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#ifdef ENABLE_OPENGL_LIBJPEGPNG
 #include "lib/libjpeg/jpeglib.h"
 #include "lib/zlib/zlib.h"
 #include "lib/libpng/png.h"

 //��libjpeg.lib, zlib.lib, libpng.lib �͎��O�ŗp�ӂ���K�v������܂��B
 #pragma comment(lib, "lib/libjpeg/libjpeg.lib")
 #pragma comment(lib, "lib/zlib/zlib.lib")
 #pragma comment(lib, "lib/libpng/libpng.lib")

 //#pragma comment(lib, "legacy_stdio_definitions.lib")

 #pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
 #pragma comment(linker, "/NODEFAULTLIB:libcpmt.lib")
#endif

#define GRAPHICS_CORE "OpenGL 1.1"		//!< �o�[�W�����\���p���

#endif	//GRAPHIC_ENGINE

#if (GRAPHIC_ENGINE == 0)||(GRAPHIC_ENGINE == 1)

//! 3D�|���S���`��p�\����
typedef struct
{
	D3DXVECTOR3 position;	//!< The position
	DWORD       color;		//!< The color 
	FLOAT       tu;			//!< The texture coordinates
	FLOAT       tv;			//!< The texture coordinates
} VERTEXTXTA;

//! 2D�|���S���`��p�\����
typedef struct
{
	FLOAT    x;		//!< position
	FLOAT    y;		//!< position
	FLOAT    z;		//!< position
	FLOAT    rhw;	//!< vector
	D3DCOLOR color;	//!< color
	FLOAT    tu;	//!< texture coordinates
	FLOAT    tv;	//!< texture coordinates
} TLVERTX;

#elif GRAPHIC_ENGINE == 8

//! ���f���f�[�^�̒��_�i�[�\����
typedef struct
{
	float x;	//!< position
	float y;	//!< position
	float z;	//!< position
	float u;	//!< texture coordinates
	float v;	//!< texture coordinates
} MODELVDATA;

//! ���f���f�[�^�\����
typedef struct
{
	bool useflag;			//!< �L�����t���O
	int polygons;			//!< �|���S����
	float *VertexAry;		//!< ���_�i�[�z��
	float *ColorAry;		//!< �F�i�[�z��
	float *ColorGrayAry;	//!< �F�i�[�z��i�Â��j
	float *TexCoordAry;		//!< �e�N�X�`�����W�i�[�z��
} MODELDATA;

//! �e�N�X�`���\����
typedef struct
{
	bool useflag;			//!< �L�����t���O
	int width;				//!< ��
	int height;				//!< ����
	unsigned char *data;	//!< ���f�[�^
} TEXTUREDATA;

#endif	//GRAPHIC_ENGINE


//! @brief ��ʕ`����s���N���X
//! @details ��ʂ̕`��@�\�₻��ɒ��ڊ֘A���鏈�����s���܂��B
//! @details ��̓I�ɁA3D�`��E2D�`��E���f���t�@�C����e�N�X�`���Ǘ��@���s���@�\������܂��B
class D3DGraphics
{

#if (GRAPHIC_ENGINE == 0)||(GRAPHIC_ENGINE == 1)

	LPDIRECT3D9 pD3D;					//!< DIRECT3D9�̃|�C���^
	LPDIRECT3DDEVICE9 pd3dDevice;		//!< DIRECT3DDEVICE9�̃|�C���^
	float aspect;						//!< ��ʂ̃A�X�y�N�g��
	bool fullscreenflag;				//!< �t���X�N���[���\��
	int LanguageMode;					//!< ����I��
	LPD3DXMESH pmesh[MAX_MODEL];				//!< �iX�t�@�C���p�jD3DXMESH�̃|�C���^
	DWORD nummaterials[MAX_MODEL];				//!< �iX�t�@�C���p�j�}�e���A����
	LPDIRECT3DTEXTURE9 ptextures[MAX_TEXTURE];	//!< �e�N�X�`�����i�[

#if GRAPHIC_ENGINE == 1
	LPDIRECT3DVERTEXSHADER9	hVertexShader;			//!< ���_�V�F�[�_�[
	LPD3DXCONSTANTTABLE hVSConstantTable;			//!< ���_�V�F�[�_�[�̒萔�e�[�u��
	D3DXHANDLE hVSWorldVertexPos;					//!< ���_�V�F�[�_�[�̕ϊ��s��
	D3DXHANDLE hVSWorldViewPos;						//!< ���_�V�F�[�_�[�̕ϊ��s��
	D3DXHANDLE hVSWorldViewProj;					//!< ���_�V�F�[�_�[�̕ϊ��s��
	D3DXHANDLE hVSDiffuse;							//!< ���_�V�F�[�_�[�̃f�B�t���[�Y�F
	LPDIRECT3DPIXELSHADER9 pPixelShader;			//!< �s�N�Z���V�F�[�_�[
	LPD3DXCONSTANTTABLE pPSConstantTable;			//!< �s�N�Z���V�F�[�_�[�̒萔�e�[�u��
	D3DXHANDLE hPSViewPos;							//!< �s�N�Z���V�F�[�_�[�̃J�������W
	D3DXHANDLE hPSEnable2DRendering;				//!< �s�N�Z���V�F�[�_�[��2D�`��t���O
	D3DXHANDLE hPSEnableTexture;					//!< �s�N�Z���V�F�[�_�[�̃e�N�X�`���t���O
	D3DXHANDLE hPSEnableFog;						//!< �s�N�Z���V�F�[�_�[�̃t�H�O�L���t���O
	D3DXHANDLE hPSFogColor;							//!< �s�N�Z���V�F�[�_�[�̃t�H�O�F
	D3DXHANDLE hPSFogStart;							//!< �s�N�Z���V�F�[�_�[�̃t�H�O�̊J�n�[�x
	D3DXHANDLE hPSFogEnd;							//!< �s�N�Z���V�F�[�_�[�̃t�H�O�̏I���[�x
	D3DXMATRIX VSWorldPosMatrix;					//!< ���_�V�F�[�_�[�ɗ^�����ϊ��s��
	D3DXMATRIX VSViewPosMatrix;						//!< ���_�V�F�[�_�[�ɗ^�����ϊ��s��
	D3DXMATRIX VSViewProjMatrix;					//!< ���_�V�F�[�_�[�ɗ^�����ϊ��s��
#endif

	class BlockDataInterface* BlockDataIF;		//!< �ǂݍ��񂾃u���b�N�f�[�^���i�[����N���X�ւ̃|�C���^
	int bs;										//!< �u���b�N��
	int mapTextureID[TOTAL_BLOCKTEXTURE];		//!< �e�N�X�`���ԍ�
#ifdef BLOCKDATA_GPUMEMORY
	LPDIRECT3DVERTEXBUFFER9 g_pVB;				//!< ���_���iGPU�Ǘ��j
#else
	VERTEXTXTA g_pVertices[MAX_BLOCKS][6][4];	//!< ���_���iCPU�Ǘ��j
#endif

	float HUD_myweapon_x[TOTAL_HAVEWEAPON];		//!< HUD�̕���`�� X���W
	float HUD_myweapon_y[TOTAL_HAVEWEAPON];		//!< HUD�̕���`�� Y���W
	float HUD_myweapon_z[TOTAL_HAVEWEAPON];		//!< HUD�̕���`�� Z���W

	bool StartRenderFlag;			//!< StartRender()�֐� ���s����\���t���O
	bool EnableFogFlag;				//!< �t�H�O�g�p����\���t���O

	LPD3DXSPRITE ptextsprite;			//!< 2D�e�N�X�`���X�v���C�g
	LPD3DXFONT pxmsfont;				//!< �V�X�e���t�H���g
	LPD3DXFONT pxmssmallfont;			//!< �V�X�e���t�H���g�i�������j
	char TextureFontFname[_MAX_PATH];	//!< �e�N�X�`���t�H���g�̃t�@�C����
	int TextureFont;					//!< �e�N�X�`���t�H���g�̃e�N�X�`��ID

	int InitSubset();
	void CleanupD3Dresource();
	void Start2DMSFontTextRender();
	void End2DMSFontTextRender();
	void Start2DRender();
	void End2DRender();

#elif GRAPHIC_ENGINE == 8

	HWND hWnd;				//!< �E�B���h�E�n���h��
	HGLRC hGLRC;			//!< OpenGL�̃R���e�L�X�g
	int width;				//!< ��
	int height;				//!< ����
	bool fullscreenflag;	//!< �t���X�N���[���\��
	int LanguageMode;		//!< ����I��
	PAINTSTRUCT Paint_ps;					//!< BeginPaint()�֐���EndPaint()�֐��p
	MODELDATA pmodel[MAX_MODEL];			//!< ���f���f�[�^���i�[
	TEXTUREDATA ptextures[MAX_TEXTURE];		//!< �e�N�X�`�����i�[
	GLuint textureobjname[MAX_TEXTURE];		//!< �e�N�X�`���I�u�W�F�N�g
	HFONT SystemFont;				//!< �V�X�e���t�H���g�p�̘_���t�H���g
	HFONT SystemSmallFont;			//!< �V�X�e���t�H���g�p�̘_���t�H���g�i�������j
	WCHAR *now_SystemFontUStr;		//!< ���ݕ\�����̃V�X�e���t�H���g�ɂ�镶����iUnicode�j
	GLuint SystemFontListIdx;		//!< �V�X�e���t�H���g�̃f�B�X�v���C���X�g
	int SystemFontListIdxSize;		//!< �V�X�e���t�H���g�̃f�B�X�v���C���X�g�̃T�C�Y
	float SystemFont_posz;			//!< �V�X�e���t�H���g��Z���W
	int now_textureid;		//!< ���ݐݒ蒆�̃e�N�X�`���ԍ�

	float camera_x;			//!< �J�������W
	float camera_y;			//!< �J�������W
	float camera_z;			//!< �J�������W
	float camera_rx;		//!< �J������]�p�x
	float camera_ry;		//!< �J������]�p�x
	float viewangle;		//!< �J�����̎���p

	class BlockDataInterface* BlockDataIF;		//!< �ǂݍ��񂾃u���b�N�f�[�^���i�[����N���X�ւ̃|�C���^
	int bs;										//!< �u���b�N��
	int mapTextureID[TOTAL_BLOCKTEXTURE];		//!< �e�N�X�`���ԍ�

	char TextureFontFname[_MAX_PATH];	//!< �e�N�X�`���t�H���g�̃t�@�C����
	int TextureFont;				//!< �e�N�X�`���t�H���g�̃e�N�X�`��ID

	float HUD_myweapon_x[TOTAL_HAVEWEAPON];		//!< HUD_A�̕���`�� X���W
	float HUD_myweapon_y[TOTAL_HAVEWEAPON];		//!< HUD_A�̕���`�� Y���W
	float HUD_myweapon_z[TOTAL_HAVEWEAPON];		//!< HUD_A�̕���`�� Z���W

#ifdef ENABLE_OPENGL_LIBJPEGPNG
	jpeg_decompress_struct cinfo;	//!< libjpeg
	jpeg_error_mgr jerr;			//!< libjpeg
#endif

	void CleanupD3Dresource();
	int CheckTextureFileExtension(const char* filename, int nowformat);
	int CheckTextureFileTypeFlag(const char* filename, int nowformat);
	bool LoadBMPTexture(const char* filename, bool BlackTransparent, TEXTUREDATA *ptexture);
	bool LoadDDSTexture(const char* filename, bool BlackTransparent, TEXTUREDATA *ptexture);
	bool LoadJPEGTexture(const char* filename, bool BlackTransparent, TEXTUREDATA *ptexture);
	bool LoadPNGTexture(const char* filename, bool BlackTransparent, TEXTUREDATA *ptexture);
	void SetTexture(int TextureID);
	int StrMaxLineLen(const char *str);
	void Start2DRender();
	void End2DRender();

#endif	//GRAPHIC_ENGINE

#ifdef ENABLE_DEBUGCONSOLE
	unsigned char DebugFontData[96][16];	//!< �f�o�b�N�p�t�H���g�f�[�^�i�[�z��
	int TextureDebugFont;					//!< �f�o�b�N�p�t�H���g�̃e�N�X�`��ID
	void InitDebugFontData();
	bool LoadDebugFontTexture();
#endif

public:
	D3DGraphics();
	~D3DGraphics();
	void SetFullScreenFlag(bool fullscreen);
	bool GetFullScreenFlag();
	void SetlanguageMode(int mode);
	int GetlanguageMode();
	int InitD3D(WindowControl *WindowCtrl, const char *TextureFontFilename, bool fullscreen, int language);
	int ResetD3D(WindowControl *WindowCtrl);
	void DestroyD3D();
	int LoadModel(const char* filename);
	int MorphingModel(int idA, int idB);
	int GetTotalModels();
	void CleanupModel(int id);
	int LoadTexture(const char* filename, bool texturefont, bool BlackTransparent);
	int GetTextureSize(int id, int *width, int *height);
	int GetTotalTextures();
	void CleanupTexture(int id);
	int StartRender();
	bool EndRender();
	void ResetZbuffer();
	void ResetWorldTransform();
	void SetWorldTransform(float x, float y, float z, float rx, float ry, float size);
	void SetWorldTransform(float x, float y, float z, float rx, float ry1, float ry2, float size);
	void SetWorldTransformEffect(float x, float y, float z, float rx, float ry, float rt, float size);
	void SetWorldTransformHumanWeapon(float x, float y, float z, float mx, float my, float mz, float rx, float ry, float size);
	void SetWorldTransformPlayerWeapon(bool rotation, float camera_rx, float camera_ry, float rx, float size);
	void GetWorldTransformPos(float *x, float *y, float *z);
	void SetFog(bool enablefog, int skynumber);
	void SetCamera(float camera_x, float camera_y, float camera_z, float camera_rx, float camera_ry, float viewangle);
	void LoadMapdata(class BlockDataInterface* in_BlockDataIF, const char *directory);
	void RenderMapdata(bool wireframe);
	int GetMapTextureID(int id);
	void CleanupMapdata();
	void RenderModel(int id_model, int id_texture, bool darkflag, bool HUDmode, bool nomodel);
	void RenderBoard(int id_texture, float alpha, bool nomodel);
	void StartEffectRender();
	void EndEffectRender();
	void ScreenBrightness(int Width, int Height, int Brightness);
	void RenderCenterline();
	void Renderline(float x1, float y1, float z1, float x2, float y2, float z2, int color);
	void Draw2DMSFontText(int x, int y, const char *str, int color);
	void Draw2DMSFontTextScaling(int x, int y, const char *str, int color);
	void Draw2DMSFontTextCenter(int x, int y, int w, int h, const char *str, int color);
	void Draw2DMSFontTextCenterScaling(int x, int y, int w, int h, const char *str, int color);
	void Draw2DMSSmallFontText(int x, int y, const char *str, int color);
	void GetFontTextSize(const char *str, int base_w, int base_h, int limitlen, int *w, int *h);
	void Draw2DTextureFontText(int x, int y, const char *str, int color, int fontwidth, int fontheight);
	void Draw2DTextureFontTextScaling(int x, int y, const char *str, int color, int fontwidth, int fontheight);
	void Draw2DTextureFontTextCenter(int x, int y, const char *str, int color, int fontwidth, int fontheight);
	void Draw2DTextureFontTextCenterScaling(int x, int y, const char *str, int color, int fontwidth, int fontheight);
#ifdef ENABLE_DEBUGCONSOLE
	void Draw2DTextureDebugFontText(int x, int y, const char *str, int color);
#endif
	void Draw2DLine(int x1, int y1, int x2, int y2, int color);
	void Draw2DCycle(int x, int y, int r, int color);
	void Draw2DBox(int x1, int y1, int x2, int y2, int color);
	void Draw2DBoxScaling(int x1, int y1, int x2, int y2, int color);
	void Draw2DTexture(int x, int y, int id, int width, int height, float alpha);
	void Draw2DTextureScaling(int x, int y, int id, int width, int height, float alpha);
	bool SaveScreenShot(const char *fname);
	int GetColorCode(float red, float green, float blue, float alpha);
};

#endif