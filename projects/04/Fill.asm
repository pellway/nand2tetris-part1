// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

(LOOP)

@SCREEN     // Get SCREEN start address
D=A
@R0         // Get RAM[0]
M=D         // Set RAM[0] = SCREEN start address

(KEEB)

@KBD        // Get RAM[24576] (Keyboard)
D=M         // Set D register to value of keyboard 
@BLACK    
D;JGT       // If keyboard not pressed, jump to white screen
@WHITE    
D;JEQ       // If keyboard not pressed, jump to white screen

@KEEB
0;JMP

// If keyboard is pressed
(BLACK)
@1
M=-1        // Fill screen bits with 1's
@DRAW    // Jump to end of loop to start filling
0;JMP

// If keyboard is not pressed
(WHITE)
@1
M=0         // Fill screen bits with 0's
@DRAW    // Jump to end of loop to start filling
0;JMP

(DRAW)

@1
D=M         // Set "drawing bit" as 0 or 1, depending on above

@R0
A=M         // Get address of screen pixel
M=D         // Fill using the "drawing bit"

@R0
D=M+1       // Goto next pixel
@KBD
D=A-D
@R0
M=M+1       // Goto next pixel
A=M
@DRAW
D;JGT       // Keep drawing until A=0

@LOOP
0;JMP