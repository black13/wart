void test_list_splice_replaces_index() {
  newDynamicScope(L"a", newCons(newNum(3), newCons(newNum(4), nil)));
  Cell* expr = wartRead(stream(L"list_splice a 1 2 '(5)")).front();
  Cell* call = eval(expr);
  checkEq(car(lookup(L"a")), newNum(3));
  checkEq(car(cdr(lookup(L"a"))), newNum(5));
  rmref(call);
  rmref(expr);
  endDynamicScope(L"a");
}

void test_list_splice_replaces_first_index() {
  newDynamicScope(L"a", newCons(newNum(3), newCons(newNum(4), nil)));
  Cell* expr = wartRead(stream(L"list_splice a 0 1 '(9)")).front();
  Cell* call = eval(expr);
  checkEq(car(lookup(L"a")), newNum(9));
  rmref(call);
  rmref(expr);
  endDynamicScope(L"a");
}

void test_list_splice_inserts_list_at_beginning() {
  newDynamicScope(L"a", newCons(newNum(3), newCons(newNum(4), nil)));
  Cell* expr = wartRead(stream(L"list_splice a 0 1 '(7 9)")).front();
  Cell* call = eval(expr);
  checkEq(car(lookup(L"a")), newNum(7));
  checkEq(car(cdr(lookup(L"a"))), newNum(9));
  rmref(call);
  rmref(expr);
  endDynamicScope(L"a");
}

void test_list_splice_inserts_shorter_list() {
  newDynamicScope(L"a", newCons(newNum(3), newCons(newNum(4), newCons(newNum(5), nil))));
  Cell* expr = wartRead(stream(L"list_splice a 1 3 '(6)")).front();
  Cell* call = eval(expr);
  checkEq(car(lookup(L"a")), newNum(3));
  checkEq(car(cdr(lookup(L"a"))), newNum(6));
  checkEq(cdr(cdr(lookup(L"a"))), nil);
  rmref(call);
  rmref(expr);
  endDynamicScope(L"a");
}

void test_list_splice_inserts_longer_list() {
  newDynamicScope(L"a", newCons(newNum(3), newCons(newNum(4), newCons(newNum(5), nil))));
  Cell* expr = wartRead(stream(L"list_splice a 1 2 '(6 7 8)")).front();
  Cell* call = eval(expr);
  Cell* curr = lookup(L"a");
  checkEq(car(curr), newNum(3)); curr=cdr(curr);
  checkEq(car(curr), newNum(6)); curr=cdr(curr);
  checkEq(car(curr), newNum(7)); curr=cdr(curr);
  checkEq(car(curr), newNum(8)); curr=cdr(curr);
  checkEq(car(curr), newNum(5)); curr=cdr(curr);
  checkEq(curr, nil);
  rmref(call);
  rmref(expr);
  endDynamicScope(L"a");
}

void test_list_splice_inserts_nil() {
  newDynamicScope(L"a", newCons(newNum(3), newCons(newNum(4), nil)));
  Cell* expr = wartRead(stream(L"list_splice a 0 1 '(nil)")).front();
  Cell* call = eval(expr);
  checkEq(car(lookup(L"a")), nil);
  checkEq(car(cdr(lookup(L"a"))), newNum(4));
  checkEq(cdr(cdr(lookup(L"a"))), nil);
  rmref(call);
  rmref(expr);
  endDynamicScope(L"a");
}
