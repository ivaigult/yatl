(add-test "let-test" (lambda ()
    (let ((x 0) (y 1) (z 2))
        (assert (= x 0) "Unexpected x value")
        (assert (= y 1) "Unexpected y value")
        (assert (= z 2) "Unexpected z value")
    )
))

(define global-x 100500)

(add-test "let-overrides-globals" (lambda ()
    (let ((x 0))
        (assert (= x 0) "Unexpected x value")
    )
))


(run-all-tests)

