#!/usr/bin/env python3
"""Turns a description .yaml into a output file (debug or c source).

Encoding is for potential efficiencies on a Sharp Memory Display which
has pixels laid out like this

01234567|01234567|...
01234567|01234567|...

To make RLE more effective, we use vertical column stripes 8 pixels wide.
It looks like this

0|3|6
1|4|7
2|5|8

The example above would represent a 18x3 font (not very realistic but it's
an example)  Each number is a 8x1 pixel slice and the data file
contains the bytes in the numbers as-shown.

If a character width is not a multiple of 8 (say it's 9), then a
full extra byte is used to hold the extra.  So a 9-picel-wide font
would be represented with 2 horizontal bytes, enougg for 16 pixels.

This may seem wasteful, but the RLE can be a big help.  For example
the 9 pixel wide font can only be either 0x00 or 0x80 in the second byte
which leads to favorible compression odds.
"""

from typing import Any, Dict, List, IO, Set

import pathlib
import sys
from PIL import Image, ImageDraw, ImageFont
import yaml

class Error(Exception):
  pass

class DuplicateCharacterError(Error):
  pass

class InvalidOutputTypeError(Error):
  pass

class InvalidSectionTypeError(Error):
  pass

class NotAMultipleError(Error):
  pass

class UnknownKeyError(Error):
  pass


class ConfigSection:
  """Used to document and present keys"""

  def __init__(self, section_data: Dict[str, Any]) -> None:
    self.config_keys = (
        ('chars', 'Characters to include.  If omitted, characters 32-127 will be used'),
        ('col_width', 'Fix the column width in pixels.  If omitted, the characters reported column with will be used'),
    )

    known_keys = set(c[0] for c in self.config_keys)
    unknown_keys = known_keys.intersection(self.config_keys)
    if unknown_keys:
      raise UnknownKeyError(
          'Unknown keys in config: %s.  Valid keys include: %s' % (
            sorted(unknown_keys), sorted(section_data)))

    self.data = section_data

  def get(self, key: str, default: Any = None) -> Any:
    if default is None:
      return self.data[key]
    return self.data.get(key, default)


def make_chars(section: ConfigSection) -> Set[str]:
  return set(section.get('chars', ('%c' % x for x in range(32, 127))))

def process_ttf_section(
    char_to_img: Dict[str, Image.Image],
    height: int,
    section: ConfigSection) -> None:
  """Processes a yaml sectin of type ttf."""
  width = section.get('col_width', 0)
  x_offset = section.get('x_offset', 0)
  y_offset = section.get('y_offset', 0)
  x_pad = section.get('x_pad', 1)
  chars = make_chars(section)
  fnt = ImageFont.truetype(section.get('path'), int(section.get('font_size')))

  def make_char(c):
    if width == 0:
      cwidth = int(fnt.getsize(c)[0] + 1) + x_pad * 2
    else:
      cwidth = width
    img = Image.new("1", (cwidth, height))
    d = ImageDraw.Draw(img)
    d.text((x_offset + x_pad, y_offset), c, font=fnt, fill=1)
    if section.get('x_scale', 0):
      # post scale the image
      new_width = int(img.width * section.get('x_scale'))
      img = img.resize((new_width, img.height))

    if section.get('right_trim', 0):
      # trim a litle off the right
      new_right = img.width - section.get('right_trim')
      img = img.crop((0, 0, new_right, img.height))

    if section.get('left_trim', 0):
      # trim a litle off the right
      img = img.crop((section.get('left_trim'), 0, img.width, img.height))

    return img

  for c in chars:
    if c in char_to_img:
      raise DuplicateCharacterError('Duplicate character: %s' % c)
    char_to_img[c] = make_char(c)


