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


(run-all-tests)

