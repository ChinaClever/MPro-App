#ifndef _PAL_AUTH_H_
#define _PAL_AUTH_H_

#if defined(__cplusplus)
extern "C" {
#endif


char *pal_auth_get_product_key();

char *pal_auth_get_device_name();

char *pal_auth_get_device_secret();

char *pal_get_x509_private_key();

char *pal_get_x509_cert();

#if defined(__cplusplus)
}
#endif

#endif /* #ifndef _PAL_AUTH_H_ */
