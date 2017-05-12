(add-test "type-predicates-test" (lambda ()
	(assert (boolean? #t) "#t is boolean")
	(assert (symbol? 'sym) "sym is symbol")
	(assert (string? "str") "str is number")
	(assert (number? 1)    "1 is number")
	(assert (pair? '(1 2)) "(1 2) is pair")
))

(add-test "type-predicates-test" (lambda ()
	;; Atomic types
	(assert (atom? 1)      "number is atom type")
	;; (assert (atom? "")    "string is atom type") FIXME!!!!!
	(assert (atom? "str")  "string is atom type")
	(assert (atom? 'sym )  "symbol is atom type")
	(assert (atom? '() )   "nil is atom")
	(assert (atom? #t)     "boolean is atom")
	;; Non-atomic types
	(assert (not (atom? '(1)))   "pair is not atom")
	(assert (not (atom? '(1 2))) "pair is not atom")
))

(run-all-tests)