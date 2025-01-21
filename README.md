Tool for patching SWBF2 (2005) to extend it's memory limits.

## Current Patches

- DLC Mission Limit Extension. Increased from 500 to 4096. This lets more mods be installed at once, provided that don't conflict and no other limits are hit.
- Runtime Heap Extension. This makes it **much** harder for modders to hit "Allocating X bytes failed - no free blocks left in Heap 5 (Runtime)" errors.
- SoundParameterized Layer Limit Extension. This keeps maps with lot's of flyers and entities that use `EngineSound` from crashing.
- Sound Limit Extension. Increases sound memory to about 250MB
- High-Res Animation Limit Extension. Enables more high-res soldiers on screen at once (limited currently by the codes use of an 8-bit integer)
- Matrix/Item Pool Limit Extension. Matrix pool (and related) extension so the above patch doesn't cause a crash
- nearScene Extension. Massively increased nearScene (100000 is used on every map)
- LOD Limit Extension. Increased LOD threshold caps (these may be polygon limits, unsure ATM)
- Default Explosion VisibleRadius Increase. Increased the explosion view distance to around 10000, though this is overridden if specified in an ODF
- RenderRadius Increase. Stops objects being cut off at large distances (roughly 10,000 game units)
- String Pool Increase. Mainly for the dbug exe, should stop related crashes.
- LightingState Pool Limit Extension. Lighting pool is now bigger, and maps can have more lights without crashing.
- RenderItem Pool Limit Extension. RenderItem pool is nnow bigger, didn't cause a crash in the first place but did cause graphical issues.

## Supported Versions

- [GoG Version](https://www.gog.com/en/game/star_wars_battlefront_ii)
- [Steam Version](https://store.steampowered.com/app/6060)
- BF2_modtools (The version of the game used to debug mods. Found in the modtools.)

If you're interested in seeing another version of the game supported feel free to open an Issue (or +1 an Issue if someone else has already asked for your version to be supported).


## Usage

The tool itself is a simple Win32 GUI app. Launch it, click "Patch Executable", browse to your game's executable (the one named `BattlefrontII.exe` and is in the same folder as your `Addon` folder) and click Open. The tool will then patch the executable, if it recognizes the executable and is able to patch it you'll get a success message.

If it fails the executable will left unmodified. Replacing it is the final step it does after everything else has succeeded.

## Credits

- Sleepy - For the patcher codebase and initial patches (DLC extension, runtime and soundlayer fix).
- Abkarino & Antstafer - For letting me get a look at their EGM patcher source before it went public, enabling me to easily transfer some of their patches over to BF2 (high-res soldier and matrix pool patches).
- S1thK3nny and various others (please let me know if you've tested in the past, as I've forgotten 💀) - Bug finding and stress testing.
