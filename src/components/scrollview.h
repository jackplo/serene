#pragma once
#include <gtkmm/listview.h>
#include <gtkmm/scrolledwindow.h>

class ScrollView : public Gtk::ScrolledWindow {
public:
  ScrollView();
  virtual ~ScrollView();
  void set_child(Gtk::Widget &widget);
};
