//=============================================================================
//
//カメラ処理 [camera.cpp]
//Date:2023/10/29	Auther:林祐也
//=============================================================================
#include "memleakcheck.h" // メモリリークチェック用のヘッダーを最初にインクルード

#ifdef _DEBUG
#pragma push_macro("new")
#undef new
#endif

#include "main.h" // DirectX のヘッダーが含まれている

#ifdef _DEBUG
#pragma pop_macro("new")
#endif

#include "input.h"
#include "camera.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	POS_X_CAM		(0.0f)			// カメラの初期位置(X座標)
#define	POS_Y_CAM		(1200.0f)		// カメラの初期位置(Y座標)
#define	POS_Z_CAM		(-600.0f)		// カメラの初期位置(Z座標)
#define	HEIGHT_CAM_OVERLOOK	(1300.0f)	// 神視点時のカメラの高さ

#define	VIEW_ANGLE		(D3DXToRadian(45.0f))							// ビュー平面の視野角
#define	VIEW_ASPECT		((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// ビュー平面のアスペクト比	
#define	VIEW_NEAR_Z		(100.0f)										// ビュー平面のNearZ値
#define	VIEW_FAR_Z		(10000.0f)										// ビュー平面のFarZ値

#define VIEW_NEAR_OVERLOOK (50.0f)//900

#define	VALUE_MOVE_CAMERA	(20.0f)										// カメラの移動量
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)							// カメラの回転量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
static CAMERA			g_Camera;		// カメラデータ

static D3DXVECTOR2 oldMousePosCa = { 0.0f,0.0f };
static D3DXVECTOR2 mousePosCa = { 0.0f,0.0f };
// ディスプレイのサイズを取得
static int screenWidthCa = GetSystemMetrics(SM_CXSCREEN);
static int screenHeightCa = GetSystemMetrics(SM_CYSCREEN);

// 画面の中央座標を計算
static int centerXCa = screenWidthCa / 2;
static int centerYCa = screenHeightCa / 2;

static int cameraCount = 0;
static int possessionCnt = 0;
static int possessionBlackoutCnt = 0;
static int possessionLightChangeCnt = 0;

bool possessionFlag = false;		//憑依の選択
bool possessionFadeInFlag = false;
bool possessionBlackoutFlag = false;
bool possessionLightChangeFlag = false;
static float cameraPossessionVel=0;

static int camera_sensi = SENS_MIDDLE;

//=============================================================================
// 初期化処理
//=============================================================================
void InitCamera(void)
{
	g_Camera.pos = D3DXVECTOR3(POS_X_CAM, POS_Y_CAM, POS_Z_CAM);
	g_Camera.at = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.up = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Camera.state = STATE_OVERLOOK;

	// 視点と注視点の距離を計算
	float vx, vz;
	vx = g_Camera.pos.x - g_Camera.at.x;
	vz = g_Camera.pos.z - g_Camera.at.z;
	g_Camera.len = sqrtf(vx * vx + vz * vz);
	
}


//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{

}


