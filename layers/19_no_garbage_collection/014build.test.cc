void test_build_handles_nil() {
  stringstream in("()");
  CHECK_EQ(nextCell(in), nil);
}

void test_build_handles_nil2() {
  stringstream in("nil");
  CHECK_EQ(nextCell(in), nil);
}

void test_build_handles_integer() {
  stringstream in("34");
  Cell* c = nextCell(in);
  CHECK_EQ(c, newNum(34));
}

void test_build_handles_float() {
  stringstream in("3.4");
  Cell* c = nextCell(in);
  CHECK(isNum(c));
  CHECK(equalFloats(toFloat(c), 3.4));
}

void test_build_warns_on_ambiguous_float() {
  stringstream in("-.4");
  Cell* c = nextCell(in);
  CHECK_EQ(raiseCount, 1); raiseCount=0;
  CHECK(isNum(c));
  CHECK(equalFloats(toFloat(c), -0.4));
}

void test_build_creates_floats_on_overflow() {
  stringstream in("100000000000000000000");
  Cell* c = nextCell(in);
  CHECK_EQ(raiseCount, 1); raiseCount=0;   // overflow warning
  CHECK_EQ(c->type, FLOAT);
}

void test_build_handles_sym() {
  stringstream in("a");
  Cell* c = nextCell(in);
  CHECK_EQ(c, newSym("a"));
}

void test_build_handles_string() {
  stringstream in("\"a\"");
  Cell* c = nextCell(in);
  CHECK_EQ(toString(c), "a");
}

void test_build_doesnt_mix_syms_and_strings() {
  Cell* s = newString("a");
  CHECK(s != newSym("a"));
}

void test_build_handles_quoted_sym() {
  stringstream in("'a");
  Cell* c = nextCell(in);
  CHECK_EQ(car(c), newSym("'"));
  CHECK_EQ(cdr(c), newSym("a"));
}

void test_build_handles_multiple_atoms() {
  stringstream in("34\n35");
  Cell* c = nextCell(in);
  CHECK_EQ(c, newNum(34));
  CHECK_EQ(cdr(c), nil);

  c = nextCell(in);
  CHECK_EQ(c, newNum(35));
  CHECK_EQ(cdr(c), nil);
}

void test_build_handles_form() {
  stringstream in("(34 35)");
  Cell* c=nextCell(in);
  CHECK_EQ(car(c), newNum(34));

  c = cdr(c);
  CHECK_EQ(car(c), newNum(35));

  CHECK_EQ(cdr(c), nil);
}

void test_build_handles_dotted_list() {
  stringstream in("(34 ... 35)");
  Cell* c=nextCell(in);
  CHECK_EQ(car(c), newNum(34));

  c = cdr(c);
  CHECK_EQ(c, newNum(35));
}

void test_build_handles_literal_ellipses() {
  stringstream in("'...");
  Cell *c=nextCell(in);
  CHECK_EQ(car(c), newSym("'"));
  CHECK_EQ(cdr(c), newSym("..."));
}

void test_build_handles_nested_form() {
  stringstream in("(3 7 (33 23))");
  Cell* c=nextCell(in);
  CHECK_EQ(car(c), newNum(3));

  c = cdr(c);
  CHECK_EQ(car(c), newNum(7));

  c = cdr(c);
    Cell* c2 = car(c);
    CHECK_EQ(car(c2), newNum(33));
    c2 = cdr(c2);
    CHECK_EQ(car(c2), newNum(23));
    CHECK_EQ(cdr(c2), nil);
  CHECK_EQ(cdr(c), nil);
}

void test_build_handles_strings() {
  stringstream in("(3 7 (33 \"abc\" 23))");
  Cell* c=nextCell(in);
  CHECK_EQ(car(c), newNum(3));
  c = cdr(c);
  CHECK_EQ(car(c), newNum(7));
  c = cdr(c);
    Cell* c2 = car(c);
    CHECK_EQ(car(c2), newNum(33));
    c2 = cdr(c2);
    CHECK(isString(car(c2)));
    CHECK_EQ(toString(car(c2)), "abc");
    c2 = cdr(c2);
    CHECK_EQ(car(c2), newNum(23));
    CHECK_EQ(cdr(c2), nil);
  CHECK_EQ(cdr(c), nil);
}

void test_build_handles_syms() {
  stringstream in("(3 7 (33 \"abc\" 3de 23))");
  Cell* c=nextCell(in);
  CHECK_EQ(car(c), newNum(3));
  c = cdr(c);
  CHECK_EQ(car(c), newNum(7));
  c = cdr(c);
    Cell* c2 = car(c);
    CHECK_EQ(car(c2), newNum(33));
    c2 = cdr(c2);
    CHECK(isString(car(c2)));
    CHECK_EQ(toString(car(c2)), "abc");
    c2 = cdr(c2);
    CHECK_EQ(car(c2), newSym("3de"));
    c2 = cdr(c2);
    CHECK_EQ(car(c2), newNum(23));
    CHECK_EQ(cdr(c2), nil);
  CHECK_EQ(cdr(c), nil);
}

void test_build_handles_indented_wrapped_lines() {
  stringstream in("a\n  (a b c\n   d e)");
  Cell *c0=nextCell(in);
  CHECK_EQ(c0, newSym("a"));

  Cell* c=nextCell(in);
  CHECK_EQ(car(c), newSym("a"));
  c = cdr(c);
  CHECK_EQ(car(c), newSym("b"));
  c = cdr(c);
  CHECK_EQ(car(c), newSym("c"));
  c = cdr(c);
  CHECK_EQ(car(c), newSym("d"));
  c = cdr(c);
  CHECK_EQ(car(c), newSym("e"));
  c = cdr(c);
  CHECK_EQ(c, nil);
}