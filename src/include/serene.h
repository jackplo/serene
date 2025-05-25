#pragma once

#include "../lib/include/app_searcher.h"
#include "../lib/include/engine.h"
#include "../lib/include/file_searcher.h"
#include "customsearchentry.h"
#include "itemdetails.h"
#include "listview.h"
#include "scrollview.h"
#include <gtkmm/window.h>

class Spotlight final : public Gtk::Window {
public:
  Spotlight();
  ~Spotlight() override;

protected:
  void on_search_changed();
  void load_css();
  void show_results();
  void hide_results();
  void update_search_results(const Glib::ustring &query);
  void on_search_activate();
  void on_selection_changed(guint position, guint n_items);
  void setup_window();
  void setup_search();
  void setup_results_view();
  void setup_item_details();
  void setup_scroll_window();

private:
  CustomSearchEntry m_searchEntry;
  ListView m_listView;
  ScrollView m_scrollWindow;
  AppSearcher m_appSearcher;
  FileSearcher m_fileSearcher;
  ItemDetails m_itemDetails;
  Engine m_engine;
};
