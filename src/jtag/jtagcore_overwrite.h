// SPDX-License-Identifier: GPL-2.0-or-later

#ifndef OPENOCD_JTAG_JTAGCORE_OVERWRITE_H
#define OPENOCD_JTAG_JTAGCORE_OVERWRITE_H

/* Adapter hooks for scan-chain discovery implementations such as Intel AJI. */
struct jtagcore_overwrite {
	int (*jtag_examine_chain)(void);
	int (*jtag_validate_ircapture)(void);
};

struct jtagcore_overwrite *jtagcore_get_overwrite_record(void);

#endif /* OPENOCD_JTAG_JTAGCORE_OVERWRITE_H */
