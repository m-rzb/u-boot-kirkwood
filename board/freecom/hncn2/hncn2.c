/*
 * Copyright (C) 2015-2017 bodhi <mibodhi@gmail.com>
 *
 * Based on
 * Copyright (C) 2014  Jason Plum <jplum@archlinuxarm.org>
 *
 * Based on nsa320.c originall written by
 * Copyright (C) 2012  Peter Schildmann <linux@schildmann.info>
 *
 * Based on guruplug.c originally written by
 * Siddarth Gore <gores@marvell.com>
 * (C) Copyright 2009
 * Marvell Semiconductor <www.marvell.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#include <common.h>
#include <miiphy.h>
#include <asm/arch/soc.h>
#include <asm/arch/mpp.h>
#include <asm/arch/cpu.h>
#include <asm/gpio.h>
#include <asm/io.h>
#include "hncn2.h"
#include <asm/arch/gpio.h>
#include <asm/mach-types.h>

DECLARE_GLOBAL_DATA_PTR;

int board_early_init_f(void)
{
	/*
	 * default gpio configuration
	 * There are maximum 64 gpios controlled through 2 sets of registers   
	 * the below configuration configures mainly initial LED status
	 */
	mvebu_config_gpio(HNCN2_OE_VAL_LOW, HNCN2_OE_VAL_HIGH,
		       HNCN2_OE_LOW, HNCN2_OE_HIGH);

	/* Multi-Purpose Pins Functionality configuration */
	/* (all LEDs & power off active high) */
	u32 kwmpp_config[] = {
		MPP0_NF_IO2,
		MPP1_NF_IO3,
		MPP2_NF_IO4,
		MPP3_NF_IO5,
		MPP4_NF_IO6,
		MPP5_NF_IO7,
		MPP6_SYSRST_OUTn,
		MPP7_GPO,           /* disk_0 pmx-led-hdd1-red  */
		MPP8_TW_SDA,		
		MPP9_TW_SCK,		
		MPP10_UART0_TXD,
		MPP11_UART0_RXD,
		MPP12_GPO,		
		MPP13_GPIO,		/* disk_1 pmx-led-hdd2-red     */
		MPP14_GPIO,		/* SATA1 Present  */
		MPP15_GPIO,		/* SATA0 ACT    */
		MPP16_GPIO,		/* SATA1 ACT  */
		MPP17_GPIO,		/* SATA0 Present  */
		MPP18_NF_IO0,
		MPP19_NF_IO1,
		MPP20_GPIO,
		MPP21_GPIO,		
		MPP22_GPIO,
		MPP23_GPIO,
		MPP24_GPIO,
		MPP25_GPIO,
		MPP26_GPIO,
		MPP27_GPIO,
		MPP28_GPIO,		
		MPP29_GPIO,		/* RESET_BUTTON pmx-btn-reset   */
		MPP30_GPIO,
		MPP31_GPIO,
		MPP32_GPIO,
		MPP33_GPIO,
		MPP34_GPIO,     /* FAN_POWER */  
		MPP35_GPIO,     /* HDD_POWER */
		MPP36_GPIO,		/* SHUTDOWN pmx-pwr-off          */
		MPP37_GPIO,		/* syserr + USB_VBUS pmx-led-status-red    */
		MPP38_GPIO,		/* syssts pmx-led-status-white  */
		MPP39_GPIO,		/* pmx-led-power-white   */
		MPP40_GPIO,		
		MPP41_GPIO,		
		MPP42_GPIO,		
		MPP43_GPIO,		
		MPP44_GPIO,		
		MPP45_GPIO,		
		MPP46_GPIO,		
		MPP47_GPIO,		/* pmx-pwr-sata1         */
		MPP48_GPIO,		
        MPP49_GPIO,		/* POWER_BUTTON pmx-btn-power          */
        0
	};
	kirkwood_mpp_conf(kwmpp_config, NULL);
	return 0;
}

int board_init(void)
{
	/*
	 * arch number of board
	 */
	gd->bd->bi_arch_number = MACH_TYPE_HNCN2;	


	/* address of boot parameters */
	gd->bd->bi_boot_params = mvebu_sdram_bar(0) + 0x100;

	return 0;
}

#ifdef CONFIG_RESET_PHY_R
/* Configure and enable MV88E1318 PHY */
void reset_phy(void)
{
	u16 reg;
	u16 devadr;
	char *name = "egiga0";

	if (miiphy_set_current_dev(name))
		return;

	/* command to read PHY dev address */
	if (miiphy_read(name, 0xEE, 0xEE, (u16 *) &devadr)) {
		printf("Err..%s could not read PHY dev address\n",
			__FUNCTION__);
		return;
	}

	/* Set RGMII delay */
	miiphy_write(name, devadr, MV88E1318_PGADR_REG, MV88E1318_MAC_CTRL_PG);
	miiphy_read(name, devadr, MV88E1318_MAC_CTRL_REG, &reg);
	reg |= (MV88E1318_RGMII_RXTM_CTRL | MV88E1318_RGMII_TXTM_CTRL);
	miiphy_write(name, devadr, MV88E1318_MAC_CTRL_REG, reg);
	miiphy_write(name, devadr, MV88E1318_PGADR_REG, 0);

	/* reset the phy */
	miiphy_reset(name, devadr);

	/* The ZyXEL NSA325 uses the 88E1310S Alaska (interface identical to 88E1318) */
	/* and has an MCU attached to the LED[2] via tristate interrupt */
	reg = 0;

	/* switch to LED register page */
	miiphy_write(name, devadr, MV88E1318_PGADR_REG, MV88E1318_LED_PG);
	/* read out LED polarity register */
	miiphy_read(name, devadr, MV88E1318_LED_POL_REG, &reg);
	/* clear 4, set 5 - LED2 low, tri-state */
	reg &= ~(MV88E1318_LED2_4);
	reg |= (MV88E1318_LED2_5);
	/* write back LED polarity register */
	miiphy_write(name, devadr, MV88E1318_LED_POL_REG, reg);
	/* jump back to page 0, per the PHY chip documenation. */
	miiphy_write(name, devadr, MV88E1318_PGADR_REG, 0);

	/* Set the phy back to auto-negotiation mode */
	miiphy_write(name, devadr, 0x4, 0x1e1);
	miiphy_write(name, devadr, 0x9, 0x300);
	/* Downshift */
	miiphy_write(name, devadr, 0x10, 0x3860);
	miiphy_write(name, devadr, 0x0, 0x9140);

	printf("MV88E1318 PHY initialized on %s\n", name);

}
#endif /* CONFIG_RESET_PHY_R */

