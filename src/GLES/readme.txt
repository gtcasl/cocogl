Flat Color:

001A0038  stmdb       sp!, {r4, r5, lr} 
001A003C  ldr         r4, [r0, #0xEC] 
001A0040  ldr         r5, [r0, #0xF0] 
001A0044  mul         r1, r5, r1 
001A0048  add         r1, r1, r2, lsl #1 
001A004C  add         r1, r1, r4 
001A0050  sub         r2, r3, r2 
001A0054  add         r2, r1, r2, lsl #1 
001A0058  ldr         r0, [r0, #0xFC] 
001A005C  mov         r3, r0, lsr #10 
001A0060  mov         r0, r0, lsl #24 
001A0064  mov         r0, r0, lsr #27 
001A0068  orr         r0, r0, r3, lsl #5 
001A006C  strh        r0, [r1], #2! 
001A0070  cmp         r1, r2 
001A0074  bne         001A006C 
001A0078  ldmia       sp!, {r4, r5, pc} 

Gradient Color:

001A0038  stmdb       sp!, {r4 - r10, lr} 
001A003C  ldr         r4, [r0, #0xEC] 
001A0040  ldr         r5, [r0, #0xF0] 
001A0044  mul         r1, r5, r1 
001A0048  add         r1, r1, r2, lsl #1 
001A004C  add         r1, r1, r4 
001A0050  sub         r2, r3, r2 
001A0054  add         r2, r1, r2, lsl #1 
001A0058  ldr         r3, [r0, #0xC] 
001A005C  ldr         r4, [r0, #4] 
001A0060  ldr         r5, [r0, #0x1C] 
001A0064  ldr         r6, [r0, #0x14] 
001A0068  ldr         r7, [r0, #0x2C] 
001A006C  ldr         r0, [r0, #0x24] 
001A0070  mov         r8, r3, lsr #12 
001A0074  mov         r9, r5, lsr #12 
001A0078  mov         r10, r7, lsr #12 
001A007C  and         r8, r8, #0xF8 
001A0080  and         r9, r9, #0xFC 
001A0084  mov         r9, r9, lsl #3 
001A0088  orr         r8, r9, r8, lsr #3 
001A008C  and         r9, r10, #0xF8 
001A0090  orr         r8, r8, r9, lsl #8 
001A0094  strh        r8, [r1], #2! 
001A0098  add         r3, r3, r4 
001A009C  add         r5, r5, r6 
001A00A0  add         r7, r7, r0 
001A00A4  cmp         r1, r2 
001A00A8  bne         001A0070 
001A00AC  ldmia       sp!, {r4 - r10, pc} 

Affine Texture:

001A0038  stmdb       sp!, {r4 - r11, lr} 
001A003C  mov         r4, #1, 12 
001A0040  sub         r4, r4, #1 
001A0044  ldr         r5, [r0, #0xEC] 
001A0048  ldr         r6, [r0, #0xF0] 
001A004C  mul         r1, r6, r1 
001A0050  add         r1, r1, r2, lsl #1 
001A0054  add         r1, r1, r5 
001A0058  sub         r2, r3, r2 
001A005C  add         r2, r1, r2, lsl #1 
001A0060  ldr         r3, [r0, #0xC] 
001A0064  ldr         r5, [r0, #4] 
001A0068  ldr         r6, [r0, #0x1C] 
001A006C  ldr         r7, [r0, #0x14] 
001A0070  ldr         r0, [r0, #0xD0] 
001A0074  ldr         r8, [r0] 
001A0078  ldrb        r9, [r0, #4] 
001A007C  ldrb        r0, [r0, #8] 
001A0080  rsb         r10, r9, #0x14 
001A0084  rsb         r0, r0, #0x14 
001A0088  and         r11, r3, r4 
001A008C  and         lr, r6, r4 
001A0090  mov         lr, lr, lsr r0 
001A0094  mov         lr, lr, lsl r9 
001A0098  add         r11, lr, r11, lsr r10 
001A009C  add         r11, r8, r11, lsl #1 
001A00A0  ldrh        r11, [r11] 
001A00A4  strh        r11, [r1], #2! 
001A00A8  add         r3, r3, r5 
001A00AC  add         r6, r6, r7 
001A00B0  cmp         r1, r2 
001A00B4  bne         001A0088 
001A00B8  ldmia       sp!, {r4 - r11, pc}  

Perspective Texture:

001B0038  stmdb       sp!, {r4 - r11, lr} 
001B003C  sub         sp, sp, #0x20 
001B0040  mov         r4, r0 
001B0044  mov         r5, r2 
001B0048  str         r3, [sp, #0xC] 
001B004C  ldr         r0, [r4, #0xEC] 
001B0050  ldr         r2, [r4, #0xF0] 
001B0054  mul         r1, r2, r1 
001B0058  add         r1, r1, r5, lsl #1 
001B005C  add         r6, r1, r0 
001B0060  ldr         r0, [r4, #0xC] 
001B0064  str         r0, [sp, #0x14] 
001B0068  ldr         r0, [r4, #4] 
001B006C  str         r0, [sp] 
001B0070  ldr         r0, [r4, #0x1C] 
001B0074  str         r0, [sp, #0x18] 
001B0078  ldr         r0, [r4, #0x14] 
001B007C  str         r0, [sp, #4] 
001B0080  ldr         r0, [r4, #0x2C] 
001B0084  str         r0, [sp, #0x1C] 
001B0088  ldr         r0, [r4, #0x24] 
001B008C  str         r0, [sp, #8] 
001B0090  ldr         r0, [sp, #0x14] 
001B0094  bl          |__inverse ( 12dd31ch )| 
001B0098  ldr         r1, [sp, #0x18] 
001B009C  smull       r7, r2, r1, r0 
001B00A0  mov         r2, r2, lsl #20 
001B00A4  orr         r7, r2, r7, lsr #12 
001B00A8  ldr         r1, [sp, #0x1C] 
001B00AC  smull       r8, r0, r1, r0 
001B00B0  mov         r0, r0, lsl #20 
001B00B4  orr         r8, r0, r8, lsr #12 
001B00B8  ldr         r0, [r4, #0xD0] 
001B00BC  ldr         r1, [r0] 
001B00C0  str         r1, [sp, #0x10] 
001B00C4  ldrb        r4, [r0, #4] 
001B00C8  ldrb        r9, [r0, #8] 
001B00CC  ldr         r0, [sp, #0xC] 
001B00D0  sub         r5, r0, r5 
001B00D4  cmp         r5, #0x10 
001B00D8  movle       r0, r5 
001B00DC  movgt       r0, #0x10 
001B00E0  clz         r0, r0 
001B00E4  rsb         r10, r0, #0x1F 
001B00E8  mov         r0, #1 
001B00EC  mov         r11, r0, lsl r10 
001B00F0  sub         r5, r5, r11 
001B00F4  ldr         r0, [sp] 
001B00F8  ldr         r1, [sp, #0x14] 
001B00FC  add         r0, r1, r0, lsl r10 
001B0100  str         r0, [sp, #0x14] 
001B0104  bl          |__inverse ( 12dd31ch )| 
001B0108  ldr         r1, [sp, #4] 
001B010C  ldr         r2, [sp, #0x18] 
001B0110  add         r1, r2, r1, lsl r10 
001B0114  str         r1, [sp, #0x18] 
001B0118  smull       r1, r2, r0, r1 
001B011C  mov         r2, r2, lsl #20 
001B0120  orr         r1, r2, r1, lsr #12 
001B0124  sub         r1, r1, r7 
001B0128  ldr         r3, [sp, #8] 
001B012C  ldr         r2, [sp, #0x1C] 
001B0130  add         r2, r2, r3, lsl r10 
001B0134  str         r2, [sp, #0x1C] 
001B0138  smull       r2, r3, r0, r2 
001B013C  mov         r3, r3, lsl #20 
001B0140  orr         r2, r3, r2, lsr #12 
001B0144  sub         r0, r2, r8 
001B0148  mov         r1, r1, lsr r10 
001B014C  mov         r0, r0, lsr r10 

001B0150  mov         r2, #1, 12 
001B0154  sub         r2, r2, #1 
001B0158  and         r3, r7, r2 
001B015C  and         r2, r8, r2 
001B0160  rsb         r10, r4, #0x14 
001B0164  rsb         lr, r9, #0x14 
001B0168  mov         r2, r2, lsr lr 
001B016C  mov         r2, r2, lsl r4 
001B0170  add         r2, r2, r3, lsr r10 
001B0174  ldr         r3, [sp, #0x10] 
001B0178  add         r2, r3, r2, lsl #1 
001B017C  ldrh        r2, [r2] 
001B0180  strh        r2, [r6], #2! 
001B0184  add         r7, r7, r1 
001B0188  add         r8, r8, r0 
001B018C  sub         r11, r11, #1 
001B0190  cmp         r11, #0
001B0194  bne         001B0150
 
001B0198  cmp         r5, #0
001B019C  bne         001B00D4
 
001B01A0  add         sp, sp, #0x20 
001B01A4  ldmia       sp!, {r4 - r11, pc}  