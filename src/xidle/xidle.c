/*	$OpenBSD: xidle.c,v 1.10 2005/10/07 19:47:26 fgsch Exp $	*/
/*
 * Copyright (c) 2005 Federico G. Schwindt.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE OPENBSD PROJECT AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OPENBSD
 * PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <X11/Xlib.h>
#include <X11/extensions/scrnsaver.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <err.h>
#include <getopt.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef PATH_PROG
#define PATH_PROG	"/usr/X11R6/bin/xlock"
#endif


enum {
	north = 0x01,
	south = 0x02,
	east  = 0x04,
	west  = 0x08
};

struct xinfo {
	Display		*dpy;
	Window		 win;
	int		 coord_x;
	int		 coord_y;

	int		 saver_event;	/* Only if Xss ext is available */

	int		 saved_timeout;
	int		 saved_interval;
	int		 saved_pref_blank;
	int		 saved_allow_exp;
};

struct xinfo x;
int	position = north|west;

const struct option longopts[] = {
	{ "area",	required_argument,	NULL,		'a' },
	{ "delay",	required_argument,	NULL,		'D' },
	{ "display",	required_argument,	NULL,		'd' },
	{ "program",	required_argument,	NULL,		'p' },
	{ "timeout",	required_argument,	NULL,		't' },

	{ "ne",		no_argument,		&position,	north|east },
	{ "nw",		no_argument,		&position,	north|west },
	{ "se",		no_argument,		&position,	south|east },
	{ "sw",		no_argument,		&position,	south|west },

	{ NULL,		0,			NULL,		0 }
};

extern char *__progname;

void	action(struct xinfo *, char **);
void	close_x(struct xinfo *);
void	init_x(const char *, struct xinfo *, int, int);
void	handler(int);
void    force_action(int);
void	usage(void);


void
usage()
{
	fprintf(stderr, "Usage:\n%s %s\n", __progname,
	    "[-area pixels] [-delay secs] [-display host:dpy] "
	    "[-ne | -nw | -se | -sw]\n      [-program path] [-timeout secs]");
	exit(1);
}


void
init_x(const char *display, struct xinfo *xi, int area, int timeout)
{
	XSetWindowAttributes attr;
	Display *dpy;
	Window win;
	int error, event;
	int screen;

	dpy = XOpenDisplay(display);
	if (!dpy) {
		errx(1, "Unable to open display %s", XDisplayName(display));
		/* NOTREACHED */
	}

	screen = DefaultScreen(dpy);

	if (position & south)
		xi->coord_y = DisplayHeight(dpy, screen) - area;
	if (position & east)
		xi->coord_x = DisplayWidth(dpy, screen) - area;

	attr.override_redirect = True;
	win = XCreateWindow(dpy, DefaultRootWindow(dpy),
	    xi->coord_x, xi->coord_y, area, area, 0, 0, InputOnly,
	    CopyFromParent, CWOverrideRedirect, &attr);

	XMapWindow(dpy, win);
	XSelectInput(dpy, win, EnterWindowMask|StructureNotifyMask
#if 0
			       |VisibilityChangeMask
#endif
	);

	/*
	 * AFAICT, we need the event number for ScreenSaverNotify
	 * _always_ since it's the only way to distinguish whether
	 * we've been obscured by an external locking program or
	 * by another window and react according.
	 */
	if (XScreenSaverQueryExtension(dpy, &event, &error) == True) {
		xi->saver_event = event;

		XScreenSaverSelectInput(dpy, DefaultRootWindow(dpy),
		    ScreenSaverNotifyMask);
	} else
		warnx("XScreenSaver extension not available.%s",
		    timeout > 0 ? " Timeout disabled." : "");

	if (timeout > 0 && xi->saver_event) {
		XGetScreenSaver(dpy, &xi->saved_timeout, &xi->saved_interval,
		    &xi->saved_pref_blank, &xi->saved_allow_exp);

		XSetScreenSaver(dpy, timeout, 0, DontPreferBlanking,
		    DontAllowExposures);
	}

	xi->dpy = dpy;
	xi->win = win;
}


void
close_x(struct xinfo *xi)
{
	XSetScreenSaver(xi->dpy, xi->saved_timeout, xi->saved_interval,
	    xi->saved_pref_blank, xi->saved_allow_exp);
	XDestroyWindow(xi->dpy, xi->win);
	XCloseDisplay(xi->dpy);
}


void
action(struct xinfo *xi, char **args)
{
	int dumb;

	switch (fork()) {
	case -1:
		err(1, "fork");
		/* NOTREACHED */

	case 0:
		execv(*args, args);
		exit(1);
		/* NOTREACHED */

	default:
		wait(&dumb);
		XSync(xi->dpy, True);
		break;
	}
}


