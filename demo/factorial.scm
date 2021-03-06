;; Factorial is a clasical example for lisp interpreters

(define (factorial x)
	(cond
		((= x 0) 1) ;; !0 = 1
		(#t (* x (factorial (- x 1))))
	)
)

;; Main entrty point of this script
(define (main)
	(print (factorial 0))
	(print (factorial 1))
	(print (factorial 2))
	(print (factorial 3))
	(print (factorial 4))
	(print (factorial 5))
	(print (factorial 6))
	(print (factorial 7))
	(print (factorial 8))
	(print (factorial 9))
	(print (factorial 10))
)

;; 
(main)
