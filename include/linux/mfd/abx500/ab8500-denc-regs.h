/*
 * Copyright (C) ST-Ericsson SA 2010
 *
 * ST-Ericsson AB8500 DENC related registers
 *
 * Author: Marcus Tunnissen <marcel.tuennissen@stericsson.com>
 * for ST-Ericsson.
 *
 * License terms: GNU General Public License (GPL), version 2.
 */

#ifndef __AB8500_DENC_H
#define __AB8500_DENC_H

#define AB8500_VAL2REG(__reg, __fld, __val) \
	(((__val) << __reg##_##__fld##_SHIFT) & __reg##_##__fld##_MASK)
#define AB8500_REG2VAL(__reg, __fld, __val) \
	(((__val) & __reg##_##__fld##_MASK) >> __reg##_##__fld##_SHIFT)

#define AB8500_CTRL3 0x00000200
#define AB8500_CTRL3_TH_SD_ENA_SHIFT 3
#define AB8500_CTRL3_TH_SD_ENA_MASK 0x00000008
#define AB8500_CTRL3_TH_SD_ENA(__x) \
	AB8500_VAL2REG(AB8500_CTRL3, TH_SD_ENA, __x)
#define AB8500_CTRL3_RESET_DENC_N_SHIFT 2
#define AB8500_CTRL3_RESET_DENC_N_MASK 0x00000004
#define AB8500_CTRL3_RESET_DENC_N(__x) \
	AB8500_VAL2REG(AB8500_CTRL3, RESET_DENC_N, __x)
#define AB8500_CTRL3_RESET_AUD_N_SHIFT 1
#define AB8500_CTRL3_RESET_AUD_N_MASK 0x00000002
#define AB8500_CTRL3_RESET_AUD_N(__x) \
	AB8500_VAL2REG(AB8500_CTRL3, RESET_AUD_N, __x)
#define AB8500_CTRL3_CLK_32K_OUT2_IS_SHIFT 0
#define AB8500_CTRL3_CLK_32K_OUT2_IS_MASK 0x00000001
#define AB8500_CTRL3_CLK_32K_OUT2_IS(__x) \
	AB8500_VAL2REG(AB8500_CTRL3, CLK_32K_OUT2_IS, __x)
#define AB8500_SYS_ULP_CLK_CONF 0x0000020A
#define AB8500_SYS_ULP_CLK_CONF_CLK_27MHZ_PD_ENA_SHIFT 7
#define AB8500_SYS_ULP_CLK_CONF_CLK_27MHZ_PD_ENA_MASK 0x00000080
#define AB8500_SYS_ULP_CLK_CONF_CLK_27MHZ_PD_ENA(__x) \
	AB8500_VAL2REG(AB8500_SYS_ULP_CLK_CONF, CLK_27MHZ_PD_ENA, __x)
#define AB8500_SYS_ULP_CLK_CONF_CLK_27MHZ_BUF_ENA_SHIFT 6
#define AB8500_SYS_ULP_CLK_CONF_CLK_27MHZ_BUF_ENA_MASK 0x00000040
#define AB8500_SYS_ULP_CLK_CONF_CLK_27MHZ_BUF_ENA(__x) \
	AB8500_VAL2REG(AB8500_SYS_ULP_CLK_CONF, CLK_27MHZ_BUF_ENA, __x)
#define AB8500_SYS_ULP_CLK_CONF_ULP_CLK_STRE_SHIFT 5
#define AB8500_SYS_ULP_CLK_CONF_ULP_CLK_STRE_MASK 0x00000020
#define AB8500_SYS_ULP_CLK_CONF_ULP_CLK_STRE(__x) \
	AB8500_VAL2REG(AB8500_SYS_ULP_CLK_CONF, ULP_CLK_STRE, __x)
#define AB8500_SYS_ULP_CLK_CONF_TVOUT_CLK_INV_SHIFT 4
#define AB8500_SYS_ULP_CLK_CONF_TVOUT_CLK_INV_MASK 0x00000010
#define AB8500_SYS_ULP_CLK_CONF_TVOUT_CLK_INV(__x) \
	AB8500_VAL2REG(AB8500_SYS_ULP_CLK_CONF, TVOUT_CLK_INV, __x)
#define AB8500_SYS_ULP_CLK_CONF_TVOUT_CLK_DE_IN_SHIFT 3
#define AB8500_SYS_ULP_CLK_CONF_TVOUT_CLK_DE_IN_MASK 0x00000008
#define AB8500_SYS_ULP_CLK_CONF_TVOUT_CLK_DE_IN(__x) \
	AB8500_VAL2REG(AB8500_SYS_ULP_CLK_CONF, TVOUT_CLK_DE_IN, __x)
#define AB8500_SYS_ULP_CLK_CONF_CLK_27MHZ_STRE_SHIFT 2
#define AB8500_SYS_ULP_CLK_CONF_CLK_27MHZ_STRE_MASK 0x00000004
#define AB8500_SYS_ULP_CLK_CONF_CLK_27MHZ_STRE(__x) \
	AB8500_VAL2REG(AB8500_SYS_ULP_CLK_CONF, CLK_27MHZ_STRE, __x)
#define AB8500_SYS_ULP_CLK_CONF_ULP_CLK_CONF_SHIFT 0
#define AB8500_SYS_ULP_CLK_CONF_ULP_CLK_CONF_MASK 0x00000003
#define AB8500_SYS_ULP_CLK_CONF_ULP_CLK_CONF_NO_FUNC 0
#define AB8500_SYS_ULP_CLK_CONF_ULP_CLK_CONF_AS_OUTPUT 1
#define AB8500_SYS_ULP_CLK_CONF_ULP_CLK_CONF_AS_INPUT 2
#define AB8500_SYS_ULP_CLK_CONF_ULP_CLK_CONF(__x) \
	AB8500_VAL2REG(AB8500_SYS_ULP_CLK_CONF, ULP_CLK_CONF, \
	AB8500_SYS_ULP_CLK_CONF_ULP_CLK_CONF_##__x)
#define AB8500_SYS_CLK_CTRL 0x0000020C
#define AB8500_SYS_CLK_CTRL_USB_CLK_VALID_SHIFT 2
#define AB8500_SYS_CLK_CTRL_USB_CLK_VALID_MASK 0x00000004
#define AB8500_SYS_CLK_CTRL_USB_CLK_VALID(__x) \
	AB8500_VAL2REG(AB8500_SYS_CLK_CTRL, USB_CLK_VALID, __x)
#define AB8500_SYS_CLK_CTRL_TVOUT_CLK_VALID_SHIFT 1
#define AB8500_SYS_CLK_CTRL_TVOUT_CLK_VALID_MASK 0x00000002
#define AB8500_SYS_CLK_CTRL_TVOUT_CLK_VALID(__x) \
	AB8500_VAL2REG(AB8500_SYS_CLK_CTRL, TVOUT_CLK_VALID, __x)
#define AB8500_SYS_CLK_CTRL_TVOUT_PLL_ENA_SHIFT 0
#define AB8500_SYS_CLK_CTRL_TVOUT_PLL_ENA_MASK 0x00000001
#define AB8500_SYS_CLK_CTRL_TVOUT_PLL_ENA(__x) \
	AB8500_VAL2REG(AB8500_SYS_CLK_CTRL, TVOUT_PLL_ENA, __x)
#define AB8500_REGU_MISC1 0x00000380
#define AB8500_REGU_MISC1_V_TVOUT_LP_SHIFT 7
#define AB8500_REGU_MISC1_V_TVOUT_LP_MASK 0x00000080
#define AB8500_REGU_MISC1_V_TVOUT_LP(__x) \
	AB8500_VAL2REG(AB8500_REGU_MISC1, V_TVOUT_LP, __x)
#define AB8500_REGU_MISC1_V_INT_CORE_12_LP_SHIFT 6
#define AB8500_REGU_MISC1_V_INT_CORE_12_LP_MASK 0x00000040
#define AB8500_REGU_MISC1_V_INT_CORE_12_LP(__x) \
	AB8500_VAL2REG(AB8500_REGU_MISC1, V_INT_CORE_12_LP, __x)
#define AB8500_REGU_MISC1_V_INT_CORE_12_SEL_SHIFT 3
#define AB8500_REGU_MISC1_V_INT_CORE_12_SEL_MASK 0x00000038
#define AB8500_REGU_MISC1_V_INT_CORE_12_SEL_1_2V 0
#define AB8500_REGU_MISC1_V_INT_CORE_12_SEL_1_225V 1
#define AB8500_REGU_MISC1_V_INT_CORE_12_SEL_1_25V 2
#define AB8500_REGU_MISC1_V_INT_CORE_12_SEL_1_275V 3
#define AB8500_REGU_MISC1_V_INT_CORE_12_SEL_1_3V 4
#define AB8500_REGU_MISC1_V_INT_CORE_12_SEL_1_325V 5
#define AB8500_REGU_MISC1_V_INT_CORE_12_SEL_1_35V 6
#define AB8500_REGU_MISC1_V_INT_CORE_12_SEL(__x) \
	AB8500_VAL2REG(AB8500_REGU_MISC1, V_INT_CORE_12_SEL, \
	AB8500_REGU_MISC1_V_INT_CORE_12_SEL_##__x)
#define AB8500_REGU_MISC1_V_INT_CORE_12_ENA_SHIFT 2
#define AB8500_REGU_MISC1_V_INT_CORE_12_ENA_MASK 0x00000004
#define AB8500_REGU_MISC1_V_INT_CORE_12_ENA(__x) \
	AB8500_VAL2REG(AB8500_REGU_MISC1, V_INT_CORE_12_ENA, __x)
#define AB8500_REGU_MISC1_V_TVOUT_ENA_SHIFT 1
#define AB8500_REGU_MISC1_V_TVOUT_ENA_MASK 0x00000002
#define AB8500_REGU_MISC1_V_TVOUT_ENA(__x) \
	AB8500_VAL2REG(AB8500_REGU_MISC1, V_TVOUT_ENA, __x)
#define AB8500_VAUX12_REGU 0x00000409
#define AB8500_VAUX12_REGU_VAUX_1_SHIFT 2
#define AB8500_VAUX12_REGU_VAUX_1_MASK 0x0000000C
#define AB8500_VAUX12_REGU_VAUX_1_DISABLE 0
#define AB8500_VAUX12_REGU_VAUX_1_FORCE_HP 1
#define AB8500_VAUX12_REGU_VAUX_1_BY_CTRL_REG 2
#define AB8500_VAUX12_REGU_VAUX_1_FORCE_LP 3
#define AB8500_VAUX12_REGU_VAUX_1(__x) \
	AB8500_VAL2REG(AB8500_VAUX12_REGU, VAUX_1, \
	AB8500_VAUX12_REGU_VAUX_1_##__x)
#define AB8500_VAUX12_REGU_VAUX_2_SHIFT 0
#define AB8500_VAUX12_REGU_VAUX_2_MASK 0x00000003
#define AB8500_VAUX12_REGU_VAUX_2_DISABLE 0
#define AB8500_VAUX12_REGU_VAUX_2_FORCE_HP 1
#define AB8500_VAUX12_REGU_VAUX_2_BY_CTRL_REG 2
#define AB8500_VAUX12_REGU_VAUX_2_FORCE_LP 3
#define AB8500_VAUX12_REGU_VAUX_2(__x) \
	AB8500_VAL2REG(AB8500_VAUX12_REGU, VAUX_2, \
	AB8500_VAUX12_REGU_VAUX_2_##__x)
#define AB8500_VAUX1_SEL 0x0000041F
#define AB8500_VAUX1_SEL_VAL_SHIFT 0
#define AB8500_VAUX1_SEL_VAL_MASK 0x0000000F
#define AB8500_VAUX1_SEL_VAL_1_1V 0
#define AB8500_VAUX1_SEL_VAL_1_2V 1
#define AB8500_VAUX1_SEL_VAL_1_3V 2
#define AB8500_VAUX1_SEL_VAL_1_4V 3
#define AB8500_VAUX1_SEL_VAL_1_5V 4
#define AB8500_VAUX1_SEL_VAL_1_8V 5
#define AB8500_VAUX1_SEL_VAL_1_85V 6
#define AB8500_VAUX1_SEL_VAL_1_9V 7
#define AB8500_VAUX1_SEL_VAL_2_5V 8
#define AB8500_VAUX1_SEL_VAL_2_65V 9
#define AB8500_VAUX1_SEL_VAL_2_7V 10
#define AB8500_VAUX1_SEL_VAL_2_75V 11
#define AB8500_VAUX1_SEL_VAL_2_8V 12
#define AB8500_VAUX1_SEL_VAL_2_9V 13
#define AB8500_VAUX1_SEL_VAL_3_0V 14
#define AB8500_VAUX1_SEL_VAL_3_3V 15
#define AB8500_VAUX1_SEL_VAL(__x) \
	AB8500_VAL2REG(AB8500_VAUX1_SEL, VAL, AB8500_VAUX1_SEL_VAL_##__x)
#define AB8500_DENC_CONF0 0x00000600
#define AB8500_DENC_CONF0_STD_SHIFT 6
#define AB8500_DENC_CONF0_STD_MASK 0x000000C0
#define AB8500_DENC_CONF0_STD_PAL_BDGHI 0
#define AB8500_DENC_CONF0_STD_PAL_N 1
#define AB8500_DENC_CONF0_STD_NTSC_M 2
#define AB8500_DENC_CONF0_STD_PAL_M 3
#define AB8500_DENC_CONF0_STD(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF0, STD, AB8500_DENC_CONF0_STD_##__x)
#define AB8500_DENC_CONF0_SYNC_SHIFT 3
#define AB8500_DENC_CONF0_SYNC_MASK 0x00000038
#define AB8500_DENC_CONF0_SYNC_F_BASED_SLAVE 1
#define AB8500_DENC_CONF0_SYNC_AUTO_TEST 7
#define AB8500_DENC_CONF0_SYNC(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF0, SYNC, AB8500_DENC_CONF0_SYNC_##__x)
#define AB8500_DENC_CONF1 0x00000601
#define AB8500_DENC_CONF1_BLK_LI_SHIFT 7
#define AB8500_DENC_CONF1_BLK_LI_MASK 0x00000080
#define AB8500_DENC_CONF1_BLK_LI_PARTIAL 0
#define AB8500_DENC_CONF1_BLK_LI_FULL 1
#define AB8500_DENC_CONF1_BLK_LI(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF1, BLK_LI, \
	AB8500_DENC_CONF1_BLK_LI_##__x)
#define AB8500_DENC_CONF1_FLT_SHIFT 5
#define AB8500_DENC_CONF1_FLT_MASK 0x00000060
#define AB8500_DENC_CONF1_FLT_1_1MHZ 0
#define AB8500_DENC_CONF1_FLT_1_3MHZ 1
#define AB8500_DENC_CONF1_FLT_1_6MHZ 2
#define AB8500_DENC_CONF1_FLT_1_9MHZ 3
#define AB8500_DENC_CONF1_FLT(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF1, FLT, AB8500_DENC_CONF1_FLT_##__x)
#define AB8500_DENC_CONF1_CO_KI_SHIFT 3
#define AB8500_DENC_CONF1_CO_KI_MASK 0x00000008
#define AB8500_DENC_CONF1_CO_KI(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF1, CO_KI, __x)
#define AB8500_DENC_CONF1_SETUP_MAIN_SHIFT 2
#define AB8500_DENC_CONF1_SETUP_MAIN_MASK 0x00000004
#define AB8500_DENC_CONF1_SETUP_MAIN_BLACK_EQ_BLANK 0
#define AB8500_DENC_CONF1_SETUP_MAIN_BLACK_GT_BLANK 1
#define AB8500_DENC_CONF1_SETUP_MAIN(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF1, SETUP_MAIN, \
	AB8500_DENC_CONF1_SETUP_MAIN_##__x)
#define AB8500_DENC_CONF1_CC_SHIFT 0
#define AB8500_DENC_CONF1_CC_MASK 0x00000003
#define AB8500_DENC_CONF1_CC_NONE 0
#define AB8500_DENC_CONF1_CC_FIELD_1 1
#define AB8500_DENC_CONF1_CC_FIELD_2 2
#define AB8500_DENC_CONF1_CC_ALL 3
#define AB8500_DENC_CONF1_CC(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF1, CC, AB8500_DENC_CONF1_CC_##__x)
#define AB8500_DENC_CONF2 0x00000602
#define AB8500_DENC_CONF2_N_INTRL_SHIFT 7
#define AB8500_DENC_CONF2_N_INTRL_MASK 0x00000080
#define AB8500_DENC_CONF2_N_INTRL(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF2, N_INTRL, __x)
#define AB8500_DENC_CONF2_EN_RST_SHIFT 6
#define AB8500_DENC_CONF2_EN_RST_MASK 0x00000040
#define AB8500_DENC_CONF2_EN_RST(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF2, EN_RST, __x)
#define AB8500_DENC_CONF2_BURST_EN_SHIFT 5
#define AB8500_DENC_CONF2_BURST_EN_MASK 0x00000020
#define AB8500_DENC_CONF2_BURST_EN(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF2, BURST_EN, __x)
#define AB8500_DENC_CONF2_SEL_RST_SHIFT 4
#define AB8500_DENC_CONF2_SEL_RST_MASK 0x00000010
#define AB8500_DENC_CONF2_SEL_RST_USE_HW_VAL 0
#define AB8500_DENC_CONF2_SEL_RST_USE_PROG_VAL 1
#define AB8500_DENC_CONF2_SEL_RST(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF2, SEL_RST, \
	AB8500_DENC_CONF2_SEL_RST_##__x)
#define AB8500_DENC_CONF2_RST_OSC_BUF_SHIFT 2
#define AB8500_DENC_CONF2_RST_OSC_BUF_MASK 0x00000004
#define AB8500_DENC_CONF2_RST_OSC_BUF(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF2, RST_OSC_BUF, __x)
#define AB8500_DENC_CONF2_VAL_RST_SHIFT 0
#define AB8500_DENC_CONF2_VAL_RST_MASK 0x00000003
#define AB8500_DENC_CONF2_VAL_RST_ALL_LINES 0
#define AB8500_DENC_CONF2_VAL_RST_EVERY_2ND_FIELD 1
#define AB8500_DENC_CONF2_VAL_RST_EVERY_4TH_FIELD 2
#define AB8500_DENC_CONF2_VAL_RST_EVERY_8TH_FIELD 3
#define AB8500_DENC_CONF2_VAL_RST(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF2, VAL_RST, \
	AB8500_DENC_CONF2_VAL_RST_##__x)
#define AB8500_DENC_CONF6 0x00000606
#define AB8500_DENC_CONF6_SOFT_RESET_SHIFT 7
#define AB8500_DENC_CONF6_SOFT_RESET_MASK 0x00000080
#define AB8500_DENC_CONF6_SOFT_RESET(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF6, SOFT_RESET, __x)
#define AB8500_DENC_CONF6_JUMP_SHIFT 6
#define AB8500_DENC_CONF6_JUMP_MASK 0x00000040
#define AB8500_DENC_CONF6_JUMP(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF6, JUMP, __x)
#define AB8500_DENC_CONF6_DEC_NINC_SHIFT 5
#define AB8500_DENC_CONF6_DEC_NINC_MASK 0x00000020
#define AB8500_DENC_CONF6_DEC_NINC(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF6, DEC_NINC, __x)
#define AB8500_DENC_CONF6_FREE_JUMP_SHIFT 4
#define AB8500_DENC_CONF6_FREE_JUMP_MASK 0x00000010
#define AB8500_DENC_CONF6_FREE_JUMP(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF6, FREE_JUMP, __x)
#define AB8500_DENC_CONF6_MAX_DYN_SHIFT 0
#define AB8500_DENC_CONF6_MAX_DYN_MASK 0x00000001
#define AB8500_DENC_CONF6_MAX_DYN(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF6, MAX_DYN, __x)
#define AB8500_DENC_CONF8 0x00000608
#define AB8500_DENC_CONF8_PH_RST_MODE_SHIFT 6
#define AB8500_DENC_CONF8_PH_RST_MODE_MASK 0x000000C0
#define AB8500_DENC_CONF8_PH_RST_MODE_DISABLED 0
#define AB8500_DENC_CONF8_PH_RST_MODE_UPDATE_FROM_PHASE_BUF 1
#define AB8500_DENC_CONF8_PH_RST_MODE_UPDATE_FROM_INC_DFS 2
#define AB8500_DENC_CONF8_PH_RST_MODE_RESET 3
#define AB8500_DENC_CONF8_PH_RST_MODE(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF8, PH_RST_MODE, \
	AB8500_DENC_CONF8_PH_RST_MODE_##__x)
#define AB8500_DENC_CONF8_VAL_422_MUX_SHIFT 4
#define AB8500_DENC_CONF8_VAL_422_MUX_MASK 0x00000010
#define AB8500_DENC_CONF8_VAL_422_MUX_TEST 0
#define AB8500_DENC_CONF8_VAL_422_MUX_ACTIVE 1
#define AB8500_DENC_CONF8_VAL_422_MUX(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF8, VAL_422_MUX, \
	AB8500_DENC_CONF8_VAL_422_MUX_##__x)
#define AB8500_DENC_CONF8_BLK_ALL_SHIFT 3
#define AB8500_DENC_CONF8_BLK_ALL_MASK 0x00000008
#define AB8500_DENC_CONF8_BLK_ALL(__x) \
	AB8500_VAL2REG(AB8500_DENC_CONF8, BLK_ALL, __x)
#define AB8500_TVOUT_CTRL 0x00000680
#define AB8500_TVOUT_CTRL_TV_LOAD_RC_SHIFT 6
#define AB8500_TVOUT_CTRL_TV_LOAD_RC_MASK 0x00000040
#define AB8500_TVOUT_CTRL_TV_LOAD_RC(__x) \
	AB8500_VAL2REG(AB8500_TVOUT_CTRL, TV_LOAD_RC, __x)
#define AB8500_TVOUT_CTRL_PLUG_TV_TIME_SHIFT 3
#define AB8500_TVOUT_CTRL_PLUG_TV_TIME_MASK 0x00000038
#define AB8500_TVOUT_CTRL_PLUG_TV_TIME_0_5S 0
#define AB8500_TVOUT_CTRL_PLUG_TV_TIME_1S 0
#define AB8500_TVOUT_CTRL_PLUG_TV_TIME_1_5S 0
#define AB8500_TVOUT_CTRL_PLUG_TV_TIME_2S 0
#define AB8500_TVOUT_CTRL_PLUG_TV_TIME_2_5S 0
#define AB8500_TVOUT_CTRL_PLUG_TV_TIME_3S 0
#define AB8500_TVOUT_CTRL_PLUG_TV_TIME(__x) \
	AB8500_VAL2REG(AB8500_TVOUT_CTRL, PLUG_TV_TIME, \
	AB8500_TVOUT_CTRL_PLUG_TV_TIME_##__x)
#define AB8500_TVOUT_CTRL_TV_PLUG_ON_SHIFT 2
#define AB8500_TVOUT_CTRL_TV_PLUG_ON_MASK 0x00000004
#define AB8500_TVOUT_CTRL_TV_PLUG_ON(__x) \
	AB8500_VAL2REG(AB8500_TVOUT_CTRL, TV_PLUG_ON, __x)
#define AB8500_TVOUT_CTRL_DAC_CTRL0_SHIFT 1
#define AB8500_TVOUT_CTRL_DAC_CTRL0_MASK 0x00000002
#define AB8500_TVOUT_CTRL_DAC_CTRL0(__x) \
	AB8500_VAL2REG(AB8500_TVOUT_CTRL, DAC_CTRL0, __x)
#define AB8500_TVOUT_CTRL_DAC_CTRL1_SHIFT 0
#define AB8500_TVOUT_CTRL_DAC_CTRL1_MASK 0x00000001
#define AB8500_TVOUT_CTRL_DAC_CTRL1(__x) \
	AB8500_VAL2REG(AB8500_TVOUT_CTRL, DAC_CTRL1, __x)
#define AB8500_TVOUT_CTRL2 0x00000681
#define AB8500_TVOUT_CTRL2_SWAP_DDR_DATA_IN_SHIFT 1
#define AB8500_TVOUT_CTRL2_SWAP_DDR_DATA_IN_MASK 0x00000002
#define AB8500_TVOUT_CTRL2_SWAP_DDR_DATA_IN(__x) \
	AB8500_VAL2REG(AB8500_TVOUT_CTRL2, SWAP_DDR_DATA_IN, __x)
#define AB8500_TVOUT_CTRL2_DENC_DDR_SHIFT 0
#define AB8500_TVOUT_CTRL2_DENC_DDR_MASK 0x00000001
#define AB8500_TVOUT_CTRL2_DENC_DDR(__x) \
	AB8500_VAL2REG(AB8500_TVOUT_CTRL2, DENC_DDR, __x)
#define AB8500_IT_MASK1 0x00000E40
#define AB8500_IT_MASK1_PON_KEY1_DBR_SHIFT 7
#define AB8500_IT_MASK1_PON_KEY1_DBR_MASK 0x00000080
#define AB8500_IT_MASK1_PON_KEY1_DBR(__x) \
	AB8500_VAL2REG(AB8500_IT_MASK1, PON_KEY1_DBR, __x)
#define AB8500_IT_MASK1_PON_KEY1_DBF_SHIFT 6
#define AB8500_IT_MASK1_PON_KEY1_DBF_MASK 0x00000040
#define AB8500_IT_MASK1_PON_KEY1_DBF(__x) \
	AB8500_VAL2REG(AB8500_IT_MASK1, PON_KEY1_DBF, __x)
#define AB8500_IT_MASK1_PON_KEY2_DBR_SHIFT 5
#define AB8500_IT_MASK1_PON_KEY2_DBR_MASK 0x00000020
#define AB8500_IT_MASK1_PON_KEY2_DBR(__x) \
	AB8500_VAL2REG(AB8500_IT_MASK1, PON_KEY2_DBR, __x)
#define AB8500_IT_MASK1_PON_KEY2_DBF_SHIFT 4
#define AB8500_IT_MASK1_PON_KEY2_DBF_MASK 0x00000010
#define AB8500_IT_MASK1_PON_KEY2_DBF(__x) \
	AB8500_VAL2REG(AB8500_IT_MASK1, PON_KEY2_DBF, __x)
#define AB8500_IT_MASK1_TEMP_WARN_SHIFT 3
#define AB8500_IT_MASK1_TEMP_WARN_MASK 0x00000008
#define AB8500_IT_MASK1_TEMP_WARN(__x) \
	AB8500_VAL2REG(AB8500_IT_MASK1, TEMP_WARN, __x)
#define AB8500_IT_MASK1_PLUG_TV_DET_SHIFT 2
#define AB8500_IT_MASK1_PLUG_TV_DET_MASK 0x00000004
#define AB8500_IT_MASK1_PLUG_TV_DET(__x) \
	AB8500_VAL2REG(AB8500_IT_MASK1, PLUG_TV_DET, __x)
#define AB8500_IT_MASK1_UNPLUG_TV_DET_SHIFT 1
#define AB8500_IT_MASK1_UNPLUG_TV_DET_MASK 0x00000002
#define AB8500_IT_MASK1_UNPLUG_TV_DET(__x) \
	AB8500_VAL2REG(AB8500_IT_MASK1, UNPLUG_TV_DET, __x)
#define AB8500_IT_MASK1_MAIN_EXT_CH_NOK_SHIFT 0
#define AB8500_IT_MASK1_MAIN_EXT_CH_NOK_MASK 0x00000001
#define AB8500_IT_MASK1_MAIN_EXT_CH_NOK(__x) \
	AB8500_VAL2REG(AB8500_IT_MASK1, MAIN_EXT_CH_NOK, __x)
#define AB8500_REV 0x00001080
#define AB8500_REV_FULL_MASK_SHIFT 4
#define AB8500_REV_FULL_MASK_MASK 0x000000F0
#define AB8500_REV_FULL_MASK(__x) \
	AB8500_VAL2REG(AB8500_REV, FULL_MASK, __x)
#define AB8500_REV_METAL_FIX_SHIFT 0
#define AB8500_REV_METAL_FIX_MASK 0x0000000F
#define AB8500_REV_METAL_FIX(__x) \
	AB8500_VAL2REG(AB8500_REV, METAL_FIX, __x)

#endif /* __AB8500_DENC_H */