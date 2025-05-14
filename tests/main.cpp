#include "test_window.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[]) {
  auto app = Gtk::Application::create("org.spotlight.search");
  return app->make_window_and_run<TestWindow>(argc, argv);
}