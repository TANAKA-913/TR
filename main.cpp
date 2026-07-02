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
// パーツ
// =========================
struct Part {

	std::string name;

	// 耐久力
	int hp;

	// 重量
	int weight;

	// EN負荷
	int enLoad;

	// 武器適性
	int fireSuitability;
	int meleeSuitability;

	// 脚専用（それ以外は0）
	int loadCapacity;

	// テクスチャ
	int textureHandle;
};

// =========================
// 武器
// =========================
struct Weapon {

	std::string name;

	// 基礎攻撃力
	int baseAttack;

	// 重量
	int weight;

	// EN負荷
	int enLoad;

	// 武器種類
	WeaponType type;

	// テクスチャ
	int textureHandle;
};

// =========================
// ジェネレーター
// =========================
struct Generator {

	std::string name;

	// EN出力
	int enOutput;

	// EN容量
	int enCapacity;

	// EN回復
	int enRecovery;

	// 重量
	int weight;

	// テクスチャ
	int textureHandle;
};

// =========================
// ブースター
// =========================
struct Booster {

	std::string name;

	// 推力
	int thrust;

	// 重量
	int weight;

	// EN負荷
	int enLoad;

	// テクスチャ
	int textureHandle;
};
class Machine {
public:
	// --- パーツ選択インデックス ---
	int selectedHead = 1;
	int selectedArm = 1;
	int selectedCore = 1;
	int selectedLeg = 1;
	int selectedRanged = 0;
	int selectedMelee = 0;
	int selectedGen = 1;
	int selectedBoost = 1;

	// --- パーツリスト ---
	const std::vector<Part>& headParts;
	const std::vector<Part>& armParts;
	const std::vector<Part>& coreParts;
	const std::vector<Part>& legParts;
	const std::vector<Weapon>& rangedWeapons;
	const std::vector<Weapon>& meleeWeapons;
	const std::vector<Generator>& generators;
	const std::vector<Booster>& boosters;

	Machine(
	    const std::vector<Part>& head, const std::vector<Part>& arm, const std::vector<Part>& core, const std::vector<Part>& leg, const std::vector<Weapon>& ranged, const std::vector<Weapon>& melee,
	    const std::vector<Generator>& gen, const std::vector<Booster>& boost)
	    : headParts(head), armParts(arm), coreParts(core), legParts(leg), rangedWeapons(ranged), meleeWeapons(melee), generators(gen), boosters(boost) {}

	int CalcTotalAP()  { return headParts[selectedHead].hp + coreParts[selectedCore].hp + armParts[selectedArm].hp + legParts[selectedLeg].hp; }

	int CalcTotalWeight()  {

		return headParts[selectedHead].weight + coreParts[selectedCore].weight + armParts[selectedArm].weight + legParts[selectedLeg].weight + rangedWeapons[selectedRanged].weight +
		       meleeWeapons[selectedMelee].weight + generators[selectedGen].weight + boosters[selectedBoost].weight;
	}

	int GetLoadCapacity()  { return legParts[selectedLeg].loadCapacity; }

	bool IsOverWeight()  { return CalcTotalWeight() > GetLoadCapacity(); }

	int CalcTotalENLoad()  {

		return headParts[selectedHead].enLoad + 
			coreParts[selectedCore].enLoad + 
			armParts[selectedArm].enLoad + 
			legParts[selectedLeg].enLoad + 
			rangedWeapons[selectedRanged].enLoad +
		       meleeWeapons[selectedMelee].enLoad + 
			boosters[selectedBoost].enLoad;
	}



	bool IsENOverLoad()  { return CalcTotalENLoad() > generators[selectedGen].enOutput; }

	int CalcEffectiveThrust()  {

		if (IsENOverLoad()) {

			return boosters[selectedBoost].thrust / 2;
		}

		return boosters[selectedBoost].thrust;
	}

	int CalcRightAtk()  {

		int suitability = (rangedWeapons[selectedRanged].type == kFire) ? armParts[selectedArm].fireSuitability : armParts[selectedArm].meleeSuitability;

		return static_cast<int>(rangedWeapons[selectedRanged].baseAttack * suitability / 100.0f);
	}

	int CalcLeftAtk()  {

		int suitability = (meleeWeapons[selectedMelee].type == kFire) ? armParts[selectedArm].fireSuitability : armParts[selectedArm].meleeSuitability;

		return static_cast<int>(meleeWeapons[selectedMelee].baseAttack * suitability / 100.0f);
	}

