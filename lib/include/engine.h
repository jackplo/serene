#include "app_searcher.h"
#include "file_searcher.h"
#include "result.h"
#include <memory>
#include <string>
#include <vector>

class Engine {

public:
  Engine();
  ~Engine();

  bool initialize();
  std::vector<Result> search(const std::string &query, size_t max_results = 20);
  Result get_details(const std::string &id);

private:
  std::unique_ptr<AppSearcher> m_appSearcher;
  std::unique_ptr<FileSearcher> m_fileSearcher;
};