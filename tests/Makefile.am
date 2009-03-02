testprogs=test-gstyleable

noinst_PROGRAMS=$(testprogs)

INCLUDES=-ggdb -Wall -O0 -I. -I$(top_srcdir)/gstyle -I$(top_builddir)/gstyle

AM_CFLAGS = ${GSTYLE_CFLAGS} -I$(top_srcdir)/gstyle

DEPS=$(top_srcdir)/gstyle/libgstyle.la

test_gstyleable_SOURCES=\
						test-gstyleable.c	\
						gstyleclass.c			\
						gstyleclass.h
test_gstyleable_DEPENDENCIES=${DEPS}
test_gstyleable_LDADD=${GSTYLE_LIBS} ${DEPS}
