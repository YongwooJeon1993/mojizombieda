//=============================================================================
//
// �^�C�g����ʏ��� [Tutorial.cpp]
// Author : 
//
//=============================================================================
#include "tutorial.h"
#include "input.h"
#include "sound.h"
#include "main.h"
#include "light.h"
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXTURE_WIDTH				(SCREEN_WIDTH)	// �w�i�T�C�Y
#define TEXTURE_HEIGHT				(SCREEN_HEIGHT)	// 
#define TEXTURE_MAX					(2)				// �e�N�X�`���̐�

#define TEXTURE_WIDTH_LOGO			(480)			// ���S�T�C�Y
#define TEXTURE_HEIGHT_LOGO			(80)			// 

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static ID3D11Buffer* g_VertexBuffer = NULL;				// ���_���
static ID3D11ShaderResourceView* g_Texture[TEXTURE_MAX] = { NULL };	// �e�N�X�`�����

static char* g_TexturName[TEXTURE_MAX] = {
	"data/TEXTURE/tutorial.png",
	"data/TEXTURE/effect000.jpg",
};

static BOOL						g_Use;						// TRUE:�g���Ă���  FALSE:���g�p
static float					g_w, g_h;					// ���ƍ���
static XMFLOAT3					g_Pos;						// �|���S���̍��W
static int						g_TexNo;					// �e�N�X�`���ԍ�

float	alphat;
BOOL	flag_alphat;

static BOOL						g_Load = FALSE;

static float	effect_dx;
static float	effect_dy;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitTutorial(void)
{
	ID3D11Device* pDevice = GetDevice();

	//�e�N�X�`������
	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		g_Texture[i] = NULL;
		D3DX11CreateShaderResourceViewFromFile(GetDevice(),
			g_TexturName[i],
			NULL,
			NULL,
			&g_Texture[i],
			NULL);
	}


	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);


	// �ϐ��̏�����
	g_Use = TRUE;
	g_w = TEXTURE_WIDTH;
	g_h = TEXTURE_HEIGHT;
	g_Pos = XMFLOAT3(g_w / 2, g_h / 2, 0.0f);
	g_TexNo = 0;

	alphat = 1.0f;
	flag_alphat = TRUE;

	effect_dx = 100.0f;
	effect_dy = 100.0f;

	g_Load = TRUE;
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitTutorial(void)
{
	if (g_Load == FALSE) return;

	if (g_VertexBuffer)
	{
		g_VertexBuffer->Release();
		g_VertexBuffer = NULL;
	}

	for (int i = 0; i < TEXTURE_MAX; i++)
	{
		if (g_Texture[i])
		{
			g_Texture[i]->Release();
			g_Texture[i] = NULL;
		}
	}

	g_Load = FALSE;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTutorial(void)
{
	if (GetKeyboardTrigger(DIK_RETURN))
	{// Enter��������A�X�e�[�W��؂�ւ���
		SetFade(FADE_OUT, MODE_GAME);
	}
	// �Q�[���p�b�h�œ��͏���
	else if (IsButtonTriggered(0, BUTTON_START))
	{
		SetMode(MODE_GAME);
	}
	else if (IsButtonTriggered(0, BUTTON_B))
	{
		SetMode(MODE_GAME);
	}



	// �Z�[�u�f�[�^�����[�h����H
	if (GetKeyboardTrigger(DIK_L))
	{

	}


	// �e�X�g�ŃG�t�F�N�g�̔����ꏊ���ړ�������
	float speed = 4.0f;

	if (GetKeyboardPress(DIK_DOWN))
	{
		effect_dy += speed;
	}
	else if (GetKeyboardPress(DIK_UP))
	{
		effect_dy -= speed;
	}

	if (GetKeyboardPress(DIK_RIGHT))
	{
		effect_dx += speed;
	}
	else if (GetKeyboardPress(DIK_LEFT))
	{
		effect_dx -= speed;
	}


#ifdef _DEBUG	// �f�o�b�O����\������
	//PrintDebugProc("Player:�� �� �� ���@Space\n");

#endif

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTutorial(void)
{
	// 2D�̕���`�悷�鏈��
	// Z��r�Ȃ�
	SetDepthEnable(false);

	// ���C�e�B���O�𖳌�
	SSetLightEnable(false);

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	GetDeviceContext()->IASetVertexBuffers(0, 1, &g_VertexBuffer, &stride, &offset);

	// �}�g���N�X�ݒ�
	SetWorldViewProjection2D();

	// �v���~�e�B�u�g�|���W�ݒ�
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// �}�e���A���ݒ�
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	// �^�C�g���̔w�i��`��
	{
		// �e�N�X�`���ݒ�
		GetDeviceContext()->PSSetShaderResources(0, 1, &g_Texture[0]);

		// �P���̃|���S���̒��_�ƃe�N�X�`�����W��ݒ�
		SetSpriteLeftTop(g_VertexBuffer, 0.0f, 0.0f, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0.0f, 0.0f, 1.0f, 1.0f);

		// �|���S���`��
		GetDeviceContext()->Draw(4, 0);
	}


	// �����Z�̃e�X�g
	SetBlendState(BLEND_MODE_ADD);		// ���Z����
//	SetBlendState(BLEND_MODE_SUBTRACT);	// ���Z����

	SetBlendState(BLEND_MODE_ALPHABLEND);	// ���������������ɖ߂�

	// ���C�e�B���O��L����
	SSetLightEnable(true);

	// Z��r����
	SetDepthEnable(true);
}


