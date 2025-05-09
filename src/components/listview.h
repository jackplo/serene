#pragma once

#include "../backend/app_searcher.h"
#include "../models/applicationlistmodel.h"
#include <giomm/liststore.h>
#include <gtkmm/listview.h>
#include <gtkmm/signallistitemfactory.h>
#include <gtkmm/singleselection.h>

class ListView : public Gtk::ListView {
public:
  ListView();
  virtual ~ListView();
  void update_model(const std::vector<Application> &applications);

private:
  void setup_factory();

  Glib::RefPtr<ApplicationListModel> m_model;
  Glib::RefPtr<Gtk::SingleSelection> m_selection;
  Glib::RefPtr<Gtk::SignalListItemFactory> m_factory;
};