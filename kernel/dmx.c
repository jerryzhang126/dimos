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

#include <inttypes.h>
#include <avr/io.h>
#include "kernel.h"
#include "tasks.h"

uint8_t	red, green, blue;

struct rgbarg rgbargs = { &red, &green, &blue };
struct pwmarg pwmargs[] = {
	{ &red, PB2 },
	{ &green, PB3 },
	{ &blue, PB4 }
};

int
main()
{
	init(STACK);

	init_uart();

	semaphore(0, 1);

	task(heartbeat, STACK, USEC(0), MSEC(750), 0);
	task(rgb, STACK, USEC(10), MSEC(10), &rgbargs);
	task(pwm, STACK, USEC(20), USEC(2500), &pwmargs[0]);
	task(pwm, STACK, USEC(30), USEC(2500), &pwmargs[1]);
	task(pwm, STACK, USEC(50), USEC(2500), &pwmargs[2]);
	task(cmd, STACK, USEC(70), MSEC(10), &rgbargs);

	for (;;);

	return 0;
}