//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{
	cameraCount++;

	//if (GetKeyboardPress(DIK_Z))
	//{// 視点旋回「左」
	//	g_Camera.rot.y += VALUE_ROTATE_CAMERA;
	//	if (g_Camera.rot.y > D3DX_PI)
	//	{
	//		g_Camera.rot.y -= D3DX_PI * 2.0f;
	//	}

	//	g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
	//	g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
	//}

	//if (GetKeyboardPress(DIK_C))
	//{// 視点旋回「右」
	//	g_Camera.rot.y -= VALUE_ROTATE_CAMERA;
	//	if (g_Camera.rot.y < -D3DX_PI)
	//	{
	//		g_Camera.rot.y += D3DX_PI * 2.0f;
	//	}

	//	g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
	//	g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
	//}

	//if (GetKeyboardPress(DIK_Y))
	//{// 視点移動「上」
	//	g_Camera.pos.y += VALUE_MOVE_CAMERA;
	//}

	//if (GetKeyboardPress(DIK_N))
	//{// 視点移動「下」
	//	g_Camera.pos.y -= VALUE_MOVE_CAMERA;
	//}

	////if (GetKeyboardPress(DIK_Q))
	////{// 注視点旋回「左」
	////	g_Camera.rot.y -= VALUE_ROTATE_CAMERA;
	////	if (g_Camera.rot.y < -D3DX_PI)
	////	{
	////		g_Camera.rot.y += D3DX_PI * 2.0f;
	////	}

	////	g_Camera.at.x = g_Camera.pos.x + sinf(g_Camera.rot.y) * g_Camera.len;
	////	g_Camera.at.z = g_Camera.pos.z + cosf(g_Camera.rot.y) * g_Camera.len;
	////}

	////if (GetKeyboardPress(DIK_E))
	////{// 注視点旋回「右」
	////	g_Camera.rot.y += VALUE_ROTATE_CAMERA;
	////	if (g_Camera.rot.y > D3DX_PI)
	////	{
	////		g_Camera.rot.y -= D3DX_PI * 2.0f;
	////	}

	////	g_Camera.at.x = g_Camera.pos.x + sinf(g_Camera.rot.y) * g_Camera.len;
	////	g_Camera.at.z = g_Camera.pos.z + cosf(g_Camera.rot.y) * g_Camera.len;
	////}

	//if (GetKeyboardPress(DIK_T))
	//{// 注視点移動「上」
	//	g_Camera.at.y += VALUE_MOVE_CAMERA;
	//}

	//if (GetKeyboardPress(DIK_B))
	//{// 注視点移動「下」
	//	g_Camera.at.y -= VALUE_MOVE_CAMERA;
	//}

	//if (GetKeyboardPress(DIK_U))
	//{// 近づく
	//	g_Camera.len -= VALUE_MOVE_CAMERA;
	//	g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
	//	g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
	//}

	//if (GetKeyboardPress(DIK_M))
	//{// 離れる
	//	g_Camera.len += VALUE_MOVE_CAMERA;
	//	g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
	//	g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
	//}

	//// カメラを初期に戻す
	//if (GetKeyboardPress(DIK_P))
	//{
	//	UninitCamera();
	//	InitCamera();
	//}


}


//=============================================================================
// カメラの更新
//=============================================================================
void SetCamera(void) 
{
	// ビューマトリックス設定
	D3DXMatrixLookAtLH(&g_Camera.mtxView, &g_Camera.pos, &g_Camera.at, &g_Camera.up);

	SetViewMatrix(&g_Camera.mtxView);


	float det;
	D3DXMatrixInverse(&g_Camera.mtxInvView, &det, &g_Camera.mtxView);


	// プロジェクションマトリックス設定(投影方法の切り替え)
	if (GetCameraState() == STATE_OVERLOOK) 
	{
		// 平行投影
		D3DXMatrixOrthoLH(&g_Camera.mtxProjection, GetWindowSize().x * WINDOW_SCALE, GetWindowSize().y * WINDOW_SCALE, VIEW_NEAR_OVERLOOK, VIEW_FAR_Z);
	}
	else if(GetCameraState() == STATE_TPS)
	{
		// 透視投影
		D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection, 1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, VIEW_NEAR_Z, VIEW_FAR_Z);
	}

	SetProjectionMatrix(&g_Camera.mtxProjection);
}


//=============================================================================
// カメラの取得
//=============================================================================
CAMERA *GetCamera(void) 
{
	return &g_Camera;
}

// カメラの視点状態の取得
CAMERA_STATE GetCameraState(void)
{
	return g_Camera.state;
}

