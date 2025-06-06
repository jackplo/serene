#include "../include/engine.h"

Engine::Engine() {
  m_appSearcher = std::make_unique<AppSearcher>();
  m_fileSearcher = std::make_unique<FileSearcher>();
}

Engine::~Engine() {}

/*
bool Engine::initialize() {
  return m_appSearcher->initialize() && m_fileSearcher->initialize();
}
*/

std::vector<Result> Engine::search(const std::string &query,
                                   size_t max_results) {
  std::vector<Result> results;

  auto app_results = m_appSearcher->search(query);
  auto file_results = m_fileSearcher->search(query, max_results);

  for (const auto &app : app_results) {
    results.push_back(Result::from_application(app));
  }

  for (const auto &file : file_results) {
    results.push_back(Result::from_file(file));
  }

  return results;
}

/*
Result Engine::get_details(const std::string &id) {
    return m_appSearcher->get_details(id) + m_fileSearcher->get_details(id);
}
*/