#!/bin/sh

autoheader \
	&& aclocal -I m4 \
	&& autoconf \
	&& automake --add-missing --foreign --copy \
	&& ./configure $@
