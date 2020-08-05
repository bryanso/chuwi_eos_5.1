/******************************************************************************
 *
 * Copyright(c) 2007 - 2017  Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
 * Realtek Corporation, No. 2, Innovation Road II, Hsinchu Science Park,
 * Hsinchu 300, Taiwan.
 *
 * Larry Finger <Larry.Finger@lwfinger.net>
 *
 *****************************************************************************/

/*@************************************************************
 * include files
 * ************************************************************
 */

#include "mp_precomp.h"
#include "phydm_precomp.h"

#if (ODM_IC_11AC_SERIES_SUPPORT)
void phydm_reset_bb_hw_cnt_ac(void *dm_void)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;

	/*@ Reset all counter when 1 (including PMAC and PHY)*/
	/* Reset Page F counter*/
	odm_set_bb_reg(dm, R_0xb58, BIT(0), 1);
	odm_set_bb_reg(dm, R_0xb58, BIT(0), 0);
}
#endif

void phydm_dynamic_ant_weighting(void *dm_void)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;

#ifdef DYN_ANT_WEIGHTING_SUPPORT
	#if (RTL8197F_SUPPORT)
	if (dm->support_ic_type & (ODM_RTL8197F))
		phydm_dynamic_ant_weighting_8197f(dm);
	#endif

	#if (RTL8812A_SUPPORT)
	if (dm->support_ic_type & (ODM_RTL8812)) {
		phydm_dynamic_ant_weighting_8812a(dm);
	}
	#endif

	#if (RTL8822B_SUPPORT)
	if (dm->support_ic_type & (ODM_RTL8822B))
		phydm_dynamic_ant_weighting_8822b(dm);
	#endif
#endif
}

#ifdef DYN_ANT_WEIGHTING_SUPPORT
void phydm_ant_weight_dbg(void *dm_void, char input[][16], u32 *_used,
			  char *output, u32 *_out_len)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	char help[] = "-h";
	u32 var1[10] = {0};
	u32 used = *_used;
	u32 out_len = *_out_len;

	if ((strcmp(input[1], help) == 0)) {
		PDM_SNPF(out_len, used, output + used, out_len - used,
			 "echo dis_dym_ant_weighting {0/1}\n");

	} else {
		PHYDM_SSCANF(input[1], DCMD_DECIMAL, &var1[0]);

		if (var1[0] == 1) {
			dm->is_disable_dym_ant_weighting = 1;
			PDM_SNPF(out_len, used, output + used, out_len - used,
				 "Disable dyn-ant-weighting\n");
		} else {
			dm->is_disable_dym_ant_weighting = 0;
			PDM_SNPF(out_len, used, output + used, out_len - used,
				 "Enable dyn-ant-weighting\n");
		}
	}
	*_used = used;
	*_out_len = out_len;
}
#endif

void phydm_iq_gen_en(void *dm_void)
{
#ifdef PHYDM_COMPILE_IC_2SS
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u8 i = 0;
	enum rf_path path = RF_PATH_A;

	#if (ODM_IC_11AC_SERIES_SUPPORT)
	if (!(dm->support_ic_type & ODM_IC_11AC_SERIES))
		return;

	for (i = RF_PATH_A; i <= RF_PATH_B; i++) {
		path = (enum rf_path)i;

		/*RF mode table write enable*/
		odm_set_rf_reg(dm, path, RF_0xef, BIT(19), 0x1);
		/*Select RX mode*/
		odm_set_rf_reg(dm, path, RF_0x33, 0xF, 3);
		/*Set Table data*/
		odm_set_rf_reg(dm, path, RF_0x3e, 0xfffff, 0x00036);
		/*Set Table data*/
		odm_set_rf_reg(dm, path, RF_0x3f, 0xfffff, 0x5AFCE);
		/*RF mode table write disable*/
		odm_set_rf_reg(dm, path, RF_0xef, BIT(19), 0x0);
	}
	#endif
	#if (ODM_IC_11N_SERIES_SUPPORT)
	if (!(dm->support_ic_type & ODM_IC_11N_SERIES))
		return;
	if (dm->support_ic_type & ODM_RTL8192F) {
		 /*RF mode table write enable*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0xef, 0x80000, 0x1);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0xef, 0x80000, 0x1);
		/* Path A */
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x30, 0xfffff, 0x08000);
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x31, 0xfffff, 0x0005f);
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x32, 0xfffff, 0x01042);
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x30, 0xfffff, 0x18000);
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x31, 0xfffff, 0x0004f);
		odm_set_rf_reg(dm, RF_PATH_A, RF_0x32, 0xfffff, 0x71fc2);
		/* Path B */
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x30, 0xfffff, 0x08000);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x31, 0xfffff, 0x00050);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x32, 0xfffff, 0x01042);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x30, 0xfffff, 0x18000);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x31, 0xfffff, 0x00040);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0x32, 0xfffff, 0x71fc2);
		/*RF mode table write disable*/
		odm_set_rf_reg(dm, RF_PATH_A, RF_0xef, 0x80000, 0x0);
		odm_set_rf_reg(dm, RF_PATH_B, RF_0xef, 0x80000, 0x0);
	}
	#endif
#endif
}

void phydm_dis_cdd(void *dm_void)
{
#ifdef PHYDM_COMPILE_IC_2SS
	struct dm_struct *dm = (struct dm_struct *)dm_void;

	#if (ODM_IC_11AC_SERIES_SUPPORT)
	if (dm->support_ic_type & ODM_IC_11AC_SERIES) {
		odm_set_bb_reg(dm, R_0x808, 0x3ffff00, 0);
		odm_set_bb_reg(dm, R_0x9ac, 0x1fff, 0);
		odm_set_bb_reg(dm, R_0x9ac, BIT(13), 1);
	}
	#endif
	#if (ODM_IC_11N_SERIES_SUPPORT)
	if (dm->support_ic_type & ODM_IC_11N_SERIES) {
		odm_set_bb_reg(dm, R_0x90c, 0xffffffff, 0x83321333);
		/* Set Tx delay setting for CCK pathA,B*/
		odm_set_bb_reg(dm, R_0xa2c, 0xf0000000, 0);
		//Enable Tx CDD for HT-portion when spatial expansion is applied
		odm_set_bb_reg(dm, R_0xd00, BIT(8), 0);
		/* Tx CDD for Legacy*/
		odm_set_bb_reg(dm, R_0xd04, 0xf0000, 0);
		/* Tx CDD for non-HT*/
		odm_set_bb_reg(dm, R_0xd0c, 0x3c0, 0);
		/* Tx CDD for HT SS1*/
		odm_set_bb_reg(dm, R_0xd0c, 0xf8000, 0);
	}
	#endif
#endif
}

void phydm_pathb_q_matrix_rotate_en(void *dm_void)
{
#ifdef PHYDM_COMPILE_IC_2SS
	struct dm_struct *dm = (struct dm_struct *)dm_void;

	#if (ODM_IC_11AC_SERIES_SUPPORT)
	if (dm->support_ic_type & ODM_IC_11AC_SERIES) {
		phydm_iq_gen_en(dm);

		#ifdef PHYDM_COMMON_API_SUPPORT
		if (!phydm_api_trx_mode(dm, BB_PATH_AB, BB_PATH_AB, true))
			return;
		#endif

		phydm_dis_cdd(dm);
		/*Set Q matrix r_v11 =1*/
		odm_set_bb_reg(dm, R_0x195c, MASKDWORD, 0x40000);
		phydm_pathb_q_matrix_rotate(dm, 0);
		/*Set Q matrix enable*/
		odm_set_bb_reg(dm, R_0x191c, BIT(7), 1);
	}
	#endif
	#if (ODM_IC_11N_SERIES_SUPPORT)
	if (dm->support_ic_type & ODM_IC_11N_SERIES) {
		phydm_iq_gen_en(dm);

		#ifdef PHYDM_COMMON_API_SUPPORT
		if (!phydm_api_trx_mode(dm, BB_PATH_AB, BB_PATH_AB, true))
			return;
		#endif
		phydm_dis_cdd(dm);
		phydm_pathb_q_matrix_rotate(dm, 0);
	}
	#endif
#endif
}

void phydm_pathb_q_matrix_rotate(void *dm_void, u16 idx)
{
#ifdef PHYDM_COMPILE_IC_2SS
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	#if (ODM_IC_11AC_SERIES_SUPPORT)
	u32 phase_table_0[12] = {0x40000, 0x376CF, 0x20000, 0x00000,
				 0xFE0000, 0xFC8930, 0xFC0000, 0xFC8930,
				 0xFDFFFF, 0x000000, 0x020000, 0x0376CF};
	u32 phase_table_1[12] = {0x00000, 0x1FFFF, 0x376CF, 0x40000,
				 0x0376CF, 0x01FFFF, 0x000000, 0xFDFFFF,
				 0xFC8930, 0xFC0000, 0xFC8930, 0xFDFFFF};
	#endif
	#if (ODM_IC_11N_SERIES_SUPPORT)
	u32 phase_table_N_0[12] = {0x00, 0x0B, 0x02, 0x00, 0x02, 0x02, 0x04,
				   0x02, 0x0D, 0x09, 0x04, 0x0B};
	u32 phase_table_N_1[12] = {0x40000100, 0x377F00DD, 0x201D8880,
				   0x00000000, 0xE01D8B80, 0xC8BF0322,
				   0xC000FF00, 0xC8BF0322, 0xDFE2777F,
				   0xFFC003FF, 0x20227480, 0x377F00DD};
	u32 phase_table_N_2[12] = {0x00, 0x1E, 0x3C, 0x4C, 0x3C, 0x1E, 0x0F,
				   0xD2, 0xC3, 0xC4, 0xC3, 0xD2};
	#endif
	if (idx >= 12) {
		PHYDM_DBG(dm, ODM_COMP_API, "Phase Set Error: %d\n", idx);
		return;
	}

	#if (ODM_IC_11AC_SERIES_SUPPORT)
	if (dm->support_ic_type & ODM_IC_11AC_SERIES) {
		/*Set Q matrix r_v21*/
		odm_set_bb_reg(dm, R_0x1954, 0xffffff, phase_table_0[idx]);
		odm_set_bb_reg(dm, R_0x1950, 0xffffff, phase_table_1[idx]);
	}
	#endif
	#if (ODM_IC_11N_SERIES_SUPPORT)
	if (dm->support_ic_type & ODM_IC_11N_SERIES) {
		/*Set Q matrix r_v21*/
		odm_set_bb_reg(dm, R_0xc4c, 0xff000000, phase_table_N_0[idx]);
		odm_set_bb_reg(dm, R_0xc88, 0xffffffff, phase_table_N_1[idx]);
		odm_set_bb_reg(dm, R_0xc9c, 0xff000000, phase_table_N_2[idx]);
	}
	#endif
#endif
}

void phydm_trx_antenna_setting_init(void *dm_void, u8 num_rf_path)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u8 rx_ant = 0, tx_ant = 0;
	u8 path_bitmap = 1;

	path_bitmap = (u8)phydm_gen_bitmask(num_rf_path);
#if 0
	/*PHYDM_DBG(dm, ODM_COMP_INIT, "path_bitmap=0x%x\n", path_bitmap);*/
#endif

	dm->tx_ant_status = path_bitmap;
	dm->rx_ant_status = path_bitmap;

	if (num_rf_path == PDM_1SS)
		return;

	#if (defined(PHYDM_COMPILE_ABOVE_2SS))
	if (dm->support_ic_type &
		   (ODM_RTL8192F | ODM_RTL8192E | ODM_RTL8197F)) {
		dm->rx_ant_status = (u8)odm_get_bb_reg(dm, R_0xc04, 0x3);
		dm->tx_ant_status = (u8)odm_get_bb_reg(dm, R_0x90c, 0x3);
	} else if (dm->support_ic_type & (ODM_RTL8812 | ODM_RTL8814A)) {
		dm->rx_ant_status = (u8)odm_get_bb_reg(dm, R_0x808, 0xf);
		dm->tx_ant_status = (u8)odm_get_bb_reg(dm, R_0x80c, 0xf);
	}
	#endif

	PHYDM_DBG(dm, ODM_COMP_INIT, "[%s]ant_status{tx,rx}={0x%x, 0x%x}\n",
		  __func__, dm->tx_ant_status, dm->rx_ant_status);
}

