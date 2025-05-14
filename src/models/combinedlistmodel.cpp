#include "../include/combinedlistmodel.h"
#include "../include/applicationobject.h"
#include "../include/fileobject.h"

Glib::RefPtr<CombinedListModel> CombinedListModel::create() {
  return Glib::make_refptr_for_instance(new CombinedListModel());
}

CombinedListModel::CombinedListModel() {
  m_store = Gio::ListStore<Glib::Object>::create();
}

void CombinedListModel::update(const std::vector<Application> &apps,
                               const std::vector<FileResult> &files) {
  m_store->remove_all();

  for (const auto &app : apps) {
    auto app_obj = ApplicationObject::create(app);
    m_store->append(app_obj);
  }

  for (const auto &file : files) {
    auto file_obj = FileObject::create(file);
    m_store->append(file_obj);
  }
}