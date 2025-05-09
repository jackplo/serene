#pragma once

#include "../backend/app_searcher.h"
#include "../components/listview.h"
#include "../components/scrollview.h"
#include "../components/searchentry.h"
#include <gtkmm/window.h>

class Spotlight : public Gtk::Window {
public:
  Spotlight();
  virtual ~Spotlight();

protected:
  void on_search_changed();
  void load_css();

private:
  SearchEntry m_searchEntry;
  ListView m_listView;
  ScrollView m_scrollWindow;
  AppSearcher m_appSearcher;
};