	void DrawMachine(int x, int y, float scale = 1.0f)  {

		Novice::DrawSprite(x, y, legParts[selectedLeg].textureHandle, scale, scale, 0.0f, WHITE);
		Novice::DrawSprite(x, y, coreParts[selectedCore].textureHandle, scale, scale, 0.0f, WHITE);
		Novice::DrawSprite(x, y, armParts[selectedArm].textureHandle, scale, scale, 0.0f, WHITE);
		Novice::DrawSprite(x, y, headParts[selectedHead].textureHandle, scale, scale, 0.0f, WHITE);

		Novice::DrawSprite(x - 120, y + 120, rangedWeapons[selectedRanged].textureHandle, 1.0f, 1.0f, 0.0f, WHITE);
		Novice::DrawSprite(x + 220, y + 120, meleeWeapons[selectedMelee].textureHandle, 1.0f, 1.0f, 0.0f, WHITE);

		// ジェネレーター（上）・ブースター（下）を武器の左に縦並び
		Novice::DrawSprite(x - 280, y + 120, generators[selectedGen].textureHandle, 1.0f, 1.0f, 0.0f, WHITE);
		Novice::DrawSprite(x - 280, y + 250, boosters[selectedBoost].textureHandle, 1.0f, 1.0f, 0.0f, WHITE);
	}

	void DrawStatus(int x, int y, int lineGap)  {

		Novice::ScreenPrintf(x, y + lineGap * 0, "==============================");

		Novice::ScreenPrintf(x, y + lineGap * 1, "HEAD : %s", headParts[selectedHead].name.c_str());
		Novice::ScreenPrintf(x, y + lineGap * 2, "CORE : %s", coreParts[selectedCore].name.c_str());
		Novice::ScreenPrintf(x, y + lineGap * 3, "ARMS : %s", armParts[selectedArm].name.c_str());
		Novice::ScreenPrintf(x, y + lineGap * 4, "LEGS : %s", legParts[selectedLeg].name.c_str());

		Novice::ScreenPrintf(x, y + lineGap * 5, "R-WEAPON : %s", rangedWeapons[selectedRanged].name.c_str());
		Novice::ScreenPrintf(x, y + lineGap * 6, "L-WEAPON : %s", meleeWeapons[selectedMelee].name.c_str());

		Novice::ScreenPrintf(x, y + lineGap * 7, "GENERATOR : %s", generators[selectedGen].name.c_str());
		Novice::ScreenPrintf(x, y + lineGap * 8, "BOOSTER   : %s", boosters[selectedBoost].name.c_str());

		Novice::ScreenPrintf(x, y + lineGap * 10, "AP            : %d", CalcTotalAP());
		Novice::ScreenPrintf(x, y + lineGap * 11, "R-WEAPON ATK  : %d", CalcRightAtk());
		Novice::ScreenPrintf(x, y + lineGap * 12, "L-WEAPON ATK  : %d", CalcLeftAtk());
		Novice::ScreenPrintf(x, y + lineGap * 13, "THRUST        : %d", CalcEffectiveThrust());

		if (IsOverWeight()) {
			Novice::ScreenPrintf(x, y + lineGap * 14, "WEIGHT        : %d / %d  [OVER]", CalcTotalWeight(), GetLoadCapacity());
		} else {
			Novice::ScreenPrintf(x, y + lineGap * 14, "WEIGHT        : %d / %d", CalcTotalWeight(), GetLoadCapacity());
		}

		if (IsENOverLoad()) {
			Novice::ScreenPrintf(x, y + lineGap * 15, "EN LOAD       : %d / %d  [EN OVER]", CalcTotalENLoad(), generators[selectedGen].enOutput);
		} else {
			Novice::ScreenPrintf(x, y + lineGap * 15, "EN LOAD       : %d / %d", CalcTotalENLoad(), generators[selectedGen].enOutput);
		}

		Novice::ScreenPrintf(x, y + lineGap * 16, "EN CAPACITY   : %d", generators[selectedGen].enCapacity);

		Novice::ScreenPrintf(x, y + lineGap * 17, "EN RECOVERY   : %d", generators[selectedGen].enRecovery);

		Novice::ScreenPrintf(x, y + lineGap * 18, "==============================");
	}
};
// =========================
// HEAD
// =========================
std::vector<Part> headParts = {

    {"Light Head", 200, 50,  120, 0, 0, 0, 0},

    {"Mid Head",   400, 100, 220, 0, 0, 0, 0},

    {"Heavy Head", 700, 200, 350, 0, 0, 0, 0}
};

// =========================
// ARMS
// =========================
std::vector<Part> armParts = {

    {"Light Arm (Fire Spec)",  300, 150, 350, 120, 80,  0, 0},

    {"Mid Arm (Balanced)",     500, 300, 500, 100, 100, 0, 0},

    {"Heavy Arm (Melee Spec)", 900, 550, 750, 70,  130, 0, 0}
};

// =========================
// CORE
// =========================
std::vector<Part> coreParts = {

    {"Light Core", 800,  300,  500,  0, 0, 0, 0},

    {"Mid Core",   1200, 600,  800,  0, 0, 0, 0},

    {"Heavy Core", 1800, 1000, 1200, 0, 0, 0, 0}
};

