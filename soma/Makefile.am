ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libsomapanel.la

libsomapanel_la_CPPFLAGS = $(GTK3_CFLAGS) -I$(top_srcdir)
libsomapanel_la_LIBADD   = $(GTK3_LIBS)
libsomapanel_la_SOURCES = \
	soma-panel-css.h \
	soma-panel-ui.h \
	soma-panel.c

somaincludedir = $(includedir)/soma
somainclude_HEADERS = \
	soma-panel.h

soma-panel-ui.h: $(top_srcdir)/data/soma-panel.glade
	exo-csource --static --strip-comments --strip-content --name=soma_panel_ui $< > $@

soma-panel-css.h: $(top_srcdir)/data/soma-panel.css
	exo-csource --static --name=soma_panel_css $< > $@

BUILT_SOURCES = \
	soma-panel-css.h \
	soma-panel-ui.h

EXTRA_DIST = \
	$(top_srcdir)/data/soma-panel.css \
	$(top_srcdir)/data/soma-panel.glade
