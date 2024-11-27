if(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
  target_sources(app PRIVATE led_control.c)
endif()