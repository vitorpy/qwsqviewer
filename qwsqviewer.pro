TEMPLATE = subdirs
SUBDIRS = app commonnbis imgtools an2k mindtct

imgtools.depends = commonnbis
an2k.depends = commonnbis imgtools
mindtct.depends = commonnbis imgtools an2k
app.depends = commonnbis imgtools an2k mindtct
