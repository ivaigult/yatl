(define std-in (create-console-input-port))
(define std-out (create-console-output-port))

(forever
    (output-port-write-string std-out "yatl> ")
    (let* ((read-obj (input-port-read std-in))
	   (eval-obj (eval read-obj)))
        (output-port-write-string std-out "$     ")
        (output-port-write std-out eval-obj)
        (output-port-newline std-out)
    )
)

