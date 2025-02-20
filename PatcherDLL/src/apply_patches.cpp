#include "pch.h"

#include "apply_patches.hpp"
#include "cfile.hpp"
#include "patch_table.hpp"

static const uintptr_t unrelocated_executable_base = 0x400000;

static bool memeq(const void* left, size_t left_size, const void* right, size_t right_size)
{
   if (left_size != right_size) return false;

   return memcmp(left, right, left_size) == 0;
}

static auto resolve_file_address(uintptr_t offset, const slim_vector<section_info>& sections) -> char*
{
   for (const section_info& section : sections) {
      if (offset >= section.file_start and offset < section.file_end) {
         return section.memory_start + (offset - section.file_start);
      }
   }

   return nullptr;
}

static auto resolve_address(uintptr_t virtual_address, const uintptr_t relocated_executable_base) -> char*
{
   return (char*)((virtual_address - unrelocated_executable_base) + relocated_executable_base);
}

static bool apply_patch(const patch& patch, const uintptr_t relocated_executable_base,
    const slim_vector<section_info>& sections)
{
    char* patch_address = patch.flags.file_offset
        ? resolve_file_address(patch.address, sections)
        : resolve_address(patch.address, relocated_executable_base);

    // Validate the current memory content.
    if (patch.flags.values_are_8bit) {

        // Cast the computed expected value to 8-bit.
        uint8_t expected_8bit = patch.flags.expected_is_va
            ? static_cast<uint8_t>(patch.expected_value - unrelocated_executable_base + relocated_executable_base)
            : static_cast<uint8_t>(patch.expected_value);
        if (!memeq(patch_address, sizeof(expected_8bit), &expected_8bit, sizeof(expected_8bit)))
            return false;

        // Cast the replacement value to 8-bit.
        uint8_t replacement_8bit = static_cast<uint8_t>(patch.replacement_value);
        memcpy(patch_address, &replacement_8bit, sizeof(expected_8bit));
    }
    else {
        // Compute the expected value in 32-bit form.
        uint32_t expected_32bit = patch.flags.expected_is_va
            ? patch.expected_value - unrelocated_executable_base + relocated_executable_base
            : patch.expected_value;
        if (!memeq(patch_address, sizeof(expected_32bit), &expected_32bit, sizeof(expected_32bit)))
            return false;

        // Pull the 32-bit value
        uint32_t replacement_32bit = patch.replacement_value;
        memcpy(patch_address, &replacement_32bit, sizeof(expected_32bit));
    }

    return true;
}


bool apply_patches(const uintptr_t relocated_executable_base, const slim_vector<section_info>& sections)
{
   cfile log{"BF2GameExt.log", "w"};

   if (not log) return false;

   for (const exe_patch_list& exe_list : patch_lists) {
      log.printf("Checking executable against patch list: %s\n", exe_list.name);

      if (exe_list.id_address_is_file_offset) {
         const char* id_address = resolve_file_address(exe_list.id_address, sections);

         if (not id_address or not memeq(id_address, sizeof(exe_list.expected_id),
                                         &exe_list.expected_id, sizeof(exe_list.expected_id))) {
            continue;
         }
      }
      else {
         if (not memeq(resolve_address(exe_list.id_address, relocated_executable_base),
                       sizeof(exe_list.expected_id), &exe_list.expected_id, sizeof(exe_list.expected_id))) {
            continue;
         }
      }

      log.printf("Identified executable as: %s\nApplying patches.\n", exe_list.name);

      for (const patch_set& set : exe_list.patches) {
         log.printf("Applying patch set: %s\n", set.name);

         for (const patch& patch : set.patches) {
            if (not apply_patch(patch, relocated_executable_base, sections)) {
               log.printf(R"(Failed to apply patch
   address = %x 
   expected_value = %x 
   replacement_value = %x
   flags = {.file_offset = %i, .expected_is_va = %i}
)",
                          patch.address, patch.expected_value, patch.replacement_value,
                          (int)patch.flags.file_offset, (int)patch.flags.expected_is_va);

               return false;
            }
         }
      }

      return true;
   }

   log.printf("Couldn't identify executable. Unable to patch.\n");

   return false;
}
