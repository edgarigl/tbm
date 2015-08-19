	.text
	.align	2
	.globl	_check_df_const_to_reg
	.ent	_check_df_const_to_reg
	.type	_check_df_const_to_reg, @function
_check_df_const_to_reg:
	.frame	r1,0,r15		# vars= 0, regs= 0, args= 0
	.mask	0x00000000
 #(insn 9 2 12 (set (reg/i:DF 3 r3)
 #        (const_double:DF 8.1985529216486896e+16 [0x0.91a2b3c4d5e6f8p+57])) mb-testsuite/check_df.c:11 44 {*movdf_internal}
 #     (expr_list:REG_EQUAL (const_double:DF 8.1985529216486896e+16 [0x0.91a2b3c4d5e6f8p+57])
 #        (nil)))
	addik	r3,r0,0x789abcdf 	 # 9	*movdf_internal/3	[length = 8]
	addik	r4,r0,0x43723456 #Xfer Lo
 #(jump_insn 19 21 20 (parallel [
 #            (use (reg:SI 15 r15))
 #            (return)
 #        ]) mb-testsuite/check_df.c:11 72 {return_internal}
 #     (expr_list:REG_BR_PRED (const_int 12 [0xc])
 #        (expr_list:REG_DEAD (reg:SI 15 r15)
 #            (nil)))
 # -> return)
	rtsd	r15,8 	 # 19	return_internal	[length = 4]
	nop		# Unfilled delay slot

	.end	_check_df_const_to_reg
$Lfe1:
	.size	_check_df_const_to_reg,$Lfe1-_check_df_const_to_reg
	.rodata
	.align	2
$LC0:
	.ascii "%f\12\0"
	.align	2
$LC1:
	.ascii "ERROR: %s:%d\12\0"
	.align	2
$LC2:
	.ascii "mb-testsuite/check_df.c\0"
	.text
	.align	2
	.globl	check_df_const_to_reg
	.ent	check_df_const_to_reg
	.type	check_df_const_to_reg, @function
check_df_const_to_reg:
	.frame	r1,36,r15		# vars= 0, regs= 2, args= 24
	.mask	0x00c08000
 #(insn/f 43 10 45 (set (reg/f:SI 1 r1)
 #        (minus:SI (reg/f:SI 1 r1)
 #            (const_int 36 [0x24]))) mb-testsuite/check_df.c:15 9 {subsi3}
 #     (nil))
	addik	r1,r1,-36	 # 43	subsi3/2	[length = 8]
 #(insn/f 45 43 46 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 28 [0x1c])) [0 S4 A32])
 #        (reg:SI 22 r22)) mb-testsuite/check_df.c:15 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 22 r22)
 #        (nil)))
	swi	r22,r1,28	 # 45	*movsi_internal2/6	[length = 4]
 #(insn/f 46 45 44 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 32 [0x20])) [0 S4 A32])
 #        (reg:SI 23 r23)) mb-testsuite/check_df.c:15 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 23 r23)
 #        (nil)))
	swi	r23,r1,32	 # 46	*movsi_internal2/6	[length = 4]
 #(insn/f 44 46 47 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])
 #        (reg:SI 15 r15)) mb-testsuite/check_df.c:15 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 15 r15)
 #        (nil)))
	swi	r15,r1,0	 # 44	*movsi_internal2/6	[length = 4]
 #(call_insn/u 5 47 41 (parallel [
 #            (set (reg:DF 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("_check_df_const_to_reg") [flags 0x3] <function_decl 0x7f1793f7d600 _check_df_const_to_reg>) [0 _check_df_const_to_reg S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:16 77 {call_value_intern}
 #     (expr_list:REG_EH_REGION (const_int 0 [0])
 #        (nil))
 #    (nil))
	brlid	r15,_check_df_const_to_reg	 # 5	call_value_intern	[length = 4]
	nop		# Unfilled delay slot

 #(insn 41 5 42 (set (reg:SI 5 r5)
 #        (reg:SI 3 r3 [orig:42 D.2156 ] [42])) mb-testsuite/check_df.c:16 40 {*movsi_internal2}
 #     (nil))
	addk	r5,r3,r0	 # 41	*movsi_internal2/1	[length = 4]
 #(insn 42 41 8 (set (reg:SI 6 r6 [+4 ])
 #        (reg:SI 4 r4 [orig:23 D.2156+4 ] [23])) mb-testsuite/check_df.c:16 40 {*movsi_internal2}
 #     (nil))
	addk	r6,r4,r0	 # 42	*movsi_internal2/1	[length = 4]
 #(insn 8 42 39 (set (reg:DF 7 r7)
 #        (const_double:DF 8.1985529216486896e+16 [0x0.91a2b3c4d5e6f8p+57])) mb-testsuite/check_df.c:16 44 {*movdf_internal}
 #     (nil))
	addik	r7,r0,0x789abcdf 	 # 8	*movdf_internal/3	[length = 8]
	addik	r8,r0,0x43723456 #Xfer Lo
 #(insn 39 8 60 (set (reg:SI 22 r22 [orig:42 D.2156 ] [42])
 #        (reg:SI 3 r3)) mb-testsuite/check_df.c:16 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 3 r3)
 #        (nil)))
	addk	r22,r3,r0	 # 39	*movsi_internal2/1	[length = 4]
 #(call_insn/u 9 39 40 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("__nedf2") [flags 0x41]) [0 S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:16 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:DF 7 r7)
 #        (expr_list:REG_DEAD (reg:DF 5 r5)
 #            (expr_list:REG_EH_REGION (const_int -2147483648 [0xffffffff80000000])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:DF 5 r5))
 #            (nil))))
	brlid	r15,__nedf2	 # 9	call_value_intern	[length = 4]
	
 #(insn 40 9 72 (set (reg:SI 23 r23 [ D.2156+4 ])
 #        (reg:SI 4 r4 [+4 ])) 40 {*movsi_internal2}
 #     (nil))
	addk	r23,r4,r0	 # 40	*movsi_internal2/1	[length = 4]
 #(jump_insn 11 60 49 (set (pc)
 #        (if_then_else (ne:SI (reg:SI 3 r3)
 #                (const_int 0 [0]))
 #            (label_ref:SI 58)
 #            (pc))) mb-testsuite/check_df.c:16 66 {branch_zero}
 #     (expr_list:REG_BR_PRED (const_int 49 [0x31])
 #        (expr_list:REG_DEAD (reg:SI 3 r3)
 #            (expr_list:REG_BR_PROB (const_int 4 [0x4])
 #                (nil))))
 # -> 58)
	bneid	r3,$L8	 # 11	branch_zero	[length = 4]
 #(insn 49 11 29 (set (reg:SI 15 r15)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])) 40 {*movsi_internal2}
 #     (nil))
	lwi	r15,r1,0	 # 49	*movsi_internal2/4	[length = 4]
 #(insn 50 48 51 (set (reg:SI 22 r22)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 28 [0x1c])) [0 S4 A32])) mb-testsuite/check_df.c:20 40 {*movsi_internal2}
 #     (nil))
	lwi	r22,r1,28	 # 50	*movsi_internal2/4	[length = 4]
 #(insn 51 50 52 (set (reg:SI 23 r23)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 32 [0x20])) [0 S4 A32])) mb-testsuite/check_df.c:20 40 {*movsi_internal2}
 #     (nil))
	lwi	r23,r1,32	 # 51	*movsi_internal2/4	[length = 4]
 #(insn 52 51 69 (unspec_volatile [
 #            (const_int 0 [0])
 #        ] 0) mb-testsuite/check_df.c:20 73 {blockage}
 #     (nil))
 #(jump_insn 54 52 53 (parallel [
 #            (use (reg:SI 15 r15))
 #            (return)
 #        ]) mb-testsuite/check_df.c:20 72 {return_internal}
 #     (expr_list:REG_BR_PRED (const_int 12 [0xc])
 #        (expr_list:REG_DEAD (reg:SI 15 r15)
 #            (nil)))
 # -> return)
	rtsd	r15,8 	 # 54	return_internal	[length = 4]
	
 #(insn 53 54 70 (set (reg/f:SI 1 r1)
 #        (plus:SI (reg/f:SI 1 r1)
 #            (const_int 36 [0x24]))) 7 {addsi3}
 #     (nil))
	addik	r1,r1,36	 # 53	addsi3/2	[length = 4]
$L8:
 #(insn 16 15 37 (set (reg:SI 5 r5)
 #        (symbol_ref/f:SI ("*$LC0") [flags 0x2] <var_decl 0x7f1792e6f500 *$LC0>)) mb-testsuite/check_df.c:17 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,$LC0	 # 16	*movsi_internal2/3	[length = 8]
 #(insn 37 16 62 (set (reg:SI 6 r6)
 #        (reg:SI 22 r22 [orig:42 D.2156 ] [42])) mb-testsuite/check_df.c:17 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 22 r22 [orig:42 D.2156 ] [42])
 #        (nil)))
	addk	r6,r22,r0	 # 37	*movsi_internal2/1	[length = 4]
 #(call_insn 18 37 38 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1793f35c00 printf>) [0 __builtin_printf S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:17 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 5 r5)
 #        (expr_list:REG_DEAD (reg:DF 6 r6)
 #            (expr_list:REG_UNUSED (reg:SI 3 r3)
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 6 r6))
 #        (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #            (nil))))
	brlid	r15,printf	 # 18	call_value_intern	[length = 4]
	
 #(insn 38 18 19 (set (reg:SI 7 r7 [+4 ])
 #        (reg:SI 23 r23 [ D.2156+4 ])) 40 {*movsi_internal2}
 #     (nil))
	addk	r7,r23,r0	 # 38	*movsi_internal2/1	[length = 4]
 #(insn 19 62 20 (set (reg:SI 5 r5)
 #        (symbol_ref/f:SI ("*$LC1") [flags 0x2] <var_decl 0x7f1792e6f5a0 *$LC1>)) mb-testsuite/check_df.c:18 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,$LC1	 # 19	*movsi_internal2/3	[length = 8]
 #(insn 20 19 64 (set (reg:SI 6 r6)
 #        (symbol_ref/f:SI ("*$LC2") [flags 0x2] <var_decl 0x7f1792e6f640 *$LC2>)) mb-testsuite/check_df.c:18 40 {*movsi_internal2}
 #     (nil))
	addik	r6,r0,$LC2	 # 20	*movsi_internal2/3	[length = 8]
 #(call_insn 22 20 21 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1793f35c00 printf>) [0 __builtin_printf S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:18 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 7 r7)
 #        (expr_list:REG_DEAD (reg:SI 6 r6)
 #            (expr_list:REG_DEAD (reg:SI 5 r5)
 #                (expr_list:REG_UNUSED (reg:SI 3 r3)
 #                    (nil)))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:SI 6 r6))
 #            (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #                (nil)))))
	brlid	r15,printf	 # 22	call_value_intern	[length = 4]
	
 #(insn 21 22 66 (set (reg:SI 7 r7)
 #        (const_int 18 [0x12])) 38 {*movsi_internal3}
 #     (nil))
	addik	r7,r0,18	# 0x12	 # 21	*movsi_internal3/2	[length = 4]
 #(call_insn 24 64 23 (parallel [
 #            (call (mem:SI (symbol_ref:SI ("exit") [flags 0x41] <function_decl 0x7f1793f4ab00 exit>) [0 __builtin_exit S4 A32])
 #                (const_int 24 [0x18]))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:18 75 {call_internal1}
 #     (expr_list:REG_DEAD (reg:SI 5 r5)
 #        (expr_list:REG_NORETURN (const_int 0 [0])
 #            (expr_list:REG_EH_REGION (const_int 0 [0])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #        (nil)))
	brlid	r15,exit	 # 24	call_internal1	[length = 4]
	
 #(insn 23 24 67 (set (reg:SI 5 r5)
 #        (const_int 1 [0x1])) 38 {*movsi_internal3}
 #     (nil))
	addik	r5,r0,1	# 0x1	 # 23	*movsi_internal3/2	[length = 4]
	.end	check_df_const_to_reg
$Lfe2:
	.size	check_df_const_to_reg,$Lfe2-check_df_const_to_reg
	.rodata
	.align	2
$LC3:
	.ascii "%f %f\12\0"
	.text
	.align	2
	.globl	check_df_const_to_mem
	.ent	check_df_const_to_mem
	.type	check_df_const_to_mem, @function
