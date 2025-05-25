#include "../include/combinedlistmodel.h"
#include "../include/applicationobject.h"
#include "../include/fileobject.h"

Glib::RefPtr<CombinedListModel> CombinedListModel::create() {
  return Glib::make_refptr_for_instance(new CombinedListModel());
}

CombinedListModel::CombinedListModel() {
  m_store = Gio::ListStore<Glib::Object>::create();
}

void CombinedListModel::update(const std::vector<Result> &results) {
  m_store->remove_all();

  for (const auto &result : results) {
    if (result.type == ResultType::APPLICATION) {
      auto app_obj = ApplicationObject::create(result);
      m_store->append(app_obj);
    } else if (result.type == ResultType::FILE) {
      auto file_obj = FileObject::create(result);
      m_store->append(file_obj);
    }
  }
}