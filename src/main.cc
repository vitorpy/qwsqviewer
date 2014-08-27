#include "canvas.h"
#include <gtkmm/application.h>
#include <gtkmm/window.h>

int main(int argc, char** argv)
{
  Glib::RefPtr<Gtk::Application> app =
	 Gtk::Application::create(argc, argv, "vbraga.gwsqviewer");

  Gtk::Window win;
  win.set_title("gwsqviewer");
  win.set_default_size(300, 200);

  Canvas canvas;
  win.add(canvas);
  canvas.show();

  return app->run(win);
}
