html-local:
	@DOXYGEN@ Doxyfile

install-html-local: html-local
install-html-local:
	mkdir -p $(datarootdir)/extant
	cp -R $(top_builddir)/html $(datarootdir)/extant
