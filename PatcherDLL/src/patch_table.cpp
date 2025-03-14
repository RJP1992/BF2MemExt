#include "pch.h"

#include "patch_table.hpp"

const static uint32_t DLC_mission_size = 0x110;
const static uint32_t DLC_mission_patch_limit = 0x1000;
static char DLC_mission_table_storage[DLC_mission_size * DLC_mission_patch_limit] = {};
static const uint32_t DLC_mission_table_address = (uint32_t)&DLC_mission_table_storage[0x0];

const static uint32_t smSampleRAMBitmapNew_size = 0x8000 * 0x8;
static char smSampleRAMBitmapNew_storage[smSampleRAMBitmapNew_size] = {};
static uint32_t smSampleRAMBitmapNew_address = (uint32_t)&smSampleRAMBitmapNew_storage[0x0];

const static uint32_t matrixPool_size = 0x2fd80 * 0x100; //provisional value
static char matrixPool_storage[matrixPool_size] = {};
static uint32_t matrixPool_address = (uint32_t)&matrixPool_storage[0x0];

const static uint32_t lightingStatePool_base_size = 0xa8 * 0x800 * 0x100;//0x540000; //provisional value - 168 * 2048 * 16
const static uint32_t lightingStatePoolOffset_size = 0xa8 * 0x3; //(168 * 3)
const static uint32_t lightingStatePool_total_size = lightingStatePool_base_size + lightingStatePoolOffset_size;
static char lightingStatePool_storage[lightingStatePool_total_size] = {};
static uint32_t lightingStatePool_address = (uint32_t)&lightingStatePool_storage[0x0];
static uint32_t lightingStatePoolOffset_address = (uint32_t)&lightingStatePool_storage[lightingStatePool_base_size];

const static uint32_t renderItemPool_size = 0x4ed30 * 0x100; //provisional value - 68 * 4748 * 16
static char renderItemPool_storage[renderItemPool_size] = {};
static uint32_t renderItemPool_address = (uint32_t)&renderItemPool_storage[0x0];

const static uint32_t BONE_hash_count = 8;
const static uint32_t BONE_hashes[BONE_hash_count] = {
    crc32bzip2("bone_root"),
    crc32bzip2("hp_weapons"),
    crc32bzip2("hp_f_chuck"),
    crc32bzip2("hp_f_chain"),
    crc32bzip2("hp_chain"),
    crc32bzip2("hp_b_chuck"),
    crc32bzip2("hp_b_chain"),
    crc32bzip2("hp_weapons_left"),
};
static uint32_t BONE_hashes_address = (uint32_t)&BONE_hashes[0];

const static uint32_t s_aComboAnimation_size = 0x24 * 90; //s_aComboAnimation_old = 0x24 * 30
static char s_aComboAnimation_storage[s_aComboAnimation_size] = {};
static uint32_t s_aComboAnimation_address = (uint32_t)&s_aComboAnimation_storage[0x0];

const static uint32_t s_aeComboAnimationPool_size = 0x4 * 256;
static char s_aeComboAnimationPool_storage[s_aeComboAnimationPool_size * 3] = {};
static uint32_t s_aeComboAnimationPool_address = (uint32_t)&s_aeComboAnimationPool_storage[0x0];

//const static uint32_t s_aBank_size = 0x2c * 16;
//static char s_aBank_storage[s_aBank_size * 2] = {};
//static uint32_t s_aBank_address = (uint32_t)&s_aBank_storage[0];

//const static uint32_t s_aMap_size = 0x8 * 30;
//static char s_aMap_storage[s_aMap_size * 2] = {};
//static uint32_t s_aMap_address = (uint32_t)&s_aMap_storage[0];

//const static uint32_t TENTACLE_BONE_hash_count = 45;
/*const static uint32_t TENTACLE_BONE_hashes[TENTACLE_BONE_hash_count] = {
    crc32bzip2("bone_string_1"),
    crc32bzip2("bone_string_2"),
    crc32bzip2("bone_string_3"),
    crc32bzip2("bone_string_4"),
    crc32bzip2("bone_string_5"),
    crc32bzip2("bone_string_6"),
    crc32bzip2("bone_string_7"),
    crc32bzip2("bone_string_8"),
    crc32bzip2("bone_string_9"),
    crc32bzip2("bone_string_10"),
    crc32bzip2("bone_string_11"),
    crc32bzip2("bone_string_12"),
    crc32bzip2("bone_string_13"),
    crc32bzip2("bone_string_14"),
    crc32bzip2("bone_string_15"),
    crc32bzip2("bone_string_16"),
    crc32bzip2("bone_string_17"),
    crc32bzip2("bone_string_19"),
    crc32bzip2("bone_string_20"),
    crc32bzip2("bone_string_21"),
    crc32bzip2("bone_string_22"),
    crc32bzip2("bone_string_23"),
    crc32bzip2("bone_string_24"),
    crc32bzip2("bone_string_25"),
    crc32bzip2("bone_string_26"),
    crc32bzip2("bone_string_27"),
    crc32bzip2("bone_string_28"),
    crc32bzip2("bone_string_29"),
    crc32bzip2("bone_string_30"),
    crc32bzip2("bone_string_31"),
    crc32bzip2("bone_string_32"),
    crc32bzip2("bone_string_33"),
    crc32bzip2("bone_string_34"),
    crc32bzip2("bone_string_35"),
    crc32bzip2("bone_string_36"),
    crc32bzip2("bone_string_37"),
    crc32bzip2("bone_string_38"),
    crc32bzip2("bone_string_39"),
    crc32bzip2("bone_string_40"),
    crc32bzip2("bone_string_41"),
    crc32bzip2("bone_string_42"),
    crc32bzip2("bone_string_43"),
    crc32bzip2("bone_string_44"),
    crc32bzip2("bone_string_45"),
};*/
//static uint32_t TENTACLE_BONE_hashes_address = (uint32_t)&TENTACLE_BONE_hashes[0];

//const static uint32_t sCachedParticles_size = 0x24 * 300;
//static char sCachedParticles_storage[sCachedParticles_size * 4] = {};
//static uint32_t sCachedParticles_address = (uint32_t)&sCachedParticles_storage[0];

const static uint32_t EntityEx_mIdMap_header_size = 0x4;
const static uint32_t EntityEx_mIdMap_table_size = 0x2000;
const static uint32_t EntityEx_mIdMap_total_size = EntityEx_mIdMap_header_size + EntityEx_mIdMap_table_size;
static char EntityEx_mIdMap_storage[EntityEx_mIdMap_total_size] = {};
static uint32_t EntityEx_mIdMap_header_address = (uint32_t)&EntityEx_mIdMap_storage[0x0];
static uint32_t EntityEx_mIdMap_table_address = (uint32_t)&EntityEx_mIdMap_storage[0x4];
static uint32_t EntityEx_mIdMap_table_mid_address = (uint32_t)&EntityEx_mIdMap_storage[(EntityEx_mIdMap_table_size / 0x2) + 0x4];

const static uint32_t EntityDoor_msHostDoors_size = 0x204 - 0x8;
static char EntityDoor_msHostDoors_storage[EntityDoor_msHostDoors_size] = {};
static uint32_t EntityDoor_msHostDoors_address = (uint32_t)&EntityDoor_msHostDoors_storage[0x0];

const static uint32_t EntityDoor_msClientDoors_size = 0x204 - 0x8;
static char EntityDoor_msClientDoors_storage[EntityDoor_msClientDoors_size] = {};
static uint32_t EntityDoor_msClientDoors_address = (uint32_t)&EntityDoor_msClientDoors_storage[0x0];

