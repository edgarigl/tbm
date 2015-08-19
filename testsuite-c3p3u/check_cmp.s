	.file	"check_cmp.c"
	.section	.text
	.align	4
	.global	check_bgt
	.type	check_bgt, @function
check_bgt:
;#(insn 6 3 7 check_cmp.c:9 (set (reg/f:SI 1 r1 [30])
;#        (lshiftrt:SI (symbol_ref:SI ("SI_r") <var_decl 0x7f1b59de0000 SI_r>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("SI_r") <var_decl 0x7f1b59de0000 SI_r>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r1, SI_r	;# 6	mov_sym_upper
;#(insn 7 6 8 check_cmp.c:9 (set (reg/f:SI 1 r1 [30])
;#        (ior:SI (reg/f:SI 1 r1 [30])
;#            (symbol_ref:SI ("SI_r") <var_decl 0x7f1b59de0000 SI_r>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("SI_r") <var_decl 0x7f1b59de0000 SI_r>)
;#        (nil)))
	symlo	$r1, SI_r	;# 7	mov_sym_lower
;#(insn 8 7 9 check_cmp.c:9 (set (reg:SI 2 r2 [31])
;#        (const_int 0 [0x0])) 2 {movsi_insn} (expr_list:REG_EQUAL (const_int 0 [0x0])
;#        (nil)))
	movi	$r2, 0	;# 8	movsi_insn/1
;#(insn 9 8 10 check_cmp.c:9 (set (reg:SI 2 r2 [31])
;#        (ior:SI (reg:SI 2 r2 [31])
;#            (const_int 390 [0x186]))) 30 {iorsi3} (expr_list:REG_EQUAL (const_int 390 [0x186])
;#        (nil)))
	ori	$r2, $r2, 390	;# 9	iorsi3/2
;#(insn 10 9 11 check_cmp.c:9 (set (mem/c/i:SI (reg/f:SI 1 r1 [30]) [0 SI_r+0 S4 A32])
;#        (reg:SI 2 r2 [31])) 2 {movsi_insn} (expr_list:REG_DEAD (reg:SI 2 r2 [31])
;#        (expr_list:REG_DEAD (reg/f:SI 1 r1 [30])
;#            (nil))))
	sw	$r2, $r1, 0	;# 10	movsi_insn/3
;#(insn 11 10 12 check_cmp.c:10 (set (reg:SI 1 r1 [32])
;#        (const_int 0 [0x0])) 2 {movsi_insn} (expr_list:REG_EQUAL (const_int 0 [0x0])
;#        (nil)))
	movi	$r1, 0	;# 11	movsi_insn/1
;#(insn 12 11 33 check_cmp.c:10 (set (reg:SI 1 r1 [32])
;#        (ior:SI (reg:SI 1 r1 [32])
;#            (const_int 389 [0x185]))) 30 {iorsi3} (expr_list:REG_EQUAL (const_int 389 [0x185])
;#        (nil)))
	ori	$r1, $r1, 389	;# 12	iorsi3/2
;#(jump_insn 13 12 15 check_cmp.c:10 (set (pc)
;#        (if_then_else (lt:SI (reg:SI 1 r1 [32])
;#                (reg/v:SI 0 r0 [orig:29 x ] [29]))
;#            (label_ref:SI 23)
;#            (pc))) 15 {branch_lt} (expr_list:REG_BR_PRED (const_int 49 [0x31])
;#        (expr_list:REG_DEAD (reg:SI 1 r1 [32])
;#            (expr_list:REG_BR_PROB (const_int 900 [0x384])
;#                (nil)))))
	blt	$r1, $r0, .L3 	;# 13	branch_lt/1
;#(insn 15 13 14 (set (reg/f:SI 1 r1 [33])
;#        (lshiftrt:SI (symbol_ref:SI ("SI_r") <var_decl 0x7f1b59de0000 SI_r>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("SI_r") <var_decl 0x7f1b59de0000 SI_r>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r1, SI_r	;# 15	mov_sym_upper
;#(insn 16 14 17 check_cmp.c:10 (set (reg/f:SI 1 r1 [33])
;#        (ior:SI (reg/f:SI 1 r1 [33])
;#            (symbol_ref:SI ("SI_r") <var_decl 0x7f1b59de0000 SI_r>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("SI_r") <var_decl 0x7f1b59de0000 SI_r>)
;#        (nil)))
	symlo	$r1, SI_r	;# 16	mov_sym_lower
;#(insn 17 16 23 check_cmp.c:10 (set (mem/c/i:SI (reg/f:SI 1 r1 [33]) [0 SI_r+0 S4 A32])
;#        (reg/v:SI 0 r0 [orig:29 x ] [29])) 2 {movsi_insn} (expr_list:REG_DEAD (reg/f:SI 1 r1 [33])
;#        (expr_list:REG_DEAD (reg/v:SI 0 r0 [orig:29 x ] [29])
;#            (nil))))
	sw	$r0, $r1, 0	;# 17	movsi_insn/3
.L3:
;#(jump_insn 30 29 31 check_cmp.c:13 (parallel [
;#            (use (reg:SI 22 lr [0]))
;#            (return)
;#        ]) 19 {return_expanded} (expr_list:REG_BR_PRED (const_int 12 [0xc])
;#        (expr_list:REG_DEAD (reg:SI 22 lr [0])
;#            (nil))))
	j	$lr 
	nop	;# 30	return_expanded
	.size	check_bgt, .-check_bgt
	.align	4
	.global	check_bgtu
	.type	check_bgtu, @function
check_bgtu:
;#(insn 6 3 7 check_cmp.c:17 (set (reg/f:SI 1 r1 [30])
;#        (lshiftrt:SI (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r1, UI_r	;# 6	mov_sym_upper
;#(insn 7 6 8 check_cmp.c:17 (set (reg/f:SI 1 r1 [30])
;#        (ior:SI (reg/f:SI 1 r1 [30])
;#            (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>)
;#        (nil)))
	symlo	$r1, UI_r	;# 7	mov_sym_lower
;#(insn 8 7 9 check_cmp.c:17 (set (reg:SI 2 r2 [31])
;#        (const_int 0 [0x0])) 2 {movsi_insn} (expr_list:REG_EQUAL (const_int 0 [0x0])
;#        (nil)))
	movi	$r2, 0	;# 8	movsi_insn/1
;#(insn 9 8 10 check_cmp.c:17 (set (reg:SI 2 r2 [31])
;#        (ior:SI (reg:SI 2 r2 [31])
;#            (const_int 390 [0x186]))) 30 {iorsi3} (expr_list:REG_EQUAL (const_int 390 [0x186])
;#        (nil)))
	ori	$r2, $r2, 390	;# 9	iorsi3/2
;#(insn 10 9 11 check_cmp.c:17 (set (mem/c/i:SI (reg/f:SI 1 r1 [30]) [0 UI_r+0 S4 A32])
;#        (reg:SI 2 r2 [31])) 2 {movsi_insn} (expr_list:REG_DEAD (reg:SI 2 r2 [31])
;#        (expr_list:REG_DEAD (reg/f:SI 1 r1 [30])
;#            (nil))))
	sw	$r2, $r1, 0	;# 10	movsi_insn/3
;#(insn 11 10 12 check_cmp.c:18 (set (reg:SI 1 r1 [32])
;#        (const_int 0 [0x0])) 2 {movsi_insn} (expr_list:REG_EQUAL (const_int 0 [0x0])
;#        (nil)))
	movi	$r1, 0	;# 11	movsi_insn/1
;#(insn 12 11 33 check_cmp.c:18 (set (reg:SI 1 r1 [32])
;#        (ior:SI (reg:SI 1 r1 [32])
;#            (const_int 389 [0x185]))) 30 {iorsi3} (expr_list:REG_EQUAL (const_int 389 [0x185])
;#        (nil)))
	ori	$r1, $r1, 389	;# 12	iorsi3/2
