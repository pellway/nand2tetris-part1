// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

@0          // Get value 0
D=A         // Set D register to 0
@R2         // Get RAM[2]
M=D         // Set RAM[2] to 0

@R1         // Get RAM[1]
D=M         // Set D register to RAM[1]
M=D-1       // RAM[1] = RAM[1] - 1 (for index purposes in the loop)

@R0         // Get RAM[0]
D=M         // Set D register to RAM[0]
@RAM0       // Declare RAM0 Variable
M=D         // RAM0 = R0

@START      
D;JGT       // Jump to start if D > 0

@STOP       // If R0 == 0, set R2 = 0 and end program
0;JMP

(START)     // Start the loop

// Loop RAM[1] number of times
@R1
D=M         // Set D register to RAM[1]
@STOP
D;JEQ       // If RAM[1] == 0, end the loop
@R1
M=D-1       // Decrement RAM[1]

// Perform D=D+RAM[0] each time in the loop
@RAM0
D=M
@R0
D=D+M
@RAM0
M=D         //RAM0 = D + RAM[0]

@START      // Goto the start of the loop
0;JMP

(STOP)
@RAM0
D=M
@R2
M=D         // Set RAM[2] = (RAM0 + RAM0) RAM1 amount of times

(END)
@END
0;JMP



