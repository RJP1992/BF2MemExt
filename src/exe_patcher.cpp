#include "exe_patcher.hpp"
#include "file_helpers.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

static bool memeq(const void* left, size_t left_size, const void* right, size_t right_size)
{
   if (left_size != right_size) return false;

   return memcmp(left, right, left_size) == 0;
}

static auto align_up(uint32_t i, uint32_t alignment) -> uint32_t
{

   return (i + alignment - 1) / alignment * alignment;
}

static const char ext_section_name[IMAGE_SIZEOF_SHORT_NAME] = ".bf2ext";

exe_patcher::~exe_patcher()
{
   if (_data) delete[] _data;
}

bool exe_patcher::load(const char* file_path)
{
   if (_data) {
      delete[] _data;

      _data = nullptr;
   }

   FILE* file = fopen(file_path, "rb");

   if (not file) return false;

   bool result = false;
   ptrdiff_t file_size = 0;

   if (fseek(file, 0, SEEK_END) != 0) goto cleanup;

   file_size = ftell(file);

   if (file_size < 0) goto cleanup;

   _size = (size_t)file_size;
   _data = new uint8_t[file_size];

   rewind(file);

   if (fread(_data, sizeof(uint8_t), _size, file) != _size) goto cleanup;

   result = true;

cleanup:
   fclose(file);

   return result;
}

bool exe_patcher::save(const char* file_path)
{
   if (not _data) return false;

   char* temp_file_name = aquire_temp_file(file_path, "BF2Patch");

   if (not temp_file_name) return false;

   FILE* file = fopen(temp_file_name, "wb");
   bool result = false;

   if (not file) goto cleanup;

   if (fwrite(_data, sizeof(uint8_t), _size, file) != _size) goto cleanup;

   fclose(file);

   file = nullptr;

   if (not move_file(temp_file_name, file_path)) goto cleanup;

   result = true;

cleanup:
   if (temp_file_name) {
      remove(temp_file_name);
      free(temp_file_name);
   }
   if (file) fclose(file);

   return result;
}

bool exe_patcher::compatible(uint32_t id_address, uint64_t expected_id)
{
   // Bounds and overflow Checks
   if (id_address + sizeof(uint64_t) >= _size) return false;
   if (id_address + sizeof(uint64_t) < id_address) return false;

   uint64_t exe_id = 0;

   memcpy(&exe_id, _data + id_address, sizeof(uint64_t));

   return exe_id == expected_id;
}

bool exe_patcher::prepare(uint32_t ext_section_size)
{
   if (not check_range(0, sizeof(IMAGE_DOS_HEADER))) return false;

   IMAGE_DOS_HEADER* dos_header = (IMAGE_DOS_HEADER*)&_data[0x0];

   if (dos_header->e_magic != 'ZM') return false;

   if (not check_range(dos_header->e_lfanew, sizeof(IMAGE_NT_HEADERS32))) return false;

   IMAGE_NT_HEADERS32* nt_headers = (IMAGE_NT_HEADERS32*)&_data[dos_header->e_lfanew];

   if (nt_headers->Signature != 'EP') return false;

   IMAGE_FILE_HEADER* file_header = &nt_headers->FileHeader;
   IMAGE_OPTIONAL_HEADER32* optional_header = &nt_headers->OptionalHeader;

   if (optional_header->Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC) return false;

   const size_t section_headers_offset =
      dos_header->e_lfanew + (sizeof(IMAGE_NT_HEADERS32) - sizeof(IMAGE_OPTIONAL_HEADER32)) +
      file_header->SizeOfOptionalHeader;
   const size_t section_headers_start_size = file_header->NumberOfSections * sizeof(IMAGE_SECTION_HEADER);

   if (not check_range(section_headers_offset, section_headers_start_size)) return false;

   // Simplify some error handling by checking this here.
   if (file_header->NumberOfSections == 0) return false;

   IMAGE_SECTION_HEADER* section_headers = (IMAGE_SECTION_HEADER*)&_data[section_headers_offset];

   for (uint32_t i = 0; i < file_header->NumberOfSections; ++i) {
      // Early out for having already added the section previously.
      if (memeq(&section_headers[i].Name, sizeof(section_headers[i].Name), &ext_section_name,
                sizeof(ext_section_name))) {
         if (section_headers[i].Characteristics !=
             (IMAGE_SCN_CNT_UNINITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE)) {
            return false;
         }

         if (i != file_header->NumberOfSections - 1) return false;

         _ext_section_va = optional_header->ImageBase + section_headers[i].VirtualAddress;

         if (section_headers[i].Misc.VirtualSize < ext_section_size) {
            optional_header->SizeOfUninitializedData -= section_headers[i].Misc.VirtualSize;
            optional_header->SizeOfImage -= section_headers[i].Misc.VirtualSize;

            section_headers[i].Misc.VirtualSize =
               align_up(ext_section_size, optional_header->SectionAlignment);

            optional_header->SizeOfUninitializedData += section_headers[i].Misc.VirtualSize;
            optional_header->SizeOfImage += section_headers[i].Misc.VirtualSize;
         }

         return true;
      }
   }

   const size_t section_headers_patched_size = section_headers_start_size + sizeof(IMAGE_SECTION_HEADER);
   const size_t section_headers_patched_end = section_headers_offset + section_headers_patched_size;

   if (not check_range(section_headers_offset, section_headers_patched_size)) return false;
   if (section_headers_patched_end > optional_header->SizeOfHeaders) return false;

   IMAGE_SECTION_HEADER* existing_last_section =
      (IMAGE_SECTION_HEADER*)&section_headers[file_header->NumberOfSections - 1];
   IMAGE_SECTION_HEADER* new_section =
      (IMAGE_SECTION_HEADER*)&section_headers[file_header->NumberOfSections];

   memset(new_section, 0, sizeof(IMAGE_SECTION_HEADER));
   memcpy(new_section->Name, ext_section_name, sizeof(ext_section_name));

   new_section->Misc.VirtualSize = align_up(ext_section_size, optional_header->SectionAlignment);
   new_section->VirtualAddress =
      align_up(existing_last_section->VirtualAddress + existing_last_section->Misc.VirtualSize,
               optional_header->SectionAlignment);
   new_section->SizeOfRawData = 0;
   new_section->PointerToRawData = 0;
   new_section->PointerToRelocations = 0;
   new_section->PointerToLinenumbers = 0;
   new_section->NumberOfRelocations = 0;
   new_section->NumberOfLinenumbers = 0;
   new_section->Characteristics =
      IMAGE_SCN_CNT_UNINITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE;

   file_header->NumberOfSections += 1;
   optional_header->SizeOfImage += new_section->Misc.VirtualSize;
   optional_header->SizeOfUninitializedData += new_section->Misc.VirtualSize;

   _ext_section_va = optional_header->ImageBase + new_section->VirtualAddress;

   return true;
}

