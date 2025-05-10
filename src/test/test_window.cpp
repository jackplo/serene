#include "test_window.h"
#include <fstream>
#include <gtkmm/cssprovider.h>
#include <iostream>

TestWindow::TestWindow() {
  set_title("Custom Search Entry Test");
  set_default_size(400, 200);
  set_decorated(false);

  // Set up the main box
  m_main_box.set_orientation(Gtk::Orientation::VERTICAL);
  m_main_box.set_margin_start(12);
  m_main_box.set_margin_end(12);
  m_main_box.set_margin_top(12);
  m_main_box.set_margin_bottom(12);

  // Add the search entry to the box
  m_main_box.append(m_search_entry);

  // Set the main box as the window's child
  set_child(m_main_box);

  // Load CSS

  try {
    auto css_provider = Gtk::CssProvider::create();

    std::ifstream css_file("assets/customsearchentry.css");
    if (css_file.is_open()) {
      std::string css_content((std::istreambuf_iterator<char>(css_file)),
                              std::istreambuf_iterator<char>());
      css_provider->load_from_data(css_content);
    } else {
      std::cerr << "Error loading customsearchentry.css" << std::endl;
    }

    Gtk::StyleContext::add_provider_for_display(
        Gdk::Display::get_default(), css_provider,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  } catch (const Glib::Error &e) {
    std::cerr << "Error loading CSS: " << e.what() << std::endl;
  }
}

TestWindow::~TestWindow() {}