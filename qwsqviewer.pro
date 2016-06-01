TEMPLATE = subdirs
SUBDIRS = app commonnbis imgtools an2k

app.depends = commonnbis imgtools an2k
