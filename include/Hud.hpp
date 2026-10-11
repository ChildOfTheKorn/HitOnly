#pragma once

#include "UnityEngine/SceneManagement/Scene.hpp"

namespace HitOnly::Hud {

void Create(UnityEngine::SceneManagement::Scene scene);
void Refresh();
void Destroy();

}  // namespace HitOnly::Hud
