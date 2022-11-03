#!/usr/bin/env python3
"""Turns a description .yaml into an image outfile file (c source)

Encoding is for potential efficiencies on a Sharp Memory Display which
has pixels laid out like this

01234567|01234567|...
01234567|01234567|...

To make RLE more effective, we use vertical column stripes 8 pixels wide.
It looks like this

0|3|6
1|4|7
2|5|8

The example above would represent a 24x3 image.

If an imagewidth is not a multiple of 8 (say it's 9), then a
full extra byte is used to hold the extra.  So a 9-pixel-wide image
would be represented with 2 horizontal bytes, enough for 16 pixels.

This may seem wasteful, but the RLE can be a big help.  For example
the 9 pixel wide image can only be either 0x00 or 0x80 in the second byte
which leads to favorible compression odds.
"""

from typing import Any, Dict, List, Tuple

import os
import sys
import yaml

from PIL import Image

from lib import rle, codegen

class Error(Exception):
  pass

class UnknownKeyError(Error):
  pass


class ConfigSection:
  """Used to document and present keys"""

  def __init__(self, section_data: Dict[str, Any]) -> None:
    self.config_keys = (
        ('path', 'Path to the image'),
         'name', 'The #define name.  Default to a names that is based on the file name.'
    )

    known_keys = set(c[0] for c in self.config_keys)
    unknown_keys = set(section_data).difference(known_keys)
    if unknown_keys:
      raise UnknownKeyError(
          'Unknown keys in config: %s.  Valid keys include: %s' % (
            sorted(unknown_keys), sorted(known_keys)))

    self.data = section_data

  def get(self, key: str, default: Any = None) -> Any:
    if default is None:
      return self.data[key]
    return self.data.get(key, default)


def process_section(section: ConfigSection) -> Tuple[str, Image.Image]:
  name = section.get('Name', '')
  path = section.get('path')
  if not name:
    name = os.path.splitext(os.path.split(path)[1])[0].upper()
    first_char = name[0]
    if first_char < 'A' or first_char > 'Z':
      name = 'I' + name
  with Image.open(path) as img:
    return (name, img.copy())


def u32_to_hexstr(val: int) -> str:
  return "0x%02X 0x%02X 0x%02X 0x%02X" % (
    val & 0xFF,
    (val >> 8) & 0xFF,
    (val >> 16) & 0xFF,
    (val >> 24) & 0xFF
  )

def dump_sources(path: str, image_list: List[Tuple[str, Image.Image]]) -> None:
  out_path = pathlib.Path(path).with_suffix('.c')
  var_name = out_path.with_suffix('').name.replace('.', '_').replace('-', '_')
  codegen.dump_c_header(path, var_name, [name for name, _ in image_list])
  
  with out_path.open('w', encoding='utf8') as fout:
    fout.write('\n'.join((
        '// Generated image data for %s' % var_name,
        '',
        '#include <inttypes.h>',
        '#include <pico/platform.h>',
        '',
        '// Note: all values are little endian per r2040 spec',
        '',
        'const uint8_t %s[] __in_flash() = {' % var_name,
        '    0x53, 0x48, 0x49, 0x31,  // id: SHI1',
        '    %s, // image count' % u32_to_hexstr(len(image_list)),
    )))
  # finish this later


def main():
  """Entry point."""
  if len(sys.argv) != 2:
    sys.exit('Usage: make_images.py <config.yaml>')

  path = sys.argv[1]
  with open(path, encoding='utf8') as f:
    cfg = yaml.safe_load(f)

  # creates a list of (name, image) tuples
  config_sections = (ConfigSection(section) for section in cfg['images'])
  image_list = [process_section(section) for section in config_sections]
  dump_sources(path, image_list)


if __name__ == '__main__':
  main()