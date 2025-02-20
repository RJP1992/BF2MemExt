#include "pch.h"

#include "patch_table.hpp"

const static uint32_t DLC_mission_size = 0x110;
const static uint32_t DLC_mission_patch_limit = 0x1000;

const static uint32_t smSampleRAMBitmapNew_size = 0x40000;

const static uint32_t matrixPool_size = 0x30D400; //provisional value

const static uint32_t lightingStatePool_base_size = 0x540000; //provisional value - 168 * 2048 * 16
const static uint32_t lightingStatePoolOffset_size = 0x1F8; //(168 * 3)
const static uint32_t lightingStatePool_total_size = lightingStatePool_base_size + lightingStatePoolOffset_size;

const static uint32_t renderItemPool_size = 0x4ED300; //provisional value - 68 * 4748 * 16

static char DLC_mission_table_storage[DLC_mission_size * DLC_mission_patch_limit] = {};
static const uint32_t DLC_mission_table_address = (uint32_t)&DLC_mission_table_storage[0];

static char smSampleRAMBitmapNew_storage[smSampleRAMBitmapNew_size] = {};
static uint32_t smSampleRAMBitmapNew_address = (uint32_t)&smSampleRAMBitmapNew_storage[0];

static char matrixPool_storage[matrixPool_size] = {};
static uint32_t matrixPool_address = (uint32_t)&matrixPool_storage[0];

static char lightingStatePool_storage[lightingStatePool_total_size] = {};
static uint32_t lightingStatePool_address = (uint32_t)&lightingStatePool_storage[0];
static uint32_t lightingStatePoolOffset_address = (uint32_t)&lightingStatePool_storage[0x540000];

static char renderItemPool_storage[renderItemPool_size] = {};
static uint32_t renderItemPool_address = (uint32_t)&renderItemPool_storage[0];

// Function names matched from BF1 Mac executable. Could be wrong in cases.

// clang-format off

