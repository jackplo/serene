#include "listview.h"
#include "../models/applicationobject.h"
#include <giomm/listmodel.h>
#include <gtkmm/box.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <iostream>

ListView::ListView() {
  set_vexpand(true);
  add_css_class("spotlight-list");
  setup_factory();
  m_model = ApplicationListModel::create();

  // Pass the model as a Gio::ListModel to SingleSelection
  m_selection = Gtk::SingleSelection::create(m_model->get_model());
  set_model(m_selection);
}

ListView::~ListView() {}

void ListView::setup_factory() {
  m_factory = Gtk::SignalListItemFactory::create();

  m_factory->signal_setup().connect(
      [](const Glib::RefPtr<Gtk::ListItem> &list_item) {
        auto box =
            Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 12);
        box->set_margin_start(6);
        box->set_margin_end(6);
        box->set_margin_top(6);
        box->set_margin_bottom(6);

        auto icon = Gtk::make_managed<Gtk::Image>();
        icon->set_size_request(32, 32);

        auto name_label = Gtk::make_managed<Gtk::Label>();
        name_label->set_xalign(0);
        name_label->set_hexpand(true);

        box->append(*icon);
        box->append(*name_label);
        list_item->set_child(*box);
      });

  m_factory->signal_bind().connect(
      [](const Glib::RefPtr<Gtk::ListItem> &list_item) {
        auto box = dynamic_cast<Gtk::Box *>(list_item->get_child());
        if (!box)
          return;

        // Get the ApplicationObject from the list item
        auto app_obj =
            std::dynamic_pointer_cast<ApplicationObject>(list_item->get_item());
        if (!app_obj)
          return;

        // Get the Application data from the object
        const Application &app = app_obj->app;

        auto icon = dynamic_cast<Gtk::Image *>(box->get_first_child());
        auto label = dynamic_cast<Gtk::Label *>(box->get_last_child());

        if (icon && label) {
          // Set icon
          if (!app.icon.empty()) {
            icon->set_from_icon_name(app.icon);
          } else {
            icon->set_from_icon_name("application-x-executable");
          }

          // Set label
          label->set_text(app.name);

          // Set tooltip if comment exists
          if (!app.comment.empty()) {
            label->set_tooltip_text(app.comment);
          }
        }
      });

  set_factory(m_factory);
}

void ListView::update_model(const std::vector<Application> &applications) {
  m_model->set_applications(applications);
}