bool exe_patcher::apply(const patch& patch)
{
    if (not _data) return false;

    const uint32_t offset = patch.address;

    // Determine whether to use 8-bit or 32-bit patch based on the 'is_8bit' flag
    const size_t patch_size = patch.is_8bit ? sizeof(uint8_t) : sizeof(uint32_t);

    // Handle expected value and replacement value based on patch size
    if (patch.is_8bit) {
        uint8_t replacement_value = static_cast<uint8_t>(patch.replacement_value);  // Treat replacement as 8-bit
        uint8_t expected_value = static_cast<uint8_t>(patch.expected_value);  // Treat expected as 8-bit

        if (patch.value_is_ext_section_relative_address) {
            replacement_value += _ext_section_va;
        }

        const bool already_patched = memeq(&_data[offset], sizeof(uint8_t), &replacement_value, sizeof(replacement_value));

        if (already_patched) {
            printf("Log: Address %08X already patched with 8-bit value %02X, skipping.\r\n", offset, replacement_value);
            return true;
        }

        const bool expected_value_check = memeq(&_data[offset], sizeof(uint8_t), &expected_value, sizeof(expected_value));

        if (not expected_value_check) {
            printf("Error: Mismatch at address %08X. Expected 8-bit value: %02X, but found: %02X.\r\n",
                offset, expected_value, _data[offset]);
            return false;
        }

        memcpy(&_data[offset], &replacement_value, sizeof(replacement_value));
        printf("Log: Successfully patched address %08X with 8-bit value %02X.\r\n", offset, replacement_value);
        return true;
    }
    else {
        uint32_t replacement_value = patch.replacement_value;
        uint32_t expected_value = patch.expected_value;

        if (patch.value_is_ext_section_relative_address) {
            replacement_value += _ext_section_va;
        }

        const bool already_patched = memeq(&_data[offset], sizeof(uint32_t), &replacement_value, sizeof(replacement_value));

        if (already_patched) {
            printf("Log: Address %08X already patched with 32-bit value %08X, skipping.\r\n", offset, replacement_value);
            return true;
        }

        const bool expected_value_check = memeq(&_data[offset], sizeof(uint32_t), &expected_value, sizeof(expected_value));

        if (not expected_value_check) {
            printf("Error: Mismatch at address %08X. Expected 32-bit value: %08X, but found: %08X.\r\n",
                offset, expected_value, _data[offset]);
            return false;
        }

        memcpy(&_data[offset], &replacement_value, sizeof(replacement_value));
        printf("Log: Successfully patched address %08X with 32-bit value %08X.\r\n", offset, replacement_value);
        return true;
    }
}

bool exe_patcher::check_range(size_t offset, size_t size) const noexcept
{
   // Bounds check
   if (offset + size >= _size) return false;

   // Overflow check
   if (offset + size < offset) return false;

   return true;
}