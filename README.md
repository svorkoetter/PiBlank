# piblank - Raspberry Pi Display Power Management

The `piblank` utility is a screen saver daemon for use with Raspbian, intended
to be launched from your X11 session manager's auto-start file. It monitors X11
user idle time (no input from the keyboard or mouse) and turns off the display
when idle, turning it back on when the user interacts with the system.

## System Requirements

The following libraries are used by `piblank` (some of these might already be
installed by default):

* libx11-6
* libxss1
* libraspberrypi-bin for the `vcgencmd` utility

If you want to rebuild `piblank` from source, you will also need the following:

* libx11-dev
* libxss-dev

## Usage

The `piblank` command should be invoked from
`~/.config/lxsession/LXDE-pi/autostart` or its equivalent if you're using a
different window manager than LXDE. In any case, it shouldn't be launched until
after your X11 session is up and running.

The command line syntax for `piblank` is as follows. All arguments are
optional.

    piblank [-n] [timeout]

By default, `piblank` runs as a daemon. The `piblank` command will return
immediately, but the daemon will continue to run in the background. If the `-n`
option is given, then `piblank` will remain in the foreground until you
terminate it by pressing `Ctrl+C`.

The _timeout_ argument specifies the time, in minutes, that the system must be
idle before the display is turned off. If not specified, the default is 30
minutes.

## Internals

Screen blanking and unblanking is done using the `vcgencmd` utility, invoked as
`vcgencmd display_power x`, where _x_ is 0 or 1 respectively.

Idle time determination is done using the `XScreenSaverQueryInfo` function of
the `libxss` library.
