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
    bi->error = TEXT_BAD_IMAGE_ID_ERROR;
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

  return (ImageInfo*)(&bi->data + (id * sizeof(ImageInfo)));
}

void image_init(struct BitmapImages* bi, const void* image_data, struct Bitmap* bitmap) {
    bi->image_data = images;
    bi->bitmap = bitmap;
    bi->error = 0;
    if (sizeof(ImageInfo) != 8) {
        bi->error = IMAGE_UNEXPECTED_HEADER_SIZE;
        return;
    }
    verify_image_data(image_data, &(bi->error));
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
  struct ImageInfo* ii = image_info(images, id);
  if (!ii) {
    return;
  }

  const struct SharpMemoryImage* img_data = (struct SharpMemoryImage*)bi->images;
  const uint16_t num_cols = (ii->width + 7) >> 3;
  const uint16_t height = ii->height;
  uint8_t* error = &(bi->error);

  map_rle_image(
    bi->bitmap,
    &img_data->data + ii->offset;
    x,
    y,
    width,
    height,
    error);
}