check_df_const_to_mem:
	.frame	r1,36,r15		# vars= 0, regs= 2, args= 24
	.mask	0x00c08000
 #(insn 40 12 47 (set (reg:DF 6 r6)
 #        (const_double:DF 8.1985529216495072e+16 [0x0.91a2b3c4d5f6fp+57])) mb-testsuite/check_df.c:25 44 {*movdf_internal}
 #     (nil))
	addik	r6,r0,0x789abede 	 # 40	*movdf_internal/3	[length = 8]
	addik	r7,r0,0x43723456 #Xfer Lo
 #(insn/f 47 40 49 (set (reg/f:SI 1 r1)
 #        (minus:SI (reg/f:SI 1 r1)
 #            (const_int 36 [0x24]))) mb-testsuite/check_df.c:24 9 {subsi3}
 #     (nil))
	addik	r1,r1,-36	 # 47	subsi3/2	[length = 8]
 #(insn/f 49 47 50 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 28 [0x1c])) [0 S4 A32])
 #        (reg:SI 22 r22)) mb-testsuite/check_df.c:24 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 22 r22)
 #        (nil)))
	swi	r22,r1,28	 # 49	*movsi_internal2/6	[length = 4]
 #(insn/f 50 49 48 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 32 [0x20])) [0 S4 A32])
 #        (reg:SI 23 r23)) mb-testsuite/check_df.c:24 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 23 r23)
 #        (nil)))
	swi	r23,r1,32	 # 50	*movsi_internal2/6	[length = 4]
 #(insn/f 48 50 51 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])
 #        (reg:SI 15 r15)) mb-testsuite/check_df.c:24 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 15 r15)
 #        (nil)))
	swi	r15,r1,0	 # 48	*movsi_internal2/6	[length = 4]
 #(insn 7 51 8 (set (mem:DF (reg:SI 5 r5 [ p ]) [2 *p_1(D)+0 S8 A32])
 #        (reg:DF 6 r6)) mb-testsuite/check_df.c:25 44 {*movdf_internal}
 #     (expr_list:REG_DEAD (reg:SI 5 r5 [ p ])
 #        (nil)))
	swi	r6,r5,0	 # 7	*movdf_internal/5	[length = 8]
	swi	r7,r5,4
 #(insn 8 7 43 (set (reg:DF 22 r22 [orig:42 g_x.9 ] [42])
 #        (mem/c/i:DF (symbol_ref:SI ("g_x") <var_decl 0x7f1793eb3140 g_x>) [2 g_x+0 S8 A32])) mb-testsuite/check_df.c:26 44 {*movdf_internal}
 #     (nil))
	lwi	r22,r0,g_x	 # 8	*movdf_internal/2	[length = 8]
	lwi	r23,r0,g_x+4
 #(insn 43 8 44 (set (reg:SI 8 r8 [+4 ])
 #        (reg:SI 7 r7 [+4 ])) mb-testsuite/check_df.c:26 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 7 r7 [+4 ])
 #        (nil)))
	addk	r8,r7,r0	 # 43	*movsi_internal2/1	[length = 4]
 #(insn 44 43 45 (set (reg:SI 7 r7)
 #        (reg:SI 6 r6)) mb-testsuite/check_df.c:26 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 6 r6)
 #        (nil)))
	addk	r7,r6,r0	 # 44	*movsi_internal2/1	[length = 4]
 #(insn 45 44 62 (set (reg:SI 5 r5)
 #        (reg:SI 22 r22 [orig:42 g_x.9 ] [42])) mb-testsuite/check_df.c:26 40 {*movsi_internal2}
 #     (nil))
	addk	r5,r22,r0	 # 45	*movsi_internal2/1	[length = 4]
 #(call_insn/u 11 45 46 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("__nedf2") [flags 0x41]) [0 S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:26 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:DF 7 r7)
 #        (expr_list:REG_DEAD (reg:DF 5 r5)
 #            (expr_list:REG_EH_REGION (const_int -2147483648 [0xffffffff80000000])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:DF 5 r5))
 #            (nil))))
	brlid	r15,__nedf2	 # 11	call_value_intern	[length = 4]
	
 #(insn 46 11 74 (set (reg:SI 6 r6 [+4 ])
 #        (reg:SI 23 r23 [ g_x.9+4 ])) 40 {*movsi_internal2}
 #     (nil))
	addk	r6,r23,r0	 # 46	*movsi_internal2/1	[length = 4]
 #(jump_insn 13 62 53 (set (pc)
 #        (if_then_else (ne:SI (reg:SI 3 r3)
 #                (const_int 0 [0]))
 #            (label_ref:SI 60)
 #            (pc))) mb-testsuite/check_df.c:26 66 {branch_zero}
 #     (expr_list:REG_BR_PRED (const_int 49 [0x31])
 #        (expr_list:REG_DEAD (reg:SI 3 r3)
 #            (expr_list:REG_BR_PROB (const_int 4 [0x4])
 #                (nil))))
 # -> 60)
	bneid	r3,$L14	 # 13	branch_zero	[length = 4]
 #(insn 53 13 32 (set (reg:SI 15 r15)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])) 40 {*movsi_internal2}
 #     (nil))
	lwi	r15,r1,0	 # 53	*movsi_internal2/4	[length = 4]
 #(insn 54 52 55 (set (reg:SI 22 r22)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 28 [0x1c])) [0 S4 A32])) mb-testsuite/check_df.c:30 40 {*movsi_internal2}
 #     (nil))
	lwi	r22,r1,28	 # 54	*movsi_internal2/4	[length = 4]
 #(insn 55 54 56 (set (reg:SI 23 r23)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 32 [0x20])) [0 S4 A32])) mb-testsuite/check_df.c:30 40 {*movsi_internal2}
 #     (nil))
	lwi	r23,r1,32	 # 55	*movsi_internal2/4	[length = 4]
 #(insn 56 55 71 (unspec_volatile [
 #            (const_int 0 [0])
 #        ] 0) mb-testsuite/check_df.c:30 73 {blockage}
 #     (nil))
 #(jump_insn 58 56 57 (parallel [
 #            (use (reg:SI 15 r15))
 #            (return)
 #        ]) mb-testsuite/check_df.c:30 72 {return_internal}
 #     (expr_list:REG_BR_PRED (const_int 12 [0xc])
 #        (expr_list:REG_DEAD (reg:SI 15 r15)
 #            (nil)))
 # -> return)
	rtsd	r15,8 	 # 58	return_internal	[length = 4]
	
 #(insn 57 58 72 (set (reg/f:SI 1 r1)
 #        (plus:SI (reg/f:SI 1 r1)
 #            (const_int 36 [0x24]))) 7 {addsi3}
 #     (nil))
	addik	r1,r1,36	 # 57	addsi3/2	[length = 4]
$L14:
 #(insn 18 17 20 (set (reg:SI 5 r5)
 #        (symbol_ref/f:SI ("*$LC3") [flags 0x2] <var_decl 0x7f1792e6f780 *$LC3>)) mb-testsuite/check_df.c:27 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,$LC3	 # 18	*movsi_internal2/3	[length = 8]
 #(insn 20 18 41 (set (reg:DF 8 r8)
 #        (const_double:DF 8.1985529216495072e+16 [0x0.91a2b3c4d5f6fp+57])) mb-testsuite/check_df.c:27 44 {*movdf_internal}
 #     (expr_list:REG_EQUAL (const_double:DF 8.1985529216495072e+16 [0x0.91a2b3c4d5f6fp+57])
 #        (nil)))
	addik	r8,r0,0x789abede 	 # 20	*movdf_internal/3	[length = 8]
	addik	r9,r0,0x43723456 #Xfer Lo
 #(insn 41 20 64 (set (reg:SI 6 r6)
 #        (reg:SI 22 r22 [orig:42 g_x.9 ] [42])) mb-testsuite/check_df.c:27 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 22 r22 [orig:42 g_x.9 ] [42])
 #        (nil)))
	addk	r6,r22,r0	 # 41	*movsi_internal2/1	[length = 4]
 #(call_insn 21 41 42 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1793f35c00 printf>) [0 __builtin_printf S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:27 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 5 r5)
 #        (expr_list:REG_DEAD (reg:DF 8 r8)
 #            (expr_list:REG_DEAD (reg:DF 6 r6)
 #                (expr_list:REG_UNUSED (reg:SI 3 r3)
 #                    (nil)))))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 8 r8))
 #        (expr_list:REG_DEP_TRUE (use (reg:DF 6 r6))
 #            (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #                (nil)))))
	brlid	r15,printf	 # 21	call_value_intern	[length = 4]
	
 #(insn 42 21 22 (set (reg:SI 7 r7 [+4 ])
 #        (reg:SI 23 r23 [ g_x.9+4 ])) 40 {*movsi_internal2}
 #     (nil))
	addk	r7,r23,r0	 # 42	*movsi_internal2/1	[length = 4]
 #(insn 22 64 23 (set (reg:SI 5 r5)
 #        (symbol_ref/f:SI ("*$LC1") [flags 0x2] <var_decl 0x7f1792e6f5a0 *$LC1>)) mb-testsuite/check_df.c:28 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,$LC1	 # 22	*movsi_internal2/3	[length = 8]
 #(insn 23 22 66 (set (reg:SI 6 r6)
 #        (symbol_ref/f:SI ("*$LC2") [flags 0x2] <var_decl 0x7f1792e6f640 *$LC2>)) mb-testsuite/check_df.c:28 40 {*movsi_internal2}
 #     (nil))
	addik	r6,r0,$LC2	 # 23	*movsi_internal2/3	[length = 8]
 #(call_insn 25 23 24 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1793f35c00 printf>) [0 __builtin_printf S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:28 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 7 r7)
 #        (expr_list:REG_DEAD (reg:SI 6 r6)
 #            (expr_list:REG_DEAD (reg:SI 5 r5)
 #                (expr_list:REG_UNUSED (reg:SI 3 r3)
 #                    (nil)))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:SI 6 r6))
 #            (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #                (nil)))))
	brlid	r15,printf	 # 25	call_value_intern	[length = 4]
	
 #(insn 24 25 68 (set (reg:SI 7 r7)
 #        (const_int 28 [0x1c])) 38 {*movsi_internal3}
 #     (nil))
	addik	r7,r0,28	# 0x1c	 # 24	*movsi_internal3/2	[length = 4]
 #(call_insn 27 66 26 (parallel [
 #            (call (mem:SI (symbol_ref:SI ("exit") [flags 0x41] <function_decl 0x7f1793f4ab00 exit>) [0 __builtin_exit S4 A32])
 #                (const_int 24 [0x18]))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:28 75 {call_internal1}
 #     (expr_list:REG_DEAD (reg:SI 5 r5)
 #        (expr_list:REG_NORETURN (const_int 0 [0])
 #            (expr_list:REG_EH_REGION (const_int 0 [0])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #        (nil)))
	brlid	r15,exit	 # 27	call_internal1	[length = 4]
	
 #(insn 26 27 69 (set (reg:SI 5 r5)
 #        (const_int 1 [0x1])) 38 {*movsi_internal3}
 #     (nil))
	addik	r5,r0,1	# 0x1	 # 26	*movsi_internal3/2	[length = 4]
	.end	check_df_const_to_mem
$Lfe3:
	.size	check_df_const_to_mem,$Lfe3-check_df_const_to_mem
	.align	2
	.globl	_check_df_reg_to_reg
	.ent	_check_df_reg_to_reg
	.type	_check_df_reg_to_reg, @function
_check_df_reg_to_reg:
	.frame	r1,0,r15		# vars= 0, regs= 0, args= 0
	.mask	0x00000000
 #(insn 18 3 20 (set (reg:SI 4 r4 [orig:43 a ] [43])
 #        (reg:SI 5 r5 [ a ])) mb-testsuite/check_df.c:34 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 5 r5 [ a ])
 #        (nil)))
	addk	r4,r5,r0	 # 18	*movsi_internal2/1	[length = 4]
 #(insn 20 18 13 (set (reg:SI 3 r3)
 #        (reg:SI 4 r4 [orig:43 a ] [43])) mb-testsuite/check_df.c:36 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 4 r4 [orig:43 a ] [43])
 #        (nil)))
	addk	r3,r4,r0	 # 20	*movsi_internal2/1	[length = 4]
 #(jump_insn 24 26 21 (parallel [
 #            (use (reg:SI 15 r15))
 #            (return)
 #        ]) mb-testsuite/check_df.c:36 72 {return_internal}
 #     (expr_list:REG_BR_PRED (const_int 12 [0xc])
 #        (expr_list:REG_DEAD (reg:SI 15 r15)
 #            (nil)))
 # -> return)
	rtsd	r15,8 	 # 24	return_internal	[length = 4]
	
 #(insn 21 24 29 (set (reg:SI 4 r4 [+4 ])
 #        (reg:SI 6 r6 [orig:5 a+4 ] [5])) 40 {*movsi_internal2}
 #     (nil))
	addk	r4,r6,r0	 # 21	*movsi_internal2/1	[length = 4]
	.end	_check_df_reg_to_reg
$Lfe4:
	.size	_check_df_reg_to_reg,$Lfe4-_check_df_reg_to_reg
	.rodata
	.align	2
$LC4:
	.ascii "a=%lx.%lx\12\0"
	.align	2
$LC5:
	.ascii "b=%lx.%lx\12\0"
	.align	2
$LC6:
	.ascii "r=%lx.%lx sizeof=%d\12\0"
	.text
	.align	2
	.globl	_check_df_reg_to_add_reg
	.ent	_check_df_reg_to_add_reg
	.type	_check_df_reg_to_add_reg, @function
