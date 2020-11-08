/*
 * Copyright (C) 2014  Jason Plum <jplum@archlinuxarm.org>
 *
 * Based on nsa320.h originall written by
 * Copyright (C) 2012  Peter Schildmann <linux@schildmann.info>
 *
 * Based on guruplug.h originally written by
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

#ifndef __HNCN2_H
#define __HNCN2_H

#define HNCN2_OE_LOW                       (~(1 << 7 | 1 << 13 | 1 << 14 | 1 << 15 | 1 << 16 | 1 << 17 | 1 << 28))
#define HNCN2_OE_HIGH                      (~(1 << 16 | 1 << 5 | 1 << 6 | 1 << 4))
#define HNCN2_OE_VAL_LOW                   (1 << 28)
#define HNCN2_OE_VAL_HIGH                  (1 << 16 | 1 << 6)


/* PHY related */
#define MV88E1318_PGADR_REG		22
#define MV88E1318_MAC_CTRL_REG	21
#define MV88E1318_MAC_CTRL_PG	2
#define MV88E1318_RGMII_TXTM_CTRL	(1 << 4)
#define MV88E1318_RGMII_RXTM_CTRL	(1 << 5)
#define MV88E1318_LED_PG        3
#define MV88E1318_LED_POL_REG	17
#define MV88E1318_LED2_4	    (1 << 4)
#define MV88E1318_LED2_5	    (1 << 5)


#endif /* __HNCN2_H */
