#include "patch_table.hpp"

const static uint32_t DLC_mission_size = 0x110;
const static uint32_t DLC_mission_patch_limit = 0x1000;
const static uint32_t smSampleRAMBitmapNew_size = 0x40000;
const static uint32_t matrixPool_size = 0x30D400; //provisional value
const static uint32_t lightingStatePool_size = 0x540000; //provisional value - 168 * 2048 * 16
const static uint32_t lightingStatePool_offsetSize = 0x1F8; //(168 * 3)
const static uint32_t renderItemPool_size = 0x4ED300; //provisional value - 68 * 4748 * 16

enum es_layout : uint32_t {
   ES_DLC_START = 0,
   ES_DLC_END = ES_DLC_START + (DLC_mission_size * DLC_mission_patch_limit),

   ES_SOUND_START = ES_DLC_END,
   ES_SOUND_END = ES_SOUND_START + smSampleRAMBitmapNew_size,

   ES_MATRIX_START = ES_SOUND_END,
   ES_MATRIX_END = ES_MATRIX_START + matrixPool_size,

   ES_LIGHTING_START = ES_MATRIX_END,
   ES_LIGHTING_END = ES_LIGHTING_START + lightingStatePool_size,

   ES_LIGHTINGOFFSET_START = ES_LIGHTING_END,
   ES_LIGHTINGOFFSET_END = ES_LIGHTINGOFFSET_START + lightingStatePool_offsetSize,

   ES_RENDERITEM_START = ES_LIGHTINGOFFSET_END,
   ES_RENDERITEM_END = ES_RENDERITEM_START + renderItemPool_size,

   ES_END = ES_RENDERITEM_END
};

// Function names matched from BF1 Mac executable. Could be wrong in cases.

// clang-format off

