/* piblank - Raspberry Pi Screen Power Management

   Copyright (c) 2019 by Stefan Vorkoetter

   This file is part of piblank.

   piblank is free software: you can redistribute it and/or modify it under the
   terms of the GNU General Public License as published by the Free Software
   Foundation, either version 3 of the License, or (at your option) any later
   version.

   piblank is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
   details.

   You should have received a copy of the GNU General Public License along with
   piblank. If not, see <http://www.gnu.org/licenses/>. */

#define _BSD_SOURCE	// To enable daemon and usleep functions.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/extensions/scrnsaver.h>
#include <X11/extensions/XTest.h>

/* Command line options (in the form expected by getopt). */
#define OPTIONS "n"

static void usage( void )
{
    /* Print usage information and exit. */
    fprintf(stderr,"Usage: piblank [%s] timeout\n",OPTIONS);
    fprintf(stderr,"-n\t- do not become a daemon, remain in the foreground\n");
    fprintf(stderr,"timeout\t- time in whole minutes (1 to 10000) until display power off (default 30)\n");
    exit(1);
}

int main( int argc, char **argv )
{
    /* Process command line options. */
    bool optDaemonize = true;
    int c;
    while( (c = getopt(argc,argv,OPTIONS)) != -1 ) {
	switch( c ) {
	case 'n':
	    optDaemonize = false;
	    break;
	default:
	    usage();
	}
    }

    /* Get idle timeout value and convert to milliseconds. */
    unsigned long timeout = 30;
    if( optind < argc ) {
        if( optind != argc - 1 )
	    usage();
	timeout = atoi(argv[optind]);
	if( timeout < 1 || timeout > 10000 )
	    usage();
    }
    timeout *= 60000;

    /* Do what it takes to become a daemon. */
    if( optDaemonize && daemon(0,0) != 0 ) {
	fprintf(stderr,"piblank: failed to become a daemon: %s\n",strerror(errno));
	return( 1 );
    }

    /* Check for idle timeout every 500ms. */
    bool screenOn = true;
    Display *display = NULL;
    for( ;; ) {
	usleep(500000);

	unsigned long idle = ULONG_MAX;

	/* Look up user idle time in milliseconds. Based on code found at
	   https://superuser.com/questions/638357 */
	if( (display != NULL || (display = XOpenDisplay(NULL)) != NULL)
	 && XScreenSaverQueryExtension(display,&event_base,&error_base) )
	{
	    XScreenSaverInfo info;
	    XScreenSaverQueryInfo(display,DefaultRootWindow(display),&info);
	    idle = info.idle;
	}

	/* Check console idle time. */
	time_t now = time(NULL);
	for( int i = 1; i <= 6 && idle > 0; ++i ) {
	    char tty[10];
	    sprintf(tty,"/dev/tty%1.1d",i);
	    struct stat st;
	    if( stat(tty,&st) == 0 ) {
		unsigned long ms = (now - st.st_mtime) * 1000UL;
		if( ms < idle )
		    idle = ms;
	    }
	}

	/* Turn screen on or off appropriately. */
	if( screenOn && idle >= timeout ) {
	    system("vcgencmd display_power 0");
	    screenOn = false;
	}
	else if( !screenOn && idle < timeout ) {
	    system("vcgencmd display_power 1");
	    screenOn = true;
	}
    }

    return( 0 );
}
