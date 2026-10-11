#include "Hud.hpp"
#include "HitOnlyConfig.hpp"
#include "Stats.hpp"

#include "bsml/shared/BSML-Lite.hpp"
#include "bsml/shared/BSML/FloatingScreen/FloatingScreen.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "TMPro/TextAlignmentOptions.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Object.hpp"
#include "UnityEngine/SceneManagement/SceneManager.hpp"

#include <cstdio>

namespace HitOnly {

SongStats currentStats;

std::string FormatPercent(float percent) {
    if (percent < 0) return "--.-%";
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%.1f%%", percent);
    return buffer;
}

}  // namespace HitOnly

namespace HitOnly::Hud {

static SafePtrUnity<BSML::FloatingScreen> screen;
static SafePtrUnity<HMUI::CurvedTextMeshPro> totalText;
static SafePtrUnity<HMUI::CurvedTextMeshPro> leftText;
static SafePtrUnity<HMUI::CurvedTextMeshPro> rightText;

void Create(UnityEngine::SceneManagement::Scene scene) {
    // Normally nothing is left: the previous HUD went with the previous gameplay scene
    Destroy();
    auto& config = getHitOnlyConfig();
    float size = config.textSize.GetValue();
    UnityEngine::Vector3 position{config.hudX.GetValue(), config.hudY.GetValue(), config.hudZ.GetValue()};
    // Hands on the same line as the total, one each side (below it they were hidden by the notes).
    // Right-aligned / left-aligned boxes butt up against the total with a fixed gap, scaled by text size.
    float handSize = size * 0.75f;
    float gap = size * 1.8f;
    UnityEngine::Vector2 handBox{size * 5, size * 1.5f};

    // The screen clips anything outside its rect, so it has to span all three labels.
    // It's centred on the configured position, so the total at (0, 0) stays exactly there.
    UnityEngine::Vector2 screenSize{2 * (gap + handBox.x) + size, size * 2};
    auto floating = BSML::Lite::CreateFloatingScreen(screenSize, position, {0, 0, 0}, 0.0f, false, false);
    auto parent = floating->get_transform();

    auto total = BSML::Lite::CreateText(parent, "", size, {0, 0}, {size * 5, size * 1.5f});
    total->set_alignment(TMPro::TextAlignmentOptions::Center);
    total->set_enableWordWrapping(false);
    auto left = BSML::Lite::CreateText(parent, "", handSize, {-gap - handBox.x / 2, 0}, handBox);
    left->set_alignment(TMPro::TextAlignmentOptions::Right);
    left->set_enableWordWrapping(false);
    auto right = BSML::Lite::CreateText(parent, "", handSize, {gap + handBox.x / 2, 0}, handBox);
    right->set_alignment(TMPro::TextAlignmentOptions::Left);
    right->set_enableWordWrapping(false);

    UnityEngine::SceneManagement::SceneManager::MoveGameObjectToScene(floating->get_gameObject(), scene);
    screen = floating;
    totalText = total;
    leftText = left;
    rightText = right;
    Refresh();
}

void Refresh() {
    if (totalText) totalText->set_text("<u>" + FormatPercent(currentStats.all.Percent()) + "</u>");
    if (leftText) leftText->set_text("L " + FormatPercent(currentStats.left.Percent()));
    if (rightText) rightText->set_text("R " + FormatPercent(currentStats.right.Percent()));
}

void Destroy() {
    if (screen) UnityEngine::Object::Destroy(screen->get_gameObject());
    screen.clear();
    totalText.clear();
    leftText.clear();
    rightText.clear();
}

}  // namespace HitOnly::Hud
