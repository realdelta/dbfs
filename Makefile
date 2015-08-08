OUTPUT = out/main.o out/smush.o out/argparse.o

all: $(OUTPUT) out/dbfs

out:
	mkdir -p out

out/%.o: %.c out
	$(CC) -Wall -Werror -c -o$@ $<

out/dbfs: $(OUTPUT)
	$(CC) $^ -o$@

clean:
	rm -rf out 