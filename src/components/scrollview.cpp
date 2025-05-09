#include "scrollview.h"

ScrollView::ScrollView() {
  // Set up scrolling policies
  set_policy(Gtk::PolicyType::NEVER, Gtk::PolicyType::AUTOMATIC);

  // Configure the scrolled window
  set_vexpand(true); // Don't expand vertically by default
  set_hexpand(true); // Expand horizontally
  set_margin_start(0);
  set_margin_end(0);
  set_margin_top(6);
  set_margin_bottom(0);

  // Add CSS class
  add_css_class("spotlight-scroll");

  // Start hidden
  hide();
}

ScrollView::~ScrollView() {}

void ScrollView::set_child(Gtk::Widget &widget) {
  ScrolledWindow::set_child(widget);

  // Set maximum height to show about 5 items comfortably
  set_max_content_height(250);
}
