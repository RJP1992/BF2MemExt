#pragma once

#include <stdint.h>

#include "slim_vector.hpp"

#define PATCH_COUNT 8
#define EXE_COUNT 3

struct patch {
   uint32_t address = 0;
   uint32_t expected_value = 0;
   uint32_t replacement_value = 0;
   bool value_is_ext_section_relative_address = false;
   bool is_8bit = false;
};

struct patch_set {
   const char* name = "";
   slim_vector<patch> patches;
};

struct exe_patch_list {
   const char* name = "";
   uint32_t id_address = 0;
   uint64_t expected_id = 0;

   const patch_set patches[PATCH_COUNT];
};

extern const exe_patch_list patch_lists[EXE_COUNT];
extern const uint32_t ext_section_size;
