(define true-objs '(#t 0 1 "" "str" 'sym ()))
(define false-objs '(#f))


(add-test "boolean-parse-test" (lambda ()
	(map (lambda (x) (assert x "unexpected false")) true-objs)
	(map (lambda (x) (assert (not x) "unexpected true")) false-objs)
))

(add-test "cond-test" (lambda ()
	;; Boolean
	(cond
		(#f (error "This shouldn't be evaluated"))
		(#t ())
	)
	;; Numbers
	(cond
		(0 ())
		(#t (error "Zero should evaluate to true"))
	)
	(cond
		(1 ())
		(#t (error "Number should work"))
	)
	;; Strings 
	(cond
		("" ())
		(#t (error "Empty string should evaluate to true"))
	)
	(cond
		("str" ())
		(#t (error "Any string should evaluate to true"))
	)
))

(add-test "and-test" (lambda ()
    ;; All args are true
	(define should-eval-to-true
		'(
			() 
			(#t) (1) 
			(#t #t)	(#t 1) (1 #t) (1 1)
			(#t #t #t) (#t #t 1) (#t 1 #t) (#t 1 1) 
			(1 #t #t)  (1 #t 1)  (1 1 #t)  (1 1 1) 
		))
	(define (and-check-true args)
		(define call-with-args (cons 'and args))
		(assert (eval call-with-args) "Unexpected false")
	)
	(map and-check-true should-eval-to-true)
	;; One of args is false
	(define should-eval-to-false
		'(
			(#f)
			(#f #t)	(#f 1) (1 #f)
			(#t #f #f) (#t #f 1) (#f 1 #t)
			(1 #f #f)  (1 #f 1)  (1 1 #f)
		))
	(define (and-check-false args)
		(define call-with-args (cons 'and args))
		(assert (not (eval call-with-args)) "Unexpected true")
	)
	(map and-check-false should-eval-to-false)
))

(add-test "or-test" (lambda ()
    ;; One arg is true
	(define should-eval-to-true
		'( 
			(#t) (1) 
			(#t #f)	(#f 1) (1 #f) (1 1)
			(#t #f #f) (#t #f 1) (#f 1 #f) (#t 1 1) 
		))
	(define (or-check-true args)
		(define call-with-args (cons 'or args))
		(assert (eval call-with-args) "Unexpected false")
	)
	(map or-check-true should-eval-to-true)
	;; All args are false
	(define should-eval-to-false '((#f) (#f #f) (#f #f #f) ))
	(define (or-check-false args)
		(define call-with-args (cons 'or args))
		(assert (not (eval call-with-args)) "Unexpected true")
	)
	(map or-check-false should-eval-to-false)
))

(run-all-tests)