// =========================
// LEGS
// =========================
std::vector<Part> legParts = {

    {"Light Legs",  1000, 500,  450,  0, 0, 2500, 0},

    {"Middle Legs", 1600, 900,  700,  0, 0, 3500, 0},

    {"Tank Legs",   2500, 1500, 1100, 0, 0, 4500, 0}
};
// =========================
// R-WEAPON
// =========================
std::vector<Weapon> rangedWeapons = {

    {"Hand Gun", 250, 80,  180, kFire, 0},

    {"Shot Gun", 500, 250, 450, kFire, 0},

    {"Grenade",  900, 400, 800, kFire, 0}
};

// =========================
// L-WEAPON
// =========================
std::vector<Weapon> meleeWeapons = {

    {"Machine Gun", 320,  120,  250,  kFire,  0},

    {"Laser Blade", 700,  250,  500,  kMelee, 0},

    {"Pile Bunker", 1100, 1000, 1200, kMelee, 0}
};

// =========================
// GENERATOR
// =========================
std::vector<Generator> generators = {
    {"Light Generator",       3200, 9000,  900, 150, 0},
    {"Standard Generator",    4500, 12000, 650, 300, 0},
    {"High Output Generator", 6500, 17000, 450, 650, 0}
};

// =========================
// BOOSTER
// =========================
std::vector<Booster> boosters = {
    {"Light Booster",    1800, 120, 350,  0},
    {"Standard Booster", 2600, 250, 700,  0},
    {"Heavy Booster",    3800, 450, 1300, 0}
};

// =========================
// ImGui UI ヘルパー（参照渡しに統一）
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

void DrawGeneratorSelector(const char* label, const std::vector<Generator>& gens, int& selected) {
	if (ImGui::BeginCombo(label, gens[selected].name.c_str())) {
		for (int i = 0; i < static_cast<int>(gens.size()); i++) {
			bool isSelected = (selected == i);
			if (ImGui::Selectable(gens[i].name.c_str(), isSelected)) {
				selected = i;
			}
		}
		ImGui::EndCombo();
	}
}

void DrawBoosterSelector(const char* label, const std::vector<Booster>& bsts, int& selected) {
	if (ImGui::BeginCombo(label, bsts[selected].name.c_str())) {
		for (int i = 0; i < static_cast<int>(bsts.size()); i++) {
			bool isSelected = (selected == i);
			if (ImGui::Selectable(bsts[i].name.c_str(), isSelected)) {
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

	// GENERATOR
	generators[0].textureHandle = Novice::LoadTexture("./resources/generators/lightgen.png");
	generators[1].textureHandle = Novice::LoadTexture("./resources/generators/gen.png");
	generators[2].textureHandle = Novice::LoadTexture("./resources/generators/heavygen.png");

	// BOOSTER
	boosters[0].textureHandle = Novice::LoadTexture("./resources/boosters/lightboost.png");
	boosters[1].textureHandle = Novice::LoadTexture("./resources/boosters/boost.png");
	boosters[2].textureHandle = Novice::LoadTexture("./resources/boosters/heavyboost.png");

	// WEAPONS
	rangedWeapons[0].textureHandle = Novice::LoadTexture("./resources/weapon/hand.png");
	rangedWeapons[1].textureHandle = Novice::LoadTexture("./resources/weapon/shot.png");
	rangedWeapons[2].textureHandle = Novice::LoadTexture("./resources/weapon/gure.png");
	meleeWeapons[0].textureHandle = Novice::LoadTexture("./resources/weapon/shot.png");
	meleeWeapons[1].textureHandle = Novice::LoadTexture("./resources/weapon/brade.png");
	meleeWeapons[2].textureHandle = Novice::LoadTexture("./resources/weapon/pail.png");

	// =========================
	// Machineインスタンス生成
	// =========================
	Machine machine(headParts, armParts, coreParts, legParts, rangedWeapons, meleeWeapons, generators, boosters);

	char keys[256] = {0};
	char preKeys[256] = {0};

	while (Novice::ProcessMessage() == 0) {

		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		// =========================
		// ImGui
		// =========================
		ImGui::Begin("Assemble Menu");

		ImGui::Text("--- FRAME PARTS ---");
		DrawPartSelector("HEAD", headParts, machine.selectedHead);
		DrawPartSelector("CORE", coreParts, machine.selectedCore);
		DrawPartSelector("ARMS", armParts, machine.selectedArm);
		DrawPartSelector("LEGS", legParts, machine.selectedLeg);

		ImGui::Separator();

		ImGui::Text("--- WEAPON PARTS ---");
		DrawWeaponSelector("R-ARM", rangedWeapons, machine.selectedRanged);
		DrawWeaponSelector("L-ARM", meleeWeapons, machine.selectedMelee);

		ImGui::Separator();

		ImGui::Text("--- INNER PARTS ---");
		DrawGeneratorSelector("GENERATOR", generators, machine.selectedGen);
		DrawBoosterSelector("BOOSTER", boosters, machine.selectedBoost);

		ImGui::End();

		// =========================
		// 機体描画
		// =========================
		machine.DrawMachine(850, 180);

		// =========================
		// ステータス表示
		// =========================
		machine.DrawStatus(50, 40, 28);

		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