_check_df_reg_to_add_reg:
	.frame	r1,44,r15		# vars= 8, regs= 2, args= 24
	.mask	0x00c08000
 #(insn/f 51 28 2 (set (reg/f:SI 1 r1)
 #        (minus:SI (reg/f:SI 1 r1)
 #            (const_int 44 [0x2c]))) mb-testsuite/check_df.c:40 9 {subsi3}
 #     (nil))
	addik	r1,r1,-44	 # 51	subsi3/2	[length = 8]
 #(insn 2 51 3 (set (mem/c/i:DF (plus:SI (reg/f:SI 1 r1)
 #                (const_int 48 [0x30])) [2 a+0 S8 A32])
 #        (reg:DF 5 r5 [ a ])) mb-testsuite/check_df.c:40 44 {*movdf_internal}
 #     (expr_list:REG_DEAD (reg:DF 5 r5 [ a ])
 #        (nil)))
	swi	r5,r1,48	 # 2	*movdf_internal/5	[length = 8]
	swi	r6,r1,52
 #(insn 3 2 10 (set (mem/c/i:DF (plus:SI (reg/f:SI 1 r1)
 #                (const_int 56 [0x38])) [2 b+0 S8 A32])
 #        (reg:DF 7 r7 [ b ])) mb-testsuite/check_df.c:40 44 {*movdf_internal}
 #     (expr_list:REG_DEAD (reg:DF 7 r7 [ b ])
 #        (nil)))
	swi	r7,r1,56	 # 3	*movdf_internal/5	[length = 8]
	swi	r8,r1,60
 #(insn 10 3 11 (set (reg:SI 6 r6)
 #        (mem/c:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 48 [0x30])) [3 MEM[(unsigned int *)&a]+0 S4 A32])) mb-testsuite/check_df.c:42 40 {*movsi_internal2}
 #     (nil))
	lwi	r6,r1,48	 # 10	*movsi_internal2/4	[length = 4]
 #(insn 11 10 9 (set (reg:SI 7 r7)
 #        (mem/c:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 52 [0x34])) [3 MEM[(unsigned int *)&a + 4B]+0 S4 A32])) mb-testsuite/check_df.c:42 40 {*movsi_internal2}
 #     (nil))
	lwi	r7,r1,52	 # 11	*movsi_internal2/4	[length = 4]
 #(insn 9 11 52 (set (reg:SI 5 r5)
 #        (symbol_ref/f:SI ("*$LC4") [flags 0x2] <var_decl 0x7f1792e6f960 *$LC4>)) mb-testsuite/check_df.c:42 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,$LC4	 # 9	*movsi_internal2/3	[length = 8]
 #(insn/f 52 9 53 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])
 #        (reg:SI 15 r15)) mb-testsuite/check_df.c:40 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 15 r15)
 #        (nil)))
	swi	r15,r1,0	 # 52	*movsi_internal2/6	[length = 4]
 #(insn/f 53 52 55 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 36 [0x24])) [0 S4 A32])
 #        (reg:SI 22 r22)) mb-testsuite/check_df.c:40 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 22 r22)
 #        (nil)))
	swi	r22,r1,36	 # 53	*movsi_internal2/6	[length = 4]
 #(call_insn 12 55 54 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1793f35c00 printf>) [0 __builtin_printf S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:42 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 7 r7)
 #        (expr_list:REG_DEAD (reg:SI 6 r6)
 #            (expr_list:REG_DEAD (reg:SI 5 r5)
 #                (expr_list:REG_UNUSED (reg:SI 3 r3)
 #                    (nil)))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:SI 6 r6))
 #            (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #                (nil)))))
	brlid	r15,printf	 # 12	call_value_intern	[length = 4]
	
 #(insn/f 54 12 18 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 40 [0x28])) [0 S4 A32])
 #        (reg:SI 23 r23)) 40 {*movsi_internal2}
 #     (nil))
	swi	r23,r1,40	 # 54	*movsi_internal2/6	[length = 4]
 #(insn 18 66 17 (set (reg:SI 6 r6)
 #        (mem/c:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 56 [0x38])) [3 MEM[(unsigned int *)&b]+0 S4 A32])) mb-testsuite/check_df.c:43 40 {*movsi_internal2}
 #     (nil))
	lwi	r6,r1,56	 # 18	*movsi_internal2/4	[length = 4]
 #(insn 17 18 68 (set (reg:SI 5 r5)
 #        (symbol_ref/f:SI ("*$LC5") [flags 0x2] <var_decl 0x7f1792e6fa00 *$LC5>)) mb-testsuite/check_df.c:43 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,$LC5	 # 17	*movsi_internal2/3	[length = 8]
 #(call_insn 20 17 19 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1793f35c00 printf>) [0 __builtin_printf S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:43 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 7 r7)
 #        (expr_list:REG_DEAD (reg:SI 6 r6)
 #            (expr_list:REG_DEAD (reg:SI 5 r5)
 #                (expr_list:REG_UNUSED (reg:SI 3 r3)
 #                    (nil)))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:SI 6 r6))
 #            (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #                (nil)))))
	brlid	r15,printf	 # 20	call_value_intern	[length = 4]
	
 #(insn 19 20 21 (set (reg:SI 7 r7)
 #        (mem/c:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 60 [0x3c])) [3 MEM[(unsigned int *)&b + 4B]+0 S4 A32])) 40 {*movsi_internal2}
 #     (nil))
	lwi	r7,r1,60	 # 19	*movsi_internal2/4	[length = 4]
 #(insn 21 68 22 (set (reg:DF 5 r5)
 #        (mem/c/i:DF (plus:SI (reg/f:SI 1 r1)
 #                (const_int 48 [0x30])) [2 a+0 S8 A32])) mb-testsuite/check_df.c:44 44 {*movdf_internal}
 #     (nil))
	lwi	r5,r1,48	 # 21	*movdf_internal/2	[length = 8]
	lwi	r6,r1,52
 #(insn 22 21 23 (set (reg:DF 7 r7)
 #        (mem/c/i:DF (plus:SI (reg/f:SI 1 r1)
 #                (const_int 56 [0x38])) [2 b+0 S8 A32])) mb-testsuite/check_df.c:44 44 {*movdf_internal}
 #     (nil))
	lwi	r7,r1,56	 # 22	*movdf_internal/2	[length = 8]
	lwi	r8,r1,60
 #(call_insn/u 23 22 47 (parallel [
 #            (set (reg:DF 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("__adddf3") [flags 0x41]) [0 S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:44 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:DF 7 r7)
 #        (expr_list:REG_DEAD (reg:DF 5 r5)
 #            (expr_list:REG_EH_REGION (const_int -2147483648 [0xffffffff80000000])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:DF 5 r5))
 #            (nil))))
	brlid	r15,__adddf3	 # 23	call_value_intern	[length = 4]
	nop		# Unfilled delay slot

 #(insn 47 23 48 (set (reg:SI 22 r22 [58])
 #        (reg:SI 3 r3)) mb-testsuite/check_df.c:44 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 3 r3)
 #        (nil)))
	addk	r22,r3,r0	 # 47	*movsi_internal2/1	[length = 4]
 #(insn 48 47 29 (set (reg:SI 23 r23 [+4 ])
 #        (reg:SI 4 r4 [+4 ])) mb-testsuite/check_df.c:44 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 4 r4 [+4 ])
 #        (nil)))
	addk	r23,r4,r0	 # 48	*movsi_internal2/1	[length = 4]
 #(insn 29 48 32 (set (reg:SI 5 r5)
 #        (symbol_ref/f:SI ("*$LC6") [flags 0x2] <var_decl 0x7f1792e6faa0 *$LC6>)) mb-testsuite/check_df.c:45 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,$LC6	 # 29	*movsi_internal2/3	[length = 8]
 #(insn 32 29 26 (set (reg:SI 8 r8)
 #        (const_int 8 [0x8])) mb-testsuite/check_df.c:45 38 {*movsi_internal3}
 #     (nil))
	addik	r8,r0,8	# 0x8	 # 32	*movsi_internal3/2	[length = 4]
 #(insn 26 32 30 (set (mem/c/i:DF (plus:SI (reg/f:SI 1 r1)
 #                (const_int 28 [0x1c])) [2 r+0 S8 A32])
 #        (reg:DF 22 r22 [58])) mb-testsuite/check_df.c:44 44 {*movdf_internal}
 #     (nil))
	swi	r22,r1,28	 # 26	*movdf_internal/5	[length = 8]
	swi	r23,r1,32
 #(insn 30 26 70 (set (reg:SI 6 r6)
 #        (mem/c:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 28 [0x1c])) [3 MEM[(unsigned int *)&r]+0 S4 A32])) mb-testsuite/check_df.c:45 40 {*movsi_internal2}
 #     (nil))
	lwi	r6,r1,28	 # 30	*movsi_internal2/4	[length = 4]
 #(call_insn 33 30 31 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1793f35c00 printf>) [0 __builtin_printf S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:45 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 8 r8)
 #        (expr_list:REG_DEAD (reg:SI 7 r7)
 #            (expr_list:REG_DEAD (reg:SI 6 r6)
 #                (expr_list:REG_DEAD (reg:SI 5 r5)
 #                    (expr_list:REG_UNUSED (reg:SI 3 r3)
 #                        (nil))))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 8 r8))
 #        (expr_list:REG_DEP_TRUE (use (reg:SI 7 r7))
 #            (expr_list:REG_DEP_TRUE (use (reg:SI 6 r6))
 #                (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #                    (nil))))))
	brlid	r15,printf	 # 33	call_value_intern	[length = 4]
	
 #(insn 31 33 49 (set (reg:SI 7 r7)
 #        (mem/c:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 32 [0x20])) [3 MEM[(unsigned int *)&r + 4B]+0 S4 A32])) 40 {*movsi_internal2}
 #     (nil))
	lwi	r7,r1,32	 # 31	*movsi_internal2/4	[length = 4]
 #(insn 49 70 50 (set (reg:SI 3 r3)
 #        (reg:SI 22 r22 [58])) mb-testsuite/check_df.c:47 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 22 r22 [58])
 #        (nil)))
	addk	r3,r22,r0	 # 49	*movsi_internal2/1	[length = 4]
 #(insn 50 49 64 (set (reg:SI 4 r4 [+4 ])
 #        (reg:SI 23 r23 [+4 ])) mb-testsuite/check_df.c:47 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 23 r23 [+4 ])
 #        (nil)))
	addk	r4,r23,r0	 # 50	*movsi_internal2/1	[length = 4]
 #(insn 57 64 58 (set (reg:SI 15 r15)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])) mb-testsuite/check_df.c:47 40 {*movsi_internal2}
 #     (nil))
	lwi	r15,r1,0	 # 57	*movsi_internal2/4	[length = 4]
 #(insn 58 57 59 (set (reg:SI 22 r22)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 36 [0x24])) [0 S4 A32])) mb-testsuite/check_df.c:47 40 {*movsi_internal2}
 #     (nil))
	lwi	r22,r1,36	 # 58	*movsi_internal2/4	[length = 4]
 #(insn 59 58 41 (set (reg:SI 23 r23)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 40 [0x28])) [0 S4 A32])) mb-testsuite/check_df.c:47 40 {*movsi_internal2}
 #     (nil))
	lwi	r23,r1,40	 # 59	*movsi_internal2/4	[length = 4]
 #(insn 60 41 72 (unspec_volatile [
 #            (const_int 0 [0])
 #        ] 0) mb-testsuite/check_df.c:47 73 {blockage}
 #     (nil))
 #(jump_insn 62 60 61 (parallel [
 #            (use (reg:SI 15 r15))
 #            (return)
 #        ]) mb-testsuite/check_df.c:47 72 {return_internal}
 #     (expr_list:REG_BR_PRED (const_int 12 [0xc])
 #        (expr_list:REG_DEAD (reg:SI 15 r15)
 #            (nil)))
 # -> return)
	rtsd	r15,8 	 # 62	return_internal	[length = 4]
	
 #(insn 61 62 73 (set (reg/f:SI 1 r1)
 #        (plus:SI (reg/f:SI 1 r1)
 #            (const_int 44 [0x2c]))) 7 {addsi3}
 #     (nil))
	addik	r1,r1,44	 # 61	addsi3/2	[length = 4]
	.end	_check_df_reg_to_add_reg
$Lfe5:
	.size	_check_df_reg_to_add_reg,$Lfe5-_check_df_reg_to_add_reg
	.align	2
	.globl	check_df_reg_to_reg
	.ent	check_df_reg_to_reg
	.type	check_df_reg_to_reg, @function
check_df_reg_to_reg:
	.frame	r1,28,r15		# vars= 0, regs= 0, args= 24
	.mask	0x00008000
 #(insn 5 13 73 (set (reg:SI 5 r5)
 #        (symbol_ref:SI ("__func__.1270") [flags 0x2] <var_decl 0x7f1793eb3780 __func__>)) mb-testsuite/check_df.c:52 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,__func__.1270	 # 5	*movsi_internal2/3	[length = 8]
 #(insn/f 73 5 74 (set (reg/f:SI 1 r1)
 #        (minus:SI (reg/f:SI 1 r1)
 #            (const_int 28 [0x1c]))) mb-testsuite/check_df.c:50 9 {subsi3}
 #     (nil))
	addik	r1,r1,-28	 # 73	subsi3/2	[length = 8]
 #(insn/f 74 73 75 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])
 #        (reg:SI 15 r15)) mb-testsuite/check_df.c:50 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 15 r15)
 #        (nil)))
	swi	r15,r1,0	 # 74	*movsi_internal2/6	[length = 4]
 #(call_insn 6 75 7 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("puts") [flags 0x41] <function_decl 0x7f1793f37300 __builtin_puts>) [0 __builtin_puts S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:52 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 5 r5)
 #        (expr_list:REG_UNUSED (reg:SI 3 r3)
 #            (nil)))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #        (nil)))
	brlid	r15,puts	 # 6	call_value_intern	[length = 4]
	nop		# Unfilled delay slot

 #(insn 7 6 8 (set (reg:DF 5 r5)
 #        (const_double:DF 1.7140398773548019712e+19 [0x0.eddeeddeeddefp+64])) mb-testsuite/check_df.c:53 44 {*movdf_internal}
 #     (nil))
	addik	r5,r0,0xbbddbbde 	 # 7	*movdf_internal/3	[length = 8]
	addik	r6,r0,0x43edbbdd #Xfer Lo
 #(call_insn/u 8 7 69 (parallel [
 #            (set (reg:DF 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("_check_df_reg_to_reg") [flags 0x3] <function_decl 0x7f1793f7dc00 _check_df_reg_to_reg>) [0 _check_df_reg_to_reg S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:53 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:DF 5 r5)
 #        (expr_list:REG_EH_REGION (const_int 0 [0])
 #            (nil)))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 5 r5))
 #        (nil)))
	brlid	r15,_check_df_reg_to_reg	 # 8	call_value_intern	[length = 4]
	nop		# Unfilled delay slot

 #(insn 69 8 11 (set (reg:SI 5 r5 [ D.2126+4 ])
 #        (reg:SI 4 r4 [+4 ])) mb-testsuite/check_df.c:53 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 4 r4 [+4 ])
 #        (nil)))
	addk	r5,r4,r0	 # 69	*movsi_internal2/1	[length = 4]
 #(insn 11 69 71 (set (reg:DF 7 r7)
 #        (const_double:DF 1.7140398773548019712e+19 [0x0.eddeeddeeddefp+64])) mb-testsuite/check_df.c:53 44 {*movdf_internal}
 #     (nil))
	addik	r7,r0,0xbbddbbde 	 # 11	*movdf_internal/3	[length = 8]
	addik	r8,r0,0x43edbbdd #Xfer Lo
 #(insn 71 11 85 (set (reg:SI 6 r6 [+4 ])
 #        (reg:SI 5 r5 [ D.2126+4 ])) mb-testsuite/check_df.c:53 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 5 r5 [ D.2126+4 ])
 #        (nil)))
	addk	r6,r5,r0	 # 71	*movsi_internal2/1	[length = 4]
 #(call_insn/u 12 71 72 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("__nedf2") [flags 0x41]) [0 S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:53 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:DF 7 r7)
 #        (expr_list:REG_DEAD (reg:DF 5 r5)
 #            (expr_list:REG_EH_REGION (const_int -2147483648 [0xffffffff80000000])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:DF 5 r5))
 #            (nil))))
	brlid	r15,__nedf2	 # 12	call_value_intern	[length = 4]
	
 #(insn 72 12 14 (set (reg:SI 5 r5)
 #        (reg:SI 3 r3 [orig:42 D.2126 ] [42])) 40 {*movsi_internal2}
 #     (nil))
	addk	r5,r3,r0	 # 72	*movsi_internal2/1	[length = 4]
 #(jump_insn 14 85 27 (set (pc)
 #        (if_then_else (ne:SI (reg:SI 3 r3)
 #                (const_int 0 [0]))
 #            (label_ref:SI 82)
 #            (pc))) mb-testsuite/check_df.c:53 66 {branch_zero}
 #     (expr_list:REG_BR_PRED (const_int 49 [0x31])
 #        (expr_list:REG_DEAD (reg:SI 3 r3)
 #            (expr_list:REG_BR_PROB (const_int 4 [0x4])
 #                (nil))))
 # -> 82)
	bnei	r3,$L26	 # 14	branch_zero	[length = 4]
 #(insn 28 38 29 (set (reg:DF 5 r5)
 #        (const_double:DF 1.7140398773548019712e+19 [0x0.eddeeddeeddefp+64])) mb-testsuite/check_df.c:56 44 {*movdf_internal}
 #     (nil))
	addik	r5,r0,0xbbddbbde 	 # 28	*movdf_internal/3	[length = 8]
	addik	r6,r0,0x43edbbdd #Xfer Lo
 #(insn 29 28 30 (set (reg:DF 7 r7)
 #        (const_double:DF 1.152921504606846976e+18 [0x0.8p+61])) mb-testsuite/check_df.c:56 44 {*movdf_internal}
 #     (nil))
	addik	r7,r0,0x00000000 	 # 29	*movdf_internal/3	[length = 8]
	addik	r8,r0,0x43b00000 #Xfer Lo
 #(call_insn 30 29 31 (parallel [
 #            (set (reg:DF 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("_check_df_reg_to_add_reg") [flags 0x3] <function_decl 0x7f1793f7de00 _check_df_reg_to_add_reg>) [0 _check_df_reg_to_add_reg S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:56 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:DF 7 r7)
 #        (expr_list:REG_DEAD (reg:DF 5 r5)
 #            (expr_list:REG_UNUSED (reg:DF 3 r3)
 #                (expr_list:REG_EH_REGION (const_int 0 [0])
 #                    (nil)))))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:DF 5 r5))
 #            (nil))))
	brlid	r15,_check_df_reg_to_add_reg	 # 30	call_value_intern	[length = 4]
	nop		# Unfilled delay slot

 #(insn 31 30 32 (set (reg:DF 5 r5)
 #        (const_double:DF 1.7140398773548019712e+19 [0x0.eddeeddeeddefp+64])) mb-testsuite/check_df.c:58 44 {*movdf_internal}
 #     (nil))
	addik	r5,r0,0xbbddbbde 	 # 31	*movdf_internal/3	[length = 8]
	addik	r6,r0,0x43edbbdd #Xfer Lo
 #(insn 32 31 33 (set (reg:DF 7 r7)
 #        (const_double:DF 1.152921504606846976e+18 [0x0.8p+61])) mb-testsuite/check_df.c:58 44 {*movdf_internal}
 #     (nil))
	addik	r7,r0,0x00000000 	 # 32	*movdf_internal/3	[length = 8]
	addik	r8,r0,0x43b00000 #Xfer Lo
 #(call_insn 33 32 65 (parallel [
 #            (set (reg:DF 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("_check_df_reg_to_add_reg") [flags 0x3] <function_decl 0x7f1793f7de00 _check_df_reg_to_add_reg>) [0 _check_df_reg_to_add_reg S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:58 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:DF 7 r7)
 #        (expr_list:REG_DEAD (reg:DF 5 r5)
 #            (expr_list:REG_EH_REGION (const_int 0 [0])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:DF 5 r5))
 #            (nil))))
	brlid	r15,_check_df_reg_to_add_reg	 # 33	call_value_intern	[length = 4]
	nop		# Unfilled delay slot

 #(insn 65 33 36 (set (reg:SI 5 r5 [ D.2130+4 ])
 #        (reg:SI 4 r4 [+4 ])) mb-testsuite/check_df.c:58 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 4 r4 [+4 ])
 #        (nil)))
	addk	r5,r4,r0	 # 65	*movsi_internal2/1	[length = 4]
 #(insn 36 65 67 (set (reg:DF 7 r7)
 #        (const_double:DF 1.8293320278154866688e+19 [0x0.fddeeddeeddefp+64])) mb-testsuite/check_df.c:58 44 {*movdf_internal}
 #     (nil))
	addik	r7,r0,0xbbddbbde 	 # 36	*movdf_internal/3	[length = 8]
	addik	r8,r0,0x43efbbdd #Xfer Lo
 #(insn 67 36 87 (set (reg:SI 6 r6 [+4 ])
 #        (reg:SI 5 r5 [ D.2130+4 ])) mb-testsuite/check_df.c:58 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 5 r5 [ D.2130+4 ])
 #        (nil)))
	addk	r6,r5,r0	 # 67	*movsi_internal2/1	[length = 4]
 #(call_insn/u 37 67 68 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("__nedf2") [flags 0x41]) [0 S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:58 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:DF 7 r7)
 #        (expr_list:REG_DEAD (reg:DF 5 r5)
 #            (expr_list:REG_EH_REGION (const_int -2147483648 [0xffffffff80000000])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:DF 5 r5))
 #            (nil))))
	brlid	r15,__nedf2	 # 37	call_value_intern	[length = 4]
	
 #(insn 68 37 102 (set (reg:SI 5 r5)
 #        (reg:SI 3 r3 [orig:43 D.2130 ] [43])) 40 {*movsi_internal2}
 #     (nil))
	addk	r5,r3,r0	 # 68	*movsi_internal2/1	[length = 4]
 #(jump_insn 39 87 77 (set (pc)
 #        (if_then_else (ne:SI (reg:SI 3 r3)
 #                (const_int 0 [0]))
 #            (label_ref:SI 83)
 #            (pc))) mb-testsuite/check_df.c:58 66 {branch_zero}
 #     (expr_list:REG_BR_PRED (const_int 49 [0x31])
 #        (expr_list:REG_DEAD (reg:SI 3 r3)
 #            (expr_list:REG_BR_PROB (const_int 4 [0x4])
 #                (nil))))
 # -> 83)
	bneid	r3,$L27	 # 39	branch_zero	[length = 4]
 #(insn 77 39 54 (set (reg:SI 15 r15)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])) 40 {*movsi_internal2}
 #     (nil))
	lwi	r15,r1,0	 # 77	*movsi_internal2/4	[length = 4]
 #(insn 78 76 99 (unspec_volatile [
 #            (const_int 0 [0])
 #        ] 0) mb-testsuite/check_df.c:61 73 {blockage}
 #     (nil))
 #(jump_insn 80 78 79 (parallel [
 #            (use (reg:SI 15 r15))
 #            (return)
 #        ]) mb-testsuite/check_df.c:61 72 {return_internal}
 #     (expr_list:REG_BR_PRED (const_int 12 [0xc])
 #        (expr_list:REG_DEAD (reg:SI 15 r15)
 #            (nil)))
 # -> return)
	rtsd	r15,8 	 # 80	return_internal	[length = 4]
	
 #(insn 79 80 100 (set (reg/f:SI 1 r1)
 #        (plus:SI (reg/f:SI 1 r1)
 #            (const_int 28 [0x1c]))) 7 {addsi3}
 #     (nil))
	addik	r1,r1,28	 # 79	addsi3/2	[length = 4]
