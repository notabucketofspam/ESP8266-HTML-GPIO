# Edit following two lines to set component requirements (see docs)
set(COMPONENT_REQUIRES )
set(COMPONENT_PRIV_REQUIRES )

set(COMPONENT_SRCS "main.c" "html_gpio_core.c")
set(COMPONENT_ADD_INCLUDEDIRS ".")

# set(EMBED_FILES "index.html" "gpio_state")

register_component()
