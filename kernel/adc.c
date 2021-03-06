/* $Id$ */
/*
 * Copyright (c) 2011 Dimitri Sokolyuk <demon@dim13.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <avr/io.h>

#include <stdint.h>
#include <stdio.h>

#include "kernel.h"
#include "tasks.h"

#define MUXMASK	0x07

void
adc(void *arg)
{
	struct adcarg *a = (struct adcarg *)arg;
	uint8_t i = 0;

	ADCSRA |= (_BV(ADEN) | ADC_FLAGS);

	for (;;) {
		for (i = 0; i < ADCCHANNELS; i++) {
			ADMUX = (i & MUXMASK) | _BV(ADLAR);
			ADCSRA |= _BV(ADSC);
			loop_until_bit_is_clear(ADCSRA, ADSC);

			lock(Adc);
			a->value[i] = ADCH;	/* ADLAR: 8-bit */
			unlock(Adc);
		}

		lock(Display);
		fprintf(stderr, "\n%8lx%8x", now(), a->value[0]);
		unlock(Display);

		sleep(0, 100000);
	}
}
