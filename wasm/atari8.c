
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "colours.h"
#include "screen.h"
#include "atari.h"
#include "cpu.h"
#include "antic.h"
#include "input.h"
#include "libatari800.h"

/* Dimensions of Screen_atari.
   Screen_atari is Screen_WIDTH * Screen_HEIGHT bytes.
   Each byte is an Atari color code - use Colours_Get[RGB] functions
   to get actual RGB codes.
   You should never display anything outside the middle 336 columns. */
#define Screen_WIDTH  384
#define Screen_HEIGHT 240

typedef struct atari8_t {
    input_template_t input;
    uint32_t rgba[Screen_WIDTH * Screen_HEIGHT];
} atari8_t;

#define MAX_SAMPLES 4096
float sample_buffer[MAX_SAMPLES];
unsigned int sample_count = 0;
char* bios_array = 0;

typedef struct {
    input_template_t input;
    uint32_t pixels[Screen_HEIGHT * Screen_WIDTH];
} Atari8ControlsState;

void audio_callback_fn(const float* samples, int num_samples, void* user_data) {
    if (sample_count + num_samples < MAX_SAMPLES) {
        memcpy(sample_buffer+sample_count, samples, num_samples*sizeof(float));
        sample_count += num_samples;
    }
}

///

int main(int argc, char** argv) {
 // can't have any code that interacts with the wrapper...?
}

///


void machine_hardreset(atari8_t* sys) {
    Atari800_Coldstart();
}

atari8_t* machine_init(char* bios) {
 // TODO: real args?
    char *test_args[] = {
            //"atari800",
            "-xl",
    };
    libatari800_init(sizeof(test_args) / sizeof(test_args[0]), test_args);
    atari8_t* sys = (atari8_t*) malloc(sizeof(atari8_t));
    libatari800_clear_input_array(&sys->input);
    Colours_SetPreset(COLOURS_PRESET_VIBRANT);
    return sys;
}

void machine_reset(atari8_t* sys) {
    Atari800_Warmstart();
}

void machine_start_frame(atari8_t* sys) {
   //TODO: should start frame, not execute entire frame...
   libatari800_next_frame(&sys->input);
   // update screen
   UBYTE* colors = (UBYTE *)Screen_atari;
   int i=0;
   for (int y=0; y<Screen_HEIGHT; y++) {
      for (int x=0; x<Screen_WIDTH; x++) {
         int col = colors[i];
         uint32_t rgba = 0xff000000;
         rgba |= (Colours_GetR(col) << 0);
         rgba |= (Colours_GetG(col) << 8);
         rgba |= (Colours_GetB(col) << 16);
         sys->rgba[i] = rgba;
         i++;
      }
   }
}

void machine_tick(atari8_t* sys) {
    //TODO
}

void machine_exec(atari8_t* sys, uint32_t micro_seconds) {
    // TODO
}

void* machine_get_pixel_buffer(const atari8_t* sys) {
    //return libatari800_get_screen_ptr();
    return (void*) sys->rgba;
}

float* machine_get_sample_buffer() {
    // TODO
    return NULL;
}

unsigned int machine_get_sample_count() {
    // TODO
    return 0;
}

uint8_t machine_mem_read(atari8_t* sys, uint16_t address) {
    // TODO return mem_rd(&sys->mem_cpu, address);
    return libatari800_get_main_memory_ptr()[address];
}
 
void machine_mem_write(atari8_t* sys, uint16_t address, uint8_t value) {
    //TODO
}

int machine_get_state_size() {
    return sizeof(emulator_state_t);
}

int machine_get_cpu_state_size() {
    return sizeof(cpu_state_t);
}

int machine_get_controls_state_size() {
    return sizeof(input_template_t);
}

void machine_save_state(const atari8_t* sys, emulator_state_t* state) {
    libatari800_get_current_state(state);
}

void machine_load_state(atari8_t* sys, const emulator_state_t* state) {
    libatari800_restore_state((emulator_state_t*)state);
}

void machine_save_cpu_state(const atari8_t* sys, uint8_t* state) {
    state[0] = CPU_regA;
    state[1] = CPU_regP;
    state[2] = CPU_regS;
    state[3] = CPU_regX;
    state[4] = CPU_regY;
    state[5] = CPU_IRQ;
    state[6] = CPU_regPC;
    state[7] = CPU_regPC >> 8;
}

void machine_load_cpu_state(atari8_t* sys, const uint8_t* state) {
    // TODO: not needed?
}

void machine_save_controls_state(const atari8_t* sys, Atari8ControlsState* state) {
    memcpy(state, &sys->input, sizeof(Atari8ControlsState));
}

void machine_load_controls_state(atari8_t* sys, const Atari8ControlsState* state) {
    memcpy(&sys->input, state, sizeof(Atari8ControlsState));
}

void machine_key_down(atari8_t* sys, int key_code) {
    sys->input.keychar = key_code & 0xff;
    sys->input.shift = (key_code & 0x100) != 0;
    sys->input.control = (key_code & 0x200) != 0;
    sys->input.option = (key_code & 0x800) != 0;
    sys->input.select = (key_code & 0x1000) != 0;
    sys->input.start = (key_code & 0x2000) != 0;
}

void machine_key_up(atari8_t* sys, int key_code) {
    sys->input.keychar = 0;
    //TODO?
}

void machine_joy_set(atari8_t* sys, int joyindex, int bits) {
    switch (joyindex) {
       case 0: sys->input.joy0 = bits; sys->input.trig0 = bits>>8; break;
       case 1: sys->input.joy1 = bits; sys->input.trig1 = bits>>8; break;
       case 2: sys->input.joy2 = bits; sys->input.trig2 = bits>>8; break;
       case 3: sys->input.joy3 = bits; sys->input.trig3 = bits>>8; break;
    }
}

void machine_paddle_set(atari8_t* sys, int device, int value) {
    sys->input.mouse_mode = INPUT_MOUSE_ST;
    switch (device) {
      case 0: sys->input.mousex = value; break;
      case 1: sys->input.mousey = value; break;
    }
}

void atari8_mouse_set(atari8_t* sys, int mousex, int mousey, int buttons, int mode) {
    sys->input.mousex = mousex;
    sys->input.mousey = mousey;
    sys->input.mouse_buttons = buttons;
    sys->input.mouse_mode = mode;
}

int machine_load_rom(atari8_t* sys, const uint8_t* ptr, int num_bytes) {
    const char* filename = "/tmp/atari8.img";
    FILE *f = fopen(filename, "wb");
    if (f != NULL) {
      fwrite(ptr, num_bytes, 1, f);
      fclose(f);
      int type = libatari800_reboot_with_file(filename);
      printf("machine_load_rom: loaded %d bytes file type %d\n", num_bytes, type);
      return 0;
    }
    printf("machine_load_rom: error %d\n", errno);
    return errno ? errno : -1;
}

unsigned int machine_cpu_get_pc(atari8_t* sys) {
    return CPU_regPC;
}

unsigned int machine_cpu_get_sp(atari8_t* sys) {
    return CPU_regS;
}

int machine_cpu_is_stable(atari8_t* sys) {
    return 1; //TODO
}

int machine_get_raster_line(atari8_t* sys) {
    return ANTIC_ypos;
}

int machine_std_display_width() {
    return Screen_WIDTH;
}

int machine_std_display_height() {
    return Screen_HEIGHT;
}

int machine_display_width() {
    return Screen_WIDTH;
}

int machine_display_height() {
    return Screen_HEIGHT;
}

int machine_max_display_size() {
    return Screen_WIDTH * Screen_HEIGHT * 4;
}

