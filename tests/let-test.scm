(load-file "test-framework.scm")

(add-test "let-test" (lambda ()
    (let ((x 0) (y 1) (z 2))
        (assert (= x 0) "Unexpected x value")
        (assert (= y 1) "Unexpected y value")
        (assert (= z 2) "Unexpected z value")
    )
))

(define global-x 100500)

(add-test "let-overrides-globals" (lambda ()
    (let ((global-x 0))
        (assert (= global-x 0) "Unexpected x value")
    )
))

(add-test "let-overrides-locals" (lambda ()
    (let ((local-x 100500))
        (define local-x-copy local-x)
        (let ((local-x 0))
            (assert (= local-x 0) "Unexpected x value")
            (assert (= 100500 local-x-copy) "Copy value should remain unchanged")
        )
        (assert (= local-x 100500) "But value should be restored")
        (assert (= 100500 local-x-copy) "Copy value should remain unchanged")
    )
))


(define counter 0)
(define (bump-counter) (set! counter (+ counter 1)))

(add-test "fluid-let-test" (lambda ()
    (fluid-let ((counter 100))
        (bump-counter)
        (assert (= counter 101) "Counter was not bumped")
    )
    (assert (= counter 0) "counter was not restored")
))


(add-test "let-bindings-assigned-in-parallel" (lambda ()
    (define x 100500)
    (let ((x 0) (y x))
        (assert (= x 0) "Unexpected x value")
        (assert (= y 100500) "y should be assigned in parallel")
    )
))

(add-test "let*-test" (lambda ()
    (define x 100500)
    (let* ((x 0) (y x))
        (assert (= x 0) "Unexpected x value")
        (assert (= y 0) "y should be assigned sequentialy")
    )
))

(run-all-tests)

