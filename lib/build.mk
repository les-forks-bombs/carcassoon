DIR_STACK := $(DIR) $(DIR_STACK)
DIR := $(DIR)/lib

include $(DIR)/libcarcassonne/build.mk
include $(DIR)/libai/build.mk
include $(DIR)/libutils/build.mk
include $(DIR)/libsdlrender/build.mk

include $(DIR)/carcassonne/build.mk

DIR := $(firstword $(DIR_STACK))
DIR_STACK := $(wordlist 2, $(words $(DIR_STACK)), $(DIR_STACK))
