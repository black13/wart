//// construct parse tree out of a list of tokens

struct AstNode {
  Token atom;
  list<AstNode> elems;

  explicit AstNode(Token t) :atom(t) {}
  explicit AstNode(list<AstNode> l) :atom(eof()), elems(l) {}

  bool operator==(const Token& x) const {
    return elems.empty() && atom == x.token;  // whitespace should be gone by now
  }
  bool operator==(const string& x) const {
    return elems.empty() && atom == x;
  }
  bool operator!=(const Token& x) const {
    return !(*this == x);
  }
  bool operator!=(const string& x) const {
    return !(*this == x);
  }
};

AstNode nextAstNode(IndentSensitiveStream& in) {
  list<Token> bufferedTokens = nextExpr(in);
  return nextAstNode(bufferedTokens);
}

AstNode nextAstNode(list<Token>& in) {
  list<AstNode> subform;
  if (in.empty()) return AstNode(subform);

  subform.push_back(AstNode(nextToken(in)));
  while (!in.empty() && isQuoteOrUnquote(subform.back().atom))
    subform.push_back(AstNode(nextToken(in)));

  if (isOpenParen(subform.back())) {
    while (!in.empty() && subform.back() != ")")
      subform.push_back(nextAstNode(in));
    if (!isCloseParen(subform.back())) RAISE << "Unbalanced (" << endl << DIE;
  }

  if (subform.size() == 1)
    return AstNode(subform.back());
  return AstNode(subform);
}



//// internals

Token nextToken(list<Token>& in) {
  Token result = in.front(); in.pop_front();
  return result;
}

Token eof() {
  return Token(0);
}

bool isList(const AstNode& n) {
  return !n.elems.empty();
}

bool isAtom(const AstNode& n) {
  return n.elems.empty();
}

bool isQuoteOrUnquote(const AstNode& n) {
  return isAtom(n) && isQuoteOrUnquote(n.atom);
}

bool isOpenParen(const AstNode& n) {
  return n == "(" || n == ":(";
}
bool isCloseParen(const AstNode& n) {
  return n == ")";
}

ostream& operator<<(ostream& os, AstNode x) {
  if (x.elems.empty()) return os << x.atom;
  bool skipNextSpace = true;
  for (list<AstNode>::iterator p = x.elems.begin(); p != x.elems.end(); ++p) {
    if (!isCloseParen(*p) && !skipNextSpace)
      os << " ";
    os << *p;
    skipNextSpace = (isOpenParen(*p) || isQuoteOrUnquote(*p));
  }
  return os;
}
