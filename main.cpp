#include <Novice.h>
#include <imgui.h>
#include <string>
#include <vector>

const char kWindowTitle[] = "LE2C_15_タナカ_オリジナルアセンブルシステム";

// =========================
// 武器タイプ
// =========================
enum WeaponType { kFire, kMelee };

// =========================
// 構造体
// =========================
struct Part {
	std::string name;
	int hp;
	int weight;
	int fireSuitability;
	int meleeSuitability;
	int loadCapacity;

	int textureHandle;
};

struct Weapon {
	std::string name;
	int baseAttack;
	int weight;

	WeaponType type;

	int textureHandle;
};

// =========================
// パーツデータ
// =========================

// HEAD
std::vector<Part> headParts = {
    {"Light Head", 200, 50,  0, 0, 0, 0},
    {"Mid Head",   400, 100, 0, 0, 0, 0},
    {"Heavy Head", 700, 200, 0, 0, 0, 0}
};

// ARMS
std::vector<Part> armParts = {
    {"Light Arm (Fire Spec)",  300, 150, 120, 80,  0, 0},
    {"Mid Arm (Balanced)",     500, 300, 100, 100, 0, 0},
    {"Heavy Arm (Melee Spec)", 900, 550, 70,  130, 0, 0}
};

// CORE
std::vector<Part> coreParts = {
    {"Light Core", 800,  300,  0, 0, 0, 0},
    {"Mid Core",   1200, 600,  0, 0, 0, 0},
    {"Heavy Core", 1800, 1000, 0, 0, 0, 0}
};

// LEGS
std::vector<Part> legParts = {
    {"Light Legs",  1000, 500,  0, 0, 2500, 0},
    {"Middle Legs", 1600, 900,  0, 0, 3500, 0},
    {"Tank",        2500, 1500, 0, 0, 4000, 0}
};

// =========================
// 武器データ
// =========================

// 右手武器
std::vector<Weapon> rangedWeapons = {
    {"Hand Gun", 250, 80,  kFire, 0},
    {"Shot Gun", 500, 250, kFire, 0},
    {"Grenade",  900, 400, kFire, 0}
};

// 左手武器
std::vector<Weapon> meleeWeapons = {
    {"Shot Gun",    500,  250, kFire,  0},
    {"Laser Blade", 700,  250, kMelee, 0},
    {"Pile Bunker", 1100, 1000, kMelee, 0}
};

// =========================
// 選択インデックス
// =========================
int selectedHead = 1;
int selectedArm = 1;
int selectedCore = 1;
int selectedLeg = 1;

int selectedRangedWeapon = 0;
int selectedMeleeWeapon = 0;

// =========================
// ImGui UI
// =========================
void DrawPartSelector(const char* label, const std::vector<Part>& parts, int& selected) {

	if (ImGui::BeginCombo(label, parts[selected].name.c_str())) {

		for (int i = 0; i < static_cast<int>(parts.size()); i++) {

			bool isSelected = (selected == i);

			if (ImGui::Selectable(parts[i].name.c_str(), isSelected)) {
				selected = i;
			}
		}

		ImGui::EndCombo();
	}
}

void DrawWeaponSelector(const char* label, const std::vector<Weapon>& weapons, int& selected) {

	if (ImGui::BeginCombo(label, weapons[selected].name.c_str())) {

		for (int i = 0; i < static_cast<int>(weapons.size()); i++) {

			bool isSelected = (selected == i);

			if (ImGui::Selectable(weapons[i].name.c_str(), isSelected)) {
				selected = i;
			}
		}

		ImGui::EndCombo();
	}
}

