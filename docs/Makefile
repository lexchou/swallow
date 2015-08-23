MARKDOWNS=$(shell find . -name 'README.md')
DOTS=$(shell find . -name '*.dot')
PNGS=$(patsubst %.dot,%.png,$(DOTS))
HTMLS := $(patsubst %/README.md,%/index.html,$(MARKDOWNS))
PANDOC=pandoc --from markdown_github --to html --standalone -c ../../markdown.css

all: $(HTMLS) $(PNGS)

zh-cn/%/index.html: zh-cn/%/README.md
	$(PANDOC) $< -o $@

en/%/index.html: en/%/README.md
	$(PANDOC) $< -o $@

index.html: README.md
	$(PANDOC) $< -o $@

images/%.png: images/%.dot
	dot -Tpng $< -o $@



clean:
	rm -f $(HTMLS) $(PNGS)
