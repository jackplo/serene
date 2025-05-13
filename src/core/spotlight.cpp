#include "spotlight.h"
#include <algorithm>
#include <fstream>
#include <gtkmm/application.h>
#include <gtkmm/box.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/separator.h>
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
      auto item = m_listView.get_selection()->get_selected_item();
      if (auto app_obj = std::dynamic_pointer_cast<ApplicationObject>(item)) {
        m_listView.launch_application(app_obj->app.exec);
      } else if (auto file_obj = std::dynamic_pointer_cast<FileObject>(item)) {
        m_listView.open_file(file_obj->result.path);
      }
    }
  });

  m_scrollWindow.set_child(m_listView);
  m_scrollWindow.set_max_content_height(400);

  auto result_box = Gtk::Box(Gtk::Orientation::HORIZONTAL, 6);
  result_box.add_css_class("result-box");

  // Set both components to expand and fill with equal width
  m_scrollWindow.set_hexpand(true);
  m_scrollWindow.set_vexpand(true);
  m_itemDetails.set_hexpand(true);
  m_itemDetails.set_vexpand(true);

  // Set both components to take up equal width
  m_scrollWindow.set_size_request(300, 0);
  m_itemDetails.set_size_request(300, 0);

  // Add a vertical separator between the components
  auto separator =
      Gtk::make_managed<Gtk::Separator>(Gtk::Orientation::VERTICAL);
  separator->set_margin_start(6);
  separator->set_margin_end(6);

  // Connect ListView selection to update ItemDetails
  m_listView.get_selection()->signal_selection_changed().connect(
      [this](guint position, guint n_items) {
        auto selected_pos = m_listView.get_selection()->get_selected();
        if (selected_pos != GTK_INVALID_LIST_POSITION) {
          auto item = std::dynamic_pointer_cast<Glib::Object>(
              m_listView.get_selection()->get_selected_item());
          m_itemDetails.set_item(item);
        } else {
          m_itemDetails.set_item(nullptr);
        }
      });

  result_box.append(m_scrollWindow);
  result_box.append(*separator);
  result_box.append(m_itemDetails);
  m_scrollWindow.set_max_content_height(400);

  main_box.append(m_searchEntry);
  main_box.append(result_box);

  // Hide result box initially
  result_box.hide();

  set_child(main_box);

  load_css();

  signal_show().connect([this]() { m_searchEntry.grab_focus(); });
}

Spotlight::~Spotlight() {}

void Spotlight::on_search_changed() {
  Glib::ustring query = m_searchEntry.get_text();

  if (query.empty()) {
    m_scrollWindow.hide();
    m_itemDetails.hide();
    m_scrollWindow.get_parent()->hide(); // Hide the result_box
    return;
  }

  auto app_results = m_appSearcher.search(query.raw());
  if (app_results.size() > 5) {
    app_results.resize(5);
  }

  std::vector<FileResult> file_results;
  if (app_results.size() < 5) {
    file_results = m_fileSearcher.search(query.raw());
    if (file_results.size() > (5 - app_results.size())) {
      file_results.resize(5 - app_results.size());
    }
  }

  m_listView.update_model(app_results, file_results);

  if (app_results.empty() && file_results.empty()) {
    m_scrollWindow.hide();
    m_itemDetails.hide();
    m_scrollWindow.get_parent()->hide(); // Hide the result_box
  } else {
    m_scrollWindow.get_parent()->show(); // Show the result_box
    m_scrollWindow.show();
    m_itemDetails.show();
    m_listView.get_selection()->set_selected(0);

    // Update ItemDetails with the first item
    auto item = std::dynamic_pointer_cast<Glib::Object>(
        m_listView.get_selection()->get_selected_item());
    m_itemDetails.set_item(item);
  }
}

void Spotlight::load_css() {
  auto css_provider = Gtk::CssProvider::create();

  std::ifstream main_css_file("assets/style.css");
  std::ifstream customsearch_css_file("assets/customsearchentry.css");
  std::ifstream itemdetails_css_file("assets/itemdetails.css");
  if (main_css_file.is_open() && customsearch_css_file.is_open() &&
      itemdetails_css_file.is_open()) {
    std::string main_css_content(
        (std::istreambuf_iterator<char>(main_css_file)),
        std::istreambuf_iterator<char>());
    std::string custom_css_content(
        (std::istreambuf_iterator<char>(customsearch_css_file)),
        std::istreambuf_iterator<char>());
    std::string itemdetails_css_content(
        (std::istreambuf_iterator<char>(itemdetails_css_file)),
        std::istreambuf_iterator<char>());
    std::string combined_css = main_css_content + "\n" + custom_css_content +
                               "\n" + itemdetails_css_content;
    css_provider->load_from_data(combined_css);
  } else {
    std::cerr << "Error loading CSS files" << std::endl;
  }

  Gtk::StyleContext::add_provider_for_display(
      Gdk::Display::get_default(), css_provider,
      GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}
