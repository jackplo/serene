#pragma once

#include "customsearchentry.h"
#include "itemdetails.h"
#include "lib/include/app_searcher.h"
#include "lib/include/file_searcher.h"
#include "listview.h"
#include "scrollview.h"
#include <gtkmm/window.h>

class Spotlight : public Gtk::Window {
public:
  Spotlight();
  virtual ~Spotlight();

protected:
  void on_search_changed();
  void load_css();

private:
  CustomSearchEntry m_searchEntry;
  ListView m_listView;
  ScrollView m_scrollWindow;
  AppSearcher m_appSearcher;
  FileSearcher m_fileSearcher;
  ItemDetails m_itemDetails;
};
