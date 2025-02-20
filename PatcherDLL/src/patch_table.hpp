#pragma once

#include <stdint.h>

#include "slim_vector.hpp"

#define PATCH_COUNT 13
#define EXE_COUNT 3

struct patch_flags {
   /// @brief Address represents a file offset instead of a virtual or unrelocated virtual address.
   bool file_offset : 1 = true;

   /// expected_value is an unrelocated virtual address pointer vs a numerical value
   bool expected_is_va : 1 = false;

   /// both the expected and replacement values are 8bit integers vs 32bit
   bool values_are_8bit : 1 = false;
};

struct patch {
   uintptr_t address = 0;
   uint32_t expected_value = 0;
   uint32_t replacement_value = 0;
   patch_flags flags = {};
};

struct patch_set {
   const char* name = "";
   slim_vector<patch> patches;
};

struct exe_patch_list {
   const char* name = "";

   bool id_address_is_file_offset = false;

   uintptr_t id_address = 0;
   uint64_t expected_id = 0;

   const patch_set patches[PATCH_COUNT];
};

extern const exe_patch_list patch_lists[EXE_COUNT];