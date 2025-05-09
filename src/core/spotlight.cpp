#include "spotlight.h"
#include <algorithm>
#include <fstream>
#include <gtkmm/application.h>
#include <gtkmm/box.h>
#include <gtkmm/cssprovider.h>
#include <iostream>

Spotlight::Spotlight() {
  set_decorated(false);
  set_resizable(false);
  set_default_size(600, 400);

  // Create main vertical box
  auto main_box = Gtk::Box(Gtk::Orientation::VERTICAL, 6);
  main_box.set_margin_start(12);
  main_box.set_margin_end(12);
  main_box.set_margin_top(12);
  main_box.set_margin_bottom(12);
  main_box.add_css_class("spotlight-box");

  // Set up SearchEntry and ListView components
  m_searchEntry.signal_search_changed().connect(
      sigc::mem_fun(*this, &Spotlight::on_search_changed));

  m_scrollWindow.set_child(m_listView);
  m_scrollWindow.set_max_content_height(400);

  // Add components to the box
  main_box.append(m_searchEntry);
  main_box.append(m_scrollWindow);

  // Set the box as the window's child
  set_child(main_box);

  // Load CSS
  load_css();
}

Spotlight::~Spotlight() {}

void Spotlight::on_search_changed() {
  Glib::ustring query = m_searchEntry.get_text();

  // Search for applications using AppSearcher
  auto results = m_appSearcher.search(query.raw());

  // Update the list view with filtered items
  m_listView.update_model(results);

  // Show/hide scroll window based on whether we have results
  if (results.empty()) {
    m_scrollWindow.hide();
  } else {
    m_scrollWindow.show();
  }
}

void Spotlight::load_css() {
  auto css_provider = Gtk::CssProvider::create();

  std::ifstream css_file("assets/style.css");
  if (css_file.is_open()) {
    std::string css_content((std::istreambuf_iterator<char>(css_file)),
                            std::istreambuf_iterator<char>());
    css_provider->load_from_data(css_content);
  } else {
    std::cerr << "Error loading style.css" << std::endl;
  }

  Gtk::StyleContext::add_provider_for_display(
      Gdk::Display::get_default(), css_provider,
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}
