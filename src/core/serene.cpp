#include "../include/serene.h"
#include <algorithm>
#include <fstream>
#include <gtkmm/application.h>
#include <gtkmm/box.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/separator.h>
#include <iostream>

namespace {
constexpr int WINDOW_WIDTH = 600;
constexpr int WINDOW_HEIGHT = 400;
constexpr int MARGIN = 12;
constexpr int SPACING = 6;
constexpr int MAX_RESULTS = 5;
constexpr int PANE_WIDTH = 300;
} // namespace

Spotlight::Spotlight() : Gtk::Window() {
  setup_window();
  setup_search();
  setup_results_view();
  load_css();
}

Spotlight::~Spotlight() = default;

void Spotlight::setup_window() {
  set_decorated(false);
  set_resizable(false);
  set_default_size(WINDOW_WIDTH, WINDOW_HEIGHT);

  auto main_box =
      Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, SPACING);
  main_box->set_margin(MARGIN);
  main_box->add_css_class("spotlight-box");

  set_child(*main_box);
  signal_show().connect([this]() { m_searchEntry.grab_focus(); });
}

void Spotlight::setup_search() {
  m_searchEntry.signal_changed().connect(
      sigc::mem_fun(*this, &Spotlight::on_search_changed));
  m_searchEntry.signal_activate().connect(
      sigc::mem_fun(*this, &Spotlight::on_search_activate));
}

void Spotlight::setup_results_view() {
  auto result_box =
      Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, SPACING);
  result_box->add_css_class("result-box");

  setup_scroll_window();
  setup_item_details();

  auto separator =
      Gtk::make_managed<Gtk::Separator>(Gtk::Orientation::VERTICAL);
  separator->set_margin_end(SPACING);

  result_box->append(m_scrollWindow);
  result_box->append(*separator);
  result_box->append(m_itemDetails);

  auto *main_box = dynamic_cast<Gtk::Box *>(get_child());
  if (main_box) {
    main_box->append(m_searchEntry);
    main_box->append(*result_box);
  }

  result_box->hide();
}

void Spotlight::setup_scroll_window() {
  m_scrollWindow.set_child(m_listView);
  m_scrollWindow.set_max_content_height(WINDOW_HEIGHT);
  m_scrollWindow.set_hexpand(true);
  m_scrollWindow.set_vexpand(true);
  m_scrollWindow.set_size_request(PANE_WIDTH, 0);

  m_listView.get_selection()->signal_selection_changed().connect(
      sigc::mem_fun(*this, &Spotlight::on_selection_changed));
}

void Spotlight::setup_item_details() {
  m_itemDetails.set_hexpand(true);
  m_itemDetails.set_vexpand(true);
  m_itemDetails.set_size_request(PANE_WIDTH, 0);
}

void Spotlight::on_search_changed() {
  const Glib::ustring query = m_searchEntry.get_text();
  if (query.empty()) {
    hide_results();
    return;
  }

  update_search_results(query);
}

void Spotlight::on_search_activate() {
  auto selected_pos = m_listView.get_selection()->get_selected();
  if (selected_pos == GTK_INVALID_LIST_POSITION) {
    return;
  }

  auto item = m_listView.get_selection()->get_selected_item();
  if (auto app_obj = std::dynamic_pointer_cast<ApplicationObject>(item)) {
    m_listView.launch_application(app_obj->app.exec);
  } else if (auto file_obj = std::dynamic_pointer_cast<FileObject>(item)) {
    m_listView.open_file(file_obj->result.path);
  }
}

void Spotlight::on_selection_changed([[maybe_unused]] guint position,
                                     [[maybe_unused]] guint n_items) {
  auto selected_pos = m_listView.get_selection()->get_selected();
  auto item = selected_pos != GTK_INVALID_LIST_POSITION
                  ? std::dynamic_pointer_cast<Glib::Object>(
                        m_listView.get_selection()->get_selected_item())
                  : nullptr;
  m_itemDetails.set_item(item);
}

void Spotlight::update_search_results(const Glib::ustring &query) {
  auto results = m_engine.search(query.raw(), 5);
  if (results.size() > MAX_RESULTS) {
    results.resize(MAX_RESULTS);
  }

  m_listView.update_model(results);

  if (results.empty()) {
    hide_results();
  } else {
    show_results();
  }
}

void Spotlight::show_results() {
  m_scrollWindow.get_parent()->show();
  m_scrollWindow.show();
  m_itemDetails.show();
  m_listView.get_selection()->set_selected(0);

  auto item = std::dynamic_pointer_cast<Glib::Object>(
      m_listView.get_selection()->get_selected_item());
  m_itemDetails.set_item(item);
}

void Spotlight::hide_results() {
  m_scrollWindow.hide();
  m_itemDetails.hide();
  m_scrollWindow.get_parent()->hide();
}

void Spotlight::load_css() {
  auto css_provider = Gtk::CssProvider::create();
  const std::vector<std::string> css_files = {"assets/style.css",
                                              "assets/customsearchentry.css",
                                              "assets/itemdetails.css"};

  std::string combined_css;
  bool all_files_loaded = true;

  for (const auto &file : css_files) {
    std::ifstream css_file(file);
    if (!css_file.is_open()) {
      std::cerr << "Failed to open CSS file: " << file << std::endl;
      all_files_loaded = false;
      break;
    }
    combined_css += std::string((std::istreambuf_iterator<char>(css_file)),
                                std::istreambuf_iterator<char>()) +
                    "\n";
  }

  if (all_files_loaded) {
    css_provider->load_from_data(combined_css);
    Gtk::StyleContext::add_provider_for_display(
        Gdk::Display::get_default(), css_provider,
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  }
}