;#(jump_insn 13 12 15 check_cmp.c:18 (set (pc)
;#        (if_then_else (ltu:SI (reg:SI 1 r1 [32])
;#                (reg/v:SI 0 r0 [orig:29 x ] [29]))
;#            (label_ref:SI 23)
;#            (pc))) 16 {branch_ltu} (expr_list:REG_BR_PRED (const_int 49 [0x31])
;#        (expr_list:REG_DEAD (reg:SI 1 r1 [32])
;#            (expr_list:REG_BR_PROB (const_int 900 [0x384])
;#                (nil)))))
	bltu	$r1, $r0, .L6 	;# 13	branch_ltu/1
;#(insn 15 13 14 (set (reg/f:SI 1 r1 [33])
;#        (lshiftrt:SI (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r1, UI_r	;# 15	mov_sym_upper
;#(insn 16 14 17 check_cmp.c:18 (set (reg/f:SI 1 r1 [33])
;#        (ior:SI (reg/f:SI 1 r1 [33])
;#            (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>)
;#        (nil)))
	symlo	$r1, UI_r	;# 16	mov_sym_lower
;#(insn 17 16 23 check_cmp.c:18 (set (mem/c/i:SI (reg/f:SI 1 r1 [33]) [0 UI_r+0 S4 A32])
;#        (reg/v:SI 0 r0 [orig:29 x ] [29])) 2 {movsi_insn} (expr_list:REG_DEAD (reg/f:SI 1 r1 [33])
;#        (expr_list:REG_DEAD (reg/v:SI 0 r0 [orig:29 x ] [29])
;#            (nil))))
	sw	$r0, $r1, 0	;# 17	movsi_insn/3
.L6:
;#(jump_insn 30 29 31 check_cmp.c:20 (parallel [
;#            (use (reg:SI 22 lr [0]))
;#            (return)
;#        ]) 19 {return_expanded} (expr_list:REG_BR_PRED (const_int 12 [0xc])
;#        (expr_list:REG_DEAD (reg:SI 22 lr [0])
;#            (nil))))
	j	$lr 
	nop	;# 30	return_expanded
	.size	check_bgtu, .-check_bgtu
	.align	4
	.global	check_bltu
	.type	check_bltu, @function
check_bltu:
;#(insn 6 3 7 check_cmp.c:25 (set (reg/f:SI 1 r1 [30])
;#        (lshiftrt:SI (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r1, UI_r	;# 6	mov_sym_upper
;#(insn 7 6 8 check_cmp.c:25 (set (reg/f:SI 1 r1 [30])
;#        (ior:SI (reg/f:SI 1 r1 [30])
;#            (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>)
;#        (nil)))
	symlo	$r1, UI_r	;# 7	mov_sym_lower
;#(insn 25 8 31 check_cmp.c:25 (set (reg:SI 2 r2)
;#        (const_int 0 [0x0])) 2 {movsi_insn} (nil))
	movi	$r2, 0	;# 25	movsi_insn/1
;#(jump_insn 10 25 9 check_cmp.c:26 (set (pc)
;#        (if_then_else (eq:SI (reg/v:SI 0 r0 [orig:29 x ] [29])
;#                (reg:SI 2 r2))
;#            (label_ref:SI 20)
;#            (pc))) 12 {branch_eq} (expr_list:REG_BR_PRED (const_int 49 [0x31])
;#        (expr_list:REG_DEAD (reg:SI 2 r2)
;#            (expr_list:REG_BR_PROB (const_int 900 [0x384])
;#                (nil)))))
	beq	$r0, $r2, .L9 	;# 10	branch_eq/1
;#(insn 9 10 11 (set (mem/c/i:SI (reg/f:SI 1 r1 [30]) [0 UI_r+0 S4 A32])
;#        (reg:SI 2 r2)) 2 {movsi_insn} (nil))
	sw	$r2, $r1, 0	;# 9	movsi_insn/3
;#(insn 12 11 13 check_cmp.c:26 (set (reg/f:SI 1 r1 [32])
;#        (lshiftrt:SI (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r1, UI_r	;# 12	mov_sym_upper
;#(insn 13 12 14 check_cmp.c:26 (set (reg/f:SI 1 r1 [32])
;#        (ior:SI (reg/f:SI 1 r1 [32])
;#            (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>)
;#        (nil)))
	symlo	$r1, UI_r	;# 13	mov_sym_lower
;#(insn 14 13 20 check_cmp.c:26 (set (mem/c/i:SI (reg/f:SI 1 r1 [32]) [0 UI_r+0 S4 A32])
;#        (reg/v:SI 0 r0 [orig:29 x ] [29])) 2 {movsi_insn} (expr_list:REG_DEAD (reg/f:SI 1 r1 [32])
;#        (expr_list:REG_DEAD (reg/v:SI 0 r0 [orig:29 x ] [29])
;#            (nil))))
	sw	$r0, $r1, 0	;# 14	movsi_insn/3
.L9:
;#(jump_insn 28 27 29 check_cmp.c:28 (parallel [
;#            (use (reg:SI 22 lr [0]))
;#            (return)
;#        ]) 19 {return_expanded} (expr_list:REG_BR_PRED (const_int 12 [0xc])
;#        (expr_list:REG_DEAD (reg:SI 22 lr [0])
;#            (nil))))
	j	$lr 
	nop	;# 28	return_expanded
	.size	check_bltu, .-check_bltu
	.align	4
	.global	check_cmp_basic
	.type	check_cmp_basic, @function
check_cmp_basic:
;#(insn 137 3 138 check_cmp.c:31 (set (reg/f:SI 23 sp)
;#        (plus:SI (reg/f:SI 23 sp)
;#            (const_int -12 [0xfffffffffffffff4]))) 0 {addsi3} (nil))
	subi	$sp, $sp, 12	;# 137	addsi3/2
;#(insn/f 138 137 140 check_cmp.c:31 (set (mem:SI (plus:SI (reg/f:SI 23 sp)
;#                (const_int 8 [0x8])) [0 S4 A32])
;#        (reg:SI 22 lr [0])) 2 {movsi_insn} (expr_list:REG_DEAD (reg:SI 22 lr [0])
;#        (nil)))
	sw	$lr, $sp, 8	;# 138	movsi_insn/3
;#(insn 131 2 6 check_cmp.c:35 (set (reg:SI 0 r0)
;#        (const_int 10 [0xa])) 2 {movsi_insn} (nil))
	movi	$r0, 10	;# 131	movsi_insn/1
;#(insn 6 131 7 check_cmp.c:35 (set (mem/v/c/i:SI (plus:SI (reg/f:SI 23 sp)
;#                (const_int 4 [0x4])) [0 x+0 S4 A32])
;#        (reg:SI 0 r0)) 2 {movsi_insn} (expr_list:REG_DEAD (reg:SI 0 r0)
;#        (nil)))
	sw	$r0, $sp, 4	;# 6	movsi_insn/3
;#(insn 7 6 132 check_cmp.c:36 (set (reg:SI 0 r0 [orig:40 x.6 ] [40])
;#        (mem/v/c/i:SI (plus:SI (reg/f:SI 23 sp)
;#                (const_int 4 [0x4])) [0 x+0 S4 A32])) 2 {movsi_insn} (nil))
	lw	$r0, $sp, 4	;# 7	movsi_insn/4
;#(insn 132 7 151 check_cmp.c:36 (set (reg:SI 1 r1)
;#        (const_int 10 [0xa])) 2 {movsi_insn} (nil))
	movi	$r1, 10	;# 132	movsi_insn/1
