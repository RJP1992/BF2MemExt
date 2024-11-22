#include "patch_table.hpp"

const static uint32_t DLC_mission_size = 0x110;
const static uint32_t DLC_mission_patch_limit = 0x1000;
const static uint32_t smSampleRAMBitmapNew_size = 0x40000;

enum es_layout : uint32_t {
   ES_DLC_START = 0,
   ES_DLC_END = ES_DLC_START + (DLC_mission_size * DLC_mission_patch_limit),

   ES_SOUND_START = ES_DLC_END,
   ES_SOUND_END = ES_SOUND_START + smSampleRAMBitmapNew_size,

   ES_END = ES_SOUND_END
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
                     patch{0x486aea + 0x1, 0x2000000, 0x10000000},          // malloc call 1 arg
                     patch{0x486939 + 0x1, 0x2000000, 0x10000000},          // malloc call 2 arg
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
                     patch{0x332aa2 + 0x1, 0x9d1258, ES_SOUND_START, true},         // Snd::Engine::Open
                     patch{0x332aac + 0x1, 0x2000000, 0x10000000},                   // malloc call 1 arg
                     patch{0x3328e7 + 0x1, 0x2000000, 0x10000000},                   // malloc call 2 arg
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
         },
   },
};

extern const uint32_t ext_section_size = ES_END;
