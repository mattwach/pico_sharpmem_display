
"""Contains logic that translates a list of integers to a run-length-encoded format."""

from typing import List
from PIL import Image

def create_rle_data(height: int, img: Image.Image, invert: bool = True) -> List[int]:
  data = []
  cols = (img.width + 7) // 8
  for col in range(cols):
    for y in range(height):
      startx = col * 8
      endx = min(startx + 8, img.width)
      byte = 0x00
      bit = 7
      for x in range(startx, endx):
        if bool(img.getpixel((x, y))) == invert:
          byte = byte | 1 << bit
        bit -= 1
      data.append(byte)

  return run_length_encode(data)


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