;#(jump_insn 8 132 10 check_cmp.c:36 (set (pc)
;#        (if_then_else (le:SI (reg:SI 0 r0 [orig:40 x.6 ] [40])
;#                (reg:SI 1 r1))
;#            (label_ref 14)
;#            (pc))) 14 {branch_le} (expr_list:REG_BR_PRED (const_int 13 [0xd])
;#        (expr_list:REG_DEAD (reg:SI 1 r1)
;#            (expr_list:REG_DEAD (reg:SI 0 r0 [orig:40 x.6 ] [40])
;#                (expr_list:REG_BR_PROB (const_int 9996 [0x270c])
;#                    (nil))))))
	ble	$r0, $r1, .L11 	;# 8	branch_le/1
;#(insn 10 8 9 (set (reg/f:SI 0 r0 [42])
;#        (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, err	;# 10	mov_sym_upper
;#(insn 11 9 12 check_cmp.c:37 (set (reg/f:SI 0 r0 [42])
;#        (ior:SI (reg/f:SI 0 r0 [42])
;#            (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#        (nil)))
	symlo	$r0, err	;# 11	mov_sym_lower
;#(call_insn 12 11 13 check_cmp.c:37 (parallel [
;#            (call (mem:SI (reg/f:SI 0 r0 [42]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 0 r0 [42])
;#        (expr_list:REG_NORETURN (const_int 0 [0x0])
;#            (nil)))
;#    (nil))
	bl	$lr, $r0, 0
	nop	;# 12	call_expanded
.L11:
;#(insn 16 15 133 check_cmp.c:38 (set (reg:SI 0 r0 [orig:39 x.7 ] [39])
;#        (mem/v/c/i:SI (plus:SI (reg/f:SI 23 sp)
;#                (const_int 4 [0x4])) [0 x+0 S4 A32])) 2 {movsi_insn} (nil))
	lw	$r0, $sp, 4	;# 16	movsi_insn/4
;#(insn 133 16 153 check_cmp.c:38 (set (reg:SI 1 r1)
;#        (const_int 10 [0xa])) 2 {movsi_insn} (nil))
	movi	$r1, 10	;# 133	movsi_insn/1
;#(jump_insn 17 133 19 check_cmp.c:38 (set (pc)
;#        (if_then_else (le:SI (reg:SI 0 r0 [orig:39 x.7 ] [39])
;#                (reg:SI 1 r1))
;#            (label_ref 23)
;#            (pc))) 14 {branch_le} (expr_list:REG_BR_PRED (const_int 13 [0xd])
;#        (expr_list:REG_DEAD (reg:SI 1 r1)
;#            (expr_list:REG_DEAD (reg:SI 0 r0 [orig:39 x.7 ] [39])
;#                (expr_list:REG_BR_PROB (const_int 9996 [0x270c])
;#                    (nil))))))
	ble	$r0, $r1, .L12 	;# 17	branch_le/1
;#(insn 19 17 18 (set (reg/f:SI 0 r0 [43])
;#        (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, err	;# 19	mov_sym_upper
;#(insn 20 18 21 check_cmp.c:39 (set (reg/f:SI 0 r0 [43])
;#        (ior:SI (reg/f:SI 0 r0 [43])
;#            (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#        (nil)))
	symlo	$r0, err	;# 20	mov_sym_lower
;#(call_insn 21 20 22 check_cmp.c:39 (parallel [
;#            (call (mem:SI (reg/f:SI 0 r0 [43]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 0 r0 [43])
;#        (expr_list:REG_NORETURN (const_int 0 [0x0])
;#            (nil)))
;#    (nil))
	bl	$lr, $r0, 0
	nop	;# 21	call_expanded
.L12:
;#(insn 25 24 26 check_cmp.c:40 (set (reg:SI 0 r0 [orig:38 x.8 ] [38])
;#        (mem/v/c/i:SI (plus:SI (reg/f:SI 23 sp)
;#                (const_int 4 [0x4])) [0 x+0 S4 A32])) 2 {movsi_insn} (nil))
	lw	$r0, $sp, 4	;# 25	movsi_insn/4
;#(insn 26 25 155 check_cmp.c:40 (set (reg:SI 1 r1 [44])
;#        (const_int 9 [0x9])) 2 {movsi_insn} (expr_list:REG_EQUIV (const_int 9 [0x9])
;#        (nil)))
	movi	$r1, 9	;# 26	movsi_insn/1
;#(jump_insn 27 26 29 check_cmp.c:40 (set (pc)
;#        (if_then_else (lt:SI (reg:SI 1 r1 [44])
;#                (reg:SI 0 r0 [orig:38 x.8 ] [38]))
;#            (label_ref 33)
;#            (pc))) 15 {branch_lt} (expr_list:REG_BR_PRED (const_int 13 [0xd])
;#        (expr_list:REG_DEAD (reg:SI 1 r1 [44])
;#            (expr_list:REG_DEAD (reg:SI 0 r0 [orig:38 x.8 ] [38])
;#                (expr_list:REG_BR_PROB (const_int 9996 [0x270c])
;#                    (nil))))))
	blt	$r1, $r0, .L13 	;# 27	branch_lt/1
;#(insn 29 27 28 (set (reg/f:SI 0 r0 [45])
;#        (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, err	;# 29	mov_sym_upper
;#(insn 30 28 31 check_cmp.c:41 (set (reg/f:SI 0 r0 [45])
;#        (ior:SI (reg/f:SI 0 r0 [45])
;#            (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#        (nil)))
	symlo	$r0, err	;# 30	mov_sym_lower
;#(call_insn 31 30 32 check_cmp.c:41 (parallel [
;#            (call (mem:SI (reg/f:SI 0 r0 [45]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 0 r0 [45])
;#        (expr_list:REG_NORETURN (const_int 0 [0x0])
;#            (nil)))
;#    (nil))
	bl	$lr, $r0, 0
	nop	;# 31	call_expanded
.L13:
;#(insn 35 34 36 check_cmp.c:42 (set (reg:SI 0 r0 [orig:37 x.9 ] [37])
;#        (mem/v/c/i:SI (plus:SI (reg/f:SI 23 sp)
;#                (const_int 4 [0x4])) [0 x+0 S4 A32])) 2 {movsi_insn} (nil))
	lw	$r0, $sp, 4	;# 35	movsi_insn/4
;#(insn 36 35 157 check_cmp.c:42 (set (reg:SI 1 r1 [46])
;#        (const_int 9 [0x9])) 2 {movsi_insn} (expr_list:REG_EQUIV (const_int 9 [0x9])
;#        (nil)))
	movi	$r1, 9	;# 36	movsi_insn/1
;#(jump_insn 37 36 39 check_cmp.c:42 (set (pc)
;#        (if_then_else (lt:SI (reg:SI 1 r1 [46])
;#                (reg:SI 0 r0 [orig:37 x.9 ] [37]))
;#            (label_ref 43)
;#            (pc))) 15 {branch_lt} (expr_list:REG_BR_PRED (const_int 13 [0xd])
;#        (expr_list:REG_DEAD (reg:SI 1 r1 [46])
;#            (expr_list:REG_DEAD (reg:SI 0 r0 [orig:37 x.9 ] [37])
;#                (expr_list:REG_BR_PROB (const_int 9996 [0x270c])
;#                    (nil))))))
	blt	$r1, $r0, .L14 	;# 37	branch_lt/1
;#(insn 39 37 38 (set (reg/f:SI 0 r0 [47])
;#        (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, err	;# 39	mov_sym_upper
;#(insn 40 38 41 check_cmp.c:43 (set (reg/f:SI 0 r0 [47])
;#        (ior:SI (reg/f:SI 0 r0 [47])
;#            (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#        (nil)))
	symlo	$r0, err	;# 40	mov_sym_lower
;#(call_insn 41 40 42 check_cmp.c:43 (parallel [
;#            (call (mem:SI (reg/f:SI 0 r0 [47]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 0 r0 [47])
;#        (expr_list:REG_NORETURN (const_int 0 [0x0])
;#            (nil)))
;#    (nil))
	bl	$lr, $r0, 0
	nop	;# 41	call_expanded
