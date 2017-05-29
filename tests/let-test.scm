(add-test "let-test" (lambda ()
    (let ((x 0) (y 1) (z 2))
        (assert (= x 0) "Unexpected x value")
        (assert (= y 1) "Unexpected y value")
        (assert (= z 2) "Unexpected z value")
    )
))

(run-all-tests)