void phydm_config_ofdm_tx_path(void *dm_void, u32 path)
{
#if (RTL8192E_SUPPORT || RTL8812A_SUPPORT)
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u8 ofdm_tx_path = 0x33;

	if (dm->num_rf_path == PDM_1SS)
		return;

	switch (dm->support_ic_type) {
	#if (RTL8192E_SUPPORT)
	case ODM_RTL8192E:
		if (path == BB_PATH_A)
			odm_set_bb_reg(dm, R_0x90c, MASKDWORD, 0x81121111);
		else if (path == BB_PATH_B)
			odm_set_bb_reg(dm, R_0x90c, MASKDWORD, 0x82221222);
		else if (path == BB_PATH_AB)
			odm_set_bb_reg(dm, R_0x90c, MASKDWORD, 0x83321333);

		break;
	#endif

	#if (RTL8812A_SUPPORT)
	case ODM_RTL8812:
		if (path == BB_PATH_A)
			ofdm_tx_path = 0x11;
		else if (path == BB_PATH_B)
			ofdm_tx_path = 0x22;
		else if (path == BB_PATH_AB)
			ofdm_tx_path = 0x33;

		odm_set_bb_reg(dm, R_0x80c, 0xff00, ofdm_tx_path);

		break;
	#endif

	default:
		break;
	}
#endif
}

void phydm_config_ofdm_rx_path(void *dm_void, u32 path)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u8 val = 0;

	if (dm->support_ic_type & (ODM_RTL8192E)) {
#if (RTL8192E_SUPPORT)
		if (path == BB_PATH_A)
			val = 1;
		else if (path == BB_PATH_B)
			val = 2;
		else if (path == BB_PATH_AB)
			val = 3;

		odm_set_bb_reg(dm, R_0xc04, 0xff, ((val << 4) | val));
		odm_set_bb_reg(dm, R_0xd04, 0xf, val);
#endif
	}
#if (RTL8812A_SUPPORT || RTL8822B_SUPPORT)
	else if (dm->support_ic_type & (ODM_RTL8812 | ODM_RTL8822B)) {
		if (path == BB_PATH_A)
			val = 1;
		else if (path == BB_PATH_B)
			val = 2;
		else if (path == BB_PATH_AB)
			val = 3;

		odm_set_bb_reg(dm, R_0x808, MASKBYTE0, ((val << 4) | val));
	}
#endif
}

void phydm_config_cck_rx_antenna_init(void *dm_void)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;

#if (defined(PHYDM_COMPILE_ABOVE_2SS))
	if (dm->support_ic_type & ODM_IC_1SS)
		return;

	/*@CCK 2R CCA parameters*/
	odm_set_bb_reg(dm, R_0xa00, BIT(15), 0x0); /*@Disable Ant diversity*/
	odm_set_bb_reg(dm, R_0xa70, BIT(7), 0); /*@Concurrent CCA at LSB & USB*/
	odm_set_bb_reg(dm, R_0xa74, BIT(8), 0); /*RX path diversity enable*/
	odm_set_bb_reg(dm, R_0xa14, BIT(7), 0); /*r_en_mrc_antsel*/
	odm_set_bb_reg(dm, R_0xa20, (BIT(5) | BIT(4)), 1); /*@MBC weighting*/

	if (dm->support_ic_type & (ODM_RTL8192E | ODM_RTL8197F | ODM_RTL8192F))
		odm_set_bb_reg(dm, R_0xa08, BIT(28), 1); /*r_cck_2nd_sel_eco*/
	else if (dm->support_ic_type & ODM_RTL8814A)
		odm_set_bb_reg(dm, R_0xa84, BIT(28), 1); /*@2R CCA only*/
#endif
}

void phydm_config_cck_rx_path(void *dm_void, enum bb_path path)
{
#if (defined(PHYDM_COMPILE_ABOVE_2SS))
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u8 path_div_select = 0;
	u8 cck_path[2] = {0};
	u8 en_2R_path = 0;
	u8 en_2R_mrc = 0;
	u8 i = 0, j = 0;
	u8 num_enable_path = 0;
	u8 cck_mrc_max_path = 2;

	if (dm->support_ic_type & ODM_IC_1SS)
		return;

	for (i = 0; i < 4; i++) {
		if (path & BIT(i)) { /*@ex: PHYDM_ABCD*/
			num_enable_path++;
			cck_path[j] = i;
			j++;
		}
		if (num_enable_path >= cck_mrc_max_path)
			break;
	}

	if (num_enable_path > 1) {
		path_div_select = 1;
		en_2R_path = 1;
		en_2R_mrc = 1;
	} else {
		path_div_select = 0;
		en_2R_path = 0;
		en_2R_mrc = 0;
	}
	/*@CCK_1 input signal path*/
	odm_set_bb_reg(dm, R_0xa04, (BIT(27) | BIT(26)), cck_path[0]);
	/*@CCK_2 input signal path*/
	odm_set_bb_reg(dm, R_0xa04, (BIT(25) | BIT(24)), cck_path[1]);
	/*@enable Rx path diversity*/
	odm_set_bb_reg(dm, R_0xa74, BIT(8), path_div_select);
	/*@enable 2R Rx path*/
	odm_set_bb_reg(dm, R_0xa2c, BIT(18), en_2R_path);
	/*@enable 2R MRC*/
	odm_set_bb_reg(dm, R_0xa2c, BIT(22), en_2R_mrc);
	if (dm->support_ic_type & ODM_RTL8192F) {
		if (path == BB_PATH_A) {
			odm_set_bb_reg(dm, R_0xa04, (BIT(27) | BIT(26)), 0);
			odm_set_bb_reg(dm, R_0xa04, (BIT(25) | BIT(24)), 0);
			odm_set_bb_reg(dm, R_0xa74, BIT(8), 0);
			odm_set_bb_reg(dm, R_0xa2c, (BIT(18) | BIT(17)), 0);
			odm_set_bb_reg(dm, R_0xa2c, (BIT(22) | BIT(21)), 0);
		} else if (path == BB_PATH_B) {/*@for DC cancellation*/
			odm_set_bb_reg(dm, R_0xa04, (BIT(27) | BIT(26)), 1);
			odm_set_bb_reg(dm, R_0xa04, (BIT(25) | BIT(24)), 1);
			odm_set_bb_reg(dm, R_0xa74, BIT(8), 0);
			odm_set_bb_reg(dm, R_0xa2c, (BIT(18) | BIT(17)), 0);
			odm_set_bb_reg(dm, R_0xa2c, (BIT(22) | BIT(21)), 0);
		} else if (path == BB_PATH_AB) {
			odm_set_bb_reg(dm, R_0xa04, (BIT(27) | BIT(26)), 0);
			odm_set_bb_reg(dm, R_0xa04, (BIT(25) | BIT(24)), 1);
			odm_set_bb_reg(dm, R_0xa74, BIT(8), 1);
			odm_set_bb_reg(dm, R_0xa2c, (BIT(18) | BIT(17)), 1);
			odm_set_bb_reg(dm, R_0xa2c, (BIT(22) | BIT(21)), 1);
		}
	}

#endif
}

void phydm_config_cck_tx_path(void *dm_void, enum bb_path path)
{
#if (defined(PHYDM_COMPILE_ABOVE_2SS))
	struct dm_struct *dm = (struct dm_struct *)dm_void;

	if (path == BB_PATH_A)
		odm_set_bb_reg(dm, R_0xa04, 0xf0000000, 0x8);
	else if (path == BB_PATH_B)
		odm_set_bb_reg(dm, R_0xa04, 0xf0000000, 0x4);
	else if (path == BB_PATH_AB)
		odm_set_bb_reg(dm, R_0xa04, 0xf0000000, 0xc);
#endif
}

void phydm_config_trx_path_v2(void *dm_void, char input[][16], u32 *_used,
			      char *output, u32 *_out_len)
{
#if (RTL8822B_SUPPORT || RTL8197F_SUPPORT || RTL8192F_SUPPORT)
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u32 used = *_used;
	u32 out_len = *_out_len;
	u32 val[10] = {0};
	char help[] = "-h";
	u8 i = 0, input_idx = 0;
	enum bb_path tx_path, rx_path;
	boolean dbg_mode_en, tx2_path_en;

	if (!(dm->support_ic_type &
	    (ODM_RTL8822B | ODM_RTL8197F | ODM_RTL8192F)))
		return;

	for (i = 0; i < 5; i++) {
		if (input[i + 1]) {
			PHYDM_SSCANF(input[i + 1], DCMD_HEX, &val[i]);
			input_idx++;
		}
	}

	if (input_idx == 0)
		return;

	dbg_mode_en = (boolean)val[0];
	tx_path = (enum bb_path)val[1];
	rx_path = (enum bb_path)val[2];
	tx2_path_en = (boolean)val[3];

	if ((strcmp(input[1], help) == 0)) {
		PDM_SNPF(out_len, used, output + used, out_len - used,
			 "{en} {tx_path} {rx_path} {1ss_tx_2_path_en}\n");

	} else if (dbg_mode_en) {
		dm->is_disable_phy_api = false;
		phydm_api_trx_mode(dm, tx_path, rx_path, tx2_path_en);
		dm->is_disable_phy_api = true;
		PDM_SNPF(out_len, used, output + used, out_len - used,
			 "tx_path = 0x%x, rx_path = 0x%x, tx2_path_en = %d\n",
			 tx_path, rx_path, tx2_path_en);
	} else {
		dm->is_disable_phy_api = false;
		PDM_SNPF(out_len, used, output + used, out_len - used,
			 "Disable API debug mode\n");
	}
#endif
}

void phydm_config_trx_path_v1(void *dm_void, char input[][16], u32 *_used,
			      char *output, u32 *_out_len)
{
#if (RTL8192E_SUPPORT || RTL8812A_SUPPORT)
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u32 used = *_used;
	u32 out_len = *_out_len;
	u32 val[10] = {0};
	char help[] = "-h";
	u8 i = 0, input_idx = 0;

	if (!(dm->support_ic_type & (ODM_RTL8192E | ODM_RTL8812)))
		return;

	for (i = 0; i < 5; i++) {
		if (input[i + 1]) {
			PHYDM_SSCANF(input[i + 1], DCMD_HEX, &val[i]);
			input_idx++;
		}
	}

	if (input_idx == 0)
		return;

	if ((strcmp(input[1], help) == 0)) {
		PDM_SNPF(out_len, used, output + used, out_len - used,
			 "{0:CCK, 1:OFDM} {1:TX, 2:RX} {1:path_A, 2:path_B, 3:path_AB}\n");

		*_used = used;
		*_out_len = out_len;
		return;

	} else if (val[0] == 0) {
	/* @CCK */
		if (val[1] == 1) { /*TX*/
			if (val[2] == 1)
				phydm_config_cck_tx_path(dm, BB_PATH_A);
			else if (val[2] == 2)
				phydm_config_cck_tx_path(dm, BB_PATH_B);
			else if (val[2] == 3)
				phydm_config_cck_tx_path(dm, BB_PATH_AB);
		} else if (val[1] == 2) { /*RX*/

			phydm_config_cck_rx_antenna_init(dm);

			if (val[2] == 1)
				phydm_config_cck_rx_path(dm, BB_PATH_A);
			else if (val[2] == 2)
				phydm_config_cck_rx_path(dm, BB_PATH_B);
			else if (val[2] == 3)
				phydm_config_cck_rx_path(dm, BB_PATH_AB);
			}
		}
	/* OFDM */
	else if (val[0] == 1) {
		if (val[1] == 1) /*TX*/
			phydm_config_ofdm_tx_path(dm, val[2]);
		else if (val[1] == 2) /*RX*/
			phydm_config_ofdm_rx_path(dm, val[2]);
	}

	PDM_SNPF(out_len, used, output + used, out_len - used,
		 "PHYDM Set path [%s] [%s] = [%s%s%s%s]\n",
		 (val[0] == 1) ? "OFDM" : "CCK",
		 (val[1] == 1) ? "TX" : "RX",
		 (val[2] & 0x1) ? "A" : "", (val[2] & 0x2) ? "B" : "",
		 (val[2] & 0x4) ? "C" : "",
		 (val[2] & 0x8) ? "D" : "");

	*_used = used;
	*_out_len = out_len;
#endif
}

