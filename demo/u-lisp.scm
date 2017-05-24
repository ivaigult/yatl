;; Minimal lisp can be implemented in lisp using only:
;; `atom`, `car`, `cdr`, `cond`, `cons`, `eq` and `quote`

;; Let's define core functions:

(define (_null? x) (equal? x '()))

(define (_and x y)
  (cond (x (cond (y 't) ('t '())))
        ('t '())))

(define (_not x)
  (cond (x '())
        ('t 't)))

(define (_append x y)
  (cond ((_null x) y)
        ('t (cons (car x) (_append (cdr x) y)))))

(define (_list x y)
  (cons x (cons y '())))

(define (_pair x y)
  (cond ((_and (_null x) (_null y)) '())
        ((_and (_not (atom? x)) (_not (atom? y)))
         (cons (_list (car x) (car y))
               (_pair (cdr x) (cdr y))))))

(define (caar x) (car (car x)))
(define (cadr x) (car (cdr x)))
(define (cadar x) (car (cdr (car x))))
(define (caddr x) (car (cdr (cdr x))))

(define (_assoc x y)
  (cond ((equal? (caar y) x) (cadar y))
        ('t (_assoc x (cdr y)))))

;; Having `_eval` implemented, we can evaluate any S-expression

(define (_eval e a)
  (cond
    ((atom? e) (_assoc e a))
    ((atom? (car e))
     (cond
       ((equal? (car e) 'quote) (cadr e))
       ((equal? (car e) 'atom)  (atom?  (_eval (cadr e) a)))
       ((equal? (car e) 'eq)    (equal?    (_eval (cadr e)  a)
                                           (_eval (caddr e) a)))
       ((equal? (car e) 'car)   (car    (_eval (cadr e) a)))
       ((equal? (car e) 'cdr)   (cdr    (_eval (cadr e) a)))
       ((equal? (car e) 'cons)  (cons   (_eval (cadr e) a)
                                     (_eval (caddr e) a)))
       ((equal? (car e) 'cond)  (_evcon (cdr e) a))
       ('t (_eval (cons (_assoc (car e) a)
                        (cdr e))
                  a))))
    ((equal? (caar e) 'label)
     (_eval (cons (caddar e) (cdr e))
            (cons (_list (cadar e) (car e)) a)))
    ((equal? (caar e) 'lambda)
     (_eval (caddar e)
            (_append (_pair (cadar e) (_evlis (cdr e) a))
                     a)))))


(define (_evcon c a)
  (cond ( (equal? (_eval (caar c) a) '()) (_evcon (cdr c)   a))
        ('t                               (_eval  (cadar c) a))))

(define (_evlis m a)
  (cond ((_null m) '())
        ('t (cons (_eval  (car m) a)
                  (_evlis (cdr m) a)))))

;; Let's try something out
;; First, we should define a variable for bindings keeping
(define env '())

;; Core functions should work
(_eval ''some-symbol env)
(_eval '(car '(a b c)) env)
(_eval '(cdr '(a b c)) env)
(_eval '(cons 'a '(b c)) env)

;; Conditionals work as well despite the fact that `true` is defined as 't
;; While `false` is defined as `()`
(_eval '(cond ('() 'false) ('t 'true) ) env)