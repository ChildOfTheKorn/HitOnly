#pragma once

#include "config-utils/shared/config-utils.hpp"

DECLARE_CONFIG(HitOnlyConfig) {
    CONFIG_VALUE(enabled, bool, "enabled", true);
    // HUD label position in metres (player at the origin, facing +Z); default sits above the
    // track, clear of the stock score/combo panels at the sides
    CONFIG_VALUE(hudX, float, "hudX", 0.0f);
    CONFIG_VALUE(hudY, float, "hudY", 3.0f);
    CONFIG_VALUE(hudZ, float, "hudZ", 6.0f);
    CONFIG_VALUE(textSize, float, "textSize", 12.0f);
};
