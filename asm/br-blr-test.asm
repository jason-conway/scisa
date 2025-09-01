.text
main:
    ; Test br instruction
    ; First, load a function address into register
    lea r0, (branch_target)

    ; Print message before branch
    msg 'Before br: About to branch to address in r0\n'

    ; Branch to the address in r0
    br (r0)

    ; This line should be skipped due to the branch
    msg 'This should not be printed!\n'

after_branch:
    ; Now test blr instruction
    ; Load function address into register
    lea r1, (call_target)

    ; Print message before call
    msg 'Before blr: About to call function at address in r1\n'

    ; Call the function at address in r1
    blr (r1)

    ; This line should be executed after returning from the call
    msg 'After blr: Returned from function call\n'
    halt

branch_target:
    ; Print message to show we reached branch target
    msg 'Successfully reached branch target\n'

    ; Jump back to continue tests
    b after_branch

call_target:
    ; Print message to show we reached call target
    msg 'Inside call_target function\n'

    ; Test if lr is correctly set
    msg 'lr value inside function: ', lr, '\n'

    ; Return using lr
    ret
