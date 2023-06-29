#ifndef _PAL_OTA_H_
#define _PAL_OTA_H_

#if defined(__cplusplus)
extern "C" {
#endif

int32_t pal_ota_alloc_storage();

int32_t pal_ota_write(unsigned char *buffer, uint32_t len);

int32_t pal_ota_finish();

#if defined(__cplusplus)
}
#endif

#endif /* #ifndef _PAL_OTA_H_ */