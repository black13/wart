COMPILE_PRIM_FUNC(string_splice, primFunc_string_splice, L"($string $start $end $val)",
  Cell* str = lookup(L"$string");
  if (!isString(str)) {
    warn << "can't set non-string: " << str << endl;
    return nil;
  }

  size_t start = toNum(lookup(L"$start"));
  size_t end = toNum(lookup(L"$end"));
  if (start > ((string*)str->car)->length()) { // append works
    warn << "string too short: " << str << " " << start << endl;
    return nil;
  }

  Cell* val = lookup(L"$val");
  if (!isString(val))
    warn << "can't set string with non-string: " << val << endl;
  ((string*)str->car)->replace(start, end-start, toString(val));
  return mkref(val);
)

COMPILE_PRIM_FUNC(string_get, primFunc_string_get, L"($string $index $end)",
  Cell* str = lookup(L"$string");
  if (!isString(str)) {
    warn << "not a string: " << str << endl;
    return nil;
  }

  size_t index = toNum(lookup(L"$index"));
  if (index > ((string*)str->car)->length()-1) {
    warn << "no such index in string: " << str << " " << index << endl;
    return nil;
  }

  size_t end = (lookup(L"$end") != nil) ? toNum(lookup(L"$end")) : index+1;
  if (end > ((string*)str->car)->length()-1) {
    warn << "no such end-index in string: " << str << " " << end << endl;
    return nil;
  }

  return mkref(newString(toString(str).substr(index, end-index)));
)