void phydm_config_trx_path(void *dm_void, char input[][16], u32 *_used,
			   char *output, u32 *_out_len)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;

	if (dm->support_ic_type & (ODM_RTL8192E | ODM_RTL8812)) {
		#if (RTL8192E_SUPPORT || RTL8812A_SUPPORT)
		phydm_config_trx_path_v1(dm, input, _used, output, _out_len);
		#endif
	} else if (dm->support_ic_type &
		   (ODM_RTL8822B | ODM_RTL8197F | ODM_RTL8192F)) {
		#if (RTL8822B_SUPPORT || RTL8197F_SUPPORT || RTL8192F_SUPPORT)
		phydm_config_trx_path_v2(dm, input, _used, output, _out_len);
		#endif
	}
}

void phydm_tx_2path(void *dm_void)
{
#if (defined(PHYDM_COMPILE_IC_2SS))
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	enum bb_path rx_path = (enum bb_path)dm->rx_ant_status;

	PHYDM_DBG(dm, ODM_COMP_API, "%s ======>\n", __func__);


	if (!(dm->support_ic_type & ODM_IC_2SS))
		return;

	#if (RTL8822B_SUPPORT || RTL8192F_SUPPORT || RTL8197F_SUPPORT)
	if (dm->support_ic_type & (ODM_RTL8822B | ODM_RTL8197F | ODM_RTL8192F))
		phydm_api_trx_mode(dm, BB_PATH_AB, rx_path, true);
	#endif

	#if (RTL8812A_SUPPORT || RTL8192E_SUPPORT)
	if (dm->support_ic_type & (ODM_RTL8812 | ODM_RTL8192E)) {
		phydm_config_cck_tx_path(dm, BB_PATH_AB);
		phydm_config_ofdm_tx_path(dm, BB_PATH_AB);
	}
	#endif
#endif
}

void phydm_stop_3_wire(void *dm_void, u8 set_type)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;

	if (set_type == PHYDM_SET) {
		/*@[Stop 3-wires]*/
		if (dm->support_ic_type & ODM_IC_11AC_SERIES) {
			odm_set_bb_reg(dm, R_0xc00, 0xf, 0x4);
			odm_set_bb_reg(dm, R_0xe00, 0xf, 0x4);
		} else {
			odm_set_bb_reg(dm, R_0x88c, 0xf00000, 0xf);
		}

	} else { /*@if (set_type == PHYDM_REVERT)*/

		/*@[Start 3-wires]*/
		if (dm->support_ic_type & ODM_IC_11AC_SERIES) {
			odm_set_bb_reg(dm, R_0xc00, 0xf, 0x7);
			odm_set_bb_reg(dm, R_0xe00, 0xf, 0x7);
		} else {
			odm_set_bb_reg(dm, R_0x88c, 0xf00000, 0x0);
		}
	}
}

u8 phydm_stop_ic_trx(void *dm_void, u8 set_type)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	struct phydm_api_stuc *api = &dm->api_table;
	u32 i;
	u8 trx_idle_success = false;
	u32 dbg_port_value = 0;

	if (set_type == PHYDM_SET) {
	/*@[Stop TRX]---------------------------------------------------------*/
		/*set debug port to 0x0*/
		if (!phydm_set_bb_dbg_port(dm, DBGPORT_PRI_3, 0x0))
			return PHYDM_SET_FAIL;

		for (i = 0; i < 10000; i++) {
			dbg_port_value = phydm_get_bb_dbg_port_val(dm);
			/* PHYTXON && CCA_all */
			if ((dbg_port_value & (BIT(17) | BIT(3))) == 0) {
				PHYDM_DBG(dm, ODM_COMP_API,
					  "Stop trx wait for (%d) times\n", i);

				trx_idle_success = true;
				break;
			}
		}
		phydm_release_bb_dbg_port(dm);

		if (trx_idle_success) {
			api->tx_queue_bitmap = odm_read_1byte(dm, R_0x522);

			/*pause all TX queue*/
			odm_set_bb_reg(dm, R_0x520, 0xff0000, 0xff);

			if (dm->support_ic_type & ODM_IC_11AC_SERIES) {
				/*@disable CCK block*/
				odm_set_bb_reg(dm, R_0x808, BIT(28), 0);
				/*@disable OFDM RX CCA*/
				odm_set_bb_reg(dm, R_0x838, BIT(1), 1);
			} else {
				/* @disable whole CCK block */
				odm_set_bb_reg(dm, R_0x800, BIT(24), 0);

				api->rxiqc_reg1 = odm_read_4byte(dm, R_0xc14);
				api->rxiqc_reg2 = odm_read_4byte(dm, R_0xc1c);
				/* @[ Set IQK Matrix = 0 ]
				 * equivalent to [ Turn off CCA]
				 */
				odm_set_bb_reg(dm, R_0xc14, MASKDWORD, 0x0);
				odm_set_bb_reg(dm, R_0xc1c, MASKDWORD, 0x0);
			}

		} else {
			return PHYDM_SET_FAIL;
		}

		return PHYDM_SET_SUCCESS;

	} else { /*@if (set_type == PHYDM_REVERT)*/
		/*Release all TX queue*/
		odm_write_1byte(dm, R_0x522, api->tx_queue_bitmap);

		if (dm->support_ic_type & ODM_IC_11AC_SERIES) {
			/*@enable CCK block*/
			odm_set_bb_reg(dm, R_0x808, BIT(28), 1);
			/*@enable OFDM RX CCA*/
			odm_set_bb_reg(dm, R_0x838, BIT(1), 0);
		} else {
			/* @enable whole CCK block */
			odm_set_bb_reg(dm, R_0x800, BIT(24), 1);
			/* @[Set IQK Matrix = 0] equivalent to [ Turn off CCA]*/
			odm_write_4byte(dm, R_0xc14, api->rxiqc_reg1);
			odm_write_4byte(dm, R_0xc1c, api->rxiqc_reg2);
		}

		return PHYDM_SET_SUCCESS;
	}
}

void phydm_set_ext_switch(void *dm_void, u32 ext_ant_switch)
{
#if (RTL8821A_SUPPORT || RTL8881A_SUPPORT)
	struct dm_struct *dm = (struct dm_struct *)dm_void;

	if (!(dm->support_ic_type & (ODM_RTL8821 | ODM_RTL8881A)))
		return;

	/*Output Pin Settings*/

	/*select DPDT_P and DPDT_N as output pin*/
	odm_set_mac_reg(dm, R_0x4c, BIT(23), 0);

	/*@by WLAN control*/
	odm_set_mac_reg(dm, R_0x4c, BIT(24), 1);

	/*@DPDT_N = 1b'0*/ /*@DPDT_P = 1b'0*/
	odm_set_bb_reg(dm, R_0xcb4, 0xFF, 77);

	if (ext_ant_switch == 1) { /*@2b'01*/
		odm_set_bb_reg(dm, R_0xcb4, (BIT(29) | BIT(28)), 1);
		PHYDM_DBG(dm, ODM_COMP_API, "8821A ant swh=2b'01\n");
	} else if (ext_ant_switch == 2) { /*@2b'10*/
		odm_set_bb_reg(dm, R_0xcb4, BIT(29) | BIT(28), 2);
		PHYDM_DBG(dm, ODM_COMP_API, "*8821A ant swh=2b'10\n");
	}
#endif
}

void phydm_csi_mask_enable(void *dm_void, u32 enable)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	boolean en = false;

	en = (enable == FUNC_ENABLE) ? true : false;

	if (dm->support_ic_type & ODM_IC_11N_SERIES) {
		odm_set_bb_reg(dm, R_0xd2c, BIT(28), en);
		PHYDM_DBG(dm, ODM_COMP_API,
			  "Enable CSI Mask:  Reg 0xD2C[28] = ((0x%x))\n", en);
	#ifdef PHYDM_IC_JGR3_SERIES_SUPPORT
	} else if (dm->support_ic_type & ODM_IC_JGR3_SERIES) {
		if (en)
			odm_set_bb_reg(dm, R_0x1ee8, 0x3, 0x3);

		odm_set_bb_reg(dm, R_0xc0c, BIT(3), en);
	#endif
	} else if (dm->support_ic_type & ODM_IC_11AC_SERIES) {
		odm_set_bb_reg(dm, R_0x874, BIT(0), en);
		PHYDM_DBG(dm, ODM_COMP_API,
			  "Enable CSI Mask:  Reg 0x874[0] = ((0x%x))\n", en);
	}
}

void phydm_clean_all_csi_mask(void *dm_void)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;

	if (dm->support_ic_type & ODM_IC_11N_SERIES) {
		odm_set_bb_reg(dm, R_0xd40, MASKDWORD, 0);
		odm_set_bb_reg(dm, R_0xd44, MASKDWORD, 0);
		odm_set_bb_reg(dm, R_0xd48, MASKDWORD, 0);
		odm_set_bb_reg(dm, R_0xd4c, MASKDWORD, 0);
	#ifdef PHYDM_IC_JGR3_SERIES_SUPPORT
	} else if (dm->support_ic_type & ODM_IC_JGR3_SERIES) {
		u8 i = 0, idx_lmt = 0;

		if (dm->support_ic_type & ODM_RTL8822C)
			idx_lmt = 127;
		else/*@for IC supporting 80 + 80*/
			idx_lmt = 255;

		odm_set_bb_reg(dm, R_0x1d94, BIT(31) | BIT(30), 0x1);
		for (i = 0; i < idx_lmt; i++) {
			odm_set_bb_reg(dm, R_0x1d94, MASKBYTE2, i);
			odm_set_bb_reg(dm, R_0x1d94, MASKBYTE0, 0x0);
		}
		odm_set_bb_reg(dm, R_0x1ee8, 0x3, 0x0);
	#endif
	} else if (dm->support_ic_type & ODM_IC_11AC_SERIES) {
		odm_set_bb_reg(dm, R_0x880, MASKDWORD, 0);
		odm_set_bb_reg(dm, R_0x884, MASKDWORD, 0);
		odm_set_bb_reg(dm, R_0x888, MASKDWORD, 0);
		odm_set_bb_reg(dm, R_0x88c, MASKDWORD, 0);
		odm_set_bb_reg(dm, R_0x890, MASKDWORD, 0);
		odm_set_bb_reg(dm, R_0x894, MASKDWORD, 0);
		odm_set_bb_reg(dm, R_0x898, MASKDWORD, 0);
		odm_set_bb_reg(dm, R_0x89c, MASKDWORD, 0);
	}
}

