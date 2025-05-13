#include "scrollview.h"

ScrollView::ScrollView() {
  set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);

  set_vexpand(true);
  set_hexpand(true);
  set_margin_start(0);
  set_margin_end(0);
  set_margin_bottom(0);

  add_css_class("spotlight-scroll");

  hide();
}

ScrollView::~ScrollView() {}

void ScrollView::set_child(Gtk::Widget &widget) {
  ScrolledWindow::set_child(widget);

  set_max_content_height(250);
}
