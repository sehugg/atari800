#ifndef CONFIG_H
#define CONFIG_H

#define DEFAULT_CFG_PATH "PROGDIR:Atari800.cfg"

#define SUPPORTS_ATARI_CONFIGINIT
#define SUPPORTS_ATARI_CONFIGSAVE
#define SUPPORTS_ATARI_CONFIGURE
#define DONT_USE_RTCONFIGUPDATE

#define WORDS_BIGENDIAN

//#define SIGNED_SAMPLES
/*#define STEREO*/
/*#define VERY_SLOW*/
/*#define SET_LED*/

#define SOUND
#define CONSOLE_SOUND

#define USE_NEW_BINLOAD
#define NO_GOTO

#define DO_DIR
#define NEW_CYCLE_EXACT

void usleep(unsigned long usec);

#endif