$L26:
 #(insn 19 18 20 (set (reg:SI 5 r5)
 #        (symbol_ref/f:SI ("*$LC1") [flags 0x2] <var_decl 0x7f1792e6f5a0 *$LC1>)) mb-testsuite/check_df.c:54 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,$LC1	 # 19	*movsi_internal2/3	[length = 8]
 #(insn 20 19 89 (set (reg:SI 6 r6)
 #        (symbol_ref/f:SI ("*$LC2") [flags 0x2] <var_decl 0x7f1792e6f640 *$LC2>)) mb-testsuite/check_df.c:54 40 {*movsi_internal2}
 #     (nil))
	addik	r6,r0,$LC2	 # 20	*movsi_internal2/3	[length = 8]
 #(call_insn 22 20 21 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1793f35c00 printf>) [0 __builtin_printf S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:54 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 7 r7)
 #        (expr_list:REG_DEAD (reg:SI 6 r6)
 #            (expr_list:REG_DEAD (reg:SI 5 r5)
 #                (expr_list:REG_UNUSED (reg:SI 3 r3)
 #                    (nil)))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:SI 6 r6))
 #            (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #                (nil)))))
	brlid	r15,printf	 # 22	call_value_intern	[length = 4]
	
 #(insn 21 22 91 (set (reg:SI 7 r7)
 #        (const_int 54 [0x36])) 38 {*movsi_internal3}
 #     (nil))
	addik	r7,r0,54	# 0x36	 # 21	*movsi_internal3/2	[length = 4]
 #(call_insn 24 89 23 (parallel [
 #            (call (mem:SI (symbol_ref:SI ("exit") [flags 0x41] <function_decl 0x7f1793f4ab00 exit>) [0 __builtin_exit S4 A32])
 #                (const_int 24 [0x18]))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:54 75 {call_internal1}
 #     (expr_list:REG_DEAD (reg:SI 5 r5)
 #        (expr_list:REG_NORETURN (const_int 0 [0])
 #            (expr_list:REG_EH_REGION (const_int 0 [0])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #        (nil)))
	brlid	r15,exit	 # 24	call_internal1	[length = 4]
	
 #(insn 23 24 92 (set (reg:SI 5 r5)
 #        (const_int 1 [0x1])) 38 {*movsi_internal3}
 #     (nil))
	addik	r5,r0,1	# 0x1	 # 23	*movsi_internal3/2	[length = 4]
$L27:
 #(insn 44 43 45 (set (reg:SI 5 r5)
 #        (symbol_ref/f:SI ("*$LC1") [flags 0x2] <var_decl 0x7f1792e6f5a0 *$LC1>)) mb-testsuite/check_df.c:59 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,$LC1	 # 44	*movsi_internal2/3	[length = 8]
 #(insn 45 44 94 (set (reg:SI 6 r6)
 #        (symbol_ref/f:SI ("*$LC2") [flags 0x2] <var_decl 0x7f1792e6f640 *$LC2>)) mb-testsuite/check_df.c:59 40 {*movsi_internal2}
 #     (nil))
	addik	r6,r0,$LC2	 # 45	*movsi_internal2/3	[length = 8]
 #(call_insn 47 45 46 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1793f35c00 printf>) [0 __builtin_printf S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:59 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 7 r7)
 #        (expr_list:REG_DEAD (reg:SI 6 r6)
 #            (expr_list:REG_DEAD (reg:SI 5 r5)
 #                (expr_list:REG_UNUSED (reg:SI 3 r3)
 #                    (nil)))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:SI 6 r6))
 #            (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #                (nil)))))
	brlid	r15,printf	 # 47	call_value_intern	[length = 4]
	
 #(insn 46 47 96 (set (reg:SI 7 r7)
 #        (const_int 59 [0x3b])) 38 {*movsi_internal3}
 #     (nil))
	addik	r7,r0,59	# 0x3b	 # 46	*movsi_internal3/2	[length = 4]
 #(call_insn 49 94 48 (parallel [
 #            (call (mem:SI (symbol_ref:SI ("exit") [flags 0x41] <function_decl 0x7f1793f4ab00 exit>) [0 __builtin_exit S4 A32])
 #                (const_int 24 [0x18]))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:59 75 {call_internal1}
 #     (expr_list:REG_DEAD (reg:SI 5 r5)
 #        (expr_list:REG_NORETURN (const_int 0 [0])
 #            (expr_list:REG_EH_REGION (const_int 0 [0])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #        (nil)))
	brlid	r15,exit	 # 49	call_internal1	[length = 4]
	
 #(insn 48 49 97 (set (reg:SI 5 r5)
 #        (const_int 1 [0x1])) 38 {*movsi_internal3}
 #     (nil))
	addik	r5,r0,1	# 0x1	 # 48	*movsi_internal3/2	[length = 4]
	.end	check_df_reg_to_reg
$Lfe6:
	.size	check_df_reg_to_reg,$Lfe6-check_df_reg_to_reg
	.align	2
	.globl	_check_df_reg_plus_const_to_reg
	.ent	_check_df_reg_plus_const_to_reg
	.type	_check_df_reg_plus_const_to_reg, @function
_check_df_reg_plus_const_to_reg:
	.frame	r1,28,r15		# vars= 0, regs= 0, args= 24
	.mask	0x00008000
 #(insn 7 3 22 (set (reg:DF 7 r7)
 #        (const_double:DF 4.294967295e+9 [0x0.ffffffffp+32])) mb-testsuite/check_df.c:66 44 {*movdf_internal}
 #     (nil))
	addik	r7,r0,0xffe00000 	 # 7	*movdf_internal/3	[length = 8]
	addik	r8,r0,0x41efffff #Xfer Lo
 #(insn/f 22 7 23 (set (reg/f:SI 1 r1)
 #        (minus:SI (reg/f:SI 1 r1)
 #            (const_int 28 [0x1c]))) mb-testsuite/check_df.c:65 9 {subsi3}
 #     (nil))
	addik	r1,r1,-28	 # 22	subsi3/2	[length = 8]
 #(insn/f 23 22 24 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])
 #        (reg:SI 15 r15)) mb-testsuite/check_df.c:65 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 15 r15)
 #        (nil)))
	swi	r15,r1,0	 # 23	*movsi_internal2/6	[length = 4]
 #(call_insn/u 8 24 31 (parallel [
 #            (set (reg:DF 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("__adddf3") [flags 0x41]) [0 S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:66 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:DF 7 r7)
 #        (expr_list:REG_DEAD (reg:DF 5 r5)
 #            (expr_list:REG_EH_REGION (const_int -2147483648 [0xffffffff80000000])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:DF 5 r5))
 #            (nil))))
	brlid	r15,__adddf3	 # 8	call_value_intern	[length = 4]
	nop		# Unfilled delay slot

 #(insn 26 31 17 (set (reg:SI 15 r15)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])) mb-testsuite/check_df.c:67 40 {*movsi_internal2}
 #     (nil))
	lwi	r15,r1,0	 # 26	*movsi_internal2/4	[length = 4]
 #(insn 27 17 33 (unspec_volatile [
 #            (const_int 0 [0])
 #        ] 0) mb-testsuite/check_df.c:67 73 {blockage}
 #     (nil))
 #(jump_insn 29 27 28 (parallel [
 #            (use (reg:SI 15 r15))
 #            (return)
 #        ]) mb-testsuite/check_df.c:67 72 {return_internal}
 #     (expr_list:REG_BR_PRED (const_int 12 [0xc])
 #        (expr_list:REG_DEAD (reg:SI 15 r15)
 #            (nil)))
 # -> return)
	rtsd	r15,8 	 # 29	return_internal	[length = 4]
	
 #(insn 28 29 34 (set (reg/f:SI 1 r1)
 #        (plus:SI (reg/f:SI 1 r1)
 #            (const_int 28 [0x1c]))) 7 {addsi3}
 #     (nil))
	addik	r1,r1,28	 # 28	addsi3/2	[length = 4]
	.end	_check_df_reg_plus_const_to_reg
$Lfe7:
	.size	_check_df_reg_plus_const_to_reg,$Lfe7-_check_df_reg_plus_const_to_reg
	.align	2
	.globl	check_df_reg_plus_const_to_reg
	.ent	check_df_reg_plus_const_to_reg
	.type	check_df_reg_plus_const_to_reg, @function
