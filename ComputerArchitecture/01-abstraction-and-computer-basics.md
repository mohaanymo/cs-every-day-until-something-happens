# Abstraction and Computer Basics

Cover:
- Levels of transformation and abstraction (why cross the layers)
- What a computer is (3 components)
- Moore's Law

# 1- Levels of transformation and abstraction

Abstraction: hiding complicated details and showing only what you need.
Transformation: converting something from one form to another so the computer can understand and execute it.

computer doesn't understand what you write with programming language, at it's core it's just electricity represented as 0s and 1s.

## the layers:

**1. High-level language**

# **C / Python**

What programmers write

**2. Assembly language**

# **MOV, ADD, SUB**

Human-readable CPU instructions

Assembler transforms instructions

**3. Machine language**

# **10110000 01100001**

Binary instructions the CPU executes

**4. Hardware**

# **Transistors & circuits**

Physical electronic components


## Why would we have multiple layers?

1- programmers don't want to write thousands of binary instructions.

2- compiler developers don't want to manually control every transistor.

3- hardware engineers don't need to understand the entire application the programmer tends to write.

So, each layer hides complexity and the important idea is you work at one layer without needing to know every detail on the layer below it.

## Then why should we learn the lower layers?

Because sometimes you need to understand what's happening underneath.

For example:

- Why is this program slow?
    
- Why does this instruction take more time than another?
    
- Why does memory access affect performance?
    
- How does the CPU actually execute my code?

this makes you have more control over the machine cause you understand every part of it.


# 2- what is a computer

we can simply describe the computer with these three main components 

1- CPU: executes instructions and performs calculations.
2- Memory: stores data and instructions.
3- Input/Output: communicates with the outside world.

we can simply for now describe the process like that

Input (like keyboard or mouse) => Memory (stores the data) => CPU (preforms the computation) => Output (like displaying results on the monitor)

we will discuss these things in details later.


# 3- Moore's Law



Moore's Law is an observation made by Gordon Moore in 1965.

![[Gordon_Moore.png]]

It states that:

**The number of transistors on an integrated circuit tends to double approximately every two years.**

But what does that actually mean?

First, what's a transistor?

A transistor is a tiny electronic switch inside a computer, it can represent two basic states

ON (1)
OFF (0)

and those 0s and 1s are just electricity,
computers use billions of these tiny switches to perform calculations and process information.