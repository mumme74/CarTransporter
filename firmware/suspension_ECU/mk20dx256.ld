/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2017 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

MEMORY
{
    bootrom (rx) : ORIGIN = 0x00000000, LENGTH = 12K
    rom (rx)     : ORIGIN = 0x00003000, LENGTH = 244K
    RAM  (rwx)   : ORIGIN = 0x1FFF8000, LENGTH = 64K
}


SECTIONS
{
    .text : {
        . = 0;
        KEEP(*(.vectors))
        *(.startup*)
        /* TODO: does linker detect startup overflow onto flashconfig? */
        . = 0x400;
        KEEP(*(.flashconfig*))
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
        KEEP(*(.init))
        . = ALIGN(4);
        __preinit_array_start = .;
        KEEP (*(.preinit_array))
        __preinit_array_end = .;
        __init_array_start = .;
        KEEP (*(SORT(.init_array.*)))
        KEEP (*(.init_array))
        __init_array_end = .;
    } > rom = 0xFF /* must relocate this if changing size of bootrom! */

    .ARM.exidx : {
        __exidx_start = .;
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        __exidx_end = .;
    } > rom
    _etext = .;

    .usbdescriptortable (NOLOAD) : {
        /* . = ORIGIN(RAM); */
        . = ALIGN(512);
        *(.usbdescriptortable*)
    } > RAM

    .dmabuffers (NOLOAD) : {
        . = ALIGN(4);
        *(.dmabuffers*)
    } > RAM

    .usbbuffers (NOLOAD) : {
        . = ALIGN(4);
        *(.usbbuffers*)
    } > RAM

    .data : AT (_etext) {
        . = ALIGN(4);
        _sdata = .; 
        *(.fastrun*)
        *(.data*)
        . = ALIGN(4);
        _edata = .; 
    } > RAM

    .noinit (NOLOAD) : {
        *(.noinit*)
    } > RAM

    .bss : {
        . = ALIGN(4);
        _sbss = .;
        __bss_start__ = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
        __bss_end = .;
        __bss_end__ = .;
    } > RAM
    . = ALIGN(4);
    _heapstart = .;


    _estack = ORIGIN(RAM) + LENGTH(RAM) -4;
    _stack  = _estack;
    _appRomStart = ORIGIN(rom);
    _appRomEnd   = ORIGIN(rom) + LENGTH(rom) -4;
    _bootRom     = ORIGIN(bootrom);
}