def process_image_grid_section(
    char_to_img: Dict[str, Image.Image],
    height: int,
    section: ConfigSection) -> None:
  """Processes a yaml sectin of type image_grid."""
  width = section.get('col_width')

  with Image.open(section.get('path')) as img:
    if img.height % height:
      raise NotAMultipleError(
          'grid height %d is not a multiple of image height %d' % (
            height, img.height))
    if img.width % width:
      raise NotAMultipleError(
          'grid width %d is not a multiple of image width %d' % (
            width, img.width))

    num_columns = img.width // width
    c = '%c' % section.get('first_char')
    for y in range(img.height // height):
      for x in range(num_columns):
        if c in char_to_img:
          raise DuplicateCharacterError('Duplicate character: %s' % c)
        cimg = img.crop((
            x * width,
            y * height,
            x * width + width,
            y * height + height))
        char_to_img[c] = Image.eval(cimg, lambda v: 1-v)
        c = '%c' % (ord(c) + 1)

def process_section(
    char_to_img: Dict[str, Image.Image],
    height: int,
    section: ConfigSection) -> None:
  if section.get('type') == 'ttf':
    process_ttf_section(char_to_img, height, section)
  elif section.get('type') == 'image_grid':
    process_image_grid_section(char_to_img, height, section)
  else:
    raise InvalidSectionTypeError('Invalid section type: %s' % section.get('type'))


def debug_dump(char_to_img: Dict[str, Image.Image]) -> None:
  def dump_char(c, img):
    print()
    print('%s:' % c)
    for y in range(img.height):
      for x in range(img.width):
        if img.getpixel((x,y)):
          print('*', end='')
        else:
          print('.', end='')
      print()

  for c, img in sorted(char_to_img.items()):
    dump_char(c, img)

def dump_c_header(path: str, var_name: str) -> None:
  """Dumps the header of a .c file"""
  out_path = pathlib.Path(path).with_suffix('.h')
  header_guard = f'{var_name}_h'.upper()
  out_path.write_text('\n'.join((
      '#ifndef %s' % header_guard,
      '#define %s' % header_guard,
      '// Generated font data for %s' % path,
      '',
      '#include <inttypes.h>',
      '#include <pico/platform.h>',
      '',
      'extern const uint8_t %s[] __in_flash();' % var_name,
      '',
      '#endif  // %s' % header_guard,
      '',
  )), encoding='utf8')

  print('Wrote %s' % out_path)

def chunks(lst, n):
  """Yield successive n-sized chunks from lst."""
  for i in range(0, len(lst), n):
    yield lst[i:i + n]

#pylint: disable=too-many-branches
def run_length_encode(data: List[int]) -> List[int]:
  """Convert a sequence of data into an RLE form."""
  current_run = []
  current_run_set = set()
  output = []
  for b in data:
    if len(current_run) < 2:
      # Always take the first two
      current_run.append(b)
      current_run_set.add(b)
    elif b == current_run[-1]:
      # There is a sequence
      if len(current_run_set) > 1:
        # Represent the last run as a changing sequence
        # not including it's last character
        output.append(0x80 | (len(current_run) - 1))
        output.extend(current_run[:-1])
        current_run = [b, b]
        current_run_set = set([b])
      else:
        # another entry for the run
        current_run.append(b)
    else:
      if len(current_run_set) == 1:
        # Output the repeating sequence and start a new sequence
        output.append(len(current_run))
        output.append(current_run[0])
        current_run = [b]
        current_run_set = set(current_run)
      else:
        # keep adding to this one
        current_run.append(b)
        current_run_set.add(b)

    if len(current_run) == 127:
      # at this point, we have to output the data because the run length
      # byte is saturated
      if len(current_run_set) == 1:
        output.append(len(current_run))
        output.append(current_run[0])
      else:
        output.append(0x80 | len(current_run))
        output.extend(current_run)

      current_run = []
      current_run_set = set()

  if current_run:
    # still a bit more to do
    if len(current_run_set) == 1:
      output.append(len(current_run))
      output.append(current_run[0])
    else:
      output.append(0x80 | len(current_run))
      output.extend(current_run)

  return output
#pylint: enable=too-many-branches

def generate_character_comment(c: str, img: Image.Image, fout: IO) -> None:
  if ord(c) < 32 or ord(c) > 128:
    c_rep = ''
  else:
    c_rep = ' (%c)' % c
  fout.write('    // Character %d (0x%02X)%s\n' % (ord(c), ord(c), c_rep))
  for y in range(img.height):
    fout.write('    // ')
    for x in range(img.width):
      fout.write('#' if img.getpixel((x,y)) else '-')
    fout.write('\n')

special_chars = '\'\\'

def generate_offsets(
    fout: IO,
    char_to_img: Dict[str, Image.Image],
    char_to_data: Dict[str, List[int]]) -> None:
  fout.write('    // Character offsets\n')
  # 3 bytes per character plus the end byte
  offset = len(char_to_img) * 4
  for c, img in sorted(char_to_img.items()):
    if ord(c) < 32 or ord(c) > 128 or c in special_chars:
      c_rep = '%d' % ord(c)
    else:
      c_rep = '\'%c\'' % c
    fout.write('    %s, %d, 0x%02X, 0x%02X,  // off=%d\n' % (
    c_rep, img.width, offset >> 8, offset & 0xFF, offset))
    offset += len(char_to_data[c])


def generate_character_data(data: List[int], fout: IO) -> None:
  for chunk in chunks(data, 16):
    fout.write('    %s,\n' % ', '.join('0x%02X' % b for b in chunk))


def create_rle_data(height: int, img: Image.Image) -> List[int]:
  data = []
  cols = (img.width + 7) // 8
  for col in range(cols):
    for y in range(height):
      startx = col * 8
      endx = min(startx + 8, img.width)
      byte = 0x00
      bit = 7
      for x in range(startx, endx):
        if img.getpixel((x, y)):
          byte = byte | 1 << bit
        bit -= 1
      data.append(byte)

  return run_length_encode(data)


def variable_font_dump(
    path: str,
    char_to_img: Dict[str, Image.Image]) -> None:
  """Dumps .c and .h files."""
  out_path = pathlib.Path(path).with_suffix('.c')
  var_name = out_path.with_suffix('').name.replace('.', '_').replace('-', '_')
  dump_c_header(path, var_name)

  height = next(iter(char_to_img.values())).height

  with out_path.open('w', encoding='utf8') as fout:
    fout.write('\n'.join((
        '// Generated font data for %s' % var_name,
        '',
        '#include <inttypes.h>',
        '#include <pico/platform.h>',
        '',
        'const uint8_t %s[] __in_flash() = {' % var_name,
        '    0x53, 0x48, 0x46, 0x31,  // id: SHF1',
        '    0x%02X, // num_chars' % len(char_to_img),
        '    0x%02X, // height' % height,
        '',
        '',
    )))

    char_to_data = {
        c:create_rle_data(height, img) for c, img in char_to_img.items()}
    generate_offsets(fout, char_to_img, char_to_data)

    fout.write('    // Character data\n')
    for c, img in sorted(char_to_img.items()):
      generate_character_comment(c, img, fout)
      generate_character_data(char_to_data[c], fout)

    fout.write('};\n')

  print('Wrote %s' % out_path)


def main():
  """Entry point."""
  if len(sys.argv) != 2:
    sys.exit('Usage: make_var_ascii_font.py <config.yaml>')

  path = sys.argv[1]
  with open(path, encoding='utf8') as f:
    cfg = yaml.safe_load(f)

  # creates a map of character to image
  char_to_img = {}

  height = int(cfg['height'])
  for section in cfg['sections']:
    process_section(char_to_img, height, ConfigSection(section))

  if cfg['output_type'] == 'debug':
    debug_dump(char_to_img)
  elif cfg['output_type'] == 'SharpMemoryFont':
    variable_font_dump(path, char_to_img)
  else:
    raise InvalidOutputTypeError(
        'Invalid output type: %s' % cfg['output_type'])


if __name__ == '__main__':
  main()
