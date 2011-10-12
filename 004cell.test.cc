void test_pointers_from_nil_are_nil() {
  checkEq(nil->car, nil);
  checkEq(nil->cdr, nil);
}

void test_newCell_has_nil_car_and_cdr() {
  Cell* x = newCell();
  checkEq(x->car, nil);
  checkEq(x->cdr, nil);
  rmref(x);
}

void test_rmref_frees_space() {
  Cell* c = newCell();
  checkEq(c->car, nil);
  checkEq(freelist, NULL);
  rmref(c);
  check(!c->car);
  checkEq(freelist, c);
}

void test_rmref_handles_nums() {
  Cell* c = newCell();
  c->type = NUM;
  c->car = (Cell*)34;
  rmref(c);
  check(!c->car);
  checkEq(freelist, c);
}
