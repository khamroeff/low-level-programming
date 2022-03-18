NAME := image-transformer
BUILDDIR = build
SOLUTION_DIR = solution
TESTDIR = tests
OBJDIR = obj/$(SOLUTION_DIR)

CC = clang
LINKER = $(CC)
NASM = nasm
NASMFLAGS = -felf64 -g
RM = rm -rf

OBJDIR = obj/$(SOLUTION_DIR)

CFLAGS += $(strip $(file < $(SOLUTION_DIR)/compile_flags.txt)) -I$(SOLUTION_DIR)/include 

all: $(OBJDIR)/filter_sepia_asm.o $(OBJDIR)/bmp.o $(OBJDIR)/image.o $(OBJDIR)/filter_sepia.o $(OBJDIR)/rotation.o $(OBJDIR)/main.o
	$(CC) -o $(BUILDDIR)/image-transformer $^

build:
	mkdir -p $(BUILDDIR)

$(OBJDIR)/image.o: $(SOLUTION_DIR)/src/image.c build
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/rotation.o: $(SOLUTION_DIR)/src/rotation.c build
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/bmp.o: $(SOLUTION_DIR)/src/bmp.c build
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/filter_sepia_asm.o: $(SOLUTION_DIR)/src/filter_sepia_asm.asm build
	$(NASM) $(NASMFLAGS) $< -o $@

$(OBJDIR)/filter_sepia.o: $(SOLUTION_DIR)/src/filter_sepia.c build
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJDIR)/main.o: $(SOLUTION_DIR)/src/main.c build
	$(CC) -c $(CFLAGS) $< -o $@


obj/$(TESTDIR)/tests.o: $(TESTDIR)/src/tests.c build
	$(CC) -c $(CFLAGS) $< -o $@

obj/$(TESTDIR)/main.o: $(TESTDIR)/src/main.c build
	$(CC) -c $(CFLAGS) $< -o $@

test: obj/$(TESTDIR)/main.o obj/$(TESTDIR)/tests.o $(OBJDIR)/filter_sepia_asm.o $(OBJDIR)/image.o $(OBJDIR)/filter_sepia.o $(OBJDIR)/bmp.o
	 $(CC) -o $(BUILDDIR)/image-tests $^
	./$(BUILDDIR)/image-tests $(TESTDIR)/1.bmp $(TESTDIR)/c_1.bmp $(TESTDIR)/asm_1.bmp

clean:
	$(RM) $(OBJDIR) $(BUILDDIR)
