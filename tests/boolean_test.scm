(add-test "boolean-parse" (lambda ()
	(assert #t "True is true")
	(assert (not #f) "False is false")
))


(run-all-tests)