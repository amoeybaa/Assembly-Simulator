START 100
READ  A
READ  B
MOVER AREG, A
ADD   AREG, B
ADD   AREG, ='5'
LTORG
ADD   AREG, ="10"
MOVEM AREG, RESULT
PRINT RESULT
STOP
A     DS  1
B     DS  1
RESULT DS 1
END
