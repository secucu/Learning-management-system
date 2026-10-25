PRAGMA foreign_keys=ON;
PRAGMA journal_mode=WAL;
CREATE TABLE IF NOT EXISTS departments (
  id INTEGER PRIMARY KEY,
  name TEXT UNIQUE NOT NULL
);
CREATE TABLE IF NOT EXISTS users (
  id INTEGER PRIMARY KEY,
  username TEXT UNIQUE NOT NULL,
  salt TEXT NOT NULL,
  password_hash TEXT NOT NULL,
  role TEXT NOT NULL CHECK(role IN ('admin','lecturer','hod','student')),
  department_id INTEGER REFERENCES departments(id)
);
CREATE TABLE IF NOT EXISTS courses (
  id INTEGER PRIMARY KEY,
  code TEXT UNIQUE NOT NULL,
  name TEXT NOT NULL,
  department_id INTEGER NOT NULL REFERENCES departments(id),
  lecturer_id INTEGER REFERENCES users(id)
);
CREATE TABLE IF NOT EXISTS enrolments (
  student_id INTEGER NOT NULL REFERENCES users(id),
  course_id INTEGER NOT NULL REFERENCES courses(id),
  PRIMARY KEY(student_id,course_id)
);
CREATE TABLE IF NOT EXISTS assignments (
  id INTEGER PRIMARY KEY,
  course_id INTEGER NOT NULL REFERENCES courses(id),
  title TEXT NOT NULL,
  deadline TEXT NOT NULL,
  max_mark REAL NOT NULL CHECK(max_mark>0)
);
CREATE TABLE IF NOT EXISTS extensions (
  assignment_id INTEGER NOT NULL REFERENCES assignments(id),
  student_id INTEGER NOT NULL REFERENCES users(id),
  deadline TEXT NOT NULL,
  PRIMARY KEY(assignment_id,student_id)
);
CREATE TABLE IF NOT EXISTS submissions (
  id INTEGER PRIMARY KEY,
  assignment_id INTEGER NOT NULL REFERENCES assignments(id),
  student_id INTEGER NOT NULL REFERENCES users(id),
  submitted_at TEXT NOT NULL,
  work TEXT NOT NULL,
  status TEXT NOT NULL CHECK(status IN ('submitted','late','marked','moderated','released')),
  mark REAL,
  feedback TEXT NOT NULL DEFAULT '',
  late_days INTEGER NOT NULL DEFAULT 0,
  UNIQUE(assignment_id,student_id)
);
CREATE TABLE IF NOT EXISTS mark_audit (
  id INTEGER PRIMARY KEY,
  submission_id INTEGER NOT NULL REFERENCES submissions(id),
  actor_id INTEGER NOT NULL REFERENCES users(id),
  changed_at TEXT NOT NULL,
  old_mark REAL,
  new_mark REAL,
  reason TEXT NOT NULL
);