//const static uint32_t preShadowTransparentItemSize_Instruction_size = 0xb;
//static char preShadowTransparentItemSize_Instruction_storage[preShadowTransparentItemSize_Instruction_size] = {
//    0x68, 0x0, 0xc8, 0x0, 0x0,   //PUSH 0xc800
//    0xb9, 0xc8, 0xbe, 0x7f, 0x0, //MOV ECX,m_preShadowTransparentItems
//    0xe9, //preShadowTransparentItemSize_Instruction_storage[11] -                       //JMP 
//};
//static uint32_t preShadowTransparentItemSize_Instruction_address = (uint32_t)&preShadowTransparentItemSize_Instruction_storage[0];
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
                    patch{0x4935c, 0xb08308, DLC_mission_table_address, {.expected_is_va = true}},                         // SetCurrentMap
                    patch{0x493ac, 0xb0830c, (0xb0830c - 0xb08308) + DLC_mission_table_address, {.expected_is_va = true}}, // SetCurrentMission
                    patch{0x49415, 0xb08310, (0xb08310 - 0xb08308) + DLC_mission_table_address, {.expected_is_va = true}}, // GetContentDirectory
                    patch{0x49472, 0xb0830c, (0xb0830c - 0xb08308) + DLC_mission_table_address, {.expected_is_va = true}}, // IsMissionDownloaded
                    patch{0x494fb, 0x1f4, DLC_mission_patch_limit, {.expected_is_va = true}},                              // AddDownloadableContent
                    patch{0x4951f, 0xb08308, DLC_mission_table_address, {.expected_is_va = true}},                         // AddDownloadableContent
                    patch{0x49542, 0xb0830c, (0xb0830c - 0xb08308) + DLC_mission_table_address, {.expected_is_va = true}}, // AddDownloadableContent
                    patch{0x49548, 0xb08310, (0xb08310 - 0xb08308) + DLC_mission_table_address, {.expected_is_va = true}}, // AddDownloadableContent
                    patch{0x49571, 0xb08413, (0xb08413 - 0xb08308) + DLC_mission_table_address, {.expected_is_va = true}}, // AddDownloadableContent
                    patch{0x4957d, 0xb08414, (0xb08414 - 0xb08308) + DLC_mission_table_address, {.expected_is_va = true}}, // AddDownloadableContent
                  },
            },

            patch_set{
               .name = "Sound Limit Extension",
               .patches =
                  {
                    patch{0x486ae0 + 0x1, 0x2331f08, smSampleRAMBitmapNew_address, {.expected_is_va = true}}, // Snd::Engine::Open
                    patch{0x486aea + 0x1, 0x2000000, 0x10000000},                    // malloc call 1 arg
                    patch{0x486939 + 0x1, 0x2000000, 0x10000000},                    // malloc call 2 arg
                  },
            },

            patch_set{
               .name = "High-Res Animation Limit Extension",
               .patches =
                  {
                    patch{0x1840c7 + 0x2, 0x32, 0x3200},                    // 10x increase
                    patch{0x1840cf + 0x1, 0x32, 0x3200},                    // 10x increase
                    patch{0x184136 + 0x2, 0x64960, 0x3200 * 0x2030},        // array size
                    patch{0x17e57e + 0x2, 0x64960, 0x3200 * 0x2030},        // array size
                    patch{0x1840b3 + 0x1, 0x64970, 0x3200 * 0x2030 + 0x10}, // heap allocation = array size - 16*/
                  },
            },

            patch_set{
               .name = "Matrix/Item Pool Limit Extension",
               .patches =
                  {
                    patch{0x405c0f + 0x2, 0xd64090, matrixPool_address, {.expected_is_va = true}},
                    patch{0x405c83 + 0x2, 0xd64090, matrixPool_address, {.expected_is_va = true}},
                    patch{0x410747 + 0x1, 0xd64090, matrixPool_address, {.expected_is_va = true}},
                    patch{0x405c15 + 0x2, 0xbf6, matrixPool_size},
                    patch{0x405c89 + 0x2, 0xbf6, matrixPool_size},
                    patch{0x61f8b0 + 0x1, 0x320, 0x32000},                        // transparentItemsSize
                    patch{0x61f8e0 + 0x1, 0x200, 0x20000},                        // postTransparentItemSize

                    //preShadowTransparentItemSize patch
                    patch{0x61f880, 0x6a, 0xeb, {.values_are_8bit = true}},       //JMP to 0x61f8a3
                    patch{0x61f880 + 0x1, 0x64, 0x21, {.values_are_8bit = true}}, //JMP to 0x61f8a3
                    patch{0x61f8a3, 0xcc, 0x68, {.values_are_8bit = true}},       //PUSH 0xc800
                    patch{0x61f8a3 + 0x1, 0xcccccccc, 0x6400},                    //PUSH 0xc800
                    patch{0x61f8a8, 0xcc, 0xeb, {.values_are_8bit = true}},       //JMP to 0x61f882
                    patch{0x61f8a8 + 0x1, 0xcc, 0xd8, {.values_are_8bit = true}}, //JMP to 0x61f882*/
                 },
            },

            patch_set{
               .name = "nearScene Extension - Skipped",
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
                    //patch{0x417956 + 0x1, 0x461c4000, 0x47c35000},   //RedCamera::SetFrustum call
                    //patch{0x40ae17 + 0x7, 0x461c4000, 0x47c35000}, //RedRenderer::internalUpdateSceneConstants float 1
                    patch{0x674cb0, 0x461c4000, 0x47c35000},         //ProjectionVectorZ float
                    patch{0x674794, 0xc61c4000, 0xc7c35000},         //ProjectionVectorZNeg float*/
                  },
            },

            patch_set{
               .name = "LOD Limit Extension",
               .patches =
                  {
                    patch{0x41d455 + 0x1, 0xc8, 0xc8 * 0x10},                     // SetClassMaxCost::count    - modelClass (0)
                    patch{0x41d450 + 0x1, 0xc350, 0xc350 * 0x10},                 // SetClassMaxCost::costLOD0 - modelClass (0)
                    patch{0x41d441 + 0x1, 0x9c40, 0x9c40 * 0x10},                 // SetClassMaxCost::costLOD3 - modelClass (0)

                    patch{0x41d3c0 + 0x1, 0x258, 0x258 * 0x10},                   // SetClassMaxCost::count    - bigModelClass (1)
                    patch{0x41d3bb + 0x1, 0x186a0, 0x186a0 * 0x10},               // SetClassMaxCost::costLOD0 - bigModelClass (1)
                    patch{0x41d3ac + 0x1, 0x9c40, 0x9c40 * 0x10},                 // SetClassMaxCost::costLOD3 - bigModelClass (1)

                    patch{0x41d38c + 0x1, 0x64, 0x7f, {.values_are_8bit = true}}, // SetClassMaxCost::count    - soldierClass (2) 8-bit signed int, needs relocating to increase it
                    patch{0x41d387 + 0x1, 0x4650, 0x4650 * 0x10},                 // SetClassMaxCost::costLOD0 - soldierClass (2)
                    patch{0x41d37a + 0x1, 0x9c40, 0x9c40 * 0x10},                 // SetClassMaxCost::costLOD3 - soldierClass (2)

                    patch{0x41d410 + 0x1, 0x5dc, 0x5dc * 0x10},                   // SetClassMaxCost::count    - hugeModelClass (3) - UBER
                    patch{0x41d40b + 0x1, 0x2710, 0x2710 * 0x10},                 // SetClassMaxCost::costLOD0 - hugeModelClass (3) - UBER

                    patch{0x41d426 + 0x1, 0x12c, 0x12c * 0x10},                   // SetClassMaxCost::count    - hugeModelClass (3)
                    patch{0x41d421 + 0x1, 0x3e8, 0x3e8 * 0x10},                   // SetClassMaxCost::costLOD0 - hugeModelClass (3)
                    patch{0x41d3fa + 0x1, 0x9c40, 0x9c40 * 0x10},                 // SetClassMaxCost::costLOD3 - hugeModelClass (3)*/
                  },
            },

            patch_set{
               .name = "Far Objects Render Extension - Skipped",
               .patches =
                  {
                    /*patch{0x3f07bb + 0x1, 0x1008, 0x4008}, //RedScene::SetupStaticWorld heap pool size
                    patch{0x3f07cc + 0x3, 0x200, 0x800},   //RedScene::SetupStaticWorld heap maxCount

                    patch{0x3efa15 + 0x2, 0x42c, 0x10b0},  //RenderFarObjects sub stack
                    patch{0x3efa36 + 0x2, 0x100, 0x400},   //RenderFarObjects sMaxNumFarObjects CMP
                    patch{0x3efa4a + 0x1, 0x100, 0x400},   //RenderFarObjects sMaxNumFarObjects minimum
                    patch{0x3efa59 + 0x1, 0xa, 0x28},      //RenderFarObjects class 2 count
                    patch{0x3efcfa + 0x2, 0x42c, 0x10b0},  //RenderFarObjects add stack

                    patch{0x3f086e + 0x1, 0x2008, 0x8008}, //RedSceneManager::RedSceneManager heap pool size
                    patch{0x3f087f + 0x3, 0x400, 0x1000},  //RedSceneManager::RedSceneManager heap maxCount*/
                  },
            },

            //patch_set{
            //   .name = "Particle Increase Test",
            //   .patches =
            //      {
            //        patch{0x26daf5 + 0x2, 0x994, 0x25b4},        //ParticleSystem::FlushParticleCache stack sub 2f08
            //        patch{0x26dd1c + 0x2, 0x994, 0x25b4},        //ParticleSystem::FlushParticleCache stack add
            //        patch{0x26db69 + 0x4, 0x12c, 0x4b0},         //heap.maxCount
            //
            //        patch{0x26d826 + 0x2, 0x12c, 0x4b0},         
            //
            //        patch{0x26d83c + 0x2, 0xb9db78, sCachedParticles_address, {.expected_is_va = true}},
            //        patch{0x26d856 + 0x2, 0xb9db84, sCachedParticles_address + 0xc, {.expected_is_va = true}},
            //        patch{0x26d874 + 0x2, 0xb9db94, sCachedParticles_address + 0x1c, {.expected_is_va = true}},
            //        patch{0x26d880 + 0x2, 0xb9db98, sCachedParticles_address + 0x20, {.expected_is_va = true}},
            //        patch{0x26db93 + 0x2, 0xb9db78, sCachedParticles_address, {.expected_is_va = true}},
            //        patch{0x26dba8 + 0x2, 0xb9db7c, sCachedParticles_address + 0x4, {.expected_is_va = true}},
            //        patch{0x26dbbd + 0x2, 0xb9db80, sCachedParticles_address + 0x8, {.expected_is_va = true}},
            //        patch{0x26dc03 + 0x2, 0xb9db94, sCachedParticles_address + 0x1c, {.expected_is_va = true}},
            //        patch{0x26dc10 + 0x2, 0xb9db90, sCachedParticles_address + 0x18, {.expected_is_va = true}},
            //        patch{0x26dc16 + 0x2, 0xb9db94, sCachedParticles_address + 0x1c, {.expected_is_va = true}},
            //        patch{0x26dc20 + 0x2, 0xb9db90, sCachedParticles_address + 0x18, {.expected_is_va = true}},
            //        patch{0x26dc76 + 0x2, 0xb9db8c, sCachedParticles_address + 0x14, {.expected_is_va = true}},
            //        patch{0x26dc87 + 0x2, 0xb9db88, sCachedParticles_address + 0x10, {.expected_is_va = true}},
            //        patch{0x26dc9c + 0x2, 0xb9db84, sCachedParticles_address + 0xc, {.expected_is_va = true}},
            //        patch{0x26dcb1 + 0x2, 0xb9db90, sCachedParticles_address + 0x18, {.expected_is_va = true}},
            //        patch{0x26dcc6 + 0x2, 0xb9db94, sCachedParticles_address + 0x1c, {.expected_is_va = true}},
            //        patch{0x26dcdb + 0x2, 0xb9db98, sCachedParticles_address + 0x20, {.expected_is_va = true}},
            //        patch{0x26dce8 + 0x2, 0xb9db78, sCachedParticles_address, {.expected_is_va = true}},
            //
            //        patch{0x424bc0 + 0x1, 0x12c, 0x4b0},
            //        patch{0x424bd1 + 0x1, 0x12c, 0x4b0},
            //      },
            //},

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
                    patch{0x4ef77 + 0x1, 0x8000, 0x800000},
                  },
            },

            patch_set{
               .name = "LightingState Pool Increase - Skipped",
               .patches =
                  {
                    /*patch{0x405bf0 + 0x1, 0xde8068, lightingStatePoolOffset_address + (168 * 2), {.expected_is_va = true}},
                    patch{0x405ce1 + 0x2, 0xd93f18, lightingStatePool_address, {.expected_is_va = true}},
                    patch{0x405ce7 + 0x2, 0x800, 0x8000},
                    patch{0x406e75 + 0x1, 0xde8068, lightingStatePoolOffset_address + (168 * 2), {.expected_is_va = true}},
                    patch{0x408b65 + 0x2, 0xd93f18, lightingStatePool_address, {.expected_is_va = true}},
                    patch{0x40cd14 + 0x1, 0xde7f18, lightingStatePoolOffset_address, {.expected_is_va = true}},
                    patch{0x40cd20 + 0x1, 0xde7fc0, lightingStatePoolOffset_address + 168, {.expected_is_va = true}},
                    patch{0x40cd2c + 0x1, 0xde8068, lightingStatePoolOffset_address + (168 * 2), {.expected_is_va = true}},
                    patch{0x40cd63 + 0x6, 0xde7f18, lightingStatePoolOffset_address, {.expected_is_va = true}},
                    patch{0x40d199 + 0x6, 0xde7f18, lightingStatePoolOffset_address, {.expected_is_va = true}},
                    patch{0x40e9dc + 0x1, 0xde7f18, lightingStatePoolOffset_address, {.expected_is_va = true}},
                    patch{0x40e9e3 + 0x1, 0xde7fc0, lightingStatePoolOffset_address + 168, {.expected_is_va = true}},
                    patch{0x4107d7 + 0x1, 0xde7f18, lightingStatePoolOffset_address, {.expected_is_va = true}},
                    patch{0x4107de + 0x1, 0xde7fc0, lightingStatePoolOffset_address + 168, {.expected_is_va = true}},
                    patch{0x410dd2 + 0x3, 0xde7fc0, lightingStatePoolOffset_address + 168, {.expected_is_va = true}},
                    patch{0x411691 + 0x3, 0xde7fc0, lightingStatePoolOffset_address + 168, {.expected_is_va = true}},
                    patch{0x411b8f + 0x1, 0xde7f18, lightingStatePoolOffset_address, {.expected_is_va = true}},
                    patch{0x411b9b + 0x1, 0xde7fc0, lightingStatePoolOffset_address + 168, {.expected_is_va = true}},
                    patch{0x411ba7 + 0x1, 0xde8068, lightingStatePoolOffset_address + (168 * 2), {.expected_is_va = true}},
                    patch{0x411bb3 + 0x1, 0xde7f18, lightingStatePoolOffset_address, {.expected_is_va = true}},
                    patch{0x411be0 + 0x2, 0xde7f68, lightingStatePoolOffset_address + 80, {.expected_is_va = true}},
                    patch{0x411bec + 0x2, 0xde7f6c, lightingStatePoolOffset_address + 84, {.expected_is_va = true}},
                    patch{0x411bf8 + 0x2, 0xde7f74, lightingStatePoolOffset_address + 92, {.expected_is_va = true}},
                    patch{0x411c04 + 0x1, 0xde7f70, lightingStatePoolOffset_address + 88, {.expected_is_va = true}},
                    patch{0x411c0e + 0x2, 0xde7f78, lightingStatePoolOffset_address + 96, {.expected_is_va = true}},
                    patch{0x411c1a + 0x2, 0xde7f80, lightingStatePoolOffset_address + 104, {.expected_is_va = true}},
                    patch{0x411c2a + 0x1, 0xde7fc0, lightingStatePoolOffset_address + 168, {.expected_is_va = true}},
                    patch{0x411c2f + 0x1, 0xde7f7c, lightingStatePoolOffset_address + 100, {.expected_is_va = true}},
                    patch{0x411c34 + 0x2, 0xde7f84, lightingStatePoolOffset_address + 108, {.expected_is_va = true}},
                    patch{0x411c3a + 0x2, 0xde7f88, lightingStatePoolOffset_address + 112, {.expected_is_va = true}},
                    patch{0x411c56 + 0x2, 0xde8014, lightingStatePoolOffset_address + 252, {.expected_is_va = true}},
                    patch{0x411c62 + 0x1, 0xde8010, lightingStatePoolOffset_address + 248, {.expected_is_va = true}},
                    patch{0x411c6c + 0x2, 0xde8018, lightingStatePoolOffset_address + 256, {.expected_is_va = true}},
                    patch{0x411c78 + 0x2, 0xde8020, lightingStatePoolOffset_address + 264, {.expected_is_va = true}},
                    patch{0x411c84 + 0x1, 0xde801c, lightingStatePoolOffset_address + 260, {.expected_is_va = true}},
                    patch{0x411c8e + 0x2, 0xde8024, lightingStatePoolOffset_address + 268, {.expected_is_va = true}},
                    patch{0x411c9a + 0x2, 0xde802c, lightingStatePoolOffset_address + 276, {.expected_is_va = true}},
                    patch{0x411ca9 + 0x1, 0xde8028, lightingStatePoolOffset_address + 272, {.expected_is_va = true}},
                    patch{0x411cae + 0x2, 0xde8030, lightingStatePoolOffset_address + 280, {.expected_is_va = true}},
                    patch{0x411cb4 + 0x6, 0xde7f18, lightingStatePoolOffset_address, {.expected_is_va = true}},
                    patch{0x411f3a + 0x6, 0xde7f18, lightingStatePoolOffset_address, {.expected_is_va = true}},

                    patch{0x61f94a + 0x1, 0x803, 0x8003},
                    patch{0x61f954 + 0x1, 0xd93f18, lightingStatePool_address, {.expected_is_va = true}},
                    patch{0x627a65 + 0x1, 0x803, 0x8003},
                    patch{0x627a6f + 0x1, 0xd93f18, lightingStatePool_address, {.expected_is_va = true}},*/
                  },
            },

            patch_set{
               .name = "RenderItem Pool Increase - Skipped",
               .patches =
                  {
                    /*patch{0x410619 + 0x6, 0x128c, 0x251800},
                    patch{0x4106d5 + 0x3, 0xde81c4, renderItemPool_address + 0x34, {.expected_is_va = true}},
                    patch{0x4106dc + 0x3, 0xde8194, renderItemPool_address + 0x4, {.expected_is_va = true}},
                    patch{0x4106e9 + 0x3, 0xde8190, renderItemPool_address, {.expected_is_va = true}},
                    patch{0x410d0f + 0x3, 0xde8190, renderItemPool_address, {.expected_is_va = true}},
                    patch{0x41165a + 0x3, 0xde8190, renderItemPool_address, {.expected_is_va = true}},
                    patch{0x61f91a + 0x1, 0x128c, 0x251800},
                    patch{0x61f921 + 0x1, 0xde8190, renderItemPool_address, {.expected_is_va = true}},
                    patch{0x627a45 + 0x1, 0x128c, 0x251800},
                    patch{0x627a4c + 0x1, 0xde8190, renderItemPool_address, {.expected_is_va = true}},*/
                  },
            },

            patch_set{
               .name = "Bone Transforms Patch",
               .patches =
                  {
                    patch{0x17e6b9 + 0x1, 0xa45d14, BONE_hashes_address, {.expected_is_va = true}},
                  },
            },

            patch_set{
               .name = "Combo Anims Increase",
               .patches =
                  {
                    patch{0x170467 + 0x3, 0xb8c620, s_aComboAnimation_address, {.expected_is_va = true}},        //SoldierAnimationBank::_GetComboAnimation
                    patch{0x1709b1 + 0x1, 0xb8c640, s_aComboAnimation_address + 0x20, {.expected_is_va = true}}, //SoldierAnimationBank::FindComboAnimation
                    patch{0x170a65 + 0x2, 0x1e, 0x5a, {.values_are_8bit = true}},                                //SoldierAnimationBank::AddComboAnimation
                    patch{0x188a40 + 0x2, 0x1e, 0x5a, {.values_are_8bit = true}},                                //SoldierAnimType::IsWeaponMeleeAnimIndex
                    //patch{0x170a90 + 0x2, 0x1f, 0x79, {.values_are_8bit = true}},                                //SoldierAnimationBank::AddComboAnimation

                    //patch{0x17ef41 + 0x2, 0x1e, 0x78, {.values_are_8bit = true}}, //SetupVelocity

                    patch{0x170a2b + 0x3, 0xb8cc80, s_aeComboAnimationPool_address, {.expected_is_va = true}},   //SoldierAnimationBank::AddComboAnimation
                    patch{0x170b31 + 0x3, 0xb8cc80, s_aeComboAnimationPool_address, {.expected_is_va = true}},   //SoldierAnimationBank::GetComboAnimationIndex
                    patch{0x170a22 + 0x1, 0x100, 0x300},                                                         //SoldierAnimationBank::AddComboAnimation
                    patch{0x170b27 + 0x2, 0x100, 0x300},                                                         //SoldierAnimationBank::GetComboAnimationIndex

                    //patch{0x173045 + 0x1, 0x1004, 0x2004}, //s_pAnimationNameTable array size
                    //patch{0x17305b + 0x1, 0x200, 0x400}, //s_pAnimationNameTable clear size
                    //patch{0x17310f + 0x1, 0x400, 0x800}, //s_pAnimationNameTable find1 size
                    //patch{0x173186 + 0x1, 0x400, 0x800}, //s_pAnimationNameTable find2 size
                    //patch{0x173208 + 0x1, 0x400, 0x800}, //s_pAnimationNameTable find3 size
                    //patch{0x1722d1 + 0x1, 0x400, 0x800}, //s_pAnimationNameTable find size
                    patch{0x1722e8 + 0x1, 0x148, 0x1fc}, //s_pAnimationNameTable find upper limit
                    //patch{0x172367 + 0x1, 0x400, 0x800}, //s_pAnimationNameTable find size
                    //patch{0x172390 + 0x1, 0x400, 0x800}, //s_pAnimationNameTable store size
                    //patch{0x173324 + 0x1, 0x400, 0x800}, //s_pAnimationNameTable find size
                    //patch{0x17335b + 0x1, 0x400, 0x800}, //s_pAnimationNameTable store size

                    //patch{0x171d18 + 0x1, 0x400, 0x800}, //PblHashTable<void,512>::Find find size
                    //patch{0x171d40 + 0x1, 0x400, 0x800}, //PblHashTable<void,512>::Store store size
                    //patch{0x171d76 + 0x1, 0x200, 0x400}, //PblHashTable<void,512>::Clear clear size
                    //patch{0x1723e6 + 0x1, 0x200, 0x400}, //PblHashTable<void,512>::PblHashTable<void,512> clear size

                    patch{0x1737be + 0x1, 0xf60, 0x17d0}, //SoldierAnimationData::InitAnimationData SoldierAnimationData size
                    patch{0x1739a6 + 0x2, 0xf60, 0x17d0}, //SoldierAnimationData::InitAnimationData SoldierAnimationData size

                    patch{0x188b06 + 0x1, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimType::GetAnimFromAnimIndex

                    patch{0x178175 + 0x1, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SoldierAnimator
                    patch{0x17ad35 + 0x1, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SetNewOwner

                    patch{0x17b02c + 0x1, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::UpdateActionAnimation
                    patch{0x17b13a + 0x1, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::UpdateActionAnimation
                    patch{0x17b1ca + 0x6, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::UpdateActionAnimation

                    patch{0x17b9d1 + 0x1, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::UpdateMovementAnimation
                    patch{0x17baaf + 0x1, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::UpdateMovementAnimation
                    patch{0x17bc81 + 0x1, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::UpdateMovementAnimation
                    patch{0x17bc89 + 0x6, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::UpdateMovementAnimation

                    patch{0x17ccc3 + 0x6, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SetupPose

                    patch{0x187951 + 0x2, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimatorLowResClass::PostLoad
                    patch{0x187a36 + 0x2, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimatorLowResClass::PostLoad
                    patch{0x18788a + 0x2, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimatorLowResClass::PostLoad

                    patch{0x176a3f + 0x1, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SetWeaponAnimationMap

                    patch{0x176c60 + 0x1, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SetWeaponComboState
                    patch{0x176c62 + 0x6, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SetWeaponComboState
                    patch{0x176c84 + 0x1, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SetWeaponComboState
                    patch{0x176c97 + 0x6, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SetWeaponComboState

                    patch{0x136d47 + 0x1, 0xa4, 0xfe},                            //EntitySoldier::Render
                    patch{0x136d4c + 0x1, 0xa4, 0xfe},                            //EntitySoldier::Render
                    patch{0x136c99 + 0x1, 0xa4, 0xfe},                            //EntitySoldier::Render
                    patch{0x136c54 + 0x1, 0xa4, 0xfe},                            //EntitySoldier::Render

                    //notes:
                    //0x1e8 (488) is the diff between SoldierAnimation *[30][2] - SoldierAnimation *[91][2] (mWeaponMeleeAnimation)
                    //SoldierAnimation *[1][2] = 0x8
                    //Therefore:
                    //offsetDiff = (91 - 30 = 61) * 0x8 = 488
                    //0x4b4 + 0x1e8 = 0x69c
                    //patch{0x17deb0 + 0x3, 0x4b4, 0x69c},                          //SoldierAnimatorClass::GetCustomAnimation

                    patch{0x1f5cf7 + 0x1, 0xa4, 0xfe, {.values_are_8bit = true}}, //FUN_005f5bb0
                    patch{0x1f6c63 + 0x3, 0xa4, 0xfe, {.values_are_8bit = true}}, //FUN_005f6b20
                    patch{0x1f7754 + 0x3, 0xa4, 0xfe, {.values_are_8bit = true}}, //FUN_005f7600
                    patch{0x200af3 + 0x2, 0xa4, 0xfe, {.values_are_8bit = true}}, //FUN_00600990
                    patch{0x1f6094 + 0x2, 0xa4, 0xfe, {.values_are_8bit = true}}, //FUN_005f6090

                    patch{0x1778b3 + 0x2, 0xa4, 0xfe, {.values_are_8bit = true}}, //g_fnAnim_Data
                    patch{0x177d4a + 0x1, 0xa4, 0xfe},                            //g_fnAnim_Data

                    //notes:
                    //0x1e8 (488) is the diff between SoldierAnimation *[30][2] - SoldierAnimation *[91][2] (mWeaponMeleeAnimation)
                    //It has to be added to the main stack for each CachedBankStack (24 total)
                    //Therefore:
                    //StackDiff = 0x1e8 (488) * 24 = 0x2dc0 (11712)
                    //0x9174 + 0x2dc0 = 0xbf34
                    //patch{0x181af6 + 0x1, 0x9174, 0xbf34},                        //SoldierAnimatorClass::_PostLoad
                  },
            },

            patch_set{
               .name = "Object Increase - Skipped",
               .patches =
                  {
                    /*//EntityEx::mIdMap needs relocating
                    //any and all references to it and to PblHashTable<EntityEx,1024> need increasing at the same ratio

                    //EntityEx::mIdMap function calls
                    //patch{0x701d5 + 0x1, 0x800, 0x1000},
                    //patch{0x70f11 + 0x1, 0x800, 0x1000},
                    //patch{0x71041 + 0x1, 0x800, 0x1000},
                    //patch{0x71171 + 0x1, 0x800, 0x1000},
                    //patch{0x713e1 + 0x1, 0x800, 0x1000},
                    //patch{0x71511 + 0x1, 0x800, 0x1000},
                    //patch{0x89e86 + 0x1, 0x800, 0x1000},
                    //patch{0xd04ea + 0x1, 0x800, 0x1000},
                    //patch{0xd0515 + 0x1, 0x800, 0x1000},
                    //patch{0xd0584 + 0x1, 0x800, 0x1000},
                    //patch{0xd0648 + 0x1, 0x800, 0x1000},
                    //patch{0x126bb5 + 0x1, 0x800, 0x1000},
                    //patch{0x126c25 + 0x1, 0x800, 0x1000},
                    //patch{0x1276d1 + 0x1, 0x800, 0x1000},
                    //patch{0x127cf0 + 0x1, 0x800, 0x1000},
                    //patch{0x127d96 + 0x1, 0x800, 0x1000},
                    //patch{0x1e4a4b + 0x1, 0x800, 0x1000},
                    //patch{0x25f084 + 0x1, 0x800, 0x1000},
                    //patch{0x25f3a0 + 0x1, 0x800, 0x1000},
                    //patch{0x265d2b + 0x1, 0x800, 0x1000},
                    //patch{0x265db5 + 0x1, 0x800, 0x1000},
                    //patch{0x2ef87a + 0x1, 0x800, 0x1000},
                    //patch{0x333d40 + 0x1, 0x400, 0x800},
                    //patch{0x33476f + 0x1, 0x400, 0x800},
                    //patch{0x338eaa + 0x1, 0x400, 0x800},
                    //patch{0x3a54c4 + 0x1, 0x800, 0x1000},
                    //patch{0x3a5504 + 0x1, 0x800, 0x1000},
                    //patch{0x3a5544 + 0x1, 0x800, 0x1000},
                    //patch{0x3a5584 + 0x1, 0x800, 0x1000},
                    //patch{0x3a7254 + 0x1, 0x800, 0x1000},
                    //patch{0x3a72e4 + 0x1, 0x800, 0x1000},
                    //patch{0x3a7374 + 0x1, 0x800, 0x1000},
                    //patch{0x3a7404 + 0x1, 0x800, 0x1000},
                    //patch{0x616490 + 0x1, 0x400, 0x800},

                    //ops
                    //patch{0x8d69d + 0x3, 0x1004, 0x2004}, //pvs::PortalReader::Read
                    //patch{0x8d6fd + 0x3, 0x1004, 0x2004}, //pvs::PortalReader::Read
                    //patch{0x8e7fc + 0x2, 0x400, 0x800},
                    //patch{0x8e83b + 0x2, 0x400, 0x800},
                    //patch{0x8eb4c + 0x2, 0x400, 0x800},
                    //patch{0x8eba5 + 0x2, 0x400, 0x800},
                    //patch{0x8ebbc + 0x2, 0x400, 0x800},

                    //EntityEx::mIdMap address pointers
                    //patch{0x701da + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x70f16 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x71046 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},     //_GetEntity<GameObject>
                    //patch{0x71176 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},     //_GetEntity<EntityfEx>
                    //patch{0x713e6 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},     //FUN_00471390
                    //patch{0x71516 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x88f30 + 0x1, 0xb7ad38, EntityEx_mIdMap_header_address, {.expected_is_va = true}},
                    //patch{0x89e8b + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x8d68b + 0x1, 0xb7ad38, EntityEx_mIdMap_header_address, {.expected_is_va = true}},
                    //patch{0x8e7f0 + 0x3, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x8e804 + 0x3, 0xb7bd3c, EntityEx_mIdMap_table_mid_address, {.expected_is_va = true}}, //FUN_0048e7e0
                    //patch{0x8e822 + 0x3, 0xb7bd3c, EntityEx_mIdMap_table_mid_address, {.expected_is_va = true}}, //FUN_0048e7e0
                    //patch{0x8e843 + 0x3, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x8e857 + 0x3, 0xb7bd3c, EntityEx_mIdMap_table_mid_address, {.expected_is_va = true}}, //FUN_0048e7e0
                    //patch{0x8eb40 + 0x3, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x8eb54 + 0x3, 0xb7bd3c, EntityEx_mIdMap_table_mid_address, {.expected_is_va = true}}, //FUN_0048eaa0
                    //patch{0x8eb79 + 0x3, 0xb7bd3c, EntityEx_mIdMap_table_mid_address, {.expected_is_va = true}}, //FUN_0048eaa0
                    //patch{0x8ebb0 + 0x3, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x8ebc4 + 0x3, 0xb7bd3c, EntityEx_mIdMap_table_mid_address, {.expected_is_va = true}}, //FUN_0048eaa0
                    //patch{0xd04ef + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0xd0500 + 0x2, 0xb7ad38, EntityEx_mIdMap_header_address, {.expected_is_va = true}},
                    //patch{0xd051a + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0xd052b + 0x2, 0xb7ad38, EntityEx_mIdMap_header_address, {.expected_is_va = true}},
                    //patch{0xd0589 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0xd059a + 0x2, 0xb7ad38, EntityEx_mIdMap_header_address, {.expected_is_va = true}},
                    //patch{0xd064d + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0xd065e + 0x2, 0xb7ad38, EntityEx_mIdMap_header_address, {.expected_is_va = true}},
                    //patch{0x126bba + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x126c2a + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x1276d6 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x127cf5 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x127d9b + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x1e4a50 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x25f089 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x25f3a5 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x265d30 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x265dba + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x2ef87f + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x333d45 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x333d52 + 0x2, 0xb7ad38, EntityEx_mIdMap_header_address, {.expected_is_va = true}},
                    //patch{0x334774 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x33477e + 0x2, 0xb7ad38, EntityEx_mIdMap_header_address, {.expected_is_va = true}},
                    //patch{0x338eaf + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x338ec3 + 0x2, 0xb7ad38, EntityEx_mIdMap_header_address, {.expected_is_va = true}},
                    //patch{0x3a54c9 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x3a5509 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x3a5549 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x3a5589 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x3a7259 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x3a72e9 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x3a7379 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x3a7409 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x616495 + 0x1, 0xb7ad3c, EntityEx_mIdMap_table_address, {.expected_is_va = true}},
                    //patch{0x6164a2 + 0x2, 0xb7ad38, EntityEx_mIdMap_header_address, {.expected_is_va = true}},

                    //PblHashTable<EntityEx,1024> functions
                    //patch{0x89f2c + 0x2, 0x400, 0x800},
                    //patch{0xd03d6 + 0x1, 0x400, 0x800},
                    //patch{0x89f56 + 0x3, 0x400, 0x800},
                    //patch{0x700c8 + 0x1, 0x800, 0x1000},
                    //patch{0x894dc + 0x2, 0x400, 0x800},
                    //patch{0x89455 + 0x3, 0x1004, 0x2004}, //PblHashTable<EntityEx,1024>::Itor::operator*
                    //patch{0x89476 + 0x2, 0x400, 0x800},
                    //patch{0x8948f + 0x2, 0x400, 0x800},
                    //patch{0x8e045 + 0x3, 0x1004, 0x2004}, //PblHashTable<EntityEx,1024>::Itor::operator->
                    //patch{0x89465 + 0x3, 0x1004, 0x2004}, //PblHashTable<EntityEx,1024>::Itor::operator_EntityEx_*
                    //patch{0xd0696 + 0x1, 0x400, 0x800},
                    //patch{0xd044b + 0x1, 0x800, 0x1000},
                    //patch{0xd0410 + 0x1, 0x800, 0x1000},*/
                  },
            },

            patch_set{
               .name = "Doors Increase - Skipped",
               .patches =
                  {
                    /*//replace EntityDoor::mspDoors pointer to new address
                    //relocate EntityDoor::msHostDoors
                    //relocate EntityDoor::msClientDoors

                    //msHostDoors
                    patch{0xd8b89 + 0x6, 0xb7d580, EntityDoor_msHostDoors_address, {.expected_is_va = true}},
                    patch{0xd8fb2 + 0x1, 0xb7d584, EntityDoor_msHostDoors_address + 0x4, {.expected_is_va = true}},
                    patch{0xd8fb7 + 0x6, 0xb7d580, EntityDoor_msHostDoors_address, {.expected_is_va = true}},
                    patch{0xd8fcd + 0x2, 0xb7d580, EntityDoor_msHostDoors_address, {.expected_is_va = true}},
                    patch{0x616752 + 0x1, 0xb7d584, EntityDoor_msHostDoors_address + 0x4, {.expected_is_va = true}},
                    patch{0x61675f + 0x2, 0xb7d580, EntityDoor_msHostDoors_address, {.expected_is_va = true}},

                    //EntityDoor::mspDoors pointer
                    patch{0x6cd6dc, 0xb7d580, EntityDoor_msHostDoors_address, {.expected_is_va = true}},

                    //msClientDoors
                    patch{0xd8b69 + 0x6, 0xb7d688, EntityDoor_msClientDoors_address, {.expected_is_va = true}},
                    patch{0xd8fc8 + 0x1, 0xb7d68c, EntityDoor_msClientDoors_address + 0x4, {.expected_is_va = true}},
                    patch{0xd8fdf + 0x2, 0xb7d688, EntityDoor_msClientDoors_address, {.expected_is_va = true}},
                    patch{0x616772 + 0x1, 0xb7d68c, EntityDoor_msClientDoors_address + 0x4, {.expected_is_va = true}},
                    patch{0x61677f + 0x2, 0xb7d688, EntityDoor_msClientDoors_address, {.expected_is_va = true}},

                    //mspDoors ops
                    patch{0xd8fb0 + 0x1, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    patch{0xd8fc6 + 0x1, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    patch{0xd900e + 0x1, 0x40, 0x80 - 0x8, {.values_are_8bit = true}},
                    patch{0xd94c9 + 0x1, 0x40, 0x80 - 0x8, {.values_are_8bit = true}},
                    patch{0xd94e8 + 0x1, 0x40, 0x80 - 0x8, {.values_are_8bit = true}},
                    patch{0xda301 + 0x2, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    patch{0xda32b + 0x3, 0x84, 0x104 - 0x8, {.values_are_8bit = true}},
                    patch{0xda339 + 0x2, 0x104, 0x204 - 0x8, {.values_are_8bit = true}},

                    //msHostDoors ops
                    patch{0x616750 + 0x1, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},

                    //msClientDoors ops
                    patch{0x616770 + 0x1, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},

                    //PblHashTable<EntityDoor,32>
                    patch{0xd930c + 0x2, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    patch{0xd8d76 + 0x1, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    patch{0xd9336 + 0x3, 0x20, 0x40 - 0x8},
                    patch{0xd8d98 + 0x1, 0x40, 0x80 - 0x8, {.values_are_8bit = true}},
                    patch{0xd8e5c + 0x2, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    patch{0xd8e16 + 0x2, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    patch{0xd8e2c + 0x2, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    patch{0xd8e05 + 0x3, 0x84, 0x104 - 0x8},
                    patch{0xd92c6 + 0x1, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    patch{0xd8dc0 + 0x1, 0x40, 0x80 - 0x8, {.values_are_8bit = true}},*/
                  },
            },

            patch_set{
               .name = "Flyer Tests - Skipped",
               .patches =
                  {
                    //msHostDoors
                    //patch{0x263db0, 0x80, 0x90, {.values_are_8bit = true}},
                    //patch{0x263db0 + 0x1, 0x89, 0x90, {.values_are_8bit = true}},
                    //patch{0x263db0 + 0x2, 0xf4, 0x90, {.values_are_8bit = true}},
                    //patch{0x263db0 + 0x3, 0x20000005, 0x90909090},
                  },
            },

            //patch_set{
            //   .name = "Animation Bank Increase",
            //   .patches =
            //      {
            //        //todo:
            //        //update SoldierAnimatorClass array size from 0xf7c0 to 0xf808 - DONE
            //        //double SoldierAnimatorClass::mAnimBank from 18 to 36 - DONE
            //        //update SoldierAnimatorClass::mAnimBank values from 0x12 to 0x24
            //        //update SoldierAnimatorClass::mAnimBankCount offsets from 0xf774 to 0xf7bc
            //        //update SoldierAnimatorClass::mJetPackMatrix offsets from 0xf780 to 0xf7c8
            //        //update AnimationFinder::mMaxCount values from 0x12 to 0x24
            //
            //        //patch{s_aBank_address + 0x20, 0x0, 0x714daa7a, {.file_offset = false}}, //human FNV-1a hash
            //
            //        //patch{0x1703c7 + 0x1, 0xacecf8, s_aBank_address, {.expected_is_va = true}},        //SoldierAnimationBank::_GetBank
            //        //patch{0x170491 + 0x1, 0xaced18, s_aBank_address + 0x20, {.expected_is_va = true}}, //SoldierAnimationBank::FindBank
            //
            //        //patch{0x17051b + 0x2, 0x10, 0x20, {.values_are_8bit = true}}, //SoldierAnimationBank::AddBank
            //
            //        //patch{0x136207 + 0x1, 0xf780, 0xf7c8},                        //EntitySoldier::Render mJetPackMatrix offset
            //        //patch{0x137ec3 + 0x2, 0xf780, 0xf7c8},                        //EntitySoldier::Render mJetPackMatrix offset
            //        
            //        //patch{0x17de45 + 0x2, 0xf774, 0xf7bc},                        //SoldierAnimatorClass::GetAnimation
            //        //patch{0x17de71 + 0x2, 0xf774, 0xf7bc},                        //SoldierAnimatorClass::GetAnimation
            //        
            //        //patch{0x17deeb + 0x2, 0xf774, 0xf7bc},                        //SoldierAnimatorClass::_AddBank mAnimBankCount offset
            //        //patch{0x17df04 + 0x2, 0xf774, 0xf7bc},                        //SoldierAnimatorClass::_AddBank mAnimBankCount offset
            //        //patch{0x17df12 + 0x2, 0x12, 0x24, {.values_are_8bit = true}}, //SoldierAnimatorClass::_AddBank mAnimBank loop count
            //        //patch{0x17df50 + 0x2, 0xf774, 0xf7bc},                        //SoldierAnimatorClass::_AddBank mAnimBankCount offset
            //        
            //        //patch{0x180c6d + 0x1, 0x12, 0x24},                            //SoldierAnimatorClass::SoldierAnimatorClass mAnimBank count
            //        //patch{0x180c8b + 0x2, 0xf774, 0xf7bc},                        //SoldierAnimatorClass::SoldierAnimatorClass mAnimBankCount offset
            //        
            //        //patch{0x181cc9 + 0x1, 0xf774, 0xf7bc},                        //SoldierAnimatorClass::_PostLoad mAnimBankCount offset
            //        //patch{0x181cdc + 0x7, 0x12, 0x24},                            //SoldierAnimatorClass::_PostLoad mMaxCount count
            //        //patch{0x184170 + 0x2, 0xf780, 0xf7c8},                        //SoldierAnimatorClass::_PostLoad mJetPackMatrix offset
            //        
            //        //patch{0x1817a9 + 0x1, 0xf7c0, 0xf808},                        //SoldierAnimatorClass::Create size
            //
            //        //patch{0x1707be + 0x2, 0x1e, 0x3c, {.values_are_8bit = true}}, //SoldierAnimationBank::AddMap compare size
            //
            //        //patch{0x17e135 + 0x1, 0x18, 0x30},                            //MapCache::MapCache loop size
            //        //patch{0x17e164 + 0x1, 0x18, 0x30},                            //MapCache::RemoveBank loop size
            //        //patch{0x17f2d0 + 0x2, 0x18, 0x30, {.values_are_8bit = true}}, //MapCache::AddMap loop size
            //        //patch{0x17f315 + 0x2, 0x18, 0x30, {.values_are_8bit = true}}, //MapCache::AddMap loop size
            //        //patch{0x17f341 + 0x2, 0x18, 0x30, {.values_are_8bit = true}}, //MapCache::AddMap compare size
            //        //patch{0x181cac + 0x1, 0x18, 0x30},                            //SoldierAnimatorClass::_PostLoad loop size
            //        //patch{0x181e18 + 0x1, 0x18, 0x30},                            //SoldierAnimatorClass::_PostLoad loop size
            //
            //        //patch{0x181af6 + 0x1, 0x9174, 0x12310},                       //SoldierAnimatorClass::_PostLoad stack size
            //
            //        //patch{0x170414 + 0x3, 0xacf558, s_aMap_address, {.expected_is_va = true}},
            //        //patch{0x170775 + 0x3, 0xacf558, s_aMap_address, {.expected_is_va = true}},
            //        //patch{0x17077e + 0x3, 0xacf55c, s_aMap_address + 0x4, {.expected_is_va = true}},
            //
            //        //patch{s_aMap_address + 0xc, 0x0, 0x1, {.file_offset = false, .values_are_8bit = true}},
            //        //patch{s_aMap_address + 0x14, 0x0, 0x2, {.file_offset = false, .values_are_8bit = true}},
            //        //patch{s_aMap_address + 0x1c, 0x0, 0x3, {.file_offset = false, .values_are_8bit = true}},
            //        //patch{s_aMap_address + 0x24, 0x0, 0x4, {.file_offset = false, .values_are_8bit = true}},
            //
            //      },
            //   .str_patches =
            //    {
            //        //strpatch{s_aBank_address, "", "human", {.file_offset = false}},
            //    }
            //},

            //patch_set{
            //   .name = "Tentacle Increase",
            //   .patches =
            //      {
            //        //todo:
            //        //change all offsets:
            //        //tPos                 - unchanged
            //        //oldPos               - 0x120 to 0x288
            //        //oldVelocity          - 0x240 to 0x510
            //        //mInternalTimer       - 0x24c to 0x51c
            //        //mTimeSinceLastUpdate - 0x250 to 0x520
            //        //mTimerOffset         - 0x254 to 0x524
            //        //mNumTentacles        - 0x258 to 0x528
            //        //mBonesPerTentacle    - 0x25c to 0x52c
            //        //mCollType            - 0x260 to 0x530
            //        //mFirstUpdate         - 0x264 to 0x534
            //        //increase relevant stack sizes
            //        //increase memory pool sizes to account for increases
            //
            //        //patch{0x16f4e5 + 0x2, 0x60, 0xc4, {.values_are_8bit = true}},                            //TentacleSimulator::DoTentacles stack sub
            //        //patch{0x16f4b0, 0xcc, 0x81, {.values_are_8bit = true}},                                    //SUB ESP
            //        //patch{0x16f4b0 + 0x1, 0xcc, 0xec, {.values_are_8bit = true}},                              //SUB ESP
            //        //patch{0x16f4b0 + 0x2, 0xcccccccc, 0x160},                                                  //0x160
            //        //patch{0x16f4b6, 0xcc, 0xeb, {.values_are_8bit = true}},                                    //JMP BACK
            //        //patch{0x16f4b6 + 0x1, 0xcc, 0x30, {.values_are_8bit = true}},                              //JMP BACK
            //        //patch{0x16f4e5, 0x83, 0xeb, {.values_are_8bit = true}},                                    //JMP TO
            //        //patch{0x16f4e5 + 0x1, 0xec, 0xc9, {.values_are_8bit = true}},                              //JMP TO
            //        //patch{0x16f4e5 + 0x2, 0x60, 0x90, {.values_are_8bit = true}},                              //JMP TO
            //        //patch{0x16f53c + 0x2, 0x24c, 0x51c},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f542 + 0x2, 0x254, 0x524},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f54f + 0x2, 0x250, 0x520},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f56a + 0x2, 0x250, 0x520},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f57c + 0x2, 0x24c, 0x51c},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f582 + 0x2, 0x254, 0x524},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f58c + 0x2, 0x250, 0x520},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f594 + 0x2, 0x24c, 0x51c},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f5e7 + 0x2, 0x24c, 0x51c},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f5f5 + 0x1, 0x100, 0x300},                                                     //TentacleSimulator::DoTentacles find size
            //        //patch{0x16f60b + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f620 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f630 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f63b + 0x3, 0xa442f0, TENTACLE_BONE_hashes_address, {.expected_is_va = true}}, //TentacleSimulator::DoTentacles bone_string_1 location
            //        //patch{0x16f64a + 0x1, 0x100, 0x300},                                                     //TentacleSimulator::DoTentacles find size
            //        //patch{0x16f65b + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f66d + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f683 + 0x2, 0x264, 0x534},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f691 + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f6c0 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f6db + 0x2, 0x120, 0x288},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f705 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f726 + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f74b + 0x2, 0x264, 0x534},                                                     //TentacleSimulator::DoTentacles offset
            //        //patch{0x16f7b0, 0xcc, 0x81, {.values_are_8bit = true}},                                    //ADD ESP
            //        //patch{0x16f7b0 + 0x1, 0xcc, 0xc4, {.values_are_8bit = true}},                              //ADD ESP
            //        //patch{0x16f7b0 + 0x2, 0xcccccccc, 0x160},                                                  //0x160
            //        //patch{0x16f7b6, 0xcc, 0xeb, {.values_are_8bit = true}},                                    //JMP BACK
            //        //patch{0x16f7b6 + 0x1, 0xcc, 0xf3, {.values_are_8bit = true}},                              //JMP BACK
            //        //patch{0x16f7a8, 0x83, 0xeb, {.values_are_8bit = true}},                                    //JMP TO
            //        //patch{0x16f7a8 + 0x1, 0xc4, 0x06, {.values_are_8bit = true}},                              //JMP TO
            //        //patch{0x16f7a8 + 0x2, 0x60, 0x90, {.values_are_8bit = true}},                              //JMP TO
            //        //patch{0x16f7a8 + 0x2,0x60, 0xc4, {.values_are_8bit = true}},                             //TentacleSimulator::DoTentacles stack add
            //
            //        //patch{0x16d436 + 0x2, 0x90, 0x1b0},                                                      //TentacleSimulator::EnforceBoxCollision stack sub
            //        //patch{0x16d430 + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::EnforceBoxCollision offset
            //        //patch{0x16d454 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::EnforceBoxCollision offset
            //        //patch{0x16d782 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::EnforceBoxCollision offset
            //        //patch{0x16d795 + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::EnforceBoxCollision offset
            //        //patch{0x16d7ab + 0x2, 0x90, 0x1b0},                                                      //TentacleSimulator::EnforceBoxCollision stack add
            //
            //        //patch{0x16f026 + 0x2, 0x104, 0x30c},                                                     //TentacleSimulator::EnforceCollisions stack sub
            //        //patch{0x16f05a + 0x2, 0x260, 0x530},                                                     //TentacleSimulator::EnforceCollisions offset
            //        //patch{0x16f294 + 0x2, 0x260, 0x530},                                                     //TentacleSimulator::EnforceCollisions offset
            //
            //        //patch{0x16d8a6 + 0x2, 0xa8, 0x1f8},                                                      //TentacleSimulator::EnforceCylinderCollision stack sub
            //        //patch{0x16d8a0 + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::EnforceCylinderCollision offset
            //        //patch{0x16d8c4 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::EnforceCylinderCollision offset
            //        //patch{0x16db7a + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::EnforceCylinderCollision offset
            //        //patch{0x16db8f + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::EnforceCylinderCollision offset
            //        //patch{0x16dba5 + 0x2,0xa8, 0x1f8},                                                       //TentacleSimulator::EnforceCylinderCollision stack add
            //
            //        //patch{0x16d296 + 0x2, 0x40, 0xc0, {.values_are_8bit = true}},                            //TentacleSimulator::EnforceSphereCollision stack sub
            //        //patch{0x16d290 + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::EnforceSphereCollision offset
            //        //patch{0x16d2b2 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::EnforceSphereCollision offset
            //        //patch{0x16d3a3 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::EnforceSphereCollision offset
            //        //patch{0x16d3bc + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::EnforceSphereCollision offset
            //        //patch{0x16d3d6 + 0x2, 0x40, 0xc0, {.values_are_8bit = true}},                            //TentacleSimulator::EnforceSphereCollision stack add
            //
            //        //patch{0x16d090 + 0x2, 0x14, 0x3c, {.values_are_8bit = true}},                            //TentacleSimulator::TentacleSimulator stack sub
            //        //patch{0x16d097 + 0x2, 0x248, 0x518},                                                     //TentacleSimulator::TentacleSimulator offset
            //        //patch{0x16d09d + 0x2, 0x244, 0x514},                                                     //TentacleSimulator::TentacleSimulator offset
            //        //patch{0x16d0a3 + 0x2, 0x240, 0x510},                                                     //TentacleSimulator::TentacleSimulator offset
            //        //patch{0x16d0a9 + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::TentacleSimulator offset
            //        //patch{0x16d0c1 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::TentacleSimulator offset
            //        //patch{0x16d0f1 + 0x2, 0x120, 0x288},                                                     //TentacleSimulator::TentacleSimulator offset
            //        //patch{0x16d10b + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::TentacleSimulator offset
            //        //patch{0x16d123 + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::TentacleSimulator offset
            //        //patch{0x16d141 + 0x2, 0x24c, 0x51c},                                                     //TentacleSimulator::TentacleSimulator offset
            //        //patch{0x16d147 + 0x2, 0x250, 0x520},                                                     //TentacleSimulator::TentacleSimulator offset
            //        //patch{0x16d14d + 0x2, 0x254, 0x524},                                                     //TentacleSimulator::TentacleSimulator offset
            //        //patch{0x16d157 + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::TentacleSimulator offset
            //        //patch{0x16d161 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::TentacleSimulator offset
            //        //patch{0x16d167 + 0x2, 0x260, 0x530},                                                     //TentacleSimulator::TentacleSimulator offset
            //        //patch{0x16d16d + 0x2, 0x264, 0x534},                                                     //TentacleSimulator::TentacleSimulator offset
            //        //patch{0x16d174 + 0x2, 0x14, 0x3c, {.values_are_8bit = true}},                            //TentacleSimulator::TentacleSimulator stack add
            //
            //        //patch{0x16dc86 + 0x2, 0x284, 0x78c},                                                   //TentacleSimulator::UpdatePose stack sub
            //        //patch{0x16dc9e + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::UpdatePose offset
            //        //patch{0x16dcc2 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::UpdatePose offset
            //        //patch{0x16e1c3 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::UpdatePose offset
            //        //patch{0x16e1d4 + 0x3, 0xa442f0, TENTACLE_BONE_hashes_address, {.expected_is_va = true}}, //TentacleSimulator::UpdatePose offset
            //        //patch{0x16e1dc + 0x1, 0x100, 0x300},                                                     //TentacleSimulator::UpdatePose remove size
            //        //patch{0x16e1fa + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::UpdatePose offset
            //        //patch{0x16e207 + 0x3, 0xa442f0, TENTACLE_BONE_hashes_address, {.expected_is_va = true}}, //TentacleSimulator::UpdatePose offset
            //        //patch{0x16e210 + 0x1, 0x100, 0x300},                                                     //TentacleSimulator::UpdatePose store size
            //        //patch{0x16e233 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::UpdatePose offset
            //        //patch{0x16e264 + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::UpdatePose offset
            //
            //        //patch{0x16e426 + 0x2, 0x214, 0x63c},                                                     //TentacleSimulator::UpdatePositions stack sub
            //        //patch{0x16e518 + 0x2, 0x240, 0x510},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16e52e + 0x2, 0x244, 0x514},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16e544 + 0x2, 0x248, 0x518},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16e656 + 0x2, 0x240, 0x510},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16e65c + 0x2, 0x244, 0x514},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16e662 + 0x2, 0x248, 0x518},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16e797 + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16e7b3 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16e7b9 + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16e7cb + 0x2, 0x130, 0x288 + 0x10},                                              //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16e7eb + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16e9eb + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16ea20 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16eb83 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16eba6 + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16ebca + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16ebfc + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16ec11 + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16ec30 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16ec6d + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16ed73 + 0x2, 0x25c, 0x52c},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16ed89 + 0x2, 0x258, 0x528},                                                     //TentacleSimulator::UpdatePositions offset
            //        //patch{0x16ed93 + 0x2, 0x5, 0x9, {.values_are_8bit = true} },                            //TentacleSimulator::UpdatePositions possible loop size
            //
            //        //patch{0x16d1e9 + 0x2, 0x250, 0x520},                                                     //TentacleSimulator::UpdateTimer offset
            //        //patch{0x16d1fc + 0x2, 0x24c, 0x51c},                                                     //TentacleSimulator::UpdateTimer offset
            //        //patch{0x16d206 + 0x2, 0x250, 0x520},                                                     //TentacleSimulator::UpdateTimer offset
            //        //patch{0x16d20c + 0x2, 0x24c, 0x51c},                                                     //TentacleSimulator::UpdateTimer offset
            //        //patch{0x16d212 + 0x2, 0x250, 0x520},                                                     //TentacleSimulator::UpdateTimer offset
            //        //patch{0x16d218 + 0x2, 0x254, 0x524},                                                     //TentacleSimulator::UpdateTimer offset
            //        //patch{0x16d225 + 0x2, 0x250, 0x520},                                                     //TentacleSimulator::UpdateTimer offset
            //        //patch{0x16d22b + 0x2, 0x24c, 0x51c},                                                     //TentacleSimulator::UpdateTimer offset
            //        //patch{0x16d231 + 0x2, 0x24c, 0x51c},                                                     //TentacleSimulator::UpdateTimer offset
            //        //patch{0x16d23a + 0x2, 0x24c, 0x51c},                                                     //TentacleSimulator::UpdateTimer offset
            //        //patch{0x16d240 + 0x2, 0x250, 0x520},                                                     //TentacleSimulator::UpdateTimer offset
            //        //patch{0x16d24d + 0x2, 0x24c, 0x51c},                                                     //TentacleSimulator::UpdateTimer offset
            //        //patch{0x16d253 + 0x2, 0x24c, 0x51c},                                                     //TentacleSimulator::UpdateTimer offset
            //
            //        //patch{0x141cd3 + 0x2, 0x4, 0x9, {.values_are_8bit = true}},                              //EntitySoldierClass::SetProperty limit
            //
            //        //patch{0x135d96 + 0x2, 0xb84, 0x228c},                                                    //EntitySoldierClass::Render stack sub
            //        //patch{0x16fe86 + 0x2, 0xa24, 0x1e6c},                                                    //AnimatedAddon::Render stack sub
            //        //patch{0x274896 + 0x2, 0xa34, 0x1e9c},                                                    //AnimatedAddon::Render stack sub
            //
            //        //patch{0x2745cc + 0x1, 0x268, 0x538},                                                     //TentacleSimulator::sMemoryPool size
            //        //patch{0x617180 + 0x1, 0x268, 0x538},                                                     //TentacleSimulator::sMemoryPool size
            //
            //      },
            //},
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
                    patch{0x8de7d, 0x1f4, DLC_mission_patch_limit},                                                         // AddDownloadableContent
                    patch{0x8de9f, 0x1e31f00, DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}},                           // AddDownloadableContent
                    patch{0x8dec3, 0x1e31f04, (0x1e31f04 - 0x1e31f00) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
                    patch{0x8dec9, 0x1e31f08, (0x1e31f08 - 0x1e31f00) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
                    patch{0x8def0, 0x1e3200b, (0x1e3200b - 0x1e31f00) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
                    patch{0x8def7, 0x1e3200c, (0x1e3200c - 0x1e31f00) + DLC_mission_table_address, {.file_offset = true, .expected_is_va = true}}, // AddDownloadableContent
                    patch{0x8df28, 0x1e31f00, DLC_mission_table_address, {.expected_is_va = true}},                           // SetCurrentMap
                    patch{0x8df68, 0x1e31f04, (0x1e31f04 - 0x1e31f00) + DLC_mission_table_address, {.expected_is_va = true}}, // SetCurrentMission
                    patch{0x8dfb4, 0x1e31f08, (0x1e31f08 - 0x1e31f00) + DLC_mission_table_address, {.expected_is_va = true}}, // GetContentDirectory
                    patch{0x8dfce, 0x1e31f04, (0x1e31f04 - 0x1e31f00) + DLC_mission_table_address, {.expected_is_va = true}}, // IsMissionDownloaded
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
                    patch{0x8de7d, 0x1f4, DLC_mission_patch_limit},                                                           // AddDownloadableContent
                    patch{0x8de9f, 0x1e30950, DLC_mission_table_address, {.expected_is_va = true}},                           // AddDownloadableContent
                    patch{0x8dec3, 0x1e30954, (0x1e30954 - 0x1e30950) + DLC_mission_table_address, {.expected_is_va = true}}, // AddDownloadableContent
                    patch{0x8dec9, 0x1e30958, (0x1e30958 - 0x1e30950) + DLC_mission_table_address, {.expected_is_va = true}}, // AddDownloadableContent
                    patch{0x8def0, 0x1e30a5b, (0x1e30a5b - 0x1e30950) + DLC_mission_table_address, {.expected_is_va = true}}, // AddDownloadableContent
                    patch{0x8def7, 0x1e30a5c, (0x1e30a5c - 0x1e30950) + DLC_mission_table_address, {.expected_is_va = true}}, // AddDownloadableContent
                    patch{0x8df28, 0x1e30950, DLC_mission_table_address, {.expected_is_va = true}},                           // SetCurrentMap
                    patch{0x8df68, 0x1e30954, (0x1e30954 - 0x1e30950) + DLC_mission_table_address, {.expected_is_va = true}}, // SetCurrentMission
                    patch{0x8dfb4, 0x1e30958, (0x1e30958 - 0x1e30950) + DLC_mission_table_address, {.expected_is_va = true}}, // GetContentDirectory
                    patch{0x8dfce, 0x1e30954, (0x1e30954 - 0x1e30950) + DLC_mission_table_address, {.expected_is_va = true}}, // IsMissionDownloaded
                  },
            },
            patch_set{
               .name = "Sound Limit Extension",
               .patches =
                  {
                     patch{0x3319b2 + 0x1, 0x9cfdb8, smSampleRAMBitmapNew_address, {.expected_is_va = true}}, // Snd::Engine::Open
                     patch{0x3319bc + 0x1, 0x2000000, 0x10000000},          // malloc call 1 arg
                     patch{0x3317f7 + 0x1, 0x2000000, 0x10000000},          // malloc call 2 arg
                  },
            },

            patch_set{
               .name = "High-Res Animation Limit Extension",
               .patches =
                  {
                    //8-bit push patch
                    patch{0x2467d2, 0x6a, 0xe9, {.values_are_8bit = true}},       //JMP to 0x247098 
                    patch{0x2467d2 + 0x1, 0x10708d32, 0x8c1},                     //JMP to 0x247098
                    patch{0x247098, 0xcc, 0x68, {.values_are_8bit = true}},       //PUSH 0x14f
                    patch{0x247098 + 0x1, 0xcccccccc, 0x14f,},                   //PUSH 0x14f
                    patch{0x24709d, 0xcc, 0xeb, {.values_are_8bit = true}},       //JMP to 0x2470a4
                    patch{0x24709d + 0x1, 0xcc, 0x05, {.values_are_8bit = true}}, //JMP to 0x2470a4
                    patch{0x2470a4, 0xcc, 0x8d, {.values_are_8bit = true}},       //LEA ESI,[EAX + 0x10]
                    patch{0x2470a4 + 0x1, 0xcc, 0x70, {.values_are_8bit = true}}, //LEA ESI,[EAX + 0x10]
                    patch{0x2470a4 + 0x2, 0xcc, 0x10, {.values_are_8bit = true}}, //LEA ESI,[EAX + 0x10]
                    patch{0x2470a7, 0xcc, 0xe9, {.values_are_8bit = true}},       //JMP to 0x2467d7
                    patch{0x2470a7 + 0x1, 0xcccccccc, 0xfffff72b},                //JMP to 0x2467d7

                    patch{0x2467d7 + 0x2, 0x32, 0x14f},                            // 32-bit signed int, must match above
                    patch{0x246839 + 0x2, 0x64640, 0x14f * 0x2020},                // array size 0x14f * 0x2020
                    patch{0x242c62 + 0x1, 0x64640, 0x14f * 0x2020},                // array size 0x14f * 0x2020
                    patch{0x2467b0 + 0x1, 0x64650, 0x14f * 0x2020 + 0x10},         // heap allocation = array size + 16 (0x7f * 0x2020 + 0x10)
                  },
            },

            patch_set{
               .name = "Matrix/Item Pool Limit Extension",
               .patches =
                  {
                    patch{0x2af682 + 0x1, 0x8bef50, matrixPool_address, {.expected_is_va = true}},
                    patch{0x2af6ef + 0x2, 0x8bef50, matrixPool_address, {.expected_is_va = true}},
                    patch{0x2b7da7 + 0x2, 0x8bef50, matrixPool_address, {.expected_is_va = true}},
                    patch{0x6992 + 0x1, 0x8bef50, matrixPool_address, {.expected_is_va = true}},
                    patch{0x2af68a + 0x2, 0xbf6, matrixPool_size},
                    patch{0x2af6f8 + 0x1, 0xbf6, matrixPool_size},
                    patch{0x6997 + 0x1, 0xbf5, matrixPool_size - 1},
                    patch{0x6b10 + 0x1, 0x320, 0x32000},                        //transparentItemsSize
                    patch{0x6a80 + 0x1, 0x200, 0x20000},                        //postTransparentItemSize

                    //preShadowTransparentItemSize patch
                    patch{0x6ab0, 0x6a, 0xeb, {.values_are_8bit = true}},       //JMP to 0x6ad3
                    patch{0x6ab0 + 0x1, 0x64, 0x21, {.values_are_8bit = true}}, //JMP to 0x6ad3
                    patch{0x6ad3, 0xcc, 0x68, {.values_are_8bit = true}},       //PUSH 0x6400
                    patch{0x6ad3 + 0x1, 0xcccccccc, 0x6400},                    //PUSH 0x6400
                    patch{0x6ad8, 0xcc, 0xeb, {.values_are_8bit = true}},       //JMP to 0x6ab2
                    patch{0x6ad8 + 0x1, 0xcc, 0xd8, {.values_are_8bit = true}}, //JMP to 0x6ab2
                  },
            },

            //patch_set{
            //   .name = "nearScene Extension",
            //   .patches =
            //      {
            //         patch{0x2be533 + 0x6, 0x0, 0x1, {.values_are_8bit = true}},
            //      },
            //},

            patch_set{
               .name = "renderRadius Limit Extension",
               .patches =
                  {
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
                     patch{0x2bbcc9 + 0x1, 0xc8, 0xc8 * 0x10},                      // SetClassMaxCost::MaxCount    - modelClass (0)
                     patch{0x2bbcc4 + 0x1, 0xc350, 0xc350 * 0x10},                  // SetClassMaxCost::MaxCostLOD0 - modelClass (0)
                     patch{0x2bbcb5 + 0x1, 0x9c40, 0x9c40 * 0x10},                  // SetClassMaxCost::MaxCostLOD3 - modelClass (0)

                     patch{0x2bbc31 + 0x1, 0x258, 0x258 * 0x10},                    // SetClassMaxCost::MaxCount    - bigModelClass (1)
                     patch{0x2bbc2c + 0x1, 0x186a0, 0x186a0 * 0x10},                // SetClassMaxCost::MaxCostLOD0 - bigModelClass (1)
                     patch{0x2bbc1d + 0x1, 0x9c40, 0x9c40 * 0x10},                  // SetClassMaxCost::MaxCostLOD3 - bigModelClass (1)

                     patch{0x2bbbf6 + 0x1, 0x64, 0x7f, {.values_are_8bit = true}},  // SetClassMaxCost::MaxCount    - soldierClass (2) 8-bit signed int, needs relocating to increase it
                     patch{0x2bbbf1 + 0x1, 0x4650, 0x4650 * 0x10},                  // SetClassMaxCost::MaxCostLOD0 - soldierClass (2)
                     patch{0x2bbbe5 + 0x1, 0x9c40, 0x9c40 * 0x10},                  // SetClassMaxCost::MaxCostLOD3 - soldierClass (2)

                     patch{0x2bbc81 + 0x1, 0x5dc, 0x5dc * 0x10},                    // SetClassMaxCost::MaxCount    - hugeModelClass (3) - UBER
                     patch{0x2bbc7c + 0x1, 0x2710, 0x2710 * 0x10},                  // SetClassMaxCost::MaxCostLOD0 - hugeModelClass (3) - UBER

                     patch{0x2bbc97 + 0x1, 0x12c, 0x12c * 0x10},                    // SetClassMaxCost::MaxCount    - hugeModelClass (3)
                     patch{0x2bbc92 + 0x1, 0x3e8, 0x3e8 * 0x10},                    // SetClassMaxCost::MaxCostLOD0 - hugeModelClass (3)
                     patch{0x2bbc6b + 0x1, 0x9c40, 0x9c40 * 0x10},                  // SetClassMaxCost::MaxCostLOD3 - hugeModelClass (3)
                  },
            },

            patch_set{
               .name = "Far Objects Render Extension",
               .patches =
                  {
                    patch{0x2e3688 + 0x1, 0x201, 0x801},                         //RedScene::SetupStaticWorld heap pool size
                    patch{0x2e36ab + 0x3, 0x200, 0x800},                         //RedScene::SetupStaticWorld heap maxCount
                    
                    patch{0x2e22d3 + 0x2, 0x410, 0x1410},                        //RenderFarObjects sub stack
                    patch{0x2e230c + 0x1, 0x100, 0x400},                         //RenderFarObjects sMaxNumFarObjects minimum
                    patch{0x2e231e + 0x2, 0xa, 0x28, {.values_are_8bit = true}}, //RenderFarObjects class 2 count
                    patch{0x2e2351 + 0x3, 0xfffffbfc, 0xffffebfc},               //RenderFarObjects offset relocation
                    patch{0x2e235d + 0x3, 0xfffffbfc, 0xffffebfc},               //RenderFarObjects offset relocation
                    patch{0x2e23f0 + 0x3, 0xfffffbfc, 0xffffebfc},               //RenderFarObjects offset relocation
                    patch{0x2e24c6 + 0x3, 0xfffffbfc, 0xffffebfc},               //RenderFarObjects offset relocation
                    patch{0x2e24e1 + 0x3, 0xfffffbfc, 0xffffebfc},               //RenderFarObjects offset relocation
                    //patch{0x3efcfa + 0x2, 0x42c, 0x10b0},                      //RenderFarObjects add stack

                    patch{0x2e178b + 0x1, 0x401, 0x1001},                        //RedSceneManager::RedSceneManager heap pool size
                    patch{0x2e17ae + 0x3, 0x400, 0x1000},                        //RedSceneManager::RedSceneManager heap maxCount*/
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
                     patch{0x13a543 + 0x1, 0x1770, 0x177000},
                  },
            },

            patch_set{
               .name = "LightingState Pool Increase - Skipped",
               .patches =
                  {
                    /*//lightingStatePool pointers
                    patch{0x6974 + 0x1, 0x86ad10, lightingStatePool_address, {.expected_is_va = true}},
                    patch{0x2af6b7 + 0x2, 0x86ad10, lightingStatePool_address, {.expected_is_va = true}},
                    patch{0x2afc6f + 0x1, 0x8bed10, lightingStatePoolOffset_address, {.expected_is_va = true}},
                    patch{0x2afcbc + 0x1, 0x8bedb8, lightingStatePoolOffset_address + 0xa8, {.expected_is_va = true}},
                    patch{0x2afccd + 0x1, 0x8bee60, lightingStatePoolOffset_address + (0xa8 * 0x2), {.expected_is_va = true}},
                    patch{0x2afcda + 0x6, 0x8bed10, lightingStatePoolOffset_address, {.expected_is_va = true}},
                    patch{0x2b015a + 0x6, 0x8bed10, lightingStatePoolOffset_address, {.expected_is_va = true}},
                    patch{0x2b01ef + 0x3, 0x8bedb8, lightingStatePoolOffset_address + 0xa8, {.expected_is_va = true}},
                    patch{0x2b05c0 + 0x1, 0x8bee60, lightingStatePoolOffset_address + (0xa8 * 0x2), {.expected_is_va = true}},
                    patch{0x2b12b5 + 0x1, 0x8bed10, lightingStatePoolOffset_address, {.expected_is_va = true}},
                    patch{0x2b12e4 + 0x1, 0x8bedb8, lightingStatePoolOffset_address + 0xa8, {.expected_is_va = true}},
                    patch{0x2b12f5 + 0x1, 0x8bee60, lightingStatePoolOffset_address + (0xa8 * 0x2), {.expected_is_va = true}},
                    patch{0x2b1310 + 0x4, 0x8bed2c, lightingStatePoolOffset_address + 0x1c, {.expected_is_va = true}},
                    patch{0x2b1320 + 0x2, 0x8bed14, lightingStatePoolOffset_address + 0x4, {.expected_is_va = true}},
                    patch{0x2b1326 + 0x1, 0x8bed14, lightingStatePoolOffset_address + 0x4, {.expected_is_va = true}},
                    patch{0x2b132b + 0x4, 0x8bed30, lightingStatePoolOffset_address + 0x20, {.expected_is_va = true}},
                    patch{0x2b1335 + 0x2, 0x8bed80, lightingStatePoolOffset_address + 0x70, {.expected_is_va = true}},
                    patch{0x2b134c + 0x2, 0x8bee28, lightingStatePoolOffset_address + 0x118, {.expected_is_va = true}},
                    patch{0x2b1358 + 0x6, 0x8bed10, lightingStatePoolOffset_address, {.expected_is_va = true}},
                    patch{0x2b1368 + 0x4, 0x8bed40, lightingStatePoolOffset_address + 0x30, {.expected_is_va = true}},
                    patch{0x2b1370 + 0x1, 0x8bed14, lightingStatePoolOffset_address + 0x4, {.expected_is_va = true}},
                    patch{0x2b1382 + 0x1, 0x8bed18, lightingStatePoolOffset_address + 0x8, {.expected_is_va = true}},
                    patch{0x2b138c + 0x4, 0x8bed50, lightingStatePoolOffset_address + 0x40, {.expected_is_va = true}},
                    patch{0x2b1394 + 0x1, 0x8bed1c, lightingStatePoolOffset_address + 0xc, {.expected_is_va = true}},
                    patch{0x2b13a6 + 0x1, 0x8bed20, lightingStatePoolOffset_address + 0x10, {.expected_is_va = true}},
                    patch{0x2b13b0 + 0x4, 0x8bed60, lightingStatePoolOffset_address + 0x50, {.expected_is_va = true}},
                    patch{0x2b13b8 + 0x1, 0x8bed24, lightingStatePoolOffset_address + 0x14, {.expected_is_va = true}},
                    patch{0x2b13ca + 0x1, 0x8bed28, lightingStatePoolOffset_address + 0x18, {.expected_is_va = true}},
                    patch{0x2b13cf + 0x4, 0x8bed70, lightingStatePoolOffset_address + 0x60, {.expected_is_va = true}},
                    patch{0x2b13d7 + 0x2, 0x8bedbc, lightingStatePoolOffset_address + 0xac, {.expected_is_va = true}},
                    patch{0x2b13e5 + 0x1, 0x8bedbc, lightingStatePoolOffset_address + 0xac, {.expected_is_va = true}},
                    patch{0x2b13ea + 0x4, 0x8bedd4, lightingStatePoolOffset_address + 0xc4, {.expected_is_va = true}},
                    patch{0x2b140d + 0x4, 0x8bedd8, lightingStatePoolOffset_address + 0xc8, {.expected_is_va = true}},
                    patch{0x2b1415 + 0x1, 0x8bedbc, lightingStatePoolOffset_address + 0xac, {.expected_is_va = true}},
                    patch{0x2b1427 + 0x1, 0x8bedc0, lightingStatePoolOffset_address + 0xb0, {.expected_is_va = true}},
                    patch{0x2b1431 + 0x4, 0x8bede8, lightingStatePoolOffset_address + 0xd8, {.expected_is_va = true}},
                    patch{0x2b1439 + 0x1, 0x8bedc4, lightingStatePoolOffset_address + 0xb4, {.expected_is_va = true}},
                    patch{0x2b144b + 0x1, 0x8bedc8, lightingStatePoolOffset_address + 0xb8, {.expected_is_va = true}},
                    patch{0x2b1455 + 0x4, 0x8bedf8, lightingStatePoolOffset_address + 0xe8, {.expected_is_va = true}},
                    patch{0x2b145d + 0x1, 0x8bedcc, lightingStatePoolOffset_address + 0xbc, {.expected_is_va = true}},
                    patch{0x2b146f + 0x1, 0x8bedd0, lightingStatePoolOffset_address + 0xc0, {.expected_is_va = true}},
                    patch{0x2b1474 + 0x4, 0x8bee08, lightingStatePoolOffset_address + 0xa8 + 0x50, {.expected_is_va = true}},
                    patch{0x2b1484 + 0x4, 0x8bee18, lightingStatePoolOffset_address + 0xa8 + 0x60, {.expected_is_va = true}},
                    patch{0x2b28b6 + 0x1, 0x8bee60, lightingStatePoolOffset_address + (0xa8 * 0x2), {.expected_is_va = true}},
                    patch{0x2b2b77 + 0x2, 0x86ad10, lightingStatePool_address, {.expected_is_va = true}},
                    patch{0x2b57df + 0x1, 0x8bed10, lightingStatePoolOffset_address, {.expected_is_va = true}},
                    patch{0x2b57e4 + 0x1, 0x8bedb8, lightingStatePoolOffset_address + 0xa8, {.expected_is_va = true}},
                    patch{0x2b62e5 + 0x6, 0x8bed10, lightingStatePoolOffset_address, {.expected_is_va = true}},
                    patch{0x2b7e2b + 0x1, 0x8bed10, lightingStatePoolOffset_address, {.expected_is_va = true}},
                    patch{0x2b7e30 + 0x1, 0x8bedb8, lightingStatePoolOffset_address + 0xa8, {.expected_is_va = true}},
                    patch{0x2b8493 + 0x3, 0x8bedb8, lightingStatePoolOffset_address + 0xa8, {.expected_is_va = true}},
                    patch{0x368bff + 0x1, 0x86ad10, lightingStatePool_address, {.expected_is_va = true}},
            
                    //ops
                    patch{0x696a + 0x1, 0x803, 0x80003},
                    patch{0x2af6bd + 0x1, 0x800, 0x80000},
                    patch{0x368bf5 + 0x1, 0x803, 0x80003},*/
                  },
            },

            patch_set{
               .name = "RenderItem Pool Increase",
               .patches =
                  {
                     patch{0x6aea + 0x1, 0x128c, 0x128c00},
                     patch{0x6af1 + 0x1, 0x8174f0, renderItemPool_address, {.expected_is_va = true}},
                     patch{0x2b7d26 + 0x2, 0x128c, 0x128c00},
                     patch{0x2b01a6 + 0x3, 0x8174f0, renderItemPool_address, {.expected_is_va = true}},
                     patch{0x2b7d42 + 0x3, 0x8174f0, renderItemPool_address, {.expected_is_va = true}},
                     patch{0x2b83ca + 0x3, 0x8174f0, renderItemPool_address, {.expected_is_va = true}},
                     patch{0x368c45 + 0x1, 0x128c, 0x128c00},
                     patch{0x368c4c + 0x1, 0x8174f0, renderItemPool_address, {.expected_is_va = true}}, //*/
                  },
            },

            patch_set{
               .name = "Bone Transforms Patch",
               .patches =
                  {
                    patch{0x243563 + 0x3, 0x7ae72c, BONE_hashes_address, {.expected_is_va = true}},
                  },
            },

            patch_set{
               .name = "Combo Anims Increase",
               .patches =
                  {
                    patch{0x23b823 + 0x3, 0x1eaf0a0, s_aComboAnimation_address, {.expected_is_va = true}},        //SoldierAnimationBank::_GetComboAnimation
                    patch{0x23c00d + 0x1, 0x1eaf0c0, s_aComboAnimation_address + 0x20, {.expected_is_va = true}}, //SoldierAnimationBank::FindComboAnimation
                    patch{0x23c0d0 + 0x2, 0x1e, 0x5a, {.values_are_8bit = true}},                                 //SoldierAnimationBank::AddComboAnimation
                    patch{0x24986d + 0x2, 0x1e, 0x5a, {.values_are_8bit = true}},                                 //SoldierAnimType::IsWeaponMeleeAnimIndex
                    //patch{0x243d4c + 0x2, 0x1e, 0x5b, {.values_are_8bit = true}},                                 //SetupVelocity

                    //patch{0x17ef41 + 0x2, 0x1e, 0x78, {.values_are_8bit = true}}, //SetupVelocity

                    patch{0x23c09f + 0x3, 0x1eaf710, s_aeComboAnimationPool_address, {.expected_is_va = true}},  //SoldierAnimationBank::AddComboAnimation
                    patch{0x23c16b + 0x3, 0x1eaf710, s_aeComboAnimationPool_address, {.expected_is_va = true}},  //SoldierAnimationBank::GetComboAnimationIndex
                    patch{0x23c080 + 0x2, 0x100, 0x300},                                                         //SoldierAnimationBank::AddComboAnimation
                    patch{0x23c152 + 0x2, 0x100, 0x300},                                                         //SoldierAnimationBank::GetComboAnimationIndex

                    //patch{0x23c9b6 + 0x1, 0x1004, 0x2004}, //s_pAnimationNameTable array size
                    //patch{0x23ca8d + 0x1, 0x400, 0x800}, //s_pAnimationNameTable store size
                    //patch{0x23cb05 + 0x1, 0x400, 0x800}, //s_pAnimationNameTable store size
                    //patch{0x23cb85 + 0x1, 0x400, 0x800}, //s_pAnimationNameTable store size
                    //patch{0x23cc1f + 0x1, 0x400, 0x800}, //s_pAnimationNameTable find size
                    //patch{0x23cc52 + 0x1, 0x400, 0x800}, //s_pAnimationNameTable store size
                    //patch{0x23d7e0 + 0x1, 0x400, 0x800}, //s_pAnimationNameTable find size
                    patch{0x23d7f7 + 0x1, 0x148, 0x1fc}, //s_pAnimationNameTable upper limit size
                    //patch{0x23d857 + 0x1, 0x400, 0x800}, //s_pAnimationNameTable find size
                    //patch{0x23d886 + 0x1, 0x400, 0x800}, //s_pAnimationNameTable store size

                    patch{0x23d17b + 0x1, 0xf60, 0x17d0}, //SoldierAnimationData::InitAnimationData SoldierAnimationData size
                    patch{0x23d2ec + 0x2, 0xa4, 0xfe},    //SoldierAnimationData::InitAnimationData SoldierAnimationData size

                    patch{0x249905 + 0x2, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimType::GetAnimFromAnimIndex

                    patch{0x23dc34 + 0x7, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SoldierAnimator
                    patch{0x23dc34 + 0x8, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SoldierAnimator
                    patch{0x23de9f + 0x7, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SetNewOwner
                    patch{0x23de9f + 0x8, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SetNewOwner

                    patch{0x23fce7 + 0x2, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::UpdateActionAnimation
                    patch{0x23fe02 + 0x1, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::UpdateActionAnimation
                    patch{0x23fe8b + 0x6, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::UpdateActionAnimation

                    patch{0x2405a2 + 0x1, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::UpdateMovementAnimation
                    patch{0x240675 + 0x1, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::UpdateMovementAnimation
                    patch{0x240865 + 0x6, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::UpdateMovementAnimation
                    patch{0x24086c + 0x1, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::UpdateMovementAnimation

                    patch{0x23f2d2 + 0x6, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SetupPose

                    patch{0x247c83 + 0x2, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimatorLowResClass::PostLoad
                    patch{0x247d56 + 0x2, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimatorLowResClass::PostLoad
                    patch{0x247bc0 + 0x2, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimatorLowResClass::PostLoad

                    patch{0x23ebd7 + 0x7, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SetWeaponAnimationMap
                    patch{0x23ebd7 + 0x8, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SetWeaponAnimationMap

                    patch{0x23edd6 + 0x6, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SetWeaponComboState
                    patch{0x23eddd + 0x1, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SetWeaponComboState
                    patch{0x23edfe + 0x1, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SetWeaponComboState
                    patch{0x23ee0f + 0x6, 0xa4, 0xfe, {.values_are_8bit = true}}, //SoldierAnimator::SetWeaponComboState

                    patch{0xe2838 + 0x1, 0xa4, 0xfe},                            //EntitySoldier::Render
                    patch{0xe283d + 0x1, 0xa4, 0xfe},                            //EntitySoldier::Render
                    patch{0xe2778 + 0x1, 0xa4, 0xfe},                            //EntitySoldier::Render
                    patch{0xe274a + 0x1, 0xa4, 0xfe},                            //EntitySoldier::Render

                    //notes:
                    //0x1e8 (488) is the diff between SoldierAnimation *[30][2] - SoldierAnimation *[91][2] (mWeaponMeleeAnimation)
                    //SoldierAnimation *[1][2] = 0x8
                    //Therefore:
                    //offsetDiff = (91 - 30 = 61) * 0x8 = 488
                    //0x4b4 + 0x1e8 = 0x69c
                    //patch{0x17deb0 + 0x3, 0x4b4, 0x69c},                          //SoldierAnimatorClass::GetCustomAnimation

                    //patch{0x1f5cf7 + 0x1, 0xa4, 0xff, {.values_are_8bit = true}}, //FUN_005f5bb0
                    //patch{0x1f6c63 + 0x3, 0xa4, 0xff, {.values_are_8bit = true}}, //FUN_005f6b20
                    //patch{0x1f7754 + 0x3, 0xa4, 0xff, {.values_are_8bit = true}}, //FUN_005f7600
                    //patch{0x200af3 + 0x2, 0xa4, 0xff, {.values_are_8bit = true}}, //FUN_00600990
                    //patch{0x1f6094 + 0x2, 0xa4, 0xff, {.values_are_8bit = true}}, //FUN_005f6090

                    //patch{0x1778b3 + 0x2, 0xa4, 0xff, {.values_are_8bit = true}}, //g_fnAnim_Data
                    //patch{0x177d4a + 0x1, 0xa4, 0xff},                            //g_fnAnim_Data

                    //notes:
                    //0x1e8 (488) is the diff between SoldierAnimation *[30][2] - SoldierAnimation *[91][2] (mWeaponMeleeAnimation)
                    //It has to be added to the main stack for each CachedBankStack (24 total)
                    //Therefore:
                    //StackDiff = 0x1e8 (488) * 24 = 0x2dc0 (11712)
                    //0x9174 + 0x2dc0 = 0xbf34
                    //patch{0x181af6 + 0x1, 0x9174, 0xbf34},                        //SoldierAnimatorClass::_PostLoad
                  },
            },

            patch_set{
               .name = "Object Increase - Skipped",
               .patches =
                  {
                    /*//EntityEx::mIdMap needs relocating
                    //any and all references to it and to PblHashTable<EntityEx,1024> need increasing at the same ratio

                    //EntityEx::mIdMap address pointers
                    patch{0x1c65 + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x1c72 + 0x2, 0x1eb9870, EntityEx_mIdMap_address, {.expected_is_va = true}},
                    patch{0x6cf36 + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x6cf8a + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x90ba3 + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x90bb6 + 0x2, 0x1eb9870, EntityEx_mIdMap_address, {.expected_is_va = true}},
                    patch{0x90c85 + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x90c96 + 0x2, 0x1eb9870, EntityEx_mIdMap_address, {.expected_is_va = true}},
                    patch{0xd16e0 + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0xdbf48 + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0xdbfa8 + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0xdcaee + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0xdcb7f + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x113bd6 + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x113c46 + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x113cb6 + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x113d26 + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x130a41 + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x130a4b + 0x2, 0x1eb9870, EntityEx_mIdMap_address, {.expected_is_va = true}},
                    patch{0x19049d + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x19052e + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x1905bd + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x19072e + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x1907be + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x220567 + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x2216f1 + 0x1, 0x1eb9870, EntityEx_mIdMap_address, {.expected_is_va = true}},
                    patch{0x23518a + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x235197 + 0x2, 0x1eb9870, EntityEx_mIdMap_address, {.expected_is_va = true}},
                    patch{0x24b7bd + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x24b8e4 + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x251630 + 0x3, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x251643 + 0x3, 0x1eba874, EntityEx_mIdMap_address + 0x2004, {.expected_is_va = true}},
                    patch{0x25165e + 0x3, 0x1eba874, EntityEx_mIdMap_address + 0x2004, {.expected_is_va = true}},
                    patch{0x251682 + 0x3, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x2516a1 + 0x3, 0x1eba874, EntityEx_mIdMap_address + 0x2004, {.expected_is_va = true}},
                    patch{0x251b45 + 0x3, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x251b58 + 0x3, 0x1eba874, EntityEx_mIdMap_address + 0x2004, {.expected_is_va = true}},
                    patch{0x251b71 + 0x3, 0x1eba874, EntityEx_mIdMap_address + 0x2004, {.expected_is_va = true}},
                    patch{0x251bb4 + 0x3, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x251bd1 + 0x3, 0x1eba874, EntityEx_mIdMap_address + 0x2004, {.expected_is_va = true}},
                    patch{0x26e9fa + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},
                    patch{0x26eaab + 0x1, 0x1eb9874, EntityEx_mIdMap_address + 0x4, {.expected_is_va = true}},

                    //ops
                    patch{0x1c60 + 0x1, 0x400, 0x800},
                    patch{0x6cf31 + 0x1, 0x800, 0x1000},
                    patch{0x6cf85 + 0x1, 0x800, 0x1000},
                    patch{0x90b9e + 0x1, 0x800, 0x1000},
                    patch{0x90c80 + 0x1, 0x800, 0x1000},
                    patch{0xd16db + 0x1, 0x800, 0x1000},
                    patch{0xdbf43 + 0x1, 0x800, 0x1000},
                    patch{0xdbfa3 + 0x1, 0x800, 0x1000},
                    patch{0xdcae9 + 0x1, 0x800, 0x1000},
                    patch{0xdcb7a + 0x1, 0x800, 0x1000},
                    patch{0x113bd1 + 0x1, 0x800, 0x1000},
                    patch{0x113c41 + 0x1, 0x800, 0x1000},
                    patch{0x113cb1 + 0x1, 0x800, 0x1000},
                    patch{0x113d21 + 0x1, 0x800, 0x1000},
                    patch{0x130a3c + 0x1, 0x400, 0x800},
                    patch{0x190498 + 0x1, 0x800, 0x1000},
                    patch{0x190529 + 0x1, 0x800, 0x1000},
                    patch{0x1905b8 + 0x1, 0x800, 0x1000},
                    patch{0x190729 + 0x1, 0x800, 0x1000},
                    patch{0x1907b9 + 0x1, 0x800, 0x1000},
                    patch{0x220562 + 0x1, 0x800, 0x1000},
                    patch{0x221702 + 0x3, 0x1004, 0x2004},
                    patch{0x22175b + 0x3, 0x1004, 0x2004},
                    patch{0x235185 + 0x1, 0x400, 0x800},
                    patch{0x24b7b8 + 0x1, 0x800, 0x1000},
                    patch{0x24b8df + 0x1, 0x800, 0x1000},
                    patch{0x25163b + 0x2, 0x400, 0x800},
                    patch{0x25167a + 0x2, 0x400, 0x800},
                    patch{0x251699 + 0x2, 0x400, 0x800},
                    patch{0x251b50 + 0x2, 0x400, 0x800},
                    patch{0x251bac + 0x2, 0x400, 0x800},
                    patch{0x251bc9 + 0x2, 0x400, 0x800},
                    patch{0x26e9f0 + 0x1, 0x800, 0x1000},
                    patch{0x26eaa6 + 0x1, 0x800, 0x1000},

                    //PblHashTable<EntityEx,1024> functions
                    //patch{0x89f2c + 0x2, 0x400, 0x800},
                    //patch{0xd03d6 + 0x1, 0x400, 0x800},
                    //patch{0x89f56 + 0x3, 0x400, 0x800},
                    //patch{0x700c8 + 0x1, 0x800, 0x1000},
                    //patch{0x894dc + 0x2, 0x400, 0x800},
                    //patch{0x89455 + 0x3, 0x1004, 0x2004}, //PblHashTable<EntityEx,1024>::Itor::operator*
                    //patch{0x89476 + 0x2, 0x400, 0x800},
                    //patch{0x8948f + 0x2, 0x400, 0x800},
                    //patch{0x8e045 + 0x3, 0x1004, 0x2004}, //PblHashTable<EntityEx,1024>::Itor::operator->
                    //patch{0x89465 + 0x3, 0x1004, 0x2004}, //PblHashTable<EntityEx,1024>::Itor::operator_EntityEx_*
                    //patch{0xd0696 + 0x1, 0x400, 0x800},
                    //patch{0xd044b + 0x1, 0x800, 0x1000},
                    //patch{0xd0410 + 0x1, 0x800, 0x1000},*/
                  },
            },

            patch_set{
               .name = "Doors Increase - Skipped",
               .patches =
                  {
                    /*//replace EntityDoor::mspDoors pointer to new address
                    //relocate EntityDoor::msHostDoors
                    //relocate EntityDoor::msClientDoors

                    //EntityDoor::mspDoors -> msHostDoors pointer
                    patch{0x3e4538, 0x1ebb9a0, EntityDoor_msHostDoors_address, {.expected_is_va = true}},

                    //msHostDoors
                    patch{0x1de2 + 0x1, 0x1ebb9a4, EntityDoor_msHostDoors_address + 0x4, {.expected_is_va = true}},
                    patch{0x1def + 0x2, 0x1ebb9a0, EntityDoor_msHostDoors_address, {.expected_is_va = true}},
                    patch{0x981d2 + 0x1, 0x1ebb9a4, EntityDoor_msHostDoors_address + 0x4, {.expected_is_va = true}},
                    patch{0x981d7 + 0x6, 0x1ebb9a0, EntityDoor_msHostDoors_address, {.expected_is_va = true}},
                    patch{0x981ed + 0x2, 0x1ebb9a0, EntityDoor_msHostDoors_address, {.expected_is_va = true}},
                    patch{0x98237 + 0x1, 0x1ebb9a0, EntityDoor_msHostDoors_address, {.expected_is_va = true}},

                    //msClientDoors
                    patch{0x1e02 + 0x1, 0x1ebbaac, EntityDoor_msClientDoors_address + 0x4, {.expected_is_va = true}},
                    patch{0x1e0f + 0x2, 0x1ebbaa8, EntityDoor_msClientDoors_address, {.expected_is_va = true}},
                    patch{0x981e8 + 0x1, 0x1ebbaac, EntityDoor_msClientDoors_address + 0x4, {.expected_is_va = true}},
                    patch{0x981ff + 0x2, 0x1ebbaa8, EntityDoor_msClientDoors_address, {.expected_is_va = true}},
                    patch{0x98217 + 0x1, 0x1ebbaa8, EntityDoor_msClientDoors_address, {.expected_is_va = true}},

                    //mspDoors ops
                    patch{0x97645 + 0x1, 0x40, 0x80 - 0x8, {.values_are_8bit = true}},
                    patch{0x97661 + 0x1, 0x40, 0x80 - 0x8, {.values_are_8bit = true}},
                    patch{0x98259 + 0x1, 0x40, 0x80 - 0x8, {.values_are_8bit = true}},

                    //msHostDoors ops
                    patch{0x1de0 + 0x1, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    patch{0x981d0 + 0x1, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},

                    //msClientDoors ops
                    patch{0x1e00 + 0x1, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    patch{0x981e6 + 0x1, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},

                    //msDoorIter ops
                    patch{0x98289 + 0x2, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    patch{0x982ab + 0x3, 0x84, 0x104 - 0x8},
                    patch{0x982ba + 0x2, 0x104, 0x204 - 0x8},
                    patch{0x982db + 0x2, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    patch{0x982f3 + 0x2, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},

                    //PblHashTable<EntityDoor,32>
                    //patch{0xd930c + 0x2, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    //patch{0xd8d76 + 0x1, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    //patch{0xd9336 + 0x3, 0x20, 0x40 - 0x8},
                    //patch{0xd8d98 + 0x1, 0x40, 0x80 - 0x8, {.values_are_8bit = true}},
                    //patch{0xd8e5c + 0x2, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    //patch{0xd8e16 + 0x2, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    //patch{0xd8e2c + 0x2, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    //patch{0xd8e05 + 0x3, 0x84, 0x104 - 0x8},
                    //patch{0xd92c6 + 0x1, 0x20, 0x40 - 0x8, {.values_are_8bit = true}},
                    //patch{0xd8dc0 + 0x1, 0x40, 0x80 - 0x8, {.values_are_8bit = true}},*/
                  },
            },

            patch_set{
               .name = "Placeholder 3",
               .patches =
                  {
                  },
            },
         },
   },
};
