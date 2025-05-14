#pragma once
#include "../src/include/customsearchentry.h"
#include <gtkmm/box.h>
#include <gtkmm/window.h>

class TestWindow : public Gtk::Window {
public:
  TestWindow();
  virtual ~TestWindow();

private:
  Gtk::Box m_main_box;
  CustomSearchEntry m_search_entry;
};