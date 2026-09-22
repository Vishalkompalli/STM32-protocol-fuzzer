.syntax unified
.cpu cortex-m3
.thumb

.section .isr_vector,"a",%progbits
.type isr_vector, %object
isr_vector:
  .word 0x20005000     /* Stack top */
  .word Reset_Handler

.section .text
.global Reset_Handler
Reset_Handler:
  bl main
  b .

.size Reset_Handler, . - Reset_Handler
