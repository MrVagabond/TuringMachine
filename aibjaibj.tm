; design a turing machine that identifies strings in th form of a^ib^ja^ib^j

; state state
#Q = {q0,q1,q2,cp,mh,cmp,accept,accept2,accept3,accept4,reject_left,reject_right,reject,reject2,reject3,reject4,reject5,halt_accept,halt_reject}

; input symbols
#S = {a,b}

; tape symbols
#G = {a,b,_,t,r,u,e,f,a,l,s}

; start state
#q0 = q0

; blank symbols
#B = _

; set of final state
#F = {halt_accept}

; number of tapes
#N = 2

; transition functions

; state q0 : start state
q0 __ __ ** reject ; empty input
q0 a_ a_ ** q1
q0 b_ __ r* reject_right

; state q1 : go through a^i
q1 a_ a_ r* q1
q1 b_ b_ ** q2
q1 __ __ l* reject_left

; state q2 : go through b^j
q2 b_ b_ r* q2
q2 a_ a_ ** cp
q2 __ __ l* reject_left

; state cp : copy the second part to 2nd tape
cp a_ _a rr cp
cp b_ _b rr cp
cp __ __ ll mh

; state mh : move tape head to the first symbol
mh _a _a ll mh
mh _b _b ll mh
mh b_ b_ l* mh
mh a_ a_ l* mh
mh __ __ rr cmp

; state cmp : compare two tapes
cmp aa __ rr cmp
cmp bb __ rr cmp
cmp __ __ ** accept
cmp a_ __ r* reject_right
cmp b_ __ r* reject_right
cmp ab __ rr reject_right
cmp ba __ rr reject_right
cmp _a __ *r reject_right
cmp _b __ *r reject_right

; state accept : accept and print "true"
accept __ t_ r* accept2
accept2 __ r_ r* accept3
accept3 __ u_ r* accept4
accept4 __ e_ ** halt_accept

; state reject_left : reject and there are symbols to remove on the left
reject_left __ __ ** reject
reject_left a_ __ l* reject_left
reject_left b_ __ l* reject_left
reject_left _a __ *l reject_left
reject_left _b __ *l reject_left
reject_left aa __ ll reject_left
reject_left bb __ ll reject_left
reject_left ab __ ll reject_left
reject_left ba __ ll reject_left

; state reject_right : reject and there are symbols to remove on the right
reject_right __ __ ** reject
reject_right a_ __ r* reject_right
reject_right b_ __ r* reject_right
reject_right _a __ *r reject_right
reject_right _b __ *r reject_right
reject_right aa __ rr reject_right
reject_right bb __ rr reject_right
reject_right ab __ rr reject_right
reject_right ba __ rr reject_right

; state reject : reject and print "false"
reject __ f_ r* reject2
reject2 __ a_ r* reject3
reject3 __ l_ r* reject4
reject4 __ s_ r* reject5
reject5 __ e_ ** halt_reject