//カメラの憑依演出状態の取得
bool GetCameraPossession(void)
{
	return possessionFlag;
}
//憑依として近づいていく時の演出
bool GetCameraPossessionFadeInFlag(void)
{
	return possessionFadeInFlag;
}
//暗転が始まったフラグ
bool GetCameraPossessionBlackout(void) 
{
	return possessionBlackoutFlag;
}
//明転が始まったフラグ
bool GetCameraPossessionLightChange(void) 
{
	return possessionLightChangeFlag;
}
//カメラの視点と注視点をセット
//カメラがターゲットに追尾
void SetCameraAT(D3DXVECTOR3 pos)
{
	// カメラの注視点をプレイヤーの座標にしてみる
	g_Camera.at = pos;

	// カメラの視点をカメラのY軸回転に対応させている
	g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
	g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
}
// カメラの視点と注視点と向きをセット
// カメラを設定したオブジェクトの正面を向き続けるように
void SetCameraDT(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// カメラの注視点をプレイヤーの座標にしてみる
	g_Camera.at = pos;
	g_Camera.rot = rot;
	// カメラの視点をカメラのY軸回転に対応させている
	g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
	g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;

}
//--------------------------------------------------
//TPSで使うカメラ
//プレイヤーの回転に応じて変かわるカメラで使用
// Date:2023/11/27	Auther:林祐也
//--------------------------------------------------
void SetCameraTPS(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	D3DXMATRIX rotmatrix;
	D3DXVECTOR3 Dir;
	D3DXMatrixRotationYawPitchRoll(&rotmatrix, rot.y, rot.x, rot.z);
	Dir.x = rotmatrix._11;
	Dir.y = rotmatrix._12;
	Dir.z = rotmatrix._13;

	g_Camera.pos = pos;
	g_Camera.at = pos + Dir * g_Camera.len;
	//// カメラの注視点をプレイヤーの座標にしてみる
	//g_Camera.at = pos;
	////回転もプレイヤー参照
	//g_Camera.rot = rot;
	//// カメラの視点をカメラのY軸回転に対応させている
	//g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
	//g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;

	//// カメラの視点をカメラのX軸回転に対応させている
	////プレイヤーの回転値によってsinとcosの-+が変わる/変えるならプレイヤーの方を変えるのを推奨
	//g_Camera.pos.y = g_Camera.at.y - (sinf(g_Camera.rot.x) + cosf(g_Camera.rot.z)) * g_Camera.len + POS_Y_CAM/2;
}

//--------------------------------------------------
//TPSで使うカメラ
//プレイヤーの回転に左右されないカメラ
//対象にしたいオブジェクトの座標を入力すればOK
// Date:2023/11/27	Auther:林祐也
//--------------------------------------------------
void SetCameraFreeLookTPS(D3DXVECTOR3 pos)
{
	if (!possessionFlag)
	{
		//マウスの過去座標を格納
		oldMousePosCa = mousePosCa;
		//マウスの座標を格納
		mousePosCa = D3DXVECTOR2(GetMousePosX(), GetMousePosY());

		//中央から右にいる場合
		if (oldMousePosCa.x > centerXCa)
		{
			//元に戻る時は反応しないように
			if (mousePosCa.x > oldMousePosCa.x)
			{
				g_Camera.rot.y -= (oldMousePosCa.x - mousePosCa.x) / camera_sensi;
			}
		}
		//中央から左にいる場合
		if (oldMousePosCa.x < centerXCa)
		{
			//元に戻る時は反応しないように
			if (mousePosCa.x < oldMousePosCa.x)
			{
				g_Camera.rot.y -= (oldMousePosCa.x - mousePosCa.x) / camera_sensi;
			}
		}
		//中央から下にいる場合
		if (oldMousePosCa.y > centerYCa)
		{
			//元に戻る時は反応しないように
			if (mousePosCa.y > oldMousePosCa.y)
			{
				g_Camera.rot.x += (oldMousePosCa.y - mousePosCa.y) / camera_sensi;
			}
		}
		//中央から上にいる場合
		if (oldMousePosCa.y < centerYCa)
		{
			//元に戻る時は反応しないように
			if (mousePosCa.y < oldMousePosCa.y)
			{
				g_Camera.rot.x += (oldMousePosCa.y - mousePosCa.y) / camera_sensi;
			}
		}
		//回転Zは処理で使ってないので本来は設定しなくていいがバグの防止のために配置
		//回転Zの上限値設定
		if (g_Camera.rot.z >= 3.14f / 2) {
			g_Camera.rot.z = (3.14f);
		}
		//回転Zの下限値設定
		if (g_Camera.rot.z <= -3.14f / 2) {
			g_Camera.rot.z = (-3.14f);
		}
		//回転Xの下限値設定
		if (g_Camera.rot.x >= 0.23f) {
			g_Camera.rot.x = (0.23f);
		}
		//回転Xの下限値設定
		//if (g_Camera.rot.x >= -0.13f) {
		//	g_Camera.rot.x = (-0.13f);
		//}
		//回転Xの上限値設定
		if (g_Camera.rot.x <= -3.14f / 2) {
			g_Camera.rot.x = (-3.14f / 2);
		}

		// カメラの注視点をプレイヤーの座標にしてみる
		g_Camera.at = pos + D3DXVECTOR3(0,g_Camera.len * 0.3f,0);
		// カメラの視点をカメラのY軸回転に対応させている
		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
		//// カメラの視点をカメラのX軸回転に対応させている
		g_Camera.pos.y = g_Camera.at.y - (sinf(g_Camera.rot.x) + cosf(g_Camera.rot.z)) * g_Camera.len + POS_Y_CAM / 2;

		//バグらないため
		if (cameraCount > 3) {
			// マウスカーソルを中央に移動させる
			SetCursorPos(centerXCa, centerYCa);
			cameraCount = 0;
		}
	

	}
}

