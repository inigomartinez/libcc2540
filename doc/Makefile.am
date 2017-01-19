TXT3 = \
	cc2540_open.txt \
	cc2540_close.txt \
	cc2540_read.txt \
	cc2540_write.txt \
	cc2540_get_timeout.txt \
	cc2540_set_timeout.txt \
	gap_cmd_dev_init.txt \
	gap_cmd_dev_addr_set.txt \
	gap_cmd_dev_disc.txt \
	gap_cmd_dev_disc_end.txt \
	gap_cmd_disc_set.txt \
	gap_cmd_adv_set.txt \
	gap_cmd_disc_end.txt \
	gap_cmd_link_set.txt \
	gap_cmd_link_end.txt \
	gap_cmd_auth.txt \
	gap_cmd_pass_update.txt \
	gap_cmd_slave_sec.txt \
	gap_cmd_link_update.txt \
	gap_cmd_param_set.txt \
	gap_cmd_param_get.txt \
	hci_cmd.txt \
	hci_evt.txt \
	hci_strerror.txt \
	print_hci_evt_info.txt \
	print_gap_evt_dev_init_done.txt \
	print_gap_evt_adv_set_done.txt \
	print_gap_evt_disc_set_done.txt \
	print_gap_evt_dev_disc.txt \
	print_gap_evt_link_set.txt \
	print_gap_evt_link_end.txt \
	print_gap_evt_link_update.txt \
	print_gap_evt_auth.txt \
	print_gap_evt_dev_info.txt \
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
