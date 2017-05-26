
(define (make_counter)
	(define counter 0)
	(define (get) counter)
	(define (increment) (set! counter (+ counter 1)) counter)

	(lambda (interface-fn) (interface-fn))
)


(add-test "simple-closure-test" (lambda ()
	(define counter (make_counter))
	(assert (= (counter 'get) 0)       "Initial value of counter is not null")
	(assert (= (counter 'increment) 1) "Increment did't increase the value")
	(assert (= (counter 'increment) 2) "Increment did't increase the value")
	(assert (= (counter 'get) 2)       "The result of two increments wasn't stored in closure")
))

(run-all-tests)
