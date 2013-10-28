/*
 * platform_max3111.c: max3111 platform data initilization file
 *
 * (C) Copyright 2013 Intel Corporation
 * Author: Sathyanarayanan Kuppuswamy <sathyanarayanan.kuppuswamy@intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */

#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/spi/intel_mid_ssp_spi.h>
#include <linux/spi/spi.h>
#include <asm/intel-mid.h>

static struct intel_mid_ssp_spi_chip chip = {
	.burst_size = DFLT_FIFO_BURST_SIZE,
	.timeout = DFLT_TIMEOUT_VAL,
};

static void __init *max3111_platform_data(void *info)
{
	struct spi_board_info *spi_info = info;
	int intr;

	spi_info->mode = SPI_MODE_0;

	if (intel_mid_identify_cpu() == INTEL_MID_CPU_CHIP_TANGIER) {
		spi_info->controller_data = &chip;

/* FIXME: workaround for invalid bus number in IAFW .25 */
#define FORCE_SPI_BUS_NUM	5

		spi_info->bus_num = FORCE_SPI_BUS_NUM;
	} else {
		intr = get_gpio_by_name("max3111_int");
		if (intr < 0) {
			pr_err("%s: failed to get gpio 'max3111_int'\n",
			       __func__);
			return ERR_PTR(intr);
		}
		spi_info->irq = intr + INTEL_MID_IRQ_OFFSET;
	}

	return NULL;
}

static const struct devs_id max3111_dev_id __initconst = {
	.name = "spi_max3111",
	.type = SFI_DEV_TYPE_SPI,
	.get_platform_data = &max3111_platform_data,
};

sfi_device(max3111_dev_id);
