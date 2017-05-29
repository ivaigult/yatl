(add-test "cons-test" (lambda ()
	(cons 1 ())
	(cons 1 '(2))
	(cons () '(2))
	(cons "a" '(1))
))

(run-all-tests)
