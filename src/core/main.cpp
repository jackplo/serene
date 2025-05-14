#include "../include/serene.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[]) {
  auto app = Gtk::Application::create("org.spotlight.search");
  return app->make_window_and_run<Spotlight>(argc, argv);
}
