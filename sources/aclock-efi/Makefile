# Try to guess host machine
ARCH            = $(shell $(CC) -dumpmachine | cut -f1 -d- | sed s,i[3456789]86,ia32,)

# Override amd64
ifeq ($(ARCH),amd64)
	override ARCH := x86_64
endif

LIB = /usr/lib

ifeq ($(ARCH),amd64)
	override LIB := /usr/lib64
endif

ifeq ($(ARCH),aarch64)
	override LIB := /usr/lib64
endif

ifeq ($(ARCH),ia64)
	CFLAGS += -mfixed-range=f32-f127
endif

ifeq ($(ARCH),ia32)
	CFLAGS += -mno-mmx -mno-sse
endif

ifeq ($(ARCH),x86_64)
	CFLAGS += -mno-red-zone -DEFI_FUNCTION_WRAPPER
endif

ifneq ($(ARCH),aarch64)
ifneq ($(ARCH),arm)
	export HAVE_EFI_OBJCOPY=y
endif
endif

ifeq ($(ARCH),arm)
	CFLAGS += -marm
endif

OBJS            = Aclock.o
TARGET          = aclock-efi-$(ARCH).efi

EFIINC          = /usr/include/efi
EFIINCS         = -I$(EFIINC) -I$(EFIINC)/$(ARCH) -I$(EFIINC)/protocol
EFILIB          = $(LIB)
EFI_CRT_OBJS    = $(EFILIB)/crt0-efi-$(ARCH).o
EFI_LDS         = $(EFILIB)/elf_$(ARCH)_efi.lds

CFLAGS          += $(EFIINCS) -DGNU_EFI -O2 -fpic -Wall -fshort-wchar -fno-strict-aliasing \
			-fno-merge-constants -ffreestanding -fno-stack-protector

LDFLAGS         = -nostdlib --no-undefined --build-id=sha1 -T $(EFI_LDS) -shared \
			-Bsymbolic -L $(EFILIB) -L $(LIB) $(EFI_CRT_OBJS) 

ifneq ($(HAVE_EFI_OBJCOPY),)
FORMAT		:= --target=efi-app-$(ARCH)
else
SUBSYSTEM	:= 0xa
FORMAT		:= -O binary
LDFLAGS		+= --defsym=EFI_SUBSYSTEM=$(SUBSYSTEM)
endif

all: $(TARGET)

aclock-efi-$(ARCH).so: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@ -lefi -lgnuefi

%.efi: %.so
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel \
		-j .rela -j .rel.* -j .rela.* -j .rel* -j .rela* \
		-j .reloc $(FORMAT) $^ binaries/$@
