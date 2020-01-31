#include "common.h"

void write_buffer(bin_t* buffer, const uint start, const byte* data, const uint size) {
    memcpy(&buffer->data[start], data, size);
}

void write_buffer(bin_t* buffer, const byte* data, const uint size) {
    write_buffer(buffer, buffer->size - size, data, size);
}

#define LPX_PROGRAMMER_PATCH_SIZE 0x142
const byte lpx_programmer_patch[LPX_PROGRAMMER_PATCH_SIZE] = {
    0x06, 0xB4, 0xF5, 0xF7, 0xD9, 0xFA, 0x06, 0xBC, 0x7F, 0x28, 0x07, 0xD1, 0xDF, 0xF8, 0x16, 0x00,
    0x10, 0xF8, 0x08, 0x00, 0x00, 0xF0, 0xFF, 0x00, 0x8D, 0xF8, 0x0B, 0x00, 0x9D, 0xF8, 0x05, 0x00,
    0xF5, 0xF7, 0x78, 0xBD, 0xDE, 0xC9, 0x01, 0x08, 0x00, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72,
    0x73, 0x74, 0x00, 0x40, 0x41, 0x42, 0x43, 0x60, 0x61, 0x62, 0x63, 0x64, 0x00, 0x3C, 0x3D, 0x3E,
    0x3F, 0x5C, 0x5D, 0x5E, 0x5F, 0x65, 0x00, 0x38, 0x39, 0x3A, 0x3B, 0x58, 0x59, 0x5A, 0x5B, 0x66,
    0x00, 0x34, 0x35, 0x36, 0x37, 0x54, 0x55, 0x56, 0x57, 0x67, 0x00, 0x30, 0x31, 0x32, 0x33, 0x50,
    0x51, 0x52, 0x53, 0x68, 0x00, 0x2C, 0x2D, 0x2E, 0x2F, 0x4C, 0x4D, 0x4E, 0x4F, 0x69, 0x00, 0x28,
    0x29, 0x2A, 0x2B, 0x48, 0x49, 0x4A, 0x4B, 0x6A, 0x00, 0x24, 0x25, 0x26, 0x27, 0x44, 0x45, 0x46,
    0x47, 0x6B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0xB4, 0xDF, 0xF8,
    0x12, 0x40, 0x14, 0xF8, 0x02, 0x40, 0x04, 0xF0, 0xFF, 0x02, 0x10, 0xBC, 0xFD, 0xF7, 0xA4, 0xFE,
    0xFC, 0xF7, 0x5C, 0xBB, 0x5E, 0xCA, 0x01, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x0C, 0x0D, 0x0E,
    0x15, 0x16, 0x17, 0x18, 0x1F, 0x20, 0x21, 0x22, 0x29, 0x2A, 0x2B, 0x2C, 0x33, 0x34, 0x35, 0x36,
    0x3D, 0x3E, 0x3F, 0x40, 0x47, 0x48, 0x49, 0x4A, 0x51, 0x52, 0x53, 0x54, 0x0F, 0x10, 0x11, 0x12,
    0x19, 0x1A, 0x1B, 0x1C, 0x23, 0x24, 0x25, 0x26, 0x2D, 0x2E, 0x2F, 0x30, 0x37, 0x38, 0x39, 0x3A,
    0x41, 0x42, 0x43, 0x44, 0x4B, 0x4C, 0x4D, 0x4E, 0x55, 0x56, 0x57, 0x58, 0x59, 0x4F, 0x45, 0x3B,
    0x31, 0x27, 0x1D, 0x13, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0xB4, 0xF5, 0xF7, 0x45, 0xFA, 0x7F, 0x28,
    0x02, 0xD1, 0x4F, 0xF0, 0x90, 0x0A, 0x01, 0xE0, 0x4F, 0xF0, 0xB0, 0x0A, 0x06, 0xBC, 0xF5, 0xF7,
    0x09, 0xBD
};

void lpx_patch(bool* args) {
    if (args[0]) { // Patch Programmer mode
        if (!reallocate_buffer(&input, LPX_PROGRAMMER_PATCH_SIZE, "firmware patches")) exit(7);
        
        // Write LPX_PROGRAMMER_PATCH (custom code)
        write_buffer(&input, lpx_programmer_patch, LPX_PROGRAMMER_PATCH_SIZE);

        // Branch to LPX_PROGRAMMER_PATCH
        *(uint*)&input.data[0x64C6] = 0xBA76F00A;
        *(uint*)&input.data[0x6506] = 0xBAEAF00A;
        *(uint*)&input.data[0xD10E] = 0xBC98F003;
        
        // Setup red color
        input.data[0x749B] = 0x69;
        *(ushort*)&input.data[0x1045C] = 0x0507;

        // Setup text
        write_buffer(&input, 0x7698, (byte*)"Cover\0mat1", 11);
    }

    if (args[1]) // Setup rename Live mode to Gay mode
        memcpy(&input.data[0x7690], "Gay", 4);
}

void lpminimk3_patch(bool* args) {

}

void lppromk3_patch(bool* args) {

}

void lpmk2_patch(bool* args) {

}

void lppro_patch(bool* args) {

}