.L14:
;#(insn 45 44 159 check_cmp.c:44 (set (reg:SI 0 r0 [orig:36 x.10 ] [36])
;#        (mem/v/c/i:SI (plus:SI (reg/f:SI 23 sp)
;#                (const_int 4 [0x4])) [0 x+0 S4 A32])) 2 {movsi_insn} (nil))
	lw	$r0, $sp, 4	;# 45	movsi_insn/4
;#(jump_insn 46 45 48 check_cmp.c:44 (set (pc)
;#        (if_then_else (ne:SI (reg:SI 0 r0 [orig:36 x.10 ] [36])
;#                (const_int 0 [0x0]))
;#            (label_ref 52)
;#            (pc))) 13 {branch_ne} (expr_list:REG_BR_PRED (const_int 13 [0xd])
;#        (expr_list:REG_DEAD (reg:SI 0 r0 [orig:36 x.10 ] [36])
;#            (expr_list:REG_BR_PROB (const_int 9996 [0x270c])
;#                (nil)))))
	bnei	$r0, 0, .L15 	;# 46	branch_ne/2
;#(insn 48 46 47 (set (reg/f:SI 0 r0 [48])
;#        (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, err	;# 48	mov_sym_upper
;#(insn 49 47 50 check_cmp.c:45 (set (reg/f:SI 0 r0 [48])
;#        (ior:SI (reg/f:SI 0 r0 [48])
;#            (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#        (nil)))
	symlo	$r0, err	;# 49	mov_sym_lower
;#(call_insn 50 49 51 check_cmp.c:45 (parallel [
;#            (call (mem:SI (reg/f:SI 0 r0 [48]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 0 r0 [48])
;#        (expr_list:REG_NORETURN (const_int 0 [0x0])
;#            (nil)))
;#    (nil))
	bl	$lr, $r0, 0
	nop	;# 50	call_expanded
.L15:
;#(insn 54 53 55 check_cmp.c:46 (set (reg:SI 0 r0 [orig:35 x.11 ] [35])
;#        (mem/v/c/i:SI (plus:SI (reg/f:SI 23 sp)
;#                (const_int 4 [0x4])) [0 x+0 S4 A32])) 2 {movsi_insn} (nil))
	lw	$r0, $sp, 4	;# 54	movsi_insn/4
;#(jump_insn 56 55 134 check_cmp.c:46 (set (pc)
;#        (if_then_else (ge:SI (reg:SI 0 r0 [orig:35 x.11 ] [35])
;#                (const_int 0 [0x0]))
;#            (label_ref 62)
;#            (pc))) 18 {branch_zero} (expr_list:REG_BR_PRED (const_int 13 [0xd])
;#        (expr_list:REG_DEAD (reg:SI 0 r0 [orig:35 x.11 ] [35])
;#            (expr_list:REG_BR_PROB (const_int 9996 [0x270c])
;#                (nil)))))
	beqi	$r0, 0, .L16 	;# 56	branch_zero
;#(insn/s 134 56 57 (set (reg:SI 0 r0)
;#        (const_int 10 [0xa])) 2 {movsi_insn} (nil))
	movi	$r0, 10	;# 134	movsi_insn/1
;#(insn 58 57 59 check_cmp.c:47 (set (reg/f:SI 0 r0 [50])
;#        (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, err	;# 58	mov_sym_upper
;#(insn 59 58 60 check_cmp.c:47 (set (reg/f:SI 0 r0 [50])
;#        (ior:SI (reg/f:SI 0 r0 [50])
;#            (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#        (nil)))
	symlo	$r0, err	;# 59	mov_sym_lower
;#(call_insn 60 59 61 check_cmp.c:47 (parallel [
;#            (call (mem:SI (reg/f:SI 0 r0 [50]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 0 r0 [50])
;#        (expr_list:REG_NORETURN (const_int 0 [0x0])
;#            (nil)))
;#    (nil))
	bl	$lr, $r0, 0
	nop	;# 60	call_expanded
.L16:
;#(insn 65 63 66 check_cmp.c:49 (set (mem/v/c/i:SI (reg/f:SI 23 sp) [0 y+0 S4 A32])
;#        (reg:SI 0 r0)) 2 {movsi_insn} (expr_list:REG_DEAD (reg:SI 0 r0)
;#        (nil)))
	sw	$r0, $sp, 0	;# 65	movsi_insn/3
;#(insn 66 65 135 check_cmp.c:50 (set (reg:SI 0 r0 [orig:34 y.12 ] [34])
;#        (mem/v/c/i:SI (reg/f:SI 23 sp) [0 y+0 S4 A32])) 2 {movsi_insn} (nil))
	lw	$r0, $sp, 0	;# 66	movsi_insn/4
;#(insn 135 66 163 check_cmp.c:50 (set (reg:SI 1 r1)
;#        (const_int 10 [0xa])) 2 {movsi_insn} (nil))
	movi	$r1, 10	;# 135	movsi_insn/1
;#(jump_insn 67 135 69 check_cmp.c:50 (set (pc)
;#        (if_then_else (leu:SI (reg:SI 0 r0 [orig:34 y.12 ] [34])
;#                (reg:SI 1 r1))
;#            (label_ref 73)
;#            (pc))) 17 {branch_leu} (expr_list:REG_BR_PRED (const_int 13 [0xd])
;#        (expr_list:REG_DEAD (reg:SI 1 r1)
;#            (expr_list:REG_DEAD (reg:SI 0 r0 [orig:34 y.12 ] [34])
;#                (expr_list:REG_BR_PROB (const_int 9996 [0x270c])
;#                    (nil))))))
	bleu	$r0, $r1, .L17 	;# 67	branch_leu/1
;#(insn 69 67 68 (set (reg/f:SI 0 r0 [52])
;#        (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, err	;# 69	mov_sym_upper
;#(insn 70 68 71 check_cmp.c:51 (set (reg/f:SI 0 r0 [52])
;#        (ior:SI (reg/f:SI 0 r0 [52])
;#            (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#        (nil)))
	symlo	$r0, err	;# 70	mov_sym_lower
;#(call_insn 71 70 72 check_cmp.c:51 (parallel [
;#            (call (mem:SI (reg/f:SI 0 r0 [52]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 0 r0 [52])
;#        (expr_list:REG_NORETURN (const_int 0 [0x0])
;#            (nil)))
;#    (nil))
	bl	$lr, $r0, 0
	nop	;# 71	call_expanded
.L17:
;#(insn 75 74 136 check_cmp.c:52 (set (reg:SI 0 r0 [orig:33 y.13 ] [33])
;#        (mem/v/c/i:SI (reg/f:SI 23 sp) [0 y+0 S4 A32])) 2 {movsi_insn} (nil))
	lw	$r0, $sp, 0	;# 75	movsi_insn/4
;#(insn 136 75 165 check_cmp.c:52 (set (reg:SI 1 r1)
;#        (const_int 10 [0xa])) 2 {movsi_insn} (nil))
	movi	$r1, 10	;# 136	movsi_insn/1
;#(jump_insn 76 136 78 check_cmp.c:52 (set (pc)
;#        (if_then_else (leu:SI (reg:SI 0 r0 [orig:33 y.13 ] [33])
;#                (reg:SI 1 r1))
;#            (label_ref 82)
;#            (pc))) 17 {branch_leu} (expr_list:REG_BR_PRED (const_int 13 [0xd])
;#        (expr_list:REG_DEAD (reg:SI 1 r1)
;#            (expr_list:REG_DEAD (reg:SI 0 r0 [orig:33 y.13 ] [33])
;#                (expr_list:REG_BR_PROB (const_int 9996 [0x270c])
;#                    (nil))))))
	bleu	$r0, $r1, .L18 	;# 76	branch_leu/1