// =========================
// メイン
// =========================
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	Novice::Initialize(kWindowTitle, 1280, 720);

	// =========================
	// テクスチャ読み込み
	// =========================

	// HEAD
	headParts[0].textureHandle = Novice::LoadTexture("./resources/light/lighthead.png");
	headParts[1].textureHandle = Novice::LoadTexture("./resources/mid/head.png");
	headParts[2].textureHandle = Novice::LoadTexture("./resources/heavy/heavyhead.png");

	// ARMS
	armParts[0].textureHandle = Novice::LoadTexture("./resources/light/lightarm.png");
	armParts[1].textureHandle = Novice::LoadTexture("./resources/mid/arm.png");
	armParts[2].textureHandle = Novice::LoadTexture("./resources/heavy/heavyarm.png");

	// CORE
	coreParts[0].textureHandle = Novice::LoadTexture("./resources/light/lightcore.png");
	coreParts[1].textureHandle = Novice::LoadTexture("./resources/mid/core.png");
	coreParts[2].textureHandle = Novice::LoadTexture("./resources/heavy/heavycore.png");

	// LEGS
	legParts[0].textureHandle = Novice::LoadTexture("./resources/light/lightleg.png");
	legParts[1].textureHandle = Novice::LoadTexture("./resources/mid/leg.png");
	legParts[2].textureHandle = Novice::LoadTexture("./resources/heavy/heavyleg.png");

	// =========================
	// 武器画像
	// =========================

	// 右手武器
	rangedWeapons[0].textureHandle = Novice::LoadTexture("./resources/weapon/hand.png");

	rangedWeapons[1].textureHandle = Novice::LoadTexture("./resources/weapon/shot.png");

	rangedWeapons[2].textureHandle = Novice::LoadTexture("./resources/weapon/gure.png");

	// 左手武器
	meleeWeapons[0].textureHandle = Novice::LoadTexture("./resources/weapon/shot.png");

	meleeWeapons[1].textureHandle = Novice::LoadTexture("./resources/weapon/brade.png");

	meleeWeapons[2].textureHandle = Novice::LoadTexture("./resources/weapon/pail.png");

	char keys[256] = {0};
	char preKeys[256] = {0};

	while (Novice::ProcessMessage() == 0) {

		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		// =========================
		// ステータス計算
		// =========================

		int totalAP = headParts[selectedHead].hp + coreParts[selectedCore].hp + armParts[selectedArm].hp + legParts[selectedLeg].hp;

		int totalWeight = headParts[selectedHead].weight + coreParts[selectedCore].weight + armParts[selectedArm].weight + legParts[selectedLeg].weight + rangedWeapons[selectedRangedWeapon].weight +
		                  meleeWeapons[selectedMeleeWeapon].weight;

		int currentLimit = legParts[selectedLeg].loadCapacity;

		// 腕適性
		int armFireStat = armParts[selectedArm].fireSuitability;

		int armMeleeStat = armParts[selectedArm].meleeSuitability;

		// =========================
		// 武器攻撃力計算
		// =========================

		// 右武器攻撃力
		int finalRightAtk = 0;

		if (rangedWeapons[selectedRangedWeapon].type == kFire) {

			finalRightAtk = rangedWeapons[selectedRangedWeapon].baseAttack * armFireStat / 100;

		} else {

			finalRightAtk = rangedWeapons[selectedRangedWeapon].baseAttack * armMeleeStat / 100;
		}

		// 左武器攻撃力
		int finalLeftAtk = 0;

		if (meleeWeapons[selectedMeleeWeapon].type == kFire) {

			finalLeftAtk = meleeWeapons[selectedMeleeWeapon].baseAttack * armFireStat / 100;

		} else {

			finalLeftAtk = meleeWeapons[selectedMeleeWeapon].baseAttack * armMeleeStat / 100;
		}

		// =========================
		// ImGui
		// =========================

		ImGui::Begin("Assemble Menu");

		ImGui::Text("--- FRAME PARTS ---");

		DrawPartSelector("HEAD", headParts, selectedHead);
		DrawPartSelector("CORE", coreParts, selectedCore);
		DrawPartSelector("ARMS", armParts, selectedArm);
		DrawPartSelector("LEGS", legParts, selectedLeg);

		ImGui::Separator();

		ImGui::Text("--- WEAPON PARTS ---");

		DrawWeaponSelector("R-ARM", rangedWeapons, selectedRangedWeapon);
		DrawWeaponSelector("L-ARM", meleeWeapons, selectedMeleeWeapon);

		ImGui::End();

		// =========================
		// 機体描画
		// =========================

		int machineX = 850;
		int machineY = 180;

		float machineScale = 1.0f;

		// 脚
		Novice::DrawSprite(machineX, machineY, legParts[selectedLeg].textureHandle, machineScale, machineScale, 0.0f, WHITE);

		// コア
		Novice::DrawSprite(machineX, machineY, coreParts[selectedCore].textureHandle, machineScale, machineScale, 0.0f, WHITE);

		// 腕
		Novice::DrawSprite(machineX, machineY, armParts[selectedArm].textureHandle, machineScale, machineScale, 0.0f, WHITE);

		// 頭
		Novice::DrawSprite(machineX, machineY, headParts[selectedHead].textureHandle, machineScale, machineScale, 0.0f, WHITE);

		// =========================
		// 武器描画
		// =========================

		// 右武器
		Novice::DrawSprite(machineX - 120, machineY + 120, rangedWeapons[selectedRangedWeapon].textureHandle, 1.0f, 1.0f, 0.0f, WHITE);

		// 左武器
		Novice::DrawSprite(machineX + 220, machineY + 120, meleeWeapons[selectedMeleeWeapon].textureHandle, 1.0f, 1.0f, 0.0f, WHITE);

		// =========================
		// ステータス表示
		// =========================

		int startX = 50;
		int startY = 40;
		int lineGap = 28;

		Novice::ScreenPrintf(startX, startY, "================================================");
		Novice::ScreenPrintf(startX, startY + lineGap * 1, "CURRENT ASSEMBLY");
		Novice::ScreenPrintf(startX, startY + lineGap * 2, "------------------------------------------------");

		Novice::ScreenPrintf(startX, startY + lineGap * 3, "HEAD  : %s", headParts[selectedHead].name.c_str());

		Novice::ScreenPrintf(startX, startY + lineGap * 4, "CORE  : %s", coreParts[selectedCore].name.c_str());

		Novice::ScreenPrintf(startX, startY + lineGap * 5, "ARMS  : %s (Fire:%d%% / Melee:%d%%)", armParts[selectedArm].name.c_str(), armFireStat, armMeleeStat);

		Novice::ScreenPrintf(startX, startY + lineGap * 6, "LEGS  : %s", legParts[selectedLeg].name.c_str());

		Novice::ScreenPrintf(startX, startY + lineGap * 7, "R-WEAP: %s (Base:%d)", rangedWeapons[selectedRangedWeapon].name.c_str(), rangedWeapons[selectedRangedWeapon].baseAttack);

		Novice::ScreenPrintf(startX, startY + lineGap * 8, "L-WEAP: %s (Base:%d)", meleeWeapons[selectedMeleeWeapon].name.c_str(), meleeWeapons[selectedMeleeWeapon].baseAttack);

		Novice::ScreenPrintf(startX, startY + lineGap * 10, "------------------------------------------------");
		Novice::ScreenPrintf(startX, startY + lineGap * 11, "TOTAL STATUS");
		Novice::ScreenPrintf(startX, startY + lineGap * 12, "------------------------------------------------");

		Novice::ScreenPrintf(startX, startY + lineGap * 13, "TOTAL AP : %d", totalAP);

		Novice::ScreenPrintf(startX, startY + lineGap * 14, "R-WEAPON ATK : %d", finalRightAtk);

		Novice::ScreenPrintf(startX, startY + lineGap * 15, "L-WEAPON ATK : %d", finalLeftAtk);

		if (totalWeight > currentLimit) {

			Novice::ScreenPrintf(startX, startY + lineGap * 16, "TOTAL WEIGHT : %d / %d [OVER LIMIT]", totalWeight, currentLimit);

		} else {

			Novice::ScreenPrintf(startX, startY + lineGap * 16, "TOTAL WEIGHT : %d / %d", totalWeight, currentLimit);
		}

		Novice::ScreenPrintf(startX, startY + lineGap * 17, "================================================");

		Novice::EndFrame();

		// ESC終了
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {

			break;
		}
	}

	Novice::Finalize();

	return 0;
}