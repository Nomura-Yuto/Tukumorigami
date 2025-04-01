//===============================================
//シーン制御[scene.cpp]
//class：上位モジュール
//Date:2023/11/28	Auther:嶋田
//===============================================
#include "memleakcheck.h"
#ifdef _DEBUG
#pragma push_macro("new")
#undef new
#endif
#include "collision.h"

#ifdef _DEBUG
#pragma pop_macro("new")
#endif

#include "scene.h"
#include "sound.h"
#define MOBGHOST_HP (60.0f)
#define MOBGHOST_ATK (2.0f)
#define MOBGHOST_RADIUS	(200.0f)
#define MOBONI_HP (200.0f)
#define MOBONI_ATK (10.0f)
#define MOBONI_RADIUS	(200.0f)
const float GRAVITY = -4.0f;
const float ONI_SURFACE = 200.0f;
const float GHOST_SURFACE = 1.0f;
const float ONMYOJI_SURFACE = 1.0f;
const float KOBUSHI_SURFACE = 1.0f;
const float TACHI_SURFACE = 1.0f;
const float YOJYUTSU_SURFACE = 1.0f;
//-------------------------------------
//変数宣言
//-------------------------------------
std::string  texturename[EFFECT_TEXTURE_MAX] = {};
//コンストラクタ
Scene::Scene(STAGE_SELECT stageselect)
{
	// 並行光源の設定（世界を照らす光）
	//g_DirectionalLight.Type = LIGHT_TYPE_DIRECTIONAL;						// 並行光源
	//g_DirectionalLight.Enable = true;										// このライトをON
	//g_DirectionalLight.Direction = D3DXVECTOR4(0.0f, -1.0f, 0.0f, 0.0f);	// 光の向き
	//g_DirectionalLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// 光の色
	//SetLight(0, &g_DirectionalLight);										// これで設定している
	int lightmax = 4;
	StopSound(GetSoundID(BGM_TITLE));
	m_explanationmanager = nullptr;
	m_stagesel = stageselect;
	SetModel();
	auto texturesmap = GetTextures();
	texturesmap["data/TEXTURE/foxfirebullet.png"] = LoadGraficTexture(GetDevice(), "foxfirebullet.png");
	texturesmap["data/TEXTURE/hiteffect.png"] = LoadGraficTexture(GetDevice(), "data/TEXTURE/hiteffect.png");
	texturesmap["data/TEXTURE/normalslasheffect.png"] = LoadGraficTexture(GetDevice(), "data/TEXTURE/normalslasheffect.png");
	texturesmap["data/TEXTURE/slasheffect.png"] = LoadGraficTexture(GetDevice(), "data/TEXTURE/slasheffect.png");
const std::string base1 = "data/PUNCHIEFFECT/image_";
for (int i = 0; i < PUNCHI_EFFECT_TEXTURE_MAX; i++) {
	texturename[i] = base1 + (i < 10 ? "00" : (i < 100 ? "0" : "")) + std::to_string(i) + ".png";
	auto PTexture = LoadGraficTexture(GetDevice(), texturename[i]);
	if (PTexture)texturesmap[texturename[i]] = PTexture;
}
const std::string base2 = "data/AREAEFFECT/aura_whiteblueSQ";
for (int i = 0; i < AREA_EFFECT_TEXTURE_MAX; i++) {
	texturename[i] = base2 + (i < 10 ? "0" : "") + std::to_string(i) + ".png";
	auto PTexture = texturesmap[texturename[i]] = LoadGraficTexture(GetDevice(), texturename[i]);
	if (PTexture)texturesmap[texturename[i]] = PTexture;
}
const std::string base3 = "data/LIGHTNINGEFFECT/Lightning";
for (int i = 0; i < LIGHTNING_EFFECT_TEXTURE_MAX; i++) {
	texturename[i] = base3 + (i < 10 ? "0" : "") + std::to_string(i) + ".png";
	auto PTexture = texturesmap[texturename[i]] = LoadGraficTexture(GetDevice(), texturename[i]);
	if (PTexture)texturesmap[texturename[i]] = PTexture;
}
const std::string base4 = "data/AURAEFFECT/image_";
for (int i = 0; i < AURA_EFFECT_TEXTURE_MAX; i++) {
	texturename[i] = base4 + (i < 10 ? "0" : "") + std::to_string(i) + ".png";
	auto PTexture = texturesmap[texturename[i]] = LoadGraficTexture(GetDevice(), texturename[i]);
	if (PTexture)texturesmap[texturename[i]] = PTexture;
}
	if (STAGE1 == stageselect)
	{
		pPlayer = new Player();
		pMap = new Map(1, pPlayer);
		pOnmyouji = new Onmyouji(D3DXVECTOR3(1190.0f, 200.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100);
		m_enemies = pMap->GetEnemys();
		m_items.push_back(new Item(D3DXVECTOR3(1000.0f, 50.0f, 500.0f), HP_RECOVERY));

		m_sikigami.push_back(new SikigamiKobushi(pPlayer, m_enemies, pMap->GetGimic(), 1));
		m_sikigami.push_back(new SikigamiTachi(pPlayer, m_enemies, pMap->GetGimic(), 2));
		m_sikigami.push_back(new SikigamiYoujyutsu(pPlayer, m_enemies, pMap->GetGimic(), 3));
		pUI = new UI(pOnmyouji, pPlayer, m_sikigami,m_enemies);//陰陽師や式神よりも後に置く
		
		pOnmyouji->SetEnemy(m_enemies);
		pPlayer->SetSikigami(m_sikigami);
		pPlayer->SetOnmyouji(pOnmyouji);
		pPlayer->SetSelectObj(pOnmyouji);
		pPlayer->SetItem(m_items);

		SetLightCnt(lightmax);

		m_planeeffect = new PlaneEffect(m_enemies, pMap->GetGimic(), m_sikigami,pPlayer);		//平面ポリゴンエフェクト
		m_polygoneffect = new PolygonEffect(m_sikigami, pPlayer);//ポリゴンエフェクト
		m_explanationmanager = new ExplanationManager;
		pPlayer->SetUiPin(pUI->GetUiPin());
		//PlaySound(GetSoundID(BGM_GAME), -1);

	}

	if (STAGE2 == stageselect)
	{
		pPlayer = new Player();
		pMap = new Map(2, pPlayer);
		pMap->SetPlayer(pPlayer);
		pOnmyouji = new Onmyouji(D3DXVECTOR3(1000.0f, 550.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100);
		m_enemies = pMap->GetEnemys();
		m_items.push_back(new Item(D3DXVECTOR3(550.0f, -100.0f, 2800.0f), HP_RECOVERY));
		//m_enemies.push_back(new MobGhost(D3DXVECTOR3(500.0f, 0.0f, 0.0f), MOBGHOST_HP, MOBGHOST_ATK, pPlayer));
		//m_enemies.push_back(new MobGhost(D3DXVECTOR3(100.0f, 0.0f, 500.0f), D3DXVECTOR3(500.0f, 0.0f, 1000.0f), MOBGHOST_HP, MOBGHOST_ATK, pPlayer));
		//m_enemies.push_back(new MobGhost(D3DXVECTOR3(200.0f, 0.0f, 0.0f), MOBGHOST_RADIUS, MOBGHOST_HP, MOBGHOST_ATK, pPlayer));
		///*m_enemies.push_back(new MobOni(D3DXVECTOR3(-100.0f, 0.0f, 800.0f), D3DXVECTOR3(500.0f, 0.0f, 300.0f), MOBGHOST_HP));*/
		//m_enemies.push_back(new MobGhost(D3DXVECTOR3(-200.0f, 0.0f, 0.0f), MOBGHOST_RADIUS, MOBGHOST_HP, MOBGHOST_ATK, pPlayer));
		//m_enemies.push_back(new MobGhost(D3DXVECTOR3(0.0f, 0.0f, 0.0f), MOBGHOST_RADIUS, MOBGHOST_HP, MOBGHOST_ATK, pPlayer));
		//m_items.push_back(new Item(D3DXVECTOR3(800.0f, 0.0f, 200.0f), HP_RECOVERY));

		m_sikigami.push_back(new SikigamiKobushi(pPlayer, m_enemies, pMap->GetGimic(), 1));
		m_sikigami.push_back(new SikigamiTachi(pPlayer, m_enemies, pMap->GetGimic(), 2));
		m_sikigami.push_back(new SikigamiYoujyutsu(pPlayer, m_enemies, pMap->GetGimic(), 3));
		pUI = new UI(pOnmyouji, pPlayer, m_sikigami,m_enemies);//陰陽師や式神よりも後に置く
		pOnmyouji->SetEnemy(m_enemies);
		pPlayer->SetSikigami(m_sikigami);
		pPlayer->SetOnmyouji(pOnmyouji);
		pPlayer->SetSelectObj(pOnmyouji);
		pPlayer->SetItem(m_items);

		SetLightCnt(lightmax);
		m_planeeffect = new PlaneEffect(m_enemies, pMap->GetGimic(),m_sikigami, pPlayer);		//平面ポリゴンエフェクト
		m_polygoneffect = new PolygonEffect(m_sikigami,pPlayer);//ポリゴンエフェクト
		pPlayer->SetUiPin(pUI->GetUiPin());
		//PlaySound(GetSoundID(BGM_GAME), -1);
	}

	if (STAGE3 == stageselect)
	{
		pPlayer = new Player();
		pMap = new Map(3, pPlayer);
		pMap->SetPlayer(pPlayer);
		pOnmyouji = new Onmyouji(D3DXVECTOR3(1500.0f, 550.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 100);
		m_enemies = pMap->GetEnemys();
    
		m_items.push_back(new Item(D3DXVECTOR3(550.0f, 300.0f, 2000.0f), HP_RECOVERY));
		m_items.push_back(new Item(D3DXVECTOR3(550.0f, 300.0f, 4000.0f), HP_RECOVERY));
		m_items.push_back(new Item(D3DXVECTOR3(600.0f, 300.0f, 9400.0f), HP_RECOVERY));

		m_sikigami.push_back(new SikigamiKobushi(pPlayer, m_enemies, pMap->GetGimic(), 1));
		m_sikigami.push_back(new SikigamiTachi(pPlayer, m_enemies, pMap->GetGimic(), 2));
		m_sikigami.push_back(new SikigamiYoujyutsu(pPlayer, m_enemies, pMap->GetGimic(), 3));
		pUI = new UI(pOnmyouji, pPlayer, m_sikigami, m_enemies);//陰陽師や式神よりも後に置く
		pOnmyouji->SetEnemy(m_enemies);
		pPlayer->SetSikigami(m_sikigami);
		pPlayer->SetOnmyouji(pOnmyouji);
		pPlayer->SetSelectObj(pOnmyouji);
		pPlayer->SetItem(m_items);

		SetLightCnt(lightmax);
		m_planeeffect = new PlaneEffect(m_enemies, pMap->GetGimic(), m_sikigami, pPlayer);		//平面ポリゴンエフェクト
		m_polygoneffect = new PolygonEffect(m_sikigami, pPlayer);//ポリゴンエフェクト
		pPlayer->SetUiPin(pUI->GetUiPin());
		//PlaySound(GetSoundID(BGM_GAME), -1);
	}

	PlaySound(GetSoundID(BGM_GAME), -1);
}
//デストラクタ
Scene::~Scene()
{
	if (m_explanationmanager) {
		delete m_explanationmanager;
		m_explanationmanager = nullptr;
	}
	if (m_planeeffect) {
		delete m_planeeffect;
		m_planeeffect = nullptr;
	}
	if (m_polygoneffect) {
		delete m_polygoneffect;
		m_polygoneffect = nullptr;
	}
	if (pMap) {
		delete pMap;
		pMap = nullptr;
	}
	if (pUI) {
		delete pUI;
		pUI = nullptr;
	}
	if (pPlayer) {
		delete pPlayer;
		pPlayer = nullptr;
	}
	for (auto it = m_sikigami.begin(); it != m_sikigami.end(); ) {
		delete* it;  // オブジェクトを削除
		it = m_sikigami.erase(it);  // リストから要素を削除し、次の要素のイテレータを取得
	}
	for (auto it = m_items.begin(); it != m_items.end(); ) {
		delete* it;  // オブジェクトを削除
		it = m_items.erase(it);  // リストから要素を削除し、次の要素のイテレータを取得
	}
	for (auto it = m_enemies.begin(); it != m_enemies.end(); ) {
		delete *it;  // オブジェクトを削除`
		it = m_enemies.erase(it);  // リストから要素を削除し、次の要素のイテレータを取得
	}
	if (pOnmyouji) {
		delete pOnmyouji;
		pOnmyouji = nullptr;
	}
	ClearModels();
	UnloadAllTextures();
	StopSoundAll();
}
//更新処理
void Scene::Update(void)
{
	CleanUp();
	float posy = FLT_MAX;
	//pMap->SetEnemies(m_enemies);
	pOnmyouji->SetEnemy(m_enemies);
	for (auto& sikigami : m_sikigami) {
		sikigami->SetEnemyList(m_enemies);
		//sikigami->Update();
	}
	
	for (auto item : m_items) {
		item->Update();
	}
	pOnmyouji->SetOldPos(pOnmyouji->GetPos());
	for (auto it :m_sikigami) {
		it->SetOldPos(it->GetPos());
		posy = min(posy, it->GetPosY());
	}
	pOnmyouji->SetEnemy(m_enemies);
	pMap->Update();
	if (pOnmyouji->GettDeathFlag()) {
		pOnmyouji->Update();
		playerdeathFlag = true;
	}
	else if (!pOnmyouji->GettDeathFlag()) 
	{

		pPlayer->Update();
		pOnmyouji->Update();
		for (auto& enemy : m_enemies) {
			enemy->Update();
		}
		for (auto& sikigami : m_sikigami) {
			sikigami->SetEnemyList(m_enemies);
			sikigami->Update();
		}
		m_planeeffect->Update();
		m_polygoneffect->Update();
		pUI->Update();
		if (m_explanationmanager) {
			m_explanationmanager->SetPlayerPos(pPlayer->GetSelectObj()->GetPos());
			m_explanationmanager->Update();
		}

		CollisionControl();
		if (GetKeyboardTrigger(DIK_K))
		{
			SetScene(SCENE_STAGE_SELECT);
		}
	}
	if (GetScene() != SCENE_RESULT) {
		if (GetCameraState() == STATE_OVERLOOK) {
			g_SelectCharLight.Attenuation = 0;
			SetLight(3, &g_SelectCharLight);
			if (m_stagesel != STAGE2)
			{
				// 点光源の設定
				g_PointLight[0].Type = LIGHT_TYPE_POINT;						// 並行光源
				g_PointLight[0].Enable = true;									// このライトをON
				g_PointLight[0].Direction = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);// 光の方向
				g_PointLight[0].Position = D3DXVECTOR4(GetCamera()->pos);		// 光の座標
				g_PointLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 光の色
				g_PointLight[0].Attenuation = 8000.0f;							// 減衰距離
				SetLight(1, &g_PointLight[0]);									// これで設定している



			}
			if (m_stagesel == STAGE2)
			{
				D3DXVECTOR4 maxpos = { pOnmyouji->GetPosX(),3000.0f,pOnmyouji->GetPosZ(),0.0f };
				// 点光源の設定
				g_PointLight[0].Type = LIGHT_TYPE_POINT;						// 並行光源
				g_PointLight[0].Enable = false;									// このライトをON
				g_PointLight[0].Direction = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);// 光の方向
				g_PointLight[0].Position = D3DXVECTOR4(GetCamera()->pos) - D3DXVECTOR4(0.0f, 800.0f, 0.0f, 0.0f);		// 光の座標
				g_PointLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 光の色
				g_PointLight[0].Attenuation = 600.0f;								// 減衰距離
				SetLight(1, &g_PointLight[0]);									// これで設定している

			}
			D3DXVECTOR4 centerpos = { 200.0f * MAP_CHIP_X,-3000.0f,200.0f * MAP_CHIP_Z,0.0f };	//マップの中央
			D3DXVECTOR4 minpos = { 0.0f,0.0f,0.0f,0.0f };
			D3DXVECTOR4 maxpos = { 200.0f * MAP_CHIP_X,3000.0f,200.0f * MAP_CHIP_Z,0.0f };
			D3DXVECTOR4 lmaxrminpos = { 200.0f * MAP_CHIP_X,3000.0f,0.0f,0.0f };
			D3DXVECTOR4 rmaxlminpos = { 0.0f,3000.0f,200.0f * MAP_CHIP_Z,0.0f };

			D3DXVECTOR4 direction = { 0.0f, 0.0f, 0.0f, 0.0f };		//方向
			float attenuation = 0.0f;							//描画距離限界
		}
		if (GetCameraState() == STATE_TPS) {
			D3DXVECTOR4 centerpos = { 200.0f * MAP_CHIP_X,3000.0f,200.0f * MAP_CHIP_Z,0.0f };	//マップの中央
			D3DXVECTOR4 minpos = { 0.0f,3000.0f,0.0f,0.0f };
			D3DXVECTOR4 maxpos = { pOnmyouji->GetPosX(),3000.0f,pOnmyouji->GetPosZ(),0.0f };
			D3DXVECTOR4 lmaxrminpos = { 200.0f * MAP_CHIP_X,3000.0f,0.0f,0.0f };
			D3DXVECTOR4 rmaxlminpos = { 0.0f,3000.0f,200.0f * MAP_CHIP_Z,0.0f };

			D3DXVECTOR4 direction = { 0.0f, 1.0f, 0.0f, 0.0f };		//方向
			float attenuation = 50.0f;							//描画距離限界
			if (m_stagesel != STAGE2)
			{
				g_PointLight[0].Attenuation = 0;
				SetLight(1, &g_PointLight[0]);

				//// 点光源の設定
				if (posy > 0) {
					g_PointLight[1].Type = LIGHT_TYPE_POINT;						// 点光源
					g_PointLight[1].Enable = true;									// このライトをON
					g_PointLight[1].Direction = direction;// 光の方向
					g_PointLight[1].Position = maxpos;// 光の座標
					g_PointLight[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 光の色
					g_PointLight[1].Attenuation = 50000;							// 減衰距離
					SetLight(2, &g_PointLight[1]);									// これで設定している
				}
				else {
					g_PointLight[1].Attenuation = 0;
					SetLight(2, &g_PointLight[1]);
				}
			}
			if (m_stagesel == STAGE2)
			{
				g_PointLight[0].Attenuation = 0;
				SetLight(1, &g_PointLight[0]);

				//// 点光源の設定
				if (posy > 0) {
					g_PointLight[1].Type = LIGHT_TYPE_POINT;						// 点光源
					g_PointLight[1].Enable = true;									// このライトをON
					g_PointLight[1].Direction = direction;// 光の方向
					g_PointLight[1].Position = maxpos;// 光の座標
					g_PointLight[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 光の色
					g_PointLight[1].Attenuation = 500;								// 減衰距離
					SetLight(2, &g_PointLight[1]);									// これで設定している
				}
				else {
					g_PointLight[1].Attenuation = 0;
					SetLight(2, &g_PointLight[1]);
				}
			}
			D3DXVECTOR3 camdir = GetCamera()->at - GetCamera()->pos;
			D3DXVECTOR4 LightDirection = { camdir.x, camdir.y, camdir.z, 0.0f };
			D3DXVec4Normalize(&LightDirection, &LightDirection);
			g_SelectCharLight.Type = LIGHT_TYPE_POINT;
			g_SelectCharLight.Enable = true;
			g_SelectCharLight.Direction = LightDirection;
			g_SelectCharLight.Position = { GetCamera()->pos.x, GetCamera()->pos.y, GetCamera()->pos.z, 0.0f };
			g_SelectCharLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// 光の色
			g_SelectCharLight.Attenuation = 2000;
			SetLight(3, &g_SelectCharLight);										// これで設定している


		}
	}
}
//描画処理
void Scene::Draw(void)
{

	if(pMap)pMap->Draw();
	if(pPlayer)pPlayer->Draw();
	if(pOnmyouji)pOnmyouji->Draw();

	for (auto &enemy : m_enemies) {
		if(enemy)enemy->Draw();
	}
	for (auto& sikigami : m_sikigami) {
		if(sikigami)sikigami->Draw();
	}
	for (auto& item : m_items) {
		if(item)item->Draw();
	}
	if(m_planeeffect)m_planeeffect->Draw();
	if(m_polygoneffect)m_polygoneffect->Draw();
	
	pUI->Draw();
	if (m_explanationmanager) {
		m_explanationmanager->Draw();
	}
	
}

void Scene::CleanUp(void)
{
	m_enemies.remove_if([](Enemy* p) {
		if (!p->GetFlag()) {
			delete p;
			return true;
		}
		return false; 
	});
	pMap->GetGimic().remove_if([](Gimic* p) {
		if (!p->GetGimicFlag()) {

			//岩の破壊エフェクト(岩の情報が消される前に描画)
			if (p->GetGimicId() == ID_GIMIC_ROCK)
			{
				p->SetGimicFlag(true);
				for (int i = 0; i < 12000; i++)
				{
					p->Draw();
				}
			}
			delete p;
			return true;
		}
		return false;
	});
	for (auto sikigami : m_sikigami) {
		auto youjitsu = dynamic_cast<SikigamiYoujyutsu*>(sikigami);
		if (youjitsu) {
			youjitsu->CleanBullet();
		}
	}
		
}
void Scene::CollisionControl()
{

	auto groundlist = pMap->GetAllGroundList();
	D3DXVECTOR3 playerpos = pPlayer->GetPos();
	auto pCamera = GetCamera();

//----------------------------------------------------
//ギミックの当たり判定
//Date:2023/01/17	Auther:林祐也
//----------------------------------------------------
	for (auto& gimic : pMap->GetGimic()) {
		if (gimic)
		{
			if (gimic->GetFlag())
			{
				if (CollisionBB_PSS(pOnmyouji->GetPos(), pOnmyouji->GetSize(), pOnmyouji->GetScl(), gimic->GetPos(), gimic->GetSize(), gimic->GetScl()))
				{
					pUI->GetUiPin()->SetFlag(false);
					pOnmyouji->SetPos(pOnmyouji->GetOldPos());
				}

				for (auto sikigami : m_sikigami)
				{
					if (CollisionBB_PSS(sikigami->GetPos(), sikigami->GetSize(), sikigami->GetScl(), gimic->GetPos(), gimic->GetSize(), gimic->GetScl())) {
						//ぶつかった座標を式神の座標にする
						sikigami->SetPos(sikigami->GetOldPos());
					}
				}
			}
		}
	}

	//----------------------------------------------------
	//マップの当たり判定
	// // Date:2023/12/24	Auther:三浦飛宇
	//----------------------------------------------------

	for (auto& obstacle : pMap->GetObstacle()) {
		if (obstacle)
		{
			if (obstacle->GetObstacleId() == ID_CEILING)
			{
				if (GetCameraState() == STATE_TPS)
				{
					obstacle->SetScl(GROUND_SCALE);
				}
				else
				{
					obstacle->SetScl(GROUND_SCALE);
					if (CollisionBB_PSS(pOnmyouji->GetPos(), pOnmyouji->GetSize() + D3DXVECTOR3(15.0f, 10000.0f, 15.0f), pOnmyouji->GetScl() + D3DXVECTOR3(15.0f, 0.0f, 15.0f), obstacle->GetPos(), obstacle->GetSize(), obstacle->GetScl()))
					{
						obstacle->SetScl(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
					}
				}
			}
			else if (obstacle->GetObstacleId() == ID_WALL)
			{
				/*===================================================================================
				カメラ自動調整
				Date:2024/2/6
				制作者：譚偉進
				====================================================================================*/
				if (pCamera->state == STATE_TPS) {
					while (CollisionBB_PSS(pCamera->pos, { 1.0f,1.0f,1.0f }, { 1.0f,1.0f,1.0f }, obstacle->GetPos(), obstacle->GetSize(), obstacle->GetScl())) {
						D3DXVECTOR3 dir = pCamera->at - pCamera->pos;
						auto distance = D3DXVec3Length(&dir);
						if (distance <= 1.0f)break;
						D3DXVec3Normalize(&dir, &dir);
						pCamera->pos += dir;
					}
				}
				//当たったら/当たっている時

				if (CollisionBB_PSS(pOnmyouji->GetPos(), pOnmyouji->GetSize(), pOnmyouji->GetScl(), obstacle->GetPos(), obstacle->GetSize(), obstacle->GetScl()))
				{

					pUI->GetUiPin()->SetFlag(false);
					//ぶつかった座標を陰陽師の座標にする
					pOnmyouji->SetPos(pOnmyouji->GetOldPos());

				}

				for (auto sikigami : m_sikigami)
				{
					//式神の当たり判定
					if (CollisionBB_PSS(sikigami->GetPos(), sikigami->GetSize(), sikigami->GetScl(), obstacle->GetPos(), obstacle->GetSize(), obstacle->GetScl()))
					{
						sikigami->SetPos(sikigami->GetOldPos());
					}
					/*====================================弾の処理=============================================== */
					auto youjitsu = dynamic_cast<SikigamiYoujyutsu*>(sikigami);
					if (youjitsu) {
						auto bullets = youjitsu->GetBullet();
						for (auto& bullet : bullets) {
							if (bullet) {
								if (CollisionBB_PSS(obstacle->GetPos(), obstacle->GetSize(), obstacle->GetScl(), bullet->GetPos(), bullet->GetSize(), bullet->GetScl())) {
									bullet->SetFlag(false);
								}
							}
						}
					}
				}
			}

			/*=========================================階層座標========================================== */

			else if (obstacle->GetObstacleId() == ID_GROUND)
			{

			}

			else if (obstacle->GetObstacleId() == ID_UNDERGROUND)
			{
				//----------------------------------------------------
				// スロープ処理
				// Date:2024/01/30	Auther:野村悠人
				//----------------------------------------------------
				GameObj* obj = pPlayer->GetSelectObj();

				if (CollisionBB_PSS(obj->GetPos(), obj->GetSize(), obj->GetScl(), obstacle->GetPos(), obstacle->GetSize(), obstacle->GetScl()))
				{
					if (obstacle->GetObstacleId() == ID_SLOPE_LEFT)
					{
						obj->SetPosY(obj->GetPosY() + pPlayer->GetVel().x);
					}
					if (obstacle->GetObstacleId() == ID_SLOPE_RIGHT)
					{
						obj->SetPosY(obj->GetPosY() - pPlayer->GetVel().x);
					}
					if (obstacle->GetObstacleId() == ID_SLOPE_FRONT)
					{
						obj->SetPosY(obj->GetPosY() - pPlayer->GetVel().z);
					}
					if (obstacle->GetObstacleId() == ID_SLOPE_BACK)
					{
						obj->SetPosY(obj->GetPosY() + pPlayer->GetVel().z);
					}
				}
			}
			else if (obstacle->GetObstacleId() == ID_GOAL)
			{
				CollisionMapPoly(*obstacle);
				CollisionPoly(pOnmyouji, pOnmyouji->GetModelObj());

				if (CollisionBB(pOnmyouji->GetModelTrasformMinPos(), pOnmyouji->GetModelTrasformMaxPos(), 
					obstacle->GetModelTrasformMinPos(), obstacle->GetModelTrasformMaxPos()))
				{
					StopSoundAll();
					PlaySound(GetSoundID(SE_GAMECLEAR), 0);
					SetScene(SCENE_RESULT);
					return;
				}
			}


		}
	}
	/*===================================================================================
	重力システム
	Date:2024/2/8
	制作者：譚偉進
	====================================================================================*/
	for (auto sikigami : m_sikigami) {
		sikigami->SetPosY(sikigami->GetPosY() + GRAVITY);
	}
	for (auto enemy : m_enemies) {
		enemy->SetPosY(enemy->GetPosY() + GRAVITY);
	}
	pOnmyouji->SetPosY(pOnmyouji->GetPosY() + GRAVITY);

	for (auto sikigami : m_sikigami) {
		bool loopbreak = false;
		for (auto& obstacle : groundlist) {
			if (obstacle->GetObstacleId() != ID_GROUND && obstacle->GetObstacleId() != ID_UNDERGROUND)continue;

			if (obstacle->GetObstacleId() == ID_GROUND) {
				CollisionMapPoly(*obstacle);
				if((sikigami->GetPos().y - (sikigami->GetSize().y*sikigami->GetScl().y)*0.5f)<= obstacle->GetModelTrasformMaxPos().y ){
					if (CollisionBB_PSS(obstacle->GetPos(), obstacle->GetSize(), obstacle->GetScl(), sikigami->GetPos(), sikigami->GetSize(), sikigami->GetScl())) {
						sikigami->SetPosY(obstacle->GetPos().y + 5);
						loopbreak = true;
						break;
					}
				}
			}
			   
			if (obstacle->GetObstacleId() == ID_UNDERGROUND) { //地下すぎたら実行しない
				CollisionMapPoly(*obstacle);
				if ((sikigami->GetPos().y - (sikigami->GetSize().y * sikigami->GetScl().y) * 0.5f) <= obstacle->GetModelTrasformMaxPos().y) {
					if (CollisionBB_PSS(obstacle->GetPos(), obstacle->GetSize(), obstacle->GetScl(), sikigami->GetPos(), sikigami->GetSize(), sikigami->GetScl())) {
						sikigami->SetPosY(obstacle->GetPos().y);
						loopbreak = true;
						break;
					}
				}
			}

			
		}
		if (loopbreak == true)continue;
	}
	for (auto& enemy : m_enemies) {
		bool breakloop = false;
		for (auto& obstacle : groundlist) {
			if (obstacle->GetObstacleId() != ID_GROUND && obstacle->GetObstacleId() != ID_UNDERGROUND)continue;
	
			if (obstacle->GetObstacleId() == ID_GROUND) {
				CollisionMapPoly(*obstacle);
				CollisionPoly(enemy, enemy->GetModelObj());
				if (enemy->GetModelTrasformMinPos().y <= obstacle->GetModelTrasformMaxPos().y) {
					if (CollisionBB(obstacle->GetModelTrasformMinPos(), obstacle->GetModelTrasformMaxPos(), enemy->GetModelTrasformMinPos(), enemy->GetModelTrasformMaxPos())) {
						enemy->SetPosY(obstacle->GetModelTrasformMaxPos().y);
						breakloop = true;
						break;
					}
				}
			}
	
			else if (obstacle->GetObstacleId() == ID_UNDERGROUND) {
				CollisionMapPoly(*obstacle);
				CollisionPoly(enemy, enemy->GetModelObj());
				if (enemy->GetModelTrasformMinPos().y <= obstacle->GetModelTrasformMaxPos().y) {  //地下すぎたら実行しない
					if (CollisionBB(obstacle->GetModelTrasformMinPos(), obstacle->GetModelTrasformMaxPos(), enemy->GetModelTrasformMinPos(), enemy->GetModelTrasformMaxPos())) {
						enemy->SetPosY(obstacle->GetModelTrasformMaxPos().y);
						breakloop = true;
						break;
					}
				}
			}
	
		}
		if (breakloop == true)continue;
	
	}

	for (auto& obstacle : groundlist) {
			if (obstacle->GetObstacleId() != ID_GROUND && obstacle->GetObstacleId() != ID_UNDERGROUND)continue;

				if (obstacle->GetObstacleId() == ID_GROUND) {
					CollisionMapPoly(*obstacle);
						if (pOnmyouji->GetModelTrasformMinPos().y <= obstacle->GetModelTrasformMaxPos().y) 
						{
						if (CollisionBB(obstacle->GetModelTrasformMinPos(), obstacle->GetModelTrasformMaxPos(), pOnmyouji->GetModelTrasformMinPos(), pOnmyouji->GetModelTrasformMaxPos())) 
							{
							pOnmyouji->SetPosY(obstacle->GetModelTrasformMaxPos().y);
							break;
							}
						}
				}
				if (obstacle->GetObstacleId() == ID_UNDERGROUND) {
					CollisionMapPoly(*obstacle);
					if (pOnmyouji->GetModelTrasformMinPos().y <= obstacle->GetModelTrasformMaxPos().y) {			//地下すぎたら実行しない
					if (CollisionBB(obstacle->GetModelTrasformMinPos(), obstacle->GetModelTrasformMaxPos(), pOnmyouji->GetModelTrasformMinPos(), pOnmyouji->GetModelTrasformMaxPos())) {
						pOnmyouji->SetPosY(obstacle->GetModelTrasformMaxPos().y);
						break;
					}
				}
			}
		}
	}