;#(insn 78 76 77 (set (reg/f:SI 0 r0 [53])
;#        (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, err	;# 78	mov_sym_upper
;#(insn 79 77 80 check_cmp.c:53 (set (reg/f:SI 0 r0 [53])
;#        (ior:SI (reg/f:SI 0 r0 [53])
;#            (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#        (nil)))
	symlo	$r0, err	;# 79	mov_sym_lower
;#(call_insn 80 79 81 check_cmp.c:53 (parallel [
;#            (call (mem:SI (reg/f:SI 0 r0 [53]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 0 r0 [53])
;#        (expr_list:REG_NORETURN (const_int 0 [0x0])
;#            (nil)))
;#    (nil))
	bl	$lr, $r0, 0
	nop	;# 80	call_expanded
.L18:
;#(insn 84 83 85 check_cmp.c:54 (set (reg:SI 0 r0 [orig:32 y.14 ] [32])
;#        (mem/v/c/i:SI (reg/f:SI 23 sp) [0 y+0 S4 A32])) 2 {movsi_insn} (nil))
	lw	$r0, $sp, 0	;# 84	movsi_insn/4
;#(insn 85 84 167 check_cmp.c:54 (set (reg:SI 1 r1 [54])
;#        (const_int 9 [0x9])) 2 {movsi_insn} (expr_list:REG_EQUIV (const_int 9 [0x9])
;#        (nil)))
	movi	$r1, 9	;# 85	movsi_insn/1
;#(jump_insn 86 85 88 check_cmp.c:54 (set (pc)
;#        (if_then_else (ltu:SI (reg:SI 1 r1 [54])
;#                (reg:SI 0 r0 [orig:32 y.14 ] [32]))
;#            (label_ref 92)
;#            (pc))) 16 {branch_ltu} (expr_list:REG_BR_PRED (const_int 13 [0xd])
;#        (expr_list:REG_DEAD (reg:SI 1 r1 [54])
;#            (expr_list:REG_DEAD (reg:SI 0 r0 [orig:32 y.14 ] [32])
;#                (expr_list:REG_BR_PROB (const_int 9996 [0x270c])
;#                    (nil))))))
	bltu	$r1, $r0, .L19 	;# 86	branch_ltu/1
;#(insn 88 86 87 (set (reg/f:SI 0 r0 [55])
;#        (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, err	;# 88	mov_sym_upper
;#(insn 89 87 90 check_cmp.c:55 (set (reg/f:SI 0 r0 [55])
;#        (ior:SI (reg/f:SI 0 r0 [55])
;#            (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#        (nil)))
	symlo	$r0, err	;# 89	mov_sym_lower
;#(call_insn 90 89 91 check_cmp.c:55 (parallel [
;#            (call (mem:SI (reg/f:SI 0 r0 [55]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 0 r0 [55])
;#        (expr_list:REG_NORETURN (const_int 0 [0x0])
;#            (nil)))
;#    (nil))
	bl	$lr, $r0, 0
	nop	;# 90	call_expanded
.L19:
;#(insn 94 93 95 check_cmp.c:56 (set (reg:SI 0 r0 [orig:31 y.15 ] [31])
;#        (mem/v/c/i:SI (reg/f:SI 23 sp) [0 y+0 S4 A32])) 2 {movsi_insn} (nil))
	lw	$r0, $sp, 0	;# 94	movsi_insn/4
;#(insn 95 94 169 check_cmp.c:56 (set (reg:SI 1 r1 [56])
;#        (const_int 9 [0x9])) 2 {movsi_insn} (expr_list:REG_EQUIV (const_int 9 [0x9])
;#        (nil)))
	movi	$r1, 9	;# 95	movsi_insn/1
;#(jump_insn 96 95 98 check_cmp.c:56 (set (pc)
;#        (if_then_else (ltu:SI (reg:SI 1 r1 [56])
;#                (reg:SI 0 r0 [orig:31 y.15 ] [31]))
;#            (label_ref 102)
;#            (pc))) 16 {branch_ltu} (expr_list:REG_BR_PRED (const_int 13 [0xd])
;#        (expr_list:REG_DEAD (reg:SI 1 r1 [56])
;#            (expr_list:REG_DEAD (reg:SI 0 r0 [orig:31 y.15 ] [31])
;#                (expr_list:REG_BR_PROB (const_int 9996 [0x270c])
;#                    (nil))))))
	bltu	$r1, $r0, .L20 	;# 96	branch_ltu/1
;#(insn 98 96 97 (set (reg/f:SI 0 r0 [57])
;#        (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, err	;# 98	mov_sym_upper
;#(insn 99 97 100 check_cmp.c:57 (set (reg/f:SI 0 r0 [57])
;#        (ior:SI (reg/f:SI 0 r0 [57])
;#            (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#        (nil)))
	symlo	$r0, err	;# 99	mov_sym_lower
;#(call_insn 100 99 101 check_cmp.c:57 (parallel [
;#            (call (mem:SI (reg/f:SI 0 r0 [57]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 0 r0 [57])
;#        (expr_list:REG_NORETURN (const_int 0 [0x0])
;#            (nil)))
;#    (nil))
	bl	$lr, $r0, 0
	nop	;# 100	call_expanded
.L20:
;#(insn 104 103 171 check_cmp.c:58 (set (reg:SI 0 r0 [orig:30 y.16 ] [30])
;#        (mem/v/c/i:SI (reg/f:SI 23 sp) [0 y+0 S4 A32])) 2 {movsi_insn} (nil))
	lw	$r0, $sp, 0	;# 104	movsi_insn/4
;#(jump_insn 105 104 107 check_cmp.c:58 (set (pc)
;#        (if_then_else (ne:SI (reg:SI 0 r0 [orig:30 y.16 ] [30])
;#                (const_int 0 [0x0]))
;#            (label_ref 111)
;#            (pc))) 13 {branch_ne} (expr_list:REG_BR_PRED (const_int 13 [0xd])
;#        (expr_list:REG_DEAD (reg:SI 0 r0 [orig:30 y.16 ] [30])
;#            (expr_list:REG_BR_PROB (const_int 9996 [0x270c])
;#                (nil)))))
	bnei	$r0, 0, .L21 	;# 105	branch_ne/2
;#(insn 107 105 106 (set (reg/f:SI 0 r0 [58])
;#        (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, err	;# 107	mov_sym_upper
;#(insn 108 106 109 check_cmp.c:59 (set (reg/f:SI 0 r0 [58])
;#        (ior:SI (reg/f:SI 0 r0 [58])
;#            (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#        (nil)))
	symlo	$r0, err	;# 108	mov_sym_lower
;#(call_insn 109 108 110 check_cmp.c:59 (parallel [
;#            (call (mem:SI (reg/f:SI 0 r0 [58]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 0 r0 [58])
;#        (expr_list:REG_NORETURN (const_int 0 [0x0])
;#            (nil)))
;#    (nil))
	bl	$lr, $r0, 0
	nop	;# 109	call_expanded
.L21:
;#(insn 113 112 114 check_cmp.c:60 (set (reg:SI 0 r0 [orig:29 y.17 ] [29])
;#        (mem/v/c/i:SI (reg/f:SI 23 sp) [0 y+0 S4 A32])) 2 {movsi_insn} (nil))
	lw	$r0, $sp, 0	;# 113	movsi_insn/4
;#(jump_insn 115 114 143 check_cmp.c:60 (set (pc)
;#        (if_then_else (ge:SI (reg:SI 0 r0 [orig:29 y.17 ] [29])
;#                (const_int 0 [0x0]))
;#            (label_ref:SI 126)
;#            (pc))) 18 {branch_zero} (expr_list:REG_BR_PRED (const_int 13 [0xd])
;#        (expr_list:REG_DEAD (reg:SI 0 r0 [orig:29 y.17 ] [29])
;#            (expr_list:REG_BR_PROB (const_int 9996 [0x270c])
;#                (nil)))))
	beqi	$r0, 0, .L23 	;# 115	branch_zero
