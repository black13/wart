void test_parse_handles_empty_stream() {
  stringstream in("");
  read_all(in);
  check_trace_contents("parse", "\n");  // extra newline artifact
}

void test_parse_handles_trailing_comment() {
  stringstream in("34 # abc");
  read_all(in);
  check_trace_contents("parse", "34\n\n");  // extra newline artifact
}

void test_parse_handles_atom() {
  stringstream in("34");
  read_all(in);
  check_trace_contents("parse", "34\n");
}

void test_parse_handles_atoms() {
  stringstream in("34\n\"a b c\"\n3.4");
  read_all(in);
  check_trace_contents("parse", "34\n\"a b c\"\n3.4\n");
}

void test_parse_handles_forms() {
  stringstream in("(34 \"a b c\")");
  read_all(in);
  check_trace_contents2("parse", 1, "(34 \"a b c\")\n");
}

void test_parse_handles_nested_forms() {
  stringstream in("(34 (2 3) \"a b c\")");
  read_all(in);
  check_trace_contents2("parse", 1, "(34 (2 3) \"a b c\")\n");
  check_trace_contents2("parse", 2, "34\n(2 3)\n\"a b c\"\n)\n");
  check_trace_contents2("parse", 3, "2\n3\n)\n");
}

void test_parse_handles_nested_forms_with_comments() {
  stringstream in("(a b (c d #\n))");
  read_all(in);
  check_trace_contents2("parse", 1, "(a b (c d))\n");
  check_trace_contents2("parse", 2, "a\nb\n(c d)\n)\n");
}