check_df_reg_plus_const_to_reg:
	.frame	r1,36,r15		# vars= 0, regs= 2, args= 24
	.mask	0x00c08000
 #(insn 5 13 45 (set (reg:SI 5 r5)
 #        (symbol_ref:SI ("__func__.1279") [flags 0x2] <var_decl 0x7f1793eb3960 __func__>)) mb-testsuite/check_df.c:71 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,__func__.1279	 # 5	*movsi_internal2/3	[length = 8]
 #(insn/f 45 5 46 (set (reg/f:SI 1 r1)
 #        (minus:SI (reg/f:SI 1 r1)
 #            (const_int 36 [0x24]))) mb-testsuite/check_df.c:70 9 {subsi3}
 #     (nil))
	addik	r1,r1,-36	 # 45	subsi3/2	[length = 8]
 #(insn/f 46 45 47 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])
 #        (reg:SI 15 r15)) mb-testsuite/check_df.c:70 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 15 r15)
 #        (nil)))
	swi	r15,r1,0	 # 46	*movsi_internal2/6	[length = 4]
 #(insn/f 47 46 49 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 28 [0x1c])) [0 S4 A32])
 #        (reg:SI 22 r22)) mb-testsuite/check_df.c:70 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 22 r22)
 #        (nil)))
	swi	r22,r1,28	 # 47	*movsi_internal2/6	[length = 4]
 #(call_insn 6 49 48 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("puts") [flags 0x41] <function_decl 0x7f1793f37300 __builtin_puts>) [0 __builtin_puts S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:71 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 5 r5)
 #        (expr_list:REG_UNUSED (reg:SI 3 r3)
 #            (nil)))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #        (nil)))
	brlid	r15,puts	 # 6	call_value_intern	[length = 4]
	
 #(insn/f 48 6 7 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 32 [0x20])) [0 S4 A32])
 #        (reg:SI 23 r23)) 40 {*movsi_internal2}
 #     (nil))
	swi	r23,r1,32	 # 48	*movsi_internal2/6	[length = 4]
 #(insn 7 60 8 (set (reg:DF 5 r5)
 #        (const_double:DF 1.152921504606846976e+18 [0x0.8p+61])) mb-testsuite/check_df.c:72 44 {*movdf_internal}
 #     (nil))
	addik	r5,r0,0x00000000 	 # 7	*movdf_internal/3	[length = 8]
	addik	r6,r0,0x43b00000 #Xfer Lo
 #(call_insn/u 8 7 43 (parallel [
 #            (set (reg:DF 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("_check_df_reg_plus_const_to_reg") [flags 0x3] <function_decl 0x7f1793f9e100 _check_df_reg_plus_const_to_reg>) [0 _check_df_reg_plus_const_to_reg S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:72 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:DF 5 r5)
 #        (expr_list:REG_EH_REGION (const_int 0 [0])
 #            (nil)))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 5 r5))
 #        (nil)))
	brlid	r15,_check_df_reg_plus_const_to_reg	 # 8	call_value_intern	[length = 4]
	nop		# Unfilled delay slot

 #(insn 43 8 44 (set (reg:SI 5 r5)
 #        (reg:SI 3 r3 [orig:42 D.2120 ] [42])) mb-testsuite/check_df.c:72 40 {*movsi_internal2}
 #     (nil))
	addk	r5,r3,r0	 # 43	*movsi_internal2/1	[length = 4]
 #(insn 44 43 11 (set (reg:SI 6 r6 [+4 ])
 #        (reg:SI 4 r4 [orig:23 D.2120+4 ] [23])) mb-testsuite/check_df.c:72 40 {*movsi_internal2}
 #     (nil))
	addk	r6,r4,r0	 # 44	*movsi_internal2/1	[length = 4]
 #(insn 11 44 41 (set (reg:DF 7 r7)
 #        (const_double:DF 1.152921508901814272e+18 [0x0.80000008p+61])) mb-testsuite/check_df.c:72 44 {*movdf_internal}
 #     (nil))
	addik	r7,r0,0x01000000 	 # 11	*movdf_internal/3	[length = 8]
	addik	r8,r0,0x43b00000 #Xfer Lo
 #(insn 41 11 62 (set (reg:SI 22 r22 [orig:42 D.2120 ] [42])
 #        (reg:SI 3 r3)) mb-testsuite/check_df.c:72 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 3 r3)
 #        (nil)))
	addk	r22,r3,r0	 # 41	*movsi_internal2/1	[length = 4]
 #(call_insn/u 12 41 42 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("__nedf2") [flags 0x41]) [0 S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:72 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:DF 7 r7)
 #        (expr_list:REG_DEAD (reg:DF 5 r5)
 #            (expr_list:REG_EH_REGION (const_int -2147483648 [0xffffffff80000000])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:DF 5 r5))
 #            (nil))))
	brlid	r15,__nedf2	 # 12	call_value_intern	[length = 4]
	
 #(insn 42 12 74 (set (reg:SI 23 r23 [ D.2120+4 ])
 #        (reg:SI 4 r4 [+4 ])) 40 {*movsi_internal2}
 #     (nil))
	addk	r23,r4,r0	 # 42	*movsi_internal2/1	[length = 4]
 #(jump_insn 14 62 51 (set (pc)
 #        (if_then_else (ne:SI (reg:SI 3 r3)
 #                (const_int 0 [0]))
 #            (label_ref:SI 58)
 #            (pc))) mb-testsuite/check_df.c:72 66 {branch_zero}
 #     (expr_list:REG_BR_PRED (const_int 49 [0x31])
 #        (expr_list:REG_DEAD (reg:SI 3 r3)
 #            (expr_list:REG_BR_PROB (const_int 4 [0x4])
 #                (nil))))
 # -> 58)
	bneid	r3,$L34	 # 14	branch_zero	[length = 4]
 #(insn 51 14 32 (set (reg:SI 15 r15)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])) 40 {*movsi_internal2}
 #     (nil))
	lwi	r15,r1,0	 # 51	*movsi_internal2/4	[length = 4]
 #(insn 52 50 53 (set (reg:SI 22 r22)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 28 [0x1c])) [0 S4 A32])) mb-testsuite/check_df.c:76 40 {*movsi_internal2}
 #     (nil))
	lwi	r22,r1,28	 # 52	*movsi_internal2/4	[length = 4]
 #(insn 53 52 54 (set (reg:SI 23 r23)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 32 [0x20])) [0 S4 A32])) mb-testsuite/check_df.c:76 40 {*movsi_internal2}
 #     (nil))
	lwi	r23,r1,32	 # 53	*movsi_internal2/4	[length = 4]
 #(insn 54 53 71 (unspec_volatile [
 #            (const_int 0 [0])
 #        ] 0) mb-testsuite/check_df.c:76 73 {blockage}
 #     (nil))
 #(jump_insn 56 54 55 (parallel [
 #            (use (reg:SI 15 r15))
 #            (return)
 #        ]) mb-testsuite/check_df.c:76 72 {return_internal}
 #     (expr_list:REG_BR_PRED (const_int 12 [0xc])
 #        (expr_list:REG_DEAD (reg:SI 15 r15)
 #            (nil)))
 # -> return)
	rtsd	r15,8 	 # 56	return_internal	[length = 4]
	
 #(insn 55 56 72 (set (reg/f:SI 1 r1)
 #        (plus:SI (reg/f:SI 1 r1)
 #            (const_int 36 [0x24]))) 7 {addsi3}
 #     (nil))
	addik	r1,r1,36	 # 55	addsi3/2	[length = 4]
$L34:
 #(insn 19 18 39 (set (reg:SI 5 r5)
 #        (symbol_ref/f:SI ("*$LC0") [flags 0x2] <var_decl 0x7f1792e6f500 *$LC0>)) mb-testsuite/check_df.c:73 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,$LC0	 # 19	*movsi_internal2/3	[length = 8]
 #(insn 39 19 64 (set (reg:SI 6 r6)
 #        (reg:SI 22 r22 [orig:42 D.2120 ] [42])) mb-testsuite/check_df.c:73 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 22 r22 [orig:42 D.2120 ] [42])
 #        (nil)))
	addk	r6,r22,r0	 # 39	*movsi_internal2/1	[length = 4]
 #(call_insn 21 39 40 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1793f35c00 printf>) [0 __builtin_printf S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:73 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 5 r5)
 #        (expr_list:REG_DEAD (reg:DF 6 r6)
 #            (expr_list:REG_UNUSED (reg:SI 3 r3)
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 6 r6))
 #        (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #            (nil))))
	brlid	r15,printf	 # 21	call_value_intern	[length = 4]
	
 #(insn 40 21 22 (set (reg:SI 7 r7 [+4 ])
 #        (reg:SI 23 r23 [ D.2120+4 ])) 40 {*movsi_internal2}
 #     (nil))
	addk	r7,r23,r0	 # 40	*movsi_internal2/1	[length = 4]
 #(insn 22 64 23 (set (reg:SI 5 r5)
 #        (symbol_ref/f:SI ("*$LC1") [flags 0x2] <var_decl 0x7f1792e6f5a0 *$LC1>)) mb-testsuite/check_df.c:74 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,$LC1	 # 22	*movsi_internal2/3	[length = 8]
 #(insn 23 22 66 (set (reg:SI 6 r6)
 #        (symbol_ref/f:SI ("*$LC2") [flags 0x2] <var_decl 0x7f1792e6f640 *$LC2>)) mb-testsuite/check_df.c:74 40 {*movsi_internal2}
 #     (nil))
	addik	r6,r0,$LC2	 # 23	*movsi_internal2/3	[length = 8]
 #(call_insn 25 23 24 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1793f35c00 printf>) [0 __builtin_printf S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:74 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 7 r7)
 #        (expr_list:REG_DEAD (reg:SI 6 r6)
 #            (expr_list:REG_DEAD (reg:SI 5 r5)
 #                (expr_list:REG_UNUSED (reg:SI 3 r3)
 #                    (nil)))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:SI 6 r6))
 #            (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #                (nil)))))
	brlid	r15,printf	 # 25	call_value_intern	[length = 4]
	
 #(insn 24 25 68 (set (reg:SI 7 r7)
 #        (const_int 74 [0x4a])) 38 {*movsi_internal3}
 #     (nil))
	addik	r7,r0,74	# 0x4a	 # 24	*movsi_internal3/2	[length = 4]
 #(call_insn 27 66 26 (parallel [
 #            (call (mem:SI (symbol_ref:SI ("exit") [flags 0x41] <function_decl 0x7f1793f4ab00 exit>) [0 __builtin_exit S4 A32])
 #                (const_int 24 [0x18]))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:74 75 {call_internal1}
 #     (expr_list:REG_DEAD (reg:SI 5 r5)
 #        (expr_list:REG_NORETURN (const_int 0 [0])
 #            (expr_list:REG_EH_REGION (const_int 0 [0])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #        (nil)))
	brlid	r15,exit	 # 27	call_internal1	[length = 4]
	
 #(insn 26 27 69 (set (reg:SI 5 r5)
 #        (const_int 1 [0x1])) 38 {*movsi_internal3}
 #     (nil))
	addik	r5,r0,1	# 0x1	 # 26	*movsi_internal3/2	[length = 4]
	.end	check_df_reg_plus_const_to_reg
$Lfe8:
	.size	check_df_reg_plus_const_to_reg,$Lfe8-check_df_reg_plus_const_to_reg
	.align	2
	.globl	_check_df_reg_to_mem
	.ent	_check_df_reg_to_mem
	.type	_check_df_reg_to_mem, @function
_check_df_reg_to_mem:
	.frame	r1,0,r15		# vars= 0, regs= 0, args= 0
	.mask	0x00000000
 #(insn 12 3 13 (set (reg:SI 4 r4 [orig:42 a ] [42])
 #        (reg:SI 5 r5 [ a ])) mb-testsuite/check_df.c:80 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 5 r5 [ a ])
 #        (nil)))
	addk	r4,r5,r0	 # 12	*movsi_internal2/1	[length = 4]
 #(insn 13 12 6 (set (reg:SI 5 r5 [ a+4 ])
 #        (reg:SI 6 r6 [ a+4 ])) mb-testsuite/check_df.c:80 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 6 r6 [ a+4 ])
 #        (nil)))
	addk	r5,r6,r0	 # 13	*movsi_internal2/1	[length = 4]
 #(insn 6 13 18 (set (mem/c/i:DF (symbol_ref:SI ("g_x") <var_decl 0x7f1793eb3140 g_x>) [2 g_x+0 S8 A32])
 #        (reg/v:DF 4 r4 [orig:42 a ] [42])) mb-testsuite/check_df.c:81 44 {*movdf_internal}
 #     (expr_list:REG_DEAD (reg/v:DF 4 r4 [orig:42 a ] [42])
 #        (nil)))
	swi	r4,r0,g_x	 # 6	*movdf_internal/5	[length = 8]
	swi	r5,r0,g_x+4
 #(jump_insn 16 18 17 (parallel [
 #            (use (reg:SI 15 r15))
 #            (return)
 #        ]) mb-testsuite/check_df.c:82 72 {return_internal}
 #     (expr_list:REG_BR_PRED (const_int 12 [0xc])
 #        (expr_list:REG_DEAD (reg:SI 15 r15)
 #            (nil)))
 # -> return)
	rtsd	r15,8 	 # 16	return_internal	[length = 4]
	nop		# Unfilled delay slot

	.end	_check_df_reg_to_mem
$Lfe9:
	.size	_check_df_reg_to_mem,$Lfe9-_check_df_reg_to_mem
	.align	2
	.globl	check_df_reg_to_mem
	.ent	check_df_reg_to_mem
	.type	check_df_reg_to_mem, @function
check_df_reg_to_mem:
	.frame	r1,28,r15		# vars= 0, regs= 0, args= 24
	.mask	0x00008000
 #(insn 5 12 35 (set (reg:SI 5 r5)
 #        (symbol_ref:SI ("__func__.1288") [flags 0x2] <var_decl 0x7f1793eb3b40 __func__>)) mb-testsuite/check_df.c:86 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,__func__.1288	 # 5	*movsi_internal2/3	[length = 8]
 #(insn/f 35 5 36 (set (reg/f:SI 1 r1)
 #        (minus:SI (reg/f:SI 1 r1)
 #            (const_int 28 [0x1c]))) mb-testsuite/check_df.c:85 9 {subsi3}
 #     (nil))
	addik	r1,r1,-28	 # 35	subsi3/2	[length = 8]
 #(insn/f 36 35 37 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])
 #        (reg:SI 15 r15)) mb-testsuite/check_df.c:85 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 15 r15)
 #        (nil)))
	swi	r15,r1,0	 # 36	*movsi_internal2/6	[length = 4]
 #(call_insn 6 37 7 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("puts") [flags 0x41] <function_decl 0x7f1793f37300 __builtin_puts>) [0 __builtin_puts S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:86 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 5 r5)
 #        (expr_list:REG_UNUSED (reg:SI 3 r3)
 #            (nil)))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #        (nil)))
	brlid	r15,puts	 # 6	call_value_intern	[length = 4]
	nop		# Unfilled delay slot

 #(insn 7 6 8 (set (reg:DF 5 r5)
 #        (const_double:DF 1.60456909850882048e+19 [0x0.deadbeefeddecp+64])) mb-testsuite/check_df.c:87 44 {*movdf_internal}
 #     (nil))
	addik	r5,r0,0xddfdbbd8 	 # 7	*movdf_internal/3	[length = 8]
	addik	r6,r0,0x43ebd5b7 #Xfer Lo
 #(call_insn 8 7 9 (parallel [
 #            (call (mem:SI (symbol_ref:SI ("_check_df_reg_to_mem") [flags 0x3] <function_decl 0x7f1793f9e400 _check_df_reg_to_mem>) [0 _check_df_reg_to_mem S4 A32])
 #                (const_int 24 [0x18]))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:87 75 {call_internal1}
 #     (expr_list:REG_DEAD (reg:DF 5 r5)
 #        (expr_list:REG_EH_REGION (const_int 0 [0])
 #            (nil)))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 5 r5))
 #        (nil)))
	brlid	r15,_check_df_reg_to_mem	 # 8	call_internal1	[length = 4]
	nop		# Unfilled delay slot

 #(insn 9 8 10 (set (reg:DF 5 r5)
 #        (mem/c/i:DF (symbol_ref:SI ("g_x") <var_decl 0x7f1793eb3140 g_x>) [2 g_x+0 S8 A32])) mb-testsuite/check_df.c:88 44 {*movdf_internal}
 #     (nil))
	lwi	r5,r0,g_x	 # 9	*movdf_internal/2	[length = 8]
	lwi	r6,r0,g_x+4
 #(insn 10 9 11 (set (reg:DF 7 r7)
 #        (const_double:DF 1.60456909850882048e+19 [0x0.deadbeefeddecp+64])) mb-testsuite/check_df.c:88 44 {*movdf_internal}
 #     (nil))
	addik	r7,r0,0xddfdbbd8 	 # 10	*movdf_internal/3	[length = 8]
	addik	r8,r0,0x43ebd5b7 #Xfer Lo
 #(call_insn/u 11 10 54 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("__nedf2") [flags 0x41]) [0 S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:88 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:DF 7 r7)
 #        (expr_list:REG_DEAD (reg:DF 5 r5)
 #            (expr_list:REG_EH_REGION (const_int -2147483648 [0xffffffff80000000])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:DF 5 r5))
 #            (nil))))
	brlid	r15,__nedf2	 # 11	call_value_intern	[length = 4]
	nop		# Unfilled delay slot

 #(jump_insn 13 11 39 (set (pc)
 #        (if_then_else (ne:SI (reg:SI 3 r3)
 #                (const_int 0 [0]))
 #            (label_ref:SI 44)
 #            (pc))) mb-testsuite/check_df.c:88 66 {branch_zero}
 #     (expr_list:REG_BR_PRED (const_int 49 [0x31])
 #        (expr_list:REG_DEAD (reg:SI 3 r3)
 #            (expr_list:REG_BR_PROB (const_int 4 [0x4])
 #                (nil))))
 # -> 44)
	bneid	r3,$L41	 # 13	branch_zero	[length = 4]
 #(insn 39 13 28 (set (reg:SI 15 r15)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])) 40 {*movsi_internal2}
 #     (nil))
	lwi	r15,r1,0	 # 39	*movsi_internal2/4	[length = 4]
 #(insn 40 38 51 (unspec_volatile [
 #            (const_int 0 [0])
 #        ] 0) mb-testsuite/check_df.c:90 73 {blockage}
 #     (nil))
 #(jump_insn 42 40 41 (parallel [
 #            (use (reg:SI 15 r15))
 #            (return)
 #        ]) mb-testsuite/check_df.c:90 72 {return_internal}
 #     (expr_list:REG_BR_PRED (const_int 12 [0xc])
 #        (expr_list:REG_DEAD (reg:SI 15 r15)
 #            (nil)))
 # -> return)
	rtsd	r15,8 	 # 42	return_internal	[length = 4]
	
 #(insn 41 42 52 (set (reg/f:SI 1 r1)
 #        (plus:SI (reg/f:SI 1 r1)
 #            (const_int 28 [0x1c]))) 7 {addsi3}
 #     (nil))
	addik	r1,r1,28	 # 41	addsi3/2	[length = 4]
