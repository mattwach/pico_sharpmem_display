#include "sharpdisp/bitmapimage.h"
#include "rle.h"

struct ImageInfo {
    uint16_t width;
    uint16_t height;
    uint32_t offset;
};

static void verify_image_data(struct BitmapImages* bi) {
  if (bi->error) {
    return;
  }
  const struct SharpMemoryImage* img_data = (struct SharpMemoryImage*)bi->images;
  const uint8_t* id = img_data->id;
  if ((id[0] != 'S') ||
      (id[1] != 'H') || 
      (id[2] != 'I') ||
      (id[3] != '1')) {
    bi->error = IMAGE_BAD_ID_ERROR;
  }
}

static struct ImageInfo* image_info(struct BitmapImages* bi, uint32_t id) {
  if (bi->error) {
    return NULL;
  }
  const struct SharpMemoryImage* img_data = (struct SharpMemoryImage*)bi->images;
  if (id >= img_data->num_images) {
    bi->error = IMAGE_INVALID_ID;
    return NULL;
  }

  return (struct ImageInfo*)(img_data->data + (id * sizeof(struct ImageInfo)));
}

void image_init(struct BitmapImages* bi, const void* image_data, struct Bitmap* bitmap) {
    bi->images = image_data;
    bi->bitmap = bitmap;
    bi->error = 0;
    if (sizeof(struct ImageInfo) != 8) {
        bi->error = IMAGE_UNEXPECTED_HEADER_SIZE;
        return;
    }
    verify_image_data(bi);
}

uint32_t image_count(struct BitmapImages* bi) {
  const struct SharpMemoryImage* img_data = (struct SharpMemoryImage*)bi->images;
  return img_data->num_images;
}

uint16_t image_width(struct BitmapImages* images, uint32_t id) {
  struct ImageInfo* ii = image_info(images, id);
  if (!ii) {
    return 0;
  }
  return ii->width;
}

uint16_t image_height(struct BitmapImages* images, uint32_t id) {
  struct ImageInfo* ii = image_info(images, id);
  if (!ii) {
    return 0;
  }
  return ii->height;
}

void image_draw(struct BitmapImages* bi, uint32_t id, int16_t x, int16_t y) {
  struct ImageInfo* ii = image_info(bi, id);
  if (!ii) {
    return;
  }

  const struct SharpMemoryImage* img_data = (struct SharpMemoryImage*)bi->images;
  const uint16_t width = ii->width;
  const uint16_t height = ii->height;
  uint8_t* error = &(bi->error);

  map_rle_image(
    bi->bitmap,
    img_data->data + ii->offset,
    x,
    y,
    width,
    height,
    error);
}

void image_draw_tiled(
    struct BitmapImages* bi,
    uint32_t first_id,
    uint16_t columns,
    uint16_t rows,
    int16_t x,
    int16_t y) {
  // The first task is to figure out the starting and
  // ending columns
  const uint16_t tile_width = image_width(bi, first_id);
  const uint16_t min_column = (-x) >= tile_width ?
      (-x) / tile_width :
      0;
  int16_t last_x = x + (tile_width * (columns - 1));
  if (last_x > bi->bitmap->width) {
    last_x = bi->bitmap->width;
  }
  const uint16_t max_column = (last_x - x) / tile_width;

  // same calculations, but for rows
  const uint16_t tile_height = image_height(bi, first_id);
  const uint16_t min_row = (-y) >= tile_height ?
      (-y) / tile_height :
      0;
  int16_t last_y = y + (tile_height * (rows - 1));
  if (last_y > bi->bitmap->height) {
    last_y = bi->bitmap->height;
  }
  const uint16_t max_row = (last_y - y) / tile_height;

  // draw the tiles
  for (uint16_t row = min_row; row < max_row; ++row) {
    for (uint16_t column = min_column; column < max_column; ++column,++first_id) {
      image_draw(bi, first_id, x + (column * tile_width), y + (row * tile_height));
    }
  }
}

uint16_t image_width_tiled(
  struct BitmapImages* bi, uint32_t first_id, uint16_t columns) {
  const uint16_t tile_width = image_width(bi, first_id);
  if (columns <= 1) {
    return tile_width;
  }
  return (tile_width * (columns - 1)) + image_width(bi, first_id + columns - 1);
}

uint16_t image_height_tiled(
  struct BitmapImages* bi, uint32_t first_id, uint16_t columns, uint16_t rows) {
  const uint16_t tile_height = image_height(bi, first_id);
  if (rows <= 1) {
    return tile_height;
  }
  return (tile_height * (rows - 1)) + image_height(bi, first_id + (rows * columns) - 1);
}