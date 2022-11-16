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

from typing import Any, Dict, IO, List, Tuple

from dataclasses import dataclass
import os
import pathlib
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
        ('name', 'The #define name.  Default to a names that is based on the file name.'),
        ('invert', 'If true, then white pixels become the 1\'s in the image'),
        ('tile_x', 'If > 0, break the image into tiles.'),
        ('tile_y', 'If > 0, break the image into tiles.'),
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

@dataclass
class ImageProps:
  name: str
  invert: bool
  image: Image.Image
  rle: List[int]


def add_image_as_tiles(
  image_list: List[ImageProps],
  defines: Dict[str, Any],
  img: Image.Image,
  name: str,
  invert: bool,
  tile_x: int,
  tile_y: int) -> None:
  if (tile_x == 0) or (tile_x > img.width):
    tile_x = img.width
  if (tile_y == 0) or (tile_y > img.height):
    tile_y = img.height

  # need to round up the values
  columns = int((img.width + tile_x - 1) / tile_x)
  rows = int((img.height + tile_y - 1) / tile_y)

  defines[f'{name}_COLUMNS'] = columns
  defines[f'{name}_ROWS'] = rows

  for row in range(0, rows):
    for column in range(0, columns):
      left = column * tile_x
      right = min(left + tile_x, img.width)
      top = row * tile_y
      bottom = min(top + tile_y, img.height)
      image_list.append(ImageProps(
          name=f'{name}_{column}_{row}',
          invert=invert,
          image=img.crop([left, top, right, bottom]),
          rle=[]
      ))


def process_section(
  image_list: List[ImageProps], 
  defines: Dict[str, Any],
  section: ConfigSection) -> None:
  name = section.get('Name', '')
  path = section.get('path')
  if not name:
    name = os.path.splitext(os.path.split(path)[1])[0].upper()
    name = name + "_IMG"
    first_char = name[0]
    if first_char < 'A' or first_char > 'Z':
      name = 'I' + name

  with Image.open(path) as img:
    tile_x = section.get('tile_x', 0)
    tile_y = section.get('tile_y', 0)

    if not tile_x and not tile_y:
      image_list.append(ImageProps(
          name=name,
          invert=section.get('invert', False),
          image=img.copy(),
          rle=[]
      ))
    else:
      add_image_as_tiles(
        image_list,
        defines,
        img,
        name,
        section.get('invert', False),
        tile_x,
        tile_y)


def generate_offsets(fout: IO, image_list: List[ImageProps]) -> None:
  fout.write('    // Image Offsets\n')
  # 2 bytes width, 2 bytes height, 4 bytes offset
  offset = len(image_list) * 8
  index = 0;
  for i in image_list:
    line = ''.join((
      '    ',
      '%s, ' % u16_to_hexstr(i.image.width),
      '%s, ' % u16_to_hexstr(i.image.height),
      '%s, ' % u32_to_hexstr(offset),
      f'// {index}: {i.name} w={i.image.width}, h={i.image.height}, off={offset}\n'
    ))
    fout.write(line)
    offset += len(i.rle)
    index += 1


def u16_to_hexstr(val: int) -> str:
  return "0x%02X, 0x%02X" % (
    val & 0xFF,
    (val >> 8) & 0xFF,
  )

def u32_to_hexstr(val: int) -> str:
  return "0x%02X, 0x%02X, 0x%02X, 0x%02X" % (
    val & 0xFF,
    (val >> 8) & 0xFF,
    (val >> 16) & 0xFF,
    (val >> 24) & 0xFF
  )


def scale_image_if_needed(
  img: Image.Image, max_image_comment_size: int) -> Image.Image:
  if (img.width <= max_image_comment_size and
      img.height <= max_image_comment_size):
      return img

  if img.width >= img.height:
    # make the width the larger dimension
    new_width = 80
    new_height = img.height * 80 // img.width
  else:
    new_width = img.width * 80 // img.height
    new_height = 80
  if new_width < 1:
    new_width = 1
  if new_height < 1:
    new_height = 1

  return img.resize((new_width, new_height))


def generate_image_comment(
  index: int,
  i: ImageProps,
  max_image_comment_size: int,
  fout: IO
) -> None:
  """Generates a comment block for an image."""
  fout.write(f'    // Image {index}: {i.name} w={i.image.width} h={i.image.height}\n')
  img = scale_image_if_needed(i.image, max_image_comment_size)
  for y in range(img.height):
    fout.write('    // ')
    for x in range(img.width):
      fout.write('#' if bool(img.getpixel((x,y)) == i.invert) else '-')
    fout.write('\n')


def dump_sources(
  path: str,
  image_list: List[ImageProps],
  defines: Dict[str, Any],
  max_image_comment_size: int) -> None:
  out_path = pathlib.Path(path).with_suffix('.c')
  var_name = out_path.with_suffix('').name.replace('.', '_').replace('-', '_')
  define_list = []
  if defines:
    for k, v in sorted(defines.items()):
      define_list.append((k, v))
  define_list.extend((i.name, idx) for idx, i in enumerate(image_list))
  codegen.dump_c_header(path, var_name, define_list)
  
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
        '    %s, // image count (%d)' % (u32_to_hexstr(len(image_list)), len(image_list)),
        ''
    )))

    for i in image_list:
      i.rle = rle.create_rle_data(i.image.height, i.image, i.invert)
    generate_offsets(fout, image_list)

    fout.write('    // Image data\n')
    index = 0
    for i in image_list:
      generate_image_comment(index, i, max_image_comment_size, fout)
      codegen.generate_character_data(i.rle, fout)

    fout.write('};\n')

  print('Wrote %s' % out_path)


def main():
  """Entry point."""
  if len(sys.argv) != 2:
    sys.exit('Usage: make_images.py <config.yaml>')

  path = sys.argv[1]
  with open(path, encoding='utf8') as f:
    cfg = yaml.safe_load(f)

  max_image_comment_size = cfg.get('max_image_comment_size', 80)

  # creates a list of (name, image) tuples
  config_sections = (ConfigSection(section) for section in cfg['images'])
  image_list = []
  defines = {}
  for section in config_sections:
    process_section(image_list, defines, section)
  dump_sources(path, image_list, defines, max_image_comment_size)


if __name__ == '__main__':
  main()
