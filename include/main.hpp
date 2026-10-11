#pragma once

#define HIT_ONLY_EXPORT __attribute__((visibility("default")))
#ifdef __cplusplus
#define HIT_ONLY_EXPORT_FUNC extern "C" HIT_ONLY_EXPORT
#else
#define HIT_ONLY_EXPORT_FUNC HIT_ONLY_EXPORT
#endif

#include "scotland2/shared/loader.hpp"
#include "paper2_scotland2/shared/logger.hpp"

extern modloader::ModInfo modInfo;

const Paper::ConstLoggerContext<8UL>& getLogger();
