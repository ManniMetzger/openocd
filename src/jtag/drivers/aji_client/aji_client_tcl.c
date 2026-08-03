// SPDX-License-Identifier: GPL-2.0-or-later

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <helper/command.h>
#include <helper/log.h>
#include <jtag/jtag.h>

__COMMAND_HANDLER(handle_aji_vjtag_create)
{
	if (CMD_ARGC < 1)
		return ERROR_COMMAND_SYNTAX_ERROR;

	const char *name = CMD_ARGV[0];
	struct jtag_tap *parent = NULL;
	uint32_t expected_id = 0;
	bool have_expected_id = false;
	bool ignore_version = false;

	for (unsigned int i = 1; i < CMD_ARGC; i++) {
		const char *option = CMD_ARGV[i];
		if (!strcmp(option, "-chain-position") && ++i < CMD_ARGC) {
			parent = jtag_tap_by_string(CMD_ARGV[i]);
		} else if (!strcmp(option, "-expected-id") && ++i < CMD_ARGC) {
			COMMAND_PARSE_NUMBER(u32, CMD_ARGV[i], expected_id);
			have_expected_id = true;
		} else if (!strcmp(option, "-ignore-version")) {
			ignore_version = true;
		} else if (!strcmp(option, "-bridge") && ++i < CMD_ARGC) {
			LOG_WARNING("AJI vjtag -bridge is not implemented; ignoring '%s'", CMD_ARGV[i]);
		} else {
			command_print(CMD, "unknown or incomplete vjtag option '%s'", option);
			return ERROR_COMMAND_ARGUMENT_INVALID;
		}
	}

	if (!parent || parent->is_virtual) {
		command_print(CMD, "-chain-position must name a physical TAP");
		return ERROR_COMMAND_ARGUMENT_INVALID;
	}
	if (!have_expected_id) {
		command_print(CMD, "exactly one -expected-id is required");
		return ERROR_COMMAND_ARGUMENT_INVALID;
	}
	if (jtag_tap_by_string(name)) {
		command_print(CMD, "TAP '%s' already exists", name);
		return ERROR_COMMAND_ARGUMENT_INVALID;
	}

	struct jtag_tap *tap = calloc(1, sizeof(*tap));
	if (!tap)
		return ERROR_FAIL;

	tap->dotted_name = strdup(name);
	tap->chip = parent->chip ? strdup(parent->chip) : NULL;
	tap->tapname = parent->tapname ? strdup(parent->tapname) : NULL;
	tap->expected_ids = malloc(sizeof(*tap->expected_ids));
	if (!tap->dotted_name || (parent->chip && !tap->chip) ||
			(parent->tapname && !tap->tapname) || !tap->expected_ids) {
		free(tap->dotted_name);
		free(tap->chip);
		free(tap->tapname);
		free(tap->expected_ids);
		free(tap);
		return ERROR_FAIL;
	}

	tap->parent = parent;
	tap->expected_ids[0] = expected_id;
	tap->expected_ids_cnt = 1;
	tap->ignore_version = ignore_version;
	tap->idcode = expected_id;
	tap->has_idcode = true;
	tap->enabled = true;
	tap->ir_length = parent->ir_length;
	tap->ir_capture_value = parent->ir_capture_value;
	tap->ir_capture_mask = parent->ir_capture_mask;
	vjtag_tap_init(tap);

	LOG_DEBUG("Created AJI virtual TAP %s on %s", tap->dotted_name, parent->dotted_name);
	return ERROR_OK;
}

__COMMAND_HANDLER(handle_aji_hardware)
{
	if (CMD_ARGC != 2)
		return ERROR_COMMAND_SYNTAX_ERROR;

	if (jtag_hardware_by_string(CMD_ARGV[0])) {
		command_print(CMD, "hardware '%s' already exists", CMD_ARGV[0]);
		return ERROR_COMMAND_ARGUMENT_INVALID;
	}

	struct jtag_hardware *hardware = calloc(1, sizeof(*hardware));
	if (!hardware)
		return ERROR_FAIL;
	hardware->name = strdup(CMD_ARGV[0]);
	hardware->address = strdup(CMD_ARGV[1]);
	if (!hardware->name || !hardware->address) {
		jtag_hardware_free(hardware);
		return ERROR_FAIL;
	}

	jtag_hardware_add(hardware);
	return ERROR_OK;
}
