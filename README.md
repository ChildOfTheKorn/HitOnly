# HitOnly

A Beat Saber **Quest** mod (game version **1.40.8**) that shows your accuracy based only on the notes you actually cut. Misses, bad cuts, bombs and the combo multiplier are ignored. Display only: it never changes your score or anything sent to BeatLeader or ScoreSaber.

## Install

1. Download [`HitOnly.qmod`](https://github.com/ChildOfTheKorn/HitOnly/releases/latest/download/HitOnly.qmod) (or pick a version from the [releases page](https://github.com/ChildOfTheKorn/HitOnly/releases)).
2. Open it with **QuestPatcher** or **ModsBeforeFriday** and install it.
3. Restart Beat Saber.

See [Requirements](#requirements) for the game version and dependencies.

## What it shows

**During the song**, as a floating label above the track:

```
L 95.2%   96.4%   R 97.6%
```

- **Middle, underlined:** hit-only accuracy for both hands together.
- **L / R:** the same for each hand (left = red notes, right = blue notes).

**On the results screen**, a label above the stock results panel shows:
- Hit-only accuracy, number of good cuts, misses and bad cuts
- Left and right hand accuracy, with the number of good cuts for each
- **Saber Surgeon precision:** the same number the multiplayer "Saber Surgeon" badge compares

Your friends never see any of it. It's only drawn in your own game.

## How the numbers are calculated

- **Hit-only accuracy** = total points from your good cuts ÷ the most those cuts could have scored. A normal note is worth up to 115, so on most maps this is `sum of good-cut scores ÷ (115 × good cuts) × 100`. Chain links count against their own 20-point maximum so they don't drag the number down.
- **Saber Surgeon precision** works the way the game does it. Each cut's 115 points are made of up to 70 for the swing before the cut, up to 30 for the follow-through, and **up to 15 for how close to the centre you cut**. Precision is your average of those 15 accuracy points, as a percentage. Only good cuts on normal and arc notes count; chains, misses, bad cuts and bombs don't. Averaging 13 of 15 gives 86.7%. It's read directly from the game's own results data.

## Where it works

- **Live label:** solo, party, practice, vanilla multiplayer and BeatSaberPlus Multiplayer+ rooms.
- **Results label:** solo, party and Multiplayer+ rooms. Vanilla multiplayer uses a different results screen, so it doesn't show there.

## Requirements

- Beat Saber 1.40.8 on Quest, modded with Scotland2 (QuestPatcher or ModsBeforeFriday)
- Dependencies, normally already installed: `beatsaber-hook`, `custom-types`, `bsml`, `paper2_scotland2`

## Settings

On the main menu, press **Mod Settings** (the BSML button, not the game's gear Settings) and select **HitOnly**.

| Setting | Default | What it does |
|---|---|---|
| Enabled | On | Show the live label and the results label. |
| HUD position X | 0.0 | Left/right position of the live label, in metres. |
| HUD position Y | 3.0 | Height of the live label. |
| HUD position Z | 6.0 | Distance in front of you. |
| Text size | 12 | Size of the live label. The hand percentages are 75% of this. |

Changes apply from the next song.

## Notes

- If you also run **ScorePad** with padding on, HitOnly sees the padded scores, so both percentages will be inflated.
- **Startup crash:** the first launch after installing or updating a mod has occasionally crashed inside NoodleExtensions during startup. Just reopen the game.

## Building from source

Requires the Android NDK, CMake, Ninja and [QPM](https://github.com/QuestPackageManager/QPM.Package).

```powershell
qpm restore
./build.ps1
qpm qmod manifest
./buildQMOD.ps1
```
