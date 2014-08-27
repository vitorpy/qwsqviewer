#include "canvas.h"
#include <cairomm/context.h>
#include <gdkmm/general.h> // set_source_pixbuf()
#include <glibmm/fileutils.h>
#include <iostream>

Canvas::Canvas()
{
  try
  {
    //m_image = Gdk::Pixbuf::create_from_file("fractal_image.png");
  }
  catch(const Glib::FileError& ex)
  {
    std::cerr << "FileError: " << ex.what() << std::endl;
  }
  catch(const Gdk::PixbufError& ex)
  {
    std::cerr << "PixbufError: " << ex.what() << std::endl;
  }

  // Show at least a quarter of the image.
  if (m_image)
    set_size_request(m_image->get_width()/2, m_image->get_height()/2);
}

Canvas::~Canvas()
{
}

bool Canvas::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  if (!m_image)
    return false;

  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();

  // Draw the image in the middle of the drawing area, or (if the image is
  // larger than the drawing area) draw the middle part of the image.
  Gdk::Cairo::set_source_pixbuf(cr, m_image,
    (width - m_image->get_width())/2, (height - m_image->get_height())/2);
  cr->paint();

  return true;
}

