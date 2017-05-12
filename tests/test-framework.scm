(define fail-count 0)
(define pass-count 0)
(define test-list '())
(define failed-tests-list '())

(define (handle-error e)
	(print "Failed!")
	(print (car e))
	(set! fail-count (+ fail-count 1))
)

(define (add-test test-name test-func)
	(define test-pair (list test-name test-func))
	(print test-pair)
	(set! test-list (cons test-pair test-list))
)

(define (run-test test-pair)
	(define test-name (car test-pair))
	(define test-fn (car (cdr test-pair)))
	(with-exception-handler
		handle-error
		(lambda () (test-fn))
	)
)

(define (assert condition message)
	(cond
		(condition ())
		(#f (error message))
	)
)

(define (run-all-tests)
	(map 
		(lambda (test-pair)
			(print "Running" (car test-pair) "...")
			(run-test test-pair)
			(print "Ok")
		)
		test-list
	)
	(cond
		((= 0 fail-count) (exit 0) )
		(#t               (exit -1))
	)
)
