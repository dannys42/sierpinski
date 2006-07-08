; This is a Sierpinski's Triangle generator with random colored pixels
; Copyright (C)1996-2006  Danny Sung <dannys@mail.com>
; 
; This program is free software; you can redistribute it and/or
; modify it under the terms of the GNU General Public License
; as published by the Free Software Foundation; either version 2
; of the License, or (at your option) any later version.
; 
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
; 
; You should have received a copy of the GNU General Public License
; along with this program; if not, write to the Free Software
; Foundation, Inc., 51 Franklin Street, Fifth Floor,
; Boston, MA 02110-1301, USA.
a
mov ax, 0013
int 10                        ; change to mode 0x13
mov bp, sp                    ; save the stack pointer
                              ; remember, bytes are reversed in Intel arch
mov word ptr [bp+00], 6480    ; (128,100)  - center
mov word ptr [bp+02], 0080    ; (128,0)  - top point
mov word ptr [bp+04], c700    ; (0, 199) - bottom left
mov word ptr [bp+06], c7ff    ; (255, 199) - bottom right
mov word ptr [bp+08], 0000   ; use this for a random number
;
mov ax, a000                  ; Set up pointer for direct memory access
mov es, ax
;
; Start of Loop here.
;
; Check if a key was pressed
mov dx, ds
xor ax, ax                    ; this takes one less byte than "mov ax, 0"
mov ds, ax
mov al, [41a]
mov ah, [41c]
mov ds, dx
cmp ah, al
jne 196                       ; go to exit code if key pressed
;
; Can't seem to get a half way decent random number generator
; So, let's just start reading from the stack, and assume the
; data we get is random enough.  (Seems to work.)
;
mov bx, word ptr [bp+08]
inc bx
cmp bx, ffff
jne 143
xor bx, bx
mov word ptr [bp+08], bx
mov dx, [bx]
and dx, 3
cmp dl, 0
je 125
;
; Set up point to draw - pick a point first
mov bx, dx
shl bx, 1
add bx, bp                    ; BX now contains index to coordinate vertex
;
mov ch, 0
mov ah, 0
mov al, byte ptr [bx]         ; Read in X coordinate of vertex
mov cl, byte ptr [bp]         ; Read in current X coordinate
add cx, ax                    ; add the two values...
shr cx, 1                     ; ... and divide by two
mov byte ptr [bp], cl         ; save new "current" X coordinate
;
mov di, cx                    ; X coordinate to draw
;
;
mov ch, 0
mov ah, 0
mov al, byte ptr [bx+1]       ; Read in Y coordinate
mov cl, byte ptr [bp+1]       ; Read in current Y coordinate
add cx, ax
shr cx, 1
mov byte ptr [bp+1], cl       ; save new "current" Y coordinate
;
mov ax, 140                   ; 140h = 320
mul cx                        ; hard code the horizontal width
add di, ax
;
; Draw the pixel
;
mov dx, di
and dx, 7f
mov ax, ds
mov cx, a000
mov ds, cx
mov bx, di
mov byte ptr [bx], dl
mov ds, ax
;
jmp 125
;
;
mov ax, 0003
int 10                        ; change to a decent text mode
xor ax, ax                    ; normal termination
int 21

n s_cool.com
r cx
9F
w
q
