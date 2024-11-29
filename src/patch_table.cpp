#include "patch_table.hpp"

const static uint32_t DLC_mission_size = 0x110;
const static uint32_t DLC_mission_patch_limit = 0x1000;
const static uint32_t smSampleRAMBitmapNew_size = 0x40000;
const static uint32_t matrixPool_size = 0x30D400; //provisional value

enum es_layout : uint32_t {
   ES_DLC_START = 0,
   ES_DLC_END = ES_DLC_START + (DLC_mission_size * DLC_mission_patch_limit),

   ES_SOUND_START = ES_DLC_END,
   ES_SOUND_END = ES_SOUND_START + smSampleRAMBitmapNew_size,

   ES_MATRIX_START = ES_SOUND_END,
   ES_MATRIX_END = ES_MATRIX_START + matrixPool_size,

   ES_END = ES_MATRIX_END
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

            //patch_set{
            //   .name = "nearScene Extension",
            //   .patches =
            //      {
            //         patch{0x2bf5c3 + 0x6, 0x0, 0x1},         excluded as it can be toggled via console!
            //      },
            //},
             
            patch_set{
               .name = "LOD Limit Extension",
               .patches =
                  {
                     patch{0x41d455 + 0x1, 0xc8, 0xc8 * 0xa},        // SetClassMaxCost::MaxCount    - modelClass (0)
                     patch{0x41d450 + 0x1, 0xc350, 0xc350 * 0xa},    // SetClassMaxCost::MaxCostLOD0 - modelClass (0)
                     patch{0x41d441 + 0x1, 0x9c40, 0x9c40 * 0xa},    // SetClassMaxCost::MaxCostLOD3 - modelClass (0)

                     patch{0x41d3c0 + 0x1, 0x258, 0x258 * 0xa},      // SetClassMaxCost::MaxCount    - bigModelClass (1)
                     patch{0x41d3bb + 0x1, 0x186a0, 0x186a0 * 0xa},  // SetClassMaxCost::MaxCostLOD0 - bigModelClass (1)
                     patch{0x41d3ac + 0x1, 0x9c40, 0x9c40 * 0xa},    // SetClassMaxCost::MaxCostLOD3 - bigModelClass (1)

                     patch{0x41d38c + 0x1, 0x64, 0x7f, false, true}, // SetClassMaxCost::MaxCount    - soldierClass (2) 8-bit signed int, needs relocating to increase it
                     patch{0x41d387 + 0x1, 0x4650, 0x4650 * 0xa},    // SetClassMaxCost::MaxCostLOD0 - soldierClass (2)
                     patch{0x41d37a + 0x1, 0x9c40, 0x9c40 * 0xa},    // SetClassMaxCost::MaxCostLOD3 - soldierClass (2)

                     patch{0x41d410 + 0x1, 0x5dc, 0x5dc * 0xa},      // SetClassMaxCost::MaxCount    - hugeModelClass (3) - UBER
                     patch{0x41d40b + 0x1, 0x2710, 0x2710 * 0xa},    // SetClassMaxCost::MaxCostLOD0 - hugeModelClass (3) - UBER

                     patch{0x41d426 + 0x1, 0x12c, 0x12c * 0xa},      // SetClassMaxCost::MaxCount    - hugeModelClass (3)
                     patch{0x41d421 + 0x1, 0x3e8, 0x3e8 * 0xa},      // SetClassMaxCost::MaxCostLOD0 - hugeModelClass (3)
                     patch{0x41d3fa + 0x1, 0x9c40, 0x9c40 * 0xa},    // SetClassMaxCost::MaxCostLOD3 - hugeModelClass (3)
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
         },
   },
};

extern const uint32_t ext_section_size = ES_END;
