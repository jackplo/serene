CREATE TABLE IF NOT EXISTS applications (
    id INTEGER PRIMARY KEY,
    name TEXT,
    exec TEXT,
    icon TEXT,
    comment TEXT,
    desktop_file TEXT,
    terminal INTEGER
);

CREATE TABLE IF NOT EXISTS files (
    id INTEGER PRIMARY KEY,
    name TEXT,
    path TEXT,
    is_dir INTEGER,
    mime_type TEXT,
    mtime INTEGER
);
