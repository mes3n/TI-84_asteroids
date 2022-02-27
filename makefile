# ----------------------------
# Makefile Options
# ----------------------------

NAME = asteroid
ICON =
DESCRIPTION = "asteroids game by mes3n"
COMPRESSED = NO
ARCHIVED = NO # YES

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------

include $(shell cedev-config --makefile)
