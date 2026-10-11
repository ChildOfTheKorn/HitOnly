#include "main.hpp"
#include "Hooks.hpp"
#include "HitOnlyConfig.hpp"
#include "UI/SettingsView.hpp"

#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"
#include "bsml/shared/BSML.hpp"
#include "custom-types/shared/register.hpp"

modloader::ModInfo modInfo{MOD_ID, VERSION, 0};

const Paper::ConstLoggerContext<8UL>& getLogger() {
    static constexpr auto Logger = Paper::ConstLoggerContext("hitonly");
    return Logger;
}

// Called at the early stages of game loading
HIT_ONLY_EXPORT_FUNC void setup(CModInfo& info) {
    info = modInfo.to_c();
    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
HIT_ONLY_EXPORT_FUNC void late_load() {
    il2cpp_functions::Init();
    getHitOnlyConfig().Init(modInfo);
    BSML::Init();
    custom_types::Register::AutoRegister();
    BSML::Register::RegisterSettingsMenu("HitOnly", HitOnly::UI::SettingsDidActivate, false);
    HitOnly::InstallHooks();
}
