#include "db_manager.h"

class Indexer {
public:
  Indexer();
  ~Indexer();

  void indexFileSystem();

private:
  DBManager *m_DBManager;
};