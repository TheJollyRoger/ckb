#ifndef OS_H
#define OS_H

// OS definitions

#ifdef __linux
#define OS_LINUX
#endif
#ifdef __APPLE__
#define OS_MAC
#endif

#if !defined(OS_LINUX) && !defined(OS_MAC)
#error Your OS is not supported. Edit os.h if you want to compile anyway.
#endif

// OS-specific includes

#ifdef OS_LINUX

#ifndef _DEFAULT_SOURCE
#define _DEFAULT_SOURCE
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <features.h>
#include <libudev.h>
#include <linux/uinput.h>
#include <linux/usbdevice_fs.h>

#ifndef UINPUT_VERSION
#define UINPUT_VERSION 2
#endif

#define INPUT_TEST unusedinput[8]

#endif  // OS_LINUX

#ifdef OS_MAC

#include <Carbon/Carbon.h>
#include <IOKit/hid/IOHIDLib.h>

void *memrchr(const void *s, int c, size_t n);

#endif  // OS_MAC

#endif
