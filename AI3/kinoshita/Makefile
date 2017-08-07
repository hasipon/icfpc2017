.PHONY: all clean

all: ai.byte

ai.byte: ai.ml parser.mly lexer.mll json.ml
	ocamlbuild json.ml ai.byte

ai.native: ai.ml parser.mly lexer.mll json.ml
	ocamlbuild json.ml ai.native

clean:
	ocamlbuild -clean
