#pragma once
#include <gtkmm/searchentry.h>

class SearchEntry : public Gtk::SearchEntry {
public:
  SearchEntry();
  virtual ~SearchEntry();

protected:
  void on_search_changed();
};
