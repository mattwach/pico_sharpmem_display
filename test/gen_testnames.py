#!/usr/bin/env python3

"""This utility collects tests names and creates a tests.inc file"""

from typing import List
import glob

def get_functions(path: str) -> List[str]:
  fn_list = []
  with open(path) as fin:
    for line in fin:
      if '(' not in line:
        continue
      line, _ = line.split('(', 1)
      tokens = line.split()
      if len(tokens) < 3:
        continue
      if tokens[0] != 'struct':
        continue
      if tokens[1] != 'TestData*':
        continue
      fn_name = tokens[2].strip()
      if fn_name.startswith('test_'):
        fn_list.append(fn_name)
  return fn_list

def create_signatures(fn_list: List[str]) -> List[str]:
  def make_sig(name):
    return f'struct TestData* {name}(struct Bitmap* bitmap);'
  return [make_sig(name) for name in fn_list]

def create_array(fn_list: List[str]) -> List[str]:
  lines = [
      'struct TestData* (*tests[])(struct Bitmap*) = {',
  ]
  lines.extend(f'  {name},' for name in fn_list)
  lines.append('};')
  return lines

def generate_output(fn_list: List[str]) -> None:
  lines = [
      '// THIS FILE IS AUTOGENERATED',
      '// Run make or ./gen_testnames.py to create it',
      '',
  ]
  lines.extend(create_signatures(fn_list))
  lines.append('')
  lines.extend(create_array(fn_list))
  lines.append('')
  with open('tests.inc', 'w') as fout:
    fout.write('\n'.join(lines))
  print('Wrote tests.inc')

def main():
  fn_list = []
  for path in glob.glob('*tests.c'):
    fn_list.extend(get_functions(path))
  generate_output(sorted(fn_list))

if __name__ == '__main__':
    main()
