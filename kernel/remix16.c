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

#include <stdint.h>
#include <avr/io.h>
#include "kernel.h"
#include "tasks.h"

uint16_t adcval[ADCCHANNELS];

struct adcarg adcarg = { adcval };
struct lcdarg lcdarg = { 0, 0, adcval };
struct ppmarg ppmarg = { adcval };

int
main()
{
	init(STACK);

	init_uart();

	semaphore(0, 1);

	exec(heartbeat, STACK, MSEC(0), MSEC(750), 0);
	exec(adc, STACK, MSEC(1), MSEC(60), &adcarg);
	exec(ppm, STACK, MSEC(3), MSEC(20), &ppmarg);
	exec(lcd, STACK, MSEC(7), MSEC(100), &lcdarg);

	for (;;);

	return 0;
}
