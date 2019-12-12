.data
x: .quad 10
.global f
f:
PUSHQ %rbp
MOVQ %rsp, %rbp
PUSHQ %rbx
PUSHQ %r12
PUSHQ %r13
PUSHQ %r14
PUSHQ %r15
MOVQ $2, %rbx
MOVQ %rbx, %rax
POPQ %r15
POPQ %r14
POPQ %r13
POPQ %r12
POPQ %rbx
MOVQ %rbp, %rsp
POPQ %rbp
RET

.global main
main:
PUSHQ %rbp
MOVQ %rsp, %rbp
PUSHQ %rbx
PUSHQ %r12
PUSHQ %r13
PUSHQ %r14
PUSHQ %r15
MOVQ x, %rbx
MOVQ $1, %r10
ADDQ %rbx, %r10
CALL f
MOV %rax, %rbx
SUBQ %r10, %rbx
MOVQ %rbx, %rax
POPQ %r15
POPQ %r14
POPQ %r13
POPQ %r12
POPQ %rbx
MOVQ %rbp, %rsp
POPQ %rbp
RET

