plang is a intermediate language that can be converted into direct PLASM2 machine code, or an assembly instruction file.

The purpose of plang is to be a C-like language that will assist in writing a full C compiler.
plang will have strict code syntax, (ex. u32* Pointer is allowed, but u32 *Pointer is not), and then
C can be sanitized and washed down into plang.

Planned syntax:

Types:
byte: 1 Byte / 8 Bits
u16:  2 Bytes / 16 Bits
u32   4 Bytes / 32 Bits
u64   8 Bytes / 64 Bits
str   1 Byte / 8 Bits, as a single character.
void  ? Byte / ? Bits, a generic type.

Any type can be converted to a pointer with a '*' suffix of the type.

Function Definition:
Type Name (void);
Type Name (byte Test);

Variable Definition:
u32 Var;
u32* VarP;

Structures:
struct struct_t {
	u32 Variable : 32;
	u32* PVariable : 64;
};

Comments:
both // and /* */ comments will be supported.