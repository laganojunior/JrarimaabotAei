Jrarimaabot
==============================

This is the AEI version of my arimaa (http://arimaa.com) bot. It doesn't
do very much now, but I'm laying the framework for later.

Requirements
--------------------------

To make the bot
~~~~~~~~~~~~~~~
gcc
Python 2

To use the GUI
~~~~~~~~~~~~~~~
WxWidgets for Python
bzr (bazaar)

Building
--------------------------
Run::
    ./genmake
    make

Running bot tournaments
~~~~~~~~~~~~~~~~~~~~~~~
Run ::
    make roundrobin

See cfg/roundrobin.cfg for configuration

Playing against the bot
~~~~~~~~~~~~~~~~~~~~~~~
To play gold ::
    make play_g

To play silver ::
    make play_s
