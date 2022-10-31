FONTS := $(wildcard *.yaml)
TARGETS := $(foreach F,$(FONTS),$(subst .yaml,.c,$F))

fonts: $(TARGETS)

%.c: %.yaml
	../tools/make_var_font.py $*.yaml
