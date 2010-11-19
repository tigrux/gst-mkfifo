AM_CPPFLAGS = $(PROGRAM_CFLAGS)

BUILT_SOURCES = vala.stamp

bin_PROGRAMS = gst-mkfifo

gst_mkfifo_VALASOURCES = \
    callbacks.vala \
    commands.vala \
    gst-mkfifo.vala

gst_mkfifo_SOURCES = \
	$(gst_mkfifo_VALASOURCES:.vala=.c)

gst_mkfifo_LDADD = \
	$(PROGRAM_LIBS)

vala.stamp: $(gst_mkfifo_VALASOURCES)
	$(VALAC) --thread -C $(VALAFLAGS) $(gst_mkfifo_VALASOURCES)
	touch $@

EXTRA_DIST = \
	$(gst_mkfifo_VALASOURCES) \
	test.sh
