#include "Hooks.hpp"
#include "HitOnlyConfig.hpp"
#include "Hud.hpp"
#include "Stats.hpp"
#include "main.hpp"

#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "bsml/shared/BSML-Lite.hpp"

#include "GlobalNamespace/ScoreController.hpp"
#include "GlobalNamespace/ScoringElement.hpp"
#include "GlobalNamespace/GoodCutScoringElement.hpp"
#include "GlobalNamespace/BadCutScoringElement.hpp"
#include "GlobalNamespace/MissScoringElement.hpp"
#include "GlobalNamespace/NoteData.hpp"
#include "GlobalNamespace/ColorType.hpp"
#include "GlobalNamespace/ResultsViewController.hpp"
#include "GlobalNamespace/LevelCompletionResults.hpp"
#include "HMUI/CurvedTextMeshPro.hpp"
#include "TMPro/TextAlignmentOptions.hpp"

using namespace GlobalNamespace;

// Display only: nothing here writes to scoring state.
// ScoreController::LateUpdate (1.40.8) queues each finished note, fires scoringForNoteFinishedEvent for it,
// then calls DespawnScoringElement once per queued note. Reading the element just before it's despawned
// sees exactly what the event sees, without a custom-types delegate to create and tear down at song end.

static SafePtrUnity<HMUI::CurvedTextMeshPro> resultsLabel;
// Set at song start so toggling mid-song can't leave half-counted stats
static bool trackingThisSong = false;

static void HandleScoringForNoteFinished(ScoringElement* element) {
    if (!element) return;
    auto noteData = element->get_noteData();
    if (noteData && noteData->get_gameplayType() == NoteData_GameplayType::Bomb) return;

    auto& stats = HitOnly::currentStats;
    if (il2cpp_utils::try_cast<GoodCutScoringElement>(element)) {
        // 115 for normal notes; chain links use their own max (20) so they don't drag the % down
        int cutScore = element->get_cutScore();
        int maxCutScore = element->get_maxPossibleCutScore();
        stats.all.Add(cutScore, maxCutScore);
        if (noteData && noteData->get_colorType() == ColorType::ColorA) stats.left.Add(cutScore, maxCutScore);
        else if (noteData && noteData->get_colorType() == ColorType::ColorB) stats.right.Add(cutScore, maxCutScore);
        HitOnly::Hud::Refresh();
    } else if (il2cpp_utils::try_cast<BadCutScoringElement>(element)) {
        stats.badCuts++;
    } else if (il2cpp_utils::try_cast<MissScoringElement>(element)) {
        stats.misses++;
    }
}

MAKE_HOOK_MATCH(ScoreController_Start, &ScoreController::Start, void, ScoreController* self) {
    ScoreController_Start(self);
    HitOnly::currentStats = {};
    trackingThisSong = getHitOnlyConfig().enabled.GetValue();
    // The HUD lives in the gameplay scene, so the game removes it along with the song
    if (trackingThisSong) HitOnly::Hud::Create(self->get_gameObject()->get_scene());
}

MAKE_HOOK_MATCH(ScoreController_DespawnScoringElement, &ScoreController::DespawnScoringElement, void, ScoreController* self, ScoringElement* scoringElement) {
    if (trackingThisSong && scoringElement && scoringElement->get_isFinished()) HandleScoringForNoteFinished(scoringElement);
    ScoreController_DespawnScoringElement(self, scoringElement);
}

MAKE_HOOK_MATCH(ResultsViewController_DidActivate, &ResultsViewController::DidActivate, void, ResultsViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    ResultsViewController_DidActivate(self, firstActivation, addedToHierarchy, screenSystemEnabling);

    bool enabled = getHitOnlyConfig().enabled.GetValue();
    if (!resultsLabel && enabled) {
        // Above the stock results panel, clear of the Continue/Restart buttons at the bottom
        auto label = BSML::Lite::CreateText(self->get_transform(), "", 4.5f, {0, 44}, {140, 18});
        label->set_alignment(TMPro::TextAlignmentOptions::Center);
        resultsLabel = label;
    }
    if (!resultsLabel) return;
    resultsLabel->get_gameObject()->SetActive(enabled);

    // Vanilla "Saber Surgeon" (MultiplayerBadgeDataPrecisionSO.GetValue, RE'd from 1.40.8): the level's
    // averageCenterDistanceCutScoreForNotesWithFullScoreScoringType / 15 * 100, shown as "{0:.0}% Precision"
    std::string surgeon;
    if (auto results = self->____levelCompletionResults) {
        float precision = results->averageCenterDistanceCutScoreForNotesWithFullScoreScoringType / 15.0f * 100.0f;
        surgeon = fmt::format("\nSaber Surgeon: <b>{:.1f}% Precision</b>", precision);
    }

    auto& stats = HitOnly::currentStats;
    resultsLabel->set_text(fmt::format(
        "Hit-only accuracy: <b>{}</b>   Good cuts: {}   Misses: {}   Bad cuts: {}\n"
        "Left: <b>{}</b> ({} cuts)   Right: <b>{}</b> ({} cuts){}",
        HitOnly::FormatPercent(stats.all.Percent()), stats.all.count, stats.misses, stats.badCuts,
        HitOnly::FormatPercent(stats.left.Percent()), stats.left.count,
        HitOnly::FormatPercent(stats.right.Percent()), stats.right.count, surgeon));
}

namespace HitOnly {

void InstallHooks() {
    getLogger().info("Installing hooks...");
    INSTALL_HOOK(getLogger(), ScoreController_Start);
    INSTALL_HOOK(getLogger(), ScoreController_DespawnScoringElement);
    INSTALL_HOOK(getLogger(), ResultsViewController_DidActivate);
    getLogger().info("Installed all hooks!");
}

}  // namespace HitOnly
