#include "UI/SettingsView.hpp"
#include "HitOnlyConfig.hpp"

#include "bsml/shared/BSML-Lite.hpp"

using namespace BSML;

namespace HitOnly::UI {

void SettingsDidActivate(HMUI::ViewController* self, bool firstActivation, bool addedToHierarchy, bool screenSystemEnabling) {
    if (!firstActivation) return;
    auto& config = getHitOnlyConfig();
    auto container = Lite::CreateScrollableSettingsContainer(self->get_transform());

    // Takes effect from the next song
    Lite::CreateToggle(container, "Enabled", config.enabled.GetValue(), [](bool value) {
        getHitOnlyConfig().enabled.SetValue(value);
    });
    Lite::CreateSliderSetting(container, "HUD position X", 0.1f, config.hudX.GetValue(), -5, 5, 0.1f, [](float value) {
        getHitOnlyConfig().hudX.SetValue(value);
    });
    Lite::CreateSliderSetting(container, "HUD position Y", 0.1f, config.hudY.GetValue(), -1, 5, 0.1f, [](float value) {
        getHitOnlyConfig().hudY.SetValue(value);
    });
    Lite::CreateSliderSetting(container, "HUD position Z", 0.1f, config.hudZ.GetValue(), 1, 15, 0.1f, [](float value) {
        getHitOnlyConfig().hudZ.SetValue(value);
    });
    Lite::CreateSliderSetting(container, "Text size", 1, config.textSize.GetValue(), 4, 30, 0.1f, [](float value) {
        getHitOnlyConfig().textSize.SetValue(value);
    });
}

}  // namespace HitOnly::UI
