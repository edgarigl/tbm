	.file	"check_movhi.c"
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.text
.Ltext0:
	.align	4
	.global	check_movhi
	.type	check_movhi, @function
check_movhi:
.LFB0:
.LM1:
	subi	$sp, $sp, 4
	sw	$lr, $sp, 0
.LCFI0:
.LM2:
	symhi	$r1, puts
	symlo	$r1, puts
	symhi	$r0, __func__.1215
	bl	$lr, $r1
	symlo	$r0, __func__.1215
.LM3:
	symhi	$r0, cmb
	symlo	$r0, cmb
	movhi	$r1, 4660
	ori	$r1, $r1, 22136
	sw	$r1, $r0, 0
.LM4:
	lh	$r1, $r0, 0
	movhi	$r1, $r1
	asri	$r1, $r1, 16
	movhi	$r2, 0
	ori	$r2, $r2, 22136
	bne	$r1, $r2, .L7 
	nop
.LM5:
	lh	$r1, $r0, 2
	movhi	$r1, $r1
	asri	$r1, $r1, 16
	movhi	$r2, 0
	ori	$r2, $r2, 4660
	bne	$r1, $r2, .L7 
	movi	$r1, 30806
.LM6:
	sh	$r1, $r0, 0
.LM7:
	movi	$r1, 13330
	sh	$r1, $r0, 2
.LM8:
	lw	$r0, $r0, 0
	movhi	$r1, 13330
	ori	$r1, $r1, 30806
	bne	$r0, $r1, .L7 
	lw	$lr, $sp, 0
.LM9:
.LCFI1:
	j	$lr 
	addi	$sp, $sp, 4
.L7:
.LM10:
	symhi	$r0, err
	symlo	$r0, err
	bl	$lr, $r0, 0
	nop
.LFE0:
	.size	check_movhi, .-check_movhi
	.local	cmb
	.comm	cmb,4,4
	.section	.rodata
	.align	4
	.type	__func__.1215, @object
	.size	__func__.1215, 12
__func__.1215:
	.string	"check_movhi"
	.section	.debug_frame,"",@progbits
.Lframe0:
	.4byte	.LECIE0-.LSCIE0
.LSCIE0:
	.4byte	0xffffffff
	.byte	0x1
	.string	""
	.uleb128 0x1
	.sleb128 -4
	.byte	0x18
	.byte	0xc
	.uleb128 0x17
	.uleb128 0x0
	.byte	0x9
	.uleb128 0x18
	.uleb128 0x16
	.align	4
.LECIE0:
.LSFDE0:
	.4byte	.LEFDE0-.LASFDE0
.LASFDE0:
	.4byte	.Lframe0
	.4byte	.LFB0
	.4byte	.LFE0-.LFB0
	.byte	0x4
	.4byte	.LCFI0-.LFB0
	.byte	0x96
	.uleb128 0x0
	.byte	0x4
	.4byte	.LCFI1-.LCFI0
	.byte	0x13
	.sleb128 1
	.align	4
.LEFDE0:
	.section	.text
