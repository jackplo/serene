#include "applicationlistmodel.h"
#include <iostream>

Glib::RefPtr<ApplicationListModel> ApplicationListModel::create() {
  return Glib::make_refptr_for_instance<ApplicationListModel>(
      new ApplicationListModel());
}

void ApplicationListModel::set_applications(
    const std::vector<Application> &applications) {
  // Clear the store safely
  m_store->remove_all();

  // Don't do anything if applications is empty
  if (applications.empty()) {
    return;
  }

  // Add applications to the store one by one
  for (const auto &app : applications) {
    try {
      auto app_obj = ApplicationObject::create(app);
      m_store->append(app_obj);
    } catch (const std::exception &e) {
      std::cerr << "Error adding application to store: " << e.what()
                << std::endl;
      // Continue with the next application
    }
  }
}

const Application &ApplicationListModel::get_application(int index) const {
  // Add bounds checking
  if (index < 0 || index >= static_cast<int>(m_store->get_n_items())) {
    static Application empty_app; // Return a static empty app if out of bounds
    return empty_app;
  }

  auto app_obj =
      std::dynamic_pointer_cast<ApplicationObject>(m_store->get_object(index));

  // Check if the cast succeeded
  if (!app_obj) {
    static Application empty_app;
    return empty_app;
  }

  return app_obj->app;
}

// Return a Gio::ListModel for compatibility with Gtk::SingleSelection
Glib::RefPtr<Gio::ListModel> ApplicationListModel::get_model() const {
  return m_store;
}

ApplicationListModel::ApplicationListModel() {
  m_store = Gio::ListStore<ApplicationObject>::create();
}