//--------------------------------------------------
//神視点(俯瞰)時に使うカメラ
// Date:2023/11/27	Auther:野村悠人
//--------------------------------------------------
void SetCameraOverLook(D3DXVECTOR3 pos) 
{
	// 俯瞰時のポジションを引数のposの真上にセット
	g_Camera.pos = D3DXVECTOR3(pos.x, HEIGHT_CAM_OVERLOOK, pos.z);

}

//--------------------------------------------------
//カメラの状態の変更
// Date:2023/11/27	Auther:野村悠人
//--------------------------------------------------
void ChangeCameraState(void) 
{
	if (g_Camera.state == STATE_TPS) 
	{
		PlaySound(GetSoundID(SE_HYOUI_CANCEL), 0);
		g_Camera.state = STATE_OVERLOOK;

		// カメラの頭を右に設定
		g_Camera.up = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);

		// マウスカーソル非表示
		ShowCursor(FALSE);

	}
	else if (g_Camera.state == STATE_OVERLOOK)
	{
		PlaySound(GetSoundID(SE_HYOUI), 0);
		g_Camera.state = STATE_TPS;

		// カメラの頭を上に設定
		g_Camera.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// マウスカーソル非表示
		ShowCursor(FALSE);

		possessionFlag = true;
	}
}

//カメラの憑依するときの演出
void CameraProduction_Possession(D3DXVECTOR3 pos)
{
	// カメラの注視点をプレイヤーの座標にしてみる
	g_Camera.at = pos;
	possessionCnt++;
	//１回だけ読み込むように
	if (possessionCnt ==1)
	{
		//座標から速度を求める
		cameraPossessionVel = (HEIGHT_CAM_OVERLOOK - (pos.y + 300.0f)) / POSSESSION_TARGET;
	}
	//カメラが近づいていく
	if (possessionCnt <= POSSESSION_TARGET)
	{
		possessionFadeInFlag = true;
		// カメラの視点をカメラのY軸回転に対応させている
		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * 1;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * 1;
		g_Camera.pos.y -= cameraPossessionVel;
	}
	//神視点から近づいていくまでの時間
	if (possessionCnt == POSSESSION_TARGET)
	{
		possessionFadeInFlag = false;
		possessionBlackoutFlag = true;
	}
	//演出の時間
	else if (possessionCnt>= POSSESSION_TARGET+ POSSESSION_BLACKOUT_TARGET+ POSSESSION_LIGHTCHANGE_TARGET)
	{
		possessionCnt = 0;
		cameraPossessionVel = 0;
		possessionBlackoutCnt = 0;
		possessionLightChangeCnt = 0;
		possessionFlag = false;
	}	
	//暗転中です！
	if (possessionBlackoutFlag)
	{		// カメラの視点をカメラのY軸回転に対応させている
		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * 1;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * 1;
		possessionBlackoutCnt++;
		//暗転の最大時間
		if (possessionBlackoutCnt >= POSSESSION_BLACKOUT_TARGET)
		{
			g_Camera.rot = { 0.0f,0.0f,0.0f };
			possessionBlackoutFlag = false;
			possessionLightChangeFlag = true;
		}
	}
	//明転中です！
	if (possessionLightChangeFlag)
	{	// カメラの視点をカメラのY軸回転に対応させている
		g_Camera.pos.x = g_Camera.at.x - sinf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.pos.z = g_Camera.at.z - cosf(g_Camera.rot.y) * g_Camera.len;
		g_Camera.pos.y = pos.y;
		// マウスカーソルを中央に移動させる
		SetCursorPos(centerXCa, centerYCa);
		possessionLightChangeCnt++;
		//明転の最大時間
		if (possessionLightChangeCnt >= POSSESSION_LIGHTCHANGE_TARGET)
		{
			possessionLightChangeFlag = false;
		}
	}
}

void SetCameraSensi(float f)
{
	camera_sensi = f;
}

float GetCameraSensi(void)
{
	return camera_sensi;
}