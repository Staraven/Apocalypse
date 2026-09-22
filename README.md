# Polygon Apocalypse

A first-person zombie wave-survival game built with Unreal Engine 5.4 and C++.

<img width="1917" height="845" alt="SS" src="https://github.com/user-attachments/assets/312615c4-4f9a-4ed1-80dd-337e393278cd" />
<img width="1896" height="1000" alt="ss2" src="https://github.com/user-attachments/assets/82cf6c87-b4b5-402b-9b34-c75e4a7ed3c8" />


## Overview

The player progresses through 7 distinct zones, each with its own wave of zombies. 
Clearing a zone unlocks a rest point, after which the next zone begins. 
The game ends with a cinematic finale at the helicopter extraction point.

## Features

- **7-Zone Progression System** — ZoneManager orchestrates zone transitions with 
  camera fade, day counter UI, and WaveManager coordination
- **3 Zombie Types** — Normal, Runner, Tank; each a Blueprint child of a parametric 
  C++ base class with Behavior Tree AI
- **Revolver Weapon System** — Hitscan with procedural bone animation (hammer, 
  trigger, cylinder), bullet depletion, reload animation, camera recoil, 
  crosshair spread, and muzzle flash
- **Weapon Architecture** — Template method pattern; WeaponBase handles hitscan/damage, 
  subclasses override PlayFireVisuals() and PlayReloadVisuals()
- **Animation Pipeline** — Mixamo → UE4 Mannequin IK Retarget workflow; 
  Anim Montage integration with Slot node for death animation
- **UI System** — HealthBar, DayCounter, Crosshair widgets owned by 
  PlayerController (separated from Character logic)
- **Day/Night Cycle** — Atmospheric visual layer, decoupled from gameplay systems

## Technical Stack

- **Engine:** Unreal Engine 5.4
- **Language:** C++
- **IDE:** JetBrains Rider
- **Assets:** Synty POLYGON Apocalypse Pack
- **Animations:** Mixamo (retargeted via IK Retargeter)

## Architecture Highlights

- `AZoneManager` — Single authority for zone progression; delegates wave completion 
  via dynamic multicast, drives camera fade and UI update chain
- `AWaveManager` — Per-zone spawn and wave tracking; loosely coupled to ZoneManager 
  via OnWaveCompleted delegate (WaveManager has no knowledge of ZoneManager)
- `AApocalypsePlayerController` — Owns all UI widgets; Character handles only 
  gameplay behavior (movement, combat, input)
- `ARevolverWeapon : AWeaponBase` — Bone animation via UPoseableMeshComponent; 
  fire/reload visuals fully encapsulated in subclass

## Project Status

Active development. Core systems complete. Remaining: attack animation, 
rest point interaction, final cinematic sequence.
