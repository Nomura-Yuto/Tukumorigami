## 【ゲーム詳細】
俯瞰視点と式神に憑依した視点を切り替え、ゴールである鳥居を目指していく3Dゲーム。

### 開発期間
2年次に制作：5ヵ月

### 担当箇所
gamemouse.cpp\
gamemouse.h\
・ゲーム内表示用マウスの制御

camera.cpp\
214行～223行\
・カメラの状態で投影方法を変える\
411行～449行\
・俯瞰視点でのカメラのセット\
・カメラの状態の切り替え

player.cpp\
486行～562行\
・プレイヤー俯瞰視点時の移動処理\
568行～670行\
・式神選択処理

scene.cpp\
554行～574行\
・スロープでの移動処理

sensi.cpp\
178行～309行\
・マウスでのマウス感度の調整

stageselect.cpp\
57行～132行\
・マウスでのステージ選択

pause.cpp\
53行～81行\
・マウスでのポーズ画面内の選択

### 開発環境
DirectX11 / C++
