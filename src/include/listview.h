#pragma once

#include "../lib/include/app_searcher.h"
#include "../lib/include/file_searcher.h"
#include "../lib/include/result.h"
#include "combinedlistmodel.h"
#include <giomm/liststore.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
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
  void update_model(const std::vector<Result> &result);
  void launch_application(const std::string &exec_command);
  void open_file(const std::string &path);
  Glib::RefPtr<Gtk::SingleSelection> get_selection() { return m_selection; }

private:
  void setup_factory();
  bool on_key_pressed(guint keyval, guint keycode, Gdk::ModifierType state);
  void on_click_pressed(int n_press, double x, double y);
  void on_row_activated(guint position);

  void set_mime_type_icon(Gtk::Image *icon, const std::string &mime_type);
  void
  handle_application_item(const std::shared_ptr<ApplicationObject> &app_obj,
                          Gtk::Image *icon, Gtk::Label *label);

  void handle_file_item(const std::shared_ptr<FileObject> &file_obj,
                        Gtk::Image *icon, Gtk::Label *label);

  Glib::RefPtr<CombinedListModel> m_model;
  Glib::RefPtr<Gtk::SingleSelection> m_selection;
  Glib::RefPtr<Gtk::SignalListItemFactory> m_factory;
};