;#(insn/s 143 115 116 (set (reg:SI 22 lr [0])
;#        (mem:SI (plus:SI (reg/f:SI 23 sp)
;#                (const_int 8 [0x8])) [0 S4 A32])) 2 {movsi_insn} (nil))
	lw	$lr, $sp, 8	;# 143	movsi_insn/4
;#(insn 117 116 118 check_cmp.c:61 (set (reg/f:SI 0 r0 [60])
;#        (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, err	;# 117	mov_sym_upper
;#(insn 118 117 119 check_cmp.c:61 (set (reg/f:SI 0 r0 [60])
;#        (ior:SI (reg/f:SI 0 r0 [60])
;#            (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#        (nil)))
	symlo	$r0, err	;# 118	mov_sym_lower
;#(call_insn 119 118 120 check_cmp.c:61 (parallel [
;#            (call (mem:SI (reg/f:SI 0 r0 [60]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 0 r0 [60])
;#        (expr_list:REG_NORETURN (const_int 0 [0x0])
;#            (nil)))
;#    (nil))
	bl	$lr, $r0, 0
	nop	;# 119	call_expanded
.L23:
;#(jump_insn 145 141 144 check_cmp.c:62 (parallel [
;#            (use (reg:SI 22 lr [0]))
;#            (return)
;#        ]) 19 {return_expanded} (expr_list:REG_BR_PRED (const_int 12 [0xc])
;#        (expr_list:REG_DEAD (reg:SI 22 lr [0])
;#            (nil))))
	j	$lr 	;# 145	return_expanded
;#(insn/f 144 145 149 (set (reg/f:SI 23 sp)
;#        (plus:SI (reg/f:SI 23 sp)
;#            (const_int 12 [0xc]))) 0 {addsi3} (nil))
	addi	$sp, $sp, 12	;# 144	addsi3/3
	.size	check_cmp_basic, .-check_cmp_basic
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	4
.LC0:
	.string	"%d\n"
	.section	.text
	.align	4
	.global	check_cmp
	.type	check_cmp, @function
check_cmp:
;#(insn 76 3 77 check_cmp.c:65 (set (reg/f:SI 23 sp)
;#        (plus:SI (reg/f:SI 23 sp)
;#            (const_int -4 [0xfffffffffffffffc]))) 0 {addsi3} (nil))
	subi	$sp, $sp, 4	;# 76	addsi3/2
;#(insn/f 77 76 78 check_cmp.c:65 (set (mem:SI (reg/f:SI 23 sp) [0 S4 A32])
;#        (reg:SI 22 lr [0])) 2 {movsi_insn} (expr_list:REG_DEAD (reg:SI 22 lr [0])
;#        (nil)))
	sw	$lr, $sp, 0	;# 77	movsi_insn/3
;#(insn 5 2 6 check_cmp.c:66 (set (reg/f:SI 0 r0 [30])
;#        (lshiftrt:SI (symbol_ref:SI ("check_cmp_basic") [flags 0x3] <function_decl 0x7f1b59de3300 check_cmp_basic>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("check_cmp_basic") [flags 0x3] <function_decl 0x7f1b59de3300 check_cmp_basic>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, check_cmp_basic	;# 5	mov_sym_upper
;#(insn 6 5 7 check_cmp.c:66 (set (reg/f:SI 0 r0 [30])
;#        (ior:SI (reg/f:SI 0 r0 [30])
;#            (symbol_ref:SI ("check_cmp_basic") [flags 0x3] <function_decl 0x7f1b59de3300 check_cmp_basic>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("check_cmp_basic") [flags 0x3] <function_decl 0x7f1b59de3300 check_cmp_basic>)
;#        (nil)))
	symlo	$r0, check_cmp_basic	;# 6	mov_sym_lower
;#(call_insn 7 6 8 check_cmp.c:66 (parallel [
;#            (call (mem:SI (reg/f:SI 0 r0 [30]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 0 r0 [30])
;#        (expr_list:REG_EH_REGION (const_int 0 [0x0])
;#            (nil)))
;#    (nil))
	bl	$lr, $r0, 0
	nop	;# 7	call_expanded
;#(insn 8 7 9 check_cmp.c:67 (set (reg/f:SI 1 r1 [31])
;#        (lshiftrt:SI (symbol_ref:SI ("check_bgt") [flags 0x3] <function_decl 0x7f1b59dcdd00 check_bgt>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("check_bgt") [flags 0x3] <function_decl 0x7f1b59dcdd00 check_bgt>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r1, check_bgt	;# 8	mov_sym_upper
;#(insn 9 8 10 check_cmp.c:67 (set (reg/f:SI 1 r1 [31])
;#        (ior:SI (reg/f:SI 1 r1 [31])
;#            (symbol_ref:SI ("check_bgt") [flags 0x3] <function_decl 0x7f1b59dcdd00 check_bgt>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("check_bgt") [flags 0x3] <function_decl 0x7f1b59dcdd00 check_bgt>)
;#        (nil)))
	symlo	$r1, check_bgt	;# 9	mov_sym_lower
;#(insn 10 9 85 check_cmp.c:67 (set (reg:SI 0 r0)
;#        (const_int 0 [0x0])) 2 {movsi_insn} (nil))
	movi	$r0, 0	;# 10	movsi_insn/1
;#(call_insn 12 10 11 check_cmp.c:67 (parallel [
;#            (call (mem:SI (reg/f:SI 1 r1 [31]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 1 r1 [31])
;#        (expr_list:REG_DEAD (reg:SI 0 r0)
;#            (expr_list:REG_EH_REGION (const_int 0 [0x0])
;#                (nil))))
;#    (expr_list:REG_DEP_TRUE (use (reg:SI 0 r0))
;#        (nil)))
	bl	$lr, $r1, 0	;# 12	call_expanded
;#(insn 11 12 13 (set (reg:SI 0 r0)
;#        (ior:SI (reg:SI 0 r0)
;#            (const_int 333 [0x14d]))) 30 {iorsi3} (expr_list:REG_EQUAL (const_int 333 [0x14d])
;#        (nil)))
	ori	$r0, $r0, 333	;# 11	iorsi3/2
;#(insn 13 85 14 check_cmp.c:68 (set (reg/f:SI 0 r0 [32])
;#        (lshiftrt:SI (symbol_ref:SI ("SI_r") <var_decl 0x7f1b59de0000 SI_r>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("SI_r") <var_decl 0x7f1b59de0000 SI_r>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, SI_r	;# 13	mov_sym_upper
;#(insn 14 13 15 check_cmp.c:68 (set (reg/f:SI 0 r0 [32])
;#        (ior:SI (reg/f:SI 0 r0 [32])
;#            (symbol_ref:SI ("SI_r") <var_decl 0x7f1b59de0000 SI_r>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("SI_r") <var_decl 0x7f1b59de0000 SI_r>)
;#        (nil)))
	symlo	$r0, SI_r	;# 14	mov_sym_lower
;#(insn 15 14 70 check_cmp.c:68 (set (reg:SI 1 r1 [orig:29 SI_r.19 ] [29])
;#        (mem/c/i:SI (reg/f:SI 0 r0 [32]) [0 SI_r+0 S4 A32])) 2 {movsi_insn} (expr_list:REG_DEAD (reg/f:SI 0 r0 [32])
;#        (nil)))
	lw	$r1, $r0, 0	;# 15	movsi_insn/4
;#(insn 70 15 71 check_cmp.c:68 (set (reg:SI 0 r0)
;#        (ashift:SI (const_int 0 [0x0])
;#            (const_int 16 [0x10]))) 27 {ashlsi3} (nil))
	movhi	$r0, 0	;# 70	ashlsi3/3
