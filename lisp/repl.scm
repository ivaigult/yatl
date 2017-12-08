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

(forever
    (with-exception-handler
        repl-on-error
        repl-iter
    )
)

