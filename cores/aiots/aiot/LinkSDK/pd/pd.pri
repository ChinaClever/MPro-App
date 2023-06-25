#INCLUDEPATH += $$PWD


HEADERS += \
    $$PWD/frameworks/sdk_init.h \
    $$PWD/pals/pal_auth.h \
    $$PWD/pals/pal_mqtt.h \
    $$PWD/pals/pal_ota.h \
    $$PWD/pals/pal_prop_post_api.h \
    $$PWD/pals/pal_prop_set_api.h \
    $$PWD/pals/pal_service_api.h \
    $$PWD/pals/sdk_params.h


SOURCES += \
    $$PWD/frameworks/sdk_init.c \
    $$PWD/pals/pal_auth_callback_impl.c \
    $$PWD/pals/pal_mqtt_callback_impl.c \
    $$PWD/pals/pal_ota_callback_impl.c \
    $$PWD/pals/pal_prop_post_api.c \
    $$PWD/pals/pal_prop_set_api.c \
    $$PWD/pals/pal_prop_set_invokes.c \
    $$PWD/pals/pal_service_call.c \
    $$PWD/pals/pal_user_topic_impl.c

