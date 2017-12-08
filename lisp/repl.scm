(define (repl)
    (define std-in (create-console-input-port))
    (define std-out (create-console-output-port))
    (define (repl-iter)
        (output-port-write-string std-out "yatl> ")
        (let* ((read-obj (input-port-read std-in))
               (eval-obj (eval read-obj)))
            (cond ((input-port-eof? std-in) (exit 0)))
            (output-port-write-string std-out "$     ")
            (output-port-write std-out eval-obj)
            (output-port-newline std-out)
        )
    )
    (define (repl-on-error err)
        (output-port-write std-out err)
        (output-port-newline std-out)
    )
    (define (repl-exec)
        (forever
            (with-exception-handler
                repl-on-error
	        repl-iter))
    )
    (repl-exec)
)

(define (load-file)
    (print "load-file")
)

(define (yatl-main)
    (define arg-counter 0)
    (define files-to-load '())

    (map (lambda (arg)
        (cond
            ((< 0 arg-counter) (set! files-to-load (cons arg files-to-load))))
        (set! arg-counter (+ arg-counter 1))
    ) yatl.argv)
    (cond
        ((not (equal? files-to-load '())) (load-file))
	(#t                               (repl)))
)

(yatl-main)
