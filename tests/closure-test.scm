
(define (make_counter)
	(define counter 0)
	(define (get) counter)
	(define (increment) (set! counter (+ counter 1)) counter)

	(named-lambda (interface fn) ((eval fn)))
)

(add-test "simple-closure-test" (lambda ()
	(define counter (make_counter))
	(assert (= (counter 'get) 0)       "Initial value of counter is not null")
	(assert (= (counter 'increment) 1) "Increment did't increase the value")
	(assert (= (counter 'increment) 2) "Increment did't increase the value")
	(assert (= (counter 'get) 2)       "The result of two increments wasn't stored in closure")
))

(add-test "closure-instance-are-independent" (lambda ()
	(define counter-a (make_counter))
	(define counter-b (make_counter))
	(assert (= (counter-a 'get) 0)       "Initial value of counter is not null")
	(assert (= (counter-b 'get) 0)       "Initial value of counter is not null")
	(assert (= (counter-a 'increment) 1) "Increment did't increase the value")
	(assert (= (counter-b 'increment) 1) "Increment did't increase the value")
	(assert (= (counter-a 'increment) 2) "Increment did't increase the value")
	(assert (= (counter-b 'get) 1)       "Closures shouldn't share their state")
))

(run-all-tests)
