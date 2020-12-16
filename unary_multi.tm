; identify all strings in the form of "11x111=111111"

; the finite set of states
#Q = {q0,q1,q2,q2a,q3,q3a,mh,begin,round,over,return,accept,accept2,accept_print,accept_print2,accept_print3,accept_print4,halt_accept,reject,reject2,reject_print,reject_print2,reject_print3,reject_print4,reject_print5,halt_reject}

; the finite set of input symbols
#S = {1,x,=}

; the complete set of tape symbols
#G = {1,x,=,_,t,r,u,e,f,a,l,s}

; the start state
#q0 = q0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 3

; the transition functions

; state q0 : start state
q0 ___ ___ *** reject ; empty input
q0 x__ x__ *** reject ; start with "x"
q0 =__ =__ *** reject ; start with "="
q0 1__ 1__ *** q1

; state q1 : get first substring
q1 1__ 1__ r** q1
q1 x__ x__ r** q2
q1 =__ =__ *** reject ; cannot find "x"
q1 ___ ___ l** reject ; cannot find "x"

; state q2 : get second substring
q2 1__ 11_ rr* q2a
q2 =__ =__ *** reject
q2 x__ x__ *** reject ; another "x"
q2 ___ ___ l** reject ; 2nd substring is empty
q2a 1__ 11_ rr* q2a
q2a =__ =__ r** q3
q2a x__ x__ *** reject
q2a ___ ___ l** reject

; state q3 : get third substring
q3 1__ 1_1 r*r q3a
q3 =__ =__ *** reject
q3 x__ x__ *** reject
q3 ___ ___ l** reject
q3a 1__ 1_1 r*r q3a
q3a ___ ___ lll mh
q3a =__ =__ *** reject
q3a x__ x__ *** reject

; state mh : move all three tape head to first symbol
mh 111 111 l** mh ; move tape[0]
mh =11 =11 l** mh
mh x11 x11 l** mh
mh _11 _11 *l* mh ; move tape[1]
mh __1 __1 **l mh ; move tape[2]
mh ___ ___ rrr begin

; state begin : begin to calculate
begin x11 x11 *** reject
begin 11_ 11_ *** reject
begin x1_ x1_ *** accept
begin 111 111 *** round ; start next round, keep tape[0] head still

round 111 11_ *rr round
round 1_1 1_1 *** over ; full round, then over
round 1__ 1__ *** over ; full round, then over
round 11_ 11_ *** reject ; not full round, so reject

over 1_1 1_1 rl* return
over 1__ 1__ rl* return

return 111 111 *l* return
return 11_ 11_ *l* return
return x11 x11 *l* return
return x1_ x1_ *l* return
return 1_1 1_1 *r* begin
return 1__ 1__ *r* begin
return x_1 x_1 *r* begin
return x__ x__ *r* begin

; state accept : clear tape[0] and print "true"

; accept ?1? ?__ l** accept
accept 111 1__ l** accept
accept x11 x__ l** accept
accept =11 =__ l** accept
accept 11_ 1__ l** accept
accept x1_ x__ l** accept
accept =1_ =__ l** accept

; accept ?__ ?__ l** accept ; keep move left
accept 1__ 1__ l** accept
accept x__ x__ l** accept
accept =__ =__ l** accept
accept ___ ___ r** accept2

; accept2 ?__ ___ r** accept2
accept2 1__ ___ r** accept2
accept2 x__ ___ r** accept2
accept2 =__ ___ r** accept2
accept2 ___ ___ *** accept_print

accept_print ___ t__ r** accept_print2
accept_print2 ___ r__ r** accept_print3
accept_print3 ___ u__ r** accept_print4
accept_print4 ___ e__ *** halt_accept

; state reject : clear tape[0] and print "false"

; reject ?1? ?__ l** reject
reject 111 1__ l** reject
reject x11 x__ l** reject
reject =11 =__ l** reject
reject 11_ 1__ l** reject
reject x1_ x__ l** reject
reject =1_ =__ l** reject

; reject ?__ ?__ l** reject ; keep move left
reject 1__ 1__ l** reject
reject x__ x__ l** reject
reject =__ =__ l** reject
reject ___ ___ r** reject2

; reject2 ?__ ___ r** reject2
reject2 1__ ___ r** reject2
reject2 x__ ___ r** reject2
reject2 =__ ___ r** reject2
reject2 ___ ___ *** reject_print

reject_print ___ f__ r** reject_print2
reject_print2 ___ a__ r** reject_print3
reject_print3 ___ l__ r** reject_print4
reject_print4 ___ s__ r** reject_print5
reject_print5 ___ e__ *** halt_reject
