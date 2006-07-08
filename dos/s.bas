' simple Sierpenski
SCREEN 1

DIM a(2, 1), c(1), o(1)

a(0, 0) = 128: a(0, 1) = 0
a(1, 0) = 0: a(1, 1) = 199
a(2, 0) = 255: a(2, 1) = 199

c(0) = 128: c(1) = 100
o(0) = 128: o(1) = 100

WHILE 1
   x = INT(RND * 3)
   c(0) = (c(0) + a(x, 0)) / 2
   c(1) = (c(1) + a(x, 1)) / 2
   c(0) = INT(c(0))
   c(1) = INT(c(1))
   PSET (c(0), c(1)), 15
   'LINE (c(0), c(1))-(o(0), o(1)), 13
   'o(0) = c(0): o(1) = c(1)
WEND

