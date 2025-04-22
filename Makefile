SUBDIRS = mySimpleComputer myTerm myBigChars myReadKey console

all: $(SUBDIRS)

$(SUBDIRS):
	@$(MAKE) -C $@

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

format:
	find . -type f -name *.[ch] | xargs clang-format --style GNU -i --verbose

.PHONY: all clean $(SUBDIRS) format
