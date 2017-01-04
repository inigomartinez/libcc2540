TXT3 = \
	cc2540_open.txt \
	cc2540_close.txt \
	cc2540_read.txt \
	cc2540_write.txt \
	cc2540_get_timeout.txt \
	cc2540_set_timeout.txt \
	gap_cmd_dev_init.txt \
	gap_cmd.txt \
	gap_evt_dev_init_done.txt \
	print_hci_evt.txt \
	print_gap_evt_dev_init_done.txt \
	print_gap_evt_cmd_status.txt


EXTRA_DIST = asciidoc.conf $(TXT3)

MAN3 = $(TXT3:%.txt=%.3)

man3_MANS = $(MAN3)

HTML = $(TXT3:%.txt=%.html)

htmldoc: $(HTML)

.txt.html:
	asciidoc -d manpage -b xhtml11 -f asciidoc.conf -alibcc2540_version=@LIBCC2540_VERSION@ $<

.txt.3:
	a2x --doctype manpage --format manpage -alibcc2540_version=@LIBCC2540_VERSION@ $<

CLEANFILES = *.3 *.html
