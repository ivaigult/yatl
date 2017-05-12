(add-test "boolean-parse-test" (lambda ()
	(assert #t "True is true")
	(assert 0 "Any number is true")
	(assert 1 "Any number is true")
	(assert 2 "Any number is true")
	;; (assert "" "Any string is true") FIXME!!!!!!
	(assert "str" "Any string is true")
	(assert (not #f) "False is false")
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
	;; (cond !FIXME
	;; 	("" ())
	;; 	(#t (error "Empty string should evaluate to true"))
	;; )
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
		(print call-with-args)
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
		(print call-with-args)
		(assert (not (eval call-with-args)) "Unexpected true")
	)
	(map and-check-false should-eval-to-false)
))

(run-all-tests)