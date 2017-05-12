(define (print-error e) 
	(print "An error occured:") 
	(print e)
)

(define (do-something-bad)
	(print "About to throw error")
	(error "Oh, error happend" 'some 'meta 'data)
	(print "This will not be print")
)

(with-exception-handler
	print-error
	do-something-bad
)

(with-exception-handler
	print-error
	(lambda () (error "Fuck!"))
)