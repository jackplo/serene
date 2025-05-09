#pragma once

#include "../backend/app_searcher.h"
#include "../models/applicationlistmodel.h"
#include <giomm/liststore.h>
#include <gtkmm/listview.h>
#include <gtkmm/signallistitemfactory.h>
#include <gtkmm/singleselection.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

class ListView : public Gtk::ListView {
public:
  ListView();
  virtual ~ListView();
  void update_model(const std::vector<Application> &applications);

private:
  void setup_factory();
  bool on_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);
  void on_click_pressed(int n_press, double x, double y);
  void on_row_activated(guint position);

  void launch_application(const std::string &exec_command);

  Glib::RefPtr<ApplicationListModel> m_model;
  Glib::RefPtr<Gtk::SingleSelection> m_selection;
  Glib::RefPtr<Gtk::SignalListItemFactory> m_factory;
};