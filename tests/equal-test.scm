(add-test "eq-test" (lambda ()
	(define foo "foo")
	(define bar foo)
	(define buz foo)
	;; (eq? a b) is true only when &a = &b
	(assert (eq? bar buz) "baz should be equal bar")

	(assert (not (eq? 1 1))     "Two different instances for 1 should be created")
	(assert (not (eq? "a" "a")) "Two different instances for 'a' should be created")
	;; And so on
))

(run-all-tests)