#include "searchentry.h"
#include <gtkmm/searchentry.h>

SearchEntry::SearchEntry() {
  set_placeholder_text("Search...");
  add_css_class("spotlight-entry");
  signal_search_changed().connect(
      sigc::mem_fun(*this, &SearchEntry::on_search_changed));
}

SearchEntry::~SearchEntry() {}

void SearchEntry::on_search_changed() {
  // Handle search changes here
}
