
#include <stdlib.h>
#include <string.h>

#include "libatari800.h"

typedef struct atari8_t {
    input_template_t input;
} atari8_t;

#define MAX_SAMPLES 4096
float sample_buffer[MAX_SAMPLES];
unsigned int sample_count = 0;
char* bios_array = 0;

typedef struct {
    input_template_t input;
} Atari8ControlsState;

void audio_callback_fn(const float* samples, int num_samples, void* user_data) {
    if (sample_count + num_samples < MAX_SAMPLES) {
        memcpy(sample_buffer+sample_count, samples, num_samples*sizeof(float));
        sample_count += num_samples;
    }
}

void machine_hardreset(atari8_t* sys) {
	char *test_args[] = {
		"atari800",
		"-atari",
	};
	libatari800_init(sizeof(test_args) / sizeof(test_args[0]), test_args);
}

atari8_t* machine_init(char* bios) {
    atari8_t* sys = (atari8_t*) malloc(sizeof(atari8_t));
    libatari800_clear_input_array(&sys->input);
    machine_hardreset(sys);
    return sys;
}

#if 0

void machine_reset(atari8_t* sys) {
    c64_reset(sys);
}

void machine_start_frame(atari8_t* sys) {
    kbd_update(&sys->kbd, 1000000/50);
}

void machine_tick(atari8_t* sys) {
    atari8_tick(sys);
}

void machine_exec(atari8_t* sys, uint32_t micro_seconds) {
    c64_exec(sys, micro_seconds);
}

void* machine_get_pixel_buffer(const atari8_t* sys) {
    return sys->pixel_buffer;
}

float* machine_get_sample_buffer() {
    return sample_buffer;
}

unsigned int machine_get_sample_count() {
    unsigned int n = sample_count;
    sample_count = 0;
    return n;
}

uint8_t machine_mem_read(atari8_t* sys, uint16_t address) {
    return mem_rd(&sys->mem_cpu, address);
}
 
void machine_mem_write(atari8_t* sys, uint16_t address, uint8_t value) {
    mem_wr(&sys->mem_cpu, address, value);
}

int machine_get_state_size() {
    return sizeof(atari8_t);
}

int machine_get_cpu_state_size() {
    return sizeof(m6502_t);
}

int machine_get_controls_state_size() {
    return sizeof(Atari8ControlsState);
}

void machine_save_state(const atari8_t* sys, atari8_t* state) {
    memcpy(state, sys, sizeof(atari8_t));
}

void machine_load_state(atari8_t* sys, const atari8_t* state) {
    memcpy(sys, state, sizeof(atari8_t));
}

void machine_save_cpu_state(const atari8_t* sys, m6502_t* state) {
    memcpy(state, &sys->cpu, sizeof(m6502_t));
}

void machine_load_cpu_state(atari8_t* sys, const m6502_t* state) {
    memcpy(&sys->cpu, state, sizeof(m6502_t));
}

void machine_save_controls_state(const atari8_t* sys, Atari8ControlsState* state) {
    state->kbd_joy1_mask = sys->kbd_joy1_mask;
    state->kbd_joy2_mask = sys->kbd_joy2_mask;
    state->joy_joy1_mask = sys->joy_joy1_mask;
    state->joy_joy2_mask = sys->joy_joy2_mask;
    state->kbd = sys->kbd;
}

void machine_load_controls_state(atari8_t* sys, const Atari8ControlsState* state) {
    sys->kbd_joy1_mask = state->kbd_joy1_mask;
    sys->kbd_joy2_mask = state->kbd_joy2_mask;
    sys->joy_joy1_mask = state->joy_joy1_mask;
    sys->joy_joy2_mask = state->joy_joy2_mask;
    sys->kbd = state->kbd;
}

void machine_key_down(atari8_t* sys, int key_code) {
    c64_key_down(sys, key_code);
}

void machine_key_up(atari8_t* sys, int key_code) {
    c64_key_up(sys, key_code);
}

void machine_load_rom(atari8_t* sys, const uint8_t* ptr, int num_bytes) {
    c64_quickload(sys, ptr, num_bytes);
}

unsigned int machine_cpu_get_pc(atari8_t* sys) {
    return sys->cpu.PC;
}

unsigned int machine_cpu_get_sp(atari8_t* sys) {
    return sys->cpu.S;
}

bool machine_cpu_is_stable(atari8_t* sys) {
    return sys->cpu.PINS & M6502_SYNC;
}

int machine_get_raster_line(atari8_t* sys) {
    return sys->vic.rs.v_count;
}
#endif
