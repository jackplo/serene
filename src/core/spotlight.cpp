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

  auto main_box = Gtk::Box(Gtk::Orientation::VERTICAL, 6);
  main_box.set_margin_start(12);
  main_box.set_margin_end(12);
  main_box.set_margin_top(12);
  main_box.set_margin_bottom(12);
  main_box.add_css_class("spotlight-box");

  m_searchEntry.signal_changed().connect(
      sigc::mem_fun(*this, &Spotlight::on_search_changed));
  m_searchEntry.signal_activate().connect([this]() {
    auto selected_pos = m_listView.get_selection()->get_selected();
    if (selected_pos != GTK_INVALID_LIST_POSITION) {
      auto app_obj = std::dynamic_pointer_cast<ApplicationObject>(
          m_listView.get_selection()->get_selected_item());
      if (app_obj) {
        m_listView.launch_application(app_obj->app.exec);
      }
    }
  });

  m_scrollWindow.set_child(m_listView);
  m_scrollWindow.set_max_content_height(400);

  main_box.append(m_searchEntry);
  main_box.append(m_scrollWindow);

  set_child(main_box);

  load_css();

  signal_show().connect([this]() { m_searchEntry.grab_focus(); });
}

Spotlight::~Spotlight() {}

void Spotlight::on_search_changed() {
  Glib::ustring query = m_searchEntry.get_text();

  if (query.empty()) {
    m_scrollWindow.hide();
    return;
  }

  auto results = m_appSearcher.search(query.raw());

  m_listView.update_model(results);

  if (results.empty()) {
    m_scrollWindow.hide();
  } else {
    m_scrollWindow.show();
    // Select the first item in the list
    m_listView.get_selection()->set_selected(0);
  }
}

void Spotlight::load_css() {
  auto css_provider = Gtk::CssProvider::create();

  std::ifstream main_css_file("assets/style.css");
  std::ifstream custom_css_file("assets/customsearchentry.css");

  if (main_css_file.is_open() && custom_css_file.is_open()) {
    std::string main_css_content(
        (std::istreambuf_iterator<char>(main_css_file)),
        std::istreambuf_iterator<char>());
    std::string custom_css_content(
        (std::istreambuf_iterator<char>(custom_css_file)),
        std::istreambuf_iterator<char>());

    // Combine both CSS contents
    std::string combined_css = main_css_content + "\n" + custom_css_content;
    css_provider->load_from_data(combined_css);
  } else {
    std::cerr << "Error loading CSS files" << std::endl;
  }

  Gtk::StyleContext::add_provider_for_display(
      Gdk::Display::get_default(), css_provider,
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}
