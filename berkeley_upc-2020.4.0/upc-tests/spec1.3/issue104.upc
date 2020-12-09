// Tester for "issue 104" compliance: PTS and relational ops
// Copyright 2013, The Regents of the University of California,
// through Lawrence Berkeley National Laboratory (subject to
// receipt of any required approvals from U.S. Dept. of Energy)
// See the full license terms at
//       https://upc.lbl.gov/download/dist/LICENSE.TXT

shared int x;

int main(void) {
  shared int  *p = &x;
  shared void *q = &x;

  // This comparision is prohibited:
  return (p < q);
}