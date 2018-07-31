/* stm32f373xRB.ld
 * Created on: 27 jul 2018
 *    Author: jof
 */
     
EXTERN(vector_table)
ENTRY(reset_handler)
MEMORY
{
 /* ram total = 24K */
 ram     (rwx) : ORIGIN = 0x20000000, LENGTH = 24K
 /* rom total = 128K */
 bootrom (rx)  : ORIGIN = 0x08000000, LENGTH = 8K
 rom     (rx)  : ORIGIN = 0x08002000, LENGTH = 120K
}
SECTIONS
{
 .text : {
  *(.vectors)
  *(.text*)
  . = ALIGN(4);
  *(.rodata*)
  . = ALIGN(4);
 } >bootrom
 .preinit_array : {
  . = ALIGN(4);
  __preinit_array_start = .;
  KEEP (*(.preinit_array))
  __preinit_array_end = .;
 } >bootrom
 .init_array : {
  . = ALIGN(4);
  __init_array_start = .;
  KEEP (*(SORT(.init_array.*)))
  KEEP (*(.init_array))
  __init_array_end = .;
 } >bootrom
 .fini_array : {
  . = ALIGN(4);
  __fini_array_start = .;
  KEEP (*(.fini_array))
  KEEP (*(SORT(.fini_array.*)))
  __fini_array_end = .;
 } >bootrom
 .ARM.extab : {
  *(.ARM.extab*)
 } >bootrom
 .ARM.exidx : {
  __exidx_start = .;
  *(.ARM.exidx*)
  __exidx_end = .;
 } >bootrom
 . = ALIGN(4);
 _etext = .;
 .data : {
  _data = .;
  *(.data*)
  . = ALIGN(4);
  _edata = .;
 } >ram AT >bootrom
 _data_loadaddr = LOADADDR(.data);
 .bss : {
  *(.bss*)
  *(COMMON)
  . = ALIGN(4);
  _ebss = .;
 } >ram
 /DISCARD/ : { *(.eh_frame) }
 . = ALIGN(4);
 end = .;
}
PROVIDE(_stack       = ORIGIN(ram) + LENGTH(ram));
PROVIDE(_appRomStart = ORIGIN(rom));
PROVIDE(_appRomEnd   = ORIGIN(rom) + LENGTH(rom));
PROVIDE(_bootRom     = ORIGIN(bootrom));