void phydm_set_csi_mask(void *dm_void, u32 tone_idx_tmp, u8 tone_direction)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u8 byte_offset = 0, bit_offset = 0;
	u32 target_reg = 0;
	u8 reg_tmp_value = 0;
	u32 tone_num = 64;
	u32 tone_num_shift = 0;
	u32 csi_mask_reg_p = 0, csi_mask_reg_n = 0;

	/* @calculate real tone idx*/
	if ((tone_idx_tmp % 10) >= 5)
		tone_idx_tmp += 10;

	tone_idx_tmp = (tone_idx_tmp / 10);

	if (dm->support_ic_type & ODM_IC_11N_SERIES) {
		tone_num = 64;
		csi_mask_reg_p = 0xD40;
		csi_mask_reg_n = 0xD48;

	} else if (dm->support_ic_type & ODM_IC_11AC_SERIES) {
		tone_num = 128;
		csi_mask_reg_p = 0x880;
		csi_mask_reg_n = 0x890;
	}

	if (tone_direction == FREQ_POSITIVE) {
		if (tone_idx_tmp >= (tone_num - 1))
			tone_idx_tmp = (tone_num - 1);

		byte_offset = (u8)(tone_idx_tmp >> 3);
		bit_offset = (u8)(tone_idx_tmp & 0x7);
		target_reg = csi_mask_reg_p + byte_offset;

	} else {
		tone_num_shift = tone_num;

		if (tone_idx_tmp >= tone_num)
			tone_idx_tmp = tone_num;

		tone_idx_tmp = tone_num - tone_idx_tmp;

		byte_offset = (u8)(tone_idx_tmp >> 3);
		bit_offset = (u8)(tone_idx_tmp & 0x7);
		target_reg = csi_mask_reg_n + byte_offset;
	}

	reg_tmp_value = odm_read_1byte(dm, target_reg);
	PHYDM_DBG(dm, ODM_COMP_API,
		  "Pre Mask tone idx[%d]:  Reg0x%x = ((0x%x))\n",
		  (tone_idx_tmp + tone_num_shift), target_reg, reg_tmp_value);
	reg_tmp_value |= BIT(bit_offset);
	odm_write_1byte(dm, target_reg, reg_tmp_value);
	PHYDM_DBG(dm, ODM_COMP_API,
		  "New Mask tone idx[%d]:  Reg0x%x = ((0x%x))\n",
		  (tone_idx_tmp + tone_num_shift), target_reg, reg_tmp_value);
}

void phydm_set_nbi_reg(void *dm_void, u32 tone_idx_tmp, u32 bw)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	/*tone_idx X 10*/
	u32 nbi_128[NBI_128TONE] = {25, 55, 85, 115, 135,
				    155, 185, 205, 225, 245,
				    265, 285, 305, 335, 355,
				    375, 395, 415, 435, 455,
				    485, 505, 525, 555, 585, 615, 635};
	/*tone_idx X 10*/
	u32 nbi_256[NBI_256TONE] = {25, 55, 85, 115, 135,
				    155, 175, 195, 225, 245,
				    265, 285, 305, 325, 345,
				    365, 385, 405, 425, 445,
				    465, 485, 505, 525, 545,
				    565, 585, 605, 625, 645,
				    665, 695, 715, 735, 755,
				    775, 795, 815, 835, 855,
				    875, 895, 915, 935, 955,
				    975, 995, 1015, 1035, 1055,
				    1085, 1105, 1125, 1145, 1175,
				    1195, 1225, 1255, 1275};
	u32 reg_idx = 0;
	u32 i;
	u8 nbi_table_idx = FFT_128_TYPE;

	if (dm->support_ic_type & ODM_IC_11N_SERIES) {
		nbi_table_idx = FFT_128_TYPE;
	} else if (dm->support_ic_type & ODM_IC_11AC_1_SERIES) {
		nbi_table_idx = FFT_256_TYPE;
	} else if (dm->support_ic_type & ODM_IC_11AC_2_SERIES) {
		if (bw == 80)
			nbi_table_idx = FFT_256_TYPE;
		else /*@20M, 40M*/
			nbi_table_idx = FFT_128_TYPE;
	}

	if (nbi_table_idx == FFT_128_TYPE) {
		for (i = 0; i < NBI_128TONE; i++) {
			if (tone_idx_tmp < nbi_128[i]) {
				reg_idx = i + 1;
				break;
			}
		}

	} else if (nbi_table_idx == FFT_256_TYPE) {
		for (i = 0; i < NBI_256TONE; i++) {
			if (tone_idx_tmp < nbi_256[i]) {
				reg_idx = i + 1;
				break;
			}
		}
	}

	if (dm->support_ic_type & ODM_IC_11N_SERIES) {
		odm_set_bb_reg(dm, R_0xc40, 0x1f000000, reg_idx);
		PHYDM_DBG(dm, ODM_COMP_API,
			  "Set tone idx:  Reg0xC40[28:24] = ((0x%x))\n",
			  reg_idx);
	} else {
		odm_set_bb_reg(dm, R_0x87c, 0xfc000, reg_idx);
		PHYDM_DBG(dm, ODM_COMP_API,
			  "Set tone idx: Reg0x87C[19:14] = ((0x%x))\n",
			  reg_idx);
	}
}

void phydm_nbi_enable(void *dm_void, u32 enable)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u32 val = 0;

	val = (enable == FUNC_ENABLE) ? 1 : 0;

	PHYDM_DBG(dm, ODM_COMP_API, "Enable NBI=%d\n", val);

	if (dm->support_ic_type & ODM_IC_11N_SERIES) {
		if (dm->support_ic_type & (ODM_RTL8192F | ODM_RTL8197F)) {
			val = (enable == FUNC_ENABLE) ? 0xf : 0;
			odm_set_bb_reg(dm, R_0xc50, 0xf000000, val);
		} else {
			odm_set_bb_reg(dm, R_0xc40, BIT(9), val);
		}
	} else if (dm->support_ic_type & ODM_IC_11AC_SERIES) {
		if (dm->support_ic_type & (ODM_RTL8822B | ODM_RTL8821C)) {
			odm_set_bb_reg(dm, R_0x87c, BIT(13), val);
			odm_set_bb_reg(dm, R_0xc20, BIT(28), val);
			if (dm->rf_type > RF_1T1R)
				odm_set_bb_reg(dm, R_0xe20, BIT(28), val);
		} else {
			odm_set_bb_reg(dm, R_0x87c, BIT(13), val);
		}
	}
}

u8 phydm_find_fc(void *dm_void, u32 channel, u32 bw, u32 second_ch, u32 *fc_in)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u32 fc = *fc_in;
	u32 start_ch_per_40m[NUM_START_CH_40M] = {36, 44, 52, 60, 100,
						  108, 116, 124, 132, 140,
						  149, 157, 165, 173};
	u32 start_ch_per_80m[NUM_START_CH_80M] = {36, 52, 100, 116, 132,
						  149, 165};
	u32 *start_ch = &start_ch_per_40m[0];
	u32 num_start_channel = NUM_START_CH_40M;
	u32 channel_offset = 0;
	u32 i;

	/*@2.4G*/
	if (channel <= 14 && channel > 0) {
		if (bw == 80)
			return PHYDM_SET_FAIL;

		fc = 2412 + (channel - 1) * 5;

		if (bw == 40 && second_ch == PHYDM_ABOVE) {
			if (channel >= 10) {
				PHYDM_DBG(dm, ODM_COMP_API,
					  "CH = ((%d)), Scnd_CH = ((%d)) Error setting\n",
					  channel, second_ch);
				return PHYDM_SET_FAIL;
			}
			fc += 10;
		} else if (bw == 40 && (second_ch == PHYDM_BELOW)) {
			if (channel <= 2) {
				PHYDM_DBG(dm, ODM_COMP_API,
					  "CH = ((%d)), Scnd_CH = ((%d)) Error setting\n",
					  channel, second_ch);
				return PHYDM_SET_FAIL;
			}
			fc -= 10;
		}
	}
	/*@5G*/
	else if (channel >= 36 && channel <= 177) {
		if (bw != 20) {
			if (bw == 40) {
				num_start_channel = NUM_START_CH_40M;
				start_ch = &start_ch_per_40m[0];
				channel_offset = CH_OFFSET_40M;
			} else if (bw == 80) {
				num_start_channel = NUM_START_CH_80M;
				start_ch = &start_ch_per_80m[0];
				channel_offset = CH_OFFSET_80M;
			}

			for (i = 0; i < (num_start_channel - 1); i++) {
				if (channel < start_ch[i + 1]) {
					channel = start_ch[i] + channel_offset;
					break;
				}
			}
			PHYDM_DBG(dm, ODM_COMP_API, "Mod_CH = ((%d))\n",
				  channel);
		}

		fc = 5180 + (channel - 36) * 5;

	} else {
		PHYDM_DBG(dm, ODM_COMP_API, "CH = ((%d)) Error setting\n",
			  channel);
		return PHYDM_SET_FAIL;
	}

	*fc_in = fc;

	return PHYDM_SET_SUCCESS;
}

u8 phydm_find_intf_distance(void *dm_void, u32 bw, u32 fc, u32 f_interference,
			    u32 *tone_idx_tmp_in)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u32 bw_up = 0, bw_low = 0;
	u32 int_distance = 0;
	u32 tone_idx_tmp = 0;
	u8 set_result = PHYDM_SET_NO_NEED;

	bw_up = fc + bw / 2;
	bw_low = fc - bw / 2;

	PHYDM_DBG(dm, ODM_COMP_API,
		  "[f_l, fc, fh] = [ %d, %d, %d ], f_int = ((%d))\n", bw_low,
		  fc, bw_up, f_interference);

	if (f_interference >= bw_low && f_interference <= bw_up) {
		int_distance = DIFF_2(fc, f_interference);
		/*@10*(int_distance /0.3125)*/
		tone_idx_tmp = (int_distance << 5);
		PHYDM_DBG(dm, ODM_COMP_API,
			  "int_distance = ((%d MHz)) Mhz, tone_idx_tmp = ((%d.%d))\n",
			  int_distance, tone_idx_tmp / 10,
			  tone_idx_tmp % 10);
		*tone_idx_tmp_in = tone_idx_tmp;
		set_result = PHYDM_SET_SUCCESS;
	}

	return set_result;
}

u8 phydm_csi_mask_setting(void *dm_void, u32 enable, u32 ch, u32 bw,
			  u32 f_intf, u32 sec_ch)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u32 fc = 2412;
	u8 direction = FREQ_POSITIVE;
	u32 tone_idx = 0;
	u8 set_result = PHYDM_SET_SUCCESS;
	u8 rpt = 0;

	if (enable == FUNC_DISABLE) {
		set_result = PHYDM_SET_SUCCESS;
		phydm_clean_all_csi_mask(dm);

	} else {
		PHYDM_DBG(dm, ODM_COMP_API,
			  "[Set CSI MASK_] CH = ((%d)), BW = ((%d)), f_intf = ((%d)), Scnd_CH = ((%s))\n",
			  ch, bw, f_intf,
			  (((bw == 20) || (ch > 14)) ? "Don't care" :
			  (sec_ch == PHYDM_ABOVE) ? "H" : "L"));

		/*@calculate fc*/
		if (phydm_find_fc(dm, ch, bw, sec_ch, &fc) == PHYDM_SET_FAIL) {
			set_result = PHYDM_SET_FAIL;
		} else {
			/*@calculate interference distance*/
			rpt = phydm_find_intf_distance(dm, bw, fc, f_intf,
						       &tone_idx);
			if (rpt == PHYDM_SET_SUCCESS) {
				if (f_intf >= fc)
					direction = FREQ_POSITIVE;
				else
					direction = FREQ_NEGATIVE;

				phydm_set_csi_mask(dm, tone_idx, direction);
				set_result = PHYDM_SET_SUCCESS;
			} else {
				set_result = PHYDM_SET_NO_NEED;
			}
		}
	}

	if (set_result == PHYDM_SET_SUCCESS)
		phydm_csi_mask_enable(dm, enable);
	else
		phydm_csi_mask_enable(dm, FUNC_DISABLE);

	return set_result;
}

