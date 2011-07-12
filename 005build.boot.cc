//// construct parse tree out of cells

extern Cell* buildCell(AstNode);

list<Cell*> buildCells(list<AstNode> in) {
  list<Cell*> result;
  if (in.empty()) return result;
  for (list<AstNode>::iterator p = in.begin(); p != in.end(); ++p)
    result.push_back(buildCell(*p));
  return result;
}

Cell* buildCell(AstNode n) {
  if (n.isNil())
    return nil;

  if (n.isAtom()) {
    if (n.atom.token == L")")
      cerr << "Syntax error: unbalanced )" << endl << DIE;

    char* end;
    long v = wcstol(n.atom.token.c_str(), &end, 0);
    if (*end == L'\0')
      return newNum(v);

    if (n.atom.token.c_str()[0] == L'"')
      return newString(n.atom.token.substr(1, n.atom.token.length()-2));
    return newSym(n.atom.token);
  }

  if (n.elems.front() == L"'" || n.elems.front() == L"`" || n.elems.front() == L"," || n.elems.front() == L",@") {
    Cell* newForm = newCell();
    setCar(newForm, buildCell(n.elems.front()));
    if (n.elems.size() == 2) {
      setCdr(newForm, buildCell(n.elems.back()));
    }
    else {
      n.elems.pop_front();
      setCdr(newForm, buildCell(n));
    }
    return newForm;
  }

  Cell* newForm = NULL;
  Cell* curr = NULL;
  for (list<AstNode>::iterator q = n.elems.begin(); q != n.elems.end(); ++q) {
    if (q->atom == L"(") {
      if (q != n.elems.begin()) cerr << "Syntax error: ( not at start of expr" << endl << DIE;
      continue;
    }
    if (q->atom == L")") {
      if (++q != n.elems.end()) cerr << "Syntax error: ) not at end of expr" << endl << DIE;
      break;
    }

    if (q->atom == L".") {
      ++q;
      if (!curr) cerr << "Syntax error: dot at start of expression" << endl << DIE;
      setCdr(curr, buildCell(*q));
      break;
    }

    if (!curr) {
      newForm = curr = newCell();
    }
    else {
      setCdr(curr, newCell());
      curr = cdr(curr);
    }

    setCar(curr, buildCell(*q));
  }

  return newForm;
}
