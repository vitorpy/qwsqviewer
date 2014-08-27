#ifndef CANVAS_H
#define CANVAS_H

#include <gtkmm/drawingarea.h>
#include <gtkmm/pixbuf.h>

class Canvas : public Gtk::DrawingArea
{
public:
	Canvas();
	virtual ~Canvas();

protected:
	virtual bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr);

private:
	Glib::RefPtr<Gdk::PixBuf> m_image;
};

#endif /*CANVAS_H*/