#ifdef PHYDM_IC_JGR3_SERIES_SUPPORT
u8 phydm_csi_mask_setting_jgr3(void *dm_void, u32 enable, u32 ch, u32 bw,
			       u32 f_intf, u32 sec_ch, u8 wgt)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u32 fc = 2412;
	u8 direction = FREQ_POSITIVE;
	u32 tone_idx = 0;
	u8 set_result = PHYDM_SET_SUCCESS;
	u8 rpt = 0;

	if (enable == FUNC_DISABLE) {
		phydm_csi_mask_enable(dm, FUNC_ENABLE);
		phydm_clean_all_csi_mask(dm);
		phydm_csi_mask_enable(dm, FUNC_DISABLE);
		set_result = PHYDM_SET_SUCCESS;
	} else {
		PHYDM_DBG(dm, ODM_COMP_API,
			  "[Set CSI MASK] CH = ((%d)), BW = ((%d)), f_intf = ((%d)), Scnd_CH = ((%s)), wgt = ((%d))\n",
			  ch, bw, f_intf,
			  (((bw == 20) || (ch > 14)) ? "Don't care" :
			  (sec_ch == PHYDM_ABOVE) ? "H" : "L"), wgt);

		/*@calculate fc*/
		if (phydm_find_fc(dm, ch, bw, sec_ch, &fc) == PHYDM_SET_FAIL) {
			set_result = PHYDM_SET_FAIL;
		} else {
			/*@calculate interference distance*/
			rpt = phydm_find_intf_distance(dm, bw, fc, f_intf,
						       &tone_idx);
			if (rpt == PHYDM_SET_SUCCESS) {
				if (f_intf >= fc)
					direction = FREQ_POSITIVE;
				else
					direction = FREQ_NEGATIVE;

				phydm_csi_mask_enable(dm, FUNC_ENABLE);
				phydm_set_csi_mask_jgr3(dm, tone_idx, direction,
							wgt);
				set_result = PHYDM_SET_SUCCESS;
			} else {
				set_result = PHYDM_SET_NO_NEED;
			}
		}
		if (!(set_result == PHYDM_SET_SUCCESS))
			phydm_csi_mask_enable(dm, FUNC_DISABLE);
	}

	return set_result;
}

void phydm_set_csi_mask_jgr3(void *dm_void, u32 tone_idx_tmp, u8 tone_direction,
			     u8 wgt)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u32 reg_tmp_value = 0;
	u32 tone_num = 64;
	u32 tone_num_shift = 0;
	u32 table_addr = 0;
	u32 addr = 0;
	u8 rf_bw = 0;

	/* @calculate real tone idx*/
	if ((tone_idx_tmp % 10) >= 5)
		tone_idx_tmp += 10;

	tone_idx_tmp = (tone_idx_tmp / 10);

	rf_bw = odm_read_1byte(dm, R_0x9b0);
	if (((rf_bw & 0xc) >> 2) == 0x2)
		tone_num = 128; /* RF80 : tone-1 at tone_idx=255 */
	else
		tone_num = 64; /* RF20/40 : tone-1 at tone_idx=127 */

	if (tone_direction == FREQ_POSITIVE) {
		if (tone_idx_tmp >= (tone_num - 1))
			tone_idx_tmp = (tone_num - 1);
	} else {
		tone_num_shift = tone_num;
		if (tone_idx_tmp >= tone_num)
			tone_idx_tmp = tone_num;

		tone_idx_tmp = (tone_num << 1) - tone_idx_tmp;
	}
	table_addr = tone_idx_tmp >> 1;

	reg_tmp_value = odm_read_4byte(dm, R_0x1d94);
	PHYDM_DBG(dm, ODM_COMP_API,
		  "Pre Mask tone idx[%d]: Reg0x1d94 = ((0x%x))\n",
		  (tone_idx_tmp + tone_num_shift), reg_tmp_value);
	odm_set_bb_reg(dm, R_0x1d94, MASKBYTE2, (table_addr & 0xff));
	if (tone_idx_tmp % 2)
		addr = 0xf;
	else
		addr = 0xf0;

	odm_set_bb_reg(dm, R_0x1d94, addr, (BIT(3) | (wgt & 0x7)));
	reg_tmp_value = odm_read_4byte(dm, R_0x1d94);
	PHYDM_DBG(dm, ODM_COMP_API,
		  "New Mask tone idx[%d]: Reg0x1d94 = ((0x%x))\n",
		  (tone_idx_tmp + tone_num_shift), reg_tmp_value);
	odm_set_bb_reg(dm, R_0x1ee8, 0x3, 0x0);
}

u8 phydm_nbi_setting_jgr3(void *dm_void, u32 enable, u32 ch, u32 bw, u32 f_intf,
			  u32 sec_ch, u8 path)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u32 fc = 2412;
	u8 direction = FREQ_POSITIVE;
	u32 tone_idx = 0;
	u8 set_result = PHYDM_SET_SUCCESS;
	u8 rpt = 0;

	if (enable == FUNC_DISABLE) {
		set_result = PHYDM_SET_SUCCESS;
	} else {
		PHYDM_DBG(dm, ODM_COMP_API,
			  "[Set NBI] CH = ((%d)), BW = ((%d)), f_intf = ((%d)), Scnd_CH = ((%s))\n",
			  ch, bw, f_intf,
			  (((sec_ch == PHYDM_DONT_CARE) || (bw == 20) ||
			  (ch > 14)) ? "Don't care" :
			  (sec_ch == PHYDM_ABOVE) ? "H" : "L"));

		/*@calculate fc*/
		if (phydm_find_fc(dm, ch, bw, sec_ch, &fc) == PHYDM_SET_FAIL) {
			set_result = PHYDM_SET_FAIL;
		} else {
			/*@calculate interference distance*/
			rpt = phydm_find_intf_distance(dm, bw, fc, f_intf,
						       &tone_idx);
			if (rpt == PHYDM_SET_SUCCESS) {
				if (f_intf >= fc)
					direction = FREQ_POSITIVE;
				else
					direction = FREQ_NEGATIVE;

				phydm_set_nbi_reg_jgr3(dm, tone_idx, direction,
						       path);
				set_result = PHYDM_SET_SUCCESS;
			} else {
				set_result = PHYDM_SET_NO_NEED;
		}
	}
	}

	if (set_result == PHYDM_SET_SUCCESS)
		phydm_nbi_enable_jgr3(dm, enable, path);
	else
		phydm_nbi_enable_jgr3(dm, FUNC_DISABLE, path);

	return set_result;
}

void phydm_set_nbi_reg_jgr3(void *dm_void, u32 tone_idx_tmp, u8 tone_direction,
			    u8 path)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u32 reg_tmp_value = 0;
	u32 tone_num = 64;
	u32 tone_num_shift = 0;
	u32 addr = 0;
	u8 rf_bw = 0;

	/* @calculate real tone idx*/
	if ((tone_idx_tmp % 10) >= 5)
		tone_idx_tmp += 10;

	tone_idx_tmp = (tone_idx_tmp / 10);

	rf_bw = odm_read_1byte(dm, R_0x9b0);
	if (((rf_bw & 0xc) >> 2) == 0x2)
		tone_num = 128; /* RF80 : tone-1 at tone_idx=255 */
	else
		tone_num = 64; /* RF20/40 : tone-1 at tone_idx=127 */

	if (tone_direction == FREQ_POSITIVE) {
		if (tone_idx_tmp >= (tone_num - 1))
			tone_idx_tmp = (tone_num - 1);
	} else {
		tone_num_shift = tone_num;
		if (tone_idx_tmp >= tone_num)
			tone_idx_tmp = tone_num;

		tone_idx_tmp = (tone_num << 1) - tone_idx_tmp;
	}

	switch (path) {
	case RF_PATH_A:
		odm_set_bb_reg(dm, R_0x1944, 0x001FF000, tone_idx_tmp);
		PHYDM_DBG(dm, ODM_COMP_API,
			  "Set tone idx[%d]:PATH-A = ((0x%x))\n",
			  (tone_idx_tmp + tone_num_shift), tone_idx_tmp);
		break;
	#if (defined(PHYDM_COMPILE_ABOVE_2SS))
	case RF_PATH_B:
		odm_set_bb_reg(dm, R_0x4044, 0x001FF000, tone_idx_tmp);
		PHYDM_DBG(dm, ODM_COMP_API,
			  "Set tone idx[%d]:PATH-B = ((0x%x))\n",
			  (tone_idx_tmp + tone_num_shift), tone_idx_tmp);
		break;
	#endif
	#if (defined(PHYDM_COMPILE_ABOVE_3SS))
	case RF_PATH_C:
		odm_set_bb_reg(dm, R_0x5044, 0x001FF000, tone_idx_tmp);
		PHYDM_DBG(dm, ODM_COMP_API,
			  "Set tone idx[%d]:PATH-C = ((0x%x))\n",
			  (tone_idx_tmp + tone_num_shift), tone_idx_tmp);
		break;
	#endif
	#if (defined(PHYDM_COMPILE_ABOVE_4SS))
	case RF_PATH_D:
		odm_set_bb_reg(dm, R_0x5144, 0x001FF000, tone_idx_tmp);
		PHYDM_DBG(dm, ODM_COMP_API,
			  "Set tone idx[%d]:PATH-D = ((0x%x))\n",
			  (tone_idx_tmp + tone_num_shift), tone_idx_tmp);
		break;
	#endif
	default:
		break;
	}
}

void phydm_nbi_enable_jgr3(void *dm_void, u32 enable, u8 path)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u32 val = 0;

	val = (enable == FUNC_ENABLE) ? 1 : 0;

	PHYDM_DBG(dm, ODM_COMP_API, "Enable NBI=%d\n", val);

	odm_set_bb_reg(dm, R_0x818, BIT(11), val);
	if (enable == FUNC_ENABLE) {
		switch (path) {
		case RF_PATH_A:
			odm_set_bb_reg(dm, R_0x1940, BIT(31), val);
			break;
		#if (defined(PHYDM_COMPILE_ABOVE_2SS))
		case RF_PATH_B:
			odm_set_bb_reg(dm, R_0x4040, BIT(31), val);
			break;
		#endif
		#if (defined(PHYDM_COMPILE_ABOVE_3SS))
		case RF_PATH_C:
			odm_set_bb_reg(dm, R_0x5040, BIT(31), val);
			break;
		#endif
		#if (defined(PHYDM_COMPILE_ABOVE_4SS))
		case RF_PATH_D:
			odm_set_bb_reg(dm, R_0x5140, BIT(31), val);
			break;
		#endif
		default:
			break;
		}
	} else {
		odm_set_bb_reg(dm, R_0x1940, BIT(31), val);
		#if (defined(PHYDM_COMPILE_ABOVE_2SS))
		odm_set_bb_reg(dm, R_0x4040, BIT(31), val);
		#endif
		#if (defined(PHYDM_COMPILE_ABOVE_3SS))
		odm_set_bb_reg(dm, R_0x5040, BIT(31), val);
		#endif
		#if (defined(PHYDM_COMPILE_ABOVE_4SS))
		odm_set_bb_reg(dm, R_0x5140, BIT(31), val);
		#endif
	}
}