const exe_patch_list patch_lists[EXE_COUNT] = {
   exe_patch_list{
      .name = "BF2_modtools",
      .id_address_is_file_offset = true,
      .id_address = 0x62b59c,
      .expected_id = 0x746163696c707041,
      .patches =
         {
            patch_set{
               .name = "RedMemory Heap Extensions",
               .patches =
                  {
                     patch{0x337921, 0x4000000, 0x10000000, {.file_offset = true}}, // malloc call arg
                     patch{0x33792c, 0x4000000, 0x10000000, {.file_offset = true}}, // malloc'd block end pointer
                  },
            },

            patch_set{
               .name = "SoundParameterized Layer Limit Extension",
               .patches =
                  {
                     patch{0x6227c2, 0xa0, 0x2000, {.file_offset = true}},
                  },
            },

            patch_set{
               .name = "DLC Mission Limit Extension",
               .patches =
                  {
                     patch{0x4935c, 0xb08308, DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}},                         // SetCurrentMap
                     patch{0x493ac, 0xb0830c, (0xb0830c - 0xb08308) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // SetCurrentMission
                     patch{0x49415, 0xb08310, (0xb08310 - 0xb08308) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // GetContentDirectory
                     patch{0x49472, 0xb0830c, (0xb0830c - 0xb08308) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // IsMissionDownloaded
                     patch{0x494fb, 0x1f4, DLC_mission_patch_limit, {.file_offset = true, .expected_is_va = true}},                              // AddDownloadableContent
                     patch{0x4951f, 0xb08308, DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}},                         // AddDownloadableContent
                     patch{0x49542, 0xb0830c, (0xb0830c - 0xb08308) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
                     patch{0x49548, 0xb08310, (0xb08310 - 0xb08308) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
                     patch{0x49571, 0xb08413, (0xb08413 - 0xb08308) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
                     patch{0x4957d, 0xb08414, (0xb08414 - 0xb08308) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
                  },
            },

            patch_set{
               .name = "Sound Limit Extension",
               .patches =
                  {
                     patch{0x486ae0 + 0x1, 0x2331f08, smSampleRAMBitmapNew_address, {.file_offset = true, .expected_is_va = true}}, // Snd::Engine::Open
                     patch{0x486aea + 0x1, 0x2000000, 0x10000000, {.file_offset = true}},                    // malloc call 1 arg
                     patch{0x486939 + 0x1, 0x2000000, 0x10000000, {.file_offset = true}},                    // malloc call 2 arg
                  },
            },

            patch_set{
               .name = "High-Res Animation Limit Extension",
               .patches =
                  {
                     patch{0x1840c7 + 0x2, 0x32, 0x1f4, {.file_offset = true}},                    // 10x increase
                     patch{0x1840cf + 0x1, 0x32, 0x1f4, {.file_offset = true}},                    // 10x increase
                     patch{0x184136 + 0x2, 0x64960, 0x1f4 * 0x2030, {.file_offset = true}},        // array size
                     patch{0x17e57e + 0x2, 0x64960, 0x1f4 * 0x2030, {.file_offset = true}},        // array size
                     patch{0x1840b3 + 0x1, 0x64970, 0x1f4 * 0x2030 + 0x10, {.file_offset = true}}, // heap allocation = array size - 16
                  },
            },

            patch_set{
               .name = "Matrix/Item Pool Limit Extension",
               .patches =
                  {
                     patch{0x405c0f + 0x2, 0xd64090, matrixPool_address, {.file_offset = true, .expected_is_va = true}},
                     patch{0x405c83 + 0x2, 0xd64090, matrixPool_address, {.file_offset = true, .expected_is_va = true}},
                     patch{0x410747 + 0x1, 0xd64090, matrixPool_address, {.file_offset = true, .expected_is_va = true}},
                     patch{0x405c15 + 0x2, 0xbf6, matrixPool_size, {.file_offset = true}},
                     patch{0x405c89 + 0x2, 0xbf6, matrixPool_size, {.file_offset = true}},
                     patch{0x61f8b0 + 0x1, 0x320, 0x40000, {.file_offset = true}},                      // transparentItemsSize
                     patch{0x61f8e0 + 0x1, 0x200, 0x25600, {.file_offset = true}},                      // postTransparentItemSize
                     patch{0x61f880 + 0x1, 0x64, 0x7f, {.file_offset = true, .values_are_8bit = true}}, // preShadowTransparentItemSize 8-bit signed int, needs relocating to increase it
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
                     patch{0x64793c, 0x461c4000, 0x47c35000, {.file_offset = true}},         //RenderRadius float
                     //patch{0x417956 + 0x1, 0x461c4000, 0x47c35000}, //RedCamera::SetFrustum call
                     //patch{0x40ae17 + 0x7, 0x461c4000, 0x47c35000}, //RedRenderer::internalUpdateSceneConstants float 1
                     patch{0x674cb0, 0x461c4000, 0x47c35000, {.file_offset = true}},         //ProjectionVectorZ float
                     patch{0x674794, 0xc61c4000, 0xc7c35000, {.file_offset = true}},         //ProjectionVectorZNeg float
                  },
            },

            patch_set{
               .name = "LOD Limit Extension",
               .patches =
                  {
                     patch{0x41d455 + 0x1, 0xc8, 0xc8 * 0x14, {.file_offset = true}},        // SetClassMaxCost::MaxCount    - modelClass (0)
                     patch{0x41d450 + 0x1, 0xc350, 0xc350 * 0x14, {.file_offset = true}},    // SetClassMaxCost::MaxCostLOD0 - modelClass (0)
                     patch{0x41d441 + 0x1, 0x9c40, 0x9c40 * 0x14, {.file_offset = true}},    // SetClassMaxCost::MaxCostLOD3 - modelClass (0)

                     patch{0x41d3c0 + 0x1, 0x258, 0x258 * 0x14, {.file_offset = true}},      // SetClassMaxCost::MaxCount    - bigModelClass (1)
                     patch{0x41d3bb + 0x1, 0x186a0, 0x186a0 * 0x14, {.file_offset = true}},  // SetClassMaxCost::MaxCostLOD0 - bigModelClass (1)
                     patch{0x41d3ac + 0x1, 0x9c40, 0x9c40 * 0x14, {.file_offset = true}},    // SetClassMaxCost::MaxCostLOD3 - bigModelClass (1)

                     patch{0x41d38c + 0x1, 0x64, 0x7f, {.file_offset = true, .values_are_8bit = true}}, // SetClassMaxCost::MaxCount    - soldierClass (2) 8-bit signed int, needs relocating to increase it
                     patch{0x41d387 + 0x1, 0x4650, 0x4650 * 0x14, {.file_offset = true}},    // SetClassMaxCost::MaxCostLOD0 - soldierClass (2)
                     patch{0x41d37a + 0x1, 0x9c40, 0x9c40 * 0x14, {.file_offset = true}},    // SetClassMaxCost::MaxCostLOD3 - soldierClass (2)

                     patch{0x41d410 + 0x1, 0x5dc, 0x5dc * 0x14, {.file_offset = true}},      // SetClassMaxCost::MaxCount    - hugeModelClass (3) - UBER
                     patch{0x41d40b + 0x1, 0x2710, 0x2710 * 0x14, {.file_offset = true}},    // SetClassMaxCost::MaxCostLOD0 - hugeModelClass (3) - UBER

                     patch{0x41d426 + 0x1, 0x12c, 0x12c * 0x14, {.file_offset = true}},      // SetClassMaxCost::MaxCount    - hugeModelClass (3)
                     patch{0x41d421 + 0x1, 0x3e8, 0x3e8 * 0x14, {.file_offset = true}},      // SetClassMaxCost::MaxCostLOD0 - hugeModelClass (3)
                     patch{0x41d3fa + 0x1, 0x9c40, 0x9c40 * 0x14, {.file_offset = true}},    // SetClassMaxCost::MaxCostLOD3 - hugeModelClass (3)
                  },
            },

            patch_set{
               .name = "Default Explosion VisibleRadius Increase",
               .patches =
                  {
                     patch{0x203637 + 0x6, 0x42700000, 0x461c4000, {.file_offset = true}}, // Increased from 60 to 10000
                  },
            },

            patch_set{
               .name = "String Pool Increase",
               .patches =
                  {
                     patch{0x4ef77 + 0x1, 0x8000, 0x20000, {.file_offset = true}},
                  },
            },

            patch_set{
               .name = "LightingState Pool Increase",
               .patches =
                  {
                     patch{0x405bf0 + 0x1, 0xde8068, lightingStatePoolOffset_address + (168 * 2), {.file_offset = true, .expected_is_va = true}},
                     patch{0x405ce1 + 0x2, 0xd93f18, lightingStatePool_address, {.file_offset = true, .expected_is_va = true}},
                     patch{0x405ce7 + 0x2, 0x800, 0x8000},
                     patch{0x406e75 + 0x1, 0xde8068, lightingStatePoolOffset_address + (168 * 2), {.file_offset = true, .expected_is_va = true}},
                     patch{0x408b65 + 0x2, 0xd93f18, lightingStatePool_address, {.file_offset = true, .expected_is_va = true}},
                     patch{0x40cd14 + 0x1, 0xde7f18, lightingStatePoolOffset_address, {.file_offset = true, .expected_is_va = true}},
                     patch{0x40cd20 + 0x1, 0xde7fc0, lightingStatePoolOffset_address + 168, {.file_offset = true, .expected_is_va = true}},
                     patch{0x40cd2c + 0x1, 0xde8068, lightingStatePoolOffset_address + (168 * 2), {.file_offset = true, .expected_is_va = true}},
                     patch{0x40cd63 + 0x6, 0xde7f18, lightingStatePoolOffset_address, {.file_offset = true, .expected_is_va = true}},
                     patch{0x40d199 + 0x6, 0xde7f18, lightingStatePoolOffset_address, {.file_offset = true, .expected_is_va = true}},
                     patch{0x40e9dc + 0x1, 0xde7f18, lightingStatePoolOffset_address, {.file_offset = true, .expected_is_va = true}},
                     patch{0x40e9e3 + 0x1, 0xde7fc0, lightingStatePoolOffset_address + 168, {.file_offset = true, .expected_is_va = true}},
                     patch{0x4107d7 + 0x1, 0xde7f18, lightingStatePoolOffset_address, {.file_offset = true, .expected_is_va = true}},
                     patch{0x4107de + 0x1, 0xde7fc0, lightingStatePoolOffset_address + 168, {.file_offset = true, .expected_is_va = true}},
                     patch{0x410dd2 + 0x3, 0xde7fc0, lightingStatePoolOffset_address + 168, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411691 + 0x3, 0xde7fc0, lightingStatePoolOffset_address + 168, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411b8f + 0x1, 0xde7f18, lightingStatePoolOffset_address, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411b9b + 0x1, 0xde7fc0, lightingStatePoolOffset_address + 168, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411ba7 + 0x1, 0xde8068, lightingStatePoolOffset_address + (168 * 2), {.file_offset = true, .expected_is_va = true}},
                     patch{0x411bb3 + 0x1, 0xde7f18, lightingStatePoolOffset_address, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411be0 + 0x2, 0xde7f68, lightingStatePoolOffset_address + 80, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411bec + 0x2, 0xde7f6c, lightingStatePoolOffset_address + 84, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411bf8 + 0x2, 0xde7f74, lightingStatePoolOffset_address + 92, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411c04 + 0x1, 0xde7f70, lightingStatePoolOffset_address + 88, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411c0e + 0x2, 0xde7f78, lightingStatePoolOffset_address + 96, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411c1a + 0x2, 0xde7f80, lightingStatePoolOffset_address + 104, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411c2a + 0x1, 0xde7fc0, lightingStatePoolOffset_address + 168, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411c2f + 0x1, 0xde7f7c, lightingStatePoolOffset_address + 100, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411c34 + 0x2, 0xde7f84, lightingStatePoolOffset_address + 108, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411c3a + 0x2, 0xde7f88, lightingStatePoolOffset_address + 112, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411c56 + 0x2, 0xde8014, lightingStatePoolOffset_address + 252, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411c62 + 0x1, 0xde8010, lightingStatePoolOffset_address + 248, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411c6c + 0x2, 0xde8018, lightingStatePoolOffset_address + 256, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411c78 + 0x2, 0xde8020, lightingStatePoolOffset_address + 264, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411c84 + 0x1, 0xde801c, lightingStatePoolOffset_address + 260, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411c8e + 0x2, 0xde8024, lightingStatePoolOffset_address + 268, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411c9a + 0x2, 0xde802c, lightingStatePoolOffset_address + 276, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411ca9 + 0x1, 0xde8028, lightingStatePoolOffset_address + 272, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411cae + 0x2, 0xde8030, lightingStatePoolOffset_address + 280, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411cb4 + 0x6, 0xde7f18, lightingStatePoolOffset_address, {.file_offset = true, .expected_is_va = true}},
                     patch{0x411f3a + 0x6, 0xde7f18, lightingStatePoolOffset_address, {.file_offset = true, .expected_is_va = true}},

                     patch{0x61f94a + 0x1, 0x803, 0x8003, {.file_offset = true}},
                     patch{0x61f954 + 0x1, 0xd93f18, lightingStatePool_address, {.file_offset = true, .expected_is_va = true}},
                     patch{0x627a65 + 0x1, 0x803, 0x8003, {.file_offset = true}},
                     patch{0x627a6f + 0x1, 0xd93f18, lightingStatePool_address, {.file_offset = true, .expected_is_va = true}},
                  },
            },

            patch_set{
               .name = "RenderItem Pool Increase",
               .patches =
                  {
                     patch{0x410619 + 0x6, 0x128c, 0x128c0, {.file_offset = true}},
                     patch{0x4106d5 + 0x3, 0xde81c4, renderItemPool_address + 52, {.file_offset = true, .expected_is_va = true}},
                     patch{0x4106dc + 0x3, 0xde8194, renderItemPool_address + 4, {.file_offset = true, .expected_is_va = true}},
                     patch{0x4106e9 + 0x3, 0xde8190, renderItemPool_address, {.file_offset = true, .expected_is_va = true}},
                     patch{0x410d0f + 0x3, 0xde8190, renderItemPool_address, {.file_offset = true, .expected_is_va = true}},
                     patch{0x41165a + 0x3, 0xde8190, renderItemPool_address, {.file_offset = true, .expected_is_va = true}},
                     patch{0x61f91a + 0x1, 0x128c, 0x128c0, {.file_offset = true}},
                     patch{0x61f921 + 0x1, 0xde8190, renderItemPool_address, {.file_offset = true, .expected_is_va = true}},
                     patch{0x627a45 + 0x1, 0x128c, 0x128c0, {.file_offset = true}},
                     patch{0x627a4c + 0x1, 0xde8190, renderItemPool_address, {.file_offset = true, .expected_is_va = true}},
                  },
            },
         },
   },
   
   exe_patch_list{
      .name = "BattlefrontII.exe GoG",
      .id_address_is_file_offset = true,
      .id_address = 0x39f298,
      .expected_id = 0x746163696c707041,
      .patches =
         {
            patch_set{
               .name = "RedMemory Heap Extensions",
               .patches =
                  {
                     patch{0x217651, 0x4000000, 0x10000000, {.file_offset = true}}, // malloc call arg
                     patch{0x217667, 0x4000000, 0x10000000, {.file_offset = true}}, // malloc'd block end pointer
                  },
            },

            patch_set{
               .name = "SoundParameterized Layer Limit Extension",
               .patches =
                  {
                     patch{0x3e310c, 0xa0, 0x2000, {.file_offset = true}},
                  },
            },

            patch_set{
               .name = "DLC Mission Limit Extension",
               .patches =
                  {
                     patch{0x8de7d, 0x1f4, DLC_mission_patch_limit, {.file_offset = true}},                                                         // AddDownloadableContent
                     patch{0x8de9f, 0x1e31f00, DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}},                           // AddDownloadableContent
                     patch{0x8dec3, 0x1e31f04, (0x1e31f04 - 0x1e31f00) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
                     patch{0x8dec9, 0x1e31f08, (0x1e31f08 - 0x1e31f00) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
                     patch{0x8def0, 0x1e3200b, (0x1e3200b - 0x1e31f00) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
                     patch{0x8def7, 0x1e3200c, (0x1e3200c - 0x1e31f00) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
                     patch{0x8df28, 0x1e31f00, DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}},                           // SetCurrentMap
                     patch{0x8df68, 0x1e31f04, (0x1e31f04 - 0x1e31f00) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // SetCurrentMission
                     patch{0x8dfb4, 0x1e31f08, (0x1e31f08 - 0x1e31f00) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // GetContentDirectory
                     patch{0x8dfce, 0x1e31f04, (0x1e31f04 - 0x1e31f00) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // IsMissionDownloaded
                  },
            },
         },
   },

   exe_patch_list{
      .name = "BattlefrontII.exe Steam",
      .id_address_is_file_offset = true,
      .id_address = 0x39e234,
      .expected_id = 0x746163696c707041,
      .patches =
         {
            patch_set{
               .name = "RedMemory Heap Extensions",
               .patches =
                  {
                     patch{0x2165b1, 0x4000000, 0x10000000, {.file_offset = true}}, // malloc call arg
                     patch{0x2165c7, 0x4000000, 0x10000000, {.file_offset = true}}, // malloc'd block end pointer
                  },
            },

            patch_set{
               .name = "SoundParameterized Layer Limit Extension",
               .patches =
                  {
                     patch{0x3e170c, 0xa0, 0x2000, {.file_offset = true}},
                  },
            },

            patch_set{
               .name = "DLC Mission Limit Extension",
               .patches =
                  {
                     patch{0x8de7d, 0x1f4, DLC_mission_patch_limit, {.file_offset = true}},                                                         // AddDownloadableContent
                     patch{0x8de9f, 0x1e30950, DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}},                           // AddDownloadableContent
                     patch{0x8dec3, 0x1e30954, (0x1e30954 - 0x1e30950) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
                     patch{0x8dec9, 0x1e30958, (0x1e30958 - 0x1e30950) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
                     patch{0x8def0, 0x1e30a5b, (0x1e30a5b - 0x1e30950) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
                     patch{0x8def7, 0x1e30a5c, (0x1e30a5c - 0x1e30950) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
                     patch{0x8df28, 0x1e30950, DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}},                           // SetCurrentMap
                     patch{0x8df68, 0x1e30954, (0x1e30954 - 0x1e30950) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // SetCurrentMission
                     patch{0x8dfb4, 0x1e30958, (0x1e30958 - 0x1e30950) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // GetContentDirectory
                     patch{0x8dfce, 0x1e30954, (0x1e30954 - 0x1e30950) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // IsMissionDownloaded
                  },
            },
         },
   },
};