;#(insn 71 70 96 check_cmp.c:68 (set (reg:SI 0 r0)
;#        (ior:SI (reg:SI 0 r0)
;#            (const_int 333 [0x14d]))) 30 {iorsi3} (nil))
	ori	$r0, $r0, 333	;# 71	iorsi3/2
;#(jump_insn 16 71 18 check_cmp.c:68 (set (pc)
;#        (if_then_else (eq:SI (reg:SI 1 r1 [orig:29 SI_r.19 ] [29])
;#                (reg:SI 0 r0))
;#            (label_ref 28)
;#            (pc))) 12 {branch_eq} (expr_list:REG_BR_PRED (const_int 13 [0xd])
;#        (expr_list:REG_DEAD (reg:SI 0 r0)
;#            (expr_list:REG_BR_PROB (const_int 9996 [0x270c])
;#                (nil)))))
	beq	$r1, $r0, .L25 	;# 16	branch_eq/1
;#(insn 18 16 17 (set (reg/f:SI 2 r2 [33])
;#        (lshiftrt:SI (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1b59d8b000 printf>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1b59d8b000 printf>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r2, printf	;# 18	mov_sym_upper
;#(insn 19 17 20 check_cmp.c:68 (set (reg/f:SI 2 r2 [33])
;#        (ior:SI (reg/f:SI 2 r2 [33])
;#            (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1b59d8b000 printf>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("printf") [flags 0x41] <function_decl 0x7f1b59d8b000 printf>)
;#        (nil)))
	symlo	$r2, printf	;# 19	mov_sym_lower
;#(insn 20 19 87 check_cmp.c:68 (set (reg:SI 0 r0)
;#        (lshiftrt:SI (symbol_ref/f:SI ("*.LC0") [flags 0x2] <string_cst 0x7f1b59bff660>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (nil))
	symhi	$r0, .LC0	;# 20	mov_sym_upper
;#(call_insn 23 20 21 check_cmp.c:68 (parallel [
;#            (set (reg:SI 0 r0)
;#                (call (mem:SI (reg/f:SI 2 r2 [33]) [0 S4 A32])
;#                    (const_int 0 [0x0])))
;#            (clobber (reg:SI 22 lr))
;#        ]) 23 {*call_value_reg_c3p3u} (expr_list:REG_DEAD (reg/f:SI 2 r2 [33])
;#        (expr_list:REG_DEAD (reg:SI 1 r1)
;#            (expr_list:REG_UNUSED (reg:SI 0 r0)
;#                (nil))))
;#    (expr_list:REG_DEP_TRUE (use (reg:SI 1 r1))
;#        (expr_list:REG_DEP_TRUE (use (reg:SI 0 r0))
;#            (nil))))
	bl	$lr, $r2	;# 23	*call_value_reg_c3p3u
;#(insn 21 23 24 (set (reg:SI 0 r0)
;#        (ior:SI (reg:SI 0 r0)
;#            (symbol_ref/f:SI ("*.LC0") [flags 0x2] <string_cst 0x7f1b59bff660>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref/f:SI ("*.LC0") [flags 0x2] <string_cst 0x7f1b59bff660>)
;#        (nil)))
	symlo	$r0, .LC0	;# 21	mov_sym_lower
;#(insn 24 87 25 check_cmp.c:68 (set (reg/f:SI 0 r0 [34])
;#        (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, err	;# 24	mov_sym_upper
;#(insn 25 24 26 check_cmp.c:68 (set (reg/f:SI 0 r0 [34])
;#        (ior:SI (reg/f:SI 0 r0 [34])
;#            (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#        (nil)))
	symlo	$r0, err	;# 25	mov_sym_lower
;#(call_insn 26 25 27 check_cmp.c:68 (parallel [
;#            (call (mem:SI (reg/f:SI 0 r0 [34]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 0 r0 [34])
;#        (expr_list:REG_NORETURN (const_int 0 [0x0])
;#            (nil)))
;#    (nil))
	bl	$lr, $r0, 0
	nop	;# 26	call_expanded
.L25:
;#(insn 30 29 31 check_cmp.c:69 (set (reg/f:SI 1 r1 [35])
;#        (lshiftrt:SI (symbol_ref:SI ("check_bgtu") [flags 0x3] <function_decl 0x7f1b59dcdf00 check_bgtu>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("check_bgtu") [flags 0x3] <function_decl 0x7f1b59dcdf00 check_bgtu>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r1, check_bgtu	;# 30	mov_sym_upper
;#(insn 31 30 32 check_cmp.c:69 (set (reg/f:SI 1 r1 [35])
;#        (ior:SI (reg/f:SI 1 r1 [35])
;#            (symbol_ref:SI ("check_bgtu") [flags 0x3] <function_decl 0x7f1b59dcdf00 check_bgtu>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("check_bgtu") [flags 0x3] <function_decl 0x7f1b59dcdf00 check_bgtu>)
;#        (nil)))
	symlo	$r1, check_bgtu	;# 31	mov_sym_lower
;#(insn 32 31 89 check_cmp.c:69 (set (reg:SI 0 r0)
;#        (const_int 0 [0x0])) 2 {movsi_insn} (nil))
	movi	$r0, 0	;# 32	movsi_insn/1
;#(call_insn 34 32 33 check_cmp.c:69 (parallel [
;#            (call (mem:SI (reg/f:SI 1 r1 [35]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 1 r1 [35])
;#        (expr_list:REG_DEAD (reg:SI 0 r0)
;#            (expr_list:REG_EH_REGION (const_int 0 [0x0])
;#                (nil))))
;#    (expr_list:REG_DEP_TRUE (use (reg:SI 0 r0))
;#        (nil)))
	bl	$lr, $r1, 0	;# 34	call_expanded
;#(insn 33 34 35 (set (reg:SI 0 r0)
;#        (ior:SI (reg:SI 0 r0)
;#            (const_int 334 [0x14e]))) 30 {iorsi3} (expr_list:REG_EQUAL (const_int 334 [0x14e])
;#        (nil)))
	ori	$r0, $r0, 334	;# 33	iorsi3/2
;#(insn 35 89 36 check_cmp.c:70 (set (reg/f:SI 0 r0 [36])
;#        (lshiftrt:SI (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, UI_r	;# 35	mov_sym_upper
;#(insn 36 35 37 check_cmp.c:70 (set (reg/f:SI 0 r0 [36])
;#        (ior:SI (reg/f:SI 0 r0 [36])
;#            (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>)
;#        (nil)))
	symlo	$r0, UI_r	;# 36	mov_sym_lower
;#(insn 37 36 72 check_cmp.c:70 (set (reg:SI 0 r0 [orig:37 UI_r ] [37])
;#        (mem/c/i:SI (reg/f:SI 0 r0 [36]) [0 UI_r+0 S4 A32])) 2 {movsi_insn} (nil))
	lw	$r0, $r0, 0	;# 37	movsi_insn/4
;#(insn 72 37 73 check_cmp.c:70 (set (reg:SI 1 r1)
;#        (ashift:SI (const_int 0 [0x0])
;#            (const_int 16 [0x10]))) 27 {ashlsi3} (nil))
	movhi	$r1, 0	;# 72	ashlsi3/3
;#(insn 73 72 98 check_cmp.c:70 (set (reg:SI 1 r1)
;#        (ior:SI (reg:SI 1 r1)
;#            (const_int 334 [0x14e]))) 30 {iorsi3} (nil))
	ori	$r1, $r1, 334	;# 73	iorsi3/2
;#(jump_insn 38 73 46 check_cmp.c:70 (set (pc)
;#        (if_then_else (eq:SI (reg:SI 0 r0 [orig:37 UI_r ] [37])
;#                (reg:SI 1 r1))
;#            (label_ref 44)
;#            (pc))) 12 {branch_eq} (expr_list:REG_BR_PRED (const_int 13 [0xd])
;#        (expr_list:REG_DEAD (reg:SI 1 r1)
;#            (expr_list:REG_DEAD (reg:SI 0 r0 [orig:37 UI_r ] [37])
;#                (expr_list:REG_BR_PROB (const_int 9996 [0x270c])
;#                    (nil))))))
	beq	$r0, $r1, .L26 	;# 38	branch_eq/1
