OUTPUT = out/smush.o

all: $(OUTPUT) out/smush

out:
	mkdir -p out

out/%.o: %.c out
	$(CC) -c -o$@ $<

out/smush: $(OUTPUT)
	$(CC) $^ -o$@

clean:
	rm -rf out 