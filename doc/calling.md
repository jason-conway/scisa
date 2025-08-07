# Calling Convention

| reg       | name            | saver   | count |
| --------- | --------------- | ------- | ----- |
| `r0-r3`   | return values   | caller  | `4`   |
| `r4-r11`  | fn arguments    | caller  | `8`   |
| `r12-r23` | saved registers | callee  | `12`  |
| `r24-r31` | temporaries     | caller* | `8`   |

> *volatile across calls. Must be saved by the caller if used later.

## RetCC

The first 4 integer return values are passed in integer registers.

```tablegen
CCIfType<[i32], CCAssignToReg<[R0, R1, R2, R3]>>,
```

Integer values get stored in stack slots that are 4 bytes in size and 4-byte
aligned.

```tablegen
CCIfType<[i32], CCAssignToStack<4, 4>>
```

## CC

1-, 8-, and 16-bit arguments are promoted to i32.

```tablegen
CCIfType<[i1, i8, i16], CCPromoteToType<i32>>,
```

The first 8 integer arguments are passed in integer registers.

```tablegen
CCIfType<[i32], CCAssignToReg<[R4, R5, R6, R7, R8, R9, R10, R11]>>,
```

Integer values get stored in stack slots that are 4 bytes in size and 4-byte
aligned.

```tablegen
CCIfType<[i32], CCAssignToStack<4, 4>>
```
