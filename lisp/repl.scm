(define std-in (create-console-input-port))
(define std-out (create-console-output-port))

(forever
    (define obj (input-port-read std-in))
    (print (eval obj))
)