;#(insn/s 46 38 39 (set (reg/f:SI 1 r1 [39])
;#        (lshiftrt:SI (symbol_ref:SI ("check_bltu") [flags 0x3] <function_decl 0x7f1b59de3100 check_bltu>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("check_bltu") [flags 0x3] <function_decl 0x7f1b59de3100 check_bltu>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r1, check_bltu	;# 46	mov_sym_upper
;#(insn 40 39 41 check_cmp.c:70 (set (reg/f:SI 0 r0 [38])
;#        (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, err	;# 40	mov_sym_upper
;#(insn 41 40 42 check_cmp.c:70 (set (reg/f:SI 0 r0 [38])
;#        (ior:SI (reg/f:SI 0 r0 [38])
;#            (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#        (nil)))
	symlo	$r0, err	;# 41	mov_sym_lower
;#(call_insn 42 41 43 check_cmp.c:70 (parallel [
;#            (call (mem:SI (reg/f:SI 0 r0 [38]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 0 r0 [38])
;#        (expr_list:REG_NORETURN (const_int 0 [0x0])
;#            (nil)))
;#    (nil))
	bl	$lr, $r0, 0
	nop	;# 42	call_expanded
.L26:
;#(insn 47 45 48 check_cmp.c:71 (set (reg/f:SI 1 r1 [39])
;#        (ior:SI (reg/f:SI 1 r1 [39])
;#            (symbol_ref:SI ("check_bltu") [flags 0x3] <function_decl 0x7f1b59de3100 check_bltu>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("check_bltu") [flags 0x3] <function_decl 0x7f1b59de3100 check_bltu>)
;#        (nil)))
	symlo	$r1, check_bltu	;# 47	mov_sym_lower
;#(insn 48 47 91 check_cmp.c:71 (set (reg:SI 0 r0)
;#        (const_int 0 [0x0])) 2 {movsi_insn} (nil))
	movi	$r0, 0	;# 48	movsi_insn/1
;#(call_insn 50 48 49 check_cmp.c:71 (parallel [
;#            (call (mem:SI (reg/f:SI 1 r1 [39]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 1 r1 [39])
;#        (expr_list:REG_DEAD (reg:SI 0 r0)
;#            (expr_list:REG_EH_REGION (const_int 0 [0x0])
;#                (nil))))
;#    (expr_list:REG_DEP_TRUE (use (reg:SI 0 r0))
;#        (nil)))
	bl	$lr, $r1, 0	;# 50	call_expanded
;#(insn 49 50 51 (set (reg:SI 0 r0)
;#        (ior:SI (reg:SI 0 r0)
;#            (const_int 335 [0x14f]))) 30 {iorsi3} (expr_list:REG_EQUAL (const_int 335 [0x14f])
;#        (nil)))
	ori	$r0, $r0, 335	;# 49	iorsi3/2
;#(insn 51 91 52 check_cmp.c:72 (set (reg/f:SI 0 r0 [40])
;#        (lshiftrt:SI (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, UI_r	;# 51	mov_sym_upper
;#(insn 52 51 53 check_cmp.c:72 (set (reg/f:SI 0 r0 [40])
;#        (ior:SI (reg/f:SI 0 r0 [40])
;#            (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("UI_r") <var_decl 0x7f1b59de00a0 UI_r>)
;#        (nil)))
	symlo	$r0, UI_r	;# 52	mov_sym_lower
;#(insn 53 52 74 check_cmp.c:72 (set (reg:SI 0 r0 [orig:41 UI_r ] [41])
;#        (mem/c/i:SI (reg/f:SI 0 r0 [40]) [0 UI_r+0 S4 A32])) 2 {movsi_insn} (nil))
	lw	$r0, $r0, 0	;# 53	movsi_insn/4
;#(insn 74 53 75 check_cmp.c:72 (set (reg:SI 1 r1)
;#        (ashift:SI (const_int 0 [0x0])
;#            (const_int 16 [0x10]))) 27 {ashlsi3} (nil))
	movhi	$r1, 0	;# 74	ashlsi3/3
;#(insn 75 74 100 check_cmp.c:72 (set (reg:SI 1 r1)
;#        (ior:SI (reg:SI 1 r1)
;#            (const_int 335 [0x14f]))) 30 {iorsi3} (nil))
	ori	$r1, $r1, 335	;# 75	iorsi3/2
;#(jump_insn 54 75 80 check_cmp.c:72 (set (pc)
;#        (if_then_else (eq:SI (reg:SI 0 r0 [orig:41 UI_r ] [41])
;#                (reg:SI 1 r1))
;#            (label_ref:SI 65)
;#            (pc))) 12 {branch_eq} (expr_list:REG_BR_PRED (const_int 13 [0xd])
;#        (expr_list:REG_DEAD (reg:SI 1 r1)
;#            (expr_list:REG_DEAD (reg:SI 0 r0 [orig:41 UI_r ] [41])
;#                (expr_list:REG_BR_PROB (const_int 9996 [0x270c])
;#                    (nil))))))
	beq	$r0, $r1, .L28 	;# 54	branch_eq/1
;#(insn/s 80 54 55 (set (reg:SI 22 lr [0])
;#        (mem:SI (reg/f:SI 23 sp) [0 S4 A32])) 2 {movsi_insn} (nil))
	lw	$lr, $sp, 0	;# 80	movsi_insn/4
;#(insn 56 55 57 check_cmp.c:72 (set (reg/f:SI 0 r0 [42])
;#        (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))) 25 {mov_sym_upper} (expr_list:REG_EQUAL (lshiftrt:SI (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#            (const_int 16 [0x10]))
;#        (nil)))
	symhi	$r0, err	;# 56	mov_sym_upper
;#(insn 57 56 58 check_cmp.c:72 (set (reg/f:SI 0 r0 [42])
;#        (ior:SI (reg/f:SI 0 r0 [42])
;#            (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>))) 26 {mov_sym_lower} (expr_list:REG_EQUAL (symbol_ref:SI ("err") [flags 0x41] <function_decl 0x7f1b59dcdc00 err>)
;#        (nil)))
	symlo	$r0, err	;# 57	mov_sym_lower
;#(call_insn 58 57 59 check_cmp.c:72 (parallel [
;#            (call (mem:SI (reg/f:SI 0 r0 [42]) [0 S4 A32])
;#                (const_int 0 [0x0]))
;#            (clobber (reg:SI 22 lr))
;#        ]) 22 {call_expanded} (expr_list:REG_DEAD (reg/f:SI 0 r0 [42])
;#        (expr_list:REG_NORETURN (const_int 0 [0x0])
;#            (nil)))
;#    (nil))
	bl	$lr, $r0, 0
	nop	;# 58	call_expanded
.L28:
;#(jump_insn 82 79 81 check_cmp.c:73 (parallel [
;#            (use (reg:SI 22 lr [0]))
;#            (return)
;#        ]) 19 {return_expanded} (expr_list:REG_BR_PRED (const_int 12 [0xc])
;#        (expr_list:REG_DEAD (reg:SI 22 lr [0])
;#            (nil))))
	j	$lr 	;# 82	return_expanded
;#(insn/f 81 82 94 (set (reg/f:SI 23 sp)
;#        (plus:SI (reg/f:SI 23 sp)
;#            (const_int 4 [0x4]))) 0 {addsi3} (nil))
	addi	$sp, $sp, 4	;# 81	addsi3/3
	.size	check_cmp, .-check_cmp
	.comm	SI_r,4,4
	.comm	UI_r,4,4
	.ident	"GCC: (GNU) 4.4.3"
