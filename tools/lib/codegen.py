"""Utilities for generating C code."""

from typing import Any, IO, List, Optional, Tuple
import pathlib

def dump_c_header(path: str, var_name: str, defines: Optional[List[Tuple[str, Any]]] = None) -> None:
  """Dumps the header of a .c file"""
  out_path = pathlib.Path(path).with_suffix('.h')
  header_guard = f'{var_name}_h'.upper()
  data = [
      '#ifndef %s' % header_guard,
      '#define %s' % header_guard,
      '// Generated data for %s' % path,
      '',
  ]
  if defines:
    for name, value in defines:
      data.append('#define %-30s %s' % (name, value))
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


def chunks(lst, n):
  """Yield successive n-sized chunks from lst."""
  for i in range(0, len(lst), n):
    yield lst[i:i + n]


def generate_character_data(data: List[int], fout: IO) -> None:
  for chunk in chunks(data, 16):
    fout.write('    %s,\n' % ', '.join('0x%02X' % b for b in chunk))

