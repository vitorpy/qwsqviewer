TEMPLATE = subdirs
SUBDIRS = app commonnbis imgtools an2k

imgtools.depends = commonnbis
an2k.depends = commonnbis imgtools
app.depends = commonnbis imgtools an2k