#endif
u8 phydm_nbi_setting(void *dm_void, u32 enable, u32 ch, u32 bw, u32 f_intf,
		     u32 sec_ch)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u32 fc = 2412;
	u8 direction = FREQ_POSITIVE;
	u32 tone_idx = 0;
	u8 set_result = PHYDM_SET_SUCCESS;
	u8 rpt = 0;

	if (enable == FUNC_DISABLE) {
		set_result = PHYDM_SET_SUCCESS;
	} else {
		PHYDM_DBG(dm, ODM_COMP_API,
			  "[Set NBI] CH = ((%d)), BW = ((%d)), f_intf = ((%d)), Scnd_CH = ((%s))\n",
			  ch, bw, f_intf,
			  (((sec_ch == PHYDM_DONT_CARE) || (bw == 20) ||
			  (ch > 14)) ? "Don't care" :
			  (sec_ch == PHYDM_ABOVE) ? "H" : "L"));

		/*@calculate fc*/
		if (phydm_find_fc(dm, ch, bw, sec_ch, &fc) == PHYDM_SET_FAIL) {
			set_result = PHYDM_SET_FAIL;
		} else {
			/*@calculate interference distance*/
			rpt = phydm_find_intf_distance(dm, bw, fc, f_intf,
						       &tone_idx);
			if (rpt == PHYDM_SET_SUCCESS) {
				if (f_intf >= fc)
					direction = FREQ_POSITIVE;
				else
					direction = FREQ_NEGATIVE;

				phydm_set_nbi_reg(dm, tone_idx, bw);

				set_result = PHYDM_SET_SUCCESS;
			} else {
				set_result = PHYDM_SET_NO_NEED;
		}
	}
	}

	if (set_result == PHYDM_SET_SUCCESS)
		phydm_nbi_enable(dm, enable);
	else
		phydm_nbi_enable(dm, FUNC_DISABLE);

	return set_result;
}

void phydm_nbi_debug(void *dm_void, char input[][16], u32 *_used, char *output,
		     u32 *_out_len)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u32 used = *_used;
	u32 out_len = *_out_len;
	u32 val[10] = {0};
	char help[] = "-h";
	u8 i = 0, input_idx = 0, idx_lmt = 0;
	u32 enable = 0; /*@function enable*/
	u32 ch = 0;
	u32 bw = 0;
	u32 f_int = 0; /*@interference frequency*/
	u32 sec_ch = 0; /*secondary channel*/
	u8 rpt = 0;
	u8 path = 0;

	if (dm->support_ic_type & ODM_IC_JGR3_SERIES)
		idx_lmt = 6;
	else
		idx_lmt = 5;
	for (i = 0; i < idx_lmt; i++) {
		if (input[i + 1]) {
			PHYDM_SSCANF(input[i + 1], DCMD_DECIMAL, &val[i]);
			input_idx++;
		}
	}

	if (input_idx == 0)
		return;

	enable = val[0];
	ch = val[1];
	bw = val[2];
	f_int = val[3];
	sec_ch = val[4];
	#ifdef PHYDM_IC_JGR3_SERIES_SUPPORT
	path = (u8)val[5];
	#endif

	if ((strcmp(input[1], help) == 0)) {
		#ifdef PHYDM_IC_JGR3_SERIES_SUPPORT
		if (dm->support_ic_type & ODM_IC_JGR3_SERIES)
			PDM_SNPF(out_len, used, output + used, out_len - used,
				 "{en:1 Dis all path:2} {ch} {BW:20/40/80} {f_intf(Mhz)} {Scnd_CH(L=1, H=2)} {Path:A~D(0~3)}\n");
		else
		#endif
			PDM_SNPF(out_len, used, output + used, out_len - used,
				 "{en:1 Dis:2} {ch} {BW:20/40/80} {f_intf(Mhz)} {Scnd_CH(L=1, H=2)}\n");
		*_used = used;
		*_out_len = out_len;
		return;
	} else if (val[0] == FUNC_ENABLE) {
		PDM_SNPF(out_len, used, output + used, out_len - used,
			 "[Enable NBI] CH = ((%d)), BW = ((%d)), f_intf = ((%d)), Scnd_CH = ((%s))\n",
			 ch, bw, f_int,
			 ((sec_ch == PHYDM_DONT_CARE) ||
			 (bw == 20) || (ch > 14)) ? "Don't care" :
			 ((sec_ch == PHYDM_ABOVE) ? "H" : "L"));
		#ifdef PHYDM_IC_JGR3_SERIES_SUPPORT
		if (dm->support_ic_type & ODM_IC_JGR3_SERIES)
			rpt = phydm_nbi_setting_jgr3(dm, enable, ch, bw, f_int,
						     sec_ch, path);
		else
		#endif
			rpt = phydm_nbi_setting(dm, enable, ch, bw, f_int,
						sec_ch);
	} else if (val[0] == FUNC_DISABLE) {
		PDM_SNPF(out_len, used, output + used, out_len - used,
			 "[Disable NBI]\n");
		#ifdef PHYDM_IC_JGR3_SERIES_SUPPORT
		if (dm->support_ic_type & ODM_IC_JGR3_SERIES)
			rpt = phydm_nbi_setting_jgr3(dm, enable, ch, bw, f_int,
						     sec_ch, path);
		else
		#endif
			rpt = phydm_nbi_setting(dm, enable, ch, bw, f_int,
						sec_ch);
	} else {
		rpt = PHYDM_SET_FAIL;
	}

	PDM_SNPF(out_len, used, output + used, out_len - used,
		 "[NBI set result: %s]\n",
		 (rpt == PHYDM_SET_SUCCESS) ? "Success" :
		 ((rpt == PHYDM_SET_NO_NEED) ? "No need" : "Error"));

	*_used = used;
	*_out_len = out_len;
}

void phydm_csi_debug(void *dm_void, char input[][16], u32 *_used, char *output,
		     u32 *_out_len)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u32 used = *_used;
	u32 out_len = *_out_len;
	u32 val[10] = {0};
	char help[] = "-h";
	u8 i = 0, input_idx = 0, idx_lmt = 0;
	u32 enable = 0;  /*@function enable*/
	u32 ch = 0;
	u32 bw = 0;
	u32 f_int = 0; /*@interference frequency*/
	u32 sec_ch = 0;  /*secondary channel*/
	u8 rpt = 0;
	u8 wgt = 0;

	if (dm->support_ic_type & ODM_IC_JGR3_SERIES)
		idx_lmt = 6;
	else
		idx_lmt = 5;

	for (i = 0; i < idx_lmt; i++) {
		if (input[i + 1]) {
			PHYDM_SSCANF(input[i + 1], DCMD_DECIMAL, &val[i]);
			input_idx++;
		}
	}

	if (input_idx == 0)
		return;

	enable = val[0];
	ch = val[1];
	bw = val[2];
	f_int = val[3];
	sec_ch = val[4];
	#ifdef PHYDM_IC_JGR3_SERIES_SUPPORT
	wgt = (u8)val[5];
	#endif

	if ((strcmp(input[1], help) == 0)) {
		#ifdef PHYDM_IC_JGR3_SERIES_SUPPORT
		if (dm->support_ic_type & ODM_IC_JGR3_SERIES)
			PDM_SNPF(out_len, used, output + used, out_len - used,
				 "{en:1 Dis:2} {ch} {BW:20/40/80} {f_intf(Mhz)} {Scnd_CH(L=1, H=2)}\n{wgt:7:3/4,6:2/1,5:1/4,4:1/8,3:1/16,2:1/32,1:1/64,0:0}\n");
		else
		#endif
			PDM_SNPF(out_len, used, output + used, out_len - used,
				 "{en:1 Dis:2} {ch} {BW:20/40/80} {f_intf(Mhz)} {Scnd_CH(L=1, H=2)}\n");

		*_used = used;
		*_out_len = out_len;
		return;

	} else if (val[0] == FUNC_ENABLE) {
		PDM_SNPF(out_len, used, output + used, out_len - used,
			 "[Enable CSI MASK] CH = ((%d)), BW = ((%d)), f_intf = ((%d)), Scnd_CH = ((%s))\n",
			 ch, bw, f_int,
			 (ch > 14) ? "Don't care" :
			 (((sec_ch == PHYDM_DONT_CARE) ||
			 (bw == 20) || (ch > 14)) ? "H" : "L"));
		#ifdef PHYDM_IC_JGR3_SERIES_SUPPORT
		if (dm->support_ic_type & ODM_IC_JGR3_SERIES)
			rpt = phydm_csi_mask_setting_jgr3(dm, enable, ch, bw,
							  f_int, sec_ch, wgt);
		else
		#endif
			rpt = phydm_csi_mask_setting(dm, enable, ch, bw, f_int,
						     sec_ch);
	} else if (val[0] == FUNC_DISABLE) {
		PDM_SNPF(out_len, used, output + used, out_len - used,
			 "[Disable CSI MASK]\n");
		#ifdef PHYDM_IC_JGR3_SERIES_SUPPORT
		if (dm->support_ic_type & ODM_IC_JGR3_SERIES)
			rpt = phydm_csi_mask_setting_jgr3(dm, enable, ch, bw,
							  f_int, sec_ch, wgt);
		else
		#endif
			rpt = phydm_csi_mask_setting(dm, enable, ch, bw, f_int,
						     sec_ch);
	} else {
		rpt = PHYDM_SET_FAIL;
	}
	PDM_SNPF(out_len, used, output + used, out_len - used,
		 "[CSI MASK set result: %s]\n",
		 (rpt == PHYDM_SET_SUCCESS) ? "Success" :
		 ((rpt == PHYDM_SET_NO_NEED) ? "No need" : "Error"));

	*_used = used;
	*_out_len = out_len;
}

void phydm_stop_ck320(void *dm_void, u8 enable)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u32 val = enable ? 1 : 0;

	if (dm->support_ic_type & ODM_IC_11AC_SERIES) {
		odm_set_bb_reg(dm, R_0x8b4, BIT(6), val);
	} else {
		if (dm->support_ic_type & ODM_IC_N_2SS) /*N-2SS*/
			odm_set_bb_reg(dm, R_0x87c, BIT(29), val);
		else /*N-1SS*/
			odm_set_bb_reg(dm, R_0x87c, BIT(31), val);
	}
}

boolean
phydm_set_bb_txagc_offset(void *dm_void, s8 power_offset, /*@(unit: dB)*/
			  u8 add_half_db /*@(+0.5 dB)*/)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	s8 power_idx = power_offset * 2;
	boolean set_success = false;

	PHYDM_DBG(dm, ODM_COMP_API, "power_offset=%d, add_half_db =%d\n",
		  power_offset, add_half_db);

	#if ODM_IC_11AC_SERIES_SUPPORT
	if (dm->support_ic_type & ODM_IC_11AC_SERIES) {
		if (power_offset > -16 || power_offset < 15) {
			if (add_half_db)
				power_idx += 1;

			power_idx &= 0x3f;

			PHYDM_DBG(dm, ODM_COMP_API, "Reg_idx =0x%x\n",
				  power_idx);
			odm_set_bb_reg(dm, R_0x8b4, 0x3f, power_idx);
			set_success = true;
		} else {
			pr_debug("[Warning] TX AGC Offset Setting error!");
		}
	}
	#endif

	#if ODM_IC_11N_SERIES_SUPPORT
	if (dm->support_ic_type & ODM_IC_11N_SERIES) {
		if (power_offset > -8 || power_offset < 7) {
			if (add_half_db)
				power_idx += 1;

			power_idx &= 0x1f;

			PHYDM_DBG(dm, ODM_COMP_API, "Reg_idx =0x%x\n",
				  power_idx);
			/*r_txagc_offset_a*/
			odm_set_bb_reg(dm, R_0x80c, 0x1f00, power_idx);
			/*r_txagc_offset_b*/
			odm_set_bb_reg(dm, R_0x80c, 0x3e000, power_idx);
			set_success = true;
		} else {
			pr_debug("[Warning] TX AGC Offset Setting error!");
		}
	}
	#endif

	return set_success;
}