.Letext0:
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST0:
	.4byte	.LFB0-.Ltext0
	.4byte	.LCFI1-.Ltext0
	.2byte	0x1
	.byte	0x67
	.4byte	.LCFI1-.Ltext0
	.4byte	.LFE0-.Ltext0
	.2byte	0x2
	.byte	0x87
	.sleb128 -4
	.4byte	0x0
	.4byte	0x0
	.section	.debug_info
	.4byte	0xd3
	.2byte	0x2
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF2
	.byte	0x1
	.4byte	.LASF3
	.4byte	.LASF4
	.4byte	.Ltext0
	.4byte	.Letext0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.byte	0x4
	.byte	0x1
	.byte	0x3
	.4byte	0x49
	.uleb128 0x3
	.string	"b"
	.byte	0x1
	.byte	0x4
	.4byte	0x49
	.uleb128 0x3
	.string	"w"
	.byte	0x1
	.byte	0x5
	.4byte	0x63
	.uleb128 0x3
	.string	"i"
	.byte	0x1
	.byte	0x6
	.4byte	0x7a
	.byte	0x0
	.uleb128 0x4
	.4byte	0x5c
	.4byte	0x59
	.uleb128 0x5
	.4byte	0x59
	.byte	0x3
	.byte	0x0
	.uleb128 0x6
	.byte	0x4
	.byte	0x7
	.uleb128 0x7
	.byte	0x1
	.byte	0x6
	.4byte	.LASF0
	.uleb128 0x4
	.4byte	0x73
	.4byte	0x73
	.uleb128 0x5
	.4byte	0x59
	.byte	0x1
	.byte	0x0
	.uleb128 0x7
	.byte	0x2
	.byte	0x5
	.4byte	.LASF1
	.uleb128 0x8
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x9
	.byte	0x1
	.4byte	.LASF5
	.byte	0x1
	.byte	0x9
	.byte	0x1
	.4byte	.LFB0
	.4byte	.LFE0
	.4byte	.LLST0
	.4byte	0xab
	.uleb128 0xa
	.4byte	.LASF6
	.4byte	0xbb
	.byte	0x1
	.byte	0x5
	.byte	0x3
	.4byte	__func__.1215
	.byte	0x0
	.uleb128 0x4
	.4byte	0x5c
	.4byte	0xbb
	.uleb128 0x5
	.4byte	0x59
	.byte	0xb
	.byte	0x0
	.uleb128 0xb
	.4byte	0xab
	.uleb128 0xc
	.string	"cmb"
	.byte	0x1
	.byte	0x7
	.4byte	0xd1
	.byte	0x5
	.byte	0x3
	.4byte	cmb
	.uleb128 0xd
	.4byte	0x25
	.byte	0x0
	.section	.debug_abbrev
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x6
	.byte	0x0
	.byte	0x0
	.uleb128 0x2
	.uleb128 0x17
	.byte	0x1
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xd
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x4
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0x5
	.uleb128 0x21
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x6
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.byte	0x0
	.byte	0x0
	.uleb128 0x7
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0x0
	.byte	0x0
	.uleb128 0x8
	.uleb128 0x24
	.byte	0x0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0x0
	.byte	0x0
	.uleb128 0x9
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0xc
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0xc
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x1
	.uleb128 0x40
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xa
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x34
	.uleb128 0xc
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0xb
	.uleb128 0x26
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.uleb128 0xc
	.uleb128 0x34
	.byte	0x0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0xa
	.byte	0x0
	.byte	0x0
	.uleb128 0xd
	.uleb128 0x35
	.byte	0x0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.section	.debug_pubnames,"",@progbits
	.4byte	0x1e
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0xd7
	.4byte	0x81
	.string	"check_movhi"
	.4byte	0x0
	.section	.debug_aranges,"",@progbits
	.4byte	0x1c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0x0
	.2byte	0x0
	.2byte	0x0
	.4byte	.Ltext0
	.4byte	.Letext0-.Ltext0
	.4byte	0x0
	.4byte	0x0
	.section	.debug_line
	.4byte	.LELT0-.LSLT0
.LSLT0:
	.2byte	0x2
	.4byte	.LELTP0-.LASLTP0
.LASLTP0:
	.byte	0x1
	.byte	0x1
	.byte	0xf6
	.byte	0xf5
	.byte	0xa
	.byte	0x0
	.byte	0x1
	.byte	0x1
	.byte	0x1
	.byte	0x1
	.byte	0x0
	.byte	0x0
	.byte	0x0
	.byte	0x1
	.byte	0x0
	.string	"check_movhi.c"
	.uleb128 0x0
	.uleb128 0x0
	.uleb128 0x0
	.byte	0x0
.LELTP0:
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.4byte	.LM1
	.byte	0x1d
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.4byte	.LM2
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.4byte	.LM3
	.byte	0x16
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.4byte	.LM4
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.4byte	.LM5
	.byte	0x16
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.4byte	.LM6
	.byte	0x18
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.4byte	.LM7
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.4byte	.LM8
	.byte	0x15
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.4byte	.LM9
	.byte	0x17
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.4byte	.LM10
	.byte	0x12
	.byte	0x0
	.uleb128 0x5
	.byte	0x2
	.4byte	.Letext0
	.byte	0x0
	.uleb128 0x1
	.byte	0x1
.LELT0:
	.section	.debug_str,"MS",@progbits,1
.LASF6:
	.string	"__func__"
.LASF1:
	.string	"short int"
.LASF0:
	.string	"char"
.LASF3:
	.string	"check_movhi.c"
.LASF5:
	.string	"check_movhi"
.LASF4:
	.string	"/home/edgar/src/verilog/ise/soc/c3p3u/sw"
.LASF2:
	.string	"GNU C 4.4.3"
	.ident	"GCC: (GNU) 4.4.3"
