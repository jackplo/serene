#include "../include/listview.h"
#include "../include/applicationobject.h"
#include <algorithm>
#include <cstdlib>
#include <gdkmm/pixbuf.h>
#include <gdkmm/texture.h>
#include <giomm/contenttype.h>
#include <giomm/listmodel.h>
#include <gtk/gtkimage.h>
#include <gtkmm/box.h>
#include <gtkmm/eventcontroller.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/eventcontrollermotion.h>
#include <gtkmm/gestureclick.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <iostream>
#include <regex>

ListView::ListView() {
  set_vexpand(true);
  add_css_class("spotlight-list");
  set_single_click_activate(true);
  setup_factory();
  m_model = CombinedListModel::create();

  m_selection = Gtk::SingleSelection::create(m_model->get_store());

  signal_activate().connect(sigc::mem_fun(*this, &ListView::on_row_activated));

  set_model(m_selection);

  auto key_controller = Gtk::EventControllerKey::create();
  key_controller->signal_key_pressed().connect(
      sigc::mem_fun(*this, &ListView::on_key_pressed), false);
  add_controller(key_controller);

  auto click_controller = Gtk::GestureClick::create();
  click_controller->signal_pressed().connect(
      sigc::mem_fun(*this, &ListView::on_click_pressed));
  add_controller(click_controller);
}

ListView::~ListView() {}

void ListView::update_model(const std::vector<Application> &applications,
                            const std::vector<FileResult> &files) {
  m_model->update(applications, files);
}

void ListView::launch_application(const std::string &exec_command) {
  std::string cmd = exec_command + " &";
  system(cmd.c_str());
}

void ListView::open_file(const std::string &path) {
  std::string cmd = "xdg-open \"" + path + "\" &";
  system(cmd.c_str());
}

void ListView::setup_factory() {
  m_factory = Gtk::SignalListItemFactory::create();

  m_factory->signal_setup().connect(
      [](const Glib::RefPtr<Gtk::ListItem> &item) {
        auto box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 6);
        box->set_margin(6); // Single call instead of 4 separate calls

        auto icon = Gtk::make_managed<Gtk::Image>();
        icon->set_icon_size(Gtk::IconSize::LARGE);

        auto label = Gtk::make_managed<Gtk::Label>();
        label->set_halign(Gtk::Align::START);
        label->set_hexpand(true);
        label->set_ellipsize(Pango::EllipsizeMode::END);

        box->append(*icon);
        box->append(*label);
        item->set_child(*box);
      });

  m_factory->signal_bind().connect(
      [this](const Glib::RefPtr<Gtk::ListItem> &item) {
        auto box = static_cast<Gtk::Box *>(item->get_child());
        auto icon = static_cast<Gtk::Image *>(box->get_first_child());
        auto label = static_cast<Gtk::Label *>(box->get_last_child());

        if (auto app_obj = std::dynamic_pointer_cast<ApplicationObject>(
                item->get_item())) {
          handle_application_item(app_obj, icon, label);
        } else if (auto file_obj = std::dynamic_pointer_cast<FileObject>(
                       item->get_item())) {
          handle_file_item(file_obj, icon, label);
        }
      });

  set_factory(m_factory);
}

bool ListView::on_key_pressed(guint keyval, guint keycode,
                              Gdk::ModifierType state) {
  if (keyval == GDK_KEY_Return || keyval == GDK_KEY_KP_Enter) {
    auto selected_pos = m_selection->get_selected();
    if (selected_pos != GTK_INVALID_LIST_POSITION) {
      on_row_activated(selected_pos);
      return true;
    }
  }
  return false;
}

void ListView::on_click_pressed(int n_press, double x, double y) {
  if (n_press == 2) {
    auto selected_pos = m_selection->get_selected();
    if (selected_pos != GTK_INVALID_LIST_POSITION) {
      on_row_activated(selected_pos);
    }
  }
}

void ListView::on_row_activated(guint position) {
  auto item = m_selection->get_selected_item();
  if (!item)
    return;

  if (auto app_obj = std::dynamic_pointer_cast<ApplicationObject>(item)) {
    launch_application(app_obj->app.exec);
  } else if (auto file_obj = std::dynamic_pointer_cast<FileObject>(item)) {
    open_file(file_obj->result.path);
  }
}

void ListView::handle_application_item(
    const std::shared_ptr<ApplicationObject> &app_obj, Gtk::Image *icon,
    Gtk::Label *label) {
  if (!app_obj->app.icon.empty()) {
    if (app_obj->app.icon[0] == '/') {
      // File path icon
      try {
        auto texture = Gdk::Texture::create_from_file(
            Gio::File::create_for_path(app_obj->app.icon));
        icon->set(texture);
      } catch (const Glib::Error &) {
        icon->set_from_icon_name("applications-system");
      }
    } else {
      // Named icon
      icon->set_from_icon_name(app_obj->app.icon);
    }
  } else {
    icon->set_from_icon_name("applications-system");
  }
  label->set_text(app_obj->app.name);
}

void ListView::handle_file_item(const std::shared_ptr<FileObject> &file_obj,
                                Gtk::Image *icon, Gtk::Label *label) {
  if (file_obj->result.is_directory) {
    icon->set_from_icon_name("folder");
  } else if (file_obj->result.mime_type.find("image/") == 0) {
    // Image file preview
    try {
      auto pixbuf =
          Gdk::Pixbuf::create_from_file(file_obj->result.path, 32, 32, true);
      icon->set(Gdk::Texture::create_for_pixbuf(pixbuf));
    } catch (const Glib::Error &) {
      set_mime_type_icon(icon, file_obj->result.mime_type);
    }
  } else {
    // Regular file with MIME type icon
    set_mime_type_icon(icon, file_obj->result.mime_type);
  }
  label->set_text(file_obj->result.name);
}

void ListView::set_mime_type_icon(Gtk::Image *icon,
                                  const std::string &mime_type) {
  auto content_type = Gio::content_type_from_mime_type(mime_type);
  auto icon_obj = Gio::content_type_get_icon(content_type);

  if (icon_obj) {
    icon->set(icon_obj);
  } else {
    icon->set_from_icon_name("text-x-generic");
  }
}