void
handler(int sig)
{
	XClientMessageEvent ev;

	ev.type = ClientMessage;
	ev.display = x.dpy;
	ev.window = x.win;
	ev.message_type = 0xdead;
	ev.format = 8;
	XSendEvent(x.dpy, x.win, False, 0L, (XEvent *)&ev);
	XFlush(x.dpy);
}

void
force_action(int sig)
{
	XClientMessageEvent ev;

	ev.type = ClientMessage;
	ev.display = x.dpy;
	ev.window = x.win;
	ev.message_type = 0x2341;
	ev.format = 8;
	XSendEvent(x.dpy, x.win, False, 0L, (XEvent *)&ev);
	XFlush(x.dpy);
}
	



int
main(int argc, char **argv)
{
	char *program = PATH_PROG;
	char *display = NULL, *p;
	char **ap, *args[10];
	int area = 2, delay = 2;
	u_long last_serial = 0;
	int timeout = 0;
	int pflag;
	int c;

	pflag = 0;
	while ((c = getopt_long_only(argc, argv, "", longopts, NULL)) != -1) {
		switch (c) {
		case 'D':
			delay = strtol(optarg, &p, 10);
			if (*p || delay < 0) {
				errx(1, "illegal value -- %s", optarg);
				/* NOTREACHED */
			}
			break;

		case 'a':
			area = strtol(optarg, &p, 10);
			if (*p || area < 1) {
				errx(1, "illegal value -- %s", optarg);
				/* NOTREACHED */
			}
			break;

		case 'd':
			display = optarg;
			break;

		case 'p':
			program = optarg;
			break;

		case 't':
			timeout = strtol(optarg, &p, 10);
			if (*p || timeout < 0) {
				errx(1, "illegal value -- %s", optarg);
				/* NOTREACHED */
			}
			break;

		case 0:
			if (pflag) {
				errx(1, "Cannot specify multiple positions");
				/* NOTREACHED */
			}
			pflag++;
			break;

		default:
			usage();
			/* NOTREACHED */
		}
	}

	if ((argc - optind) != 0) {
		usage();
		/* NOTREACHED */
	}

	for (ap = args; ap < &args[9] &&
	    (*ap = strsep(&program, " ")) != NULL;) {
		if (**ap != '\0')
			ap++;
	}
	*ap = NULL;

	bzero(&x, sizeof(struct xinfo));

	init_x(display, &x, area, timeout);

	signal(SIGINT, handler);
	signal(SIGTERM, handler);
	signal(SIGHUP, force_action);

	for (;;) {
		XEvent ev;
		u_long mask;

		XNextEvent(x.dpy, &ev);

#ifdef DEBUG
		printf("got event %d\n", ev.type);
#endif

		switch (ev.type) {
		case VisibilityNotify:
			/*
			 * If we got here and the current serial matches
			 * the one from saver_event, we're being obscured
			 * by a locking program. Disable further
			 * screen saver events and raises.
			 */
			if (ev.xvisibility.serial == last_serial)
				mask = 0;
			else
				mask = ScreenSaverNotifyMask;

			XScreenSaverSelectInput(x.dpy,
			    DefaultRootWindow(x.dpy), mask);

			if (mask)
				XMapRaised(x.dpy, x.win);

			XSync(x.dpy, True);
			break;

		case MapNotify:
			XMapRaised(x.dpy, x.win);
			break;

		case ClientMessage:
			if(ev.xclient.message_type == 0x2341) {
				action(&x, args);
				break;
			} else
			if (ev.xclient.message_type != 0xdead)
				break;

			close_x(&x);
			exit(0);
			/* NOTREACHED */

		case EnterNotify:
			sleep(delay);

			XQueryPointer(x.dpy, x.win, &ev.xcrossing.root,
			    &ev.xcrossing.window,
			    &ev.xcrossing.x_root, &ev.xcrossing.y_root,
			    &ev.xcrossing.x, &ev.xcrossing.y,
			    &ev.xcrossing.state);

			/* Check it was for real. */
			if (ev.xcrossing.x_root < x.coord_x ||
			    ev.xcrossing.y_root < x.coord_y ||
			    ev.xcrossing.x_root > x.coord_x + area ||
			    ev.xcrossing.y_root > x.coord_y + area)
				break;
			/* FALLTHROUGH */

		default:
			if (ev.type != EnterNotify &&
			    ev.type != x.saver_event)
				break;

			if (ev.type == x.saver_event) {
				XScreenSaverNotifyEvent *se =
				    (XScreenSaverNotifyEvent *)&ev;

				/* Take note of the serial for this event. */
				last_serial = se->serial;

				/*
				 * Was for real or due to terminal
				 * switching or a locking program?
				 */
				if (se->forced != False)
					break;
			}
			action(&x, args);
			break;
		}
	}

	/* NOTREACHED */
}
