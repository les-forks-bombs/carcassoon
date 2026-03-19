SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))

$(PROG): $(SRCS)
	@$(CC) $^ $(CFLAGS) $(LIBS) -o $@
	@echo "    BIN   $(notdir $(PROG))"

build: $(PROG)

.PHONY: clean
clean:
	@$(RM) -f $(OBJS) $(PROG)
	@$(RM) -rf $(PROG){,.exe}
	@echo "    CL    $(notdir $(OBJS))"
	@echo "    CL    $(notdir $(PROG))"