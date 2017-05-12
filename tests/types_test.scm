(add-test "type-predicates-test" (lambda ()
	(assert (boolean? #t) "#t is boolean")
	(assert (symbol? 'sym) "sym is symbol")
	(assert (string? "str") "str is number")
	(assert (number? 1)    "1 is number")
	(assert (pair? '(1 2)) "(1 2) is pair")
))


(run-all-tests)