const exe_patch_list patch_lists[EXE_COUNT] = {
   exe_patch_list{
      .name = "BF2_modtools",
      .id_address = 0x62b59c,
      .expected_id = 0x746163696c707041,
      .patches =
         {
            patch_set{
               .name = "RedMemory Heap Extensions",
               .patches =
                  {
                     patch{0x337921, 0x4000000, 0x10000000}, // malloc call arg
                     patch{0x33792c, 0x4000000, 0x10000000}, // malloc'd block end pointer
                  },
            },

            patch_set{
               .name = "SoundParameterized Layer Limit Extension",
               .patches =
                  {
                     patch{0x6227c2, 0xa0, 0x2000},
                  },
            },

            patch_set{
               .name = "DLC Mission Limit Extension",
               .patches =
                  {
                     patch{0x4935c, 0xb08308, ES_DLC_START, true},                         // SetCurrentMap
                     patch{0x493ac, 0xb0830c, (0xb0830c - 0xb08308) + ES_DLC_START, true}, // SetCurrentMission
                     patch{0x49415, 0xb08310, (0xb08310 - 0xb08308) + ES_DLC_START, true}, // GetContentDirectory
                     patch{0x49472, 0xb0830c, (0xb0830c - 0xb08308) + ES_DLC_START, true}, // IsMissionDownloaded
                     patch{0x494fb, 0x1f4, DLC_mission_patch_limit, true},                 // AddDownloadableContent
                     patch{0x4951f, 0xb08308, ES_DLC_START, true},                         // AddDownloadableContent
                     patch{0x49542, 0xb0830c, (0xb0830c - 0xb08308) + ES_DLC_START, true}, // AddDownloadableContent
                     patch{0x49548, 0xb08310, (0xb08310 - 0xb08308) + ES_DLC_START, true}, // AddDownloadableContent
                     patch{0x49571, 0xb08413, (0xb08413 - 0xb08308) + ES_DLC_START, true}, // AddDownloadableContent
                     patch{0x4957d, 0xb08414, (0xb08414 - 0xb08308) + ES_DLC_START, true}, // AddDownloadableContent
                  },
            },

            patch_set{
               .name = "Sound Limit Extension",
               .patches =
                  {
                     patch{0x486ae0 + 0x1, 0x2331f08, ES_SOUND_START, true}, // Snd::Engine::Open
                     patch{0x486aea + 0x1, 0x2000000, 0x10000000},           // malloc call 1 arg
                     patch{0x486939 + 0x1, 0x2000000, 0x10000000},           // malloc call 2 arg
                  },
            },

            patch_set{
               .name = "High-Res Animation Limit Extension",
               .patches =
                  {
                     patch{0x1840c7 + 0x2, 0x32, 0x1f4, false, true},       // 10x increase
                     patch{0x1840cf + 0x1, 0x32, 0x1f4},                    // 10x increase
                     patch{0x184136 + 0x2, 0x64960, 0x1f4 * 0x2030},        // array size
                     patch{0x17e57e + 0x2, 0x64960, 0x1f4 * 0x2030},        // array size
                     patch{0x1840b3 + 0x1, 0x64970, 0x1f4 * 0x2030 + 0x10}, // heap allocation = array size - 16
                  },
            },

            patch_set{
               .name = "Matrix/Item Pool Limit Extension",
               .patches =
                  {
                     patch{0x405c0f + 0x2, 0xd64090, ES_MATRIX_START, true},
                     patch{0x405c83 + 0x2, 0xd64090, ES_MATRIX_START, true},
                     patch{0x410747 + 0x1, 0xd64090, ES_MATRIX_START, true},
                     patch{0x405c15 + 0x2, 0xbf6, matrixPool_size},
                     patch{0x405c89 + 0x2, 0xbf6, matrixPool_size},
                     patch{0x61f8b0 + 0x1, 0x320, 0x40000},          // transparentItemsSize
                     patch{0x61f8e0 + 0x1, 0x200, 0x25600},          // postTransparentItemSize
                     patch{0x61f880 + 0x1, 0x64, 0x7f, false, true}, // preShadowTransparentItemSize 8-bit signed int, needs relocating to increase it
                  },
            },

            patch_set{
               .name = "nearScene Extension",
               .patches =
                  {
                     //patch{0x398b75, 0x0, 0x1},
                  },
            },
             
            patch_set{
               .name = "renderRadius Limit Extension",
               .patches =
                  {
                     patch{0x64793c, 0x461c4000, 0x47c35000},         //RenderRadius float
                     //patch{0x417956 + 0x1, 0x461c4000, 0x47c35000}, //RedCamera::SetFrustum call
                     //patch{0x40ae17 + 0x7, 0x461c4000, 0x47c35000}, //RedRenderer::internalUpdateSceneConstants float 1
                     patch{0x674cb0, 0x461c4000, 0x47c35000},         //ProjectionVectorZ float
                     patch{0x674794, 0xc61c4000, 0xc7c35000},         //ProjectionVectorZNeg float
                  },
            },
             
            patch_set{
               .name = "LOD Limit Extension",
               .patches =
                  {
                     patch{0x41d455 + 0x1, 0xc8, 0xc8 * 0x14},        // SetClassMaxCost::MaxCount    - modelClass (0)
                     patch{0x41d450 + 0x1, 0xc350, 0xc350 * 0x14},    // SetClassMaxCost::MaxCostLOD0 - modelClass (0)
                     patch{0x41d441 + 0x1, 0x9c40, 0x9c40 * 0x14},    // SetClassMaxCost::MaxCostLOD3 - modelClass (0)

                     patch{0x41d3c0 + 0x1, 0x258, 0x258 * 0x14},      // SetClassMaxCost::MaxCount    - bigModelClass (1)
                     patch{0x41d3bb + 0x1, 0x186a0, 0x186a0 * 0x14},  // SetClassMaxCost::MaxCostLOD0 - bigModelClass (1)
                     patch{0x41d3ac + 0x1, 0x9c40, 0x9c40 * 0x14},    // SetClassMaxCost::MaxCostLOD3 - bigModelClass (1)

                     patch{0x41d38c + 0x1, 0x64, 0x7f, false, true}, // SetClassMaxCost::MaxCount    - soldierClass (2) 8-bit signed int, needs relocating to increase it
                     patch{0x41d387 + 0x1, 0x4650, 0x4650 * 0x14},    // SetClassMaxCost::MaxCostLOD0 - soldierClass (2)
                     patch{0x41d37a + 0x1, 0x9c40, 0x9c40 * 0x14},    // SetClassMaxCost::MaxCostLOD3 - soldierClass (2)

                     patch{0x41d410 + 0x1, 0x5dc, 0x5dc * 0x14},      // SetClassMaxCost::MaxCount    - hugeModelClass (3) - UBER
                     patch{0x41d40b + 0x1, 0x2710, 0x2710 * 0x14},    // SetClassMaxCost::MaxCostLOD0 - hugeModelClass (3) - UBER

                     patch{0x41d426 + 0x1, 0x12c, 0x12c * 0x14},      // SetClassMaxCost::MaxCount    - hugeModelClass (3)
                     patch{0x41d421 + 0x1, 0x3e8, 0x3e8 * 0x14},      // SetClassMaxCost::MaxCostLOD0 - hugeModelClass (3)
                     patch{0x41d3fa + 0x1, 0x9c40, 0x9c40 * 0x14},    // SetClassMaxCost::MaxCostLOD3 - hugeModelClass (3)
                  },
            },

            patch_set{
               .name = "Default Explosion VisibleRadius Increase",
               .patches =
                  {
                     patch{0x203637 + 0x6, 0x42700000, 0x461c4000}, // Increased from 60 to 10000
                  },
            },

            patch_set{
               .name = "String Pool Increase",
               .patches =
                  {
                     patch{0x4ef77 + 0x1, 0x8000, 0x20000},
                  },
            },

            patch_set{
               .name = "LightingState Pool Increase",
               .patches =
                  {
                     patch{0x405bf0 + 0x1, 0xde8068, ES_LIGHTING_END + (168 * 2), true},
                     patch{0x405ce1 + 0x2, 0xd93f18, ES_LIGHTING_START, true},
                     patch{0x405ce7 + 0x2, 0x800, 0x8000},
                     patch{0x406e75 + 0x1, 0xde8068, ES_LIGHTING_END + (168 * 2), true},
                     patch{0x408b65 + 0x2, 0xd93f18, ES_LIGHTING_START, true},
                     patch{0x40cd14 + 0x1, 0xde7f18, ES_LIGHTING_END, true},
                     patch{0x40cd20 + 0x1, 0xde7fc0, ES_LIGHTING_END + 168, true},
                     patch{0x40cd2c + 0x1, 0xde8068, ES_LIGHTING_END + (168 * 2), true},
                     patch{0x40cd63 + 0x6, 0xde7f18, ES_LIGHTING_END, true},
                     patch{0x40d199 + 0x6, 0xde7f18, ES_LIGHTING_END, true},
                     patch{0x40e9dc + 0x1, 0xde7f18, ES_LIGHTING_END, true},
                     patch{0x40e9e3 + 0x1, 0xde7fc0, ES_LIGHTING_END + 168, true},
                     patch{0x4107d7 + 0x1, 0xde7f18, ES_LIGHTING_END, true},
                     patch{0x4107de + 0x1, 0xde7fc0, ES_LIGHTING_END + 168, true},
                     patch{0x410dd2 + 0x3, 0xde7fc0, ES_LIGHTING_END + 168, true},
                     patch{0x411691 + 0x3, 0xde7fc0, ES_LIGHTING_END + 168, true},
                     patch{0x411b8f + 0x1, 0xde7f18, ES_LIGHTING_END, true},
                     patch{0x411b9b + 0x1, 0xde7fc0, ES_LIGHTING_END + 168, true},
                     patch{0x411ba7 + 0x1, 0xde8068, ES_LIGHTING_END + (168 * 2), true},
                     patch{0x411bb3 + 0x1, 0xde7f18, ES_LIGHTING_END, true},
                     patch{0x411be0 + 0x2, 0xde7f68, ES_LIGHTING_END + 80, true},
                     patch{0x411bec + 0x2, 0xde7f6c, ES_LIGHTING_END + 84, true},
                     patch{0x411bf8 + 0x2, 0xde7f74, ES_LIGHTING_END + 92, true},
                     patch{0x411c04 + 0x1, 0xde7f70, ES_LIGHTING_END + 88, true},
                     patch{0x411c0e + 0x2, 0xde7f78, ES_LIGHTING_END + 96, true},
                     patch{0x411c1a + 0x2, 0xde7f80, ES_LIGHTING_END + 104, true},
                     patch{0x411c2a + 0x1, 0xde7fc0, ES_LIGHTING_END + 168, true},
                     patch{0x411c2f + 0x1, 0xde7f7c, ES_LIGHTING_END + 100, true},
                     patch{0x411c34 + 0x2, 0xde7f84, ES_LIGHTING_END + 108, true},
                     patch{0x411c3a + 0x2, 0xde7f88, ES_LIGHTING_END + 112, true},
                     patch{0x411c56 + 0x2, 0xde8014, ES_LIGHTING_END + 252, true},
                     patch{0x411c62 + 0x1, 0xde8010, ES_LIGHTING_END + 248, true},
                     patch{0x411c6c + 0x2, 0xde8018, ES_LIGHTING_END + 256, true},
                     patch{0x411c78 + 0x2, 0xde8020, ES_LIGHTING_END + 264, true},
                     patch{0x411c84 + 0x1, 0xde801c, ES_LIGHTING_END + 260, true},
                     patch{0x411c8e + 0x2, 0xde8024, ES_LIGHTING_END + 268, true},
                     patch{0x411c9a + 0x2, 0xde802c, ES_LIGHTING_END + 276, true},
                     patch{0x411ca9 + 0x1, 0xde8028, ES_LIGHTING_END + 272, true},
                     patch{0x411cae + 0x2, 0xde8030, ES_LIGHTING_END + 280, true},
                     patch{0x411cb4 + 0x6, 0xde7f18, ES_LIGHTING_END, true},
                     patch{0x411f3a + 0x6, 0xde7f18, ES_LIGHTING_END, true},

                     patch{0x61f94a + 0x1, 0x803, 0x8003},
                     patch{0x61f954 + 0x1, 0xd93f18, ES_LIGHTING_START, true},
                     patch{0x627a65 + 0x1, 0x803, 0x8003},
                     patch{0x627a6f + 0x1, 0xd93f18, ES_LIGHTING_START, true},
                  },
            },

            patch_set{
               .name = "RenderItem Pool Increase",
               .patches =
                  {
                     patch{0x410619 + 0x6, 0x128c, 0x128c0},
                     patch{0x4106d5 + 0x3, 0xde81c4, ES_RENDERITEM_START + 52, true},
                     patch{0x4106dc + 0x3, 0xde8194, ES_RENDERITEM_START + 4, true},
                     patch{0x4106e9 + 0x3, 0xde8190, ES_RENDERITEM_START, true},
                     patch{0x410d0f + 0x3, 0xde8190, ES_RENDERITEM_START, true},
                     patch{0x41165a + 0x3, 0xde8190, ES_RENDERITEM_START, true},
                     patch{0x61f91a + 0x1, 0x128c, 0x128c0},
                     patch{0x61f921 + 0x1, 0xde8190, ES_RENDERITEM_START, true},
                     patch{0x627a45 + 0x1, 0x128c, 0x128c0},
                     patch{0x627a4c + 0x1, 0xde8190, ES_RENDERITEM_START, true},
                  },
            },
         },
   },
   
   exe_patch_list{
      .name = "BattlefrontII.exe GoG",
      .id_address = 0x39f298,
      .expected_id = 0x746163696c707041,
      .patches =
         {
            patch_set{
               .name = "RedMemory Heap Extensions",
               .patches =
                  {
                     patch{0x217651, 0x4000000, 0x10000000}, // malloc call arg
                     patch{0x217667, 0x4000000, 0x10000000}, // malloc'd block end pointer
                  },
            },

            patch_set{
               .name = "SoundParameterized Layer Limit Extension",
               .patches =
                  {
                     patch{0x3e310c, 0xa0, 0x2000},
                  },
            },

            patch_set{
               .name = "DLC Mission Limit Extension",
               .patches =
                  {
                     patch{0x8de7d, 0x1f4, DLC_mission_patch_limit, true},                    // AddDownloadableContent
                     patch{0x8de9f, 0x1e31f00, ES_DLC_START, true},                           // AddDownloadableContent
                     patch{0x8dec3, 0x1e31f04, (0x1e31f04 - 0x1e31f00) + ES_DLC_START, true}, // AddDownloadableContent
                     patch{0x8dec9, 0x1e31f08, (0x1e31f08 - 0x1e31f00) + ES_DLC_START, true}, // AddDownloadableContent
                     patch{0x8def0, 0x1e3200b, (0x1e3200b - 0x1e31f00) + ES_DLC_START, true}, // AddDownloadableContent
                     patch{0x8def7, 0x1e3200c, (0x1e3200c - 0x1e31f00) + ES_DLC_START, true}, // AddDownloadableContent
                     patch{0x8df28, 0x1e31f00, ES_DLC_START, true},                           // SetCurrentMap
                     patch{0x8df68, 0x1e31f04, (0x1e31f04 - 0x1e31f00) + ES_DLC_START, true}, // SetCurrentMission
                     patch{0x8dfb4, 0x1e31f08, (0x1e31f08 - 0x1e31f00) + ES_DLC_START, true}, // GetContentDirectory
                     patch{0x8dfce, 0x1e31f04, (0x1e31f04 - 0x1e31f00) + ES_DLC_START, true}, // IsMissionDownloaded
                  },
            },

            patch_set{
               .name = "Sound Limit Extension",
               .patches =
                  {
                     patch{0x332aa2 + 0x1, 0x9d1258, ES_SOUND_START, true}, // Snd::Engine::Open
                     patch{0x332aac + 0x1, 0x2000000, 0x10000000},          // malloc call 1 arg
                     patch{0x3328e7 + 0x1, 0x2000000, 0x10000000},          // malloc call 2 arg
                  },
            },

            patch_set{
               .name = "High-res Animation Limit Extension",
               .patches =
                  {
                     patch{0x247872 + 0x1, 0x32, 0x7f, false, true}, // 1-byte signed int, needs relocating to increase it - 127 max
                     patch{0x247877 + 0x2, 0x32, 0x7f},              // 4-byte signed int, must match above
                     patch{0x2478d9 + 0x2, 0x64640, 0xfefe0},        // array size 0x7f * 0x2020
                     patch{0x243d02 + 0x1, 0x64640, 0xfefe0},        // array size 0x7f * 0x2020
                     patch{0x247850 + 0x1, 0x64650, 0xfeff0},        // heap allocation = array size + 16 0x7f * 0x2020 + 0x10
                  },
            },

            patch_set{
               .name = "Matrix/Item Pool Limit Extension",
               .patches =
                  {
                     patch{0x2b0702 + 0x1, 0x8c03f0, ES_MATRIX_START, true},
                     patch{0x2b8e37 + 0x2, 0x8c03f0, ES_MATRIX_START, true},
                     patch{0x2b076f + 0x2, 0x8c03f0, ES_MATRIX_START, true},
                     patch{0x6992 + 0x1, 0x8c03f0, ES_MATRIX_START, true},
                     patch{0x2b070a + 0x2, 0xbf6, matrixPool_size},
                     patch{0x2b0778 + 0x1, 0xbf6, matrixPool_size},
                     patch{0x6997 + 0x1, 0xbf5, matrixPool_size - 1},
                     patch{0x6b10 + 0x1, 0x320, 0x40000},          // transparentItemsSize
                     patch{0x6a80 + 0x1, 0x200, 0x25600},          // postTransparentItemSize
                     patch{0x6ab0 + 0x1, 0x64, 0x7f, false, true}, // preShadowTransparentItemSize 8-bit signed int, needs relocating to increase it
                  },
            },

            patch_set{
               .name = "nearScene Extension",
               .patches =
                  {
                     patch{0x2bf5c3 + 0x6, 0x0, 0x1, false, true},
                  },
            },

            patch_set{
               .name = "renderRadius Limit Extension",
               .patches =
                  {
                     //patch{0x64793c, 0x461c4000, 0x47c35000},       //RenderRadius float - not required for main exe
                     //patch{0x417956 + 0x1, 0x461c4000, 0x47c35000}, //RedCamera::SetFrustum call
                     //patch{0x40ae17 + 0x7, 0x461c4000, 0x47c35000}, //RedRenderer::internalUpdateSceneConstants float 1
                     patch{0x3b1f94, 0x461c4000, 0x47c35000},         //ProjectionVectorZ float
                     patch{0x3722fc, 0xc61c4000, 0xc7c35000},         //ProjectionVectorZNeg float
                  },
            },

            patch_set{
               .name = "LOD Limit Extension",
               .patches =
                  {
                     patch{0x2bcd59 + 0x1, 0xc8, 0xc8 * 0xa},        // SetClassMaxCost::MaxCount    - modelClass (0)
                     patch{0x2bcd54 + 0x1, 0xc350, 0xc350 * 0xa},    // SetClassMaxCost::MaxCostLOD0 - modelClass (0)
                     patch{0x2bcd45 + 0x1, 0x9c40, 0x9c40 * 0xa},    // SetClassMaxCost::MaxCostLOD3 - modelClass (0)
            
                     patch{0x2bccc1 + 0x1, 0x258, 0x258 * 0xa},      // SetClassMaxCost::MaxCount    - bigModelClass (1)
                     patch{0x2bccbc + 0x1, 0x186a0, 0x186a0 * 0xa},  // SetClassMaxCost::MaxCostLOD0 - bigModelClass (1)
                     patch{0x2bccad + 0x1, 0x9c40, 0x9c40 * 0xa},    // SetClassMaxCost::MaxCostLOD3 - bigModelClass (1)
            
                     patch{0x2bcc86 + 0x1, 0x64, 0x7f, false, true}, // SetClassMaxCost::MaxCount    - soldierClass (2) 8-bit signed int, needs relocating to increase it
                     patch{0x2bcc81 + 0x1, 0x4650, 0x4650 * 0xa},    // SetClassMaxCost::MaxCostLOD0 - soldierClass (2)
                     patch{0x2bcc75 + 0x1, 0x9c40, 0x9c40 * 0xa},    // SetClassMaxCost::MaxCostLOD3 - soldierClass (2)
            
                     patch{0x2bcd11 + 0x1, 0x5dc, 0x5dc * 0xa},      // SetClassMaxCost::MaxCount    - hugeModelClass (3) - UBER
                     patch{0x2bcd0c + 0x1, 0x2710, 0x2710 * 0xa},    // SetClassMaxCost::MaxCostLOD0 - hugeModelClass (3) - UBER
            
                     patch{0x2bcd27 + 0x1, 0x12c, 0x12c * 0xa},      // SetClassMaxCost::MaxCount    - hugeModelClass (3)
                     patch{0x2bcd22 + 0x1, 0x3e8, 0x3e8 * 0xa},      // SetClassMaxCost::MaxCostLOD0 - hugeModelClass (3)
                     patch{0x2bccfb + 0x1, 0x9c40, 0x9c40 * 0xa},    // SetClassMaxCost::MaxCostLOD3 - hugeModelClass (3)
                  },
            },

            patch_set{
               .name = "Default Explosion VisibleRadius Increase",
               .patches =
                  {
                     patch{0x11bf59 + 0x6, 0x42700000, 0x461c4000}, // Increased from 60 to 10000
                  },
            },

            patch_set{
               .name = "String Pool Increase",
               .patches =
                  {
                     patch{0x13b293 + 0x1, 0x1770, 0x20000},
                  },
            },

            patch_set{
               .name = "LightingState Pool Increase",
               .patches =
                  {
                     patch{0x696a + 0x1, 0x803, 0x8003},
                     patch{0x6974 + 0x1, 0x86c1b0, ES_LIGHTING_START, true},
                     patch{0x2b0737 + 0x2, 0x86c1b0, ES_LIGHTING_START, true},
                     patch{0x2b073d + 0x1, 0x800, 0x8000},
                     patch{0x2b3c07 + 0x2, 0x86c1b0, ES_LIGHTING_START, true},
                     patch{0x369ea5 + 0x1, 0x803, 0x8003},
                     patch{0x369eaf + 0x1, 0x86c1b0, ES_LIGHTING_START, true},
                  },
            },

            patch_set{
               .name = "RenderItem Pool Increase",
               .patches =
                  {
                     patch{0x6aea + 0x1, 0x128c, 0x128c0},
                     patch{0x6af1 + 0x1, 0x818990, ES_RENDERITEM_START, true},
                     patch{0x2b1226 + 0x3, 0x818990, ES_RENDERITEM_START, true},
                     patch{0x2b8db6 + 0x2, 0x128c, 0x128c0},
                     patch{0x2b8dd2 + 0x3, 0x818990, ES_RENDERITEM_START, true},
                     patch{0x2b945a + 0x3, 0x818990, ES_RENDERITEM_START, true},
                     patch{0x369ef5 + 0x1, 0x128c, 0x128c0},
                     patch{0x369efc + 0x1, 0x818990, ES_RENDERITEM_START, true},
                  },
            },
         },
   },

   exe_patch_list{
      .name = "BattlefrontII.exe Steam",
      .id_address = 0x39e234,
      .expected_id = 0x746163696c707041,
      .patches =
         {
            patch_set{
               .name = "RedMemory Heap Extensions",
               .patches =
                  {
                     patch{0x2165b1, 0x4000000, 0x10000000}, // malloc call arg
                     patch{0x2165c7, 0x4000000, 0x10000000}, // malloc'd block end pointer
                  },
            },
            
            patch_set{
               .name = "SoundParameterized Layer Limit Extension",
               .patches =
                  {
                     patch{0x3e170c, 0xa0, 0x2000},
                  },
            },

            patch_set{
               .name = "DLC Mission Limit Extension",
               .patches =
                  {
                     patch{0x8de7d, 0x1f4, DLC_mission_patch_limit, true},                    // AddDownloadableContent
                     patch{0x8de9f, 0x1e30950, ES_DLC_START, true},                           // AddDownloadableContent
                     patch{0x8dec3, 0x1e30954, (0x1e30954 - 0x1e30950) + ES_DLC_START, true}, // AddDownloadableContent
                     patch{0x8dec9, 0x1e30958, (0x1e30958 - 0x1e30950) + ES_DLC_START, true}, // AddDownloadableContent
                     patch{0x8def0, 0x1e30a5b, (0x1e30a5b - 0x1e30950) + ES_DLC_START, true}, // AddDownloadableContent
                     patch{0x8def7, 0x1e30a5c, (0x1e30a5c - 0x1e30950) + ES_DLC_START, true}, // AddDownloadableContent
                     patch{0x8df28, 0x1e30950, ES_DLC_START, true},                           // SetCurrentMap
                     patch{0x8df68, 0x1e30954, (0x1e30954 - 0x1e30950) + ES_DLC_START, true}, // SetCurrentMission
                     patch{0x8dfb4, 0x1e30958, (0x1e30958 - 0x1e30950) + ES_DLC_START, true}, // GetContentDirectory
                     patch{0x8dfce, 0x1e30954, (0x1e30954 - 0x1e30950) + ES_DLC_START, true}, // IsMissionDownloaded
                  },
            },

            patch_set{
               .name = "Sound Limit Extension",
               .patches =
                  {
                     patch{0x3319b2 + 0x1, 0x9cfdb8, ES_SOUND_START, true}, // Snd::Engine::Open
                     patch{0x3319bc + 0x1, 0x2000000, 0x10000000},          // malloc call 1 arg
                     patch{0x3317f7 + 0x1, 0x2000000, 0x10000000},          // malloc call 2 arg
                  },
            },

            patch_set{
               .name = "High-Res Animation Limit Extension",
               .patches =
                  {
                     patch{0x2467d2 + 0x1, 0x32, 0x7f, false, true}, // 8-bit signed int, needs relocating to increase it - 127 max
                     patch{0x2467d7 + 0x2, 0x32, 0x7f},              // 32-bit signed int, must match above
                     patch{0x246839 + 0x2, 0x64640, 0xfefe0},        // array size 0x7f * 0x2020
                     patch{0x242c62 + 0x1, 0x64640, 0xfefe0},        // array size 0x7f * 0x2020
                     patch{0x2467b0 + 0x1, 0x64650, 0xfeff0},        // heap allocation = array size + 16 0x7f * 0x2020 + 0x10
                  },
            },

            patch_set{
               .name = "Matrix/Item Pool Limit Extension",
               .patches =
                  {
                     patch{0x2af682 + 0x1, 0x8bef50, ES_MATRIX_START, true},
                     patch{0x2af6ef + 0x2, 0x8bef50, ES_MATRIX_START, true},
                     patch{0x2b7da7 + 0x2, 0x8bef50, ES_MATRIX_START, true},
                     patch{0x6992 + 0x1, 0x8bef50, ES_MATRIX_START, true},
                     patch{0x2af68a + 0x2, 0xbf6, matrixPool_size},
                     patch{0x2af6f8 + 0x1, 0xbf6, matrixPool_size},
                     patch{0x6997 + 0x1, 0xbf5, matrixPool_size - 1},
                     patch{0x6b10 + 0x1, 0x320, 0x40000},          // transparentItemsSize
                     patch{0x6a80 + 0x1, 0x200, 0x25600},          // postTransparentItemSize
                     patch{0x6ab0 + 0x1, 0x64, 0x7f, false, true}, // preShadowTransparentItemSize 8-bit signed int, needs relocating to increase it
                  },
            },

            patch_set{
               .name = "nearScene Extension",
               .patches =
                  {
                     patch{0x2be533 + 0x6, 0x0, 0x1, false, true},
                  },
            },

            patch_set{
               .name = "renderRadius Limit Extension",
               .patches =
                  {
                     //patch{0x64793c, 0x461c4000, 0x47c35000},       //RenderRadius float - not required for main exe
                     //patch{0x417956 + 0x1, 0x461c4000, 0x47c35000}, //RedCamera::SetFrustum call
                     //patch{0x40ae17 + 0x7, 0x461c4000, 0x47c35000}, //RedRenderer::internalUpdateSceneConstants float 1
                     patch{0x3b0e1c, 0x461c4000, 0x47c35000},         //ProjectionVectorZ float
                     patch{0x37114c, 0xc61c4000, 0xc7c35000},         //ProjectionVectorZNeg float
                  },
            },
            patch_set{
               .name = "LOD Limit Extension",
               .patches =
                  {
                     patch{0x2bbcc9 + 0x1, 0xc8, 0xc8 * 0xa},        // SetClassMaxCost::MaxCount    - modelClass (0)
                     patch{0x2bbcc4 + 0x1, 0xc350, 0xc350 * 0xa},    // SetClassMaxCost::MaxCostLOD0 - modelClass (0)
                     patch{0x2bbcb5 + 0x1, 0x9c40, 0x9c40 * 0xa},    // SetClassMaxCost::MaxCostLOD3 - modelClass (0)

                     patch{0x2bbc31 + 0x1, 0x258, 0x258 * 0xa},      // SetClassMaxCost::MaxCount    - bigModelClass (1)
                     patch{0x2bbc2c + 0x1, 0x186a0, 0x186a0 * 0xa},  // SetClassMaxCost::MaxCostLOD0 - bigModelClass (1)
                     patch{0x2bbc1d + 0x1, 0x9c40, 0x9c40 * 0xa},    // SetClassMaxCost::MaxCostLOD3 - bigModelClass (1)

                     patch{0x2bbbf6 + 0x1, 0x64, 0x7f, false, true}, // SetClassMaxCost::MaxCount    - soldierClass (2) 8-bit signed int, needs relocating to increase it
                     patch{0x2bbbf1 + 0x1, 0x4650, 0x4650 * 0xa},    // SetClassMaxCost::MaxCostLOD0 - soldierClass (2)
                     patch{0x2bbbe5 + 0x1, 0x9c40, 0x9c40 * 0xa},    // SetClassMaxCost::MaxCostLOD3 - soldierClass (2)

                     patch{0x2bbc81 + 0x1, 0x5dc, 0x5dc * 0xa},      // SetClassMaxCost::MaxCount    - hugeModelClass (3) - UBER
                     patch{0x2bbc7c + 0x1, 0x2710, 0x2710 * 0xa},    // SetClassMaxCost::MaxCostLOD0 - hugeModelClass (3) - UBER

                     patch{0x2bbc97 + 0x1, 0x12c, 0x12c * 0xa},      // SetClassMaxCost::MaxCount    - hugeModelClass (3)
                     patch{0x2bbc92 + 0x1, 0x3e8, 0x3e8 * 0xa},      // SetClassMaxCost::MaxCostLOD0 - hugeModelClass (3)
                     patch{0x2bbc6b + 0x1, 0x9c40, 0x9c40 * 0xa},    // SetClassMaxCost::MaxCostLOD3 - hugeModelClass (3)
                  },
            },

            patch_set{
               .name = "Default Explosion VisibleRadius Increase",
               .patches =
                  {
                     patch{0x11bf59 + 0x6, 0x42700000, 0x461c4000}, // Increased from 60 to 10000
                  },
            },

            patch_set{
               .name = "String Pool Increase",
               .patches =
                  {
                     patch{0x13a543 + 0x1, 0x1770, 0x20000},
                  },
            },

            patch_set{
               .name = "LightingState Pool Increase",
               .patches =
                  {
                     patch{0x696a + 0x1, 0x803, 0x8003},
                     patch{0x6974 + 0x1, 0x86ad10, ES_LIGHTING_START, true},
                     patch{0x2af6b7 + 0x2, 0x86ad10, ES_LIGHTING_START, true},
                     patch{0x2af6bd + 0x1, 0x800, 0x8000},
                     patch{0x2b2b77 + 0x2, 0x86ad10, ES_LIGHTING_START, true},
                     patch{0x368bf5 + 0x1, 0x803, 0x8003},
                     patch{0x368bff + 0x1, 0x86ad10, ES_LIGHTING_START, true},
                  },
            },

            patch_set{
               .name = "RenderItem Pool Increase",
               .patches =
                  {
                     patch{0x6aea + 0x1, 0x128c, 0x128c0},
                     patch{0x6af1 + 0x1, 0x8174f0, ES_RENDERITEM_START, true},
                     patch{0x2b7d26 + 0x2, 0x128c, 0x128c0},
                     patch{0x2b01a6 + 0x3, 0x8174f0, ES_RENDERITEM_START, true},
                     patch{0x2b7d42 + 0x3, 0x8174f0, ES_RENDERITEM_START, true},
                     patch{0x2b83ca + 0x3, 0x8174f0, ES_RENDERITEM_START, true},
                     patch{0x368c45 + 0x1, 0x128c, 0x128c0},
                     patch{0x368c4c + 0x1, 0x8174f0, ES_RENDERITEM_START, true},
                  },
            },
         },
   },

   exe_patch_list{
      .name = "BattlefrontII.exe DVD",
      .id_address = 0x3bf12c,
      .expected_id = 0x746163696c707041,
      .patches =
         {
            patch_set{
               .name = "RedMemory Heap Extensions",
               .patches =
                  {
                     patch{0x221a60 + 0x1, 0x4000000, 0x10000000}, // malloc call arg
                     patch{0x221a6c + 0x2, 0x4000000, 0x10000000}, // malloc'd block end pointer
                  },
            },
            
            patch_set{
               .name = "SoundParameterized Layer Limit Extension",
               .patches =
                  {
                     patch{0x45b148, 0xa0, 0x2000},
                  },
            },

            //patch_set{
            //   .name = "DLC Mission Limit Extension",
            //   .patches =
            //      {
            //         patch{0x8de7d, 0x1f4, DLC_mission_patch_limit, true},                    // AddDownloadableContent
            //         patch{0x8de9f, 0x1e30950, ES_DLC_START, true},                           // AddDownloadableContent
            //         patch{0x8dec3, 0x1e30954, (0x1e30954 - 0x1e30950) + ES_DLC_START, true}, // AddDownloadableContent
            //         patch{0x8dec9, 0x1e30958, (0x1e30958 - 0x1e30950) + ES_DLC_START, true}, // AddDownloadableContent
            //         patch{0x8def0, 0x1e30a5b, (0x1e30a5b - 0x1e30950) + ES_DLC_START, true}, // AddDownloadableContent
            //         patch{0x8def7, 0x1e30a5c, (0x1e30a5c - 0x1e30950) + ES_DLC_START, true}, // AddDownloadableContent
            //         patch{0x8df28, 0x1e30950, ES_DLC_START, true},                           // SetCurrentMap
            //         patch{0x8df68, 0x1e30954, (0x1e30954 - 0x1e30950) + ES_DLC_START, true}, // SetCurrentMission
            //         patch{0x8dfb4, 0x1e30958, (0x1e30958 - 0x1e30950) + ES_DLC_START, true}, // GetContentDirectory
            //         patch{0x8dfce, 0x1e30954, (0x1e30954 - 0x1e30950) + ES_DLC_START, true}, // IsMissionDownloaded
            //      },
            //},

            patch_set{
               .name = "Sound Limit Extension",
               .patches =
                  {
                     patch{0x271f24 + 0x6, 0x8000, 0x40000},       // Snd::Engine::Open
                     patch{0x271eff + 0x1, 0x2000, 0x10000},       // Snd::Engine::Open - possibly unnecessary
                     patch{0x271d8b + 0x1, 0x2000000, 0x10000000}, // malloc call 1 arg
                     patch{0x271f0a + 0x6, 0x2000000, 0x10000000}, // malloc call 2 arg
                  },
            },

            patch_set{
               .name = "High-Res Animation Limit Extension",
               .patches =
                  {
                     patch{0xf3ca9 + 0x1, 0x32, 0x7f},       // 32-bit signed int
                     patch{0xf3ca1 + 0x2, 0x32, 0x7f},       // 32-bit signed int
                     patch{0xf0186 + 0x2, 0x64640, 0xfefe0}, // array size 0x7f * 0x2020
                     patch{0xf3b24 + 0x2, 0x64640, 0xfefe0}, // array size 0x7f * 0x2020
                     patch{0xf3ab6 + 0x1, 0x64650, 0xfeff0}, // heap allocation = array size + 16 0x7f * 0x2020 + 0x10
                  },
            },

            patch_set{
               .name = "Matrix/Item Pool Limit Extension",
               .patches =
                  {
                     patch{0x1d1999 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x29bd1c + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x29d350 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x29d37e + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x29dd31 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x29dde0 + 0x1, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x29df9b + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x29e265 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2a5380 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2a9341 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2a979d + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2a9876 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2aa473 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2ab279 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2abf62 + 0x1, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2b8b89 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2b8ba2 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2b8bbb + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2b8bc5 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2b8cd4 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2b8cee + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2b8cf8 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2b8d04 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2b8d70 + 0x1, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2b8d89 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2b8d93 + 0x1, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2b8d9e + 0x1, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2b8e55 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2b8e81 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2cd752 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2cd772 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2cd879 + 0x2, 0x1d71984, ES_MATRIX_START, true},
                     patch{0x2cd8a2 + 0x2, 0x1d71984, ES_MATRIX_START, true},

                     patch{0x2b8d82 + 0x1, 0xbf6, matrixPool_size},
                     patch{0x29d363 + 0x2, 0xbf6, matrixPool_size},
                     patch{0x2b8b9a + 0x2, 0xbf6, matrixPool_size},
                     patch{0x2b8e68 + 0x2, 0xbf6, matrixPool_size},
                     patch{0x2cd88c + 0x2, 0xbf6, matrixPool_size},
                     patch{0x2cd766 + 0x2, 0xbf6, matrixPool_size},
                     patch{0x2b8ce6 + 0x2, 0xbf6, matrixPool_size},
                     //patch{0x3834b1 + 0x1, 0x320, 0x40000},          // transparentItemsSize
                     //patch{0x384ce0 + 0x1, 0x200, 0x25600},          // postTransparentItemSize
                     //patch{0x6ab0 + 0x1, 0x64, 0x7f, false, true},   // preShadowTransparentItemSize 8-bit signed int, needs relocating to increase it
                  },
            },

            patch_set{
               .name = "nearScene Extension",
               .patches =
                  {
                     patch{0x2a9d30 + 0x1, 0x51, 0x1, false, true}, // patch instruction to move 0x1 into AL vs nearScene pointer
                     patch{0x2a9d30 + 0x2, 0x92, 0x0, false, true},
                     patch{0x2a9d30 + 0x3, 0xd7, 0x0, false, true},
                     patch{0x2a9d30 + 0x4, 0x01, 0x0, false, true},
                  },
            },

            patch_set{
               .name = "LOD Limit Extension",
               .patches =
                  {
                     patch{0x2a8881 + 0x1, 0xc8, 0xc8 * 0xa},        // SetClassMaxCost::MaxCount    - modelClass (0)
                     patch{0x2a888b + 0x1, 0xc350, 0xc350 * 0xa},    // SetClassMaxCost::MaxCostLOD0 - modelClass (0)
                     patch{0x2a8874 + 0x1, 0x9c40, 0x9c40 * 0xa},    // SetClassMaxCost::MaxCostLOD3 - modelClass (0)

                     patch{0x2a87f4 + 0x1, 0x258, 0x258 * 0xa},      // SetClassMaxCost::MaxCount    - bigModelClass (1)
                     patch{0x2a87fe + 0x1, 0x186a0, 0x186a0 * 0xa},  // SetClassMaxCost::MaxCostLOD0 - bigModelClass (1)
                     patch{0x2a87d3 + 0x1, 0x9c40, 0x9c40 * 0xa},    // SetClassMaxCost::MaxCostLOD3 - bigModelClass (1)

                     patch{0x2a87bb + 0x1, 0x64, 0x7f, false, true}, // SetClassMaxCost::MaxCount    - soldierClass (2) 8-bit signed int, needs relocating to increase it
                     patch{0x2a87bf + 0x1, 0x4650, 0x4650 * 0xa},    // SetClassMaxCost::MaxCostLOD0 - soldierClass (2)
                     patch{0x2a87b1 + 0x1, 0x9c40, 0x9c40 * 0xa},    // SetClassMaxCost::MaxCostLOD3 - soldierClass (2)

                     patch{0x2a884d + 0x1, 0x5dc, 0x5dc * 0xa},      // SetClassMaxCost::MaxCount    - hugeModelClass (3) - UBER
                     patch{0x2a8843 + 0x1, 0x2710, 0x2710 * 0xa},    // SetClassMaxCost::MaxCostLOD0 - hugeModelClass (3) - UBER
                     patch{0x2a8848 + 0x1, 0x2710, 0x2710 * 0xa},    // SetClassMaxCost::MaxCostLOD0 - hugeModelClass (3) - UBER

                     patch{0x2a8860 + 0x1, 0x12c, 0x12c * 0xa},      // SetClassMaxCost::MaxCount    - hugeModelClass (3)
                     patch{0x2a8856 + 0x1, 0x3e8, 0x3e8 * 0xa},      // SetClassMaxCost::MaxCostLOD0 - hugeModelClass (3)
                     patch{0x2a885b + 0x1, 0x3e8, 0x3e8 * 0xa},      // SetClassMaxCost::MaxCostLOD0 - hugeModelClass (3)
                     patch{0x2a8837 + 0x1, 0x9c40, 0x9c40 * 0xa},    // SetClassMaxCost::MaxCostLOD3 - hugeModelClass (3)
                  },
            },

            patch_set{
               .name = "Default Explosion VisibleRadius Increase",
               .patches =
                  {
                     patch{0x143ca3 + 0x6, 0x42700000, 0x461c4000}, // Increased from 60 to 10000
                  },
            },
         },
   },
};

extern const uint32_t ext_section_size = ES_END;
