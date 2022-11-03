"""Utilities for generating C code."""

from typing import List, Optional
import pathlib

def dump_c_header(path: str, var_name: str, indexed_defines: Optional[List[str]] = None) -> None:
  """Dumps the header of a .c file"""
  out_path = pathlib.Path(path).with_suffix('.h')
  header_guard = f'{var_name}_h'.upper()
  data = [
      '#ifndef %s' % header_guard,
      '#define %s' % header_guard,
      '// Generated data for %s' % path,
      '',
  ]
  if indexed_defines:
    for idx, name in enumerate(indexed_defines):
      data.append('#define %-60s %u' % (name, idx))
    data.append('')
  data.extend([
      '#include <inttypes.h>',
      '#include <pico/platform.h>',
      '',
      'extern const uint8_t %s[] __in_flash();' % var_name,
      '',
      '#endif  // %s' % header_guard,
      '',
  ])
  out_path.write_text('\n'.join(data), encoding='utf8')

  print('Wrote %s' % out_path)