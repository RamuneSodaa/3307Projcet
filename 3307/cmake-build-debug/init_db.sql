CREATE TABLE IF NOT EXISTS courses (
                                       course_id INTEGER PRIMARY KEY,
                                       name TEXT NOT NULL,
                                       schedule TEXT NOT NULL,
                                       capacity INTEGER NOT NULL,
                                       prerequisites TEXT
);

CREATE TABLE IF NOT EXISTS students (
                                        student_id INTEGER PRIMARY KEY,
                                        username TEXT NOT NULL,
                                        email TEXT NOT NULL,
                                        status TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS enrollments (
                                           student_id INTEGER NOT NULL,
                                           course_id INTEGER NOT NULL,
                                           course_name TEXT NOT NULL,
                                           enrollment_date TEXT NOT NULL,
                                           FOREIGN KEY(student_id) REFERENCES students(student_id),
                                           FOREIGN KEY(course_id) REFERENCES courses(course_id),
                                           UNIQUE(student_id, course_id)
);