$L41:
 #(insn 18 17 19 (set (reg:SI 5 r5)
 #        (symbol_ref/f:SI ("*$LC1") [flags 0x2] <var_decl 0x7f1792e6f5a0 *$LC1>)) mb-testsuite/check_df.c:89 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,$LC1	 # 18	*movsi_internal2/3	[length = 8]
 #(insn 19 18 46 (set (reg:SI 6 r6)
 #        (symbol_ref/f:SI ("*$LC2") [flags 0x2] <var_decl 0x7f1792e6f640 *$LC2>)) mb-testsuite/check_df.c:89 40 {*movsi_internal2}
 #     (nil))
	addik	r6,r0,$LC2	 # 19	*movsi_internal2/3	[length = 8]
 #(call_insn 21 19 20 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1793f35c00 printf>) [0 __builtin_printf S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:89 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 7 r7)
 #        (expr_list:REG_DEAD (reg:SI 6 r6)
 #            (expr_list:REG_DEAD (reg:SI 5 r5)
 #                (expr_list:REG_UNUSED (reg:SI 3 r3)
 #                    (nil)))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:SI 6 r6))
 #            (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #                (nil)))))
	brlid	r15,printf	 # 21	call_value_intern	[length = 4]
	
 #(insn 20 21 48 (set (reg:SI 7 r7)
 #        (const_int 89 [0x59])) 38 {*movsi_internal3}
 #     (nil))
	addik	r7,r0,89	# 0x59	 # 20	*movsi_internal3/2	[length = 4]
 #(call_insn 23 46 22 (parallel [
 #            (call (mem:SI (symbol_ref:SI ("exit") [flags 0x41] <function_decl 0x7f1793f4ab00 exit>) [0 __builtin_exit S4 A32])
 #                (const_int 24 [0x18]))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:89 75 {call_internal1}
 #     (expr_list:REG_DEAD (reg:SI 5 r5)
 #        (expr_list:REG_NORETURN (const_int 0 [0])
 #            (expr_list:REG_EH_REGION (const_int 0 [0])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #        (nil)))
	brlid	r15,exit	 # 23	call_internal1	[length = 4]
	
 #(insn 22 23 49 (set (reg:SI 5 r5)
 #        (const_int 1 [0x1])) 38 {*movsi_internal3}
 #     (nil))
	addik	r5,r0,1	# 0x1	 # 22	*movsi_internal3/2	[length = 4]
	.end	check_df_reg_to_mem
$Lfe10:
	.size	check_df_reg_to_mem,$Lfe10-check_df_reg_to_mem
	.align	2
	.globl	_check_df_mem_plus_mem_to_mem
	.ent	_check_df_mem_plus_mem_to_mem
	.type	_check_df_mem_plus_mem_to_mem, @function
_check_df_mem_plus_mem_to_mem:
	.frame	r1,32,r15		# vars= 0, regs= 1, args= 24
	.mask	0x00088000
 #(insn/f 20 5 21 (set (reg/f:SI 1 r1)
 #        (minus:SI (reg/f:SI 1 r1)
 #            (const_int 32 [0x20]))) mb-testsuite/check_df.c:94 9 {subsi3}
 #     (nil))
	addik	r1,r1,-32	 # 20	subsi3/2	[length = 8]
 #(insn/f 21 20 22 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])
 #        (reg:SI 15 r15)) mb-testsuite/check_df.c:94 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 15 r15)
 #        (nil)))
	swi	r15,r1,0	 # 21	*movsi_internal2/6	[length = 4]
 #(insn/f 22 21 23 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 28 [0x1c])) [0 S4 A32])
 #        (reg:SI 19 r19)) mb-testsuite/check_df.c:94 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 19 r19)
 #        (nil)))
	swi	r19,r1,28	 # 22	*movsi_internal2/6	[length = 4]
 #(insn 9 23 2 (set (reg:DF 7 r7)
 #        (mem:DF (reg/v/f:SI 7 r7 [orig:47 b ] [47]) [2 *b_3(D)+0 S8 A32])) mb-testsuite/check_df.c:95 44 {*movdf_internal}
 #     (nil))
	lwi	r8,r7,4	 # 9	*movdf_internal/2	[length = 8]
	lwi	r7,r7,0
 #(insn 2 9 8 (set (reg/v/f:SI 19 r19 [orig:45 d ] [45])
 #        (reg:SI 5 r5 [ d ])) mb-testsuite/check_df.c:94 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 5 r5 [ d ])
 #        (expr_list:REG_EQUIV (mem/f/c/i:SI (plus:SI (reg/f:SI 33 $ap)
 #                    (const_int 4 [0x4])) [4 d+0 S4 A32])
 #            (nil))))
	addk	r19,r5,r0	 # 2	*movsi_internal2/1	[length = 4]
 #(insn 8 2 10 (set (reg:DF 5 r5)
 #        (mem:DF (reg/v/f:SI 6 r6 [orig:46 a ] [46]) [2 *a_1(D)+0 S8 A32])) mb-testsuite/check_df.c:95 44 {*movdf_internal}
 #     (nil))
	lwi	r5,r6,0	 # 8	*movdf_internal/2	[length = 8]
	lwi	r6,r6,4
 #(call_insn/u 10 8 18 (parallel [
 #            (set (reg:DF 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("__adddf3") [flags 0x41]) [0 S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:95 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:DF 7 r7)
 #        (expr_list:REG_DEAD (reg:DF 5 r5)
 #            (expr_list:REG_EH_REGION (const_int -2147483648 [0xffffffff80000000])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:DF 5 r5))
 #            (nil))))
	brlid	r15,__adddf3	 # 10	call_value_intern	[length = 4]
	nop		# Unfilled delay slot

 #(insn 18 10 19 (set (reg:SI 5 r5 [+4 ])
 #        (reg:SI 4 r4 [+4 ])) mb-testsuite/check_df.c:95 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 4 r4 [+4 ])
 #        (nil)))
	addk	r5,r4,r0	 # 18	*movsi_internal2/1	[length = 4]
 #(insn 19 18 12 (set (reg:SI 4 r4 [48])
 #        (reg:SI 3 r3)) mb-testsuite/check_df.c:95 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 3 r3)
 #        (nil)))
	addk	r4,r3,r0	 # 19	*movsi_internal2/1	[length = 4]
 #(insn 12 19 31 (set (mem:DF (reg/v/f:SI 19 r19 [orig:45 d ] [45]) [2 *d_6(D)+0 S8 A32])
 #        (reg:DF 4 r4 [48])) mb-testsuite/check_df.c:95 44 {*movdf_internal}
 #     (expr_list:REG_DEAD (reg/v/f:SI 19 r19 [orig:45 d ] [45])
 #        (expr_list:REG_DEAD (reg:DF 4 r4 [48])
 #            (nil))))
	swi	r4,r19,0	 # 12	*movdf_internal/5	[length = 8]
	swi	r5,r19,4
 #(insn 25 31 26 (set (reg:SI 15 r15)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])) mb-testsuite/check_df.c:96 40 {*movsi_internal2}
 #     (nil))
	lwi	r15,r1,0	 # 25	*movsi_internal2/4	[length = 4]
 #(insn 26 25 27 (set (reg:SI 19 r19)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 28 [0x1c])) [0 S4 A32])) mb-testsuite/check_df.c:96 40 {*movsi_internal2}
 #     (nil))
	lwi	r19,r1,28	 # 26	*movsi_internal2/4	[length = 4]
 #(insn 27 26 33 (unspec_volatile [
 #            (const_int 0 [0])
 #        ] 0) mb-testsuite/check_df.c:96 73 {blockage}
 #     (nil))
 #(jump_insn 29 27 28 (parallel [
 #            (use (reg:SI 15 r15))
 #            (return)
 #        ]) mb-testsuite/check_df.c:96 72 {return_internal}
 #     (expr_list:REG_BR_PRED (const_int 12 [0xc])
 #        (expr_list:REG_DEAD (reg:SI 15 r15)
 #            (nil)))
 # -> return)
	rtsd	r15,8 	 # 29	return_internal	[length = 4]
	
 #(insn 28 29 34 (set (reg/f:SI 1 r1)
 #        (plus:SI (reg/f:SI 1 r1)
 #            (const_int 32 [0x20]))) 7 {addsi3}
 #     (nil))
	addik	r1,r1,32	 # 28	addsi3/2	[length = 4]
	.end	_check_df_mem_plus_mem_to_mem
$Lfe11:
	.size	_check_df_mem_plus_mem_to_mem,$Lfe11-_check_df_mem_plus_mem_to_mem
	.align	2
	.globl	check_df_mem_to_mem
	.ent	check_df_mem_to_mem
	.type	check_df_mem_to_mem, @function
check_df_mem_to_mem:
	.frame	r1,44,r15		# vars= 16, regs= 0, args= 24
	.mask	0x00008000
 #(insn 45 19 47 (set (reg:DF 6 r6)
 #        (const_double:DF 1.7140398773548019712e+19 [0x0.eddeeddeeddefp+64])) mb-testsuite/check_df.c:100 44 {*movdf_internal}
 #     (nil))
	addik	r6,r0,0xbbddbbde 	 # 45	*movdf_internal/3	[length = 8]
	addik	r7,r0,0x43edbbdd #Xfer Lo
 #(insn/f 47 45 9 (set (reg/f:SI 1 r1)
 #        (minus:SI (reg/f:SI 1 r1)
 #            (const_int 44 [0x2c]))) mb-testsuite/check_df.c:99 9 {subsi3}
 #     (nil))
	addik	r1,r1,-44	 # 47	subsi3/2	[length = 8]
 #(insn 9 47 6 (set (reg:SI 5 r5)
 #        (symbol_ref:SI ("__func__.1303") [flags 0x2] <var_decl 0x7f1793eb3e60 __func__>)) mb-testsuite/check_df.c:101 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,__func__.1303	 # 9	*movsi_internal2/3	[length = 8]
 #(insn 6 9 46 (set (mem/c/i:DF (plus:SI (reg/f:SI 1 r1)
 #                (const_int 36 [0x24])) [2 a+0 S8 A32])
 #        (reg:DF 6 r6)) mb-testsuite/check_df.c:100 44 {*movdf_internal}
 #     (expr_list:REG_DEAD (reg:DF 6 r6)
 #        (nil)))
	swi	r6,r1,36	 # 6	*movdf_internal/5	[length = 8]
	swi	r7,r1,40
 #(insn 46 6 48 (set (reg:DF 6 r6)
 #        (const_double:DF 1.152921504606846976e+18 [0x0.8p+61])) mb-testsuite/check_df.c:100 44 {*movdf_internal}
 #     (nil))
	addik	r6,r0,0x00000000 	 # 46	*movdf_internal/3	[length = 8]
	addik	r7,r0,0x43b00000 #Xfer Lo
 #(insn/f 48 46 49 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])
 #        (reg:SI 15 r15)) mb-testsuite/check_df.c:99 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 15 r15)
 #        (nil)))
	swi	r15,r1,0	 # 48	*movsi_internal2/6	[length = 4]
 #(insn 8 49 10 (set (mem/c/i:DF (plus:SI (reg/f:SI 1 r1)
 #                (const_int 28 [0x1c])) [2 b+0 S8 A32])
 #        (reg:DF 6 r6)) mb-testsuite/check_df.c:100 44 {*movdf_internal}
 #     (expr_list:REG_DEAD (reg:DF 6 r6)
 #        (nil)))
	swi	r6,r1,28	 # 8	*movdf_internal/5	[length = 8]
	swi	r7,r1,32
 #(call_insn 10 8 12 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("puts") [flags 0x41] <function_decl 0x7f1793f37300 __builtin_puts>) [0 __builtin_puts S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:101 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 5 r5)
 #        (expr_list:REG_UNUSED (reg:SI 3 r3)
 #            (nil)))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #        (nil)))
	brlid	r15,puts	 # 10	call_value_intern	[length = 4]
	nop		# Unfilled delay slot

 #(insn 12 10 13 (set (reg:SI 5 r5)
 #        (symbol_ref:SI ("g_x") <var_decl 0x7f1793eb3140 g_x>)) mb-testsuite/check_df.c:102 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,g_x	 # 12	*movsi_internal2/3	[length = 8]
 #(insn 13 12 58 (set (reg:SI 6 r6)
 #        (plus:SI (reg/f:SI 1 r1)
 #            (const_int 36 [0x24]))) mb-testsuite/check_df.c:102 7 {addsi3}
 #     (nil))
	addik	r6,r1,36	 # 13	addsi3/2	[length = 4]
 #(call_insn 15 13 14 (parallel [
 #            (call (mem:SI (symbol_ref:SI ("_check_df_mem_plus_mem_to_mem") [flags 0x3] <function_decl 0x7f1793f9e700 _check_df_mem_plus_mem_to_mem>) [0 _check_df_mem_plus_mem_to_mem S4 A32])
 #                (const_int 24 [0x18]))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:102 75 {call_internal1}
 #     (expr_list:REG_DEAD (reg:SI 7 r7)
 #        (expr_list:REG_DEAD (reg:SI 6 r6)
 #            (expr_list:REG_DEAD (reg:SI 5 r5)
 #                (expr_list:REG_EH_REGION (const_int 0 [0])
 #                    (nil)))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:SI 6 r6))
 #            (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #                (nil)))))
	brlid	r15,_check_df_mem_plus_mem_to_mem	 # 15	call_internal1	[length = 4]
	
 #(insn 14 15 16 (set (reg:SI 7 r7)
 #        (plus:SI (reg/f:SI 1 r1)
 #            (const_int 28 [0x1c]))) 7 {addsi3}
 #     (nil))
	addik	r7,r1,28	 # 14	addsi3/2	[length = 4]
 #(insn 16 58 17 (set (reg:DF 5 r5)
 #        (mem/c/i:DF (symbol_ref:SI ("g_x") <var_decl 0x7f1793eb3140 g_x>) [2 g_x+0 S8 A32])) mb-testsuite/check_df.c:103 44 {*movdf_internal}
 #     (nil))
	lwi	r5,r0,g_x	 # 16	*movdf_internal/2	[length = 8]
	lwi	r6,r0,g_x+4
 #(insn 17 16 18 (set (reg:DF 7 r7)
 #        (const_double:DF 1.8293320278154866688e+19 [0x0.fddeeddeeddefp+64])) mb-testsuite/check_df.c:103 44 {*movdf_internal}
 #     (nil))
	addik	r7,r0,0xbbddbbde 	 # 17	*movdf_internal/3	[length = 8]
	addik	r8,r0,0x43efbbdd #Xfer Lo
 #(call_insn/u 18 17 68 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("__nedf2") [flags 0x41]) [0 S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:103 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:DF 7 r7)
 #        (expr_list:REG_DEAD (reg:DF 5 r5)
 #            (expr_list:REG_EH_REGION (const_int -2147483648 [0xffffffff80000000])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:DF 5 r5))
 #            (nil))))
	brlid	r15,__nedf2	 # 18	call_value_intern	[length = 4]
	nop		# Unfilled delay slot

 #(jump_insn 20 18 51 (set (pc)
 #        (if_then_else (ne:SI (reg:SI 3 r3)
 #                (const_int 0 [0]))
 #            (label_ref:SI 56)
 #            (pc))) mb-testsuite/check_df.c:103 66 {branch_zero}
 #     (expr_list:REG_BR_PRED (const_int 49 [0x31])
 #        (expr_list:REG_DEAD (reg:SI 3 r3)
 #            (expr_list:REG_BR_PROB (const_int 4 [0x4])
 #                (nil))))
 # -> 56)
	bneid	r3,$L48	 # 20	branch_zero	[length = 4]
 #(insn 51 20 35 (set (reg:SI 15 r15)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])) 40 {*movsi_internal2}
 #     (nil))
	lwi	r15,r1,0	 # 51	*movsi_internal2/4	[length = 4]
 #(insn 52 50 65 (unspec_volatile [
 #            (const_int 0 [0])
 #        ] 0) mb-testsuite/check_df.c:105 73 {blockage}
 #     (nil))
 #(jump_insn 54 52 53 (parallel [
 #            (use (reg:SI 15 r15))
 #            (return)
 #        ]) mb-testsuite/check_df.c:105 72 {return_internal}
 #     (expr_list:REG_BR_PRED (const_int 12 [0xc])
 #        (expr_list:REG_DEAD (reg:SI 15 r15)
 #            (nil)))
 # -> return)
	rtsd	r15,8 	 # 54	return_internal	[length = 4]
	
 #(insn 53 54 66 (set (reg/f:SI 1 r1)
 #        (plus:SI (reg/f:SI 1 r1)
 #            (const_int 44 [0x2c]))) 7 {addsi3}
 #     (nil))
	addik	r1,r1,44	 # 53	addsi3/2	[length = 4]
