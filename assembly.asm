;A sample program that makes the bot go to the position defined at the top.
.define TARGET_X 320
.define TARGET_Y 240

.define HWID_MOVEMENT 0x2
.define INT_HWQ 0x1
.define INT_HWI 0x2
.define MOVE_MOVE 0x0
.define MOVE_ROT_CW 0x1
.define MOVE_ROT_CCW 0x2
.define MOVE_QUERY 0x3

;First, we find the movement hardware and store it in [movementslot].
mov A, HWID_MOVEMENT
call findhardware

;Next, we query our position.
call posquery

;See what direction we need to go in
cmp [x], TARGET_X
jge go_west

;Set dir to 0 (for rotating back later), rotate, and set [togo] to the distance to travel.
go_east:
mov [dir], 0
mov B, TARGET_X
sub B, [x]
mov [togo], B
mov B, MOVE_ROT_CW
int INT_HWI
jmp x_go

;Set dir to 1 (for rotating back later), rotate, and set [togo] to the distance to travel.
go_west:
mov [dir], 1
mov B, [x]
sub B, TARGET_X
mov [togo], B
mov B, MOVE_ROT_CCW
int INT_HWI

x_go:
;Go to the target X
mov B, MOVE_MOVE

go_x_loop:
;See if we have nowhere to go and if so, jmp out of the loop.
cmp [togo], 0
jz go_x_done
;Else, move and sub 1 from [togo].
sub [togo], 1
int INT_HWI
jmp go_x_loop

go_x_done:
;We're done. Rotate north first to make the Y rotation easier.
cmp [dir], 0
jnz rot_done_west

rot_done_east:
mov B, MOVE_ROT_CCW
jmp rot_done_do

rot_done_west:
mov B, MOVE_ROT_CW

rot_done_do:
int INT_HWI


;Do the same thing, but for Y.

;See what direction we need to go in
cmp [y], TARGET_Y
jge go_south

;Set [togo] to the distance to travel.
go_north:
mov B, TARGET_Y
sub B, [y]
mov [togo], B
jmp y_go

;Rotate south and set [togo] to the distance to travel.
go_south:
mov B, [y]
sub B, TARGET_Y
mov [togo], B
mov B, MOVE_ROT_CW
int INT_HWI
int INT_HWI

y_go:
;Go to the target Y
mov B, MOVE_MOVE

go_y_loop:
;See if we have nowhere to go and if so, jmp out of the loop.
cmp [togo], 0
jz go_y_done
;Else, move and sub 1 from [togo].
sub [togo], 1
int INT_HWI
jmp go_y_loop

go_y_done:

;We're done! Now we'll just store our x,y,rot in A,B,C just to verify.
mov B, MOVE_QUERY
int INT_HWI
pop C
pop B
pop A

hlt


;Find first hardware with ID stored in A and store the slot in A
findhardware:
    ;Push registers to save
    push B
    push C
    push D
    mov [hwid_looking], A
    
    ;Hardware Query
    int 0x1
    ;Start with 0xFFFE because when 2 is added to it, it will be 0x0
    mov C, 0xFFFE
    
    fhloop:
        add C, 2
        mov D, [C]
        mov B, D
        ;And D with 0xFF to get the hardware ID
        and D, 0x00FF
        ;Compare with A (the HWID we want to find)
        cmp D, [hwid_looking]
        ;If not the right one, jmp to loop to keep searching
        jnz fhloop

    ;We found it! Since we copied the hardware descriptor to B, we shift it 8 bits right to get the slot.
    shr B, 8
    mov A, B
    
    ;Pop registers back
    pop D
    pop C
    pop B
    
    ret
    
    ;vars for findhardware
    hwid_looking:
        .dw 0x0
    
    
;Queries position and stores it in the vars.
posquery:
    push B
    push A
    
    ;We'll query our position. It's pushed to the stack in the order of x,y,rot.
    ;rot is 0 for north, 1 for east, and so on.
    mov B, MOVE_QUERY
    int INT_HWI
    
    pop [rot] ;rot
    pop [y] ;y
    pop [x] ;x
    
    pop A
    pop B
    ret

;Data storage
x:
    .dw 0x0
y:
    .dw 0x0
rot:
    .dw 0x0
togo:
    .dw 0x0
dir:
    .dw 0x0