#ifdef PHYDM_COMMON_API_SUPPORT
boolean
phydm_api_shift_txagc(void *dm_void, u32 pwr_offset, enum rf_path path,
		      boolean is_positive) {
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	boolean ret = false;
	u32 txagc_cck = 0;
	u32 txagc_ofdm = 0;
	u32 r_txagc_ofdm[4] = {0x18e8, 0x41e8, 0x52e8, 0x53e8};
	u32 r_txagc_cck[4] = {0x18a0, 0x41a0, 0x52a0, 0x53a0};

	#if (RTL8822C_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8822C) {
		if (path > RF_PATH_B) {
			PHYDM_DBG(dm, ODM_PHY_CONFIG, "Unsupported path (%d)\n",
				  path);
			return false;
		}
		txagc_cck = (u8)odm_get_bb_reg(dm, r_txagc_cck[path],
						   0x7F0000);
		txagc_ofdm = (u8)odm_get_bb_reg(dm, r_txagc_ofdm[path],
						    0x1FC00);
		if (is_positive) {
			if (((txagc_cck + pwr_offset) > 127) ||
			    ((txagc_ofdm + pwr_offset) > 127))
				return false;

			txagc_cck += pwr_offset;
			txagc_ofdm += pwr_offset;
		} else {
			if (pwr_offset > txagc_cck || pwr_offset > txagc_ofdm)
				return false;

			txagc_cck -= pwr_offset;
			txagc_ofdm -= pwr_offset;
		}

		ret = config_phydm_write_txagc_ref_8822c(dm, (u8)txagc_cck,
							 path, PDM_CCK);
		ret &= config_phydm_write_txagc_ref_8822c(dm, (u8)txagc_ofdm,
							 path, PDM_OFDM);
		PHYDM_DBG(dm, ODM_PHY_CONFIG,
			  "%s: path-%d txagc_cck_ref=%x txagc_ofdm_ref=0x%x\n",
			  __func__, path, txagc_cck, txagc_ofdm);
	}
	#endif

	return ret;
}

boolean
phydm_api_set_txagc(void *dm_void, u32 pwr_idx, enum rf_path path,
		    u8 rate, boolean is_single_rate)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	boolean ret = false;
	#if (RTL8198F_SUPPORT || RTL8822C_SUPPORT)
	u8 base = 0;
	u8 txagc_tmp = 0;
	s8 pw_by_rate_tmp = 0;
	s8 pw_by_rate_new = 0;
	#endif
	#if (DM_ODM_SUPPORT_TYPE & ODM_AP)
	u8 i = 0;
	#endif

#if (RTL8822B_SUPPORT || RTL8821C_SUPPORT || RTL8195B_SUPPORT)
	if (dm->support_ic_type &
	    (ODM_RTL8822B | ODM_RTL8821C | ODM_RTL8195B)) {
		if (is_single_rate) {
			#if (RTL8822B_SUPPORT)
			if (dm->support_ic_type == ODM_RTL8822B)
				ret = phydm_write_txagc_1byte_8822b(dm, pwr_idx,
								    path, rate);
			#endif

			#if (RTL8821C_SUPPORT)
			if (dm->support_ic_type == ODM_RTL8821C)
				ret = phydm_write_txagc_1byte_8821c(dm, pwr_idx,
								    path, rate);
			#endif

			#if (RTL8195B_SUPPORT)
			if (dm->support_ic_type == ODM_RTL8195B)
				ret = phydm_write_txagc_1byte_8195b(dm, pwr_idx,
								    path, rate);
			#endif

			#if (DM_ODM_SUPPORT_TYPE & ODM_AP)
			set_current_tx_agc(dm->priv, path, rate, (u8)pwr_idx);
			#endif

		} else {
			#if (RTL8822B_SUPPORT)
			if (dm->support_ic_type == ODM_RTL8822B)
				ret = config_phydm_write_txagc_8822b(dm,
								     pwr_idx,
								     path,
								     rate);
			#endif

			#if (RTL8821C_SUPPORT)
			if (dm->support_ic_type == ODM_RTL8821C)
				ret = config_phydm_write_txagc_8821c(dm,
								     pwr_idx,
								     path,
								     rate);
			#endif

			#if (RTL8195B_SUPPORT)
			if (dm->support_ic_type == ODM_RTL8195B)
				ret = config_phydm_write_txagc_8195b(dm,
								     pwr_idx,
								     path,
								     rate);
			#endif

			#if (DM_ODM_SUPPORT_TYPE & ODM_AP)
			for (i = 0; i < 4; i++)
				set_current_tx_agc(dm->priv, path, (rate + i),
						   (u8)pwr_idx);
			#endif
		}
	}
#endif

#if (RTL8198F_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8198F) {
		if (rate < 0x4)
			txagc_tmp = config_phydm_read_txagc_8198f(dm, path,
								  rate,
								  PDM_CCK);
		else
			txagc_tmp = config_phydm_read_txagc_8198f(dm, path,
								  rate,
								  PDM_OFDM);

		pw_by_rate_tmp = config_phydm_read_txagc_diff_8198f(dm, rate);
		base = txagc_tmp -  pw_by_rate_tmp;
		if (DIFF_2(pwr_idx, base) > 63)
			return false;

		pw_by_rate_new = (s8)(pwr_idx - base);
		ret = phydm_write_txagc_1byte_8198f(dm, pw_by_rate_new, rate);
		PHYDM_DBG(dm, ODM_PHY_CONFIG,
			  "%s: path-%d rate_idx=%x base=0x%x new_diff=0x%x\n",
			  __func__, path, rate, base, pw_by_rate_new);
	}
#endif

#if (RTL8822C_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8822C) {
		if (rate < 0x4)
			txagc_tmp = config_phydm_read_txagc_8822c(dm, path,
								  rate,
								  PDM_CCK);
		else
			txagc_tmp = config_phydm_read_txagc_8822c(dm, path,
								  rate,
								  PDM_OFDM);

		pw_by_rate_tmp = config_phydm_read_txagc_diff_8822c(dm, rate);
		base = txagc_tmp - pw_by_rate_tmp;
		if (DIFF_2(pwr_idx, base) > 63)
			return false;

		pw_by_rate_new = (s8)(pwr_idx - base);
		ret = phydm_write_txagc_1byte_8822c(dm, pw_by_rate_new, rate);
		PHYDM_DBG(dm, ODM_PHY_CONFIG,
			  "%s: path-%d rate_idx=%x base=0x%x new_diff=0x%x\n",
			  __func__, path, rate, base, pw_by_rate_new);
	}
#endif

#if (RTL8197F_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8197F)
		ret = config_phydm_write_txagc_8197f(dm, pwr_idx, path, rate);
#endif

#if (RTL8192F_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8192F)
		ret = config_phydm_write_txagc_8192f(dm, pwr_idx, path, rate);
#endif
	return ret;
}

u8 phydm_api_get_txagc(void *dm_void, enum rf_path path, u8 hw_rate)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u8 ret = 0;

#if (RTL8822B_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8822B)
		ret = config_phydm_read_txagc_8822b(dm, path, hw_rate);
#endif

#if (RTL8197F_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8197F)
		ret = config_phydm_read_txagc_8197f(dm, path, hw_rate);
#endif

#if (RTL8821C_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8821C)
		ret = config_phydm_read_txagc_8821c(dm, path, hw_rate);
#endif

#if (RTL8195B_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8195B)
		ret = config_phydm_read_txagc_8195b(dm, path, hw_rate);
#endif

/*@jj add 20170822*/
#if (RTL8192F_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8192F)
		ret = config_phydm_read_txagc_8192f(dm, path, hw_rate);
#endif

#if (RTL8198F_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8198F) {
		if (hw_rate < 0x4) {
			ret = config_phydm_read_txagc_8198f(dm, path, hw_rate,
							    PDM_CCK);
		} else {
			ret = config_phydm_read_txagc_8198f(dm, path, hw_rate,
							    PDM_OFDM);
		}
	}
#endif

#if (RTL8822C_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8822C) {
		if (hw_rate < 0x4) {
			ret = config_phydm_read_txagc_8822c(dm, path, hw_rate,
							    PDM_CCK);
		} else {
			ret = config_phydm_read_txagc_8822c(dm, path, hw_rate,
							    PDM_OFDM);
		}
	}
#endif
	return ret;
}

boolean
phydm_api_switch_bw_channel(void *dm_void, u8 ch, u8 pri_ch,
			    enum channel_width bw)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	boolean ret = false;

#if (RTL8822B_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8822B)
		ret = config_phydm_switch_channel_bw_8822b(dm, ch, pri_ch, bw);
#endif

#if (RTL8197F_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8197F)
		ret = config_phydm_switch_channel_bw_8197f(dm, ch, pri_ch, bw);
#endif

#if (RTL8821C_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8821C)
		ret = config_phydm_switch_channel_bw_8821c(dm, ch, pri_ch, bw);
#endif

/*@jj add 20170822*/
#if (RTL8192F_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8192F)
		ret = config_phydm_switch_channel_bw_8192f(dm, ch, pri_ch, bw);
#endif

#if (RTL8198F_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8198F)
		ret = config_phydm_switch_channel_bw_8198f(dm, ch, pri_ch, bw);
#endif

#if (RTL8822C_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8822C)
		ret = config_phydm_switch_channel_bw_8822c(dm, ch, pri_ch, bw);
#endif
	return ret;
}

boolean
phydm_api_trx_mode(void *dm_void, enum bb_path tx_path, enum bb_path rx_path,
		   boolean is_2tx)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	boolean ret = false;

#if (RTL8822B_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8822B)
		ret = config_phydm_trx_mode_8822b(dm, tx_path, rx_path, is_2tx);
#endif

#if (RTL8197F_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8197F)
		ret = config_phydm_trx_mode_8197f(dm, tx_path, rx_path, is_2tx);
#endif

#if (RTL8192F_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8192F)
		ret = config_phydm_trx_mode_8192f(dm, tx_path, rx_path, is_2tx);
#endif

#if (RTL8198F_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8198F)
		ret = config_phydm_trx_mode_8198f(dm, tx_path, rx_path, is_2tx);
#endif

#if (RTL8822C_SUPPORT)
	if (dm->support_ic_type & ODM_RTL8822C)
		ret = config_phydm_trx_mode_8822c(dm, tx_path, rx_path, is_2tx);