$L48:
 #(insn 25 24 26 (set (reg:SI 5 r5)
 #        (symbol_ref/f:SI ("*$LC1") [flags 0x2] <var_decl 0x7f1792e6f5a0 *$LC1>)) mb-testsuite/check_df.c:104 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,$LC1	 # 25	*movsi_internal2/3	[length = 8]
 #(insn 26 25 60 (set (reg:SI 6 r6)
 #        (symbol_ref/f:SI ("*$LC2") [flags 0x2] <var_decl 0x7f1792e6f640 *$LC2>)) mb-testsuite/check_df.c:104 40 {*movsi_internal2}
 #     (nil))
	addik	r6,r0,$LC2	 # 26	*movsi_internal2/3	[length = 8]
 #(call_insn 28 26 27 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1793f35c00 printf>) [0 __builtin_printf S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:104 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 7 r7)
 #        (expr_list:REG_DEAD (reg:SI 6 r6)
 #            (expr_list:REG_DEAD (reg:SI 5 r5)
 #                (expr_list:REG_UNUSED (reg:SI 3 r3)
 #                    (nil)))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:SI 6 r6))
 #            (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #                (nil)))))
	brlid	r15,printf	 # 28	call_value_intern	[length = 4]
	
 #(insn 27 28 62 (set (reg:SI 7 r7)
 #        (const_int 104 [0x68])) 38 {*movsi_internal3}
 #     (nil))
	addik	r7,r0,104	# 0x68	 # 27	*movsi_internal3/2	[length = 4]
 #(call_insn 30 60 29 (parallel [
 #            (call (mem:SI (symbol_ref:SI ("exit") [flags 0x41] <function_decl 0x7f1793f4ab00 exit>) [0 __builtin_exit S4 A32])
 #                (const_int 24 [0x18]))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:104 75 {call_internal1}
 #     (expr_list:REG_DEAD (reg:SI 5 r5)
 #        (expr_list:REG_NORETURN (const_int 0 [0])
 #            (expr_list:REG_EH_REGION (const_int 0 [0])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #        (nil)))
	brlid	r15,exit	 # 30	call_internal1	[length = 4]
	
 #(insn 29 30 63 (set (reg:SI 5 r5)
 #        (const_int 1 [0x1])) 38 {*movsi_internal3}
 #     (nil))
	addik	r5,r0,1	# 0x1	 # 29	*movsi_internal3/2	[length = 4]
	.end	check_df_mem_to_mem
$Lfe12:
	.size	check_df_mem_to_mem,$Lfe12-check_df_mem_to_mem
	.align	2
	.globl	_check_extend_sf_df
	.ent	_check_extend_sf_df
	.type	_check_extend_sf_df, @function
_check_extend_sf_df:
	.frame	r1,28,r15		# vars= 0, regs= 0, args= 24
	.mask	0x00008000
 #(insn/f 21 3 22 (set (reg/f:SI 1 r1)
 #        (minus:SI (reg/f:SI 1 r1)
 #            (const_int 28 [0x1c]))) mb-testsuite/check_df.c:109 9 {subsi3}
 #     (nil))
	addik	r1,r1,-28	 # 21	subsi3/2	[length = 8]
 #(insn/f 22 21 23 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])
 #        (reg:SI 15 r15)) mb-testsuite/check_df.c:109 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 15 r15)
 #        (nil)))
	swi	r15,r1,0	 # 22	*movsi_internal2/6	[length = 4]
 #(call_insn/u 7 23 30 (parallel [
 #            (set (reg:DF 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("__extendsfdf2") [flags 0x41]) [0 S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:110 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SF 5 r5)
 #        (expr_list:REG_EH_REGION (const_int -2147483648 [0xffffffff80000000])
 #            (nil)))
 #    (expr_list:REG_DEP_TRUE (use (reg:SF 5 r5))
 #        (nil)))
	brlid	r15,__extendsfdf2	 # 7	call_value_intern	[length = 4]
	nop		# Unfilled delay slot

 #(insn 25 30 16 (set (reg:SI 15 r15)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])) mb-testsuite/check_df.c:111 40 {*movsi_internal2}
 #     (nil))
	lwi	r15,r1,0	 # 25	*movsi_internal2/4	[length = 4]
 #(insn 26 16 32 (unspec_volatile [
 #            (const_int 0 [0])
 #        ] 0) mb-testsuite/check_df.c:111 73 {blockage}
 #     (nil))
 #(jump_insn 28 26 27 (parallel [
 #            (use (reg:SI 15 r15))
 #            (return)
 #        ]) mb-testsuite/check_df.c:111 72 {return_internal}
 #     (expr_list:REG_BR_PRED (const_int 12 [0xc])
 #        (expr_list:REG_DEAD (reg:SI 15 r15)
 #            (nil)))
 # -> return)
	rtsd	r15,8 	 # 28	return_internal	[length = 4]
	
 #(insn 27 28 33 (set (reg/f:SI 1 r1)
 #        (plus:SI (reg/f:SI 1 r1)
 #            (const_int 28 [0x1c]))) 7 {addsi3}
 #     (nil))
	addik	r1,r1,28	 # 27	addsi3/2	[length = 4]
	.end	_check_extend_sf_df
$Lfe13:
	.size	_check_extend_sf_df,$Lfe13-_check_extend_sf_df
	.align	2
	.globl	check_df_extend_sf_df
	.ent	check_df_extend_sf_df
	.type	check_df_extend_sf_df, @function
check_df_extend_sf_df:
	.frame	r1,28,r15		# vars= 0, regs= 0, args= 24
	.mask	0x00008000
 #(insn 5 11 38 (set (reg:SF 5 r5)
 #        (const_double:SF -1.0e+0 [-0x0.8p+1])) mb-testsuite/check_df.c:115 43 {*movsf_internal}
 #     (nil))
	addik	r5,r0,0xbf800000	 # 5	*movsf_internal/4	[length = 4]
 #(insn/f 38 5 39 (set (reg/f:SI 1 r1)
 #        (minus:SI (reg/f:SI 1 r1)
 #            (const_int 28 [0x1c]))) mb-testsuite/check_df.c:114 9 {subsi3}
 #     (nil))
	addik	r1,r1,-28	 # 38	subsi3/2	[length = 8]
 #(insn/f 39 38 40 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])
 #        (reg:SI 15 r15)) mb-testsuite/check_df.c:114 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 15 r15)
 #        (nil)))
	swi	r15,r1,0	 # 39	*movsi_internal2/6	[length = 4]
 #(call_insn/u 6 40 34 (parallel [
 #            (set (reg:DF 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("_check_extend_sf_df") [flags 0x3] <function_decl 0x7f1793f9eb00 _check_extend_sf_df>) [0 _check_extend_sf_df S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:115 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SF 5 r5)
 #        (expr_list:REG_EH_REGION (const_int 0 [0])
 #            (nil)))
 #    (expr_list:REG_DEP_TRUE (use (reg:SF 5 r5))
 #        (nil)))
	brlid	r15,_check_extend_sf_df	 # 6	call_value_intern	[length = 4]
	nop		# Unfilled delay slot

 #(insn 34 6 9 (set (reg:SI 5 r5 [ D.2106+4 ])
 #        (reg:SI 4 r4 [+4 ])) mb-testsuite/check_df.c:115 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 4 r4 [+4 ])
 #        (nil)))
	addk	r5,r4,r0	 # 34	*movsi_internal2/1	[length = 4]
 #(insn 9 34 36 (set (reg:DF 7 r7)
 #        (const_double:DF -1.0e+0 [-0x0.8p+1])) mb-testsuite/check_df.c:115 44 {*movdf_internal}
 #     (nil))
	addik	r7,r0,0x00000000 	 # 9	*movdf_internal/3	[length = 8]
	addik	r8,r0,0xffffffffbff00000 #Xfer Lo
 #(insn 36 9 49 (set (reg:SI 6 r6 [+4 ])
 #        (reg:SI 5 r5 [ D.2106+4 ])) mb-testsuite/check_df.c:115 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 5 r5 [ D.2106+4 ])
 #        (nil)))
	addk	r6,r5,r0	 # 36	*movsi_internal2/1	[length = 4]
 #(call_insn/u 10 36 37 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("__nedf2") [flags 0x41]) [0 S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:115 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:DF 7 r7)
 #        (expr_list:REG_DEAD (reg:DF 5 r5)
 #            (expr_list:REG_EH_REGION (const_int -2147483648 [0xffffffff80000000])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:DF 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:DF 5 r5))
 #            (nil))))
	brlid	r15,__nedf2	 # 10	call_value_intern	[length = 4]
	
 #(insn 37 10 59 (set (reg:SI 5 r5)
 #        (reg:SI 3 r3 [orig:42 D.2106 ] [42])) 40 {*movsi_internal2}
 #     (nil))
	addk	r5,r3,r0	 # 37	*movsi_internal2/1	[length = 4]
 #(jump_insn 12 49 42 (set (pc)
 #        (if_then_else (ne:SI (reg:SI 3 r3)
 #                (const_int 0 [0]))
 #            (label_ref:SI 47)
 #            (pc))) mb-testsuite/check_df.c:115 66 {branch_zero}
 #     (expr_list:REG_BR_PRED (const_int 49 [0x31])
 #        (expr_list:REG_DEAD (reg:SI 3 r3)
 #            (expr_list:REG_BR_PROB (const_int 4 [0x4])
 #                (nil))))
 # -> 47)
	bneid	r3,$L55	 # 12	branch_zero	[length = 4]
 #(insn 42 12 27 (set (reg:SI 15 r15)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])) 40 {*movsi_internal2}
 #     (nil))
	lwi	r15,r1,0	 # 42	*movsi_internal2/4	[length = 4]
 #(insn 43 41 56 (unspec_volatile [
 #            (const_int 0 [0])
 #        ] 0) mb-testsuite/check_df.c:117 73 {blockage}
 #     (nil))
 #(jump_insn 45 43 44 (parallel [
 #            (use (reg:SI 15 r15))
 #            (return)
 #        ]) mb-testsuite/check_df.c:117 72 {return_internal}
 #     (expr_list:REG_BR_PRED (const_int 12 [0xc])
 #        (expr_list:REG_DEAD (reg:SI 15 r15)
 #            (nil)))
 # -> return)
	rtsd	r15,8 	 # 45	return_internal	[length = 4]
	
 #(insn 44 45 57 (set (reg/f:SI 1 r1)
 #        (plus:SI (reg/f:SI 1 r1)
 #            (const_int 28 [0x1c]))) 7 {addsi3}
 #     (nil))
	addik	r1,r1,28	 # 44	addsi3/2	[length = 4]
$L55:
 #(insn 17 16 18 (set (reg:SI 5 r5)
 #        (symbol_ref/f:SI ("*$LC1") [flags 0x2] <var_decl 0x7f1792e6f5a0 *$LC1>)) mb-testsuite/check_df.c:116 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,$LC1	 # 17	*movsi_internal2/3	[length = 8]
 #(insn 18 17 51 (set (reg:SI 6 r6)
 #        (symbol_ref/f:SI ("*$LC2") [flags 0x2] <var_decl 0x7f1792e6f640 *$LC2>)) mb-testsuite/check_df.c:116 40 {*movsi_internal2}
 #     (nil))
	addik	r6,r0,$LC2	 # 18	*movsi_internal2/3	[length = 8]
 #(call_insn 20 18 19 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1793f35c00 printf>) [0 __builtin_printf S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:116 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 7 r7)
 #        (expr_list:REG_DEAD (reg:SI 6 r6)
 #            (expr_list:REG_DEAD (reg:SI 5 r5)
 #                (expr_list:REG_UNUSED (reg:SI 3 r3)
 #                    (nil)))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 7 r7))
 #        (expr_list:REG_DEP_TRUE (use (reg:SI 6 r6))
 #            (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #                (nil)))))
	brlid	r15,printf	 # 20	call_value_intern	[length = 4]
	
 #(insn 19 20 53 (set (reg:SI 7 r7)
 #        (const_int 116 [0x74])) 38 {*movsi_internal3}
 #     (nil))
	addik	r7,r0,116	# 0x74	 # 19	*movsi_internal3/2	[length = 4]
 #(call_insn 22 51 21 (parallel [
 #            (call (mem:SI (symbol_ref:SI ("exit") [flags 0x41] <function_decl 0x7f1793f4ab00 exit>) [0 __builtin_exit S4 A32])
 #                (const_int 24 [0x18]))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:116 75 {call_internal1}
 #     (expr_list:REG_DEAD (reg:SI 5 r5)
 #        (expr_list:REG_NORETURN (const_int 0 [0])
 #            (expr_list:REG_EH_REGION (const_int 0 [0])
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #        (nil)))
	brlid	r15,exit	 # 22	call_internal1	[length = 4]
	
 #(insn 21 22 54 (set (reg:SI 5 r5)
 #        (const_int 1 [0x1])) 38 {*movsi_internal3}
 #     (nil))
	addik	r5,r0,1	# 0x1	 # 21	*movsi_internal3/2	[length = 4]
	.end	check_df_extend_sf_df
