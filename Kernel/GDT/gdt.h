// TODO Remove this ugly types ...
typedef unsigned short u16int;
typedef unsigned int u32int;
typedef unsigned char u8int;

typedef short s16int;
typedef int s32int;
typedef char s8int;

// This structure contains the value of one GDT entry.
// We use the attribute 'packed' to tell GCC not to change
// any of the alignment in the structure.
struct gdt_entry_struct {
    u16int limit_low; // The lower 16 bits of the limit.
    u16int base_low; // The lower 16 bits of the base.
    u8int base_middle; // The next 8 bits of the base.
    u8int access; // Access flags, determine what ring this segment can be used in.
    u8int granularity;
    u8int base_high; // The last 8 bits of the base.
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

struct gdt_ptr_struct {
    u16int limit; // The upper 16 bits of all selector limits.
    u32int base; // The address of the first gdt_entry_t struct.
} __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

struct idt_entry_struct {
    u16int base_lo; // The lower 16 bits of the address to jump to when this interrupt fires.
    u16int sel; // Kernel segment selector.
    u8int always0; // This must always be zero.
    u8int flags; // More flags. See documentation.
    u16int base_hi; // The upper 16 bits of the address to jump to.
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct idt_ptr_struct {
    u16int limit;
    u32int base; // The address of the first element in our idt_entry_t array.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

// Initialisation function is publicly accessible.
void init_descriptor_tables();
