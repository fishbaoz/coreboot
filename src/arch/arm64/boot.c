/*
 * This file is part of the coreboot project.
 *
 * Copyright 2013 Google Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.
 */

#include <arch/cache.h>
#include <arch/lib_helpers.h>
#include <arch/secmon.h>
#include <arch/stages.h>
#include <arch/spintable.h>
#include <arch/transition.h>
#include <arm_tf.h>
#include <cbmem.h>
#include <console/console.h>
#include <program_loading.h>
#include <rules.h>
#include <string.h>

static void run_payload(struct prog *prog)
{
	void (*doit)(void *);
	void *arg;

	doit = prog_entry(prog);
	arg = prog_entry_arg(prog);
	u64 payload_spsr = SPSR_EXCEPTION_MASK | get_eret_el(EL2, SPSR_USE_L);

	if (IS_ENABLED(CONFIG_ARM64_USE_ARM_TRUSTED_FIRMWARE))
		arm_tf_run_bl31((u64)doit, (u64)arg, payload_spsr);
	else if (IS_ENABLED(CONFIG_ARM64_USE_SECURE_MONITOR))
		secmon_run(doit, arg);
	else {
		uint8_t current_el = get_current_el();

		/* Start the other CPUs spinning. */
		if (IS_ENABLED(CONFIG_ARM64_USE_SPINTABLE))
			spintable_start();

		cache_sync_instructions();

		printk(BIOS_SPEW, "entry    = %p\n", doit);

		/* If current EL is not EL3, jump to payload at same EL. */
		if (current_el != EL3)
			doit(arg);
		else {
			/* If current EL is EL3, we transition to payload in EL2. */
			struct exc_state exc_state;
			memset(&exc_state, 0, sizeof(exc_state));
			exc_state.elx.spsr = payload_spsr;

			transition_with_entry(doit, arg, &exc_state);
		}
	}
}

void arch_prog_run(struct prog *prog)
{
	void (*doit)(void *);
	void *arg;

	if (ENV_RAMSTAGE && prog_type(prog) == ASSET_PAYLOAD) {
		run_payload(prog);
		return;
	}

	doit = prog_entry(prog);
	arg = prog_entry_arg(prog);

	doit(prog_entry_arg(prog));
}
