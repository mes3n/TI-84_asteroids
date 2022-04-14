# ----------------------------
# Makefile Options
# ----------------------------

NAME = asteroid
ICON = icon.png
DESCRIPTION = "asteroids game by mes3n"
COMPRESSED = NO
ARCHIVED = NO # YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