#endif
	return ret;
}
#else
u8 config_phydm_read_txagc_n(void *dm_void, enum rf_path path, u8 hw_rate)
{
	struct dm_struct *dm = (struct dm_struct *)dm_void;
	u8 read_back_data = INVALID_TXAGC_DATA;
	u32 reg_txagc;
	u32 reg_mask;
	/* This function is for 92E/88E etc... */
	/* @Input need to be HW rate index, not driver rate index!!!! */

	/* @Error handling */
	if (path > RF_PATH_B || hw_rate > ODM_RATEMCS15) {
		PHYDM_DBG(dm, ODM_PHY_CONFIG, "%s: unsupported path (%d)\n",
			  __func__, path);
		return INVALID_TXAGC_DATA;
	}

	if (path == RF_PATH_A) {
		switch (hw_rate) {
		case ODM_RATE1M:
			reg_txagc = R_0xe08;
			reg_mask = 0x00007f00;
			break;
		case ODM_RATE2M:
			reg_txagc = R_0x86c;
			reg_mask = 0x00007f00;
			break;
		case ODM_RATE5_5M:
			reg_txagc = R_0x86c;
			reg_mask = 0x007f0000;
			break;
		case ODM_RATE11M:
			reg_txagc = R_0x86c;
			reg_mask = 0x7f000000;
			break;

		case ODM_RATE6M:
			reg_txagc = R_0xe00;
			reg_mask = 0x0000007f;
			break;
		case ODM_RATE9M:
			reg_txagc = R_0xe00;
			reg_mask = 0x00007f00;
			break;
		case ODM_RATE12M:
			reg_txagc = R_0xe00;
			reg_mask = 0x007f0000;
			break;
		case ODM_RATE18M:
			reg_txagc = R_0xe00;
			reg_mask = 0x7f000000;
			break;
		case ODM_RATE24M:
			reg_txagc = R_0xe04;
			reg_mask = 0x0000007f;
			break;
		case ODM_RATE36M:
			reg_txagc = R_0xe04;
			reg_mask = 0x00007f00;
			break;
		case ODM_RATE48M:
			reg_txagc = R_0xe04;
			reg_mask = 0x007f0000;
			break;
		case ODM_RATE54M:
			reg_txagc = R_0xe04;
			reg_mask = 0x7f000000;
			break;

		case ODM_RATEMCS0:
			reg_txagc = R_0xe10;
			reg_mask = 0x0000007f;
			break;
		case ODM_RATEMCS1:
			reg_txagc = R_0xe10;
			reg_mask = 0x00007f00;
			break;
		case ODM_RATEMCS2:
			reg_txagc = R_0xe10;
			reg_mask = 0x007f0000;
			break;
		case ODM_RATEMCS3:
			reg_txagc = R_0xe10;
			reg_mask = 0x7f000000;
			break;
		case ODM_RATEMCS4:
			reg_txagc = R_0xe14;
			reg_mask = 0x0000007f;
			break;
		case ODM_RATEMCS5:
			reg_txagc = R_0xe14;
			reg_mask = 0x00007f00;
			break;
		case ODM_RATEMCS6:
			reg_txagc = R_0xe14;
			reg_mask = 0x007f0000;
			break;
		case ODM_RATEMCS7:
			reg_txagc = R_0xe14;
			reg_mask = 0x7f000000;
			break;

		case ODM_RATEMCS8:
			reg_txagc = R_0xe18;
			reg_mask = 0x0000007f;
			break;
		case ODM_RATEMCS9:
			reg_txagc = R_0xe18;
			reg_mask = 0x00007f00;
			break;
		case ODM_RATEMCS10:
			reg_txagc = R_0xe18;
			reg_mask = 0x007f0000;
			break;
		case ODM_RATEMCS11:
			reg_txagc = R_0xe18;
			reg_mask = 0x7f000000;
			break;
		case ODM_RATEMCS12:
			reg_txagc = R_0xe1c;
			reg_mask = 0x0000007f;
			break;
		case ODM_RATEMCS13:
			reg_txagc = R_0xe1c;
			reg_mask = 0x00007f00;
			break;
		case ODM_RATEMCS14:
			reg_txagc = R_0xe1c;
			reg_mask = 0x007f0000;
			break;
		case ODM_RATEMCS15:
			reg_txagc = R_0xe1c;
			reg_mask = 0x7f000000;
			break;

		default:
			PHYDM_DBG(dm, ODM_PHY_CONFIG, "Invalid HWrate!\n");
			break;
		}
	} else if (path == RF_PATH_B) {
		switch (hw_rate) {
		case ODM_RATE1M:
			reg_txagc = R_0x838;
			reg_mask = 0x00007f00;
			break;
		case ODM_RATE2M:
			reg_txagc = R_0x838;
			reg_mask = 0x007f0000;
			break;
		case ODM_RATE5_5M:
			reg_txagc = R_0x838;
			reg_mask = 0x7f000000;
			break;
		case ODM_RATE11M:
			reg_txagc = R_0x86c;
			reg_mask = 0x0000007f;
			break;

		case ODM_RATE6M:
			reg_txagc = R_0x830;
			reg_mask = 0x0000007f;
			break;
		case ODM_RATE9M:
			reg_txagc = R_0x830;
			reg_mask = 0x00007f00;
			break;
		case ODM_RATE12M:
			reg_txagc = R_0x830;
			reg_mask = 0x007f0000;
			break;
		case ODM_RATE18M:
			reg_txagc = R_0x830;
			reg_mask = 0x7f000000;
			break;
		case ODM_RATE24M:
			reg_txagc = R_0x834;
			reg_mask = 0x0000007f;
			break;
		case ODM_RATE36M:
			reg_txagc = R_0x834;
			reg_mask = 0x00007f00;
			break;
		case ODM_RATE48M:
			reg_txagc = R_0x834;
			reg_mask = 0x007f0000;
			break;
		case ODM_RATE54M:
			reg_txagc = R_0x834;
			reg_mask = 0x7f000000;
			break;

		case ODM_RATEMCS0:
			reg_txagc = R_0x83c;
			reg_mask = 0x0000007f;
			break;
		case ODM_RATEMCS1:
			reg_txagc = R_0x83c;
			reg_mask = 0x00007f00;
			break;
		case ODM_RATEMCS2:
			reg_txagc = R_0x83c;
			reg_mask = 0x007f0000;
			break;
		case ODM_RATEMCS3:
			reg_txagc = R_0x83c;
			reg_mask = 0x7f000000;
			break;
		case ODM_RATEMCS4:
			reg_txagc = R_0x848;
			reg_mask = 0x0000007f;
			break;
		case ODM_RATEMCS5:
			reg_txagc = R_0x848;
			reg_mask = 0x00007f00;
			break;
		case ODM_RATEMCS6:
			reg_txagc = R_0x848;
			reg_mask = 0x007f0000;
			break;
		case ODM_RATEMCS7:
			reg_txagc = R_0x848;
			reg_mask = 0x7f000000;
			break;

		case ODM_RATEMCS8:
			reg_txagc = R_0x84c;
			reg_mask = 0x0000007f;
			break;
		case ODM_RATEMCS9:
			reg_txagc = R_0x84c;
			reg_mask = 0x00007f00;
			break;
		case ODM_RATEMCS10:
			reg_txagc = R_0x84c;
			reg_mask = 0x007f0000;
			break;
		case ODM_RATEMCS11:
			reg_txagc = R_0x84c;
			reg_mask = 0x7f000000;
			break;
		case ODM_RATEMCS12:
			reg_txagc = R_0x868;
			reg_mask = 0x0000007f;
			break;
		case ODM_RATEMCS13:
			reg_txagc = R_0x868;
			reg_mask = 0x00007f00;
			break;
		case ODM_RATEMCS14:
			reg_txagc = R_0x868;
			reg_mask = 0x007f0000;
			break;
		case ODM_RATEMCS15:
			reg_txagc = R_0x868;
			reg_mask = 0x7f000000;
			break;

		default:
			PHYDM_DBG(dm, ODM_PHY_CONFIG, "Invalid HWrate!\n");
			break;
		}
	} else {
		PHYDM_DBG(dm, ODM_PHY_CONFIG, "Invalid RF path!!\n");
	}
	read_back_data = (u8)odm_get_bb_reg(dm, reg_txagc, reg_mask);
	PHYDM_DBG(dm, ODM_PHY_CONFIG, "%s: path-%d rate index 0x%x = 0x%x\n",
		  __func__, path, hw_rate, read_back_data);
	return read_back_data;
}
#endif

#if (DM_ODM_SUPPORT_TYPE == ODM_WIN)
void phydm_normal_driver_rx_sniffer(
	struct dm_struct *dm,
	u8 *desc,
	PRT_RFD_STATUS rt_rfd_status,
	u8 *drv_info,
	u8 phy_status)
{
#if (defined(CONFIG_PHYDM_RX_SNIFFER_PARSING))
	u32 *msg;
	u16 seq_num;

	if (rt_rfd_status->packet_report_type != NORMAL_RX)
		return;

	if (!dm->is_linked) {
		if (rt_rfd_status->is_hw_error)
			return;
	}

	if (phy_status == true) {
		if (dm->rx_pkt_type == type_block_ack ||
		    dm->rx_pkt_type == type_rts || dm->rx_pkt_type == type_cts)
			seq_num = 0;
		else
			seq_num = rt_rfd_status->seq_num;

		PHYDM_DBG_F(dm, ODM_COMP_SNIFFER,
			    "%04d , %01s, rate=0x%02x, L=%04d , %s , %s",
			    seq_num,
			    /*rt_rfd_status->mac_id,*/
			    (rt_rfd_status->is_crc ? "C" :
			    rt_rfd_status->is_ampdu ? "A" : "_"),
			    rt_rfd_status->data_rate,
			    rt_rfd_status->length,
			    ((rt_rfd_status->band_width == 0) ? "20M" :
			    ((rt_rfd_status->band_width == 1) ? "40M" : "80M")),
			    (rt_rfd_status->is_ldpc ? "LDP" : "BCC"));

		if (dm->rx_pkt_type == type_asoc_req)
			PHYDM_DBG_F(dm, ODM_COMP_SNIFFER, " , [%s]", "AS_REQ");
		else if (dm->rx_pkt_type == type_asoc_rsp)
			PHYDM_DBG_F(dm, ODM_COMP_SNIFFER, " , [%s]", "AS_RSP");
		else if (dm->rx_pkt_type == type_probe_req)
			PHYDM_DBG_F(dm, ODM_COMP_SNIFFER, " , [%s]", "PR_REQ");
		else if (dm->rx_pkt_type == type_probe_rsp)
			PHYDM_DBG_F(dm, ODM_COMP_SNIFFER, " , [%s]", "PR_RSP");
		else if (dm->rx_pkt_type == type_deauth)
			PHYDM_DBG_F(dm, ODM_COMP_SNIFFER, " , [%s]", "DEAUTH");
		else if (dm->rx_pkt_type == type_beacon)
			PHYDM_DBG_F(dm, ODM_COMP_SNIFFER, " , [%s]", "BEACON");
		else if (dm->rx_pkt_type == type_block_ack_req)
			PHYDM_DBG_F(dm, ODM_COMP_SNIFFER, " , [%s]", "BA_REQ");
		else if (dm->rx_pkt_type == type_rts)
			PHYDM_DBG_F(dm, ODM_COMP_SNIFFER, " , [%s]", "__RTS_");
		else if (dm->rx_pkt_type == type_cts)
			PHYDM_DBG_F(dm, ODM_COMP_SNIFFER, " , [%s]", "__CTS_");
		else if (dm->rx_pkt_type == type_ack)
			PHYDM_DBG_F(dm, ODM_COMP_SNIFFER, " , [%s]", "__ACK_");
		else if (dm->rx_pkt_type == type_block_ack)
			PHYDM_DBG_F(dm, ODM_COMP_SNIFFER, " , [%s]", "__BA__");
		else if (dm->rx_pkt_type == type_data)
			PHYDM_DBG_F(dm, ODM_COMP_SNIFFER, " , [%s]", "_DATA_");
		else if (dm->rx_pkt_type == type_data_ack)
			PHYDM_DBG_F(dm, ODM_COMP_SNIFFER, " , [%s]", "Data_Ack");
		else if (dm->rx_pkt_type == type_qos_data)
			PHYDM_DBG_F(dm, ODM_COMP_SNIFFER, " , [%s]", "QoS_Data");
		else
			PHYDM_DBG_F(dm, ODM_COMP_SNIFFER, " , [0x%x]",
				    dm->rx_pkt_type);

		PHYDM_DBG_F(dm, ODM_COMP_SNIFFER, " , [RSSI=%d,%d,%d,%d ]",
			    dm->rssi_a,
			    dm->rssi_b,
			    dm->rssi_c,
			    dm->rssi_d);

		msg = (u32 *)drv_info;

		PHYDM_DBG_F(dm, ODM_COMP_SNIFFER,
			    " , P-STS[28:0]=%08x-%08x-%08x-%08x-%08x-%08x-%08x\n",
			    msg[6], msg[5], msg[4], msg[3],
			    msg[2], msg[1], msg[1]);
	} else {
		PHYDM_DBG_F(dm, ODM_COMP_SNIFFER,
			    "%04d , %01s, rate=0x%02x, L=%04d , %s , %s\n",
			    rt_rfd_status->seq_num,
			    /*rt_rfd_status->mac_id,*/
			    (rt_rfd_status->is_crc ? "C" :
			    (rt_rfd_status->is_ampdu) ? "A" : "_"),
			    rt_rfd_status->data_rate,
			    rt_rfd_status->length,
			    ((rt_rfd_status->band_width == 0) ? "20M" :
			    ((rt_rfd_status->band_width == 1) ? "40M" : "80M")),
			    (rt_rfd_status->is_ldpc ? "LDP" : "BCC"));
	}

#endif
}
#endif