$Lfe14:
	.size	check_df_extend_sf_df,$Lfe14-check_df_extend_sf_df
	.align	2
	.globl	check_simple_float
	.ent	check_simple_float
	.type	check_simple_float, @function
check_simple_float:
	.frame	r1,0,r15		# vars= 0, regs= 0, args= 0
	.mask	0x00000000
 #(jump_insn 11 10 12 (parallel [
 #            (use (reg:SI 15 r15))
 #            (return)
 #        ]) mb-testsuite/check_df.c:125 72 {return_internal}
 #     (expr_list:REG_BR_PRED (const_int 12 [0xc])
 #        (expr_list:REG_DEAD (reg:SI 15 r15)
 #            (nil)))
 # -> return)
	rtsd	r15,8 	 # 11	return_internal	[length = 4]
	nop		# Unfilled delay slot

	.end	check_simple_float
$Lfe15:
	.size	check_simple_float,$Lfe15-check_simple_float
	.rodata
	.align	2
$LC7:
	.ascii "%s %lx.%lx\12\0"
	.align	2
$LC8:
	.ascii "%s DONE.\12\0"
	.text
	.align	2
	.globl	check_df
	.ent	check_df
	.type	check_df, @function
check_df:
	.frame	r1,28,r15		# vars= 0, regs= 0, args= 24
	.mask	0x00008000
 #(insn 5 14 7 (set (reg:DF 8 r8 [orig:45 d ] [45])
 #        (const_double:DF 1.23456788999999997336054491370305186137557029724121094e-1 [0x0.fcd6e9b9cb1af8p-3])) mb-testsuite/check_df.c:133 44 {*movdf_internal}
 #     (nil))
	addik	r8,r0,0x3739635f 	 # 5	*movdf_internal/3	[length = 8]
	addik	r9,r0,0x3fbf9add #Xfer Lo
 #(insn 7 5 6 (set (reg:SI 6 r6)
 #        (symbol_ref:SI ("__func__.1323") [flags 0x2] <var_decl 0x7f1793fa4320 __func__>)) mb-testsuite/check_df.c:135 40 {*movsi_internal2}
 #     (nil))
	addik	r6,r0,__func__.1323	 # 7	*movsi_internal2/3	[length = 8]
 #(insn 6 7 8 (set (reg:SI 5 r5)
 #        (symbol_ref/f:SI ("*$LC7") [flags 0x2] <var_decl 0x7f1792eac320 *$LC7>)) mb-testsuite/check_df.c:135 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,$LC7	 # 6	*movsi_internal2/3	[length = 8]
 #(insn 8 6 30 (set (reg:SI 7 r7)
 #        (reg:SI 8 r8 [orig:45 d ] [45])) mb-testsuite/check_df.c:135 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 8 r8 [orig:45 d ] [45])
 #        (expr_list:REG_EQUAL (const_int 926507871 [0x3739635f])
 #            (nil))))
	addk	r7,r8,r0	 # 8	*movsi_internal2/1	[length = 4]
 #(insn/f 30 8 31 (set (reg/f:SI 1 r1)
 #        (minus:SI (reg/f:SI 1 r1)
 #            (const_int 28 [0x1c]))) mb-testsuite/check_df.c:128 9 {subsi3}
 #     (nil))
	addik	r1,r1,-28	 # 30	subsi3/2	[length = 8]
 #(insn/f 31 30 32 (set (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])
 #        (reg:SI 15 r15)) mb-testsuite/check_df.c:128 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg:SI 15 r15)
 #        (nil)))
	swi	r15,r1,0	 # 31	*movsi_internal2/6	[length = 4]
 #(call_insn 10 32 9 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1793f35c00 printf>) [0 __builtin_printf S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:135 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 8 r8)
 #        (expr_list:REG_DEAD (reg:SI 7 r7)
 #            (expr_list:REG_DEAD (reg:SI 6 r6)
 #                (expr_list:REG_DEAD (reg:SI 5 r5)
 #                    (expr_list:REG_UNUSED (reg:SI 3 r3)
 #                        (nil))))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 8 r8))
 #        (expr_list:REG_DEP_TRUE (use (reg:SI 7 r7))
 #            (expr_list:REG_DEP_TRUE (use (reg:SI 6 r6))
 #                (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #                    (nil))))))
	brlid	r15,printf	 # 10	call_value_intern	[length = 4]
	
 #(insn 9 10 11 (set (reg:SI 8 r8)
 #        (reg:SI 9 r9 [ d+4 ])) 40 {*movsi_internal2}
 #     (expr_list:REG_EQUAL (const_int 1069521629 [0x3fbf9add])
 #        (nil)))
	addk	r8,r9,r0	 # 9	*movsi_internal2/1	[length = 4]
 #(insn 11 41 12 (set (reg/f:SI 3 r3 [46])
 #        (symbol_ref:SI ("g_x") <var_decl 0x7f1793eb3140 g_x>)) mb-testsuite/check_df.c:138 40 {*movsi_internal2}
 #     (expr_list:REG_EQUIV (symbol_ref:SI ("g_x") <var_decl 0x7f1793eb3140 g_x>)
 #        (nil)))
	addik	r3,r0,g_x	 # 11	*movsi_internal2/3	[length = 8]
 #(insn 12 11 13 (set (mem/f/c/i:SI (symbol_ref:SI ("gp_x") <var_decl 0x7f1793eb31e0 gp_x>) [4 gp_x+0 S4 A32])
 #        (reg/f:SI 3 r3 [46])) mb-testsuite/check_df.c:138 40 {*movsi_internal2}
 #     (expr_list:REG_DEAD (reg/f:SI 3 r3 [46])
 #        (nil)))
	swi	r3,r0,gp_x	 # 12	*movsi_internal2/7	[length = 8]
 #(call_insn 13 12 15 (parallel [
 #            (call (mem:SI (symbol_ref:SI ("check_df_const_to_reg") [flags 0x3] <function_decl 0x7f1793f7d800 check_df_const_to_reg>) [0 check_df_const_to_reg S4 A32])
 #                (const_int 24 [0x18]))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:141 75 {call_internal1}
 #     (expr_list:REG_EH_REGION (const_int 0 [0])
 #        (nil))
 #    (nil))
	brlid	r15,check_df_const_to_reg	 # 13	call_internal1	[length = 4]
	nop		# Unfilled delay slot

 #(insn 15 13 16 (set (reg:SI 5 r5)
 #        (mem/f/c/i:SI (symbol_ref:SI ("gp_x") <var_decl 0x7f1793eb31e0 gp_x>) [4 gp_x+0 S4 A32])) mb-testsuite/check_df.c:142 40 {*movsi_internal2}
 #     (nil))
	lwi	r5,r0,gp_x	 # 15	*movsi_internal2/5	[length = 8]
 #(call_insn 16 15 17 (parallel [
 #            (call (mem:SI (symbol_ref:SI ("check_df_const_to_mem") [flags 0x3] <function_decl 0x7f1793f7da00 check_df_const_to_mem>) [0 check_df_const_to_mem S4 A32])
 #                (const_int 24 [0x18]))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:142 75 {call_internal1}
 #     (expr_list:REG_DEAD (reg:SI 5 r5)
 #        (expr_list:REG_EH_REGION (const_int 0 [0])
 #            (nil)))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #        (nil)))
	brlid	r15,check_df_const_to_mem	 # 16	call_internal1	[length = 4]
	nop		# Unfilled delay slot

 #(call_insn 17 16 18 (parallel [
 #            (call (mem:SI (symbol_ref:SI ("check_df_reg_to_reg") [flags 0x3] <function_decl 0x7f1793f9e000 check_df_reg_to_reg>) [0 check_df_reg_to_reg S4 A32])
 #                (const_int 24 [0x18]))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:143 75 {call_internal1}
 #     (expr_list:REG_EH_REGION (const_int 0 [0])
 #        (nil))
 #    (nil))
	brlid	r15,check_df_reg_to_reg	 # 17	call_internal1	[length = 4]
	nop		# Unfilled delay slot

 #(call_insn 18 17 19 (parallel [
 #            (call (mem:SI (symbol_ref:SI ("check_df_reg_plus_const_to_reg") [flags 0x3] <function_decl 0x7f1793f9e300 check_df_reg_plus_const_to_reg>) [0 check_df_reg_plus_const_to_reg S4 A32])
 #                (const_int 24 [0x18]))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:144 75 {call_internal1}
 #     (expr_list:REG_EH_REGION (const_int 0 [0])
 #        (nil))
 #    (nil))
	brlid	r15,check_df_reg_plus_const_to_reg	 # 18	call_internal1	[length = 4]
	nop		# Unfilled delay slot

 #(call_insn 19 18 20 (parallel [
 #            (call (mem:SI (symbol_ref:SI ("check_df_reg_to_mem") [flags 0x3] <function_decl 0x7f1793f9e600 check_df_reg_to_mem>) [0 check_df_reg_to_mem S4 A32])
 #                (const_int 24 [0x18]))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:145 75 {call_internal1}
 #     (expr_list:REG_EH_REGION (const_int 0 [0])
 #        (nil))
 #    (nil))
	brlid	r15,check_df_reg_to_mem	 # 19	call_internal1	[length = 4]
	nop		# Unfilled delay slot

 #(call_insn 20 19 21 (parallel [
 #            (call (mem:SI (symbol_ref:SI ("check_df_mem_to_mem") [flags 0x3] <function_decl 0x7f1793f9ea00 check_df_mem_to_mem>) [0 check_df_mem_to_mem S4 A32])
 #                (const_int 24 [0x18]))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:146 75 {call_internal1}
 #     (expr_list:REG_EH_REGION (const_int 0 [0])
 #        (nil))
 #    (nil))
	brlid	r15,check_df_mem_to_mem	 # 20	call_internal1	[length = 4]
	nop		# Unfilled delay slot

 #(call_insn 21 20 22 (parallel [
 #            (call (mem:SI (symbol_ref:SI ("check_df_extend_sf_df") [flags 0x3] <function_decl 0x7f1793f9ed00 check_df_extend_sf_df>) [0 check_df_extend_sf_df S4 A32])
 #                (const_int 24 [0x18]))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:147 75 {call_internal1}
 #     (expr_list:REG_EH_REGION (const_int 0 [0])
 #        (nil))
 #    (nil))
	brlid	r15,check_df_extend_sf_df	 # 21	call_internal1	[length = 4]
	nop		# Unfilled delay slot

 #(insn 22 21 23 (set (reg:SI 5 r5)
 #        (symbol_ref/f:SI ("*$LC8") [flags 0x2] <var_decl 0x7f1792eac3c0 *$LC8>)) mb-testsuite/check_df.c:149 40 {*movsi_internal2}
 #     (nil))
	addik	r5,r0,$LC8	 # 22	*movsi_internal2/3	[length = 8]
 #(insn 23 22 24 (set (reg:SI 6 r6)
 #        (symbol_ref:SI ("__func__.1323") [flags 0x2] <var_decl 0x7f1793fa4320 __func__>)) mb-testsuite/check_df.c:149 40 {*movsi_internal2}
 #     (nil))
	addik	r6,r0,__func__.1323	 # 23	*movsi_internal2/3	[length = 8]
 #(call_insn 24 23 39 (parallel [
 #            (set (reg:SI 3 r3)
 #                (call (mem:SI (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1793f35c00 printf>) [0 __builtin_printf S4 A32])
 #                    (const_int 24 [0x18])))
 #            (clobber (reg:SI 15 r15))
 #        ]) mb-testsuite/check_df.c:149 77 {call_value_intern}
 #     (expr_list:REG_DEAD (reg:SI 6 r6)
 #        (expr_list:REG_DEAD (reg:SI 5 r5)
 #            (expr_list:REG_UNUSED (reg:SI 3 r3)
 #                (nil))))
 #    (expr_list:REG_DEP_TRUE (use (reg:SI 6 r6))
 #        (expr_list:REG_DEP_TRUE (use (reg:SI 5 r5))
 #            (nil))))
	brlid	r15,printf	 # 24	call_value_intern	[length = 4]
	nop		# Unfilled delay slot

 #(insn 34 39 35 (set (reg:SI 15 r15)
 #        (mem:SI (plus:SI (reg/f:SI 1 r1)
 #                (const_int 0 [0])) [0 S4 A32])) mb-testsuite/check_df.c:150 40 {*movsi_internal2}
 #     (nil))
	lwi	r15,r1,0	 # 34	*movsi_internal2/4	[length = 4]
 #(insn 35 34 43 (unspec_volatile [
 #            (const_int 0 [0])
 #        ] 0) mb-testsuite/check_df.c:150 73 {blockage}
 #     (nil))
 #(jump_insn 37 35 36 (parallel [
 #            (use (reg:SI 15 r15))
 #            (return)
 #        ]) mb-testsuite/check_df.c:150 72 {return_internal}
 #     (expr_list:REG_BR_PRED (const_int 12 [0xc])
 #        (expr_list:REG_DEAD (reg:SI 15 r15)
 #            (nil)))
 # -> return)
	rtsd	r15,8 	 # 37	return_internal	[length = 4]
	
 #(insn 36 37 44 (set (reg/f:SI 1 r1)
 #        (plus:SI (reg/f:SI 1 r1)
 #            (const_int 28 [0x1c]))) 7 {addsi3}
 #     (nil))
	addik	r1,r1,28	 # 36	addsi3/2	[length = 4]
	.end	check_df
$Lfe16:
	.size	check_df,$Lfe16-check_df
	.bss
	.comm	gp_x,4,4
	.type	gp_x, @object
	.comm	g_x,8,4
	.type	g_x, @object
	.rodata
	.align	2
	.type	__func__.1270,@object
	.size	__func__.1270,20
__func__.1270:
	.ascii "check_df_reg_to_reg\0"
	.align	2
	.type	__func__.1279,@object
	.size	__func__.1279,31
__func__.1279:
	.ascii "check_df_reg_plus_const_to_reg\0"
	.align	2
	.type	__func__.1288,@object
	.size	__func__.1288,20
__func__.1288:
	.ascii "check_df_reg_to_mem\0"
	.align	2
	.type	__func__.1303,@object
	.size	__func__.1303,20
__func__.1303:
	.ascii "check_df_mem_to_mem\0"
	.align	2
	.type	__func__.1323,@object
	.size	__func__.1323,9
__func__.1323:
	.ascii "check_df\0"
