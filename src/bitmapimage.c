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